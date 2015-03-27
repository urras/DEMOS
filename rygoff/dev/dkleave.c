/*
 * $Log:	dkleave.c,v $
 * Revision 1.1  86/04/19  17:54:03  avg
 * Initial revision
 * 
 */

#include "param.h"
#include <sys/buf.h>

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
