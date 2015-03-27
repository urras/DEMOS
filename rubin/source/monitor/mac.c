# include "code.h"
# include	<useful.h>
# include	<sccs.h>
# include       <trace.h>

# define TRACE if (tTf(70, 0)) printf

SCCSID(@(#)mac.c	7.1	2/5/81)

/*
**  MACRO PROCESSOR
*/


# define        ANYDELIM        '\020'          /* \| -- zero or more delims */
# define        ONEDELIM        '\021'          /* \^ -- exactly one delim */
# define        CHANGE          '\022'          /* \& -- token change */

# define        PARAMN          '\023'          /* $ -- non-preprocessed param */
# define        PARAMP          '\024'          /* $$ -- preprocessed param */

# define        PRESCANENABLE   '@'             /* character to enable prescan */
# define        LBRACE          '{'             /* left brace */
# define        RBRACE          '}'             /* right brace */
# define        BACKSLASH       '\\'            /* backslash */
# define        LQUOTE          '`'             /* left quote */
# define        RQUOTE          '\''            /* right quote */
# define        SPACE           ' '
# define        TAB             '\t'
# define        NEWLINE         '\n'

# define        QUOTED          0400            /* pass right through bit */
# define        BYTEMASK        0377            /* one byte */
# define        CHARMASK        0377            /* character part */

# define        ITERTHRESH      100             /* iteration limit */
# define        NPRIMS          (sizeof Macprims / sizeof Macprims[0])

/* token modes, used to compute token changes */
# define        NONE            0               /* guarantees a token change */
# define        ID              1               /* identifier */
# define        NUMBER          2               /* number (int or float) */
# define        DELIM           3               /* delimiter, guarantees a token change */
# define        QUOTEMODE       4               /* quoted construct */
# define        OP              5               /* operator */
# define        NOCHANGE        6               /* guarantees no token change */



# include       "buf.h"                 /* headers for buffer manip */


/* macro definitions */
struct macro
{
	struct macro    *nextm;         /* pointer to next macro header */
	CHAR            *template;      /* pointer to macro template */
	CHAR            *substitute;    /* pointer to substitution text */
};


/* Macros for primitive declaration */
CHAR PDM00[] = { '{','d','e','f','i','n','e',';',020,024,'t',';',020,024,'s','}', 0                                };
CHAR PDM01[] = { '{','r','a','w','d','e','f','i','n','e',';',020,024,'t',';',020,024,'s','}', 0                    };
CHAR PDM02[] = { '{','r','e','m','o','v','e',';',020,024,'t','}', 0                                                };
CHAR PDM03[] = { '{','d','u','m','p','}', 0                                                                        };
CHAR PDM04[] = { '{','t','y','p','e',020,024,'m','}', 0                                                            };
CHAR PDM05[] = { '{','r','e','a','d',020,024,'m','}', 0                                                            };
CHAR PDM06[] = { '{','r','e','a','d','d','e','f','i','n','e',';',020,024,'n',';',020,024,'m','}', 0                };
CHAR PDM07[] = { '{','i','f','s','a','m','e',';',020,024,'a',';',020,024,'b',';',020,023,'t',';',020,023,'f','}', 0};
CHAR PDM08[] = { '{','i','f','e','q',';',020,024,'a',';',020,024,'b',';',020,023,'t',';',020,023,'f','}', 0        };
CHAR PDM09[] = { '{','i','f','g','t',';',020,024,'a',';',020,024,'b',';',020,023,'t',';',020,023,'f','}', 0        };
CHAR PDM10[] = { '{','e','v','a','l',020,024,'e','}', 0                                                            };
CHAR PDM11[] = { '{','s','u','b','s','t','r',';',020,024,'f',';',020,024,'t',';',024,'s','}', 0                    };
CHAR PDM12[] = { '{','d','n','l','}', 0                                                                            };
CHAR PDM13[] = { '{','r','e','m','o','v','e','}', 0                                                                };
CHAR PDM14[] = { '{','d','u','m','p',';',020,024,'n','}', 0                                                        };

/* primitive declarations */
struct macro    Macprims[]      =
{
	&Macprims[1],   PDM00, (CHAR *)1,
	&Macprims[2],   PDM01, (CHAR *)2,
	&Macprims[3],   PDM02, (CHAR *)3,
	&Macprims[4],   PDM03, (CHAR *)4,
	&Macprims[5],   PDM04, (CHAR *)5,
	&Macprims[6],   PDM05, (CHAR *)6,
	&Macprims[7],   PDM06, (CHAR *)7,
	&Macprims[8],   PDM07, (CHAR *)8,
	&Macprims[9],   PDM08, (CHAR *)9,
	&Macprims[10],  PDM09, (CHAR *)10,
	&Macprims[11],  PDM10, (CHAR *)11,
	&Macprims[12],  PDM11, (CHAR *)12,
	&Macprims[13],  PDM12, (CHAR *)13,
	&Macprims[14],  PDM13, (CHAR *)3,
	0,              PDM14, (CHAR *)4,
};

struct macro    *Machead   =    &Macprims[0];   /* head of macro list */


/* parameters */
struct param
{
	struct param    *nextp;
	CHAR            mode;
	CHAR            name;
	CHAR            *paramt;
};



/* the environment */
struct env
{
	struct env      *nexte;         /* next environment */
	CHAR            (*rawget)();    /* raw character get routine */
	CHAR            **rawpar;       /* a parameter to that routine */
	CHAR            prevchar;       /* previous character read */
	CHAR            tokenmode;      /* current token mode */
	CHAR            change;         /* token change flag */
	CHAR            eof;            /* eof flag */
	CHAR            newline;        /* set if bol */
	CHAR            rawnewline;     /* same for raw input */
	struct buf      *pbuf;          /* peek buffer */
	struct buf      *mbuf;          /* macro buffer */
	CHAR            endtrap;        /* endtrap flag */
	CHAR            pass;           /* pass flag */
	CHAR            pdelim;         /* current parameter delimiter */
	struct param    *params;        /* parameter list */
	int             itercount;      /* iteration count */
	int             quotelevel;     /* quote nesting level */
};

/* current environment pointer */
struct env      *Macenv;

/*
**  MACINIT -- initialize for macro processing
**
**      *** EXTERNAL INTERFACE ***
**
**      The macro processor is initialized.  Any crap left over from
**      previous processing (which will never occur normally, but may
**      happen on an interrupt, for instance) will be cleaned up.  The
**      raw input is defined, and the 'endtrap' parameter tells whether
**      this is "primary" processing or not; in other words, it tells
**      whether to spring {begintrap} and {endtrap}.
**
**      This routine must always be called prior to any processing.
*/

macinit(rawget, rawpar, endtrap)
CHAR    (*rawget)();
CHAR    **rawpar;
int     endtrap;
{
	static struct env       env;
	register struct env     *e;
	register struct env     *f;

	/* clear out old crap */
	for (e = Macenv; e != 0; e = f)
	{
		bufpurge(&e->mbuf);
		bufpurge(&e->pbuf);
		macpflush(e);
		f = e->nexte;
		if (f != 0)
			buffree(e);
	}

	/* set up the primary environment */
	Macenv = e = &env;
	clrmem(e, sizeof *e);

	e->rawget = rawget;
	e->rawpar = rawpar;
	e->endtrap = endtrap;
	e->newline = 1;

	if (endtrap)
		macspring("{begintrap}");
}

/*
**  MACGETCH -- get character after macro processing
**
**      *** EXTERNAL INTERFACE ROUTINE ***
**
**      The macro processor must have been previously initialized by a
**      call to macinit().
*/

macgetch()
{
	register struct env     *e;
	register CHAR           c;

	e = Macenv;
	for (;;)
	{
		/* get an input character */
		c = macgch();

		/* check for end-of-file processing */
		if (c == 0)
		{
			/* check to see if we should spring {endtrap} */
			if (e->endtrap)
			{
				e->endtrap = 0;
				macspring("{endtrap}");
				continue;
			}

			/* don't spring endtrap -- real end of file */
			return (0);
		}

		/* not an end of file -- check for pass character through */
		if (e->pass)
		{
			e->pass = 0;
			e->change = 0;
		}
		if ((c & QUOTED) != 0 || !e->change || e->tokenmode == DELIM)
		{
			/* the character is to be passed through */
			/* reset iteration count and purge macro buffer */
			e->itercount = 0;
			bufflush(&e->mbuf);
			e->newline = (c == NEWLINE);
			return (c & CHARMASK);
		}

		/* this character is a candidate for macro processing */
		macunget(0);
		bufflush(&e->mbuf);

		/* check for infinite loop */
		if (e->itercount > ITERTHRESH)
		{
			printf("Infinite loop in macro\n");
			e->pass++;
			continue;
		}

		/* see if we have a macro match */
		if (macallscan())
		{
			/* yep -- count iterations and rescan it */
			e->itercount++;
		}
		else
		{
			/* nope -- pass the next token through raw */
			e->pass++;
		}
	}
}
/*
**  MACGCH -- get input character, knowing about tokens
**
**      The next input character is returned.  In addition, the quote
**      level info is maintained and the QUOTED bit is set if the
**      returned character is (a) quoted or (b) backslash escaped.
**      As a side effect the change flag is maintained.  Also, the
**      character is saved in mbuf.
*/

macgch()
{
	register CHAR           c;
	register struct env     *e;
	register int            i;

	e = Macenv;

	for (;;)
	{
		/* get virtual raw character, save in mbuf, and set change */
		c = macfetch(e->quotelevel > 0);

		/* test for magic frotz */
		switch (c)
		{
		  case 0:       /* end of file */
			return (0);

		  case LQUOTE:
			if (e->quotelevel++ == 0)
				continue;
			break;

		  case RQUOTE:
			if (e->quotelevel == 0)
				return (c);
			if (--e->quotelevel == 0)
			{
				continue;
			}
			break;

		  case BACKSLASH:
			if (e->quotelevel > 0)
				break;
			c = macfetch(1);

			/* handle special cases */
			if (c == e->pdelim)
				break;

			/* do translations */
			switch (c)
			{
			  case SPACE:   /* space */
			  case TAB:     /* tab */
			  case NEWLINE: /* newline */
			  case RQUOTE:
			  case LQUOTE:
			  case '$':
			  case LBRACE:
			  case RBRACE:
			  case BACKSLASH:
				break;

			  default:
				/* take character as is (unquoted) */
				c = 0;
				break;
			}

			if (c != 0)
				break;

			/* not an escapable character -- treat it normally */
			macunget(1);
			c = BACKSLASH;
			/* do default character processing on backslash */

		  default:
			if (e->quotelevel > 0)
				break;
			return (c);
		}

		/* the character is quoted */
		return (c | QUOTED);
	}
}
/*
**  MACFETCH -- fetch virtual raw character
**
**      A character is fetched from the peek buffer.  If that buffer is
**      empty, it is fetched from the raw input.  The character is then
**      saved away, and the change flag is set accordingly.
**      The QUOTED bit on the character is set if the 'quote' flag
**      parameter is set; used for backslash escapes.
**      Note that the QUOTED bit appears only on the character which
**      goes into the macro buffer; the character returned is normal.
*/

macfetch(quote)
int     quote;
{
	register struct env     *e;
	register CHAR           c;
	register CHAR           escapech;

	e = Macenv;
	escapech = 0;

	for (;;)
	{
		/* get character from peek buffer */
		c = bufget(&e->pbuf);

		if (c == 0)
		{
			/* peek buffer is empty */
			/* check for already raw eof */
			if (!e->eof)
			{
				/* note that c must be int so that the QUOTED bit is not negative */
				c = (*e->rawget)(e->rawpar);
				if (c <= 0)
				{
					c = 0;
					e->eof++;
				}
				else
				{
					if (e->rawnewline)
						e->prevchar = NEWLINE;
					e->rawnewline = (c == NEWLINE);
				}
			}
		}

		/* test for escapable character */
		if (escapech)
		{
			switch (c)
			{
			  case 't':     /* become quoted tab */
				c = TAB | QUOTED;
				break;

			  case 'n':     /* become quoted newline */
				c = NEWLINE | QUOTED;
				break;

			  default:
				bufput(c, &e->pbuf);
				c = BACKSLASH;
			}
			escapech = 0;
		}
		else
		{
			if (c == BACKSLASH)
			{
				escapech++;
				continue;
			}
		}
		break;
	}

	/* quote the character if appropriate to mask change flag */
	/* ('escapech' now becomes the maybe quoted character) */
	escapech = c;
	if (quote && c != 0)
		escapech |= QUOTED;

	/* set change flag */
	macschng(escapech);

	if (c != 0)
	{
		/* save the character in the macro buffer */
		bufput(escapech, &e->mbuf);
	}

	return (c);
}
/*
**  MACSCHNG -- set change flag and compute token type
**
**      The change flag and token type is set.  This does some tricky
**      stuff to determine just when a new token begins.  Most notably,
**      notice that quoted stuff IS scanned, but the change flag is
**      reset in a higher level routine so that quoted stuff looks
**      like a single token, but any begin/end quote causes a token
**      change.
*/

macschng(ch)
CHAR    ch;
{
	register struct env     *e;
	register CHAR           c;
	register CHAR           thismode;
	int                     changeflag;

	e = Macenv;
	c = ch;
	changeflag = 0;
	thismode = macmode(c);

	switch (e->tokenmode)
	{
	  case NONE:
		/* always cause token change */
		break;

	  case QUOTEMODE:
		/* change only on initial entry to quotes */
		break;

	  case DELIM:
		changeflag++;
		break;

	  case ID:
		/* take any sequence of letters and numerals */
		if (thismode == NUMBER)
			thismode = ID;
		break;

	  case NUMBER:
		/* take string of digits and decimal points */
		if (c == '.')
			thismode = NUMBER;
		break;

	  case OP:
		switch (e->prevchar)
		{
		  case '<':
		  case '>':
		  case '!':
			if (c != '=')
				changeflag++;
			break;

		  case '*':
			if (c != '*' && c != '/')
				changeflag++;
			break;

		  case '/':
			if (c != '*')
				changeflag++;
			break;

		  case '.':
			if (thismode == NUMBER)
				e->tokenmode = thismode;
			break;

		  default:
			changeflag++;
			break;
		}
		break;

	  case NOCHANGE:        /* never cause token change */
		e->tokenmode = thismode;
		break;
	}

	e->prevchar = c;
	if (thismode != e->tokenmode)
		changeflag++;
	e->tokenmode = thismode;
	e->change = changeflag;
}
/*
**  MACMODE -- return mode of a character
*/

macmode(ch)
CHAR    ch;
{
	register short  c;

	c = ch & 0377;

	if ((ch & QUOTED) != 0)
		return (QUOTEMODE);
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_')||
#ifdef Ucode
	    (c >= ('а'&0377) && c <= ('я'&0377)) || (c >= ('А'&0377) && c <= ('Я'&0377)))
#endif
#ifdef KOI8
	    (c >= ('ю'&0377) && c <= ('ъ'&0377)) || (c >= ('Ю'&0377) && c <= ('\377'&0377)))
#endif
#if !defined(KOI8) && !defined( Ucode )
							)
