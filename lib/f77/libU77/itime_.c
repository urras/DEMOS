/*
char id_itime[] = "@(#)itime_.c	1.1";
 *
 * return the current time in numerical form
 *
 * calling sequence:
 *	integer iarray(3)
 *	call itime(iarray)
 * where:
 *	iarray will receive the current time; hour, min, sec.
 */

#include	"../libI77/fiodefs.h"
#include	<sys/types.h>
#include	<time.h>

itime_(iar)
struct { ftnint ihr; ftnint imin; ftnint isec; } *iar;
{
	struct tm *localtime(), *lclt;
	time_t time(), t;

	t = time(0);
	lclt = localtime(&t);
	iar->ihr = (ftnint) lclt->tm_hour;
	iar->imin = (ftnint) lclt->tm_min;
	iar->isec = (ftnint) lclt->tm_sec;
}
