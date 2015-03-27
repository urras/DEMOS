# include "scr.h"
# include "dir.h"
# include "deco.h"

# ifndef NULL
#    define NULL        ((char *) 0)
# endif

# define SWAP(a,b)      { register t; t = a; a = b; b = t; }

struct choice {
	char *cname;
	int namlen;
	int cr, c;
};

static struct choice tab [3];
static cnum;
static (*attron) (), (*attroff) ();
static BOX *mbox;

static initchoice (), menuchoice (), drawchoice ();
extern VSetDim (), VSetBold (), VStandOut (), VStandEnd ();
extern char *strncpy (), *mdup ();

/* VARARGS 1 */
error (s, aa, bb, cc, dd)
char *s;
long aa, bb, cc, dd;
{
	char buf [100];

	sprintf (buf, s, aa, bb, cc, dd);
	getchoice (1, " Error ", buf, NULL, " Ok ", NULL, NULL);
}

/* VARARGS 1 */
message (name, s, aa, bb, cc, dd)
char *name, *s;
long aa, bb, cc, dd;
{
	char buf [100];

	sprintf (buf, s, aa, bb, cc, dd);
	getchoice (0, name, buf, NULL, NULL, NULL, NULL);
}

endmesg ()
{
	if (mbox)
		VUngetBox (mbox);
	mbox = 0;
}

getchoice (bold, head, mesg, mesg2, c1, c2, c3)
char *head, *mesg, *mesg2, *c1, *c2, *c3;
{
	int len, ch;
	int isattr;
	register r, c, w, h;
	BOX *box;

	w = strlen (mesg);
	if (mesg2) {
		len = strlen (mesg2);
		if (len > w)
			w = len;
	}
	len = strlen (head);
	if (len > w)
		w = len;
	len = 0;
	if (c1)
		len += strlen (c1);
	if (c2)
		len += strlen (c2);
	if (c3)
		len += strlen (c3);
	if (len > w)
		w = len;
	h = 6;
	w += 10;
	if (bold)
		r = LINES/2;
	else
		r = LINES/4;
	c = (80 - w) / 2;

	box = VGetBox (r-1, c-2, h+2, w+4);             /* save box */
	isattr = VStandOut ();
	if (bold)
		VSetBold ();
	else
		VSetDim ();
	VFillBox (r-1, c-2, h+2, w+4, ' ');             /* clear */
	VDrawBox (r, c, h, w);                          /* draw margins */
	VMPutString (r, c + (w-strlen(head)) / 2, head); /* head */
	if (mesg2) {
		VMPutString (r+1, c + (w-strlen(mesg)) / 2, mesg);
		VMPutString (r+2, c + (w-strlen(mesg2)) / 2, mesg2);
	} else
		VMPutString (r+2, c + (w-strlen(mesg)) / 2, mesg);

	if (c1) {
		if (isattr) {
			attron = VStandEnd;
			attroff = VStandOut;
		} else
			attron = attroff = 0;

		initchoice (r+4, c+w/2, c1, c2, c3);

		ch = menuchoice ();
		VStandEnd ();
		VSetNormal ();
		VUngetBox (box);
		VFreeBox (box);
		return (ch);
	} else {
		/* message */
		VStandEnd ();
		VSetNormal ();
		mbox = box;
		hidecursor ();
		VSync ();
# ifdef DOC
		KeyGet ();
# endif
		return (0);
	}
}

static initchoice (row, col, c1, c2, c3)
char *c1, *c2, *c3;
{
	register i, w;

	cnum = c2 ? (c3 ? 3 : 2) : 1;
	tab[0].cname = c1;
	tab[1].cname = c2;
	tab[2].cname = c3;
	w = cnum-1;
	for (i=0; i<cnum; ++i) {
		w += tab[i].namlen = strlen (tab[i].cname);
		tab[i].cr = row;
	}
	tab[0].c = col-w/2;
	tab[1].c = tab[0].c + tab[0].namlen + 1;
	tab[2].c = tab[1].c + tab[1].namlen + 1;
}

static menuchoice ()
{
	int ch = 0;

	for (;;) {
		drawchoice (ch);
		hidecursor ();
		VSync ();
		switch (KeyGet ()) {
		default:
			VBeep ();
			continue;
		case cntrl (']'):       /* redraw screen */
			VRedraw ();
			continue;
		case cntrl ('['):
		case cntrl ('C'):
		case meta ('J'):        /* f0 */
			return (-1);
		case cntrl ('M'):
		case cntrl ('J'):
			return (ch);
		case ' ':
		case cntrl ('I'):
		case meta ('r'):        /* right */
			if (++ch >= cnum)
				ch = 0;
			continue;
		case cntrl ('H'):
		case meta ('l'):        /* left */
			if (--ch < 0)
				ch = cnum-1;
			continue;
		}
	}
}

