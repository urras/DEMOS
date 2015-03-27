/*
 * 2-� ���� ���������� �������
 *
 *  $Log:	boot.c,v $
 * Revision 1.17  90/12/12  17:15:25  korotaev
 * ������ ��� ���.
 * 
 * Revision 1.16  90/11/12  19:25:35  root
 * ����� ���� ��� ��1425 � ����������� include.
 * 
 * Revision 1.15  89/05/03  17:34:06  korotaev
 * �������� label'� �� CHECKSUM.
 * 
 * Revision 1.14  89/05/03  14:01:54  korotaev
 * New label.
 * 
 * Revision 1.13  89/04/11  17:17:02  korotaev
 * ������ ��������� ����� ������� ���� � ������� ���������
 * 8 �� � 16 ��. ��� ������� ��� �������������. ������ ������
 * ������� ������� 16 ��.
 * 
 * Revision 1.12  88/09/26  19:07:54  avg
 * ������ ���� � �-�� ��� �������� ����, �� � ���������� ������������ �����
 * �������������� ������ �-��, � �������� � ������� ���� ����.
 *
 * Revision 1.11  88/06/23  21:36:30  ache
 * + 31 ������� � ����
 *
 * Revision 1.10  88/06/13  15:55:38  avg
 * ������ ����� editboot ��� ��������� ����������� ���� %?
 *
 * Revision 1.9  87/09/29  15:41:58  avg
 * ������ ���� ���� ��������� ���� ���������� ��� ��������
 * ���� � ���������� cc ��������� -DMY_DEFLINE='"dd(min,maj)name"'
 * ��� (��� �-85) MY_DEF85.
 *
 * Revision 1.8  87/05/17  18:29:00  avg
 * ������� ��������� �����: ������ && ����� &.
 *
 * Revision 1.7  87/04/24  09:40:42  dmitry
 * ���� �� ��������� �������� � ������� �� ������ ��������,
 * ����� ������� �� ���������.
 *
 * Revision 1.6  87/03/03  22:24:55  avg
 * ��������� ������ � defname ��� �����������-85.
 *
 * Revision 1.5  86/12/13  19:31:08  alex
 * ��� ������ � ������� �������� read �� reada, ���������� ����� getc.
 *
 * Revision 1.4  86/12/03  22:03:22  alex
 * ���������� ������ ������ � �������� �������������.
 *
 * Revision 1.3  86/11/27  23:02:17  alex
 * ������� ��������� ��������� �� ������������
 * � ��������� ����
 *
 * Revision 1.2  86/11/24  23:32:58  alex
 * rcsid
 *
 * Revision 1.1  86/07/14  20:14:09  avg
 * Initial revision
 *
 */
/* #define AUTOBOOT */
#define P(x)    /* printf(x) */
char rcsid[]="$Header: boot.c,v 1.17 90/12/12 17:15:25 korotaev Exp $";
#include        <sys/param.h>
#include        <sys/seg.h>
#include        <sys/ino.h>
#include        <sys/inode.h>
#include        <sys/filsys.h>
#include        <sys/dir.h>
#include        <sys/reboot.h>
#include        "../../include/koverlay.h"
#include        <a.out.h>

#ifndef TEST
#include        "../saio.h"
#else
int segflag;
#define setsep() /* */
#define setnosep() /* */
#endif

#define RB_MAGIC 005043 /* #\n - ������� - ������ ��� boot */

#undef  btoc
#define KB      * 1024L

#undef  KISA0
#undef  KDSD0

#define KISD0   ((u_short *) 0172300)
#define KISD2   ((u_short *) 0172304)
#define KISD3   ((u_short *) 0172306)
#define KDSD0   ((u_short *) 0172320)
#define KISA0   ((u_short *) 0172340)
#define KISA2   ((u_short *) 0172344)
#define KISA3   ((u_short *) 0172346)
#define KDSA0   ((u_short *) 0172360)

#define SEG_DATA        01
#define SEG_TEXT        02
#define SEG_OVLY        04

extern  int     cputype;
extern  bool_t  ksep;                   /* �������� ���������� I/D ���� */
extern  bool_t  sep_id;                 /* ��������� ������������ ���������� I/D */
extern  int     bootopts, bootdev, checkword, rbootdev;
char    module[]        = "Boot";       /* ��� ���� ��������� (������������ trap-��) */
char    line[100];
char    defline[100]    = RB_DEFNAME;
bool_t  overlaid        = 0;
bool_t  xoverlaid       = 0;
u_short pdrproto[16 + NXOVL]     = {0};
struct  exec    exec;
struct  xovlhdr ovlhdr;
unsigned        btoc();

