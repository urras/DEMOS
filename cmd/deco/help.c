# include "scr.h"
# include "env.h"

# define CS             34
# define HELPFILE       "/usr/lib/deco/help/"

struct helptab {
	char row;
	char col;
	char bold;
	char *str;
};

struct help {
	char *name;
	struct helptab *htab;
	char **mesg;
};

static char *genmesg [] = {
	"The Demos Commander, Copyright (C) 1989, 1990 Demos/*, Moscow",
	" Cursor Arrows ",
	"Left             ^[ l",
	"Right            ^[ r",
	"Up               ^[ u",
	"Down             ^[ d",
	" Cursor Movement ",
	"Char left        ^S or ^A",
	"Char right       ^D",
	" Panels ",
	"Switch panels    ^I or <Tab>",
	"Panels on/off    ^O or ^P",
	"Swap panels      ^U",
	"Toggle status    ^L",
	"Full screen      ^F",
	"Double width     ^W",
	"The \"+\" and \"-\" keys select and unselect groups of files.",
	" Delete ",
	"Char left        ^H or <BS>",
	"Under cursor     ^G or <Del>",
	"Line             ^Y",
	" Other ",
	"History          ^B",
	"Home directory   ^\\",
	"Reread directory ^R",
	"Select/Unselect  ^T or <Ins>",
	"Previous command ^E",
	"Next command     ^X",
	"Run file or",
	"Enter directory  ^M or <Ret>",
	"Use file name    ^J or <LF>",
	"Function keys    ^[ + digit",
	0,
};

static struct helptab gentab [] = {
	1,      0,      0,      0,
    /*  2, */
	3,      2,      1,      0,
	4,      6,      0,      0,
	5,      6,      0,      0,
	6,      6,      0,      0,
	7,      6,      0,      0,
	8,      2,      1,      0,
	9,      6,      0,      0,
       10,      6,      0,      0,
       11,      2,      1,      0,
       12,      6,      0,      0,
       13,      6,      0,      0,
       14,      6,      0,      0,
       15,      6,      0,      0,
       16,      6,      0,      0,
       17,      6,      0,      0,
    /* 18, */
       19,      0,      0,      0,
    /*  2, */
	3,  CS+ 2,      1,      0,
	4,  CS+ 6,      0,      0,
	5,  CS+ 6,      0,      0,
	6,  CS+ 6,      0,      0,
	7,  CS+ 2,      1,      0,
	8,  CS+ 6,      0,      0,
	9,  CS+ 6,      0,      0,
       10,  CS+ 6,      0,      0,
       11,  CS+ 6,      0,      0,
       12,  CS+ 6,      0,      0,
       13,  CS+ 6,      0,      0,
       14,  CS+ 6,      0,      0,
       15,  CS+ 6,      0,      0,
       16,  CS+ 6,      0,      0,
       17,  CS+ 6,      0,      0,
	0,      0,      0,      0,
};

static char *editmesg [] = {
	"The Demos Commander Text Editor, Copyright (C) 1990 Demos/*",
	" Cursor Movement ",
	"Char Left        ^A",
	"Char Right       ^D",
	"Line Up          ^E",
	"Line Down        ^X",
	"Word Left        ^W",
	"Word Right       ^F",
	"Prev Page        ^R",
	"Next Page        ^B",
	"Begin of line    Home",
	"End of line      End",
	"Top of file      F5",
	"End of file      F6",
	"Top of screen    F8",
	" Search ",
	"Search string    F7",
	" Delete ",
	"Char left        ^H or <BS>",
	"Under cursor     ^G or <Del>",
	"Line             ^Y",
	"To end of line   ^K",
	" Other ",
	"Redraw screen    ^]",
	"Quote next char  ^V",
	"Save file        F2",
	"Exit             ^C",
	" Modes ",
	"Raw mode         F3",
	"Visible spaces   F9",
	0,
};

