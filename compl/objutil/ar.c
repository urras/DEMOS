/*
**      Обслуживание библиотечных (архивных) файлов.
**
**      ar [mrxtdpq][uvnbail] имя_архива файлы ...
**
**      Стандартное местоположение в системе:  /bin
**
**      Метод сборки:  cc -O -n -s
**
**      $Header: ar.c,v 1.1 86/10/30 04:21:21 alex Exp $
**      $Log:	ar.c,v $
 * Revision 1.1  86/10/30  04:21:21  alex
 * Initial revision
 * 
**
*/

static  char Rcs_id[] = "$Header: ar.c,v 1.1 86/10/30 04:21:21 alex Exp $";

#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef sparc
#include "/home/mellorn/zaitcev/d22/include/ar.h"
#else
#include <ar.h>
#endif

/* Manage values in PDP-11 byte order */
#ifdef sparc
#define GET16(v)   ((((v)&0377)<<8)+((v)>>8&0377))
#define PUT16(v)   ((((v)&0377)<<8)+((v)>>8&0377))
#define GET32(v)   ((((long)(v)&0xFF0000)<<8)+((long)(v)>>8&0xFF0000)\
			+(((v)&0377)<<8)+((v)>>8&0377))
#define PUT32(v)   ((((long)(v)&0xFF0000)<<8)+((long)(v)>>8&0xFF0000)\
			+(((v)&0377)<<8)+((v)>>8&0377))
#else
#define GET16(v)   (v)
#define PUT16(v)   (v)
#define GET32(v)   (v)
#define PUT32(v)   (v)
#endif

/* Incore representation of member header */
struct ar_xhdr {
	long	x_size;
	long    x_date;
	char    x_name[14];
	char    x_uid;
	char    x_gid;
	short   x_mode;
};

struct stat	stbuf;
struct ar_xhdr  arbuf;

#define	SKIP	1
#define	IODD	2
#define	OODD	4
#define	HEAD	8

char   *man = {
    "mrxtdpq"
};
char   *opt = {
    "uvnbail"
};

int     signum[] = {
    SIGHUP, SIGINT, SIGQUIT, 0
};
int     sigdone ();
long    lseek ();
int     rcmd ();
int     dcmd ();
int     xcmd ();
int     tcmd ();
int     pcmd ();
int     mcmd ();
int     qcmd ();
int     (*comfun) ();
char    flg[26];
char  **namv;
int     namc;
char   *arnam;
char   *ponam;
char   *tmpname = {
    "/tmp/vXXXXX"
};
char   *tmp1nam = {
    "/tmp/v1XXXXX"
};
char   *tmp2nam = {
    "/tmp/v2XXXXX"
};
char   *tfnam;
char   *tf1nam;
char   *tf2nam;
char   *file;
char    name[16];
int     af;
int     tf;
int     tf1;
int     tf2;
int     qf;
int     bastate;
char    buf[512];

char   *trim ();
char   *mktemp ();
char   *ctime ();

