/*
 * $Log:	machdep.c,v $
 * Revision 22.8  90/12/12  16:23:00  korotaev
 * ������ ��� ���
 * 
 * Revision 22.7  90/11/12  19:11:27  root
 * ����� ���� ��� ��1425 � ����������� include.
 * 
 * Revision 22.6  89/10/31  13:26:38  korotaev
 * �������� ������ ��� ������ 4 �� ������ (����� ����������
 * ���� unsigned ��������� � 0 � �������� ������ � ������).
 * 
 * Revision 22.5  89/07/06  14:32:48  avg
 * �������� ������� syncing disks...
 * 
 * Revision 22.4  89/05/19  17:58:28  avg
 * ������ syncing discs... ���� �� ������� ����������� �������, � �� ��
 * ������� �.
 *
 * Revision 22.3  89/04/27  14:15:23  korotaev
 * ��������� � ����� � ������������� ��������� � ������
 *
 * Revision 22.2  89/04/12  15:23:03  korotaev
 * "param.h" ==> <sys/param.h>
 *
 * Revision 22.0  89/03/30  14:01:54  korotaev
 * Begining DEMOS 2.2.
 *
 * Revision 1.12  89/01/26  15:37:11  korotaev
 * ��������� ��������� �����������.
 *
 * Revision 1.11  89/01/07  20:42:05  korotaev
 * ������� � ������������� ������ FIFO, +fcntl, + ������ ������ ��������
 *
 * (from VAX/UTS SV.2)
 *
 * Revision 1.10  89/01/07  16:56:09  korotaev
 * *** empty log message ***
 *
 * Revision 1.9  88/10/06  13:15:45  dvolodin
 * ������������� ������� �-85 ������� ��� #if PDP11 == 23.
 *
 * Revision 1.8  88/08/30  13:48:31  korotaev
 * ��������� ��������� ����������� �������� ����������.
 *
 * Revision 1.7  88/03/23  13:34:03  korotaev
 * ��������� ����� ������� � ����, �-85 � ���������� �������������
 *
 * Revision 1.6  88/01/23  17:44:10  korotaev
 * ������� � ������� AVG-�-85.
 *
 * Revision 1.5  88/01/04  19:29:33  korotaev
 * ���������, ��������� �� �������� ���� �� ���� � ���.
 * ��������� ������� � �������� � ������������ ra � dhu.
 *
 * Revision 1.4  87/09/23  16:02:21  root
 * ��������� ��������� � truncate
 *
 * Revision 1.3  87/04/18  13:30:31  avg
 * ������� ������ ��� EXMOUNT.
 *
 * Revision 1.2  87/03/12  17:46:55  alex
 * ���������� �������� � boot - delay (�� ������� �� �������� ���).
 *
 * Revision 1.1  86/04/19  15:50:12  avg
 * Initial revision
 *
 */

/*NOXSTR*/

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/acct.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/inode.h>
#include <sys/proc.h>
#include <sys/seg.h>
#include "../include/map.h"
#include "../include/reg.h"
#include "../include/buf.h"
#include "../include/psw.h"
#include <sys/tty.h>
#include <sys/reboot.h>
#include "../include/uba.h"
#include "../include/iopage.h"
#include <sys/fcntl.h>

extern  memaddr bpaddr;

#ifndef NOKA5
segm    seg5;                   /* filled in by initialization */
#endif

#ifdef  UCB_CLIST
extern  memaddr clststrt;
#else
extern  struct  cblock  cfree[];
#endif  UCB_CLIST
extern  ubadr_t clstaddr;
#ifdef  UCB_NET
extern  memaddr mbbase;
extern  int mbsize;
#endif

int     bootflags, checkword;           /* saved from r4, r2 by mch.s */
int     spcnum = -1;

/*
 * Icode is the octal bootstrap
 * program executed in user mode
 * to bring up the system.
 */
int     icode[] = {
     0104413,        /* sys exec; init; initp */
     0000020,
     0000010,
     0104401,        /* sys exit */
     0000020,        /* initp: init; bootopts; spc; 0 */
     0000032,
     0000034,
     0000000,
     0062457,        /* init: </etc/init\0> */
     0061564,
     0064457,
     0064556,
     0000164,
     RB_SINGLE,      /* bootopts:  RB_SINGLE */
     0000000,        /* spc:  0 */
     0000000
#define ICODE_OPTS      13      /* location of bootopts in icode */
#define ICODE_SPC       14
};
int     szicode = sizeof (icode);

