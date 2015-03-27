/*
char id_alarm[] = "@(#)alarm_.c	1.1";
 *
 * set an alarm time, arrange for user specified action, and return.
 *
 * calling sequence:
 *	integer	flag
 *	external alfunc
 *	lastiv = alarm (intval, alfunc)
 * where:
 *	intval	= the alarm interval in seconds; 0 turns off the alarm.
 *	alfunc	= the function to be called after the alarm interval,
 *
 *	The returned value will be the time remaining on the last alarm.
 */

#include	"../libI77/fiodefs.h"
#include	<signal.h>

ftnint alarm_(sec, proc)
ftnint	*sec;
int	(* proc)();
{
	register ftnint	lt;

	lt = (ftnint) alarm(1000);	/* time to maneuver */

	if (*sec)
		signal(SIGALRM, proc);

	alarm((unsigned) *sec);
	return(lt);
}
