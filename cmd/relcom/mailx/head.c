#include "rcv.h"

/*
 * Mail -- a mail program
 *
 * Routines for processing and detecting headlines.
 *
 * $Log: head.c,v $
 * Revision 1.12  1990/12/22  22:54:39  ache
 * sORTIROWKA + WYDA^A fio
 *
 * Revision 1.11  90/12/03  03:45:35  ache
 * pRAWLEN RAZNOS STEKA
 * 
 * Revision 1.10  90/10/16  09:09:11  ache
 * tEPERX NE PADAET NA NEWERNOM ZAGOLOWKE.
 * 
 * Revision 1.9  90/09/29  19:06:34  ache
 * isupper, islower added.
 * 
 * Revision 1.8  90/09/29  18:57:08  ache
 * Added MSDOS alternate code CTYPE and Yo!
 * 
 * Revision 1.7  90/09/29  18:21:48  ache
 * <ctype.h> kicked out...
 * 
 * Revision 1.6  90/09/21  21:58:52  ache
 * MS-DOS extends + some new stuff
 * 
 * Revision 1.5  90/09/13  13:19:24  ache
 * MS-DOS & Unix together...
 * 
 * Revision 1.4  90/06/10  00:44:58  avg
 * pRAWLENA FUNKCIQ raise NA PREDMET RUSSKIH BUKW.
 * 
 * Revision 1.3  88/07/23  20:33:59  ache
 * rUSSKIE DIAGNOSTIKI
 * 
 * Revision 1.2  88/01/11  12:42:20  avg
 * dOBAWLEN NOXSTR U rcsid.
 * 
 * Revision 1.1  87/12/25  15:59:37  avg
 * Initial revision
 * 
 */

/*NOXSTR
static char rcsid[] = "$Header: /usr/src/Relcom/relcom/mailx/RCS/head.c,v 1.12 1990/12/22 22:54:39 ache Exp $";
YESXSTR*/
static void fail();

/*
 * See if the passed line buffer is a mail header.
 * Return true if yes.  Note the extreme pains to
 * accomodate all funny formats.
 */

ishead(linebuf)
	char linebuf[];
{
	register char *cp;
	struct headline hl;
	char parbuf[BUFSIZ];

	cp = linebuf;
	if (*cp != 'F' || strncmp("From ", cp, 5) != 0)
		return(0);
	parse(cp, &hl, parbuf);
	if (hl.l_from == NOSTR || hl.l_date == NOSTR) {
		fail(linebuf, ediag(
"no from or date field",
"NET NI POLQ from NI POLQ date"));
		return(0);
	}
	if (!isdate(hl.l_date)) {
		fail(linebuf, ediag(
"date field not legal date",
"W POLE date NEWERNAQ DATA"));
		return(0);
	}

	/*
	 * I guess we got it!
	 */

	return(1);
}

static
void
fail(linebuf, reason)
	char linebuf[], reason[];
{

	if (value("debug") == NOSTR)
		return;
	fprintf(stderr, ediag(
"\"%s\"\nnot a header because %s\n",
"\"%s\"\nNE ZAGOLOWOK, POTOMU ^TO %s\n"),
linebuf, reason);
}

/*
 * Split a headline into its useful components.
 * Copy the line into dynamic string space, then set
 * pointers into the copied line in the passed headline
 * structure.  Actually, it scans.
 */

parse(line, hl, pbuf)
	char line[], pbuf[];
	struct headline *hl;
{
	register char *cp, *dp;
	char *sp;
	char word[LINESIZE];

	hl->l_from = NOSTR;
	hl->l_tty = NOSTR;
	hl->l_date = NOSTR;
	cp = line;
	sp = pbuf;

	/*
	 * Skip the first "word" of the line, which should be "From"
	 * anyway.
	 */

