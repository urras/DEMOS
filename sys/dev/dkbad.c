/*
 * $Log:	dkbad.c,v $
 * Revision 22.2  90/11/12  19:05:44  root
 * ����� ���� ��� ��1425 � ����������� include.
 * 
 * Revision 22.1  89/04/12  14:19:08  korotaev
 * "param.h" ==> <sys/param.h>
 * 
 * Revision 22.0  89/03/25  12:22:50  korotaev
 * ������ ���������� ������ 2.2
 * 
 * Revision 1.1  86/04/19  17:54:01  avg
 * Initial revision
 * 
 */

#include <sys/param.h>
#ifdef BADSECT
#include "../include/buf.h"
#include "../include/dkbad.h"

/*
 * Search the bad sector table looking for
 * the specified sector.  Return index if found.
 * Return -1 if not found.
 */

isbad(bt, cyl, trk, sec)
	register struct dkbad *bt;
{
	register int i;
	register long blk, bblk;

	blk = ((long)cyl << 16) + (trk << 8) + sec;
	for (i = 0; i < MAXBAD; i++) {
		bblk = ((long)bt->bt_bad[i].bt_cyl << 16) + bt->bt_bad[i].bt_trksec;
		if (blk == bblk)
			return (i);
		if (blk < bblk || bblk < 0)
			break;
	}
	return (-1);
}
#endif

