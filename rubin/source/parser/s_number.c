# include	<defines.h>
# include	<aux.h>
# include	"scanner.h"
# include	<sccs.h>

SCCSID(@(#)s_number.c	7.2	5/4/83)

/*
** NUMBER
**	scans numerical constants (both integer and floating).  Each
**	constant is converted from ascii to its numerical representation
**	and is entered into the symbol table, indexed by 'yylval'.
**	A token is returned for the number type.
**
**      due to the current Atof in the utility library, floating overflow
**	is not checked.
*/
number(chr)
char	chr;
{
	extern int	yylval;
	double		ftemp;
	long		ltemp;
	int		itemp;
	char		buf[256];
	register int	lsave;
	register char	*ptr;

	lsave = Lcase;
	Lcase = 0;
	ptr = buf;
	if ((*ptr = chr) != '.')
	{
		do
		{
			/* get integer portion */
			if ((ptr - buf) >= 256)
				/* buffer overflow */
				par_error(NUMBUFOFLO, WARN, 0);
			*++ptr = get_scan(NORMAL);
		} while (CTYPE(*ptr) == NUMBR);
	}

	/* do rest of type determination */
	switch (*ptr)
	{
	  case '.':
		/* floating point */
		do
		{
			/* fill into ptr with up to next non-digit */
			if ((ptr - buf) >= 256)
				par_error(NUMBUFOFLO, WARN, 0);	/* buf oflo */
			*++ptr = get_scan(NORMAL);
		} while (CTYPE(*ptr) == NUMBR);
		if (*ptr != 'e' && *ptr != 'E')
		{
			backup(*ptr);
			*ptr = 0;
			goto convr;
		}

	  case 'e':
	  case 'E':
		if ((ptr - buf) >= 256)
			par_error(NUMBUFOFLO, WARN, 0);	/* buf oflo */
		*++ptr = get_scan(NORMAL);
		if (CTYPE(*ptr) == NUMBR || *ptr == '-' || *ptr == '+')
		{
			do
			{
				/* get exponent */
				if ((ptr - buf) >= 256)
					par_error(NUMBUFOFLO, WARN, 0);	/* buf oflo */
				*++ptr = get_scan(NORMAL);
			} while (CTYPE(*ptr) == NUMBR);
		}
		backup(*ptr);
		*ptr = 0;
	convr:
		if (Atof(buf, &ftemp))
			par_error(FCONSTERR, WARN, buf, 0);	/* floating conversion error */
		yylval = syment(&ftemp, 8);
		Lastok.toktyp = Tokens.f8const;
		break;

	  default:
		/* integer */
		backup(*ptr);
		*ptr = 0;
		if (atol(buf, &ltemp))	/* long conversion error */
			goto convr;
		if (ltemp > 32767)
		{
			yylval = syment(&ltemp, 4);
			Lastok.toktyp = Tokens.i4const;
			break;
		}
		itemp = ltemp;
		{
			short sval;
			sval = itemp;
			yylval = syment(&sval, 2);
		}
		Lastok.toktyp = Tokens.i2const;
		break;
	}
	Lcase = lsave;
	Lastok.tok = (char *) yylval;
	Lastok.tokop = 0;
	return (Lastok.toktyp);
}