size_t  physmem;        /* total amount of physical memory (for savecore) */

/*YESXSTR*/

/*
 * Machine dependent startup code
 */
startup()
{
	register memaddr i, freebase;
	extern  end;

#ifndef NOKA5
	saveseg5(seg5);         /* must be done before clear() is called */
	if (&remap_area > SEG5)
		panic("&remap_area>0120000");
	if (&end > 0140000)
		panic("_end>0140000");
#else
	if (&end > SEG5)
		panic("_end > 0120000");
#endif

	/*
	 * zero and free all of core
	 */
	i = freebase = *ka6 + USIZE;
	UISD[0] = ((stoc(1) - 1) << 8) | RW;
	for (;i;) {
		UISA[0] = i;
		if (fuibyte((caddr_t)0) < 0)
			break;
		maxmem++;
		i++;
		/*
		 * avoid testing locations on the IO page if possible,
		 * since some people have dz's at 0160000 (0760000).
		 * Note that more than 248K of memory is not currently
		 * supported without a Unibus map anyway.(av q22bus)
		 * (3968 is btoc(248K); the macro doesn't do longs.)
		 */
#if PDP11 != 23
		if (!ubmap && !q22bus && i >= 3968)
			break;
#endif
	}
	clear(freebase, i - freebase);
	mfree(coremap, i - freebase, freebase);
	physmem = i;
#define B  (size_t)(((long)nbuf * (bsize)) / ctob(1))
	if ((bpaddr = malloc(coremap, B)) == 0)
		panic("bufs");
	maxmem -= B;
#undef  B

#ifdef  UCB_CLIST
#define C       (nclist * sizeof(struct cblock))
	if ((clststrt = malloc(coremap, btoc(C))) == 0)
		panic("clist");
	maxmem -= btoc(C);
	clstaddr = ((ubadr_t) clststrt) << 6;
#undef  C
#else
	clstaddr = (ubadr_t) &cfree;
#endif  UCB_CLIST

#ifdef PROFILE
	maxmem -= msprof();
#endif

#ifdef  UCB_NET
	if ((mbbase = malloc(coremap, btoc(mbsize))) == 0)
		panic("mbbase");
	maxmem -= btoc(mbsize);
#endif

	maxmem -= stfrflck();

	printf("mem=%D\n", ctob((long)maxmem));
	if (MAXMEM < maxmem)
		maxmem = MAXMEM;
	mfree(swapmap, nswap, 1);
	swplo--;

	UISA[7] = ka6[1]; /* io segment */
	UISD[7] = ((stoc(1) - 1) << 8) | RW;
	if (checkword == ~bootflags){
		icode[ICODE_OPTS] = bootflags;
		if( bootflags & RB_FROMTAPE )
			icode[ICODE_SPC] = spcnum;
	}
}

#ifdef PROFILE
/*
 *  Allocate memory for system profiling.  Called
 *  once at boot time.  Returns number of clicks
 *  used by profiling.
 *
 *  The system profiler uses supervisor I space registers 2 and 3
 *  (virtual addresses 040000 through 0100000) to hold the profile.
 */

static int nproclicks;
memaddr proloc;

msprof()
{
	nproclicks = btoc(8192*2);
	proloc = malloc(coremap, nproclicks);
	if (proloc == 0)
		panic("msprof");

	*SISA2 = proloc;
	*SISA3 = proloc + btoc(8192);
	*SISD2 = 077400|RW;
	*SISD3 = 077400|RW;
	*SISD0 = RW;
	*SISD1 = RW;

	esprof();
	return (nproclicks);
}

/*
 *  Enable system profiling.
 *  Zero out the profile buffer and then turn the
 *  clock (KW11-P) on.
 */
esprof()
{
	clear(proloc, nproclicks);
	isprof();
	printf("profiling on\n");
}
#endif

#ifdef  UNIBUS_MAP
/*
 * Re-initialize the Unibus map registers to statically map
 * the clists and buffers.  Free the remaining registers for
 * physical I/O.
 */