static struct helptab edittab [] = {
	1,      0,      0,      0,
	3,      2,      1,      0,
	4,      6,      0,      0,
	5,      6,      0,      0,
	6,      6,      0,      0,
	7,      6,      0,      0,
	8,      6,      0,      0,
	9,      6,      0,      0,
       10,      6,      0,      0,
       11,      6,      0,      0,
       12,      6,      0,      0,
       13,      6,      0,      0,
       14,      6,      0,      0,
       15,      6,      0,      0,
       16,      6,      0,      0,
       17,      2,      1,      0,
       18,      6,      0,      0,
	3,  CS+ 2,      1,      0,
	4,  CS+ 6,      0,      0,
	5,  CS+ 6,      0,      0,
	6,  CS+ 6,      0,      0,
	7,  CS+ 6,      0,      0,
	9,  CS+ 2,      1,      0,
       10,  CS+ 6,      0,      0,
       11,  CS+ 6,      0,      0,
       12,  CS+ 6,      0,      0,
       13,  CS+ 6,      0,      0,
       15,  CS+ 2,      1,      0,
       16,  CS+ 6,      0,      0,
       17,  CS+ 6,      0,      0,
	0,      0,      0,      0,
};

static char *viewmesg [] = {
	"The Demos Commander Viewer, Copyright (C) 1990 Demos/*",
	" Cursor Movement ",
	"Page Left        ^A",
	"Page Right       ^D",
	"Line Up          ^E",
	"Line Down        ^X",
	"Prev Page        ^R",
	"Next Page        ^B",
	"Top of file      Home or F5",
	"End of file      End or F6",
	" Search ",
	"Search string    F7",
	" Other ",
	"Redraw screen    ^]",
	"Exit             ^C",
	" Modes ",
	"Raw mode         F3",
	"Visible spaces   F9",
	"Lines longer than 1024 bytes",
	"are broken and marked with \"\\\"",
	"Spaces are displayed as \".\"",
	"and tabs as \"_\"",
	0,
};

static struct helptab viewtab [] = {
	1,      0,      0,      0,
	3,      3,      1,      0,
	5,      7,      0,      0,
	6,      7,      0,      0,
	7,      7,      0,      0,
	8,      7,      0,      0,
	9,      7,      0,      0,
       10,      7,      0,      0,
       11,      7,      0,      0,
       12,      7,      0,      0,
       15,      3,      1,      0,
       17,      7,      0,      0,
	3,  CS+ 3,      1,      0,
	5,  CS+ 7,      0,      0,
	6,  CS+ 7,      0,      0,
	9,  CS+ 3,      1,      0,
       11,  CS+ 7,      0,      0,
       12,  CS+ 7,      0,      0,
       15,  CS+ 3,      0,      0,
       16,  CS+ 3,      0,      0,
       17,  CS+ 3,      0,      0,
       18,  CS+ 3,      0,      0,
	0,      0,      0,      0,
};

static char *hviewmesg [] = {
	"The Demos Commander Hexadecimal Viewer, Copyright (C) 1990 Demos/*",
	" Cursor Movement ",
	"Page Left        ^A",
	"Page Right       ^D",
	"Line Up          ^E",
	"Line Down        ^X",
	"Prev Page        ^R",
	"Next Page        ^B",
	"Top of file      Home or F5",
	"End of file      End or F6",
	" Search ",
	"Search string    F7",
	" Other ",
	"Redraw screen    ^]",
	"Exit             ^C",
	" Modes ",
	"Raw mode         F3",
	"Visible spaces   F9",
	"Spaces are displayed as \".\"",
	"and tabs as \"_\"",
	"For hexadecimal search",
	"type char codes as \"\\xx\"",
	0,
};