main (argc, argv)
char   *argv[];
{
    register    i;
    register char  *cp;

    for (i = 0; signum[i]; i++)
	if (signal (signum[i], SIG_IGN) != SIG_IGN)
	    signal (signum[i], sigdone);
    if (argc < 3)
	usage ();
    cp = argv[1];
    for (cp = argv[1]; *cp; cp++)
	switch (*cp) {
	    case 'l': 
	    case 'v': 
	    case 'u': 
	    case 'n': 
	    case 'a': 
	    case 'b': 
	    case 'c': 
	    case 'i': 
		flg[*cp - 'a']++;
		continue;

	    case 'r': 
		setcom (rcmd);
		continue;

	    case 'd': 
		setcom (dcmd);
		continue;

	    case 'x': 
		setcom (xcmd);
		continue;

	    case 't': 
		setcom (tcmd);
		continue;

	    case 'p': 
		setcom (pcmd);
		continue;

	    case 'm': 
		setcom (mcmd);
		continue;

	    case 'q': 
		setcom (qcmd);
		continue;

	    default: 
		fprintf (stderr, "ar: bad option `%c'\n", *cp);
		done (1);
	}
    if (flg['l' - 'a']) {
	tmpname = "vXXXXX";
	tmp1nam = "v1XXXXX";
	tmp2nam = "v2XXXXX";
    }
    if (flg['i' - 'a'])
	flg['b' - 'a']++;
    if (flg['a' - 'a'] || flg['b' - 'a']) {
	bastate = 1;
	ponam = trim (argv[2]);
	argv++;
	argc--;
	if (argc < 3)
	    usage ();
    }
    arnam = argv[2];
    namv = argv + 3;
    namc = argc - 3;
    if (comfun == 0) {
	if (flg['u' - 'a'] == 0) {
	    fprintf (stderr,
		    "ar: one of [%s] must be specified\n", man);
	    done (1);
	}
	setcom (rcmd);
    }
    (*comfun) ();
    done (notfound ());
}

setcom (fun)
int     (*fun) ();
{

    if (comfun != 0) {
	fprintf (stderr,
		"ar: only one of [%s] allowed\n", man);
	done (1);
    }
    comfun = fun;
}

rcmd () {
    register    f;

    init ();
    getaf ();
    while (!getdir ()) {
	bamatch ();
	if (namc == 0 || match ()) {
	    f = stats ();
	    if (f < 0) {
		if (namc)
		    fprintf (stderr,
			    "ar: cannot open %s\n", file);
		goto cp;
	    }
	    if (flg['u' - 'a']) {
		if (stbuf.st_mtime <= arbuf.x_date)
                {
		    close (f);
		    goto cp;
		}
            }
	    mesg ('r');
	    copyfil (af, -1, IODD + SKIP);
	    movefil (f);
	    continue;
	}
cp: 
	mesg ('c');
	copyfil (af, tf, IODD + OODD + HEAD);
    }
    cleanup ();
}

dcmd () {

    init ();
    if (getaf ())
	noar ();
    while (!getdir ()) {
	if (match ()) {
	    mesg ('d');
	    copyfil (af, -1, IODD + SKIP);
	    continue;
	}
	mesg ('c');
	copyfil (af, tf, IODD + OODD + HEAD);
    }
    install ();
}

xcmd () {
    register    f;

    if (getaf ())
	noar ();
    while (!getdir ()) {
	if (namc == 0 || match ()) {
	    f = creat (file, arbuf.x_mode & 0777);
	    if (f < 0) {
		fprintf (stderr,
			"ar: %s cannot create\n", file);
		goto sk;
	    }
	    mesg ('x');
	    copyfil (af, f, IODD);
	    close (f);
	    continue;
	}
sk: 
	mesg ('c');
	copyfil (af, -1, IODD + SKIP);
	if (namc > 0 && !morefil ())
	    done (0);
    }
}

pcmd () {

    if (getaf ())
	noar ();
    while (!getdir ()) {
	if (namc == 0 || match ()) {
	    if (flg['v' - 'a']) {
		printf ("\n<%s>\n\n", file);
		fflush (stdout);
	    }
	    copyfil (af, 1, IODD);
	    continue;
	}
	copyfil (af, -1, IODD + SKIP);
    }
}

mcmd () {

    init ();
    if (getaf ())
	noar ();
    tf2nam = mktemp (tmp2nam);
    close (creat (tf2nam, 0600));
    tf2 = open (tf2nam, 2);
    if (tf2 < 0) {
	fprintf (stderr, "ar: cannot create third temp\n");
	done (1);
    }
    while (!getdir ()) {
	bamatch ();
	if (match ()) {
	    mesg ('m');
	    copyfil (af, tf2, IODD + OODD + HEAD);
	    continue;
	}
	mesg ('c');
	copyfil (af, tf, IODD + OODD + HEAD);
    }
    install ();
}