#endif
		return (ID);
	if (c >= '0' && c <= '9')
		return (NUMBER);
	if (c == SPACE || c == TAB || c == NEWLINE)
		return (DELIM);
	return (OP);
}
/*
**  MACALLSCAN -- scan to see if input matches a macro
**
**      Returns true if there was a match, false if not.  In any case,
**      the virtual raw input (i.e., the peek buffer) will contain
**      either the old raw input, or the substituted macro.
*/

macallscan()
{
	register struct macro   *m;

	for (m = Machead; m != 0; m = m->nextm)
	{
		/* check to see if it matches this macro */
		if (macscan(m))
		{
			/* it does -- substituted value is in mbuf */
			macrescan();
			return (1);
		}

		/* it doesn't match this macro -- try the next one */
		macrescan();
	}

	/* it doesn't match any of them -- tough luck */
	return (0);
}
/*
**  MACSCAN -- scan a single macro for a match
**
**      As is scans it also collects parameters for possible future
**      substitution.  If it finds a match, it takes responsibility
**      for doing the substitution.
*/

macscan(mac)
struct macro    *mac;
{
	register struct macro   *m;
	register CHAR           c;
	register CHAR           *temp;
	CHAR                    pname, pdelim;

	m = mac;

	/* check for anchored mode */
	temp = m->template;
	if (*temp == ONEDELIM)
	{
		if (!Macenv->newline)
			return (0);
		temp++;
	}

