/*
 * ������� ����������� ������� ����� MY ( ��� Qbus22)
 *
 * $Log:	my.c,v $
 * Revision 1.1  90/12/12  16:46:44  korotaev
 * ������ ��� ���
 * 
 * Revision 1.1  89/01/16  15:09:31  chech
 * Initial revision
 * 
 * ������������������ �������
 */

#include "h/my.h"
#if     NMY > 0
#include <sys/param.h>
#include "../include/buf.h"
#include <sys/conf.h>
#include <sys/dir.h>
#include "../include/iopage.m"
#include "../include/myreg.h"
#include <sys/systm.h>
#include <sys/user.h>

/* #define MY_DEBUG        1       /* */

#define MYU(dev)  ((dev)&03)    /* ���. ��-��, �� ������� ��������� */
#define MYC(dev)  (((dev)&074)>>03)     /* ���. ����������� */
#define MYDOUBLE(dev) (((dev)&0200))    /* 2-� ��������� ��-�� */
#define MY9(dev)  ((dev)&0100)          /* 9-�� ��������� ��-�� */

#define MYTRK   80
#define MYSEC(dev)  (MY9(dev)?9:10)     /* �������� �� ��-�� */
#define MYSIDE(dev) (MYDOUBLE(dev)?2:1) /* ������ �� ��-�� */

#define NMYBLK(d) (MYTRK*MYSEC(d)*MYSIDE(d)) /* ����� ������ �� ���������� */
#define MYTRYS  5                       /* ����� �������� �� ������ */
#define MYRESET 25                      /* ������ ����-���� � ����� */

#define MYPRF(dev) ((dev)&077),((MYDOUBLE(dev))?"d":"s"),MYSEC(dev)

#ifdef UCB_DEVERR
#ifdef UNFAST
HARDERR(b) register struct buf *b;
{printf("my%d%ss%d: err bn %D ", MYPRF(b->b_dev), b->b_blkno );}
#else
#define HARDERR(b) printf("my%d%ss%d: err bn %D ",MYPRF(b->b_dev),b->b_blkno);
#endif
#endif

#ifdef MY_SPL
#define SPLL    _spl1 ()
#define SPLM    _spl5 ()
#else
#define SPLL    /* */
#define SPLM    /* */
#endif

struct  my_parm {               /* ���� ���������� -- ��� �������� ������ */
	char     p_dev;         /* 0-1 - drive, 2 - side */
	char     p_xmem;        /* 8-13 - ��� ��� ������� ������ */
	caddr_t  p_addr;        /* ������� ����� ������ ������� ������ */
	char     p_sec;         /* ������ */
	char     p_trk;         /* ������� */
	u_short  p_wcnt;        /* ������� ���� */
} myparm [NMYC];

struct  my_soft {               /* ����� ����� ���������� */
	char    m_hmem;         /* ������� ����� ������ */
	int     m_lmem;         /* ������� ����� ������ */
} mysoft [NMYC];

struct  mydevice *MYADDR[NMYC]; /* �������� ��������� */
struct  buf     mytab[NMYC];    /* ������� �������� � ����������� */
struct  buf     rmybuf[NMYC];   /* ����. ��������� ��� RAW */

myattach (addr, unit)
register struct mydevice *addr;
register unit;
{
	if (unit >= NMYC )
		return(0);
	MYADDR[unit] = addr;
	return(1);
}

myopen (dev, rw)
dev_t dev;
{
	register struct mydevice *myaddr;
	register drive, ctrl;
	struct my_soft *M;

	ctrl  = MYC(dev);
	drive = MYU(dev);
	if(drive >= NMY || ctrl >= NMYC || !(myaddr = MYADDR[ctrl])) {
		u.u_error = ENXIO;
		return;
	}
	/* ��������� ������ ����� ���������� ��� ��������� my_soft */
	M = &mysoft [ctrl];
	if (!M->m_hmem && !M->m_lmem) {
		register ts, nb;
		register unsigned base;

		base = &myparm [ctrl];
		nb = (base >> 6) & 01777;
		ts = ((u_short *)(sep_id ? KDSA0:KISA0))[nb >> 7] +
		     (nb & 0177);
		M->m_lmem = (caddr_t)(ts << 6) + (base & 077);
		M->m_hmem = (ts >> 10) & 077;
	}
#ifdef MY_DEBUG
	printf("my%d%ss%d: open lmem %o hmem %o\n",MYPRF(dev),
	       M->m_lmem,M->m_hmem);
#endif
}

mystrategy (bp)
register struct buf *bp;
{
	register c;
	register struct buf *dp;
	int s;

	c = MYC(bp->b_dev);
	if (MYADDR[c] == (struct mydevice *) NULL) {
		bp->b_error = ENXIO;
		goto errexit;
	}
	if( bp->b_blkno >= NMYBLK(bp->b_dev) ) {
		bp->b_error = EINVAL;
errexit:
		bp->b_flags |= B_ERROR;
		iodone (bp);
		return;
	}
	s = spl5 ();
	bp->av_forw = (struct buf *)NULL;
	dp = &mytab[c];
	if(dp->b_actf == NULL)
		dp->b_actf = bp;
	else
		dp->b_actl->av_forw = bp;
	dp->b_actl = bp;
	if(dp->b_active == NULL)
		mystart (c);
	splx (s);
}

