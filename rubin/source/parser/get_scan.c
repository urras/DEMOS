# include	<defines.h>
# include	"scanner.h"
# include	<sccs.h>
# include       <code.h>

SCCSID(@(#)get_scan.c	7.2	5/4/83)

/*
** GET_SCAN -- gets characters from monitor
**
**	Parameters:
**		mode --
**	   	    modes are:
**			NORMAL = read normally
**			PRIME = prime the pipe
**			SYNC = sync (or flush) the pipe
**
**	Returns:
**		character or '\0' on eof
**
**	Trace Flags:
**		Getscan ~~ 54.0
*/

get_scan(mode)
int	mode;
{
	extern int		yyline;
	register int		ctr;
	char                    c;
	int cc;
	extern int		Pctr;		/* vble for backup stack in scanner */
	extern char		Pchar[2];
# ifdef	xPTR3
	tTfp(54, 0, "get_scan: mode %d ", mode);
# endif
	switch (mode)
	{
	    case NORMAL:
		if (Pctr)
		{
			c = Pchar[--Pctr]&0377;
			ctr = 1;
		}
		else
			ctr = readmon(&c, 1);
		if (c == '\n')
			yyline++;
		cc = c&0377;
		c = (Lcase? to_lower(cc):cc) ;

		break;

	    case PRIME:
		Pctr = 0;
		ctr = 0;
		break;

	    case SYNC:				/* flush pipe */
		while (readmon(&c, 1) > 0);
		ctr = 0;
		break;

	    default:
		syserr("bad arg '%d' in get_scan", mode);
	}

# ifdef	xPTR3
	tTfp(54, 1, " ctr %d: '%c' (0%o).\n", ctr & 0377, c, c);
# endif

	return (ctr ? ( (int)c&0377) : 0);
}
