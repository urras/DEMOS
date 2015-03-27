# include "scr.h"
# include "dir.h"
# include "deco.h"
# include "mem.h"
# include "env.h"

# ifndef NULL
#    define NULL ((char *) 0)
# endif

# define SWAP(a,b)      { register t; t = a; a = b; b = t; }

# define HISTSZ 16                      /* length of history */

char command [CMDLEN];                  /* command string */
extern char *home;

static char *history [HISTSZ];          /* previous executed commands */
static hpos;                            /* history pointer */
static char *prompt;
extern visualwin;                       /* cursor in command line */

static esclen (), putescstr (), wresccmd (), wrescchar (), wrescback ();
static erasecmd (), mycmd (), mycmd2 ();
static char *getpar ();
extern char *gets (), *strcpy ();

drawcmd ()
{
	int c;
	CURSOR cursor;

	if (H==LINES-7 && strlen (cur->shortcwd) + 3 + esclen (command) > 79) {
		fullscreen ();
		draw ();
	}
	for (c=H+5; c<LINES-1; ++c) {
		VMove (c, 0);
		VClearLine ();
	}
	VSetDim ();
	VMPutString (H+5, 0, cur->shortcwd);
	VSetBold ();
	VPutString (" > ");
	VSetNormal ();
	cursor = VGetCursor ();
	putescstr (command);
	if (c = command [cpos]) {
		VSetCursor (cursor);
		command [cpos] = 0;
		putescstr (command);
		command [cpos] = c;
	}
}

static insname (name)
char *name;
{
	register char *s;

	for (s=name; *s; ++s) {
		inscmd (*s);
		if (cpos > CMDLEN-2)
			return (0);
	}
	inscmd (' ');
	return (1);
}

namecmd ()
{
	register i, savepos;

	if (command [cpos])
		return;
	if (cpos && command [cpos-1] != ' ')
		return;
	savepos = cpos;
	if (tagged ()) {
		for (i=0; i<cur->num; ++i)
			if (cur->cat[i].tag && ! insname (cur->cat[i].name))
				goto fail;
	} else
		if (! insname (cur->cat[cur->curfile].name)) {
fail:                   command [cpos = savepos] = 0;
			return;
		}
}

inscmd (key)
{
	register k;

	for (k=cpos; command[k]; ++k)
		SWAP (key, command[k]);
	if (k < CMDLEN-1) {
		command [k] = key;
		command [k+1] = 0;
	}
	rightcmd ();
}

delcmd ()
{
	register k;

	if (! command [cpos])
		return;
	for (k=cpos+1; command[k]; ++k)
		command[k-1] = command[k];
	command [k-1] = 0;
}

homecmd ()
{
	cpos = 0;
}

endcmd ()
{
	while (command [cpos])
		++cpos;
}

rightcmd ()
{
	if (command [cpos]) {
		++cpos;
		if (cpos >= CMDLEN)
			cpos = CMDLEN-1;
	}
}

leftcmd ()
{
	if (--cpos < 0)
		cpos = 0;
}

upcmd ()
{
	while (cpos > 0 && command [--cpos] == ' ');
	if (cpos <= 0)
		return;
	while (cpos > 0 && command [--cpos] != ' ');
	if (command [cpos] == ' ')
		++cpos;
}

downcmd ()
{
	while (command [cpos] && command [++cpos] == ' ');
	if (! command [cpos])
		return;
	while (command [cpos] && command [++cpos] != ' ');
	if (command [cpos] == ' ')
		--cpos;
}

nextcmd ()
{
	if (hpos <= 0) {
		hpos = -1;
		command [cpos = 0] = 0;
		return;
	}
	--hpos;
	strcpy (command, history [hpos]);
	for (cpos=0; command[cpos]; ++cpos);
}

