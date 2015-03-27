/*
char id_fgetc[] = "@(#)fgetc_.c	1.4";
 *
 * get a character from a logical unit bypassing formatted I/O
 *
 * calling sequence:
 *	integer fgetc
 *	ierror = fgetc (unit, char)
 * where:
 *	char will return a character from logical unit
 *	ierror will be 0 if successful; a system error code otherwise.
 */

#include	"../libI77/fiodefs.h"

extern unit units[];	/* logical units table from iolib */

ftnint fgetc_(u, c, clen)
ftnint *u; char *c; ftnlen clen;
{
	int	i;
	unit	*lu;

	if (*u < 0 || *u >= MXUNIT)
		return((ftnint)(errno=F_ERUNIT));
	lu = &units[*u];
	if (!lu->ufd)
		return((ftnint)(errno=F_ERNOPEN));
	if (lu->uwrt && ! nowreading(lu))
		return((ftnint)errno);
	if ((i = getc (lu->ufd)) < 0)
	{
		if (feof(lu->ufd))
			return((ftnint) -1);
		i = errno;
		clearerr(lu->ufd);
		return((ftnint)i);
	}
	*c = i & 0177;
	return((ftnint) 0);
}