void
ubinit()
{
	long    paddr;
	register i, ub_nreg;
	extern  struct map ub_map[];
#ifdef  UCB_NET
	extern  int ub_inited;
#endif

	if (!ubmap)
		return;
#ifdef  UCB_NET
	ub_inited++;
#endif
	/*
	 * Clists start at UNIBUS virtual address 0.  The size of
	 * the clist segment can be no larger than UBPAGE bytes.
	 * Clstaddt was the physical address of clists.
	 */
	if (nclist * sizeof (struct cblock) > ctob(stoc(1)))
		panic("clist>UBPAGE");
	setubregno(0, clstaddr);
	clstaddr = (ubadr_t) 0;

	/*
	 * Buffers start at UNIBUS virtual address BUF_UBADDR.
	 */
	paddr = ((long) bpaddr) << 6;
	ub_nreg = nubreg(nbuf, bsize);
	for (i = BUF_UBADDR / UBPAGE; i < ub_nreg + (BUF_UBADDR/UBPAGE); i++) {
		setubregno(i, paddr);
		paddr += (long) UBPAGE;
	}
	mfree(ub_map, 31 - ub_nreg - 1, 1 + ub_nreg);
}
#endif  UNIBUS_MAP

#include "h/video.h"

/*
 * set up a physical address
 * into users virtual address space.
 */
sysphys()
{
	int d, j;
	register i, s;
	register struct a {
		int     segno;
		int     size;
		int     phys;
	} *uap;

	uap = (struct a *)u.u_ap;
#if NVI > 0               /* Chunk for E-85 video RAM */
#define U(x) (unsigned)(x)
	if( U(uap->phys) >= U(0160000) &&
	    U(uap->phys + uap->size) < U(0170000) ) {
	    ;
	} else
#endif NVI
	if (!suser())
		return;
	i = uap->segno;
	if( i < 0 || (j = i+(uap->size / 128)) >= 8 )
		goto bad;
	for( j-- ; j>=i ; j-- ) {
#ifdef  NONSEPARATE
		d = u.u_uisd[j];
#else
		d = u.u_uisd[j + 8];
#endif  NONSEPARATE
		if (d != 0 && (d & ABS) == 0)
			goto bad;
	}
Looping:
	s = uap->size;
	if (s < 0)
		goto bad;
	if(s > 128)
		s = 128;
#ifdef  NONSEPARATE
	u.u_uisd[i] = 0;
	u.u_uisa[i] = 0;
#else
	u.u_uisd[i + 8] = 0;
	u.u_uisa[i + 8] = 0;
	if (!u.u_sep) {
		u.u_uisd[i] = 0;
		u.u_uisa[i] = 0;
	}
#endif  NONSEPARATE
	if (s) {
#ifdef  NONSEPARATE
		u.u_uisd[i] = ((s - 1) << 8) | RW | ABS;
		u.u_uisa[i] = uap->phys;
#else
		u.u_uisd[i + 8] = ((s - 1) << 8) | RW | ABS;
		u.u_uisa[i + 8] = uap->phys;
		if (!u.u_sep) {
			u.u_uisa[i] = u.u_uisa[i + 8];
			u.u_uisd[i] = u.u_uisd[i + 8];
		}
#endif  NONSEPARATE
	}
	if( uap->size > 128 ) {
		uap->size -= 128;
		uap->phys += 128;
		i++;
		goto Looping;
	}
	sureg();
	return;

bad:
	u.u_error = EINVAL;
}

/*
 * Determine whether clock is attached. If so, start it.
 */
#if PDP11 == 23
int isclock;            /* ���� ��� ����� ����� */
#endif
clkstart()
{
	register junk;

	lks = LKS;
	if (fioword((caddr_t)lks) == -1) {
		lks = KW11P_CSR;
		if (fioword((caddr_t)lks) == -1) {
#if PDP11 == 23 && !defined(DVK4)
			if(fioword((caddr_t)TIM85_A) != -1) {
#                               define rB(x)  *((char *)x)
#                               define rW(x)  *((int  *)x)
				rW(TIM85_A) = 052;      /* 64 HZ */
				rW(TIM85_B) = 0100;     /* ENABLE */

				/* set fixed priorities */
				rB(I85_CR1) = 0200;
				rB(I85_CR2) = 0200;
				rB(I85_CR3) = 0200;

				/* enable timer interrupts */
				rB(I85_CR1) = 037;

				junk = rB(TIM85_C);

#                               undef rB
#                               undef rW
			} else
#endif
			lks = 0;
		}
	}
	if (lks)                        /*���� ������� ����,�� ���������*/
		*lks = 0115;            /*����������, ���� ���-�� ����..*/
#ifdef DVK4                             /*��������� ������� !!!*/
	isclock++;                      /* �������� ������ � ������� */
	_spl5 ();                       /* ������ ��� ������ ��������� */
	for (junk = 06000; --junk; );   /* ��������� ����-���� islock */
	for (junk=0; isclock; )         /* ������ ���� ��������� ����� */
		if (junk == 0) {        /* ���� ��� ��������... */
			printf ("\7\nPlease, switch on the Clock ...");
			junk++;
		}
	if (junk)
		printf (" OK\n");
#endif
}

