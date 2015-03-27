# include "dir.h"
# include "deco.h"
# include "mem.h"

# define QUANT 128

extern uid, gid;

static struct ddir *compdir;

static comptype (), retag ();
extern char *extension (), *getwd (), *strncpy (), *strtail ();

/*
 *      Compare files. Used in call to qsort.
 *      Return -1, 0, 1 iff a is less, equal or greater than b.
 */

compfile (a, b)
register struct file *a, *b;
{
	register atype, btype;
	int rez;

	if (compdir->revsort) {
		struct file *t;

		t = a;
		a = b;
		b = t;
	}
	if (compdir->typesort) {
		atype = comptype (a);
		btype = comptype (b);
		if (atype != btype)
			return (atype<btype ? -1 : 1);
	}
	switch (compdir->sort) {
	case SORTEXT:
		atype = a->name[0] != '.';
		btype = b->name[0] != '.';
		if (atype != btype)
			return (atype<btype ? -1 : 1);
		if (rez = strcmp (extension (a->name), extension (b->name)))
			return (rez);
	case SORTNAME:
		return (strcmp (a->name, b->name));
	case SORTTIME:
		return (a->fstat.st_mtime < b->fstat.st_mtime ? -1 :
			a->fstat.st_mtime > b->fstat.st_mtime ? 1 : 0);
	case SORTSIZE:
		return (a->fstat.st_size < b->fstat.st_size ? -1 :
			a->fstat.st_size > b->fstat.st_size ? 1 : 0);
	}
	return (0);
}

/*
 *      Return integer number, used for comparison file types.
 */

static comptype (f)
register struct file *f;
{
	switch (f->fstat.st_mode & S_IFMT) {
	case S_IFDIR:
		if (f->name[0] == '.') {
			if (! f->name[1])
				return (-1);
			if (f->name[1] == '.' && ! f->name[2])
				return (-2);
		}
		if (! f->execable)
			return (1);
		return (0);
	case S_IFCHR:
		return (100);
	case S_IFBLK:
		return (110);
	case S_IFIFO:
		return (120);
# ifdef S_IFNAM
	case S_IFNAM:
		return (130);
# endif
# ifdef S_IFSOCK
	case S_IFSOCK:
		return (140);
# endif
# ifdef S_IFLNK
	case S_IFLNK:
		return (150);
# endif
	case S_IFREG:
		return (500);
	default:
		return (900);
	}
}

static chdirup (name)
register char *name;
{
	register char *s, *p;

	for (;;) {
		if (chdir (name) < 0)
			error ("Cannot chdir to %s", name);
		else if (access (".", 4) < 0)
			error ("Cannot read %s", name);
		else
			break;
		if (name[0]=='/' && name[1]==0) {       /* root directory */
			error ("DECO cannot work on such a MARASMATIC systems -- good bye !");
			quitdeco ();
		}
		/* cut last directory name */
		for (p=s=name; *s; ++s)
			if (*s == '/')
				p = s;
		*p = 0;
		if (! *name) {
			name [0] = '/';
			name [1] = 0;
		}
	}
}

/*
 *      Set up new directory.
 *      struct ddir *d   - durectory structure
 *      char *dirname   - name of directory.
 *                      If 0 then use d->cwd.
 */