	/* scan the template */
	for ( ; c = *temp; temp++)
	{
		if (c == PARAMN || c == PARAMP)
		{
			/* we have a parameter */
			pname = *++temp;
			pdelim = *++temp;
			if (macparam(c, pname, pdelim))
			{
				/* parameter ok */
				continue;
			}

			/* failure on parameter scan */
			return (0);
		}

		if (!macmatch(c))
		{
			/* failure on literal match */
			return (0);
		}
	}

	/* it matches!!  substitute the macro */
	macsubs(m);
	return (1);
}
/*
**  MACPARAM -- collect a parameter
**
**      The parameter is collected and stored away "somewhere" with
**      name 'name'.  The delimiter is taken to be 'delim'.  'Mode'
**      tells whether to prescan the parameter (done immediately before
**      substitute time to avoid side effects if the macro actually
**      turns out to not match).
*/

macparam(mode, name, delim)
CHAR    mode;
CHAR    name;
CHAR    delim;
{
	register CHAR           c;
	register struct env     *e;
	struct buf              *b;
	register struct param   *p;
	int                     bracecount;
	extern CHAR             *bufcrunch();
	extern char             *bufalloc();
	e = Macenv;
	b = 0;

	e->pdelim = delim;
	TRACE("\nmacparam(%d, %c, %c):\n", mode, name, delim);
	if (mode == PARAMP)
	{
		/* check for REALLY prescan */
		c = macgch();
		if (c != PRESCANENABLE)
		{
			mode = PARAMN;
			macunget(0);
		}
	}

	bracecount = 0;
	e->tokenmode = NOCHANGE;
	while (!macmatch(delim))
	{
		do
		{
			c = macgch();
			if (c == 0 || c == NEWLINE)
			{
				e->pdelim = 0;
				bufpurge(&b);
				TRACE("macparam fails\n");
				return (0);
			}
			bufput(c, &b);
			if (c == LBRACE)
				bracecount++;
			else if (c == RBRACE && bracecount > 0)
				bracecount--;
		} while (bracecount > 0);
	}

	e->pdelim = 0;

	/* allocate and store the parameter */
	p = (struct param *) bufalloc(sizeof *p);
	p->mode = mode;
	p->name = name;
	p->nextp = e->params;
	e->params = p;
	p->paramt = bufcrunch(&b);
	bufpurge(&b);
	TRACE("macparam: |%s|\n", p->paramt);

	return (1);
}
/*
**  MACMATCH -- test for a match between template character and input.
**
**      The parameter is the character from the template to match on.
**      The input is read.  The template character may be a meta-
**      character.  In all cases if the match occurs the input is
**      thrown away; if no match occurs the input is left unchanged.
**
**      Return value is true for a match, false for no match.
*/

