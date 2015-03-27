/*
char id_getuid[] = "@(#)getuid_.c	1.1";
 *
 * get user id
 *
 * calling sequence:
 *	integer getuid, uid
 *	uid = getuid()
 * where:
 *	uid will be the real user id
 */

#include	"../libI77/fiodefs.h"

ftnint getuid_()
{
	return((ftnint)getuid());
}
