/*
 * ������� ���������� �����������-85
 *
 * $Log:	rw.c,v $
 * Revision 1.5  90/01/29  15:51:25  rygoff
 * ��������� ����� ������� ��� �������.
 * ��������� �������� ������.
 * 
 * Revision 1.4  90/01/29  07:20:47  rygoff
 * ��� ��� �������� ������� �� �����. 
 * ����� rw0a ��� ����� ����� ����������� �� ������ 50 ���������
 * ���� ������� ������. 
 * 
 * Revision 1.3  90/01/10  16:59:37  rygoff
 * ������� ����� ���������.
 * ��������������� ���������� ������������� ������� �� �����������.
 * ��� ��� ������� �������� ���������� � �������������.
 * ������ �������� ������������ �������.
 * ����� ������������ ������� � ���� ����.
 * 
 * Revision 1.2  90/01/08  18:47:24  rygoff
 * ������ ������� ������ � ���������������� ������������ 
 * (RD52). � ����� � ���� �������� ������� ������ �� �����.
 * 
 * Revision 1.1  90/01/08  18:41:50  rygoff
 * Initial revision (��������� ������� � ������)
 * 
 * Revision 22.3  89/05/05  20:10:15  avg
 * ����������� (��� ���������� ������ ������!), ��� �� �����
 * ����� ���������� ����������� ��� ������� ������� ������������ �����.
 * ��������� ��� �������� �������� ������ � ������ � 2 ����!
 *
 * Revision 22.2  89/04/27  13:52:47  korotaev
 * ��������� ��������� � ��������� ������������ ��������� � ������
 *
 * Revision 22.1  89/04/12  14:34:12  korotaev
 * "param.h" ==> <sys/param.h>
 *
 * Revision 22.0  89/03/25  12:32:41  korotaev
 * ������ ���������� ������ 2.2
 *
 * Revision 1.2  88/01/21  11:49:31  korotaev
 * ��������� � �-85.
 *
 */

#include "rw.h"
#if     NRW > 0
#include "param.h"
#include <sys/systm.h>
#include <sys/buf.h>
#include <sys/conf.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/seg.h>
#include <sys/iopage.h>

/* #define RWWCHK                  /* Enable Write Check */
#define ERR_TRY         5       /* Number of repeats on IOerror */
#define SOFT_TRY        2       /* Softerr retries */


#define SET_IE  030
#define RES_IE  070
#define CLR_IN  0170

#define RB(x)   *((char *)(x))

#define RWSLOT(adr)  (((int)(adr) >> 7) & 07)

struct rwdevice {
	short   rw_rid;         /* ������� �������������          */
	short   rw_m1;          /* unused                         */
	short   rw_err;         /* ������� ������/��������������� */
	short   rw_sec;         /* ����� �������                  */
	short   rw_data;        /* ������� ������                 */
	short   rw_cyl;         /* ����� ��������                 */
	short   rw_trk;         /* ����� �����������              */
	short   rw_rks2;        /* ��� 2                          */
	short   rw_stat;        /* ������� ���������/���������    */
};

/*      ��������� �����������                           */
/*         �����  ����� �������.       �����            */
/*�����    �����  ����� �������  ���������  ��������    */
struct { int ntrk,shift,scyl /* ,ncyl */; long nsec; } rw_parts[]=
{
  /*a*/  {    4,    0,    0,     50L    *  16 * 4, },
  /*b*/  {    4,    0,   50,     25L    *  16 * 4, },
  /*c*/  {    4,    0,   75,    949L    *  16 * 4, },
  /*d*/  {    8,    0,   75,    949L    *  16 * 8, },
  /*e*/  {    4,    4,    0,     75L    *  16 * 4, },
  /*f*/  {    4,    4,   75,    949L    *  16 * 4, },
  /*g*/  {    4,    4,    0,   1024L    *  16 * 4, },
  /*h*/  {    8,    0,    0,   1024L    *  16 * 8, }
};

int rwWPC = -1;

struct  rwdevice *RWADDR[NRW];

struct  buf     rwtab;
struct  buf     rrwbuf;
static short    rwwchk = 0, rwwcerr;

rwattach(addr, unit)
struct rwdevice *addr;
{
	if (unit > NRW)
		return(0);
	RWADDR[unit] = addr;
	return(1);
}

