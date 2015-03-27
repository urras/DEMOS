#include "rcv.h"

/*
 * Mail -- a mail program
 *
 * Message list handling.
 *
 * $Log: list.c,v $
 * Revision 1.10  1991/07/17  19:56:23  ache
 * tEPERX WEKTOR SOOB]ENIJ FORMIRUETSQ W PORQDKE IH UKAZANIQ
 *
 * Revision 1.9  1990/11/11  20:06:21  ache
 * iSPRAWLENO RASPOZNAWANIE IMEN FAJLOW
 *
 * Revision 1.8  90/09/29  18:22:22  ache
 * <ctype.h> kicked out...
 * 
 * Revision 1.7  90/09/13  13:19:37  ache
 * MS-DOS & Unix together...
 * 
 * Revision 1.6  90/06/26  17:13:42  avg
 * pOPRAWLENA OBRABOTKA KAWY^EK W getrawlist W STORONU shell-style.
 * 
 * Revision 1.5  90/06/10  00:43:17  avg
 * sDELANO SOPOSTAWLENIE S OBRAZCOM PRI POISKE SOOB]ENIJ W KOMANDE from.
 * 
 * Revision 1.4  90/04/20  19:16:59  avg
 * pRIKRU^ENO POD System V
 * 
 * Revision 1.3  88/07/23  20:34:56  ache
 * rUSSKIE DIAGNOSTIKI
 * 
 * Revision 1.2  88/01/11  12:43:13  avg
 * dOBAWLEN NOXSTR U rcsid.
 * 
 * Revision 1.1  87/12/25  15:59:49  avg
 * Initial revision
 * 
 */

/*NOXSTR
static char rcsid[] = "$Header: /usr/src/Relcom/relcom/mailx/RCS/list.c,v 1.10 1991/07/17 19:56:23 ache Exp $";
YESXSTR*/

/*
 * Convert the user string of message numbers and
 * store the numbers into vector.
 *
 * Returns the count of messages picked up or -1 on error.
 */

getmsglist(buf, vector, flags)
	char *buf;
	int *vector;
{
	int *ip;
	register struct message *mp;

	ip = vector;
	if (markall(buf, flags, &ip) < 0)
		return(-1);
	return(ip - vector);
}

/*
 * Mark all messages that the user wanted from the command
 * line in the message structure.  Return 0 on success, -1
 * on error.
 */

/*
 * Bit values for colon modifiers.
 */

#define CMNEW           01              /* New messages */
#define CMOLD           02              /* Old messages */
#define CMUNREAD        04              /* Unread messages */
#define CMDELETED       010             /* Deleted messages */
#define CMREAD          020             /* Read messages */

/*
 * The following table describes the letters which can follow
 * the colon and gives the corresponding modifier bit.
 */

struct coltab {
	char    co_char;                /* What to find past : */
	int     co_bit;                 /* Associated modifier bit */
	int     co_mask;                /* m_status bits to mask */
	int     co_equal;               /* ... must equal this */
} coltab[] = {
	'n',            CMNEW,          MNEW,           MNEW,
	'o',            CMOLD,          MNEW,           0,
	'u',            CMUNREAD,       MREAD,          0,
	'd',            CMDELETED,      MDELETED,       MDELETED,
	'r',            CMREAD,         MREAD,          MREAD,
	0,              0,              0,              0
};

static  int     lastcolmod;