macmatch(template)
CHAR     template;
{
	register CHAR   t;
	register CHAR   c;
	register CHAR   res;

	t = template;
	TRACE("\tmacmatch(%c)", t);

	switch (t)
	{
	  case ANYDELIM:        /* match zero or more delimiters */
		/* chew and chuck delimiters */
		while (macdelim())
			;

		/* as a side effect, must match a token change */
		if (!macckch())
		{
			TRACE(" fail\n");
			return (0);
		}
		TRACE(" succeed\n");
		return (1);

	  case ONEDELIM:        /* match exactly one delimiter */
		TRACE(":\n");
		res = macdelim();
		return (res);

	  case CHANGE:          /* match a token change */
	  case 0:               /* end of template */
		TRACE(":\n");
		res = macckch();
		return (res);

	  default:              /* must have exact character match */
		c = macgch();
		TRACE(" against %c ", c);
		if (c == t)
		{
			TRACE("succeed\n");
			return (1);
		}

		/* failure */
		macunget(0);
		TRACE("fail\n");
		return (0);
	}
}
/*
**  MACDELIM -- test for next input character a delimiter
**
**      Returns true if the next input character is a delimiter, false
**      otherwise.  Delimiters are chewed.
*/

macdelim()
{
	register CHAR   c;

	c = macgch();
	TRACE("\t\tmacdelim against %c: ", c);
	if (macmode(c) == DELIM)
	{
		TRACE("succeed\n");
		return (1);
	}
	macunget(0);
	TRACE("fail\n");
	return (0);
}
/*
**  MACCKCH -- check for token change
**
**      Returns true if a token change occurs between this and the next
**      character.  No characters are ever chewed, however, the token
**      change (if it exists) is always chewed.
*/

macckch()
{
	register CHAR           change;
	register CHAR           c;
	register struct env     *e;

	e = Macenv;

	if (e->tokenmode == NONE)
	{
		/* then last character has been ungotten: take old change */
		change = e->change;
	}
	else
	{
		c = macgch();
		change = Macenv->change;
		macunget(0);
	}
	TRACE("macckch got %c ret %d\n", c, change);

	/* chew the change and return */
	e->tokenmode = NOCHANGE;
	return (change);
}
/*
**  MACSUBS -- substitute in macro substitution
**
**      This routine prescans appropriate parameters and then either
**      loads the substitution into the macro buffer or calls the
**      correct primitive routine.
*/

macsubs(mac)
struct macro    *mac;
{
	register struct param   *p;
	register struct env     *e;
	register CHAR           *s;
	CHAR *macprim();
	e = Macenv;

	for (p = e->params; p != 0; p = p->nextp)
	{
		/* check to see if we should prescan */
		if (p->mode != PARAMP)
		{
			continue;
		}

		/* prescan parameter */
		macprescan(&p->paramt);
		p->mode = PARAMN;
	}

	s = mac->substitute;

	/* clear out the macro call */
	bufflush(&e->mbuf);

	if (s <= (CHAR *) NPRIMS)
	{
		/* it is a primitive */
		macload(macprim(s), 0);
	}
	else
	{
		/* it is a user-defined macro */
		macload(s, 1);
	}
}
/*
**  MACPRESCAN -- prescan a parameter
**
**      The parameter pointed to by 'pp' is fed once through the macro
**      processor and replaced with the new version.
*/

macprescan(pp)
CHAR    **pp;
{
	struct buf              *b;
	CHAR                    *p;
	register struct env     *e;
	register CHAR           c;
	extern CHAR             macsget();

	b = 0;
	p = *pp;

	/* set up a new environment */
	macnewev(macsget, &p);
	e = Macenv;

	/* scan the parameter */
	while ((c = macgetch()) != 0)
		bufput(c, &b);

	/* free the old parameter */
	buffree(*pp);

	/* move in the new one */
	*pp = bufcrunch(&b);
	bufpurge(&b);

	/* restore the old environment */
	macpopev();
}
/*
**  MACNEWEV -- set up new environment
**
**      Parameters are raw get routine and parameter
*/

macnewev(rawget, rawpar)
CHAR    (*rawget)();
CHAR    **rawpar;
{
	register struct env     *e;
	extern char		*bufalloc();

	e = (struct env *) bufalloc(sizeof *e);
	e->rawget = rawget;
	e->rawpar = rawpar;
	e->nexte = Macenv;
	e->newline = 1;
	Macenv = e;
}
/*
**  MACPOPEV -- pop an environment
**
**      Makes sure all buffers and stuff are purged
*/

macpopev()
{
	register struct env     *e;

	e = Macenv;
	bufpurge(&e->mbuf);
	bufpurge(&e->pbuf);
	macpflush(e);
	Macenv = e->nexte;
	buffree(e);
}
/*
**  MACPFLUSH -- flush all parameters
**
**      Used to deallocate all parameters in a given environment.
*/