/*
 * Fetch a word from an address on the I/O page,
 * returning -1 if address does not exist.
 */
fioword(addr)
{
	register val, saveUI7, saveUD7;

	saveUI7 = UISA[7];
	saveUD7 = UISD[7];
	UISA[7] = ka6[1]; /* io segment */
	UISD[7] = ((stoc(1) - 1) << 8) | RW;
	val = fuiword(addr);
	UISA[7] = saveUI7;
	UISD[7] = saveUD7;
	return(val);
}

/*
 * Let a process handle a signal by simulating an interrupt
 */
sendsig(p)
caddr_t p;
{
	register unsigned n;

	n = u.u_ar0[R6] - 4;
	if (n >= -ctob(u.u_ssize) || grow(n)) {
		suword((caddr_t)n + 2, u.u_ar0[RPS]);
		suword((caddr_t)n, u.u_ar0[R7]);
		u.u_ar0[R6] = n;
		u.u_ar0[RPS] &= ~PS_T;
		u.u_ar0[R7] = (int)p;
	} else {
		/*
		 * Process has trashed its stack.
		 * Blow him away.
		 */
		u.u_signal[SIGSEGV] = SIG_DFL;
		u.u_procp->p_cursig = SIGSEGV;
		psig();
	}
}

/*
 * Simulate a return from interrupt on return from the syscall.
 * after popping n words of the users stack.
 */
dorti(n)
{
	register int opc, ops;

	u.u_ar0[R6] += n * sizeof(int);

	if (((opc = fuword((caddr_t)u.u_ar0[R6])) == -1)
	    || ((ops = fuword((caddr_t)(u.u_ar0[R6] + sizeof(int)))) == -1))
		psignal(u.u_procp, SIGSEGV);
	else {
		u.u_ar0[R6] += 2 * sizeof(int);
		u.u_ar0[PC] = opc;
		ops |= PS_CURMOD | PS_PRVMOD;   /* assure user space */
		ops &= ~PS_USERCLR;             /* priority 0 */
		u.u_ar0[RPS] = ops;
	}
}

#ifdef  UNIBUS_MAP

int     ub_wantmr;

#define UMAPSIZ 10
struct  mapent  _ubmap[UMAPSIZ];
struct  map     ub_map[1] = {
/*NOXSTR*/
	&_ubmap[0],     &_ubmap[UMAPSIZ],       "ub_map"
/*YESXSTR*/
};

#ifdef  UCB_METER
struct  ubmeter {
	long    ub_requests;            /* total # of calls to mapalloc */
	long    ub_remaps;              /* total # of buffer remappings */
	long    ub_failures;            /* total # of allocation failures */
	long    ub_pages;               /* total # of pages allocated */

} ub_meter;
#endif

/*
 * Routine to allocate the UNIBUS map
 * and initialize for a UNIBUS device.
 * For buffers already mapped by the UNIBUS map,
 * perform the physical-to-UNIBUS-virtual address translation.
 */
