/*
char id_stat[] = "@(#)lstat_.c	1.3";
 *
 * get file status
 *
 * calling sequence:
 *	integer lstat, statb(11)
 *	external lstat
 *	ierr = lstat (name, statb)
 * where:
 *	'statb' will receive the stat structure for file 'name'.
 */

#include	<sys/param.h>
#ifndef	MAXPATHLEN
#define MAXPATHLEN	128
#endif
#include	<sys/stat.h>
#include	"../libI77/fiodefs.h"

ftnint lstat_(name, stbuf, namlen)
char *name; ftnint *stbuf; ftnlen namlen;
{
	char buf[MAXPATHLEN];
	struct stat statb;

	if (namlen >= sizeof buf)
		return((ftnint)(errno=F_ERARG));
	g_char(name, namlen, buf);
	if (lstat(buf, &statb) == 0)
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