prevcmd ()
{
	command [cpos = 0] = 0;
	if (! history [0])
		return;
	if (hpos >= HISTSZ)
		return;
	if (hpos >= 0 && ! history [hpos])
		return;
	++hpos;
	if (hpos >= HISTSZ)
		return;
	if (! history [hpos])
		return;
	strcpy (command, history [hpos]);
	for (cpos=0; command[cpos]; ++cpos);
}

histcmd ()              /* store command in history */
{
	register n;

	/* forget oldest command */
	if (history [HISTSZ-1])
		MemFree ((mem *) history [HISTSZ-1]);
	history [HISTSZ-1] = 0;

	/* shift the rest */
	for (n=0; history[n]; ++n);
	while (--n >= 0)
		history [n+1] = history [n];

	/* create copy of command */
	history [0] = (char *) MemAlloc (strlen (command) + 1);
	strcpy (history [0], command);
}

outprompt ()
{
	write (1, prompt, (unsigned) strlen (prompt));
}

execmd (hit, savhist)
{
	char pr [256];
	char buf [256];

	/* run command */
	if (savhist)
		histcmd ();     /* save it in history */
	hpos = -1;              /* reset history pointer */
	if (mycmd (1)) {        /* exec built in commands */
		command [cpos = 0] = 0;
		return;
	}
	VClear ();              /* clear screen */
	endcmd ();              /* move cursor to the end of command */
	drawcmd ();             /* draw comand */
	VSync ();
	VRestore ();            /* restore terminal */
	write (1, "\n", 1);     /* new line */
	if (! mycmd2 (1))       /* exec built in commands */
		syscmd (command);       /* execute command */
	if (hit) {
		sprintf (buf, "\7\1%s\3 > \2", cur->shortcwd);
		VExpandString (buf, pr);
		prompt = pr;
		VReopen ();
		command [cpos = 0] = 0;
		for (;;) {
			if (! inputcmd (strlen (cur->shortcwd) + 3) || ! command [0])
				break;
			histcmd ();
			hpos = -1;
			if (mycmd (0) || mycmd2 (0)) {
				sprintf (buf, "\7\1%s\3 > \2", cur->shortcwd);
				VExpandString (buf, pr);
			} else {
				VRestore ();
				syscmd (command);
				VReopen ();
			}
		}
	} else {
		VReopen ();
		command [cpos = 0] = 0;
	}
	VClear ();
	if (visualwin) {
		setdir (cur==&left ? &right : &left, NULL);
		setdir (cur, NULL);
	}
}

inputcmd (plen)
{
	register c;
	char cc;

	outprompt ();
	command [cpos = 0] = 0;
	for (;;) {
		c = KeyGet ();
		if (c>=' ' && c<='~' || c>=0300 && c<=0376) {
			if (cpos || c!=' ') {
				inscmd (c);
				cc = c;
				write (1, &cc, 1);
			}
			continue;
		}
		switch (c) {
		default:
			write (1, "\7", 1);
			continue;
		case cntrl ('V'):       /* quote next char */
			c = quote ();
			inscmd (c);
			wrescchar (c);
			continue;
		case cntrl ('B'):
		case meta ('A'):
		case meta ('B'):
		case meta ('G'):
		case meta ('I'):
		case meta ('J'):
			KeyUnget (c);
		case cntrl ('P'):
		case cntrl ('O'):
			cmdreg = 1;
			visualwin = 0;
			return (0);
		case cntrl ('M'):
		case cntrl ('J'):
			cmdreg = 0;
			visualwin = 1;
			write (1, "\r\n", 2);
			hpos = -1;              /* reset history pointer */
			return (1);
		case meta ('u'):          /* up */
		case cntrl ('E'):
			erasecmd (plen);
			prevcmd ();
			if (cpos)
				wresccmd (command, cpos);
			continue;
		case meta ('d'):          /* down */
		case cntrl ('X'):
			erasecmd (plen);
			nextcmd ();
			if (cpos)
				wresccmd (command, cpos);
			continue;
		case cntrl ('Y'):
			erasecmd (plen);
			continue;
		case cntrl ('H'):
			if (cpos) {
				wrescback (command [cpos-1]);
				leftcmd ();
				delcmd ();
			}
			continue;
		}
	}
}