mystart (c)
{
	register struct mydevice *myaddr = MYADDR[c];
	register struct buf *bp;
	register struct buf *dp = &mytab[c];
	struct my_parm *P = &myparm[c];
	daddr_t bn;
	int dn;

	if ((bp = dp->b_actf) == NULL)
		return;
	dp->b_active++;
	bn = bp->b_blkno;
	P->p_wcnt = bp->b_bcount>>1;
	P->p_xmem = bp->b_xmem;
	P->p_addr = bp->b_un.b_addr;
	P->p_sec = 1 + bn % MYSEC(bp->b_dev);
	bn = bn / MYSEC(bp->b_dev);   /* ���. ����� * 2 */
	P->p_trk = MYDOUBLE(bp->b_dev) ? bn >> 1 : bn;
	P->p_dev = MYU(bp->b_dev) | ((MYDOUBLE(bp->b_dev))?(bn & 01)<<2:0);

#ifdef MY_DEBUG
	printf("my%d%ss%d: start lmem %o hmem %o\n",MYPRF(dev),
		mysoft[c].m_lmem,mysoft[c].m_hmem);
	printf("\tReq: dev %o xmem %o addr %o sec %o trk %o wcnt %o\n",
	    P->p_dev, P->p_xmem, P->p_addr, P->p_sec, P->p_trk, P->p_wcnt);
#endif
	for (dn = 010000; !(myaddr->my_csr & MY_DONE); --dn)
		if (!dn)
			goto STOUT;
	myaddr->my_csr = MY_GO | MY_IE |
			((bp->b_flags&B_READ) ? MY_READ:MY_WRITE) |
			(mysoft[c].m_hmem << 8);
	for (dn = 010000; (myaddr->my_csr & (MY_DONE|MY_TR)) != MY_TR; --dn) {
		if (!dn) {
STOUT:
#ifdef UCB_DEVERR
			HARDERR(bp);
			printf ("timeout\n");
#else
			printf("my%d%ss%d: timeout\n",MYPRF(bp->b_dev));
#endif
			timeout(mystart, (caddr_t)c, MYRESET);
			return;
		}
	}
	myaddr->my_dat = mysoft[c].m_lmem;
#ifdef  MY_DKN
	dk_busy |= 1 << MY_DKN;
	dk_numb[MY_DKN]++;
	dk_wds[MY_DKN] += bp->b_bcount >> 6;
#endif  MY_DKN
}

myintr(c)
{
	register struct mydevice *myaddr = MYADDR[c];
	register struct buf *bp;
	register struct buf *dp = &mytab[c];
	int err;

	if (dp->b_active == NULL)
		return;
#ifdef  MY_DKN
	dk_busy &= ~(1 << MY_DKN);
	bp = dp->b_actf;
	dp->b_active = NULL;
	SPLL;
	if (myaddr->my_csr & MY_ERR) {
#ifdef  UCB_DEVERR
		err = myaddr->my_dat;
		HARDERR(bp);
		if (err & MYDS_ERR)     /* �� ������ ���������� ...*/
		     goto RETRY;        /*...���� ������ ��������� ����� */
		else if (err & MYDS_WLO && !(bp->b_flags & B_READ)) {
			bp->b_error = EROFS;
			printf("write locked\n");
		}
		else if (err & MYDS_FMT) {
			bp->b_error = ENXIO;
			printf ("unknown format\n");
		}
		else if (err & MYDS_NXM) {
			bp->b_error = EFAULT;
			printf ("nonexistent memory\n");
		}
		else {
			if (!(err & MYDS_ROT))
				printf ("drive not ready ");
RETRY:                  printf("cs=%o ds=%b\n", myaddr->my_csr,
				myaddr->my_dat, MYDS_BITS);
#else
		{
			deverror(bp, myaddr->my_csr, myaddr->my_dat);
#endif
			SPLM;
			if (++dp->b_errcnt > MYTRYS) goto ERROR;
			if ((err & (MYDS_SK0E|MYDS_SKE|MYDS_ERR)) ||
			   (dp->b_errcnt == MYTRYS/2))  {
				myaddr->my_csr = MY_RESET | MY_GO;
				timeout(mystart, (caddr_t)c, MYRESET);
			}
			else
				mystart(c);
			return;
		}
ERROR:
		bp->b_flags |= B_ERROR;
		bp->b_resid = bp->b_bcount;
	}
	else
		bp->b_resid = 0;
	SPLM;
	dp->b_errcnt = 0;
	dp->b_actf = bp->av_forw;
	SPLL;
	iodone(bp);
	SPLM;
	mystart(c);
}

myread(dev)
register dev_t   dev;
{
	physio(mystrategy, &rmybuf[MYC(dev)], dev, B_READ);
}

mywrite(dev)
register dev_t   dev;
{
	physio(mystrategy, &rmybuf[MYC(dev)], dev, B_WRITE);
}
#endif  NMY
