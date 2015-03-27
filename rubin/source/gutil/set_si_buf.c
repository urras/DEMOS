# include	<stdio.h>
# include	<defines.h>
# include	<sccs.h>

SCCSID(@(#)set_si_buf.c	7.1	2/5/81)

/*
**  SET_SI_BUF -- set standard input buffer unconditionally
**
**	This routine sets the standard input buffer to give
**	buffered input.  The buffer is contained internally.
**
**	Parameters:
**		none
**
**	Returns:
**		TRUE.
**
**	Side Effects:
**		The standard input is left buffered.
**
**	Trace Flags:
**		none
*/
/*
 * Смысла тут мало, так как stdio само все что надо буферизует
 */
set_si_buf()
{
#if 0
	static char	buffer[BUFSIZ];

	setbuf(stdin, buffer);
#endif
	return (TRUE);
}
