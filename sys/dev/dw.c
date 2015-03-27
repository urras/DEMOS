/*
 * Драйвер Винчестера ДВК-4 по мотивам драйвера RW(avg)
 * Чечик И.Е. & Co
 * Вариант с одним контроллером и винчестером 5M
 * $Log:	dw.c,v $
 * Revision 1.1  90/12/12  16:43:06  korotaev
 * Правки для ДВК
 * 
 * Revision 1.1  89/01/23  15:05:25  chech
 * Initial revision
 * 
 */

#include "h/dw.h"
#if     NDW > 0
#include <sys/param.h>
#include <sys/systm.h>
#include "../include/buf.h"
#include <sys/conf.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/seg.h>
#include "../include/iopage.h"
#include "../include/dwreg.h"

#define DWTRYS 10                       /* число повторов по ошибке */
#define DWRESET 25                      /* таймаут по неготовности */

#ifdef DW_SPL
#define SPLL    _spl1 ()
#define SPLM    _spl5 ()
#else
#define SPLL    /* */
#define SPLM    /* */
#endif

struct  dwdevice *DWADDR;

struct  buf     dwtab;
struct  buf     rdwbuf;

/*
 * A size of -1 says the partition extends to the end of
 * the disk.
 */
#define DWSEC   16
#define DWTRK   4
#define DWCYL   154
#define NDWBLK  (DWCYL*DWSEC*DWTRK)     /* размер диска */
struct dwsize {
	daddr_t nblocks;
	daddr_t blkoff;
}dw_sizes[] = {         /* size of part must be devided by DWSEC*DWTRK */
	 7168,  0,              /* A=cil 0-111 (part1 10Mb disk) */
	12480,  7168,           /* B=cil 112-307 (part2 10Mb disk) */
	34688,  7168,           /* C=cil 112-653 (part2 20Mb disk) */
	-1,     7168,           /* D=cil 112-end (part2 unknownMb disk) */
	9856,   0,              /* E=cil 0-153 (full 5Mb disk) */
	19648,  0,              /* F=cil 0-307 (full 10Mb disk) */
	41856,  0,              /* G=cil 0-654 (full 20 Mb disk) */
	-1,     0,              /* H=cil 0-end */
};

dwattach (addr, unit)
register struct dwdevice *addr;
register unit;
{
	if (unit != 0)
		return(0);
	DWADDR = addr;
	return(1);
}

dwstrategy (bp)
register struct buf *bp;
{
	register s;
	register struct buf *dp;
	daddr_t  msz;

	if (DWADDR == (struct dwdevice *) NULL) {
		bp->b_error = ENXIO;
		goto errexit;
	}
	s = bp->b_dev & 07;
	msz = dw_sizes[s].nblocks;
	if( bp->b_blkno < 0 ||
	   (msz > 0 && bp->b_blkno+((bp->b_bcount+511)>>9) >= msz) ){
		bp->b_error = EINVAL;
errexit:
		bp->b_flags |= B_ERROR;
		iodone (bp);
		return;
	}

	bp->av_forw = (struct buf *) NULL;
	bp->b_cylin = (bp->b_blkno+dw_sizes[s].blkoff) / (DWSEC * DWTRK);

	s = spl5 ();
	dp = &dwtab;
	disksort (dp, bp);
	if(dwtab.b_active == NULL)
		dwstart (0);
	splx (s);
}

/*
 * Программные регистры для псевдо-DMA
 */
static unsigned dw_addr, dwbytec, dwxmem, dwisread;

/*
 * Запустить операцию на накопителе
 *
 *      0 - с блоком из bp->b_blkno (начало обмена)
 *      1 - с тем же номером блока  (повтор обмена)
 *      2 - со след. номером блока  (прод. обмена)
 */
dwstart (arg) register arg;
{
	register struct dwdevice *dwaddr = DWADDR;
	register struct buf *bp;
	static daddr_t bn;
	int    dummy;

	if ((bp = dwtab.b_actf) == NULL)
		return;


	/* CLEAR CONTROLLER */
	if (dwaddr->dw_stat & DWDS_BSY) {
		harderr (bp, "dw");
		printf ("not Ready, timeout\n");
		timeout (dwstart, (caddr_t) arg, DWRESET);
		return;
	}
	dummy = dwaddr->dw_sec;

	dwtab.b_active++;
	if (arg == 0) {
		bn = bp->b_blkno + dw_sizes[bp->b_dev&07].blkoff;
		dwbytec  = bp->b_bcount;
		dw_addr  = bp->b_un.b_addr;
		dwxmem   = bp->b_xmem;
		dwisread = ((bp->b_flags & B_READ) != 0);
	} else if (arg == 2)
		bn++;

	dwaddr->dw_cyl = (bn / (DWTRK * DWSEC));
	dwaddr->dw_trk = (bn / DWSEC) % DWTRK;
	dwaddr->dw_sec = bn % DWSEC;

#ifdef  DW_DKN
	dk_busy |= 1 << DW_DKN;
	dk_numb[DW_DKN]++;
	dk_wds[DW_DKN] += 512 >> 6;
#endif  DW_DKN

	dwaddr->dw_rks2 = dwisread ? DWCS_RCOM : DWCS_WCOM;
	if (!dwisread)
		dwpdma();
	dwaddr->dw_stat |= DWDS_IE;     /* разрешить прерывание */
}