	cp = nextword(cp, word);
	dp = nextword(cp, word);
	if (*word != '\0')
		hl->l_from = copyin(word, &sp);
	if (dp != NOSTR) {
		if (strncmp(dp, "tty", 3) == 0) {
			cp = nextword(dp, word);
			hl->l_tty = copyin(word, &sp);
			if (cp != NOSTR)
				hl->l_date = copyin(cp, &sp);
		}
		else
			hl->l_date = copyin(dp, &sp);
	}
	if (sp - pbuf > BUFSIZ)
		panic("Parse buf overflow");
}

/*
 * Copy the string on the left into the string on the right
 * and bump the right (reference) string pointer by the length.
 * Thus, dynamically allocate space in the right string, copying
 * the left string into it.
 */

char *
copyin(src, space)
	char src[];
	char **space;
{
	register char *cp, *top;
	register int s;

	s = strlen(src);
	cp = *space;
	top = cp;
	strcpy(cp, src);
	cp += s + 1;
	*space = cp;
	return(top);
}

/*
 * Test to see if the passed string is a ctime(3) generated
 * date string as documented in the manual.  The template
 * below is used as the criterion of correctness.
 * Also, we check for a possible trailing time zone using
 * the auxtype template.
 */

#define L       1               /* A lower case char */
#define S       2               /* A space */
#define D       3               /* A digit */
#define O       4               /* An optional digit or space */
#define C       5               /* A colon */
#define N       6               /* A new line */
#define U       7               /* An upper case char */
#define Z       8               /* Plus or minus */

char ctypes[] = {U,L,L,S,U,L,L,S,O,D,S,D,D,C,D,D,C,D,D,S,D,D,D,D,0};
char Ctypes[] = {L,L,L,S,L,L,L,S,O,D,S,D,D,C,D,D,C,D,D,S,D,D,D,D,0};
char tmztypes[] = {U,L,L,S,U,L,L,S,O,D,S,D,D,C,D,D,C,D,D,S,U,U,U,S,D,D,D,D,0};
char offtypes[] = {U,L,L,S,U,L,L,S,O,D,S,D,D,C,D,D,C,D,D,S,Z,D,D,D,D,S,D,D,D,D,0};

isdate(date)
	char date[];
{
	register char *cp;

	cp = date;
	if (cmatch(cp, ctypes))
		return(1);
	if (cmatch(cp, Ctypes))
		return(1);
	if (cmatch(cp, tmztypes))
		return(1);
	return(cmatch(cp, offtypes));
}

/*
 * Match the given string against the given template.
 * Return 1 if they match, 0 if they don't
 */

cmatch(str, temp)
	char str[], temp[];
{
	register char *cp, *tp;
	register int c;

	cp = str;
	tp = temp;
	while (*cp != '\0' && *tp != 0) {
		c = (*cp++)&0377;
		switch (*tp++) {
		case Z:
			if (c != '+' && c != '-')
				return(0);
			break;

		case L:
			if (!isalpha(c) || !islower(c))
				return(0);
			break;

		case U:
			if (!isalpha(c) || !isupper(c))
				return(0);
			break;

		case S:
			if (!isspace(c))
				return(0);
			break;

		case D:
			if (!isdigit(c))
				return(0);
			break;

		case O:
			if (!isspace(c) && !isdigit(c))
				return(0);
			break;

		case C:
			if (c != ':')
				return(0);
			break;

		case N:
			if (c != '\n')
				return(0);
			break;
		}
	}
	if (*cp != '\0' || *tp != 0)
		return(0);
	return(1);
}

/*
 * Collect a liberal (space, tab delimited) word into the word buffer
 * passed.  Also, return a pointer to the next word following that,
 * or NOSTR if none follow.
 */

char *
nextword(wp, wbuf)
	char wp[], wbuf[];
{
	register char *cp, *cp2;

	if ((cp = wp) == NOSTR) {
		copy("", wbuf);
		return(NOSTR);
	}
	cp2 = wbuf;
	while (!any(*cp, " \t") && *cp != '\0')
		if (*cp == '"') {
			*cp2++ = *cp++;
			while (*cp != '\0' && *cp != '"')
				*cp2++ = *cp++;
			if (*cp == '"')
				*cp2++ = *cp++;
		} else
			*cp2++ = *cp++;
	*cp2 = '\0';
	while (any(*cp, " \t"))
		cp++;
	if (*cp == '\0')
		return(NOSTR);
	return(cp);
}

