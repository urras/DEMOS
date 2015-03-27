# ifdef GETCWD

# ifndef MAXPATHLEN
#    define MAXPATHLEN  128
# endif

char *getwd (pathname)
char *pathname;
{
	extern char *getcwd ();

	return (getcwd (pathname, MAXPATHLEN));
}
# endif /* GETCWD */

# if !defined (GETWD) && !defined (GETCWD)

# include <sys/types.h>
# include <sys/stat.h>
# include <sys/dir.h>

# ifdef DIRENT
#    include <dirent.h>
# endif

# ifndef MAXPATHLEN
#    define MAXPATHLEN  128
# endif

# ifndef MAXNAMLEN
#    define MAXNAMLEN  14
# endif

extern char *strcpy ();
static pathsize;                        /* pathname length */

/*
 * prepend () tacks a directory name onto the front of a pathname.
 */

static char *prepend (dirname, pathname)
register char *dirname, *pathname;
{
	register i;                     /* directory name size counter */

	for (i=0; i<MAXNAMLEN && *dirname; ++i, ++dirname);
	if ((pathsize += i) < MAXPATHLEN)
		while (--i >= 0)
			*--pathname = *--dirname;
	return (pathname);
}

/*
 * getwd () returns the pathname of the current working directory. On error
 * an error message is copied to pathname and null pointer is returned.
 */

# ifdef DIRENT

char *getwd (pathname)
char *pathname;
{
	char pathbuf [MAXPATHLEN];      /* temporary pathname buffer */
	char *pnptr = &pathbuf [(sizeof pathbuf)-1]; /* pathname pointer */
	char curdir [MAXPATHLEN];       /* current directory buffer */
	char *dptr = curdir;		/* directory pointer */
	char *prepend ();               /* prepend dirname to pathname */
	dev_t cdev, rdev;		/* current & root device number */
	DIR *dirp;			/* directory stream */
	struct dirent *dir;             /* directory entry struct */
	struct stat d, dd;		/* file status struct */
# if defined (DIRBUF) && defined (i386) /* Special case of GETWD - INODE truncated to 32 bit's */
	ushort dirino, cino, rino;      /* current & root inode number */
# else
	ino_t dirino, cino, rino;       /* current & root inode number */
# endif

	pathsize = 0;
	*pnptr = '\0';
	stat ("/", &d);
	rdev = d.st_dev;
	rino = d.st_ino;
	strcpy (dptr, "./");
	dptr += 2;
	if (stat (curdir, &d) < 0) {
		/* GETWDERR ("getwd: can't stat ."); */
		goto fail;
	}
	for (;;) {
		if (d.st_ino == rino && d.st_dev == rdev)
			break;		/* reached root directory */
		cino = d.st_ino;
		cdev = d.st_dev;
		strcpy (dptr, "../");
		dptr += 3;
		if (! (dirp = opendir (curdir))) {
			/* GETWDERR ("getwd: can't open .."); */
			goto fail;
		}
		fstat (dirp->dd_fd, &d);
		if (cdev == d.st_dev) {
			if (cino == d.st_ino) {
				/* reached root directory */
				closedir (dirp);
				break;
			}
			do {
				if (! (dir = readdir (dirp))) {
					closedir (dirp);
					/* GETWDERR ("getwd: read error in .."); */
					goto fail;
				}
			/* PT(dir); */
			dirino = dir->d_ino;
			} while (dirino != cino);
		} else
			do {
				if (! (dir = readdir (dirp))) {
					closedir (dirp);
					/* GETWDERR ("getwd2: read error in .."); */
					goto fail;
				}
				strcpy (dptr, dir->d_name);
				stat (curdir, &dd);
				/* PT (dir); */
			} while (dd.st_ino != cino || dd.st_dev != cdev);
		closedir (dirp);
		pnptr = prepend ("/", prepend (dir->d_name, pnptr));
	}
	if (*pnptr == '\0')		/* current dir == root dir */
		strcpy (pathname, "/");
	else
		strcpy (pathname, pnptr);
	return (pathname);

fail:
	if (*pnptr)
		chdir (prepend (".", pnptr));
	strcpy (pathname, ".");
	return (pathname);
}

# else /* DIRENT */

char *getwd (pathname)
char *pathname;
{
	char pathbuf [MAXPATHLEN];               /* temporary pathname buffer */
	char *pnptr = & pathbuf [(sizeof pathbuf)-1]; /* pathname pointer */
	dev_t rdev;                     /* root device number */
	int fd;                         /* directory file descriptor */
	ino_t rino;                     /* root inode number */
	struct direct dir;              /* directory entry struct */
	struct stat d, dd;              /* file status struct */

	pathsize = 0;
	*pnptr = '\0';
	stat ("/", &d);
	rdev = d.st_dev;
	rino = d.st_ino;
	for (;;) {
		stat (".", &d);
		if (d.st_ino == rino && d.st_dev == rdev)
			break;          /* reached root directory */
		if ((fd = open ("..", 0)) < 0) {
			/* error ("Cannot open '..'"); */
			goto fail;
		}
		if (chdir ("..") < 0) {
			/* error ("Cannot chdir to '..'"); */
			goto fail;
		}
		fstat (fd, &dd);
		if (d.st_dev == dd.st_dev) {
			if (d.st_ino == dd.st_ino) {
				/* reached root directory */
				close (fd);
				break;
			}
			do {
				if (read (fd, (char *) &dir, sizeof (dir)) < sizeof(dir)) {
					close (fd);
					/* error ("Read error in '..'"); */
					goto fail;
				}
			} while (dir.d_ino != d.st_ino);
		} else
			do {
				if (read (fd, (char *) &dir, sizeof (dir)) < sizeof(dir)) {
					close (fd);
					/* error ("Read error in '..'"); */
					goto fail;
				}
				stat (dir.d_name, &dd);
			} while (dd.st_ino != d.st_ino || dd.st_dev != d.st_dev);
		close (fd);
		pnptr = prepend ("/", prepend (dir.d_name, pnptr));
	}
	if (*pnptr == '\0')             /* current dir == root dir */
		strcpy (pathname, "/");
	else if (chdir (pnptr) >= 0)
		strcpy (pathname, pnptr);
	else {
		/* error ("Cannot chdir to %s", pnptr); */
		chdir ("/");
		strcpy (pathname, "/");
	}
	return (pathname);

fail:
	if (*pnptr)
		chdir (prepend (".", pnptr));
	strcpy (pathname, ".");
	return (pathname);
}

# endif /* GETWD */

# endif /* DIRENT */