mapalloc(bp)
register struct buf *bp;
{
	long    paddr;
	ubadr_t ubaddr;
	int     s, ub_nregs;
	register ub_first;
	register struct ubmap *ubp;
	extern  memaddr bpaddr;

	if (!ubmap)
		return;
#if     defined(UNIBUS_MAP) && defined(UCB_METER)
	ub_meter.ub_requests++;
#endif
	paddr = ((long) ((unsigned) bp->b_xmem)) << 16
		| ((long) ((unsigned) bp->b_un.b_addr));
	if ((bp->b_flags & B_PHYS) == 0) {
		/*
		 * Transfer in the buffer cache.
		 * Change the buffer's physical address
		 * into a UNIBUS address for the driver.
		 */
#if     defined(UNIBUS_MAP) && defined(UCB_METER)
		ub_meter.ub_remaps++;
#endif
		ubaddr = paddr - (((ubadr_t) bpaddr) << 6) + BUF_UBADDR;
		bp->b_un.b_addr = loint(ubaddr);
		bp->b_xmem = hiint(ubaddr);
		bp->b_flags |= B_UBAREMAP;
	} else {
		/*
		 * Physical I/O.
		 * Allocate a section of the UNIBUS map.
		 */
		ub_nregs = (int) btoub(bp->b_bcount);
#if     defined(UNIBUS_MAP) && defined(UCB_METER)
		ub_meter.ub_pages       += ub_nregs;
#endif
		s = spl6();
		while ((ub_first = malloc(ub_map, ub_nregs)) == NULL) {
			ub_wantmr = 1;
#if     defined(UNIBUS_MAP) && defined(UCB_METER)
			ub_meter.ub_failures++;
#endif
			sleep(ub_map, PSWP + 1);
		}
		splx(s);

		ubp = &UBMAP[ub_first];
		bp->b_xmem = ub_first >> 3;
		bp->b_un.b_addr = (ub_first & 07) << 13;
		bp->b_flags |= B_MAP;

		while (ub_nregs--) {
			ubp->ub_lo = loint(paddr);
			ubp->ub_hi = hiint(paddr);
			ubp++;
			paddr += (ubadr_t) UBPAGE;
		}
	}
}

mapfree(bp)
register struct buf *bp;
{
	register s;
	extern  memaddr bpaddr;
	long    paddr;
	ubadr_t ubaddr;

	if (bp->b_flags & B_MAP) {
		/*
		 * Free the UNIBUS map registers
		 * allocated to this buffer.
		 */
		s = spl6();
		mfree(ub_map, (int) btoub(bp->b_bcount),
			(bp->b_xmem << 3) | ((bp->b_un.b_addr >> 13) & 07));
		splx(s);
		bp->b_flags &= ~B_MAP;
		if (ub_wantmr)
			wakeup((caddr_t) ub_map);
		ub_wantmr = 0;
	} else if (bp->b_flags & B_UBAREMAP) {
		/*
		 * Translate the UNIBUS virtual address of this buffer
		 * back to a physical memory address.
		 */
		ubaddr = ((long) ((unsigned) bp->b_xmem)) << 16
			| ((long) ((unsigned) bp->b_un.b_addr));
		paddr = ubaddr - (long) BUF_UBADDR
			+ (((long) bpaddr) << 6);
		bp->b_un.b_addr = loint(paddr);
		bp->b_xmem = hiint(paddr);
		bp->b_flags &= ~B_UBAREMAP;
	}
}
#endif  UNIBUS_MAP

#ifndef NOKA5
/*
 * Save the current kernel mapping and set it to the normal map.
 * Called at interrupt time to access proc, text or file structures
 * or the user structure.
 * Called only if *KDSA5 != seg5.se_addr from the macro savemap.
 * If NOKA5 is defined, the macro does the whole job.
 */
Savemap(map)
register mapinfo map;
{
	map[0].se_addr = *KDSA5;
	map[0].se_desc = *KDSD5;
	if (kdsa6) {
		map[1].se_addr = *KDSA6;
		map[1].se_desc = *KDSD6;
		*KDSD6 = KD6;
		*KDSA6 = kdsa6;
	} else
		map[1].se_desc = NOMAP;
	restorseg5(seg5);
}

/*
 * Restore the mapping information saved above.
 * Called only if map[0].se_desc != NOMAP (from macro restormap).
 */
Restormap(map)
register mapinfo map;
{
	*KDSA5 = map[0].se_addr;
	*KDSD5 = map[0].se_desc;
	if (map[1].se_desc != NOMAP) {
		*KDSD6 = map[1].se_desc;
		*KDSA6 = map[1].se_addr;
	}
}
#endif  NOKA5

#if     !defined(NOKA5) && defined(DIAGNOSTIC)
struct  buf     *hasmap;
#endif

