/*
char id_chdir[] = "@(#)chdir_.c	1.3";
 *
 * change default directory
 *
 * calling sequence:
 *	integer chdir
 *	ierror = chdir(dirname)
 * where:
 *	ierror will receive a returned status (0 == OK)
 *	dirname is the directory name
 */

#include	"../libI77/fiodefs.h"
#include	<sys/param.h>
#ifndef	MAXPATHLEN
#define MAXPATHLEN	128
#endif

ftnint chdir_(dname, dnamlen)
char *dname;
ftnlen dnamlen;
{
	char buf[MAXPATHLEN];

	if (dnamlen >= sizeof buf)
		return((ftnint)(errno=F_ERARG));
	g_char(dname, dnamlen, buf);
	if (chdir(buf) != 0)
		return((ftnint)errno);
	return((ftnint) 0);
}