static
markall(buf, f, vector)
	char buf[];
	int **vector;
{
	register char **np;
	register int i;
	register struct message *mp;
	char *namelist[NMLSIZE], *bufp;
	int tok, beg, mc, star, other, valdot, colmod, colresult;
	int noorder;
	register int *ip, *startip;

	valdot = dot - &message[0] + 1;
	colmod = 0;
	for (i = 1; i <= msgCount; i++)
		unmark(i);
	bufp = buf;
	ip = *vector;
	mc = 0;
	np = &namelist[0];
	scaninit();
	tok = scan(&bufp);
	star = 0;
	other = 0;
	beg = 0;
	while (tok != TEOL) {
		switch (tok) {
		case TNUMBER:
number:
			if (star) {
				printf(ediag(
"No numbers mixed with *\n",
"nOMERA NE DOLVNY BYTX WMESTE SO *\n"));
				return(-1);
			}
			mc++;
			other++;
			startip = ip;
			if (beg != 0) {
				if (check(lexnumber, f))
					return(-1);
				for (i = beg; i <= lexnumber; i++)
					if ((message[i - 1].m_flag & MDELETED) == f) {
						mark(i);
						if (ip != NULL)
							*ip++ = i;
					}
				beg = 0;
				break;
			}
			beg = lexnumber;
			if (check(beg, f))
				return(-1);
			tok = scan(&bufp);
			regret(tok);
			if (tok != TDASH) {
				mark(beg);
				if (ip != NULL) {
					*startip = beg;
					if (ip == startip)
						ip++;
				}
				beg = 0;
			}
			break;

		case TPLUS:
			if (beg != 0) {
				printf(ediag(
"Non-numeric second argument\n",
"wTOROJ ARGUMENT NE^ISLOWOJ\n"));
				return(-1);
			}
			if (valdot < msgCount) {
				mark(valdot+1);
				if (ip != NULL)
					*ip++ = valdot + 1;
			}
			else {
				printf(ediag(
"Referencing beyond EOF\n",
"sSYLKA ZA KONCOM SPISKA\n"));
				return(-1);
			}
			break;

		case TDASH:
			if (beg == 0) {
				if (valdot > 1) {
					mark(valdot-1);
					if (ip != NULL)
						*ip++ = valdot - 1;
				}
				else {
					printf(ediag(
"Referencing before 1\n",
"sSYLKA PERED NA^ALOM SPISKA\n"));
					return(-1);
				}
			}
			break;

		case TSTRING:
			if (beg != 0) {
				printf(ediag(
"Non-numeric second argument\n",
"wTOROJ ARGUMENT NE^ISLOWOJ\n"));
				return(-1);
			}
			other++;
			if (lexstring[0] == ':') {
				colresult = evalcol(lexstring[1]);
				if (colresult == 0) {
					printf(ediag(
"Unknown colon modifier \"%s\"\n",
"nEWERNAQ POSLEDOWATELXNOSTX \"%s\" POSLE ':'\n"),
					    lexstring);
					return(-1);
				}
				colmod |= colresult;
			}
			else
				*np++ = savestr(lexstring);
			break;

		case TDOLLAR:
		case TUP:
		case TDOT:
			lexnumber = metamess(lexstring[0], f);
			if (lexnumber == -1)
				return(-1);
			goto number;

		case TSTAR:
			if (other) {
				printf(ediag(
"Can't mix \"*\" with anything\n",
"nELXZQ, ^TOBY WMESTE BYLI \"*\" I ^TO-TO E]E\n"));
				return(-1);
			}
			star++;
			break;
		}
		tok = scan(&bufp);
	}
	lastcolmod = colmod;
	*np = NOSTR;
	mc = 0;
	if (star) {
		for (i = 0; i < msgCount; i++)
			if ((message[i].m_flag & MDELETED) == f) {
				mark(i+1);
				if (ip != NULL)
					*ip++ = i + 1;
				mc++;
			}
		if (mc == 0) {
			printf(ediag(
"No applicable messages.\n",
"nET PODHODQ]IH SOOB]ENIJ.\n"));
			return(-1);
		}
		if (ip != NULL) {
			*ip = (int)NULL;
			*vector = ip;
		}
		return(0);
	}

	noorder = (np > namelist || colmod != 0);

	/*
	 * If no numbers were given, mark all of the messages,
	 * so that we can unmark any whose sender was not selected
	 * if any user names were given.
	 */

	if (noorder && mc == 0)
		for (i = 1; i <= msgCount; i++)
			if ((message[i-1].m_flag & (MSAVED|MDELETED)) == f)
				mark(i);

	/*
	 * If any names were given, go through and eliminate any
	 * messages whose senders were not requested.
	 */

	if (np > namelist) {
		for (i = 1; i <= msgCount; i++) {
			for (mc = 0, np = namelist; *np != NOSTR; np++)
				if (**np == '/'
#ifdef	MSDOS
					|| **np == SEPCHAR
					|| **np && *(*np + 1) == ':'
#endif
				   ) {
					if (matchsubj(*np, i)) {
						mc++;
						break;
					}
				}
				else {
					if (sender(*np, i)) {
						mc++;
						break;
					}
				}
			if (mc == 0)
				unmark(i);
		}

		/*
		 * Make sure we got some decent messages.
		 */

		mc = 0;
		for (i = 1; i <= msgCount; i++)
			if (message[i-1].m_flag & MMARK) {
				mc++;
				break;
			}
		if (mc == 0) {
			printf(ediag(
"No applicable messages from {%s",
"nET PODHODQ]IH SOOB]ENIJ OT {%s"),
				namelist[0]);
			for (np = &namelist[1]; *np != NOSTR; np++)
				printf(", %s", *np);
			printf("}\n");
			return(-1);
		}
	}

	/*
	 * If any colon modifiers were given, go through and
	 * unmark any messages which do not satisfy the modifiers.
	 */

	if (colmod != 0) {
		for (i = 1; i <= msgCount; i++) {
			register struct coltab *colp;

			mp = &message[i - 1];
			for (colp = &coltab[0]; colp->co_char; colp++)
				if (colp->co_bit & colmod)
					if ((mp->m_flag & colp->co_mask)
					    != colp->co_equal)
						unmark(i);
		}
		for (mp = &message[0]; mp < &message[msgCount]; mp++)
			if (mp->m_flag & MMARK)
				break;
		if (mp >= &message[msgCount]) {
			register struct coltab *colp;

			printf(ediag(
"No messages satisfy",
"nET PODHODQ]IH SOOB]ENIJ"));
			for (colp = &coltab[0]; colp->co_char; colp++)
				if (colp->co_bit & colmod)
					printf(" :%c", colp->co_char);
			printf("\n");
			return(-1);
		}
	}
	if (ip != NULL) {
		if (noorder) {
			ip = *vector;
			for (i = 1; i <= msgCount; i++)
				if (message[i-1].m_flag & MMARK)
					*ip++ = i;
		}
		*ip = (int)NULL;
		*vector = ip;
	}
	return(0);
}

