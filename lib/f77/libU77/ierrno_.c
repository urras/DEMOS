/*
char id_ierrno[] = "@(#)ierrno_.c	1.1";
 *
 * return the current value of the system error register
 *
 * calling sequence:
 *	ier = ierrno()
 * where:
 *	ier will receive the current value of errno
 */

#include	"../libI77/fiodefs.h"

extern int errno;

ftnint ierrno_()
{
	return((ftnint)errno);
}