static drawchoice (ch)
{
	register i;

	for (i=0; i<cnum; ++i) {
		if (i == ch) {
			if (attron)
				(*attron) ();
			VMPutString (tab[i].cr, tab[i].c, tab[i].cname);
			if (! attron) {
				VMPutChar (tab[i].cr, tab[i].c, '[');
				VMPutChar (tab[i].cr, tab[i].c + strlen (tab[i].cname) - 1, ']');
			}
			if (attroff)
				(*attroff) ();
		} else
			VMPutString (tab[i].cr, tab[i].c, tab[i].cname);
	}
}

static char *editstring (r, c, w, str, cp)
char *str;
register cp;                                    /* cursor position */
{
	register key, k;
	int firstkey = 1;

	if (cp) {
		for (cp=0; str[cp]; ++cp);
		firstkey = 0;
	}
	for (; ; firstkey=0) {
		VClearBox (r, c, 1, w);
		VMPutString (r, c, str);
		VMove (r, c+cp);
		VSync ();
		switch (key = KeyGet ()) {
		default:
			if (key<' ' || key>'~' && key<0300 || key>0376) {
				VBeep ();
				continue;
			}
			if (firstkey) {
				str[0] = key;
				str[1] = 0;
				cp = 1;
				continue;
			}
			for (k=cp; str[k]; ++k)
				SWAP (key, str[k]);
			str [k] = key;
			str [w] = str [k+1] = 0;
			/* fall through */
		case meta ('r'):        /* right */
			if (str [cp]) {
				++cp;
				if (cp >= w)
					cp = w-1;
			}
			continue;
		case meta ('l'):        /* left */
			if (--cp < 0)
				cp = 0;
			continue;
		case cntrl (']'):       /* redraw screen */
			VRedraw ();
			continue;
		case cntrl ('C'):
		case cntrl ('['):
		case meta ('J'):        /* f0 */
			return (0);
		case cntrl ('M'):
		case cntrl ('J'):
			return (str);
		case cntrl ('I'):
			if (str [cp])
				while (str [++cp]);
			else
				cp = 0;
			continue;
		case meta ('h'):        /* home */
			cp = 0;
			continue;
		case meta ('e'):        /* end */
			while (str [cp])
				++cp;
			continue;
		case cntrl ('H'):               /* back space */
			if (cp) {
				for (k=cp--; str[k]; ++k)
					str[k-1] = str[k];
				str [k-1] = 0;
			}
			continue;
		case cntrl ('G'):               /* delete */
			if (! str [cp])
				continue;
			for (k=cp+1; str[k]; ++k)
				str[k-1] = str[k];
			str [k-1] = 0;
			continue;
		case cntrl ('Y'):               /* clear line */
			str [cp = 0] = 0;
			continue;
		}
	}
}

char *getstring (w, str, head, mesg)
char *str, *head, *mesg;
register w;
{
	register r, c, h;
	int len;
	BOX *box;
	static char buf [81];

	len = strlen (mesg);
	if (len > w)
		w = len;
	len = strlen (head);
	if (len > w)
		w = len;
	h = 4;
	w += 4;
	r = LINES/4;
	c = (78 - w) / 2;

	box = VGetBox (r-1, c-2, h+2, w+4);             /* save box */
	VStandOut ();
	VSetDim ();
	VFillBox (r-1, c-2, h+2, w+4, ' ');             /* clear */
	VDrawBox (r, c, h, w);                          /* draw margins */
	VMPutString (r, c + (w-strlen(head)) / 2, head); /* head */
	VMPutString (r+1, c+2, mesg);                   /* message */
	VStandEnd ();
	VSetNormal ();

	strncpy (buf, str ? str : "", 80);

	str = editstring (r+2, c+2, w-4, buf, 0);
	VUngetBox (box);
	VFreeBox (box);
	return (str);
}

char *getwstring (w, str, head, mesg)
char *str, *head, *mesg;
register w;
{
	register r, c, h;
	int len;
	BOX *box;
	static char buf [81];

	len = strlen (mesg);
	if (len > w)
		w = len;
	len = strlen (head);
	if (len > w)
		w = len;
	h = 4;
	w += 4;
	r = LINES/4;
	c = BASECOL (cur) + (PAGEWID - w) / 2;
	if (c < 3)
		c = 3;
	else if (c > 76-w)
		c = 76-w;

	box = VGetBox (r-1, c-2, h+2, w+4);             /* save box */
	VStandOut ();
	VSetDim ();
	VFillBox (r-1, c-2, h+2, w+4, ' ');             /* clear */
	VDrawBox (r, c, h, w);                          /* draw margins */
	VMPutString (r, c + (w-strlen(head)) / 2, head); /* head */
	VMPutString (r+1, c+2, mesg);                   /* message */
	VStandEnd ();
	VSetNormal ();

	strncpy (buf, str ? str : "", 80);

	str = editstring (r+2, c+2, w-4, buf, 1);
	VUngetBox (box);
	VFreeBox (box);
	return (str);
}
