# include "scr.h"
# include "rec.h"

int viewfd;
int viewraw, viewhex, viewtabs; /* modes */
long viewseek;                  /* current seek for hex/text switches */
int viewhlen;                   /* number of bytes */
char viewsrch [SEARCHSZ];       /* search string */
char viewsbuf [SEARCHSZ];       /* search string buffer */
int viewsbsz;                   /* search string size */
REC *viewrec;

static textview (), viewhead ();
extern char *getstring (), *strcpy ();

viewfile (d, filename)
char *filename;
{
	viewfd = d;
	viewrec = RecOpen (viewfd, 0);
	if (! viewrec)
		return;
	viewseek = 0;
	viewhlen = (viewrec->size + 15) / 16;
	for (;;) {
		VClearBox (1, 0, LINES-2, COLS);
		if (viewhex) {
			if (! hexview (filename))
				break;
			viewhex = 0;
		} else {
			if (! textview (filename))
				break;
			viewhex = 1;
		}
	}
	VClearBox (1, 0, LINES-2, COLS);
	RecClose (viewrec);
}

static textview (filename)
char *filename;
{
	register baseline, basecol;
	int steplr = 8 + (COLS-1)/16*8;         /* step left-right */
	int stepud = LINES-3;                   /* step up-down */
	int ccol;                               /* cursor position */
	int sline, soff;                        /* search position */

	baseline = basecol = 0;
	while (viewrec->lindex[baseline+1].seek < viewseek)
		++baseline;
	while (viewrec->lindex[baseline].seek + offset (baseline, basecol) + 40 < viewseek)
		basecol += 40;
	ccol = 0;
	sline = soff = -1;
	VMPutString (LINES-1, 0, "\0011\16Help  \17 2\16      \17 3\16Raw   \17 4\16Hex   \17 5\16Top   \17 6\16Bottom\17 7\16Search\17 8\16      \17 9\16Spaces\01710\16Quit \17\2");
	for (;;) {
		viewpage (baseline, basecol);
		for (;;) {
			viewhead (filename, baseline, basecol);
			if (sline>=baseline && sline<baseline+LINES-2 &&
			    ccol>=basecol && ccol<basecol+80)
				VMove (sline-baseline+1, ccol-basecol);
			else {
				hidecursor ();
				sline = soff = -1;
			}
			VSync ();
			switch (KeyGet ()) {
			case meta ('A'):        /* f1 */
				runhelp ("view");
				continue;
			default:
				VBeep ();
				continue;
			case cntrl (']'):       /* redraw screen */
				VRedraw ();
				continue;
			case meta ('C'):        /* f3 - raw */
				viewraw ^= 1;
				break;
			case meta ('D'):        /* f4 - hex */
				viewseek = viewrec->lindex[baseline].seek + offset (baseline, basecol);
				return (1);
			case cntrl ('C'):
			case cntrl ('['):
			case meta ('J'):        /* f0 - quit */
				return (0);
			case meta ('I'):        /* f9 - spaces */
				viewtabs ^= 1;
				break;
			case meta ('G'): {      /* f7 - search */
				register char *p;

				if (! (p = getstring (SEARCHSZ-1, viewsrch, " Search ", "Search for the string")))
					continue;
				strcpy (viewsrch, p);
				if (sline < 0) {
					sline = baseline;
					soff = offset (baseline, basecol) - 1;
				}
				if (search (p, sline, soff+1, &sline, &soff)) {
					if (baseline > sline || baseline + LINES-2 <= sline)
						baseline = sline;
					if (baseline > viewrec->len - (LINES-2))
						baseline = viewrec->len - (LINES-2);
					if (baseline < 0)
						baseline = 0;
					ccol = column (sline, soff);
					if (ccol >= 40)
						basecol = (ccol + strlen (p) - 41) / 40 * 40;
					else
						basecol = 0;
					break;
				} else
					sline = soff = -1;
				continue;
			}
			case cntrl ('E'):
			case meta ('u'):        /* up */
				if (baseline <= 0)
					continue;
				--baseline;
				VDelLine (LINES-2);
				VInsLine (1);
				viewline (baseline, basecol, 1);
				continue;
			case cntrl ('M'):
			case cntrl ('J'):       /* down */
				if (basecol) {
					basecol = 0;
					break;
				}
				/* fall through */
			case cntrl ('X'):
			case meta ('d'):        /* down */
				if (baseline >= viewrec->len - (LINES-2))
					continue;
				VDelLine (1);
				VInsLine (LINES-2);
				viewline (++baseline+LINES-3, basecol, LINES-2);
				continue;
			case cntrl ('F'):
			case cntrl ('D'):
			case meta ('r'):        /* right */
				basecol += steplr;
				break;
			case cntrl ('S'):
			case cntrl ('A'):
			case cntrl ('W'):
			case meta ('l'):        /* left */
				if ((basecol -= steplr) < 0)
					basecol = 0;
				break;
			case cntrl ('B'):
			case meta ('n'):        /* next page */
				if (baseline >= viewrec->len - (LINES-2))
					continue;
				baseline += stepud;
				if (baseline > viewrec->len - (LINES-2))
					baseline = viewrec->len - (LINES-2);
				break;
			case cntrl ('R'):
			case meta ('p'):        /* prev page */
				if (baseline <= 0)
					continue;
				baseline -= stepud;
				if (baseline < 0)
					baseline = 0;
				break;
			case meta ('E'):        /* top */
			case meta ('h'):        /* home */
				if (baseline==0 && basecol==0)
					continue;
				baseline = basecol = 0;
				break;
			case meta ('F'):        /* bottom */
			case meta ('e'):        /* end */
				if (baseline >= viewrec->len - (LINES-2) && basecol==0)
					continue;
				baseline = viewrec->len - (LINES-2);
				basecol = 0;
				break;
			}
			break;
		}
	}
}

