/*
char id_unlink[] = "@(#)unlink_.c	1.2";
 *
 * unlink (remove) a file
 *
 * calling sequence:
 *	integer unlink
 *	ierror = unlink(filename)
 * where:
 *	ierror will be a returned status (0 == OK)
 *	filename is the file to be unlinked
 */

#include	"../libI77/fiodefs.h"
#include	<sys/param.h>
#ifndef	MAXPATHLEN
#define MAXPATHLEN	128
#endif

ftnint
unlink_(fname, namlen)
char *fname;
ftnlen namlen;
{
	char buf[MAXPATHLEN];

	if (namlen >= sizeof buf)
		return((ftnint)(errno=F_ERARG));
	g_char(fname, namlen, buf);
	if (unlink(buf) != 0)
		return((ftnint)errno);
	return((ftnint) 0);
}