tcmd () {

    if (getaf ())
	noar ();
    while (!getdir ()) {
	if (namc == 0 || match ()) {
	    if (flg['v' - 'a'])
		longt ();
	    printf ("%s\n", trim (file));
	}
	copyfil (af, -1, IODD + SKIP);
    }
}

qcmd () {
    register    i,
                f;

    if (flg['a' - 'a'] || flg['b' - 'a']) {
	fprintf (stderr,
		"ar: abi not allowed with q\n");
	done (1);
    }
    getqf ();
    for (i = 0; signum[i]; i++)
	signal (signum[i], SIG_IGN);
    lseek (qf, 0l, 2);
    for (i = 0; i < namc; i++) {
	file = namv[i];
	if (file == 0)
	    continue;
	namv[i] = 0;
	mesg ('q');
	f = stats ();
	if (f < 0) {
	    fprintf (stderr, "ar: %s cannot open\n", file);
	    continue;
	}
	tf = qf;
	movefil (f);
	qf = tf;
    }
}

init () {
    static short mbuf = PUT16(ARMAG);

    tfnam = mktemp (tmpname);
    close (creat (tfnam, 0600));
    tf = open (tfnam, 2);
    if (tf < 0) {
	fprintf (stderr,
		"ar: cannot create temp file\n");
	done (1);
    }
    if (write (tf, (char *) & mbuf, sizeof (short))
	    != sizeof (short))
	wrerr ();
}

getaf () {
    short mbuf;

    af = open (arnam, 0);
    if (af < 0)
	return (1);
    if (read (af, (char *) & mbuf, sizeof (short)) != sizeof (short)
	    || mbuf != PUT16(ARMAG)) {
	fprintf (stderr,
		"ar: %s not in archive format\n", arnam);
	done (1);
    }
    return (0);
}

getqf () {
    short mbuf;

    if ((qf = open (arnam, 2)) < 0) {
	if (!flg['c' - 'a'])
	    fprintf (stderr,
		    "ar: creating %s\n", arnam);
	close (creat (arnam, 0666));
	if ((qf = open (arnam, 2)) < 0) {
	    fprintf (stderr,
		    "ar: cannot create %s\n", arnam);
	    done (1);
	}
	mbuf = PUT16(ARMAG);
	if (write (qf, (char *) & mbuf, sizeof (short)) != sizeof (short))
	    wrerr ();
    }
    else
	if (read (qf, (char *) & mbuf, sizeof (short)) != sizeof (short)
		|| mbuf != PUT16(ARMAG)) {
	    fprintf (stderr,
		    "ar: %s not in archive format\n", arnam);
	    done (1);
	}
}

usage () {
    printf ("usage: ar [%s][%s] archive files ...\n", opt, man);
    done (1);
}

noar () {

    fprintf (stderr, "ar: %s does not exist\n", arnam);
    done (1);
}

sigdone () {
    done (100);
}

done (c) {

    if (tfnam)
	unlink (tfnam);
    if (tf1nam)
	unlink (tf1nam);
    if (tf2nam)
	unlink (tf2nam);
    exit (c);
}

notfound () {
    register    i,
                n;

    n = 0;
    for (i = 0; i < namc; i++)
	if (namv[i]) {
	    fprintf (stderr,
		    "ar: %s not found\n", namv[i]);
	    n++;
	}
    return (n);
}

morefil () {
    register    i,
                n;

    n = 0;
    for (i = 0; i < namc; i++)
	if (namv[i])
	    n++;
    return (n);
}

cleanup () {
    register    i,
                f;

    for (i = 0; i < namc; i++) {
	file = namv[i];
	if (file == 0)
	    continue;
	namv[i] = 0;
	mesg ('a');
	f = stats ();
	if (f < 0) {
	    fprintf (stderr, "ar: %s cannot open\n", file);
	    continue;
	}
	movefil (f);
    }
    install ();
}

