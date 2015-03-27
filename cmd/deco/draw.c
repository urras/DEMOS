# include "dir.h"
# include "deco.h"
# include "scr.h"

# define DECOVERSION "The Demos Commander, Version 3.3"

static BOX *cbox;                       /* box under cursor */
static struct dir *cdir;                /* drawed directory */

static char *filnam ();
static draw1win (), drawstatus (), drawpage (), drawfull (), drawmode ();
static drawtime (), drawlong (), drawcwd (), drawpat (), drawsdir ();
extern char *ltoac (), *filemode (), *filesize ();
extern char *username (), *groupname (), *strncpy ();

extern char *timestr (), *extension ();
extern long time ();

draw ()
{
	if (! visualwin)
		VClearBox (1, 0, LINES-2, 80);
	drawwin ();
	draw2dir ();
}

drawdir (c)             /* redraw window */
register struct dir *c;
{
	draw1dir (c);
	c = (c == &left ? &right : &left);
	if (c->status)
		draw1dir (c);
}

draw2dir ()
{
	if (! visualwin)
		return;
	draw1dir (&left);
	draw1dir (&right);
}

drawwin ()
{
	register i;
	register char *curtime;

	if (visualwin)
		if (fullwin)
			draw1win (BASECOL (cur), cur->full, cur->status);
		else {
			draw1win (left.basecol, left.full, left.status);
			draw1win (right.basecol, right.full, right.status);
		}
	VSetDim ();
	VStandOut ();
	VMove (0, 0);
	for (i=80; --i>=0; VPutChar (' '));
	VMove (0, 1);
	if (machine) {
		VPutString (machine);
		VPutChar (':');
	}
	if (tty)
		VPutString (tty);
	if (user)
		VMPutString (0, 39 - strlen (user), user);
	VPutChar ('.');
	if (group)
		VPutString (group);
	if (curtime = timestr (time ((long *) 0)))
		VMPutString (0, 79 - strlen (curtime), curtime);
	VStandEnd ();
	VMPutString (LINES-1, 0, cmdreg ?
"1\16Help  \17 2\16User  \17 3\16      \17 4\16      \17 5\16    \
  \17 6\16      \17 7\16      \17 8\16      \17 9\16Menu  \017 10\16Quit \17" :
"1\16Help  \17 2\16User  \17 3\16View  \17 4\16Edit  \17 5\16Copy\
  \17 6\16RenMov\17 7\16Mkdir \17 8\16Delete\17 9\16Menu  \017 10\16Quit \17");
	VSetNormal ();
}

draw1dir (c)             /* redraw window */
register struct dir *c;
{
	register len = c->num - c->topfile;

	/* check visibility in full window regime */
	if (! visualwin || fullwin && c != cur)
			return;

	cdir = c;

	/* if it is status window - draw status of another one */
	if (c->status) {
		drawsdir (c);
		return;
	}

	/* reprint dir name */
	VSetDim ();
	VHorLine (1, BASECOL (c) + 1, PAGEWID-2);
	VHorLine (H+4, BASECOL (c) + 1, PAGEWID-2);
	if (fullwin)
		if (c->full == 0) {
			VCorner (1, 19, 2);
			VCorner (1, 38, 2);
			VCorner (1, 57, 2);
		} else if (c->full == 1)
			VCorner (1, 38, 2);
	drawcwd (c->basecol, c->shortcwd);
	if (c->pattern [0])
		drawpat (BASECOL (c), c->pattern, PAGEWID);
	VSetNormal ();

	switch (c->full) {
	case 2:
		if (fullwin) {
			VClearBox (2, 1, H, 75);
			drawpage (1, &c->cat[c->topfile], len, 2);
			break;
		}
		/* fall through */
	case 1:
		VClearBox (2, BASECOL (c) + 1, H, 37);
		drawpage (BASECOL (c) + 1, &c->cat[c->topfile], len, 1);
		if (fullwin) {
			VClearBox (2, BASECOL (c) + 39, H, 37);
			if (len > H)
				drawpage (BASECOL (c) + 39, &c->cat[c->topfile+H], len-H, 1);
		}
		break;
	default:
		VClearBox (2, BASECOL (c) + 1, H, 18);
		drawpage (BASECOL (c) + 1, &c->cat[c->topfile], len, 0);

		VClearBox (2, BASECOL (c) + 20, H, 18);
		if (len > H)
			drawpage (BASECOL (c) + 20, &c->cat[c->topfile+H], len-=H, 0);
		if (fullwin) {
			VClearBox (2, BASECOL (c) + 39, H, 18);
			if (len > H)
				drawpage (BASECOL (c) + 39, &c->cat[c->topfile+H+H], len-=H, 0);

			VClearBox (2, BASECOL (c) + 58, H, 18);
			if (len > H)
				drawpage (BASECOL (c) + 58, &c->cat[c->topfile+H+H+H], len-H, 0);
		}
		break;
	}
	drawstatus (BASECOL (c), &c->cat[c->curfile], c->full);
}