/*
 * Turn the character after a colon modifier into a bit
 * value.
 */
evalcol(col)
{
	register struct coltab *colp;

	if (col == 0)
		return(lastcolmod);
	for (colp = &coltab[0]; colp->co_char; colp++)
		if (colp->co_char == col)
			return(colp->co_bit);
	return(0);
}

/*
 * Check the passed message number for legality and proper flags.
 */

check(mesg, f)
{
	register struct message *mp;

	if (mesg < 1 || mesg > msgCount) {
		printf(ediag(
"%d: Invalid message number\n",
"%d: nEWERNYJ NOMER SOOB]ENIQ\n"),
mesg);
		return(-1);
	}
	mp = &message[mesg-1];
	if ((mp->m_flag & MDELETED) != f) {
		printf(ediag(
"%d: Inappropriate message\n",
"%d: nEPODHODQ]EE SOOB]ENIE\n"),
mesg);
		return(-1);
	}
	return(0);
}

/*
 * Scan out the list of string arguments, shell style
 * for a RAWLIST.
 */

getrawlist(line, argv)
	char line[];
	char **argv;
{
	register char **ap, *cp, *cp2;
	char linebuf[BUFSIZ], quotec;

	ap = argv;
	cp = line;
	while (*cp != '\0') {
		while (any(*cp, " \t"))
			cp++;
		cp2 = linebuf;
		quotec = 0;
		while( *cp != '\0' && *cp != '\t' && *cp != ' ' ) {
			if( *cp == '\'' || *cp == '"' ) {
				quotec = *cp++;
				while( *cp != '\0' && *cp != quotec )
					*cp2++ = *cp++;
				if( *cp != '\0' )
					cp++;
				continue;
			}
			*cp2++ = *cp++;
		}
		*cp2 = '\0';
		if (cp2 == linebuf)
			break;
		*ap++ = savestr(linebuf);
	}
	*ap = NOSTR;
	return(ap-argv);
}

