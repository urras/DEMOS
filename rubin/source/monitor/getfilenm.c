# include	"monitor.h"
# include	<defines.h>
# include	<aux.h>
# include	<sccs.h>

SCCSID(@(#)getfilenm.c	7.1	2/5/81)



/*
**  GET FILE NAME
**
**	This routine collects a file name up to a newline and returns a
**	pointer to it.  Keep in mind that it is stored in a static
**	buffer.
**      FLAG=1 - not expand macro
**	Trace Flags:
**		40
*/

char *
getfilenm(no_macro)
{
	static char	filename[81];
	register short   c;
	register int	i;
	register char	*p;
	extern   getch(), macgetch();
	int (*get)();
	Oneline = TRUE;
	if (no_macro)
		get = getch;
	else
	{
		macinit(getch, 0, 0);
		get = macgetch;
	}
	/* skip initial spaces */
	while ((c = (*get)()) == ' ' || c == '\t')
		continue;

	i = 0;
	for (p = filename; c > 0; )
	{
		if (i++ <= 80)
			*p++ = c;
		c = (*get)();
	}
	*p = '\0';
	Prompt = Newline = TRUE;

#	ifdef xMTR2
	if (tTf(40, 0))
		printf("filename \"%s\"\n", filename);
#	endif
	Oneline = FALSE;
	getc(Input);
	return (filename);
}
