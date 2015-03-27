/*
 *      Record-oriented file access packadge.
 *      -------------------------------------
 *
 *      REC *RecOpen (int fd, int mode)
 *              - initialize Rec packadge for file fd,
 *              for writing if mode!=0.
 *
 *      int RecSave (REC *r, char *filename)
 *              - and write out changes,
 *              returns 0 if ok or -1 if cannot write file.
 *
 *      int RecClose (REC *r)
 *              - close Rec discarding changes.
 *
 *      RecBroken (REC *r)
 *              - if Rec has broken lines, return 1, else 0.
 *
 *      LINE *RecGet (REC *r, int linenum)
 *              - get line.
 *
 *      RecPut (LINE *l, int newlen)
 *              - put line.
 *
 *      RecInsLine (REC *r, int linenum)
 *              - insert empty line before line #linenum
 *
 *      RecDelLine (REC *r, int linenum)
 *              - delete line #linenum
 *
 *      RecInsChar (REC *r, int line, int off, int sym)
 *              - insert char into line
 *
 *      RecDelChar (REC *r, int line, int off)
 *              - delete char from line
 */

# include "rec.h"
# include "mem.h"

# define BUFSIZ         512
# define TEMPCELL       16              /* minimal size of temp space */
# define QUANT          512
# define MAXLINE        1024            /* maximum length of unbroken line */
# define MAXLEN         4000            /* maximum length of file */
# define NOINDEX        (POOLSZ+2)

# define BAKSUFFIX      ".b"

static char mask [8] = { 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80, };
static char tfilepattern [] = "/tmp/recXXXXXX";

static lenbuf;
static char *scanbuf, *pbuf;
static char *tfilename;
static eoln;

extern char *mktemp (), *strcpy (), *strcat ();
extern long lseek ();

static ffcopy (from, to)
{
	char buf [512], *p;
	register n, k;

	for (;;) {
		n = read (from, buf, sizeof (buf));
		if (n < 0) {
			error ("Cannot read file");
			return (-1);
		}
		if (n == 0)
			return (0);
		p = buf;
		while (n > 0) {
			k = write (to, p, (unsigned) n);
			if (k <= 0) {
				error ("Cannot write file");
				return (-1);
			}
			n -= k;
			p += k;
		}
	}
}

static tempfree (r, seek, len)  /* mark temp space free */
REC *r;
register long seek;
{
	register i;

	len = (len + TEMPCELL - 1) / TEMPCELL;
	seek /= TEMPCELL;
	for (i=0; i<len; ++i, ++seek)
		r->tmap [seek>>3] &= ~mask [seek & 7];
}

static long tempbusy (r, len)   /* search & mark temp space busy */
REC *r;
{
	register i, n;
	register long seek;

	len = (len + TEMPCELL - 1) / TEMPCELL;
	for (seek=0; seek<MAXTEMP; ++seek) {
		for (n=0; n<len; ++n)
			if (r->tmap [seek>>3] & mask [seek & 7])
				break;
		if (n >= len)
			break;
	}
	if (seek >= MAXTEMP) {
		error ("No temp space");
		return (0);
	}
	for (n=seek, i=0; i<len; ++i, ++n)
		r->tmap [n>>3] |= mask [n & 7];
	return (seek * TEMPCELL);
}

static long tempsave (r, str, len) /* save string in temp file, return seek */
register REC *r;
char *str;
{
	register long seek;

	if (! len)
		return (0);
	seek = tempbusy (r, len);
	if (lseek (r->tfd, seek, 0) < 0)
		error ("Cannot lseek on writing temp file");
	if (write (r->tfd, str, (unsigned) len) != len)
		error ("Cannot write temporary file");
	return (seek);
}

static scanline (fd)
{
	register len;

	len = 0;
	eoln = 0;
	for (;;) {
		if (pbuf >= &scanbuf[lenbuf]) {
			lenbuf = read (fd, pbuf = scanbuf, (unsigned) BUFSIZ);
			if (lenbuf <= 0)
				return (len ? len : -1);
		}
		if (*pbuf++ == '\n') {
			eoln = 1;
			return (len);
		}
		if (++len >= MAXLINE)
			return (len);
	}
}