/*
 *      Map in an out-of-address space buffer.
 *      If this is done from interrupt level,
 *      the previous map must be saved before mapin,
 *      and restored after mapout; e.g.
 *              segm save;
 *              saveseg5(save);
 *              mapin(bp);
 *              ...
 *              mapout(bp);
 *              restorseg5(save);
 */
#ifdef  UCB_NET
segm    Bmapsave;
#   ifdef       NOKA5
	ERROR!          /* NOKA5 must not be defined with the net for now. */
#   endif
#endif

caddr_t
mapin(bp)
register struct buf *bp;
{
	register caddr_t paddr;
	register caddr_t offset;

#ifdef  UCB_NET
	saveseg5(Bmapsave);
#endif
#if     !defined(NOKA5) && defined(DIAGNOSTIC)
	if (hasmap != (struct buf *) 0) {
		printf("mapping %o over %o\n", bp, hasmap);
		panic("mapin");
	}
	hasmap = bp;
#endif
	offset = bp->b_un.b_addr & 077;
	paddr = (bp->b_un.b_addr >> 6) & 01777;
	paddr |= bp->b_xmem << 10;
	mapseg5(paddr, (BSIZE << 2) | RW);
	return(SEG5 + offset);
}

#ifndef NOKA5
void
mapout(bp)
register struct buf *bp;
{
#ifdef  DIAGNOSTIC
	if (bp != hasmap) {
		printf("unmapping %o, not %o\n", bp, hasmap);
		panic("mapout");
	}
	hasmap = (struct buf *) NULL;
#endif
#ifndef UCB_NET
	normalseg5();
#else
	restorseg5(Bmapsave);
#endif
}
#endif  NOKA5

/*
 * ���������� wcnt ���� �� ������ bfrom � ����� bto.
 */
copybb( bfrom, bto, offfrom, offto, wcnt )
struct buf *bfrom, *bto;
register wcnt;
{
	caddr_t          a1, a2;
	register int    *o1, *o2;
	int              worka[64];
	int             *p, *q, iwc;

	/* MAP IN */
#ifdef  UCB_NET
	saveseg5(Bmapsave);
#endif
#if     !defined(NOKA5) && defined(DIAGNOSTIC)
	if (hasmap != (struct buf *) 0) {
		printf("mapping %o over %o\n", bto, hasmap);
		panic("copybb:in");
	}
	hasmap = bto;
#endif

	/* COMPUTE ADDRESSES */
	o1  = (bfrom->b_un.b_addr & 077) + SEG5 + offfrom;
	a1  = (bfrom->b_un.b_addr >> 6) & 01777;
	a1 |=  bfrom->b_xmem << 10;
	o2  = (bto  ->b_un.b_addr & 077) + SEG5 + offto;
	a2  = (bto  ->b_un.b_addr >> 6) & 01777;
	a2 |=  bto  ->b_xmem << 10;

	/* COPY IT */
	while( wcnt > 0 ) {
		mapseg5(a1, (BSIZE << 2) | RW);
		p = worka;
		iwc = wcnt;
		if( iwc > 64 ) iwc = 64;
		q = &worka[iwc];
		while( p < q )
			*p++ = *o1++;
		mapseg5(a2, (BSIZE << 2) | RW);
		p = worka;
		while( p < q )
			*o2++ = *p++;
		wcnt -= iwc;
	}

	/* MAP OUT */
#ifdef  DIAGNOSTIC
	if (bto != hasmap) {
		printf("unmapping %o, not %o\n", bto, hasmap);
		panic("copybb:out");
	}
	hasmap = (struct buf *) NULL;
#endif
#ifndef UCB_NET
	normalseg5();
#else
	restorseg5(Bmapsave);
#endif
}