setdir (d, dirname)
register struct ddir *d;
char *dirname;
{
# ifdef STDDIR
	DIR *f;
#    ifdef DIRENT
	struct dirent *recp;
#    else
	struct direct *recp;
#    endif
# define rec (*recp)
# else
	int f;
	struct direct rec;
# endif
	register struct file *p;
	struct ddir olddir;

	/* change directory */
	if (dirname)
		if (chdir (dirname) < 0)
			error ("Cannot chdir to %s", dirname);
		else
			getwd (d->cwd);
	chdirup (d->cwd);
	d->shortcwd = strtail (d->cwd, '/', 35);
	stat (d->cwd, &d->dstat);

# ifdef STDDIR
	f = opendir (".");
	if (f == 0)
# else
	f = open (".", 0);
	if (f == -1)
# endif
	{
		error ("Cannot read '.' -- good bye !");
		quitdeco ();
	}

	if (! dirname) {
		/* save directory structure, then use it for retagging */
		olddir = *d;
		d->catsz = 0;
		d->cat = 0;
	}

	if (! d->cat) {
		d->catsz = QUANT;
		d->cat = (struct file *) MemAlloc ((int) (d->catsz * sizeof (struct file)));
	}

	d->num = 0;
	d->nfiles = 0;
	d->nbytes = 0;
	d->mfiles = 0;
	d->mbytes = 0;
# ifdef STDDIR
	while (recp = readdir (f))
# else
	while (read (f, &rec, sizeof (rec)) == sizeof (rec))
# endif
	{
		if (! rec.d_ino)
			continue;
		MemCheckIndex (d->cat, struct file *, d->catsz, QUANT, d->num);
		p = & d->cat [d->num];
		p->link = 0;
# ifdef S_IFLNK
		lstat (rec.d_name, &p->fstat);
		if ((p->fstat.st_mode & S_IFMT) == (unsigned) S_IFLNK) {
			p->link = 1;
			stat (rec.d_name, &p->fstat);
		}
# else
		stat (rec.d_name, &p->fstat);
# endif

		/* skip .. in root directory */
		if (rec.d_name[0] == '.' && rec.d_name[1] == '.' &&
			rec.d_name[2] == 0 &&
			p->fstat.st_ino == d->dstat.st_ino &&
			p->fstat.st_dev == d->dstat.st_dev)
			continue;

		/* skip names not matched with pattern */
		if ((p->fstat.st_mode & S_IFMT) == (unsigned) S_IFREG) {
			++d->nfiles;
			d->nbytes += p->fstat.st_size;
			if (d->pattern [0] && ! match (rec.d_name, d->pattern))
				continue;
			++d->mfiles;
			d->mbytes += p->fstat.st_size;
		}

		strncpy (p->name, rec.d_name, 14);
		p->name[14] = 0;
# ifdef STDDIR
		p->truncname = (strlen (rec.d_name) > 14);
# else
		p->truncname = 0;
# endif
		p->execable = faccess (&p->fstat) & 1;
		p->tag = 0;
		++d->num;
	}

	d->topfile = d->curfile = 0;

	compdir = d;
	qsort ((char *) d->cat, (unsigned) d->num, sizeof (d->cat[0]), compfile);

	if (! dirname) {
		/* retag files, restore curfile */
		retag (d, &olddir);
		MemFree ((mem *) olddir.cat);
		counttag (d);
	}

# ifdef STDDIR
	closedir (f);
# else
	close (f);
# endif
	return (1);
}

char *filesize (f)
register struct file *f;
{
	static char buf [9];

	switch (f->fstat.st_mode & S_IFMT) {
	case S_IFDIR:
		return ("   <DIR>");
	case S_IFCHR:
		sprintf (buf, " %3d,%3d", f->fstat.st_rdev>>8&0377, f->fstat.st_rdev&0377);
		return (buf);
	case S_IFBLK:
		sprintf (buf, " %3d,%3d", f->fstat.st_rdev>>8&0377, f->fstat.st_rdev&0377);
		return (buf);
	case S_IFIFO:
		return ("  <FIFO>");
# ifdef S_IFNAM
	case S_IFNAM:
		return ("  <NAME>");
# endif
# ifdef S_IFSOCK
	case S_IFSOCK:
		return ("  <SOCK>");
# endif
# ifdef S_IFLNK
	case S_IFLNK:
		return ("  <LINK>");
# endif
	case S_IFREG:
		sprintf (buf, "%8ld", (long) f->fstat.st_size);
		return (buf);
	default:
		return ("     ???");
	}
}