rwstrategy(bp)
register struct buf *bp;
{
	register s, unit, drive;
	register struct buf *dp;
	int  ntrk;
	long nsec;

	drive = minor(bp->b_dev) >> 3;
	unit = minor(bp->b_dev) & 07;
	ntrk = rw_parts[unit].ntrk;
	nsec = rw_parts[unit].nsec;

	if (drive >= NRW || RWADDR[drive] == (struct rwdevice *) NULL) {
		bp->b_error = ENXIO;
		goto errexit;
	}

	if( bp->b_blkno < 0 || bp->b_blkno >= nsec ) {
		printf("rw%d%c: bad block %D\n", drive,'A'+unit,bp->b_blkno);
		bp->b_error = EINVAL;
errexit:
		bp->b_flags |= B_ERROR;
		iodone(bp);
		return;
	}


	bp->av_forw = (struct buf *)NULL;
	bp->b_cylin = bp->b_blkno/(RWSEC*ntrk) + rw_parts[unit].scyl;
	s = spl5();
	dp = &rwtab;
	disksort(dp, bp);
	if(dp->b_active == NULL)
		rwstart(0);
	splx(s);
}

/*
 * ����������� �������� ��� ������-DMA
 */
static unsigned rw_addr, rwbytec, rwxmem, rwisread, rwactive = 0, rwdev;

/*
 * ��������� �������� �� ����������
 *
 *      0 - � ������ �� bp->b_blkno (������ ������)
 *      1 - � ��� �� ������� �����  (������ ������)
 *      2 - �� ����. ������� �����  (����. ������)
 */
rwstart(arg)
{
	register struct rwdevice *rwaddr;
	register struct buf *bp;
	static daddr_t bn;
	int tn, cn, sn, cyloff, slot, dev;

	int unit, ntrk;
	long nsec;


	if ((bp = rwtab.b_actf) == NULL)
		return;
	rwtab.b_active++;
	rwaddr = RWADDR[dev = (minor(bp->b_dev) >> 3)];
	slot   = RWSLOT(rwaddr);

	/* CLEAR CONTROLLER */
	tn = rwaddr->rw_sec;

	if( arg == 0 ) {
		bn = bp->b_blkno;
		rwbytec  = bp->b_bcount;
		rw_addr  = bp->b_un.b_addr;
		rwxmem   = bp->b_xmem;
		rwisread = ((bp->b_flags & B_READ) != 0);
		rwdev    = dev;
	} else if( arg == 2 )
		bn++;

	unit = minor(bp->b_dev) & 07;
	ntrk = rw_parts[unit].ntrk;

	rwaddr->rw_cyl  =   rw_parts[unit].scyl + (bn/(ntrk*RWSEC));
	rwaddr->rw_trk  =   (bn/RWSEC) % ntrk + rw_parts[unit].shift;
	rwaddr->rw_sec  =   bn         % RWSEC;

	rwactive = 1;
	if( rwisread || rwwchk ) {
		RB(I85_CR2) = SET_IE | slot;
		RB(I85_CR2) = CLR_IN | slot;
		RB(I85_CR3) = SET_IE | slot;
		RB(I85_CR3) = CLR_IN | slot;
		rwaddr->rw_rks2 = 040;
	} else {
		if( rwWPC != -1 )
			rwaddr->rw_err = rwWPC >> 2;
		RB(I85_CR3) = RES_IE | slot;
		rwaddr->rw_rks2 = 060;
	}

#ifdef  RW_DKN
	dk_busy |= 1 << RW_DKN;
	dk_numb[RW_DKN]++;
	dk_wds[RW_DKN] += bp->b_bcount >> 6;
#endif  RW_DKN

	if( !(rwisread || rwwchk) ) {
		RB(I85_CR2) = CLR_IN | slot;
		rwpdma();
	}
}

#define PROTO5  ((15<<8) | RW)

/*
 * ������ ������ "�"
 *   '���������� ������'
 */