short   oldkernel = 0;  /* ���� ������ ������������� �������� (base<=8K) */

struct  loadmap {
	int     seg_type;
	long    seg_len;
};

struct  loadtable       {
	short   lt_flag;
	short   lt_magic;
	struct  loadmap *lt_map;
};

struct  loadmap load407[]       =       {
	  SEG_DATA,       56 KB,
	  0,              0  KB
};
struct  loadmap load410[]       =       {
	  SEG_TEXT,       48 KB,
	  SEG_DATA,       56 KB,
	  0,              0  KB
};
struct  loadmap load411[]       =       {
	  SEG_DATA,       56 KB,
	  SEG_TEXT,       64 KB,
	  0,              0  KB
};
struct  loadmap load430[]       =       {
	  SEG_TEXT,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_DATA,       32 KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  0,              0  KB
};
struct  loadmap load444[]       =       {
	  SEG_TEXT,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_DATA,       32 KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  0,              0  KB
};
struct  loadmap load431[]       =       {
	  SEG_DATA,       56 KB,        /* minumum, 48 KB + 1 */
	  SEG_TEXT,       56 KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  SEG_OVLY,       8  KB,
	  0,              0  KB
};

struct  loadtable       loadtable[]     =       {
	0, A_MAGIC1,       load407,
	0, A_MAGIC2,       load410,
	0, A_MAGIC3,       load411,
	1, A_MAGIC5,       load430,
	0, A_MAGIC6,       load431,
	1, A_MAGIC7,       load444
};

int fileondev = -1; /* opened file on load device -- if positive */

main ()
{
	extern video;
	int i, j;
	int retry       = 0;
	int cmdfile = 0;
	struct  loadtable       *setup (), *ko;
	static char wstr[] = "Boot (%s) co-operative demos/*\n";
	register char *a;

/*
 * ������� � �������� ����������� �����.
 * ����������� ����� ����������� �����.
 */
	for( i = 0, a = wstr ; *a != '\0' ; a++ )
		i += *a;
#define CHECKSUM 2598
	if( i != CHECKSUM )
		goto loop;
	segflag = 3;                    /* ��a����� �-� ����� ���� ���� */
	if( video && cputype == 23 )    /* �����������-85 */
		printf("\n\n\n\n\n\n");
	printf ("\nCPU: %d\n", cputype);
	rcsid[18+13] = '\0';
	printf (wstr,rcsid+18);
	rcsid[18+13] = ' ';
	if( (rbootdev>>8) == 3 || (rbootdev>>8) == 9 ) {
		for( i = 0 ; defline[i] = (RB_DEFTAPE)[i] ; i++ );
		printf("<from tape>\n");
	}
	strcpy(line, defline);

	/*
	 * ��������� �� ���������� ��������� ������ ��������
	 * � ������ ������������.
	 * ���� ������� r2 (checkword) �������� ���������� � bootopts,
	 * ���������� ������������, ����� �������� ������ ���
	 * �������.
	 */

#ifdef  AUTOBOOT
	if ((bootopts & 0177600) | (checkword != ~bootopts)) {
		printf ("To abort autoboot type ^C ");
		if (autoboot())
			bootopts = RB_SINGLE;
		else
			bootopts = RB_SINGLE | RB_ASKNAME;
	printf("\r                          \n");
	}
#else
	if (checkword != ~bootopts)
		bootopts = RB_SINGLE | RB_ASKNAME;
#endif  AUTOBOOT
	j = -1;
	i = -1;
	initpar();
	do {
		if ( i >= 0 ) close(i);
		i = -1;
		if ( ++retry > 3 ) {
			retry = 0;
			bootopts        = RB_SINGLE | RB_ASKNAME;
		}
		if ( (bootopts & RB_ASKNAME) && !cmdfile ) {
			extern int boottmout ;
			static wuztmout = 1 ;

			initpar();
			printf(": ");
			boottmout = wuztmout ;
			fileondev = -1;
			gets(line);
			if( boottmout == 1 ) {
				boottmout = wuztmout = bootopts = 0 ;
			}
			if ( line[0] == 0 ) {
				strcpy(line, defline);
				printf("> %s\n",line);
			}
		} else
			printf("> %s\n",line);
		switch( editboot(line) ) {
		    case 0:
			continue;
		    case 1:
			printf("> %s\n", line);
		}
		cmdfile = 0;
		if ( !getpar(line) || (i = open(line,0)) <0 )
		    continue;
		fileondev = i;
		ko = setup(i);
		if ( !ko ) continue;
		if ( ko == (struct loadtable *)-1) {
			getl(i,line);
			cmdfile = 1;
			continue;
		}
		j = checkunix(i,ko);
		P("exit checkunix\n");
	}
	while ( j < 0 );
	close(i);
	i= -1;
	return;
loop:
	while( 1 ) ;
}