install () {
    register    i;

    for (i = 0; signum[i]; i++)
	signal (signum[i], SIG_IGN);
    if (af < 0)
	if (!flg['c' - 'a'])
	    fprintf (stderr,
		    "ar: creating %s\n", arnam);
    close (af);
    af = creat (arnam, 0666);
    if (af < 0) {
	fprintf (stderr,
		"ar: cannot create %s\n", arnam);
	done (1);
    }
    if (tfnam) {
	lseek (tf, 0l, 0);
	while ((i = read (tf, buf, 512)) > 0)
	    if (write (af, buf, i) != i)
		wrerr ();
    }
    if (tf2nam) {
	lseek (tf2, 0l, 0);
	while ((i = read (tf2, buf, 512)) > 0)
	    if (write (af, buf, i) != i)
		wrerr ();
    }
    if (tf1nam) {
	lseek (tf1, 0l, 0);
	while ((i = read (tf1, buf, 512)) > 0)
	    if (write (af, buf, i) != i)
		wrerr ();
    }
}

/*
 * insert the file 'file'
 * into the temporary file
 */
movefil (f) {
    register char  *cp;
    register    i;

    cp = trim (file);
    for (i = 0; i < 14; i++)
	if (arbuf.x_name[i] = *cp)
	    cp++;
    arbuf.x_size = stbuf.st_size;
    arbuf.x_date = stbuf.st_mtime;
    arbuf.x_uid = stbuf.st_uid;
    arbuf.x_gid = stbuf.st_gid;
    arbuf.x_mode = stbuf.st_mode;
    copyfil (f, tf, OODD + HEAD);
    close (f);
}

stats () {
    register    f;

    f = open (file, 0);
    if (f < 0)
	return (f);
    if (fstat (f, &stbuf) < 0) {
	close (f);
	return (-1);
    }
    return (f);
}

/*
 * copy next file
 * size given in arbuf
 */
copyfil (fi, fo, flag) {
    register    i,
                o;
    int     pe;

    if (flag & HEAD) {
        struct ar_hdr abuf;
	memcpy( abuf.ar_name, arbuf.x_name, 14 );
#ifdef sparc
        abuf.ar_date[0] = PUT16(arbuf.x_date>>16);
        abuf.ar_date[1] = PUT16(arbuf.x_date);
        abuf.ar_size[0] = PUT16(arbuf.x_size>>16);
        abuf.ar_size[1] = PUT16(arbuf.x_size);
#else
	abuf.ar_date = arbuf.x_date;
	abuf.ar_size = arbuf.x_size;
#endif
	abuf.ar_uid = arbuf.x_uid;
	abuf.ar_gid = arbuf.x_gid;
        abuf.ar_mode = PUT16(arbuf.x_mode);
	if (write (fo, (char *) & abuf, sizeof abuf) != sizeof abuf)
	    wrerr ();
    }
    pe = 0;
    while (arbuf.x_size > 0) {
	i = o = 512;
	if (arbuf.x_size < i) {
	    i = o = arbuf.x_size;
	    if (i & 1) {
		if (flag & IODD)
		    i++;
		if (flag & OODD)
		    o++;
	    }
	}
	if (read (fi, buf, i) != i)
	    pe++;
	if ((flag & SKIP) == 0)
	    if (write (fo, buf, o) != o)
		wrerr ();
	arbuf.x_size -= 512;
    }
    if (pe)
	phserr ();
}

getdir () {
    register    i;
    struct ar_hdr abuf;

    i = read (af, (char *) & abuf, sizeof abuf);
    if (i != sizeof abuf) {
	if (tf1nam) {
	    i = tf;
	    tf = tf1;
	    tf1 = i;
	}
	return (1);
    }
    memcpy( arbuf.x_name, abuf.ar_name, 14 );
#ifdef sparc
    arbuf.x_date = GET16(abuf.ar_date[0])<<16;
    arbuf.x_date += (unsigned)GET16(abuf.ar_date[1]);
    arbuf.x_size = GET16(abuf.ar_size[0])<<16;
    arbuf.x_size += (unsigned)GET16(abuf.ar_size[1]);
#else
    arbuf.x_date = abuf.ar_date;
    arbuf.x_size = abuf.ar_size;
#endif
    arbuf.x_uid = abuf.ar_uid;
    arbuf.x_gid = abuf.ar_gid;
    arbuf.x_mode = GET16(abuf.ar_mode);
    for (i = 0; i < 14; i++)
	name[i] = abuf.ar_name[i];
    file = name;
    return (0);
}