REC *RecOpen (fd, wmode)
{
	register REC *r;
	register struct ldesc *x;
	register i;

	r = (REC *) MemAlloc (sizeof (REC));
	r->fd = fd;
	r->bakfd = -1;
	if (wmode) {
		if (! tfilename)
			tfilename = tfilepattern;
		r->tfd = creat (tfilename, 0600);
		if (r->tfd < 0) {
			error ("Cannot open temporary file");
			return (0);
		}
		close (r->tfd);
		r->tfd = open (tfilename, 2);
		if (r->tfd < 0) {
			error ("Cannot reopen temporary file");
			return (0);
		}
		unlink (tfilename);
	} else
		r->tfd = -1;

	for (i=0; i<TEMPSZ; ++i)
		r->tmap [i] = 0;

	for (i=0; i<POOLSZ; ++i)
		r->rmap[i].busy = 0;

	lseek (r->fd, 0L, 0);
	r->broken = 0;
	r->len = 0;
	r->lindex = 0;
	r->nindex = 0;
	r->size = 0;
	scanbuf = (char *) MemAlloc (BUFSIZ);
	pbuf = scanbuf;
	lenbuf = 0;
	for (;;) {
		MemCheckIndex (r->lindex, struct ldesc *, r->nindex, QUANT, r->len+1);
		x = &r->lindex[r->len];
		x->seek = r->size;              /* store seek of current line */
		x->poolindex = NOINDEX;         /* clear pool index of current line */
		x->flags = 0;                   /* clear flags */
		x->llen = scanline (r->fd);     /* scan next line */
		if (x->llen < 0)
			break;
		r->size += x->llen;             /* increment seek by length of line */
		if (eoln)
			++r->size;              /* \n at end of line */
		else {
			x->flags |= XNOEOLN;    /* no end of line */
			++r->broken;
		}
		if (++r->len >= MAXLEN) {
			error ("file too long");
			return (0);
		}
	}
	MemFree ((mem *) scanbuf);
	return (r);
}

RecClose (r)
register REC *r;
{
	register i;

	for (i=0; i<POOLSZ; ++i)
		if (r->rmap[i].busy) {
			if (r->pool[i].slen)
				MemFree ((mem *) r->pool[i].ptr);
			r->lindex[r->rmap[i].index].poolindex = NOINDEX;
			r->rmap[i].busy = 0;
		}
	MemFree ((mem *) r->lindex);
	if (r->tfd >= 0)
		close (r->tfd);
	if (r->bakfd >= 0)
		close (r->bakfd);
}

RecSave (r, filename)
register REC *r;
char *filename;
{
	register i, fd;
	register LINE *p;
	char bak [40];

	if (r->bakfd < 0) {
		strcpy (bak, filename);
		strcat (bak, BAKSUFFIX);
		r->bakfd = creat (bak, 0600);
		if (r->bakfd < 0) {
			error ("Cannot create %s", bak);
			return (-1);
		}
		lseek (r->fd, 0L, 0);
		if (ffcopy (r->fd, r->bakfd) < 0) {
			close (r->bakfd);
			r->bakfd = -1;
			unlink (bak);
			return (-1);
		}
		close (r->bakfd);
		r->bakfd = open (bak, 0);
		if (r->bakfd < 0) {
			error ("Cannot open %s", bak);
			unlink (bak);
			return (-1);
		}
		close (r->fd);
		r->fd = r->bakfd;
	}
	fd = creat (filename, 0664);
	if (fd < 0) {
		error ("Cannot create %s", filename);
		unlink (bak);
		return (-1);
	}
	for (i=0; i<r->len; ++i) {
		p = RecGet (r, i);
		if (p->slen)
			write (fd, p->ptr, (unsigned) p->slen);
		write (fd, "\n", 1);
	}
	close (fd);
	return (0);
}

RecBreak (r)
REC *r;
{
	register i;
	register struct ldesc *x;

	x = r->lindex;
	for (i=0; i<r->len; ++i, ++x)
		x->flags &= ~XNOEOLN;
}

static LINE readline (fd, seek, len)
long seek;
{
	register l, n;
	register char *s;
	LINE rez;

	rez.slen = len;
	rez.mod = 0;
	if (! len) {
		rez.ptr = "";
		return (rez);
	}
	rez.ptr = (char *) MemAlloc (rez.slen);
	if (lseek (fd, seek, 0) < 0)
		error ("Cannot lseek on reading");
	for (l=rez.slen, s=rez.ptr; l>0; l-=n, s+=n) {
		n = read (fd, s, (unsigned) l);
		if (n <= 0) {
			error ("Cannot read line");
			if (rez.slen)
				MemFree ((mem *) rez.ptr);
			rez.slen = 0;
			rez.ptr = "";
			return (rez);
		}
	}
	return (rez);
}

