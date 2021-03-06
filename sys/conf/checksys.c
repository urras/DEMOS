/*
 * checksys
 *      checks the system size and reports any limits exceeded.
 *
 * $Log:	checksys.c,v $
 * Revision 1.4  89/07/05  15:06:26  avg
 * �������� �������� �� remap_area (����� �� ���� ���������� ����
 * "��������� ������ ������ �� 0 ������".
 * 
 * Revision 1.3  89/04/11  14:05:50  avg
 * �������� ��������� �����������.
 *
 * Revision 1.2  88/06/23  21:31:23  ache
 * + 31 ������� � ����
 *
 * Revision 1.1  86/04/19  15:01:55  avg
 * Initial revision
 *
 */

#include <sys/param.h>
#include <a.out.h>
#include <stdio.h>
#include <ediag.h>
#include <sys/tty.h>
#include <sys/fcntl.h>
#include "h/space.h"

#define KB      * 1024

/*
 *  Round up to a click boundary.
 */
#define cround(bytes)   ((bytes + ctob(1) - 1) / ctob(1) * ctob(1));

struct exec obj;
static xoverlaid = 0;
struct xovlhdr   ovlhdr;

struct nlist nl[] =
{
	"_end", 0, 0,
#define N_END           0
	"_remap_a", 0, 0,
#define N_REMAPAREA     1
	"NOKA5", 0, 0,
#define N_NOKA5         2
	"_nbuf", 0, 0,
#define N_NBUF          3
	"_bsize", 0, 0,
#define N_BSIZE         4
	"UCB_CLIST", 0, 0,
#define N_CLIST         5
	"_nclist", 0, 0,
#define N_NCLIST        6
	"_nfslot", 0, 0,
#define N_FLOCK         7
	"_msgtota", 0, 0,
#define N_MSG           8
	0
};

char *file;
int fi;
long getval();