char *filemode (m)
{
	static char buf [10];
	register char *p = buf;

	*p++ = m & S_IREAD ? 'r' : '-';
	*p++ = m & S_IWRITE ? 'w' : '-';
	*p++ = m & S_IEXEC ?
		(m & S_ISUID ? 's' : 'x') :
		(m & S_ISUID ? 'S' : '-');
	*p++ = m & S_IREAD >> 3 ? 'r' : '-';
	*p++ = m & S_IWRITE >> 3 ? 'w' : '-';
	*p++ = m & S_IEXEC >> 3 ?
		(m & S_ISGID ? 's' : 'x') :
		(m & S_ISGID ? 'S' : '-');
	*p++ = m & S_IREAD >> 6 ? 'r' : '-';
	*p++ = m & S_IWRITE >> 6 ? 'w' : '-';
	*p++ = m & S_IEXEC >> 6 ?
		(m & S_ISVTX ? 't' : 'x') :
		(m & S_ISVTX ? 'T' : '-');
	*p = 0;
	return (buf);
}

faccess (s)
register struct stat *s;
{
	if (uid == 0)
		return (s->st_mode & 0111 ? 7 : 6);
	if (uid == s->st_uid)
		return (s->st_mode >> 6 & 7);
	if (gid == s->st_gid)
		return (s->st_mode >> 3 & 7);
	return (s->st_mode & 7);
}

filetype (f)
register struct file *f;
{
	switch (f->fstat.st_mode & S_IFMT) {
	case S_IFDIR:
		if (! f->execable)
			return ('&');
		return ('/');
	case S_IFCHR:
		return ('$');
	case S_IFBLK:
		return ('#');
	case S_IFIFO:
		return ('=');
# ifdef S_IFNAM
	case S_IFNAM:
		return ('@');
# endif
# ifdef S_IFSOCK
	case S_IFSOCK:
		return ('!');
# endif
# ifdef S_IFLNK
	case S_IFLNK:
		return ('~');
# endif
	case S_IFREG:
		if (f->execable)
			return ('*');
		return (' ');
	default:
		return ('?');
	}
}

exist (name)
char *name;
{
	struct stat st;

	if (stat (name, &st) < 0)
		return (0);
	switch (st.st_mode & S_IFMT) {
	case S_IFDIR:
		return ('d');
	case S_IFREG:
		return ('f');
	default:
		return ('?');
	}
}

static retag (d, od)
struct ddir *d, *od;
{
	register struct file *f, *of;
	struct file *end, *oend, *ocur, *otop;
	int cmp;

	end = d->cat + d->num;
	oend = od->cat + od->num;
	ocur = od->cat + od->curfile;
	otop = od->cat + od->topfile;
	d->curfile = -1;
	compdir = d;
	for (f=d->cat, of=od->cat; of<oend; ++of) {
		while ((cmp = compfile (f, of)) < 0)
			if (++f >= end)
				goto breakloop;
		if (of == ocur)
			d->curfile = f - d->cat;
		if (of == otop)
			d->topfile = f - d->cat;
		if (cmp == 0)
			f->tag = of->tag;
	}
breakloop:
	if (d->curfile < 0) {
		d->curfile = d->num - 1;
		d->topfile = d->curfile - PAGELEN (d) + 1;
	} else if (d->topfile > d->curfile || d->topfile + PAGELEN (d) <= d->curfile)
		d->topfile = d->curfile - PAGELEN (d) + 1;
	while (d->topfile + PAGELEN (d) - H > d->curfile)
		d->topfile -= H;
	if (d->topfile < 0)
		d->topfile = 0;
}

counttag (d)
register struct ddir *d;
{
	register struct file *f;

	/* compute number of files and bytes */
	d->tfiles = 0;
	d->tbytes = 0;
	for (f=d->cat; f<d->cat+d->num; ++f)
		if ((f->fstat.st_mode & S_IFMT) == (unsigned) S_IFREG && f->tag) {
			++d->tfiles;
			d->tbytes += f->fstat.st_size;
		}
}

findfile (d, name)
register struct ddir *d;
char *name;
{
	/* set current file by name */
	register struct file *f;

	if (strlen (name) > 14)
		return;
	for (f=d->cat; f<d->cat+d->num; ++f)
		if (! strcmp (f->name, name)) {
			d->curfile = f - d->cat;
			break;
		}
	while (d->topfile > d->curfile)
		d->topfile -= H;
	if (d->topfile < 0)
		d->topfile = 0;
	while (d->topfile + PAGELEN (d) <= d->curfile)
		cur->topfile += H;
}
