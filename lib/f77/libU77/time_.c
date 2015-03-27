/*
char id_time[] = "@(#)time_.c	1.1";
 *
 * return the current time as an integer
 *
 * calling sequence:
 *	integer time
 *	i = time()
 * where:
 *	i will receive the current GMT in seconds.
 */

#include	<sys/types.h>
#include	"../libI77/fiodefs.h"

time_t time();

time_t time_()
{
	return(time((time_t *) 0));
}