match () {
    register    i;

    for (i = 0; i < namc; i++) {
	if (namv[i] == 0)
	    continue;
	if (strcmp (trim (namv[i]), file) == 0) {
	    file = namv[i];
	    namv[i] = 0;
	    return (1);
	}
    }
    return (0);
}

bamatch () {
    register    f;

    switch (bastate) {

	case 1: 
	    if (strcmp (file, ponam) != 0)
		return;
	    bastate = 2;
	    if (flg['a' - 'a'])
		return;

	case 2: 
	    bastate = 0;
	    tf1nam = mktemp (tmp1nam);
	    close (creat (tf1nam, 0600));
	    f = open (tf1nam, 2);
	    if (f < 0) {
		fprintf (stderr,
			"ar: cannot create second temp\n");
		return;
	    }
	    tf1 = tf;
	    tf = f;
    }
}

phserr () {

    fprintf (stderr, "ar: phase error on %s\n", file);
}

mesg (c) {

    if (flg['v' - 'a'])
	if (c != 'c' || flg['v' - 'a'] > 1)
	    printf ("%c - %s\n", c, file);
}

char   *
        trim (s)
char   *s;
{
    register char  *p1,
                   *p2;

    for (p1 = s; *p1; p1++);
    while (p1 > s) {
	if (*--p1 != '/')
	    break;
	*p1 = 0;
    }
    p2 = s;
    for (p1 = s; *p1; p1++)
	if (*p1 == '/')
	    p2 = p1 + 1;
    return (p2);
}

#define	IFMT	060000
#define	ISARG	01000
#define	LARGE	010000
#define	SUID	04000
#define	SGID	02000
#define	ROWN	0400
#define	WOWN	0200
#define	XOWN	0100
#define	RGRP	040
#define	WGRP	020
#define	XGRP	010
#define	ROTH	04
#define	WOTH	02
#define	XOTH	01
#define	STXT	01000

longt () {
    register char  *cp;
    long dbuf;

    pmode ();
    printf ("%3d/%1d", arbuf.x_uid, arbuf.x_gid);
    printf ("%7D", arbuf.x_size);
    cp = ctime (&arbuf.x_date);
    printf (" %-12.12s %-4.4s ", cp + 4, cp + 20);
}

int     m1[] = {
    1, ROWN, 'r', '-'
};
int     m2[] = {
    1, WOWN, 'w', '-'
};
int     m3[] = {
    2, SUID, 's', XOWN, 'x', '-'
};
int     m4[] = {
    1, RGRP, 'r', '-'
};
int     m5[] = {
    1, WGRP, 'w', '-'
};
int     m6[] = {
    2, SGID, 's', XGRP, 'x', '-'
};
int     m7[] = {
    1, ROTH, 'r', '-'
};
int     m8[] = {
    1, WOTH, 'w', '-'
};
int     m9[] = {
    2, STXT, 't', XOTH, 'x', '-'
};

int    *m[] = {
    m1, m2, m3, m4, m5, m6, m7, m8, m9
};

pmode () {
    register int  **mp;

    for (mp = &m[0]; mp < &m[9];)
	select (*mp++);
}

select (pairp)
int    *pairp;
{
    register int    n,
                   *ap;

    ap = pairp;
    n = *ap++;
    while (--n >= 0 && (arbuf.x_mode & *ap++) == 0)
	ap++;
    putchar (*ap);
}

wrerr () {
    perror ("ar write error");
    done (1);
}