struct  loadtable       *
setup (io)
register io;
{
	register        i;

	exec.a_magic = getw(io);
	if ( exec.a_magic == RB_MAGIC ) return((struct loadtable *)-1);
	reada(io,&exec.a_text, (sizeof exec)-(sizeof exec.a_magic));
	/*
	 * ���� ���������� ����, ��������� ��������� �������.
	 */
	if (exec.a_magic == A_MAGIC5 || exec.a_magic == A_MAGIC6 ||
	    exec.a_magic == A_MAGIC7) {
		overlaid++;
		xoverlaid = (exec.a_magic == A_MAGIC7);
		reada(io,&ovlhdr, xoverlaid ?
			 sizeof (struct xovlhdr) : sizeof (struct ovlhdr));
		}
	for (i = 0; i < sizeof (loadtable) / sizeof (struct loadtable); i++)
		if (loadtable[i].lt_magic == exec.a_magic){
		       /*
			* �.�. ���� ��������� ������� ������� �� ��, ��� ��
			* ������ 8 �� � �������������� ��������� �������
			* ������� � �������� ������� ������.
			*/
			if( loadtable[i].lt_flag && exec.a_text > (8 KB) ){
				register struct loadmap *pl;

				for( pl = loadtable[i].lt_map ; pl->seg_type ; pl++ )
					if( pl->seg_type == SEG_TEXT )
						pl->seg_len += (8 KB);
					else if( pl->seg_type == SEG_DATA )
						pl->seg_len -= (8 KB);
				oldkernel++;
			}
			return (&loadtable[i]);
		}
	printf ("Bad magic number 0%o\n", exec.a_magic);
	return ((struct loadtable *) NULL);
}


checkunix (io, lt)
struct  loadtable       *lt;
{
	char    *segname;
	register        ovseg, segtype;
	register unsigned               seglen;
	struct  loadmap *lm     = lt->lt_map;

	if (lt == (struct loadtable *) NULL)
		return (-1);

	/*
	 * ��������� � ���������� ���������� ���������� ����� I/D.
	 */
	if (exec.a_magic == A_MAGIC3 || exec.a_magic == A_MAGIC6)
		if (!sep_id)    {
			printf ("Cannot load separate I & D object files\n");
			return (-1);
			}
		else
			setsep ();
	else
		if (sep_id)
			setnosep ();

	/*
	 * ��������� ������� ������� ��������.
	 */
	ovseg   = 0;
	while (segtype = lm->seg_type)  {
		switch (segtype)        {
			case SEG_TEXT:
				/*
				 * ��������� ������ ����������
				 * �������� �� ����. ��������.
				 */
				if (exec.a_magic == A_MAGIC2)
					seglen  = ctob (stoc (ctos (btoc (exec.a_text))));
				else
					seglen  = exec.a_text;
				segname = "Text";
				break;

			case SEG_DATA:
				seglen  = exec.a_data + exec.a_bss;
				segname = "Data";
				if (exec.a_magic == A_MAGIC1)
					seglen  += exec.a_text;
				else
					/*
					 *      Force a complaint if the file
					 *      won't fit.  It's here instead
					 *      of in the SEG_TEXT case above
					 *      because it's more likely to be
					 *      a data overflow problem.
					 */
					if (exec.a_magic == A_MAGIC2)
						seglen  += ctob (stoc (ctos (btoc (exec.a_text))));
				break;

			case SEG_OVLY:
				seglen  = ovlhdr.xov_siz[ovseg];
				segname = "Overlay";
				ovseg++;
				break;

			default:
				/*
				 *      ��� ``�� ����� ����.''
				 */
				printf ("Unknown segment type in load table:  %d\n", segtype);
				return (-1);
				/*NOTREACHED*/
			}

		seglen  = ctob (btoc (seglen));
		if (((long) seglen) > lm->seg_len) {
			if (segtype == SEG_OVLY)
				printf ("%s %d too large by %D bytes", segname, ovseg, lm->seg_len - ((long) seglen));
			else
				printf ("%s too large by %D bytes", segname, lm->seg_len - ((long) seglen));
			return (-1);
		}
		if (segtype == SEG_TEXT)
		    switch (exec.a_magic) {
			case A_MAGIC7:
			case A_MAGIC5:
			    if( oldkernel && seglen <= (8 KB) ) {
				 printf("Base segment too small, 8K minimum\n");
				 return(-1);
			    }
			    break;
			case A_MAGIC6:
			    if (seglen <= 48 KB) {
				printf("Base segment too small, 48K minimum\n");
				return(-1);
			    }
			    break;
			default:
			    break;
		}

		lm++;
		}