/*
 * scan out a single lexical item and return its token number,
 * updating the string pointer passed **p.  Also, store the value
 * of the number or string scanned in lexnumber or lexstring as
 * appropriate.  In any event, store the scanned `thing' in lexstring.
 */

struct lex {
	char    l_char;
	char    l_token;
} singles[] = {
	'$',    TDOLLAR,
	'.',    TDOT,
	'^',    TUP,
	'*',    TSTAR,
	'-',    TDASH,
	'+',    TPLUS,
	'(',    TOPEN,
	')',    TCLOSE,
	0,      0
};

scan(sp)
	char **sp;
{
	register char *cp, *cp2;
	register int c;
	register struct lex *lp;
	int quotec;

	if (regretp >= 0) {
		copy(stringstack[regretp], lexstring);
		lexnumber = numberstack[regretp];
		return(regretstack[regretp--]);
	}
	cp = *sp;
	cp2 = lexstring;
	c = *cp++;

	/*
	 * strip away leading white space.
	 */

	while (any(c, " \t"))
		c = *cp++;

	/*
	 * If no characters remain, we are at end of line,
	 * so report that.
	 */

	if (c == '\0') {
		*sp = --cp;
		return(TEOL);
	}

	/*
	 * If the leading character is a digit, scan
	 * the number and convert it on the fly.
	 * Return TNUMBER when done.
	 */

	if (isdigit(c)) {
		lexnumber = 0;
		while (isdigit(c)) {
			lexnumber = lexnumber*10 + c - '0';
			*cp2++ = c;
			c = *cp++;
		}
		*cp2 = '\0';
		*sp = --cp;
		return(TNUMBER);
	}

	/*
	 * Check for single character tokens; return such
	 * if found.
	 */

	for (lp = &singles[0]; lp->l_char != 0; lp++)
		if (c == lp->l_char) {
			lexstring[0] = c;
			lexstring[1] = '\0';
			*sp = cp;
			return(lp->l_token);
		}

	/*
	 * We've got a string!  Copy all the characters
	 * of the string into lexstring, until we see
	 * a null, space, or tab.
	 * If the lead character is a " or ', save it
	 * and scan until you get another.
	 */

	quotec = 0;
	if (any(c, "'\"")) {
		quotec = c;
		c = *cp++;
	}
	while (c != '\0') {
		if (c == quotec)
			break;
		if (quotec == 0 && any(c, " \t"))
			break;
		if (cp2 - lexstring < STRINGLEN-1)
			*cp2++ = c;
		c = *cp++;
	}
	if (quotec && c == 0)
		fprintf(stderr, ediag(
"Missing %c\n",
"pROPU]EN %c\n"),
quotec);
	*sp = --cp;
	*cp2 = '\0';
	return(TSTRING);
}

/*
 * Unscan the named token by pushing it onto the regret stack.
 */

regret(token)
{
	if (++regretp >= REGDEP)
		panic(ediag("Too many regrets","sLI[KOM MNOGO regret'OW"));
	regretstack[regretp] = token;
	lexstring[STRINGLEN-1] = '\0';
	stringstack[regretp] = savestr(lexstring);
	numberstack[regretp] = lexnumber;
}

/*
 * Reset all the scanner global variables.
 */

scaninit()
{
	regretp = -1;
}

/*
 * Find the first message whose flags & m == f  and return
 * its message number.
 */

first(f, m)
{
	register int mesg;
	register struct message *mp;

	mesg = dot - &message[0] + 1;
	f &= MDELETED;
	m &= MDELETED;
	for (mp = dot; mp < &message[msgCount]; mp++) {
		if ((mp->m_flag & m) == f)
			return(mesg);
		mesg++;
	}
	mesg = dot - &message[0];
	for (mp = dot-1; mp >= &message[0]; mp--) {
		if ((mp->m_flag & m) == f)
			return(mesg);
		mesg--;
	}
	return((int)NULL);
}

/*
 * See if the passed name sent the passed message number.  Return true
 * if so.
 */