macpflush(env)
struct env      *env;
{
	register struct env     *e;
	register struct param   *p;
	register struct param   *q;

	e = env;

	for (p = e->params; p != 0; p = q)
	{
		buffree(p->paramt);
		q = p->nextp;
		buffree(p);
	}

	e->params = 0;
}
/*
**  MACSGET -- get from string
**
**      Works like a getchar from a string.  Used by macprescan().
**      The parameter is a pointer to the string.
*/

CHAR
macsget(pp)
CHAR  **pp;
{
	register CHAR   **p;
	register CHAR   c;

	p = pp;

	c = **p & CHARMASK;
	if (c != 0)
		(*p)++;
	return (c);
}

Macsget(pp)     /* external inteface routine by @VG */
char    **pp;
{
	register char   **p;
	register CHAR   c;

	p = pp;

	c = **p & BYTEMASK;
	if (c != 0)
		(*p)++;
	return (c);
}
/*
**  MACLOAD -- load a string into the macro buffer
**
**      The parameters are a pointer to a string to be appended to
**      the macro buffer and a flag telling whether parameter substi-
**      tution can occur.
*/

macload(str, flag)
CHAR    *str;
int     flag;
{
	register struct env     *e;
	register CHAR           *s;
	register CHAR            c;
	extern   CHAR           *macplkup();

	e = Macenv;
	s = str;

	if (s == 0)
		return;

	while ((c = *s++) != 0)
	{
		if (c == PARAMN)
			macload(macplkup(*s++), 0);
		else
			bufput(c & CHARMASK, &e->mbuf);
	}
}
/*
**  MACRESCAN -- rescan the macro buffer
**
**      Copies the macro buffer into the peek buffer so that it will be
**      reread.  Also deallocates any parameters which may happen to be
**      stored away.
*/

macrescan()
{
	register struct env     *e;
	register CHAR            c;

	e = Macenv;

	while ((c = bufget(&e->mbuf) & CHARMASK) != 0)
		bufput(c, &e->pbuf);

	e->quotelevel = 0;
	e->tokenmode = NONE;
	macpflush(e);
}
/*
**  MACUNGET -- unget a character
**
**      Moves one character from the macro buffer to the peek buffer.
**      If 'mask' is set, the character has the quote bit stripped off.
*/

macunget(mask)
int     mask;
{
	register struct env     *e;
	register CHAR            c;

	e = Macenv;

	if (e->prevchar != 0)
	{
		c = bufget(&e->mbuf);
		if (mask)
			c &= CHARMASK;
		bufput(c, &e->pbuf);
		e->tokenmode = NONE;
	}
}
/*
**  MACPLKUP -- look up parameter
**
**      Returns a pointer to the named parameter.  Returns null
**      if the parameter is not found ("cannot happen").
*/

CHAR *
macplkup(name)
CHAR     name;
{
	register struct param   *p;

	for (p = Macenv->params; p != 0; p = p->nextp)
	{
		if (p->name == name)
			return (p->paramt);
	}

	return (0);
}



/*
**  MACSPRING -- spring a trap
**
**      The named trap is sprung, in other words, if the named macro is
**      defined it is called, otherwise there is no replacement text.
*/

macspring(trap)
CHAR    *trap;
{
	register struct env     *e;
	register CHAR           *p;
	CHAR *Macro();
	e = Macenv;

	bufflush(&e->mbuf);

	/* fetch the macro */
	p = Macro(trap);

	/* if not defined, don't bother */
	if (p == 0)
		return;

	/* load the trap */
	macload(p);

	/* insert a newline after the trap */
	bufput('\n', &e->mbuf);

	macrescan();
}



/*
**  MACPRIM -- do primitives
**
**      The parameter is the primitive to execute.
*/

CHAR *
macprim(n)
int     n;
{
	register struct env     *e;
	CHAR *macsstr();
	e = Macenv;

	switch (n)
	{
	  case 1:       /* {define; $t; $s} */
		macdnl();
		Macdefine(macplkup('t'), macplkup('s'), 0);
		break;

	  case 2:       /* {rawdefine; $t; $s} */
		macdnl();
		Macdefine(macplkup('t'), macplkup('s'), 1);
		break;

	  case 3:       /* {remove $t} */
		macdnl();
		macremove(macplkup('t'));
		break;

	  case 4:       /* {dump} */
			/* {dump; $n} */
		macdnl();
		macdump(macplkup('n'));
		break;

	  case 5:       /* {type $m} */
		macdnl();
		Put2s(macplkup('m'),"\n");
		break;

	  case 6:       /* {read $m} */
		Put2s(macplkup('m')," ");
		macread();
		break;

	  case 7:       /* {read; $n; $m} */
		Put2s(macplkup('m')," ");
		macread();
		Macdefine(macplkup('n'), bufcrunch(&e->mbuf), 1);
		{ static CHAR _x[] = {'{','r','e','a','d','c','o','u','n','t','}',0};
		return( _x ); };

	  case 8:       /* {ifsame; $a; $b; $t; $f} */
		if (Sequal(macplkup('a'), macplkup('b')))
			return (macplkup('t'));
		else
			return (macplkup('f'));

	  case 9:       /* {ifeq; $a; $b; $t; $f} */
		if (macnumber(macplkup('a')) == macnumber(macplkup('b')))
			return (macplkup('t'));
		else
			return (macplkup('f'));

	  case 10:      /* {ifgt; $a; $b; $t; $f} */
		if (macnumber(macplkup('a')) > macnumber(macplkup('b')))
			return (macplkup('t'));
		else
			return (macplkup('f'));

	  case 12:      /* {substr; $f; $t; $s} */
		return (macsstr(macnumber(macplkup('f')), macnumber(macplkup('t')), macplkup('s')));

	  case 13:      /* {dnl} */
		macdnl();
		break;

	  default:
		syserr("macro: bad primitive %d", n);
	}

	{ static CHAR _x[] = { 0 };
	return ( _x ); }
}
/*
**  MACDNL -- delete to newline
**
**      Used in general after macro definitions to avoid embarrassing
**      newlines.  Just reads input until a newline character, and
**      then throws it away.
*/

