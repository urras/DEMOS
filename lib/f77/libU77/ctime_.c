/*
char id_ctime[] = "@(#)ctime_.c	1.1";
 *
 * convert system time to ascii string
 *
 * calling sequence:
 *	character*24 string, ctime
 *	integer clock
 *	string = ctime (clock)
 * where:
 *	string will receive the ascii equivalent of the integer clock time.
 */

#include	"../libI77/fiodefs.h"
#include	<sys/types.h>

char *ctime();

ctime_(str, len, clock)
char *str; ftnlen len; time_t *clock;
{
	char *s = ctime(clock);
	s[24] = '\0';
	b_char(s, str, len);
}