isprint(c)
register int c;
{
	return (c >= ' ' && c < 0177) ||
#ifndef MSDOS
	       ((c & 0300) == 0300)   ||
	       (c == 0243 || c == 0263) ;       /* Yo */
#else
		(c >= 0x80 && c <= 0xaf) ||
		(c >= 0xe0 && c <= 0xf1) ;
#endif
}

/*
 * Test to see if the character is an ascii alphabetic.
 */

isalpha(c)
register int c;
{
	return (c >= 'a' && c <= 'z') ||
	       (c >= 'A' && c <= 'Z') ||
#ifndef MSDOS
	       ((c & 0300) == 0300)   ||          /* koi-8 */
	       (c == 0243 || c == 0263) ;       /* Yo */
#else
		(c >= 0x80 && c <= 0xaf) ||
		(c >= 0xe0 && c <= 0xf1) ;
#endif
}

/*
 * Test to see if the character is an ascii digit.
 */

isdigit(c)
register int c;
{
	return(c >= '0' && c <= '9');
}

isspace(c)
register int c;
{
	return(c == ' ' || c == '\t' || c == '\n');
}

islower(c)
	register int c;
{
	c &= 0xff;
	return c >= 'a' && c <= 'z'
#ifndef MSDOS
	    || c >= 0300 && c <= 0337  /* koi-8, russian letters */
	    || c == 0243               /* Yo */
#else
	    || c >= 0xa0 && c <= 0xaf
	    || c >= 0xe0 && c <= 0xef
	    || c == 0xf1               /* Yo */
#endif
	;
}

/*
 * Convert lower case letters to upper case.
 */

toupper(c)
	register int c;
{
	c &= 0xff;
	if (   c >= 'a' && c <= 'z'
#ifndef MSDOS
	    || c >= 0300 && c <= 0337  /* koi-8, russian letters */
#endif
	   )
		c ^= 040;
#ifndef MSDOS
	else if (c == 0243)     /* Yo */
		c = 0263;
#else
	else if (c >= 0xa0 && c <= 0xaf)
		c -= 0x20;
	else if (c >= 0xe0 && c <= 0xef)
		c -= 0x50;
	else if (c == 0xf1)     /* Yo */
		c--;
#endif
	return(c);
}

isupper(c)
	register int c;
{
	c &= 0xff;
	return c >= 'A' && c <= 'Z'
#ifndef MSDOS
	    || c >= 0340 && c <= 0377   /* Damned KOI8 */
	    || c == 0263                /* Yo */
#else
	    || c >= 0x80 && c <= 0x8f
	    || c >= 0x90 && c <= 0x9f
	    || c == 0xf0                /* Yo */
#endif
	;
}

/*
 * Make a little character.
 */

tolower(c)
	register int c;
{
	c &= 0xff;
	if (   c >= 'A' && c <= 'Z'
#ifndef MSDOS
	    || c >= 0340 && c <= 0377   /* Damned KOI8 */
#endif
	   )
		c ^= 040;
#ifndef MSDOS
	else if (c == 0263)     /* Yo */
		c = 0243;
#else
	else if (c >= 0x80 && c <= 0x8f)
		c += 0x20;
	else if (c >= 0x90 && c <= 0x9f)
		c += 0x50;
	else if (c == 0xf0)     /* Yo */
		c++;
#endif
	return(c);
}

/*
 * Copy str1 to str2, return pointer to null in str2.
 */

char *
copy(str1, str2)
	char *str1, *str2;
{
	register char *s1, *s2;

	s1 = str1;
	s2 = str2;
	while (*s1)
		*s2++ = *s1++;
	*s2 = 0;
	return(s2);
}