macdnl()
{
	register CHAR           c;
	register struct env     *e;

	e = Macenv;

	if((c = macgch()) == 0 || c == NEWLINE )
		bufflush(&e->mbuf);
	else
		macunget(0);
}
/*
**  MACDEFINE -- define primitive
**
**      This function defines a macro.  The parameters are the
**      template, the substitution string, and a flag telling whether
**      this is a raw define or not.  Syntax checking is done.
*/

macdefine(template, subs, raw)  /* interface routine - by @VG */
char    *template;
char    *subs;
int     raw;
{
	CHAR    Tbuf[512], Sbuf[512];
	register CHAR *C;
	register char *c;

	c = template;
	C = Tbuf;
	while( *C++ = ( *c++ & BYTEMASK ) );
	c = subs;
	C = Sbuf;
	while( *C++ = ( *c++ & BYTEMASK ) );
	Macdefine( Tbuf, Sbuf, raw );
	return;
}

Macdefine(template, subs, raw)
CHAR    *template;
CHAR    *subs;
int     raw;
{
	register struct env     *e;
	CHAR                    paramdefined[128];
	CHAR                    *p;
	register CHAR           c;
	CHAR                    d;
	struct buf              *b;
	register struct macro   *m;
	extern CHAR             macsget();
	CHAR                    escapech;
	extern CHAR             *bufcrunch();
	extern char             *bufalloc();
	CHAR                    *mactcvt();

	/* remove any old macro definition */
	macremove(template);

	/* get a new environment */
	macnewev(macsget, &p);
	b = 0;
	e = Macenv;

	/* undefine all parameters */
	clrmem(paramdefined, 128 * sizeof(CHAR));

	/* avoid an initial token change */
	e->tokenmode = NOCHANGE;
	escapech = 1;

	/* allocate macro header and template */
	m = (struct macro *) bufalloc(sizeof *m);

	/* scan and convert template, collect available parameters */
	p = template;
	m->template = mactcvt(raw, paramdefined);
	if (m->template == 0)
	{
		/* some sort of syntax error */
		buffree(m);
		macpopev();
		return;
	}

	bufflush(&e->mbuf);
	bufflush(&e->pbuf);
	e->eof = 0;

	/* scan substitute string */
	for (p = subs; c = macfetch(0); )
	{
		if (c != '$')
		{
			/* substitute non-parameters literally */
			bufput(c & CHARMASK, &b);
			continue;
		}

		/* it's a parameter */
		bufput(PARAMN, &b);
		c = macfetch(0);

		/* check to see if name is supplied */
		if (paramdefined[c] == 0)
		{
			/* nope, it's not */
			printf("define: parameter %c referenced but not defined\n", c);
			buffree(m->template);
			buffree(m);
			macpopev();
			bufpurge(&b);
			return;
		}
		bufput(c & CHARMASK, &b);
	}

	/* allocate substitution string */
	m->substitute = bufcrunch(&b);

	/* allocate it as a macro */
	m->nextm = Machead;
	Machead = m;

	/* finished... */
	macpopev();
	bufpurge(&b);
}
/*
**  MACTCVT -- convert template to internal form
**
**      Converts the template from external form to internal form.
**
**      Parameters:
**      raw -- set if only raw type conversion should take place.
**      paramdefined -- a map of flags to determine declaration of
**              parameters, etc.  If zero, no parameters are allowed.
**
**      Return value:
**      A character pointer off into mystic space.
**
**      The characters of the template are read using macfetch, so
**      a new environment should be created which will arrange to
**      get this.
*/

CHAR *
mactcvt(raw, paramdefined)
int     raw;
CHAR    paramdefined[128];
{
	register int		c;
	struct buf              *b;
	register CHAR           d;
	register CHAR           escapech;
	CHAR                    *p;

	b = 0;
	escapech = 1;

	while (c = macfetch(0))
	{
		switch (c)
		{
		  case '$':             /* parameter */
			if (escapech < 0)
			{
				printf("define: every parameter needs a delimiter\n");
				bufpurge(&b);
				return (0);
			}

			/* skip delimiters before parameter in non-raw */
			if (Macenv->change && !escapech && !raw)
				bufput(ANYDELIM, &b);

			escapech = 0;
			c = macfetch(0);
			d = PARAMN;
			if (c == '$')
			{
				/* prescanned parameter */
				d = PARAMP;
				c = macfetch(0);
			}

			/* process parameter name */
			if (c == 0)
			{
				/* no parameter name */
				printf("define: null parameter name\n");
				bufpurge(&b);
				return (0);
			}

			bufput(d, &b);
			escapech = -1;

			/* check for legal parameter */
			if (paramdefined == 0)
				break;

			if (paramdefined[c])
			{
				printf("define: parameter %c redeclared\n", c);
				bufpurge(&b);
				return (0);
			}
			paramdefined[c]++;

			/* get parameter delimiter */
			break;

		  case BACKSLASH:               /* a backslash escape */
			escapech = 1;
			c = macfetch(0);
			switch (c)
			{
			  case '|':
				c = ANYDELIM;
				break;

			  case '^':
				c = ONEDELIM;
				break;

			  case '&':
				c = CHANGE;
				break;

			  default:
				escapech = 0;
				c = BACKSLASH;
				macunget(0);
				break;
			}
			break;

		  case NEWLINE | QUOTED:
		  case TAB | QUOTED:
		  case SPACE | QUOTED:
			if (escapech < 0)
				c &= CHARMASK;
			escapech = 1;
			break;

		  default:
			/* change delimiters to ANYDELIM */
			if (macmode(c) == DELIM && !raw)
			{
				while (macmode(c = macfetch(0)) == DELIM)
					;
				macunget(0);
				if (c == 0)
					c = ONEDELIM;
				else
					c = ANYDELIM;
				escapech = 1;
			}
			else
			{
				if (Macenv->change && !escapech)
				{
					bufput(ANYDELIM, &b);
				}

				if (escapech < 0)
				{
					/* parameter: don't allow quoted delimiters */
					c &= CHARMASK;
				}
				escapech = 0;
			}
			break;
		}
		bufput(c, &b);
	}
	if (escapech <= 0)
		bufput(CHANGE, &b);

	p = bufcrunch(&b);
	bufpurge(&b);
	TRACE("mactcvt: '%s'\n", p);
	return (p);
}
/*
**  MACREMOVE -- remove macro
**
**      The named macro is looked up.  If it is found it is removed
**      from the macro list.
*/

