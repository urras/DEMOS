/*
char id_fstat[] = "@(#)fstat_.c	1.4";
 *
 * get file status
 *
 * calling sequence:
 *	integer fstat, statb(11)
 *	call fstat (name, statb)
 * where:
 *	'statb' will receive the stat structure for file 'name'.
 */

#include	<sys/types.h>
#include	<sys/stat.h>
#include	"../libI77/fiodefs.h"

extern unit units[];

ftnint fstat_(lunit, stbuf)
ftnint *lunit, *stbuf;
{
	struct stat statb;

	if (*lunit < 0 || *lunit >= MXUNIT)
		return((ftnint)(errno=F_ERUNIT));
	if (!units[*lunit].ufd)
		return((ftnint)(errno=F_ERNOPEN));
	if (fstat(fileno(units[*lunit].ufd), &statb) == 0)
	{
		*stbuf++ = statb.st_dev;
		*stbuf++ = statb.st_ino;
		*stbuf++ = statb.st_mode;
		*stbuf++ = statb.st_nlink;
		*stbuf++ = statb.st_uid;
		*stbuf++ = statb.st_gid;
		*stbuf++ = statb.st_rdev;
		*stbuf++ = statb.st_size;
		*stbuf++ = statb.st_atime;
		*stbuf++ = statb.st_mtime;
		*stbuf++ = statb.st_ctime;
		return((ftnint) 0);
	}
	return ((ftnint)errno);
}
