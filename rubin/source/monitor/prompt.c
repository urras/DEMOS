# include	"monitor.h"
# include	<defines.h>
# include	<aux.h>
# include	<sccs.h>

SCCSID(@(#)prompt.c	7.1	2/5/81)



/*
**  OUTPUT PROMPT CHARACTER
**
**	The prompt is output to the standard output.  It will not be
**	output if -ss mode is set or if we are not at a newline.
**
**	The parameter is printed out if non-zero.
**
**	Uses trace flag 14
*/

prompt(msg)
char	*msg;
{
	if (!Prompt || GiveEof)
		return;
	if (Nodayfile >= 0)
	{
		if (msg)
			printf("\07%s\n", msg);
		printf(SQL_flag ==0?"* ":(Sql?"sql> ":"quel> "));
	}
	fflush(stdout);
}


/*
**  PROMPT WITH CONTINUE OR GO
*/

cgprompt()
{
	prompt(Notnull ? "continue" : "go");
}
