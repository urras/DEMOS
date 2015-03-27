/*
 * $Log:	dsort.c,v $
 * Revision 22.2  90/11/12  19:06:08  root
 * Новые вещи для СМ1425 и перемещение include.
 * 
 * Revision 22.1  89/04/12  14:20:11  korotaev
 * "param.h" ==> <sys/param.h>
 * 
 * Revision 22.0  89/03/25  12:23:21  korotaev
 * Отсюда начинается версия 2.2
 * 
 * Revision 1.1  86/04/19  17:54:06  avg
 * Initial revision
 * 
 */

/*
 * generalized seek sort for disk
 */

#include <sys/param.h>
#include <sys/systm.h>
#include "../include/buf.h"

disksort(dp, bp)
register struct buf *dp, *bp;
{
	register struct buf *ap;
	struct buf *tp;

	ap = dp->b_actf;
	if(ap == NULL) {
		dp->b_actf = bp;
		dp->b_actl = bp;
		bp->av_forw = NULL;
		return;
	}
	tp = NULL;
	for(; ap != NULL; ap = ap->av_forw) {
		if ((bp->b_flags&B_READ) && (ap->b_flags&B_READ) == 0) {
			if (tp == NULL)
				tp = ap;
			break;
		}
		if ((bp->b_flags&B_READ) == 0 && (ap->b_flags&B_READ))
			continue;
		if(ap->b_cylin <= bp->b_cylin)
			if(tp == NULL || ap->b_cylin >= tp->b_cylin)
				tp = ap;
	}
	if(tp == NULL)
		tp = dp->b_actl;
	bp->av_forw = tp->av_forw;
	tp->av_forw = bp;
	if(tp == dp->b_actl)
		dp->b_actl = bp;
}
