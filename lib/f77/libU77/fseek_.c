/*
char id_fseek[] = "@(#)fseek_.c	1.2";
 *
 * position a file associated with a fortran logical unit
 *
 * calling sequence:
 *	ierror = fseek(lunit, ioff, ifrom)
 * where:
 *	lunit is an open logical unit
 *	ioff is an offset in bytes relative to the position specified by ifrom
 *	ifrom	- 0 means 'beginning of the file'
 *		- 1 means 'the current position'
 *		- 2 means 'the end of the file'
 *	ierror will be 0 if successful, a system error code otherwise.
 */

#include	"../libI77/fiodefs.h"
#include	<sys/types.h>

extern unit units[];

ftnint fseek_(lu, off, from)
ftnint *lu; off_t *off; ftnint *from;
{
	if (*lu < 0 || *lu >= MXUNIT)
		return((ftnint)(errno=F_ERUNIT));
	if (*from < 0 || *from > 2)
		return((ftnint)(errno=F_ERARG));
	if (!units[*lu].ufd)
		return((ftnint)(errno=F_ERNOPEN));
	if (fseek(units[*lu].ufd, (ftnint) *off, (int)*from) < 0)
		return((ftnint)errno);
	return((ftnint) 0);
}
