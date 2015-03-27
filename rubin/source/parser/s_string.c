# include <defines.h>
# include "scanner.h"
# include	<sccs.h>
# include <datetime.h>

SCCSID(@(#)s_string.c	7.2	5/4/83)

/*
** STRING
** A string is defined as any sequence of MAXSTRING or fewer characters,
** surrounded by string delimiters.  New-line ;characters are purged
** from strings unless preceeded by a '\'; QUOTE's must be similarly
** prefixed in order to be correctly inserted within a string.  Each
** string is entered in the symbol table, indexed by 'yylval'.  A
** token or the error condition -1 is returned.
*/
string(op)
struct optab	*op;
{
	extern char	*yylval;
	extern char	*syment();
	register int	esc;
	register int	save;
	register char	*ptr;
	char		buf[MAXSTRING + 1];
	long l;
	int tkn,tkn1;
	/* disable case conversion and fill in string */
	ptr = buf;
	save = Lcase;
	Lcase = 0;
	tkn = op->token;
	tkn1 = 0;
	do
	{
		/* get next character */
		if ((*ptr = get_scan(NORMAL)) == 0)
		{
			Lcase = save;
			/* non term string */
			par_error(STRTERM, FATAL, 0);
		}
		if (tkn == Tokens.dateconst && *ptr == ':')
			tkn1 = Tokens.timeconst;
		/* handle escape characters */
		esc = (*ptr == '\\');
		if (*ptr == '\n')
		{
			if ((*ptr = get_scan(NORMAL)) == 0)
			{
				Lcase = save;
				*ptr = 0;
				/* non term string */
				par_error(STRTERM, FATAL, 0);
			}
		}
		if (esc == 1)
		{
			if ((*++ptr = get_scan(NORMAL)) == 0)
			{
				Lcase = save;
				*ptr = 0;
				/* non term string */
				par_error(STRTERM, FATAL, 0);
			}
			if (*ptr == *(op->term))
				*--ptr = *(op->term);
		}

		/* check length */
		if ((ptr - buf) > MAXSTRING - 1)
		{
			Lcase = save;
			/* string too long */
			par_error(STRLONG, WARN, 0);
		}
		if (CTYPE(*ptr) == CNTRL)
			/* cntrl in string from equel */
			par_error(CNTRLCHR, WARN, 0);
	} while (*ptr++ != *(op->term) || esc == 1);

	/* restore case conversion and return */
	*--ptr = '\0';
	Lcase = save;
#	ifdef	xSTR2
	tTfp(71, 8, "STRING: %s\n", buf);
#	endif
	if (tkn == Tokens.sconst)
		yylval = syment(buf, (ptr - buf) + 1);
	else
	{
		if ( tkn == Tokens.dateconst)
		{
			save = BADDATE;
			l      = char_date(buf,NULL);
			if (l < 0 && tkn1)
				tkn = tkn1;
			else
				goto Ready;
		}
		if (tkn == Tokens.timeconst)
		{
			/* Это же - признак типа ВРЕМЯ */
			save = BADTIME;
			l      = char_time(buf,NULL);
		}
		else
			syserr("Bad TOKEN %d in s_string\n",op->token);
Ready:
		if (l < 0) par_error(save,WARN,buf,0);
		yylval = syment( &l, 4);
	}
	Lastok.tok = yylval;
	Lastok.toktyp = op->token;
	if ( save == BADTIME)
		Lastok.toktyp = Tokens.timeconst;
	return (Lastok.toktyp);
}
