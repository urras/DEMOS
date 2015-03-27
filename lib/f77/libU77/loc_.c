/*
char id_loc[] = "@(#)loc_.c	1.1";
 *
 * Return the address of the argument.
 *
 * calling sequence:
 *	iloc = loc (arg)
 * where:
 *	iloc will receive the address of arg
 */

#include	"../libI77/fiodefs.h"

ftnint loc_(arg)
ftnint *arg;
{
	return((ftnint)arg);
}