#define PROTO5  ((15<<8) | RW)

/*
 * Запрос обмена "Б"
 *   'требование данных'
 */
dwpdma ()
{
	register struct dwdevice *dwaddr = DWADDR;
	register *a, i;
	segm    save5;
	int     cnt, j, s;

	saveseg5 (save5);
	mapseg5 ((dwxmem << 10) | ((dw_addr >> 6) & 01777), PROTO5);
	s = spl1 ();
	a = (dw_addr & 077) | SEG5;
	i = dwbytec/2;
	if (i > 256)
		i = 256;
	j = i;
	if (dwisread) {
		do {
			cnt = -1;
			while (!(dwaddr->dw_stat&DWDS_ZOA) && --cnt);
			*a++ = dwaddr->dw_data;
		} while (--i != 0);
		if( (i = 256-j) > 0 ) {
			do {
				cnt = -1;
				while (!(dwaddr->dw_stat&DWDS_ZOA) && --cnt);
				cnt = dwaddr->dw_data;
			} while (--i != 0);
		}
	} else {
		do {
			cnt = -1;
			while (!(dwaddr->dw_stat&DWDS_ZOA) && --cnt);
			dwaddr->dw_data = *a++;
		} while (--i != 0);
		if ((i = 256-j) > 0) {
			do {
				cnt = -1;
				while (!(dwaddr->dw_stat&DWDS_ZOA) && --cnt);
				dwaddr->dw_data = 0;
			} while (--i != 0);
		}
	}
	splx (s);
	restorseg5 (save5);
}

/*
 * Запрос обмена "A"
 *   'операция завершена'
 */
dwintr ()
{
	register struct dwdevice *dwaddr = DWADDR;
	register struct buf *bp;
	int     cnt;

	dwaddr->dw_stat &= ~DWDS_IE;    /* закрыть прерывание */
	if ((dwtab.b_active == NULL) || (DWADDR == NULL))
		return;
#ifdef  DW_DKN
	dk_busy &= ~(1 << DW_DKN);
#endif  DW_DKN
	bp = dwtab.b_actf;
	dwtab.b_active = NULL;

	SPLL;
	if (dwaddr->dw_rks2 & (DWCS_WRE|DWCS_ERR)) {
#ifdef UCB_DEVERR
		harderr (bp, "dw");
		printf ("er=%b ds2=%b\n", dwaddr->dw_err, DWER_BITS,
			dwaddr->dw_rks2, DWCS_BITS);
#else
		deverror(bp, dwaddr->dw_err, dwaddr->dw_rks2);
#endif
		if (++dwtab.b_errcnt <= DWTRYS) {
			SPLM;
			/* RESET THE CONTROLLER */
			dwaddr->dw_stat = DWDS_INI;
			cnt = 0;
			while ((dwaddr->dw_stat & DWDS_BSY) && --cnt) {
				cnt++; cnt--; cnt++; cnt--;
			}
			cnt = dwaddr->dw_rid;
			dwstart (1);
			return;
		}
		bp->b_flags |= B_ERROR;
	}
	else {
		if (dwisread)
			dwpdma ();
		if (dwbytec > 512) {
			dwbytec -= 512;
			bp->b_resid = dwbytec;  /* новый b_resid */
			if ((dw_addr+512) < dw_addr)
				dwxmem++;
			dw_addr += 512;
			SPLM;
			dwstart (2);
			return;
		}
		bp->b_resid = 0;                /* конец операции */
	}
	SPLM;
	dwtab.b_errcnt = 0;
	dwtab.b_actf = bp->av_forw;
	SPLL;
	iodone (bp);
	SPLM;
	dwstart (0);
}

dwread (dev)
register dev_t dev;
{
	physio (dwstrategy, &rdwbuf, dev, B_READ);
}

dwwrite (dev)
register dev_t dev;
{
	physio (dwstrategy, &rdwbuf, dev, B_WRITE);
}
#endif  NDW
