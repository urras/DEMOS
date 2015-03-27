/*
char id_iargc[] = "@(#)iargc_.c	1.1";
 *
 * return the number of args on the command line following the command name
 *
 * calling sequence:
 *	nargs = iargc()
 * where:
 *	nargs will be set to the number of args
 */

#include	"../libI77/fiodefs.h"

extern int xargc;

ftnint iargc_()
{
	return ((ftnint)(xargc - 1));
}