sender(str, mesg)
	char *str;
{
	register struct message *mp;
	register char *cp;
	char *index(), *rindex();
	char *p, *q, savc, savc1;
	char buf[30], buf1[30];         /* enough fudge factor, yeah? */
	char patt[120], addr[120];
	char tugelem();

	mp = &message[mesg-1];
	cp = nameof(mp, 0);
	if( icequal(cp, str) )          /* Fast check */
		return 1;

	/*
	 * Try to match name given and source address of this message
	 */

	/* Try to get a user name from pattern */
	patt[0] = '\0';
	if( (p = index(str, '%')) != NULL ||
	    (p = index(str, '@')) != NULL ) {
		savc = *p;
		*p = '\0';
	}
	if( (q = rindex(str, '!')) != NULL ) {
		q++;
		savc1 = *q;
		*q = '\0';
		strcpy(patt, str);      /* uucp prefix */
		*q = savc1;
	} else
		q = str;
	strcpy(buf, q);
	if( p != NULL ) {
		*p = savc;
		strcat(patt, p);
	}

	/* Try to get a user name from address given */
	addr[0] = '\0';
	if( (p = index(cp, '%')) != NULL ||
	    (p = index(cp, '@')) != NULL ) {
		savc = *p;
		*p = '\0';
	}
	if( (q = rindex(cp, '!')) != NULL ) {
		q++;
		savc1 = *q;
		*q = '\0';
		strcpy(addr, cp);       /* uucp prefix */
		*q = savc1;
	} else
		q = cp;
	if( !(buf[0] == '*' && buf[1] == '\0') && !icequal(buf, q) ) {
		if( p != NULL )
			*p = savc;
		return 0;               /* user names differ - fatal */
	}
	if( p != NULL ) {
		*p = savc;
		strcat(addr, p);
	}

	/*
	 * Now we have host names with user names stripped
	 * Get components of name step-by-step from
	 * sender part to destination (from host name to universe).
	 */
	while( patt[0] != '\0' ) {
		if( addr[0] == '\0' )   /* no more components */
			return 0;
		savc  = tugelem(patt, buf);
    flushtail:  savc1 = tugelem(addr, buf1);
		if( savc != savc1 ) {
			if( savc1 == '.' )
				goto flushtail;
			return 0;
		}
		if( buf[0] == '*' && buf[1] == '\0' )
			continue;
		if( !icequal(buf, buf1) )
			return 0;       /* oops, mismatch */
	}
	return 1;
}

/*
 * Tug an element of network address.
 * Returns a delimitor (one of  @ . ! ).
 * Writes selected part into buf.
 */
char tugelem(addr, buf)
	char addr[];
	char buf[];
{
	register char *p, *q;
	char *index(), *rindex();
	char  x[120], sc, del;

	strcpy(x, addr);
	addr[0] = '\0';

	/*
	 * Highest priority is assigned to leftside delimitor(s)  % . . .
	 *      %.  we shall change to 1  (CTRL/A)
	 *      @.  we shall change to 2  (CTRL/B)
	 */
	if( (p = index(x, '\1'))  != NULL ||
	    (p = index(x, '%')) != NULL ) {
		del = *p;
		*p = '\0';
		strcpy(addr, x);
		q = p+1;
		while( *q && *q != '.' && *q != '%' && *q != '@' ) q++;
		if( *q == '.' ) {
			*q = '\0';
			strcpy(buf, p+1);
			*q = '\1';
			strcat(addr, q);
		} else if( *q == '@' || *q == '%' ) {
			sc = *q;
			*q = '\0';
			strcpy(buf, p+1);
			*q = sc;
			strcat(addr, q);
		} else                          /* End Of Address */
			strcpy(buf, p+1);
		return (del=='\1') ? '.' : '@';
	}

	/*
	 * Medium priority is assigned to rightside delimitor ...!
	 */
	if( (p = rindex(x, '!')) != NULL ) {
		if( p == x ) {  /* What a buzz ? */
			if( x[1] == '\0' )
				return 'E';     /* invalid - causes fail */
			strcpy(addr, x+1);
			strcpy(x, addr);
			addr[0] = '\0';
			goto skipuucp;
		}

		*p = '\0';
		if( (q = rindex(x, '!')) != NULL ) {
			strcpy(buf, q+1);
			q[1] = '\0';
			strcpy(addr, x);
		} else
			strcpy(buf, x);
		if( p[1] != '\0' )
			strcat(addr, p+1);
		return '!';
	}
skipuucp: ;

	/*
	 * Lowest priority is assigned to leftside delimitor(s)  @ . . .
	 */
	if( (p = index(x, '\2'))  != NULL ||
	    (p = index(x, '@')) != NULL ) {
		del = *p;
		q = p+1;
		while( *q && *q != '.' && *q != '@' ) q++;
		if( *q == '.' ) {
			*q = '\0';
			strcpy(buf, p+1);
			*q = '\2';
			strcpy(addr, q);
		} else if( *q == '@' ) {
			sc = *q;
			*q = '\0';
			strcpy(buf, p+1);
			*q = sc;
			strcpy(addr, q);
		} else {                        /* End Of Address */
			strcpy(buf, p+1);
			addr[0] = '\0';
		}
		return (del=='\2') ? '.' : '@';
	}
	strcpy(buf, x);
	return 'X';                     /* Unknown type of address */
}