drawcursor ()
{
	register col, line, w;

	cdir = cur;
	if (cur->status) {
		VStandOut ();
		drawcwd (cur->basecol, " Status ");
		VStandEnd ();
		return;
	}
	w = cur->full==2 ? PAGEWID-2 : cur->full ? 37 : 18;
	line = cur->curfile - cur->topfile;
	if (line < H)
		col = BASECOL (cur) + 1;
	else {
		col = BASECOL (cur) + line/H * (w+1) + 1;
		line %= H;
	}
	line += 2;
	if (cbox)
		VFreeBox (cbox);
	cbox = VGetBox (line, col, 1, w);
	if (VStandOut ())
		if (cur->cat[cur->curfile].tag) {
			VSetBold ();
			VPrintBox (cbox);
			VSetNormal ();
		} else
			VPrintBox (cbox);
	else {
		VMPutChar (line, col, '[');
		VMPutChar (line, col+w-1, ']');
	}
	drawcwd (cur->basecol, cur->shortcwd);
	VStandEnd ();
	drawstatus (BASECOL (cur), &cur->cat[cur->curfile], cur->full);
}

undrawcursor ()
{
	if (cbox) {
		VUngetBox (cbox);
		cbox = 0;
	}
	VSetDim ();
	drawcwd (cur->basecol, cur->status ? " Status " : cur->shortcwd);
	VSetNormal ();
}

static draw1win (c, full, stawin)
register c;
{
	VSetDim ();
	if (stawin) {
		VHorLine (4, c+1, PAGEWID-2);
		VHorLine (11, c+1, PAGEWID-2);
		VVertLine (c, 2, H+2);
		VVertLine (c+PAGEWID-1, 2, H+2);
		VCorner (1, c, 1);
		VCorner (1, c+PAGEWID-1, 3);
		VCorner (4, c, 4);
		VCorner (4, c+PAGEWID-1, 6);
		VCorner (11, c, 4);
		VCorner (11, c+PAGEWID-1, 6);
		VCorner (H+4, c, 7);
		VCorner (H+4, c+PAGEWID-1, 9);
	} else if (fullwin) switch (full) {
	case 2:
		VHorLine (H+2, 1, PAGEWID-2);
		VVertLine (0, 2, H+2);
		VVertLine (PAGEWID-1, 2, H+2);
		VCorner (1, 0, 1);
		VCorner (1, PAGEWID-1, 3);
		VCorner (H+2, 0, 4);
		VCorner (H+2, PAGEWID-1, 6);
		VCorner (H+4, 0, 7);
		VCorner (H+4, PAGEWID-1, 9);
		break;
	case 1:
		VHorLine (H+2, 1, PAGEWID-2);
		VVertLine (0, 2, H+2);
		VVertLine (38, 2, H);
		VVertLine (PAGEWID-1, 2, H+2);
		VCorner (1, 0, 1);
		VCorner (1, PAGEWID-1, 3);
		VCorner (H+2, 0, 4);
		VCorner (H+2, 38, 8);
		VCorner (H+2, PAGEWID-1, 6);
		VCorner (H+4, 0, 7);
		VCorner (H+4, PAGEWID-1, 9);
		break;
	case 0:
		VHorLine (H+2, 1, PAGEWID-2);
		VVertLine (0, 2, H+2);
		VVertLine (19, 2, H);
		VVertLine (38, 2, H);
		VVertLine (57, 2, H);
		VVertLine (PAGEWID-1, 2, H+2);
		VCorner (1, 0, 1);
		VCorner (1, PAGEWID-1, 3);
		VCorner (H+2, 0, 4);
		VCorner (H+2, 19, 8);
		VCorner (H+2, 38, 8);
		VCorner (H+2, 57, 8);
		VCorner (H+2, PAGEWID-1, 6);
		VCorner (H+4, 0, 7);
		VCorner (H+4, PAGEWID-1, 9);
		break;
	} else {
		VHorLine (H+2, c+1, PAGEWID-2);
		VVertLine (c, 2, H+2);
		if (! full)
			VVertLine (c+19, 2, H);
		VVertLine (c+38, 2, H+2);
		VCorner (1, c, 1);
		VCorner (1, c+38, 3);
		VCorner (H+2, c, 4);
		VCorner (H+2, c+38, 6);
		VCorner (H+4, c, 7);
		if (! full)
			VCorner (H+2, c+19, 8);
		VCorner (H+4, c+38, 9);
	}
	VSetNormal ();
}

