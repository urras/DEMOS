/*
 * Драйвер виртуального диска для
 * СМ-1420 с большой памятью.
 *
 * размер памяти в Kб для вирт. диска задается константой VDSIZE.
 */
#include "h/vd.h"
#if     NVD > 0
#include <sys/param.h>
#include <sys/systm.h>
#include "../include/buf.h"
#include <sys/conf.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/seg.h>
#include "../include/iopage.h"
#include "../include/map.h"

struct  buf     vdtab;
struct  buf     rvdbuf;

memaddr         vdaddr = NULL;

vdattach(addr, unit)
char    *addr;
{
	if (unit != 0)
		return(0);
	return(1);
}

vdroot()
{
	/* Заказать память для виртуального диска */

#define B  (size_t)(((long)(VDSIZE) * (bsize)) / ctob(1))
	vdaddr = malloc(coremap, B);
	printf( "virtual disk=%dK\n", VDSIZE );
#undef B
}

vdstrategy(bp)
register struct buf *bp;
{
	register s;

	if ( vdaddr == NULL ) {
		bp->b_error = ENXIO;
		goto errexit;
	}
	if (bp->b_blkno >= (VDSIZE*2l) || minor(bp->b_dev) ) {
		bp->b_error = EINVAL;
errexit:
		bp->b_flags |= B_ERROR;
		iodone(bp);
		return;
	}
	bp->av_forw = (struct buf *)NULL;
	s = spl5();
	if(vdtab.b_actf == NULL)
		vdtab.b_actf = bp;
	else
		vdtab.b_actl->av_forw = bp;
	vdtab.b_actl = bp;
	splx(s);

	/* КОПИРОВАНИЕ */
	vcopy(bp, vdaddr);

	bp = vdtab.b_actf;
	vdtab.b_errcnt = 0;
	vdtab.b_actf = bp->av_forw;
	bp->b_resid = 0;
	iodone(bp);
}

#define PROTO5  ((15<<8) | RW)

/*
 * Модуль копирования блока данных в/из адреса в bp в соотв. блок
 */
vcopy( bp, mem )
struct buf *bp;
memaddr mem;
{
	segm      seg5;
	memaddr   a1, a2;
	int       wcnt, iwc;
	register *o1, *o2, *p;
	int      *q, worka[64];
	int      *oo1, *oo2;


	saveseg5(seg5);

	/* COMPUTE ADDRESSES */
	wcnt = bp->b_bcount >> 1;

#ifdef VDDEBUG
printf( "vcopy: mem=%o wcnt=%d\n", mem, wcnt );
printf( "xmem=%o adr=%o flags=%o\n", bp->b_xmem, bp->b_un.b_addr, bp->b_flags );
#endif

	if( bp->b_flags & B_READ ) {
		oo1 = SEG5;
		a1  = (int)mem + (int)(bp->b_blkno*512/ctob(1));
		oo2 = (int)(bp->b_un.b_addr & 076) + (char *)SEG5;
		a2  = ((bp->b_un.b_addr >> 6) & 01777) | (bp->b_xmem<<10);
	} else {
		oo1 = (int)(bp->b_un.b_addr & 076) + (char *)SEG5;
		a1  = ((bp->b_un.b_addr >> 6) & 01777) | (bp->b_xmem<<10);
		oo2 = SEG5;
		a2  = (int)mem + (int)(bp->b_blkno*512/ctob(1));
	}

#ifdef VDDEBUG
printf( "a1=%o oo1=%o\n", a1, oo1 );
printf( "a2=%o oo2=%o\n", a2, oo2 );
#endif

	/* COPY IT */
	while( wcnt > 0 ) {
		o1 = oo1; o2 = oo2;
		iwc = wcnt;
		if( iwc > 64 ) iwc = 64;
		q = &worka[iwc];

		p = worka;
		mapseg5(a1, PROTO5);
		while( p < q )
			*p++ = *o1++;

		p = worka;
		mapseg5(a2, PROTO5);
		while( p < q )
			*o2++ = *p++;

		wcnt -= iwc;
		a1 += 2;  a2 += 2;       /* step - 2 clicks (64 words) */
	}

	restorseg5( seg5 );
}

vdread(dev)
dev_t   dev;
{
	physio(vdstrategy, &rvdbuf, dev, B_READ);
}

vdwrite(dev)
dev_t   dev;
{
	physio(vdstrategy, &rvdbuf, dev, B_WRITE);
}
#endif  NVD
