/*
char id_gmtime[] = "@(#)gmtime_.c	1.1";
 *
 * return broken down time
 *
 * calling sequence:
 *	integer time, t[9]
 *	call gmtime(time, t)
 * where:
 *	time is a system time. (see time(3F))
 *	t will receive the broken down time assuming GMT.
 *	(see ctime(3))
 */

#include	<time.h>
#include	<sys/types.h>

struct tm *gmtime();

gmtime_(clock, t)
time_t *clock; struct tm *t;
{
	struct tm *g;

	g = gmtime(clock);
	*t = *g;
}