/*
 * See if the given string matches inside the subject field of the
 * given message.  For the purpose of the scan, we ignore case differences.
 * If it does, return true.  The string search argument is assumed to
 * have the form "/search-string."  If it is of the form "/," we use the
 * previous search string.
 */

char lastscan[128];

matchsubj(str, mesg)
	char *str;
{
	register struct message *mp;
	register char *cp, *cp2, *backup;

	str++;
	if (strlen(str) == 0)
		str = lastscan;
	else
		strcpy(lastscan, str);
	mp = &message[mesg-1];

	/*
	 * Now look, ignoring case, for the word in the string.
	 */

	cp = str;
	cp2 = hfield("subject", mp);
	if (cp2 == NOSTR)
		return(0);
	backup = cp2;
	while (*cp2) {
		if (*cp == 0)
			return(1);
		if (toupper(*cp++) != toupper(*cp2++)) {
			cp2 = ++backup;
			cp = str;
		}
	}
	return(*cp == 0);
}

/*
 * Mark the named message by setting its mark bit.
 */

mark(mesg)
{
	register int i;

	i = mesg;
	if (i < 1 || i > msgCount)
		panic(ediag("Bad message number to mark",
				"pLOHOJ NOMER SOOB]ENIQ DLQ mark"));
	message[i-1].m_flag |= MMARK;
}

/*
 * Unmark the named message.
 */

unmark(mesg)
{
	register int i;

	i = mesg;
	if (i < 1 || i > msgCount)
		panic(ediag("Bad message number to unmark",
				"pLOHOJ NOMER SOOB]ENIQ DLQ unmark"));
	message[i-1].m_flag &= ~MMARK;
}

/*
 * Return the message number corresponding to the passed meta character.
 */

metamess(meta, f)
{
	register int c, m;
	register struct message *mp;

	c = meta;
	switch (c) {
	case '^':
		/*
		 * First 'good' message left.
		 */
		for (mp = &message[0]; mp < &message[msgCount]; mp++)
			if ((mp->m_flag & MDELETED) == f)
				return(mp - &message[0] + 1);
		printf(ediag("No applicable messages\n",
			"nET PODHODQ]IH SOOB]ENIJ\n"));
		return(-1);

	case '$':
		/*
		 * Last 'good message left.
		 */
		for (mp = &message[msgCount-1]; mp >= &message[0]; mp--)
			if ((mp->m_flag & MDELETED) == f)
				return(mp - &message[0] + 1);
		printf(ediag("No applicable messages\n",
			"nET PODHODQ]IH SOOB]ENIJ\n"));
		return(-1);

	case '.':
		/*
		 * Current message.
		 */
		m = dot - &message[0] + 1;
		if ((dot->m_flag & MDELETED) != f) {
			printf(ediag(
"%d: Inappropriate message\n",
"%d: nEPODHODQ]EE SOOB]ENIE\n"),
m);
			return(-1);
		}
		return(m);

	default:
		printf(ediag(
"Unknown metachar (%c)\n",
"nEWERNYJ METASIMWOL (%c)\n"),
c);
		return(-1);
	}
}
