/*
 * $Log:	dkleave.c,v $
 * Revision 22.2  90/11/12  19:05:51  root
 * Новые вещи для СМ1425 и перемещение include.
 * 
 * Revision 22.1  89/04/12  14:19:25  korotaev
 * "param.h" ==> <sys/param.h>
 * 
 * Revision 22.0  89/03/25  12:22:59  korotaev
 * Отсюда начинается версия 2.2
 * 
 * Revision 1.1  86/04/19  17:54:03  avg
 * Initial revision
 * 
 */

#include <sys/param.h>
#include "../include/buf.h"

#ifdef  INTRLVE

daddr_t
dkblock(bp)
register struct buf *bp;
{
	register int dminor;

	if (((dminor=minor(bp->b_dev))&0100) == 0)
		return(bp->b_blkno);
	dminor >>= 3;
	dminor &= 07;
	dminor++;
	return(bp->b_blkno/dminor);
}

dkunit(bp)
register struct buf *bp;
{
	register int dminor;

	dminor = minor(bp->b_dev) >> 3;
	if ((dminor&010) == 0)
		return(dminor);
	dminor &= 07;
	dminor++;
	return(bp->b_blkno%dminor);
}

#endif	INTRLVE