static struct helptab hviewtab [] = {
	1,      0,      0,      0,
	3,      3,      1,      0,
	5,      7,      0,      0,
	6,      7,      0,      0,
	7,      7,      0,      0,
	8,      7,      0,      0,
	9,      7,      0,      0,
       10,      7,      0,      0,
       11,      7,      0,      0,
       12,      7,      0,      0,
       15,      3,      1,      0,
       17,      7,      0,      0,
	3,  CS+ 3,      1,      0,
	5,  CS+ 7,      0,      0,
	6,  CS+ 7,      0,      0,
	9,  CS+ 3,      1,      0,
       11,  CS+ 7,      0,      0,
       12,  CS+ 7,      0,      0,
       15,  CS+ 3,      0,      0,
       16,  CS+ 3,      0,      0,
       17,  CS+ 3,      0,      0,
       18,  CS+ 3,      0,      0,
	0,      0,      0,      0,
};

static struct help help [] = {
	"general",      gentab,         genmesg,
	"edit",         edittab,        editmesg,
	"view",         viewtab,        viewmesg,
	"hexview",      hviewtab,       hviewmesg,
	0,              0,              0,
};

static inithelp (), initnewhelp ();
extern char *strcpy (), *strncpy (), *strcat (), *mdup ();

struct help *findhelp (name)
char *name;
{
	register struct help *p;

	for (p=help; p->name; ++p)
		if (! strcmp (p->name, name))
			return (p);
	return (0);
}

genhelp ()
{
	runhelp ("general");
}

runhelp (name)
char *name;
{
	register r, c, w, h;
	BOX *box;
	register struct helptab *p;
	struct help *hp;

	hp = findhelp (name);
	if (! hp)
		return;
	p = hp->htab;
	if (! p->str)
		inithelp (hp);
	w = 72;
	h = 21;
	r = 2;
	c = 4;

	box = VGetBox (r, c-1, h, w+2);                 /* save box */
	VStandOut ();
	VSetDim ();
	VFillBox (r, c-1, h, w+2, ' ');                 /* clear */
	VDrawBox (r, c, h, w);                          /* draw margins */
	VMPutString (r, c + (w-6) / 2, " Help ");       /* head */

	for (; p->row; ++p) {
		if (p->row >= h)
			continue;
		if (p->bold) {
			VSetBold ();
			VStandEnd ();
		}
		if (! p->col)
			VMPutString (r+p->row, c + (w-strlen(p->str))/2, p->str);
		else
			VMPutString (r+p->row, c + p->col, p->str);
		if (p->bold) {
			VSetDim ();
			VStandOut ();
		}
	}
	VStandEnd ();
	VSetNormal ();
	for (;;) {
		hidecursor ();
		VSync ();
		switch (KeyGet ()) {
		default:
			break;
		case cntrl (']'):       /* redraw screen */
			VRedraw ();
			continue;
		}
		break;
	}
	VUngetBox (box);
	VFreeBox (box);
}

static inithelp (hp)
struct help *hp;
{
	register struct helptab *p;
	register n, fd;
	register char **q, *m, *s;
	char filename [60];
	char buf [1024];

	strcpy (filename, HELPFILE);
	m = EnvGet ("MSG");
	if (m) {
		s = filename + strlen (filename);
		*s++ = *m;
		*s++ = '.';
		*s = 0;
	}
	strcat (filename, hp->name);
	fd = open (filename, 0);
	if (fd >= 0) {
		n = read (fd, buf, sizeof (buf) - 1);
		if (n > 0) {
			buf [n] = 0;
			initnewhelp (hp->mesg, mdup (buf));
		}
		close (fd);
	}
	q = hp->mesg;
	for (p=hp->htab; p->row && *q; ++p, ++q)
		p->str = *q;
}

static initnewhelp (q, p)
register char **q;
register char *p;
{
	for (; *q; ++q) {
		for(*q=p; *p && *p != '\n'; ++p)
			if (*p == '@')
				*p = ' ';
		if (! *p)
			break;
		*p++ = 0;
	}
	*q = 0;
}