/*
 * Is ch any of the characters in str?
 */

any(ch, str)
	char *str;
{
	register char *f;
	register c;

	f = str;
	c = ch;
	while (*f)
		if (c == *f++)
			return(1);
	return(0);
}

/*
 * lEKSIKOGRAFI^ESKOE SRAWNENIE BUKW
 *      lcmp(c1, c2) = 0  ESLI c1 == c2
 *                   > 0  -//- c1 > c2
 *                   < 0  -//- c1 < c2
 *      Ctou(c)         koi-8 ---> U-KOD
 *      Ctok(c)         U-KOD ---> koi-8
 */
/*NOXXSTR*/

#ifndef MSDOS
static char _utok[] = {
	0341, 0342, 0367, 0347, 0344, 0345, 0366, 0372,
	0351, 0352, 0353, 0354, 0355, 0356, 0357, 0360,
	0362, 0363, 0364, 0365, 0346, 0350, 0343, 0376,
	0373, 0375, 0377, 0371, 0370, 0374, 0340, 0361,
	0301, 0302, 0327, 0307, 0304, 0305, 0326, 0332,
	0311, 0312, 0313, 0314, 0315, 0316, 0317, 0320,
	0322, 0323, 0324, 0325, 0306, 0310, 0303, 0336,
	0333, 0335, 0337, 0331, 0330, 0334, 0300, 0321
};
static char _ktou[] = {
	0376, 0340, 0341, 0366, 0344, 0345, 0364, 0343,
	0365, 0350, 0351, 0352, 0353, 0354, 0355, 0356,
	0357, 0377, 0360, 0361, 0362, 0363, 0346, 0342,
	0374, 0373, 0347, 0370, 0375, 0371, 0367, 0372,
	0336, 0300, 0301, 0326, 0304, 0305, 0324, 0303,
	0325, 0310, 0311, 0312, 0313, 0314, 0315, 0316,
	0317, 0337, 0320, 0321, 0322, 0323, 0306, 0302,
	0334, 0333, 0307, 0330, 0335, 0331, 0327, 0332
};
static int Ctok(c) register c; {return (((c&0300)==0300)?_utok[c&077]:c)&0377;}
static int Ctou(c) register c; {return (((c&0300)==0300)?_ktou[c&077]:c)&0377;}

int lcmp(c1, c2) {return Ctou(c1) - Ctou(c2);}
#else   /* MSDOS */
int lcmp(c1, c2) register char c1, c2; { return (c1&0377)-(c2&0377); }
#endif  /* MSDOS */

int
strlcmp(s1, s2)
register char *s1, *s2;
{
	while (*s1 && *s1 == *s2)
		s1++, s2++;
	return lcmp(*s1, *s2);
}

int
istrlcmp(s1, s2)
register char *s1, *s2;
{
	while (*s1 && toupper(*s1) == toupper(*s2))
		s1++, s2++;
	return lcmp(toupper(*s1), toupper(*s2));
}

int
inumlcmp(s1, s2)
register char *s1, *s2;
{
	char *st1 = s1, *sc1;
	char *st2 = s2, *sc2;
	long i1, i2, atol();

Again:
	while (*s1 && toupper(*s1) == toupper(*s2))
		s1++, s2++;
	if (isdigit(*s1) || isdigit(*s2)) {
		sc1 = s1;
		sc2 = s2;
		while(s1 > st1 && isdigit(s1[-1]))
			s1--;
		while(s2 > st2 && isdigit(s2[-1]))
			s2--;
		if (isdigit(*s1) && isdigit(*s2)) {
			i1 = atol(s1);
			i2 = atol(s2);
			if (i1 != i2)
				return  ((i1 < i2) ? -1 : 1);
			while(isdigit(*s1))
				s1++;
			while(isdigit(*s2))
				s2++;
			goto Again;
		}
		s1 = sc1;
		s2 = sc2;
	}
	return lcmp(toupper(*s1), toupper(*s2));
}