static erasecmd (plen)
{
	if (plen+esclen(command) >= 80) {
		write (1, "\r\n", 2);
		write (1, prompt, (unsigned) strlen (prompt));
	} else
		while (--cpos >= 0)
			wrescback (command [cpos]);
	command [cpos = 0] = 0;
}

static mycmd (scr)
{
	/* execute built in commands which print anything */
	/* these are: */
	/* cd           - chdir to $HOME */
	/* cd dirname   - chdir to dirname */
	/* chdir        - chdir to $HOME */
	/* chdir dirname - chdir to dirname */
	/* exit         - quit from deco */
	/* return 1 if executed one of these */
	/* else return 0 */

	if (strbcmp (command, "cd") || strbcmp (command, "chdir")) {
		char dir [100];

		getpar (dir);
		if (! dir [0])
			setdir (cur, home);
		else if (! strcmp (dir, cur->cwd))
			setdir (cur, NULL);
		else if (chdir (dir) < 0)
			if (scr)
				error ("Cannot chdir to %s", dir);
			else
				outerr ("Cannot chdir to %s\n", dir);
		else
			setdir (cur, ".");
		return (1);
	}
	if (strbcmp (command, "exit")) {
		quitdeco ();
		return (1);
	}
	return (0);
}

/* ARGSUSED */
static mycmd2 (scr)
{
	/* execute built in commands wqhich print something */
	/* these are: */
	/* env          - chdir to $HOME */
	/* printenv     - chdir to dirname */
	/* set          - set global variable */
	/* setenv       - set global variable */
	/* unset        - set global variable */
	/* unsetenv     - set global variable */
	/* pwd          - print current directory name */
	/* return 1 if executed one of these */
	/* else return 0 */

	if (strbcmp (command, "env") || strbcmp (command, "printenv")) {
		register char **p;
printenv:
		for (p=EnvVector; p && *p; ++p) {
			write (1, *p, (unsigned) strlen (*p));
			write (1, "\r\n", 2);
		}
		return (1);
	}
	if (strbcmp (command, "pwd")) {
		write (1, cur->cwd, (unsigned) strlen (cur->cwd));
		write (1, "\r\n", 2);
		return (1);
	}
	if (strbcmp (command, "set") || strbcmp (command, "setenv")) {
		char dir [100];
		char *v;

		v = getpar (dir);
		if (! dir [0])
			goto printenv;
		EnvPut (dir, v);
		return (1);
	}
	if (strbcmp (command, "unset") || strbcmp (command, "unsetenv")) {
		char dir [100];

		getpar (dir);
		if (! dir [0])
			goto printenv;
		EnvDelete (dir);
		return (1);
	}
	return (0);
}

static char *getpar (s)
register char *s;
{
	register char *p;

	*s = 0;
	p = command;
	while (*p && *p!=' ')           /* skip command name */
		++p;
	while (*p && *p==' ')           /* skip spaces */
		++p;
	if (! *p)                       /* return if no parameters */
		return (0);
	while (*p && *p!=' ')           /* copy parameter # 1 */
		*s++ = *p++;
	*s = 0;
	if (*p == ' ')
		++p;
	return (p);                     /* address of next parameter */
}

