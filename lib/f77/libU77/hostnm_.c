/*
 * hostnm - return this machines hostname
 *	@(#)hostnm_.c	1.1
 *
 * synopsis:
 *	integer function hostnm (name)
 *	character(*) name
 *
 * where:
 *	name	will receive the host name
 *	The returned value will be 0 if successful, an error number otherwise.
 */

#include	"../libI77/fiodefs.h"

extern int	errno;

ftnint
hostnm_ (name, len)
char	*name;
ftnlen	len;
{
	char	buf[64];
	register char	*bp;
	int	blen	= sizeof buf;

	if (gethostname (buf, blen) == 0)
	{
		b_char (buf, name, len);
		return ((ftnint) 0);
	}
	else
		return((ftnint)errno);
}
