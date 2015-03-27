# include <defines.h>
# include "scanner.h"
# include	<sccs.h>

SCCSID(@(#)s_inout.c	7.2	5/4/83)

/* TWO CHARACTER STACK FOR 'UNGETC' BACKUP */
char	Pchar[2];
int	Pctr;


/*
** BACKUP
** saves the character argument in the global stack 'Pchar'
**/
backup(chr)
char	chr;
{
	extern int	yyline;

	if (Pctr == 2)
		syserr("overflow in backup()");
	Pchar[Pctr++] = chr;
	if (chr == '\n')
		yyline--;
}