macremove(name)
CHAR     *name;
{
	register struct macro   *m;
	register struct macro   **mp;
	extern CHAR             macsget();
	CHAR                    *p;
	register CHAR           *cname;
	struct macro		*macmlkup();

	if (name != NULL)
	{
		/* convert name to internal format */
		macnewev(macsget, &p);
		p = name;
		cname = mactcvt(0, 0);
		macpopev();
		if (cname == 0)
		{
			/* some sort of syntax error */
			return;
		}
	}
	/* find macro */
	while (name == NULL ?
	      ( (unsigned)((m = Machead)->substitute) > (unsigned) (NPRIMS)) :
	       ((unsigned)(m = macmlkup(cname)) != NULL))
	{
		/* remove macro from list */
		mp = &Machead;

		/* find it's parent */
		while (*mp != m)
			mp = &(*mp)->nextm;

		/* remove macro from list */
		*mp = m->nextm;
		buffree(m->template);
		buffree(m->substitute);
		buffree(m);
	}
	buffree(cname);
}
/*
**  MACMLKUP -- look up macro
**
**      The named macro is looked up and a pointer to the macro header
**      is returned.  Zero is returned if the macro is not found.
**      The name must be in internal form.
*/

struct macro *
macmlkup(name)
CHAR    *name;
{
	register struct macro   *m;
	register CHAR           *n;

	n = name;

	/* scan the macro list for it */
	for (m = Machead; m != 0; m = m->nextm)
	{
		if (macmmatch(n, m->template, 0))
			return (m);
	}
	return (0);
}
/*
**  MACMMATCH -- check for macro name match
**
**      The given 'name' and 'temp' are compared for equality.  If they
**      match true is returned, else false.
**      Both must be converted to internal format before the call is
**      given.
**
**      "Match" is defined as two macros which might scan as equal.
**
**      'Flag' is set to indicate that the macros must match exactly,
**      that is, neither may have any parameters and must end with both
**      at end-of-template.  This mode is used for getting traps and
**      such.
*/

macmmatch(name, temp, flag)
CHAR    *name;
CHAR    *temp;
int     flag;
{
	register CHAR   ac;
	register CHAR   bc;
	CHAR            *ap, *bp;

	ap = name;
	bp = temp;

	/* scan character by character */
	for (;; ap++, bp++)
	{
		ac = *ap;
		bc = *bp;
		TRACE("macmmatch: ac=%c/%u, bc=%c/%u\n", ac, ap, bc, bp);

		if (bc == ANYDELIM)
		{
			if (macmmchew(&ap))
				continue;
		}
		else
		{
			switch (ac)
			{
			  case SPACE:
			  case NEWLINE:
			  case TAB:
				if (ac == bc || bc == ONEDELIM)
					continue;
				break;

			  case ONEDELIM:
				if (ac == bc || macmode(bc) == DELIM)
					continue;
				break;

			  case ANYDELIM:
				if (macmmchew(&bp))
					continue;
				break;

			  case PARAMP:
			  case PARAMN:
			  case 0:
				if (bc == PARAMN || bc == PARAMP || bc == 0 ||
				    bc == ANYDELIM || bc == ONEDELIM ||
				    bc == CHANGE || macmode(bc) == DELIM)
				{
					/* success */
					if (!flag)
						return (1);
					if (ac == 0 && bc == 0)
						return (1);
				}
				break;

			  default:
				if (ac == bc)
					continue;
				break;
			}
		}

		/* failure */
		return (0);
	}
}
/*
**  MACMMCHEW -- chew nonspecific match characters
**
**      The pointer passed as parameter is scanned so as to skip over
**      delimiters and pseudocharacters.
**      At least one character must match.
*/

macmmchew(pp)
CHAR    **pp;
{
	register CHAR   *p;
	register CHAR   c;
	register int    matchflag;

	p = *pp;

	for (matchflag = 0; ; matchflag++)
	{
		c = *p;
		if (c != ANYDELIM && c != ONEDELIM && c != CHANGE &&
		    macmode(c) != DELIM)
			break;
		p++;
	}

	p--;
	if (matchflag == 0)
		return (0);
	*pp = p;
	return (1);
}
/*
**  MACREAD -- read a terminal input line
**
**      Reads one line from the user.  Returns the line into mbuf,
**      and a count of the number of characters read into the macro
**      "{readcount}" (-1 for end of file).
*/

macread()
{
	register struct env     *e;
	register int            count;
	register CHAR           c;

	e = Macenv;
	count = -1;

	while ((c = getchar()) > 0)
	{
		count++;
		if (c == NEWLINE)
			break;
		bufput(c, &e->mbuf);
	}

	macdefine("{readcount}", iocv(count), 1);
}
/*
**  MACNUMBER -- return converted number
**
**      This procedure is essentially identical to the system atoi
**      routine, in that it does no syntax checking whatsoever.
*/

macnumber(s)
CHAR    *s;
{
	register CHAR           *p;
	register CHAR           c;
	register int            result;
	int                     minus;

	result = 0;
	p = s;
	minus = 0;

	while ((c = *p++) == SPACE)
		;

	if (c == '-')
	{
		minus++;
		while ((c = *p++) == SPACE)
			;
	}

	while (c >= '0' && c <= '9')
	{
		result = result * 10 + (c - '0');
		c = *p++;
	}

	if (minus)
		result = -result;

	return (result);
}
/*
**  MACSUBSTR -- substring primitive
**
**      The substring of 'string' from 'from' to 'to' is extracted.
**      A pointer to the result is returned.  Note that macsstr
**      in the general case modifies 'string' in place.
*/

