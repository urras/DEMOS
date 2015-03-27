/*
char id_getpid[] = "@(#)getpid_.c	1.1";
 *
 * get process id
 *
 * calling sequence:
 *	integer getpid, pid
 *	pid = getpid()
 * where:
 *	pid will be the current process id
 */

#include	"../libI77/fiodefs.h"

ftnint getpid_()
{
	return((ftnint)getpid());
}