	if( !copyunix (io, lt)) return(-1);;
	setregs (lt);
	return (0);
}

copyunix (io, lt)
register        io;
struct  loadtable       *lt;
{
	int     i; int ko = -1;
	bool_t  donedata = 0;
	register        addr;
	register unsigned       seglen;
	off_t   segoff;
	int     segtype;
	int     nseg, phys,physn, ovseg;
	struct  loadmap *lm     = lt->lt_map;
	/*
	 * ��������� �������� � ���������� ����������� ����� ���.
	 */
	nseg    = 0;
	physn   = phys    = 0;
	ovseg   = 0;
	lm      = lt->lt_map;
	while (segtype = lm++->seg_type)        {

		segoff  = (off_t) N_TXTOFF(exec);

		switch (segtype)        {
			case SEG_TEXT:
				seglen  = exec.a_text;
				break;

			case SEG_DATA:
				seglen  = exec.a_data;
				if (exec.a_magic != A_MAGIC1)   {
					segoff  += (off_t) exec.a_text;
					if (overlaid)
						for (i = 0; i < (xoverlaid?NXOVL:NOVL); i++)
							segoff  += (off_t) ovlhdr.xov_siz[i];
					}
				else
					seglen  += exec.a_text;
				donedata++;
				break;

			case SEG_OVLY:
				seglen  = ovlhdr.xov_siz[ovseg];
				segoff  += exec.a_text;
				for (i = 0; i < ovseg; i++)
					segoff  += (off_t) ovlhdr.xov_siz[i];
				ovseg++;
				break;

			}

		if (!seglen)
			continue;
		setseg (phys);
		if (exec.a_magic != A_MAGIC1)
			(void) lseek (io, segoff, 0);
		for (addr = 0; addr < seglen; addr += 2)
			mtpi (getw (io), addr);

		if (segtype == SEG_DATA)        {
			physn = phys;
			clrseg (addr, exec.a_bss);
			seglen  += exec.a_bss;
			}

		pdrproto[nseg++]        = btoc (seglen);
		if (!donedata)
			seglen  = ctob (stoc (ctos (btoc (seglen))));
		phys    += btoc (seglen);
		}
		setseg (physn);
		ko = nastroi(io);
	P("exit copyunix\n");
	return( ko );
}

/*
 * ���������� ���������� �������� �������� �������.
 */