void
boot(dev, howto)
dev_t   dev;
int     howto;
{
	struct buf *bp;
	caddr_t     block;
	int col, cnt, i;

	if ((howto & RB_NOSYNC) == 0 && bfreelist.b_forw) {
		printf(" syncing disks ... ");
		update();
		col = 18;
		for( cnt = 0 ; cnt < 107 ; cnt++ ) {
			if( cnt > 100 )
				delay(1);
			if( i = breqcount() ) {
				if( col > 72 )
					printf("\n"), col = 0;
				printf(" %d", i);
				if( i > 99 ) col++;
				if( i > 9  ) col++;
				col += 2;
			} else
				break;
		}
		if( col > 70 )
			printf("\n");
		printf(" done\n");
	}
	(void) _spl7();
	if (howto & RB_DUMP) {
		/*
		 * save the registers in low core.
		 */
		saveregs();
		dumpsys();
	}
	if (howto & RB_HALT) {
HLT:
		printf("halting\n");
		halt();
		/*NOTREACHED*/
	} else {
		(void)_spl1();                  /* turn on disk interrupts */
		bp = bread(dev, (daddr_t)0);
		if( bp == (struct buf *)NULL ||
		    (bp->b_flags & B_ERROR) ) {
			printf( "can't read boot: " );
			goto HLT;
		}
		(void)_spl7();                  /* Turn all of interrupts off */
#if !defined(NOKA5) && defined(DIAGNOSTIC)
		hasmap = (struct buf *)NULL; /* for panic */
#endif
		block = mapin(bp);
#if UCB_NKB == 1
		if( major(dev) == 10 )      /* Dec Pro HD: phys blk 1 */
			block += 512;
#endif
		doboot(dev, howto, block,
		      (major(dev) == 10||major(dev) == 12)); /* Dec Pro Bootstraps */
		/*NOTREACHED*/
	}
}

/*
 * wait for time "del", approximately in seconds.
 * Used to avoid rescheduling from sleep().
 */
delay(del)
register del;
{
	register j;
	time_t oldtime = time;
	while (time - oldtime <= del) {
			for (j=32000; j>0; --j)
				;
	}
}

/*
 *  Dumpsys takes a dump of memory by calling (*dump)(), which must
 *  correspond to dumpdev.  *(dump)() should dump from dumplo blocks
 *  to the end of memory or to the end of the logical device.
 */
dumpsys()
{
	extern  int (*dump)();

	if (dumpdev != NODEV) {
		printf("\ndump to dev %o,%D\n", dumpdev, dumplo);
		printf("dump ");
		switch ((*dump)(dumpdev)) {

		case EFAULT:
			printf("dev not ready\n");
			break;
		case EINVAL:
			printf("args invalid\n");
			break;
		case EIO:
			printf("I/O err\n");
			break;
		default:
			printf("err(?)\n");
			break;
		case 0:
			printf("ok\n");
			return (1);
		}
	}
	return (0);     /* failure */
}

/*
 * lock user into core as much
 * as possible. swapping may still
 * occur if core grows.
 */
syslock()
{
	register struct proc *p;
	struct a {
		int     flag;
	};

	if (suser()) {
		p = u.u_procp;
		p->p_flag &= ~SULOCK;
		if (((struct a *)u.u_ap)->flag)
			p->p_flag |= SULOCK;
	}
}

/*
 * �������� ������ ���������� ������������ �����.
 */
vect_conflict()
{
	extern struct vecconfl {
		int  vc_name;
		int *vc_vect;
		int  vc_major;
	} vecconfl[];
	register struct vecconfl *vcfl;
	register int              sd,rd;
	segm                      saveregs;

	vcfl = vecconfl;
	rd = major(rootdev);
	sd = major(swapdev);
	saveseg5(saveregs);   /* Set seg5 to 0 data */
	mapseg5( 0, (((1024/64-1)<<8)|RW) );
	while( vcfl->vc_name != 0 ){
		if( vcfl->vc_major == rd || vcfl->vc_major == sd ){
			*(vcfl->vc_vect+050000) = vcfl->vc_name;
			*(vcfl->vc_vect+050001) = 0240;
		}
		vcfl++;
	}
	restorseg5(saveregs);
}

/*
 * ������������� ������� ������ ��� record locking (fcntl)
 */
stfrflck()
{
	register struct mflock *pfl,*epfl;
		 segm           saveregs;

	flock_size = btoc( nfslot * sizeof(struct mflock) );
	if ((flock_base = malloc(coremap, flock_size)) == 0)
		panic("flock");
	saveseg5(saveregs);
	mapseg5(flock_base, (((--flock_size) << 8) | RW) );
	pfslot = (struct mflock *)SEG5;
	epfl = &pfslot[nfslot];
	for( pfl = pfslot ; pfl < epfl ; pfl++ )
		pfl->m_next = (pfl+1);
	(--pfl)->m_next = NULL;
	restorseg5(saveregs);
	return(flock_size+1);
}
