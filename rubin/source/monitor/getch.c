# include	"monitor.h"
# include	<defines.h>
# include	<aux.h>
# include	<sccs.h>

SCCSID(@(#)getch.c	7.1	2/5/81)



/*
**  GET CHARACTER
**
**	This routine is just a getchar, except it allows a pseudo-
**	EOF marker.
*/

int
getch()
{
	register int   c;

	if (GiveEof)
		c = '\0';
	else
		c = getc(Input);
	if (c < 0)
		c = '\0';

	/* deliver EOF if newline in Oneline mode */
	if (c == '\n' && Oneline)
	{
		ungetc(c, Input);
		c = '\0';
	}

	GiveEof = FALSE;
#			ifdef xMTR2
			if (tTf(33, 5))
				putchar(c);
#			endif
	return (c);
}