main(argc,argv)
int argc;
char **argv;
{
	register i;
	long size, totsize;
	int errs = 0;
	int texterrs = 0;
	long  maxdata;

	if (argc != 2) {
		printf(ediag(" Usage: %s demos-binary\n",
		       " �����: %s �����������_����_����_�����\n"),
		       argv[0]);
		exit(20);
	}
	file = argv[1];
	if ((fi=open(file,0)) < 0)
	{
		perror(file);
		exit(20);
	}
	if (read(fi,&obj,sizeof(obj)) != sizeof(obj))
	{
		printf(ediag(" %s is not an object file.\n",
		       " ���� %s �� �������� ��������� ������\n"),
		       file);
		close(fi);
		exit(20);
	}
	if (obj.a_magic == A_MAGIC5 || obj.a_magic == A_MAGIC6 ||
	    obj.a_magic == A_MAGIC7) {
		xoverlaid = (obj.a_magic == A_MAGIC7);
		if (read(fi,&ovlhdr,
xoverlaid?sizeof(struct xovlhdr):sizeof(struct ovlhdr)) !=
(xoverlaid?sizeof(struct xovlhdr):sizeof(struct ovlhdr))) {
			printf(ediag(" %s is not an object file.\n",
			       " ���� %s �� �������� ��������� ������\n"),
				file);
			close(fi);
			exit(20);
		}
	}
	switch(obj.a_magic)
	{

	/*
	 *      0407-- nonseparate I/D "vanilla"
	 */
	case A_MAGIC1:
		size = (long)obj.a_text + obj.a_data + obj.a_bss;
		maxdata = (unsigned)48 KB;
		if (size > maxdata) {
			printf(ediag(" Total size larger than 48K by %D bytes.\n",
			" ������ ���� ������ 48K �� %D ����\n"),
				size - maxdata);
			errs++;
		}
		maxdata -= (long)obj.a_text;
		totsize = cround(size);
		break;

	/*
	 *      0411-- separate I/D
	 */
	case A_MAGIC3:
		size = (long) obj.a_data + obj.a_bss;
		maxdata = (unsigned)48 KB;
		if (size > maxdata) {
			printf(ediag(" Data larger than 48K by %D bytes.\n",
			" ������ ������ ������ 48K �� %D ����\n"),
				size - maxdata);
			errs++;
		}
		totsize = obj.a_text + cround(size);
		break;

	/*
	 *      0430-- overlaid nonseparate I/D
	 *      0444-- extended overlaid nonsep I/D
	 */
	case A_MAGIC5:
	case A_MAGIC7:
		if (obj.a_text > 16 KB) {
			printf(ediag(" Base segment larger than 16K by %u bytes.\n",
			" ������� ������� ������ 16K �� %u ����.\n"),
				obj.a_text - 16 KB);
			errs++;
			texterrs++;
		}
		maxdata = (obj.a_text <= (8 KB)) ? (unsigned)(32 KB) : (24 KB);
		size = (long) obj.a_data + obj.a_bss;
		if (size > maxdata) {
			printf(ediag(" Data larger than %DK by %D bytes.\n",
			" ������ ������ ������ %DK �� %D ����\n"),
			maxdata/1024, size - maxdata);
			errs++;
		}
		/*
		 *  Base and overlay 1 occupy 16/8K and 8K of physical
		 *  memory, respectively, regardless of actual size.
		 */
		totsize = 16 KB + ((obj.a_text > (8 KB))? (8 KB): (0 KB))
			  + cround(size);

		/*
		 *  Subtract the first overlay, it will be added below
		 *  and it has already been included.
		 */
		totsize -= ovlhdr.xov_siz[0];
		goto checkov;
		break;

	/*
	 *      0431-- overlaid separate I/D
	 */
	case A_MAGIC6:
		if (obj.a_text > (unsigned) 56 KB) {
			printf(ediag(" Base segment larger than 56K by %u bytes.\n",
			" ������� ������� ������ 56K �� %u ����.\n"),
				obj.a_text - (unsigned) 56 KB);
			errs++;
		}
		if (obj.a_text <= (unsigned) 48 KB) {
			printf(ediag(" Base segment smaller than 48K by %u bytes.\n",
			" ������� ������� ������ 48K �� %u ����.\n"),
				(unsigned) 48 KB - obj.a_text);
			errs++;
		}
		size = (long)obj.a_data + obj.a_bss;
		maxdata = (unsigned) 48 KB;
		if (size > maxdata) {
			printf(ediag(" Data larger than 48K by %D bytes.\n",
			" ������ ������ ������ 48K �� %D ����\n"),
				size - maxdata);
			errs++;
		}
		totsize = (long)obj.a_text + cround(size);
checkov:
		for (i=0; i<(xoverlaid?NXOVL:NOVL); i++) {
			totsize += ovlhdr.xov_siz[i];
			if (ovlhdr.xov_siz[i] > 8 KB) {
			    printf(ediag(" Overlay %d larger than 8K by %u bytes.\n",
			    " ������� %d ������ 8K �� %u ����\n"),
				i+1, ovlhdr.xov_siz[i] - 8 KB);
			    errs++;
			    texterrs++;
			}
		}
		break;

	default:
		printf( ediag(" Magic number not recognized.\n",
			" �������� magic number\n") );
		close(fi);
		exit(20);
	}

	nlist(file, nl);
	if (nl[N_NOKA5].n_type == 0) {
		printf(ediag(" Symbols not found in namelist\n",
		       " �� ������� ������� � ������� ����\n") );
		exit(20);
	}
	if (texterrs == 0) {
	    if (nl[N_NOKA5].n_value == 0) {
		if (nl[N_REMAPAREA].n_value > 0120000) {
			printf(ediag(
" The remapping area (0120000-0140000)\n",
" ����� ���� (������ 0120000-0140000) ������ ��������� ������\n"));
			printf(ediag(
"contains data other than the proc, text and file tables.\n",
"������� proc, text � file. � ������ ������ � ���� �����\n"));
			printf(ediag(
"Reduce other data by %u bytes.\n",
"���������� ������ ������. ���� �� ��������� �� %u ����\n"),
			nl[N_REMAPAREA].n_value - 0120000);
			errs++;
		}
		if (nl[N_END].n_value >= 0140000) {
			printf(ediag(
" Too big data size (exeeds 0140000), extends by %u bytes.\n",
" ������� ������ �������� ����� 0140000 �� %u ����.\n"),
		nl[N_END].n_value - 0140000);
			errs++;
		}
		if( errs == 0 ){
			printf(ediag(
" There are %u bytes free in data (%s %d).\n",
" %u ���� ������ �������� (%s %d)\n"),
0140000 - nl[N_END].n_value, "0120000 - &_remap_area ==",
			0120000-nl[N_REMAPAREA].n_value);
		}
	    } else {
		if (nl[N_END].n_value > 0120000) {
			printf(ediag(
" Data extends into the remapping area (0120000-0140000)\n by %u bytes; undefine NOKA5 or reduce data size.\n",
" ������� ������ �������� ����� 0120000 �� %u ����. ����\n ��������� ������ ��� ��������� �������� NOKA5 � ����� localopts.h.\n"),
		nl[N_END].n_value - 0120000);
			errs++;
		} else {
			printf(ediag( " There are %u free bytes in data\n",
			 " ��������� ����� � ������: %u\n"),
			 0120000 - nl[N_END].n_value);
		}
	    }
	}

	totsize += cround( getval(N_NBUF,0) * getval(N_BSIZE,0));
	if (nl[N_CLIST].n_value)
		totsize += cround(getval(N_NCLIST,0)
			* (long) sizeof(struct cblock));
	totsize +=  getval(N_FLOCK,0) * sizeof(struct mflock);
	totsize += ctob(USIZE);
	totsize +=  getval(N_MSG,1);
	printf(ediag(
" System will occupy %D bytes of memory\n (including all outplaced data).\n",
" ������� (�� ����� ����������� �� ���� �������) �������� %D ����.\n"),
		totsize);

	close(fi);
	if (errs)
		printf(ediag("**** SYSTEM IS NOT BOOTABLE ****\n",
		"**** ������� ���������������� ****\n"));
	exit(errs);
}

#define round(x) (ctob(stoc(ctos(btoc(x)))))
/*
 *  Get the value of an initialized variable from the object file.
 */
long getval(index,flg)
int flg;
{
	int ret = 0;
	long retl = 0l;
	off_t offst;

	if( nl[index].n_type == 0 )
		return 0l;
	offst = (off_t)nl[index].n_value
		+ (off_t) obj.a_text + sizeof(obj);
	if (obj.a_magic == A_MAGIC2 || obj.a_magic == A_MAGIC5 ||
	    obj.a_magic == A_MAGIC7)
		offst -= (off_t)round(obj.a_text);
	if (obj.a_magic == A_MAGIC5 || obj.a_magic == A_MAGIC6 ||
	    obj.a_magic == A_MAGIC7) {
		register i;

		offst += xoverlaid ? sizeof(struct xovlhdr) :
				     sizeof(struct ovlhdr);
		if (obj.a_magic == A_MAGIC5 || obj.a_magic == A_MAGIC7)
			offst -= (off_t)round(ovlhdr.xmax_ovl);
		for(i=0; i<(xoverlaid?NXOVL:NOVL); i++)
			offst += (off_t)ovlhdr.xov_siz[i];
	}
	lseek(fi, offst, 0);
	if( flg == 1 ){
		read(fi, &retl, sizeof(retl));
		return(retl);
	}else{
		read(fi, &ret, sizeof(ret));
		return( (long)ret );
	}
}