rwpdma()
{
	struct rwdevice *rwaddr;
	register *a, i, *rwData;
	segm    save5;
	int     cnt, slot;
	short   count4, count1, skips;
	int     *savea;

	if( rwactive == 0 )
		return;
	rwaddr = RWADDR[rwdev];
	slot   = RWSLOT(rwaddr);

	if( rwisread || rwwchk )
		RB(I85_CR3) = RES_IE | slot;
	saveseg5(save5);
	a = (rw_addr & 077) | SEG5;
	mapseg5( (rwxmem<<10) | ((rw_addr>>6) & 01777), PROTO5 );
	i = rwbytec/2;
	if( i > 256 )
		i = 256;

	count4 = i/4;           /* ����������� �������� �� 4 ����� */
	count1 = i & 03;        /* ����������� �� ������ ����� */
	skips  = 256-i;         /* ������ ����� */

	cnt = -1;
	while( (rwaddr->rw_stat & 0200) == 0 && --cnt );
	rwData = &(rwaddr->rw_data);

	i = count4;

	if( rwisread ) {
		do {
			*a++ = *rwData;
			*a++ = *rwData;
			*a++ = *rwData;
			*a++ = *rwData;
		} while( --i != 0 );
		switch( count1 ) {
		    case 3:
			*a++ = *rwData;
		    case 2:
			*a++ = *rwData;
		    case 1:
			*a++ = *rwData;
		    case 0: ;
		}
		if( skips ) {
			a = &cnt;
			i = skips;
			do {
				*a = *rwData;
			} while( --i != 0 );
		}
#ifdef RWWCHK
	} else if( rwwchk ) {
		savea = a;
		do {
			if( *rwData != *a++ ||
			    *rwData != *a++ ||
			    *rwData != *a++ ||
			    *rwData != *a++ )
				goto Err;
		} while( --i != 0 );
		switch( count1 ) {
		    case 3:
			if( *rwData != *a++ )
				goto Err;
		    case 2:
			if( *rwData != *a++ )
				goto Err;
		    case 1:
			if( *rwData != *a++ )
				goto Err;
		    case 0: ;
		}
		rwwcerr = 0;
		goto Ok;
	Err:
		rwwcerr = 1;
		skips = savea - a;
	Ok:
		if( skips ) {
			a = &cnt;
			i = skips;
			do {
				*a = *rwData;
			} while( --i != 0 );
		}
#endif RWWCHK
	} else {
		do {
			*rwData = *a++;
			*rwData = *a++;
			*rwData = *a++;
			*rwData = *a++;
		} while( --i != 0 );
		switch( count1 ) {
		    case 3:
			*rwData = *a++;
		    case 2:
			*rwData = *a++;
		    case 1:
			*rwData = *a++;
		    case 0: ;
		}
		if( skips ) {
			i = skips;
			do {
				*rwData = 0;
			} while( --i != 0 );
		}
		RB(I85_CR2) = SET_IE | slot;
	}

	restorseg5(save5);
	rwactive = 0;
}

/*
 * ������ ������ "A"
 *   '�������� ���������'
 */
rwintr()
{
	register struct rwdevice *rwaddr;
	register struct buf *bp;
	int     cnt, slot;
	static rwERR, rwRKS2;
	static char *rwSTRING;

	if (rwtab.b_active == NULL)
		return;
	rwaddr = RWADDR[rwdev];
	slot = RWSLOT(rwaddr);
	RB(I85_CR2) = RES_IE | slot;
	RB(I85_CR3) = RES_IE | slot;
	rwactive = 0;
#ifdef  RW_DKN
	dk_busy &= ~(1 << RW_DKN);
#endif  RW_DKN
	bp = rwtab.b_actf;
	rwtab.b_active = NULL;

	if ( (rwaddr->rw_rks2 & 020400) || rwwcerr ) {
		if( rwtab.b_errcnt == 0 ) {
			rwERR    = rwaddr->rw_err;
			rwRKS2   = rwaddr->rw_rks2;
			rwSTRING = rwwchk? (rwwcerr? " wchk-data\n" : " wchk-csum\n") : "\n";
		}
		rwwchk = rwwcerr = 0;
		rwaddr->rw_stat = 010;  /* RESET THE CONTROLLER */
		cnt = 0;
		while ((rwaddr->rw_stat & 0100000) && --cnt) {
			cnt++; cnt--; cnt++; cnt--;
		}
		cnt = rwaddr->rw_rid;
		if (++rwtab.b_errcnt <= ERR_TRY) {
			rwstart(1);
			return;
		}
#ifdef UCB_DEVERR
		harderr(bp, "rw");
		printf( "er=%o ds2=%o %s", rwERR, rwRKS2, rwSTRING );
#else
		deverror(bp, rwERR, rwRKS2);
		printf( rwSTRING );
#endif
		bp->b_flags |= B_ERROR;
	}
	if( rwbytec > 512 ) {
		rwbytec -= 512;
		if( (rw_addr+512) < rw_addr )
			rwxmem++;
		rw_addr += 512;
		rwstart(2);
		return;
	}
#ifdef RWWCHK
	if( !(bp->b_flags & (B_READ|B_ERROR)) && !rwwchk ) {
		rwwchk++;
		rwstart(0);
		return;
	}
	rwwchk = 0;
#endif RWWCHK
#ifdef UCB_DEVERR
	if( rwtab.b_errcnt > SOFT_TRY && !(bp->b_flags&B_ERROR) ) {
		harderr(bp, "rw"); printf(" (soft, er=%o)\n", rwERR);
	}
#endif
	rwtab.b_errcnt = 0;
	rwtab.b_actf = bp->av_forw;
	bp->b_resid = 0;
	iodone(bp);
	rwstart(0);
}

rwread(dev)
dev_t   dev;
{
	physio(rwstrategy, &rrwbuf, dev, B_READ);
}

rwwrite(dev)
dev_t   dev;
{
	physio(rwstrategy, &rrwbuf, dev, B_WRITE);
}
#endif  NRW