static drawcwd (col, wd)
register char *wd;
{
	register len;

	if (! visualwin)
		return;
	len = strlen (wd);
	if (fullwin && col)
		col = 38;
	VMPutString (1, col + (39-len) / 2, wd);
}

static drawpat (col, pat, width)
register char *pat;
{
	col += width - strlen (pat) - 1;
	VMove (H+4, col);
	VPutString (pat);
}

static drawsdir (c)            /* draw status window */
register struct dir *c;
{
	struct dir *d = c == &left ? &right : &left;
	char buf [80];

	/* print window name " Status " */
	VSetDim ();
	VHorLine (1, BASECOL (c) + 1, PAGEWID-2);
	drawcwd (c->basecol, " Status ");
	VHorLine (H+4, BASECOL (c) + 1, PAGEWID-2);
	VSetNormal ();
	VClearBox (2, BASECOL (c) + 1, 2, PAGEWID-2);
	VClearBox (5, BASECOL (c) + 1, 6, PAGEWID-2);
	VClearBox (12, BASECOL (c) + 1, H-8, PAGEWID-2);
	mvcaddstr (2, BASECOL (c) + PAGEWID/2 + 1, DECOVERSION);
	mvcaddstr (3, BASECOL (c) + PAGEWID/2 + 1, "Copyright (C) 1990 by Demos/*");
	mvcaddstr (5, BASECOL (c) + PAGEWID/2 + 1, "Directory");
	mvcaddstr (6, BASECOL (c) + PAGEWID/2 + 1, d->shortcwd);
	sprintf (buf, "\3%d\2 file%s", d->nfiles, d->nfiles==1 ? "" : "s");
	mvcaddstr (7, BASECOL (c) + PAGEWID/2 + 2, buf);
	sprintf (buf, "\3%s\2 byte%s", ltoac (d->nbytes), d->nbytes==1 ? "" : "s");
	mvcaddstr (8, BASECOL (c) + PAGEWID/2 + 2, buf);
	if (d->pattern [0]) {
		sprintf (buf, "\3%s\2 byte%s in \3%d\2 matched file%s",
			ltoac (d->mbytes), d->mbytes==1 ? "" : "s",
			d->mfiles, d->mfiles==1 ? "" : "s");
		mvcaddstr (9, BASECOL (c) + PAGEWID/2 + 3, buf);
	}
	if (d->tfiles) {
		sprintf (buf, "\3%s\2 byte%s in \3%d\2 selected file%s",
			ltoac (d->tbytes), d->tbytes==1 ? "" : "s",
			d->tfiles, d->tfiles==1 ? "" : "s");
		mvcaddstr (10, BASECOL (c) + PAGEWID/2 + 3, buf);
	}
# ifdef notyet
	mvcaddstr (13, BASECOL (c) + PAGEWID/2 + 1, "No .dirinfo file in this directory");
# endif
}

