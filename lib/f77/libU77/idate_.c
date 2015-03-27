/*
char id_idate[] = "@(#)idate_.c	1.1";
 *
 * return date in numerical form
 *
 * calling sequence:
 *	integer iarray(3)
 *	call idate(iarray)
 * where:
 *	iarray will receive the current date; day, mon, year.
 */

#include	"../libI77/fiodefs.h"
#include	<sys/types.h>
#include	<time.h>

idate_(iar)
struct { ftnint iday; ftnint imon; ftnint iyer; } *iar;
{
	struct tm *localtime(), *lclt;
	time_t time(), t;

	t = time(0);
	lclt = localtime(&t);
	iar->iday = (ftnint) lclt->tm_mday;
	iar->imon = (ftnint) lclt->tm_mon + 1;
	iar->iyer = (ftnint) lclt->tm_year + 1900;
}