static freeline (r)
register REC *r;
{
	register struct map *m;
	register struct ldesc *x;
	register LINE *l;
	register mintime, minindex;

	/* find free place in pool */
	for (m=r->rmap; m<r->rmap+POOLSZ; ++m)
		if (! m->busy)
			return (m - r->rmap);
	/* pool is full; find the oldest line */
	mintime = r->rmap[0].time;
	minindex = 0;
	for (m=r->rmap; m<r->rmap+POOLSZ; ++m)
		if (m->time < mintime)
			minindex = m - r->rmap;
	m = &r->rmap[minindex];
	l = &r->pool[minindex];
	x = &r->lindex[m->index];
	/* remove line from pool */
	if (l->mod) {           /* line is modified, save it in temp file */
		if ((x->flags & XTEMP) && l->oldlen)
			tempfree (r, x->seek, l->oldlen);
		x->seek = tempsave (r, l->ptr, l->slen);
		x->llen = l->slen;
		x->flags |= XTEMP;
	}
	if (l->slen)
		MemFree ((mem *) l->ptr);
	x->poolindex = NOINDEX;
	m->busy = 0;
	return (minindex);
}

LINE *RecGet (r, n)
register REC *r;
{
	register struct ldesc *x;
	register struct map *m;
	register LINE *p;
	static long timecount = 1;              /* time stamp */

	if (n < 0 || n >= r->len)
		return (0);
	x = &r->lindex[n];
	if (x->poolindex != NOINDEX) {          /* line is in cache */
		r->rmap[x->poolindex].time = ++timecount;
		return (&r->pool[x->poolindex]);
	}
	x->poolindex = freeline (r);            /* get free pool index */
	p = &r->pool[x->poolindex];
	m = &r->rmap[x->poolindex];
	m->time = ++timecount;
	m->index = n;
	m->busy = 1;
	/* read line from file */
	*p = readline ((x->flags & XTEMP) ? r->tfd : r->fd, x->seek, x->llen);
	p->noeoln = (x->flags & XNOEOLN) != 0;
	return (p);
}

RecPut (p, newlen)
register LINE *p;
{
	if (! p->mod)
		p->oldlen = p->slen;
	p->mod = 1;
	p->slen = newlen;
}

RecDelChar (r, line, off)
register REC *r;
{
	register LINE *p;
	char *s;

	if (! (p = RecGet (r, line)))
		return;
	if (p->slen <= off)
		return;
	s = (char *) MemAlloc (p->slen-1);
	if (off)
		MemCopy (s, p->ptr, off);
	if (off <= p->slen-1)
		MemCopy (s+off, p->ptr+off+1, p->slen-off-1);
	MemFree ((mem *) p->ptr);
	p->ptr = s;
	RecPut (p, p->slen - 1);
}

RecInsChar (r, line, off, sym)
register REC *r;
{
	register LINE *p;
	char *s;

	if (! (p = RecGet (r, line)))
		return;
	s = (char *) MemAlloc (p->slen+1);
	if (off)
		MemCopy (s, p->ptr, off);
	s [off] = sym;
	if (off < p->slen)
		MemCopy (s+off+1, p->ptr+off, p->slen-off);
	if (p->slen)
		MemFree ((mem *) p->ptr);
	p->ptr = s;
	RecPut (p, p->slen + 1);
}

RecInsLine (r, n)
register REC *r;
{
	register struct ldesc *x, *i;
	register struct map *m;
	register k;

	if (n<0 || n>r->len)
		return;
	++r->len;
	MemCheckIndex (r->lindex, struct ldesc *, r->nindex, QUANT, r->len+1);
	i = &r->lindex[n];
	for (x= &r->lindex[r->len-1]; x>i; --x)
		x[0] = x[-1];
	i->seek = 0;
	i->llen = 0;
	i->poolindex = NOINDEX;
	i->flags = XTEMP;
	m = r->rmap;
	for (k=0; k<POOLSZ; ++k, ++m)
		if (m->index >= n)
			++m->index;
}

RecDelLine (r, n)
register REC *r;
{
	register struct ldesc *x, *i;
	register struct map *m;
	register LINE *l;
	register k;

	if (n<0 || n>=r->len)
		return;
	x = &r->lindex[n];
	m = r->rmap;
	if (x->poolindex != NOINDEX) {          /* exclude line from pool */
		l = &r->pool[x->poolindex];
		if (x->flags & XTEMP)
			if (l->mod) {
				if (l->oldlen)
					tempfree (r, x->seek, l->oldlen);
			} else if (l->slen)
				tempfree (r, x->seek, l->slen);
		if (l->slen)
			MemFree ((mem *) l->ptr);
		m[x->poolindex].busy = 0;
	}
	for (k=0; k<POOLSZ; ++k, ++m)
		if (m->index > n)
			--m->index;
	i = &r->lindex[r->len-1];
	for (x= &r->lindex[n]; x<i; ++x)
		x[0] = x[1];
	--r->len;
}