static drawstatus (col, f, full)
struct file *f;
{
	/* draw current file status box */
	VMove (H+3, col+2);
	VSetDim ();
	VPutChar (filetype (f));
	VSetNormal ();
	VPutChar (' ');
	if (fullwin) switch (full) {
	case 2:
		VPutString (filnam (f->name, (int) f->truncname));
		drawtime (f);
		break;
	case 1:
	case 0:
		drawfull (f);
		drawlong (f);
		break;
	} else
		drawfull (f);
}

static drawpage (col, c, n, full)      /* redraw file window */
register struct file *c;        /* catalog */
register n;                     /* number of names to print */
{
	register i;

	if (n > H)
		n = H;
	++col;
	for (i=0; i<n; ++i, ++c) {
		VSetDim ();
		VMPutChar (i+2, col, filetype (c));
		if (c->tag) {
			VPutChar ('>');
			VSetBold ();
		} else if (c->link) {
			VPutChar ('@');
			VSetNormal ();
		} else {
			VPutChar (' ');
			VSetNormal ();
		}
		switch (full) {
		case 2:
			if (fullwin) {
				drawfull (c);
				drawlong (c);
				break;
			}
			/* fall through */
		case 1:
			drawfull (c);
			break;
		case 0:
			VPutString (filnam (c->name, (int) c->truncname));
			break;
		}
		if (c->tag)
			VSetNormal ();
	}
}

static drawfull (f)
register struct file *f;
{
	VPutString (filnam (f->name, (int) f->truncname));      /* 14 */
	VPutString (filesize (f));                              /* 8 */
	VPutChar (' ');                                         /* 1 */
	VPutChar (' ');                                         /* 1 */
	drawmode (filemode ((int) f->fstat.st_mode), (int) f->fstat.st_uid, (int) f->fstat.st_gid);
}

static drawmode (m, u, g)
register char *m;
register u, g;
{
	if (uid == 0) {
		VPutString (m);
		return;
	}
	u = (u == uid);
	g = (g == gid);
	if (! u)
		VSetDim ();
	VPutChar (*m++);
	VPutChar (*m++);
	VPutChar (*m++);
	if (u)
		VSetDim ();
	else if (g)
		VSetPrev ();
	VPutChar (*m++);
	VPutChar (*m++);
	VPutChar (*m++);
	if (! u)
		if (g)
			VSetDim ();
		else
			VSetPrev ();
	VPutChar (*m++);
	VPutChar (*m++);
	VPutChar (*m++);
	if (u || g)
		VSetPrev ();
}

static drawtime (f)
register struct file *f;
{
	register struct stat *st = &f->fstat;

	printw ("%4d/%-5d", st->st_dev, st->st_ino);
	if (f->fstat.st_nlink > 1)
		printw (" %-3d ", st->st_nlink);
	else
		VPutString ("     ");
	VPutString (timestr (st->st_atime));
	VPutChar (' ');
	VPutChar (' ');
	VPutString (timestr (st->st_ctime));
}

static drawlong (f)
register struct file *f;
{
	register struct stat *st = &f->fstat;

	printw (" %8.8s.%-8.8s", username ((int) st->st_uid), groupname ((int) st->st_gid));
	VPutString (timestr (st->st_mtime));
}

static char *filnam (p, t)
register char *p;
{
	static char buf [20];
	register char *s, *e;

	if (t) {
		strncpy (buf, p, 13);
		buf [13] = '~';
		return (buf);
	}
	for (s=buf; s<buf+14; *s++=' ');        /* clear buf */
	if (cdir->alignext) {
		e = extension (p);                      /* make ptr to extension */
		for (s=buf; p<e; *s++= *p++);           /* copy file name */
		for (s=e; *s; ++s);                     /* search extension end */
		for (p=buf+13, --s; s>=e; *p--= *s--);  /* copy extension */
	} else {
		e = p + 14;                             /* make ptr to end */
		for (s=buf; *p && p<e; *s++= *p++);     /* copy file name */
	}
	buf [14] = 0;
	return (buf);
}