setregs (lt)
struct  loadtable       *lt;
{
#ifdef TEST
	return;
}
#else
	register        i;
	register u_short        *par_base, *pdr_base;
	bool_t  donedata = 0;
	int     phys, segtype;
	int     nseg, ntextpgs, novlypgs, npages, pagelen;
	struct  loadmap *lm     = lt->lt_map;
	P("start setregs\n");
	nseg    = 0;
	phys    = 0;
	ntextpgs        = 0;
	novlypgs        = 0;

	setseg (0);
	if (exec.a_magic == A_MAGIC1)
		return;

	/*
	 * ������ �����, ������� ������ �� �����, ����� �������� �/�.
	 */
	par_base        = KISA0;
	pdr_base        = KISD0;
	for (i = 0; i < (ksep ?  8 : 7); i++)   {
		*par_base++     = 0;
		*pdr_base++     = NOACC;
		}
	if (ksep)       {
		par_base        = KDSA0;
		pdr_base        = KDSD0;
		for (i = 0; i < 7; i++) {
			*par_base++     = 0;
			*pdr_base++     = NOACC;
			}
		}

	if (overlaid)   {
		/*
		 * ����� �������� �������� ������� ����������� ��������.
		 * N.B.:  �������������� ���
		 *        ��� ������� ����������� � ������ 8K
		 *        ����������� ������ ���� � �����. ��������
		 *        ����������� �� ���. ������ 0.
		 */
		if (ksep)
			*KDSD0  =       ((128 -1) << 8) | RW;
		else
			*KISD0  =       ((128 -1) << 8) | RW;
		par_base        = &(((u_short *) OVLY_TABLE_BASE)[0]);
		pdr_base        = &(((u_short *) OVLY_TABLE_BASE)[1 + (xoverlaid?NXOVL:NOVL)]);
		for (i = 0; i < (xoverlaid?NXOVL:NOVL); i++)      {
			mtpd (0, par_base++);
			mtpd (NOACC, pdr_base++);
			}
		}

	/*
	 * ���������� ��� ��������� ��������.
	 */
	lm      = lt->lt_map;
	while (segtype = lm++->seg_type)        {
		if (!(npages = ctos (pdrproto[nseg])))
			continue;

		switch (segtype)        {
			case SEG_TEXT:
				/*
				 * ����� ������ ���������� � KI0;
				 */
				par_base        = KISA0;
				pdr_base        = KISD0;
				ntextpgs        += npages;
				break;

			case SEG_DATA:
				if (overlaid)
					if (ksep)       {
						par_base        = I_DATA_PAR_BASE;
						pdr_base        = I_DATA_PDR_BASE;
						}
					else {
		/*
		 * ���� ���� �� ������ ����������� �� ��������,
		 * �� ������ ������ ���������� � 3-��� �����.
		 * ����� ��� ���������� �� ������� �����.
		 * (��. <sys/koverlay.h>)
		 */
						if( oldkernel ){
							par_base        = ON_DATA_PAR_BASE;
							pdr_base        = ON_DATA_PDR_BASE;
						} else {
							par_base        = N_DATA_PAR_BASE;
							pdr_base        = N_DATA_PDR_BASE;
						}
					}
				else
					if (ksep)       {
						par_base        = KDSA0;
						pdr_base        = KDSD0;
						}
					else
						{
						par_base        = &KISA0[ntextpgs + novlypgs];
						pdr_base        = &KISD0[ntextpgs + novlypgs];
						}
				donedata++;
				break;

			case SEG_OVLY:
				par_base        = &(((u_short *) OVLY_TABLE_BASE)[1 + novlypgs]);
				pdr_base        = &(((u_short *) OVLY_TABLE_BASE)
				  [1 + (xoverlaid?NXOVL:NOVL) + 1 + novlypgs]);
				novlypgs        += npages;
				break;

			}

		for (i = 0; i < npages; i++)    {
			pagelen = MIN (btoc ((int) (8 KB)), pdrproto[nseg]);
			if (segtype == SEG_OVLY)        {
				mtpd (phys, par_base);
				mtpd (((pagelen - 1) << 8) | RO, pdr_base);
				}
			else
				{
				*par_base       = phys;
				if (segtype == SEG_TEXT)
					if (ksep)
						*pdr_base       = ((pagelen - 1) << 8) | RO;
					else
						/*
						 * ������������� ����
						 * ������ ���-������ ���������� �
						 * ���� 0.
						 */
						if (i == 0)
							*pdr_base       = ((pagelen - 1) << 8) | RW;
						else
							*pdr_base       = ((pagelen - 1) << 8) | RO;
				else
					*pdr_base       = ((pagelen - 1) << 8) | RW;
				}
			par_base++, pdr_base++;
			if (donedata)
				phys    += pagelen;
			else
				phys    += stoc (ctos (pagelen));
			pdrproto[nseg]  -= pagelen;
			}

		nseg++;
		}

	/*
	 * ���������� phys �������� (������) ����� ������
	 * ��������� ������. ���� ���������� K[ID]6 ���
	 * systrap � ��������� ������ ������ ����� �� ������ 0140000.
	 */
	if (ksep)       {
		KDSA0[6]        = phys;
		KDSD0[6]        = (stoc(1) - 1) << 8 | RW;
		}
	else
		{
		KISA0[6]        = phys;
		KISD0[6]        = (stoc(1) - 1) << 8 | RW;
		}

	if (overlaid)
		mtpd (phys, &(((u_short *) OVLY_TABLE_BASE)
			    [2 + 2*(xoverlaid?NXOVL:NOVL)]));
		P("exit setregs\n");
}
#endif

unsigned
btoc (nclicks)
unsigned nclicks;
{
	return ((unsigned) (((((long) nclicks) + ((long) 63)) >> 6)));
}

getl(io,buf)
register char *buf;
{
	register int i;
	while ( (i=getc(io)) >= 040 && i <= 0177 )
		*buf++ = i;
		*buf = 0;
}

reada(fd,buf,count)
register char *buf;
register fd,count;
{
 while ( count--) *buf++ = getc(fd);
}

clrseg(addr, leng)
register unsigned addr;
register unsigned leng;
{
	while (leng) {
		leng -= 2;
		mtpi(0, addr+leng);
	}
}