CHAR *
macsstr(from, to, string)
int     from;
int     to;
CHAR    *string;
{
	register int    f;
	int             l;
	register CHAR   *s;
	register int    t;

	s = string;
	t = to;
	f = from;

	if (f < 1)
		f = 1;
	if (f >= t)
		{ static CHAR _x[] = { 0 };
		return ( _x ); }
	l = Slength(s);
	if (t < l)
		s[t] = 0;
	return (&s[f - 1]);
}
/*
**  MACDUMP -- dump a macro definition to the terminal
**
**      All macros matching 'name' are output to the buffer.  If
**      'name' is the null pointer, all macros are printed.
*/

macdump(name)
CHAR    *name;
{
	register struct macro   *m;
	register CHAR           *p;
	register CHAR           *n;
	extern CHAR             *macmocv();
	extern CHAR             macsget();
	CHAR                    *ptr;

	n = name;
	if (n != 0)
	{
		macnewev(macsget, &ptr);
		ptr = n;
		n = mactcvt(0, 0);
		macpopev();
		if (n == 0)
			return;
	}

	for (m = Machead; m != 0; m = m->nextm)
	{
		if (n == 0 || macmmatch(n, m->template, 0))
		{
			static CHAR p_rawd[]={'`','{','r','a','w','d','e','f','i','n','e',';',' ',0};
			static CHAR p_semi[]={';',' ',0};
			static CHAR p_ebra[]={'}','\'','\n',0};
			if (m->substitute <= (CHAR *) NPRIMS)
				continue;
			p = macmocv(m->template);
			macload(p_rawd, 0);
			macload(p, 0);
			macload(p_semi, 0);
			p = macmocv(m->substitute);
			macload(p, 0);
			macload(p_ebra, 0);
		}
	}
	if (n != 0)
		buffree(n);
}
/*
**  MACMOCV -- macro output conversion
**
**      This routine converts the internal format of the named macro
**      to an unambigous external representation.
**
**      Note that much work can be done to this routine to make it
**      produce cleaner output, for example, translate "\|" to " "
**      in most cases.
*/

CHAR *
macmocv(m)
CHAR    *m;
{
	register CHAR   *p;
	struct buf      *b;
	register CHAR   c;
	register CHAR   pc;
	static CHAR     *lastbuf;
	extern CHAR     *bufcrunch();
	CHAR            c1;
	p = m;
# define  SETCC(x,y) c=x;c1=y;


	/* release last used buffer (as appropriate) */
	if (lastbuf != 0)
	{
		buffree(lastbuf);
		lastbuf = 0;
	}

	if (p <= (CHAR *) NPRIMS)
	{
		static CHAR pbuf[16];
		char *pc;
		/* we have a primitive */
		pc = "Primitive xxx";
		itoa(m, &pc[10]);
		 p = pbuf;
		 while (*pc) *p++ = *pc++;
		 *p++ = 0;
		return (pbuf);
	}

	b = 0;

	for (; (c = *p++) != 0; pc = c)
	{
		c1 = 0;
		switch (c)
		{
		  case BACKSLASH:
		  case '|':
		  case '&':
		  case '^':
			break;

		  case ANYDELIM:
			SETCC('\\','|');
			break;

		  case ONEDELIM:
			SETCC('\\','^');
			break;

		  case CHANGE:
			SETCC('\\','&');
			break;

		  case PARAMN:
			c = '$';
			break;

		  case PARAMP:
			SETCC('$','$');
			break;

		  case '$':
			SETCC('\\','$');
			break;

		  case NEWLINE:
			c = ('\\' | QUOTED);
			c1 = '\n';
			break;

		  default:
			bufput(c, &b);
			continue;
		}

		if (pc == BACKSLASH)
			bufput(pc, &b);
		pc = c & (CHARMASK|QUOTED);
		bufput(pc, &b);
		if (c1 != 0)
		{
			c = c1;
			bufput(c, &b);
		}
	}

	p = bufcrunch(&b);
	bufpurge(&b);
	lastbuf = p;
	return (p);
}
/*
**  MACRO -- get macro substitution value
**
**      ***  EXTERNAL INTERFACE  ***
**
**      This routine handles the rather specialized case of looking
**      up a macro and returning the substitution value.  The name
**      must match EXACTLY, character for character.
**
**      The null pointer is returned if the macro is not defined.
*/

char *
macro(name)             /* interface routine - by @VG */
char	*name;
{
	static CHAR        mbf[80];
	register CHAR *p = mbf;
	register char *q = name;
	CHAR *Macro();
	while( *p++ = (*q++ & BYTEMASK) );
	if( (p = Macro(mbf)) == 0 )
		return(0);
	q = (char *)mbf;
	while( *q++ = *p++ );
	return( (char *)mbf );
}

CHAR *
Macro(name)
CHAR    *name;
{
	register struct macro   *m;
	register CHAR           *n;
	extern CHAR             macsget();
	CHAR                    *p;

	/* convert macro name to internal format */
	macnewev(macsget, &p);
	p = name;
	n = mactcvt(0, 0);
	macpopev();
	if (n == 0)
	{
		/* some sort of syntax error */
		return (0);
	}

	for (m = Machead; m != 0; m = m->nextm)
	{
		if (macmmatch(n, m->template, 1))
		{
			buffree(n);
			return (m->substitute);
		}
	}

	buffree(n);
	return (0);
}

/*
 * Программы, введенные для работы с 16-битными "символами"
 */
Put2s(ss,s)
register CHAR *ss;
char *s;
{
	while ( *ss) putchar(*ss++);
	printf(s);
}

/* bool */
Sequal(a, b)
register CHAR   *a, *b;
{
	while (*a || *b)
		if (*a++ != *b++)
			return(FALSE);
	return(TRUE);
}

Slength(s)
register CHAR *s;
{
	register int i=0;
	while(*s++) i++;
	return(i);
}