static viewhead (filename, line, col)
char *filename;
{
	register i;

	VSetDim ();
	VStandOut ();
	VMove (0, 0);
	for (i=80; --i>=0; VPutChar (' '));
	VMPutString (0, 1, "File ");
	VPutString (filename);
	VMove (0, 22);
	printw ("Lines %-5d Bytes %ld", viewrec->len, viewrec->size);
	VMove (0, 57);
	printw ("Line %-5d Col %d", line+1, col);
	VSetNormal ();
	VStandEnd ();
}

viewpage (line, col)
{
	register i;
	int len;

	len = viewrec->len - line;
	if (len > LINES-2)
		len = LINES-2;
	for (i=0; i<len; ++i)
		viewline (i+line, col, i+1);
}

viewline (line, col, scrline)
register col;
{
	register LINE *l;
	register char *p;
	char *e;
	register len;

	VMove (scrline, 0);                     /* move to line */
	VClearLine ();                          /* clear it */
	l = RecGet (viewrec, line);
	if (! l || ! l->slen)                   /* get line */
		return;                         /* check if empty */
	p = l->ptr;                             /* set ptr to line */
	e = p + l->slen;                        /* set ptr to end */
	len = COLS;                             /* width of print */
	if (col)
		p += offset (line, col);
	if (p >= e)
		return;
	while (--len >= 0 && p < e)             /* print char loop */
		if (*p == '\t') {               /* print tab */
			if (viewtabs) {
				VSetDim ();
				VPutChar ('_');
				VSetNormal ();
			}
			len = len / 8 * 8;
			VMove (scrline, COLS-len);
			++p;
		} else
			viewchar (*p++);        /* else print char */
	if (p < e) {                            /* put tag */
		if (len < 0)
			VMove (scrline, COLS-1);
		VSetBold ();
		VPutChar ('>');                 /* "continued" */
		VSetNormal ();
	} else if (l->noeoln) {
		VSetBold ();
		VPutChar ('\\');                /* "broken" */
		VSetNormal ();
	}
}

viewchar (c)
register c;
{
	register hibit;

	if (c == ' ') {
		if (viewhex || viewtabs) {
			VSetDim ();
			VPutChar ('.');
			VSetNormal ();
		} else
			VPutChar (' ');
	} else if (viewraw) {
		c &= 0377;
		if (c >= ' ')
			VPutChar (c);
		else {
			VSetDim ();
			VPutChar ((c + 0100) & 0177);
			VSetNormal ();
		}
	} else {
		if (hibit = (c & 0200))
			VStandOut ();
		c &= 0177;
		if (c >= ' ' && c <= '~')
			VPutChar (c);
		else {
			VSetDim ();
			VPutChar ((c + 0100) & 0177);
			VSetNormal ();
		}
		if (hibit)
			VStandEnd ();
	}
}

search (str, l, c, pline, pcol)
char *str;
int *pline, *pcol;
{
	LINE *p;
	register char *s, *e;
	int len = strlen (str);

	for (; l<viewrec->len && (p = RecGet (viewrec, l)); ++l) {
		s = p->ptr;
		e = p->ptr + p->slen - len + 1;
		if (c) {
			s += c;
			c = 0;
		}
		for (; s<e; ++s)
			if (! strncmp (s, str, len)) {
				*pline = l;
				*pcol = s - p->ptr;
				return (1);
			}
	}
	error ("String not found");
	return (0);
}

offset (l, n)
register n;
{
	LINE *p;
	register char *s, *e;
	register len;

	if (! (p = RecGet (viewrec, l)))
		return (0);
	s = p->ptr;
	e = s + p->slen;
	for (len=0; s<e; ++s) {
		if (*s == '\t')         /* handle tabs correctly */
			len = (len + 8) / 8 * 8;
		else
			++len;
		if (len > n)
			break;
	}
	return (s - p->ptr);
}

column (l, n)
{
	LINE *p;
	register char *s, *e;
	register len;

	if (! (p = RecGet (viewrec, l)))
		return (0);
	if (n > p->slen)
		n = p->slen;
	s = p->ptr;
	e = s + n;
	/* count length of string as it appears on screen */
	for (len=0; s<e; ++s)
		if (*s == '\t')         /* handle tabs correctly */
			len = (len + 8) / 8 * 8;
		else
			++len;
	return (len);
}