histmenu ()
{
	register ch, nh;
	int n, histwid, histrow, histcol, execit;
	BOX *box, *curbox;

	histwid = 1;
	for (nh=0; nh<HISTSZ && history[nh]; ++nh)
		if ((n = esclen (history[nh])) > histwid)
			histwid = n;
	if (! nh) {
		error ("History is empty");
		return;
	}
	histwid += 2;
	if (histwid < 20)
		histwid = 20;
	else if (histwid > 70)
		histwid = 70;
	histrow = (LINES-nh) / 2;
	histcol = (79-histwid) / 2;

	box = VGetBox (histrow-2, histcol-4, nh+4, histwid+8);
	VSetDim ();
	VStandOut ();
	VFillBox (histrow-2, histcol-4, nh+4, histwid+8, ' ');
	VDrawBox (histrow-1, histcol-1, nh+2, histwid+2);
	mvcaddstr (histrow-1, 40, " History ");

	for (ch=0; ch<nh; ++ch) {
		VMove (histrow+ch, histcol+1);
		putescstr (history[nh-ch-1]);
	}

	VSetNormal ();
	VStandEnd ();
	ch = 0;
	for (;;) {
		curbox = VGetBox (histrow+nh-ch-1, histcol, 1, histwid);
		VPrintBox (curbox);
		hidecursor ();
		VSync ();
		n = KeyGet ();
		VUngetBox (curbox);
		VFreeBox (curbox);
		switch (n) {
		default:
			VBeep ();
			continue;
		case cntrl (']'):          /* redraw screen */
			VRedraw ();
			continue;
		case cntrl ('J'):
			execit = 0;
			break;
		case cntrl ('M'):
			execit = 1;
			break;
		case cntrl ('C'):
		case cntrl ('['):
		case meta ('J'):           /* f0 */
			ch = -1;
			break;
		case meta ('d'):          /* down */
			if (--ch < 0)
				ch = nh-1;
			continue;
		case meta ('u'):          /* up */
			if (++ch >= nh)
				ch = 0;
			continue;
		}
		break;
	}
	if (ch >= 0) {
		/* execute command from history */
		strcpy (command, history [ch]);
		cpos = strlen (command);
		if (command [0] && execit)
			execmd (1, 1);
	}
	VUngetBox (box);
	VFreeBox (box);
}

static esclen (str)
register char *str;
{
	register c, count;

	for (count=0; c=0377&*str; ++count, ++str)
		if (c<' ' || c==0177)
			++count;
		else if (c>=0200 && c<0300 || c==0377)
			count += 3;
	return (count);
}

static putescstr (str)
register char *str;
{
	register c;

	for (; c=0377&*str; ++str)
		if (c<' ' || c==0177) {
			VPutChar ('^');
			VPutChar (c ^ '@');
		} else if (c>=0200 && c<0300 || c==0377) {
			VPutChar ('\\');
			VPutChar (c>>6 | '0');
			VPutChar (c>>3 & 7 | '0');
			VPutChar (c & 7 | '0');
		} else
			VPutChar (c);
}

static wresccmd (str, n)
register char *str;
{
	char buf [CMDLEN*2];
	register char *p;
	register c;

	for (p=buf; --n>=0; ++str) {
		c = 0377 & *str;
		if (c<' ' || c==0177) {
			*p++ = '^';
			*p++ = c ^ '@';
		} else if (c>=0200 && c<0300 || c==0377) {
			*p++ = '\\';
			*p++ = c>>6 | '0';
			*p++ = c>>3 & 7 | '0';
			*p++ = c & 7 | '0';
		} else
			*p++ = c;
	}
	write (1, buf, (unsigned) (p-buf));
}

static wrescchar (c)
register c;
{
	char buf [8];
	register char *p;

	p = buf;
	c &= 0377;
	if (c<' ' || c==0177) {
		*p++ = '^';
		*p++ = c ^ '@';
	} else if (c>=0200 && c<0300 || c==0377) {
		*p++ = '\\';
		*p++ = c>>6 | '0';
		*p++ = c>>3 & 7 | '0';
		*p++ = c & 7 | '0';
	} else
		*p++ = c;
	write (1, buf, (unsigned) (p-buf));
}

static wrescback (c)
register c;
{
	c &= 0377;
	if (c<' ' || c==0177)
		write (1, "\b\b  \b\b", 6);
	else if (c>=0200 && c<0300 || c==0377)
		write (1, "\b\b\b\b    \b\b\b\b", 12);
	else
		write (1, "\b \b", 3);
}
