# include <stdio.h>
# include <ediag.h>

# define PL 256
# define ESC '\033'
# define RLF '\013'
# define SI '\017'
# define SO '\016'
# define CLOSE '\003'
# define MINUS '-'
# define VERT  '|'
# define PLUS  '+'
# define GREEK 0200
# define LINELN 800
# define BYTE 0377

char   *page[PL];               /* ������ ���������� �� ������ �������� */
char    lbuff[LINELN],
       *line;
int     bflag,                  /* ���� ����� -b */
	hflag,                  /* ���� ����� -h */
	fflag;                  /* ���� ����� -f */
int     tflag,                  /* ���� ����� -t (�����) */
	pflag;                  /* ���� ����� -p (�����) */
int     half;
int     lp,
	cp;                     /* ��������� ������� �� ������ */
int     ll;                     /* ��������� ������ ������ � ������
				   �������� */
int     llh;                    /* ������������ ����� ���������� ������ */
int     mustwr;                 /* ���������� ������������ ����� */
int     pcp = 0;
char   *pgmname;
char   *strcpy ();

char    l1[264],
	l2[264],
	l3[264];
char   *pb,                     /* ��������� �� ���������� ������ */
       *pc,                     /* ��������� �� ������� ������ */
       *pf,                     /* ��������� �� ��������� ������ */
       *pp;                     /* ��������� �� ������������ ������ */
int     ns = 0;                 /* ���������� ����������� ����� */

main (argc, argv)
int     argc;
char  **argv;
{
    int     i;
    int     greek;
    register int    c;
    char    fbuff[BUFSIZ];

    setbuf (stdout, fbuff);
    pgmname = argv[0];

    for (i = 1; i < argc; i++) {
	register char  *p;
	if (*argv[i] != '-') {
	Usage:
	    fprintf (stderr, ediag("Usage: %s [-bhftp]\n",
				   "�����: %s [-bhftp]\n"), pgmname);
	    exit (2);
	}
	for (p = argv[i] + 1; *p; p++) {
	    switch (*p) {
		case 'b':
		    bflag++;
		    break;

		case 'h':
		    hflag++;
		    break;

		case 'f':
		    fflag++;
		    break;

		case 't':
		    tflag++;
		    break;

		case 'p':
		    pflag++;
		    break;

		default:
		    goto Usage;
	    }
	}
    }

    if (tflag) {
	pb = l1;
	pc = l2;
	pf = l3;
	pp = pc;
    }
    for (ll = 0; ll < PL; ll++)
	page[ll] = 0;

    cp = 0;
    ll = 0;
    greek = 0;
    mustwr = PL;
    line = lbuff;

    while ((c = getchar ()) != EOF) {
	switch (c) {
	    case '\n':
		incr ();
		incr ();
		cp = 0;
		continue;

	    case '\0':
		continue;

	    case ESC:
		c = getchar ();
		switch (c) {
		    case '7':   /* ��������� �� ������ */
			decr ();
			decr ();
			break;

		    case '8':   /* ��������� �� ���-������ */
			if (fflag)
			    decr ();
			else {
			    if (--half < -1) {
				decr ();
				decr ();
				half += 2;
			    }
			}
			break;

		    case '9':   /* ������������ �� ���-������ */
			if (fflag)
			    incr ();
			else {
			    if (++half > 0) {
				incr ();
				incr ();
				half -= 2;
			    }
			}
			break;
		}
		continue;

	    case SO:
		greek = GREEK;
		continue;

	    case SI:
		greek = 0;
		continue;

	    case RLF:
		decr ();
		decr ();
		continue;

	    case '\r':
		cp = 0;
		continue;

	    case '\t':
		cp = (cp + 8) & -8;
		continue;

	    case '\b':
		if (cp > 0)
		    cp--;
		continue;

	    case ' ':
		cp++;
		continue;

	    default:
		c &= BYTE;
		if (c > 040 && c < 0377) {
		    outc (c | greek);
		    cp++;
		}
		continue;
	}
    }

    for (i = 0; i < PL; i++)
	if (page[(mustwr + i) % PL] != 0)
	    emit (page[(mustwr + i) % PL], mustwr + i - PL);
    emit (" ", (llh + 1) & -2);
    outchar (CLOSE);            /* ����� ��������� ������ */
    exit (0);
}

outc (c)
register char   c;
{
    if (lp > cp) {
	line = lbuff;
	lp = 0;
    }

    while (lp < cp) {
	switch (*line) {
	    case '\0':
		*line = ' ';
		lp++;
		break;

	    case '\b':
		lp--;
		break;

	    default:
		lp++;
	}
	line++;
    }
    while (*line == '\b') {
	line += 2;
    }
    if (bflag || *line == '\0' || *line == ' ')
	*line = c;
    else {
	register char   c1,
			c2,
			c3;
	c1 = *++line;
	*line++ = '\b';
	c2 = *line;
	*line++ = c;
	while (c1) {
	    c3 = *line;
	    *line++ = c1;
	    c1 = c2;
	    c2 = c3;
	}
	lp = 0;
	line = lbuff;
    }
}

store (lno) {                   /* ������ ������ � ���������� ����� */
    char   *malloc ();

    lno %= PL;
    if (page[lno] != 0)
	free (page[lno]);
    page[lno] = malloc ((unsigned) strlen (lbuff) + 2);
    if (page[lno] == 0) {
	fprintf (stderr, ediag("%s: no memory\n",
			       "%s: ��� ������\n"), pgmname);
	exit (2);
    }
    strcpy (page[lno], lbuff);
}

fetch (lno) {                   /* ���������� ������ �� ���������� ������
				*/
    register char  *p;

    lno %= PL;
    p = lbuff;
    while (*p)
	*p++ = '\0';
    line = lbuff;
    lp = 0;
    if (page[lno])
	strcpy (line, page[lno]);
}

emit (s, lineno)
char   *s;
int     lineno;
{
    static int  cline = 0;
    register int    ncp;
    register char  *p;
    static int  gflag = 0;

    if (*s) {
	while (cline < lineno - 1) {
	    outchar ('\n');
	    pcp = 0;
	    cline += 2;
	}
	if (cline != lineno) {
	    outchar (ESC);
	    outchar ('9');
	    cline++;
	}
	if (pcp)
	    outchar ('\r');
	pcp = 0;
	p = s;
	while (*p) {
	    ncp = pcp;
	    while (*p++ == ' ') {
		if ((++ncp & 7) == 0 && hflag) {
		    pcp = ncp;
		    outchar ('\t');
		}
	    }
	    if (!*--p)
		break;
	    while (pcp < ncp) {
		outchar (' ');
		pcp++;
	    }

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/*                      if (gflag != (*p & GREEK) && *p != '\b') {  */
/*                              if (gflag)                          */
/*                                      outchar (SI);               */
/*                              else                                */
/*                                      outchar (SO);               */
/*                              gflag ^= GREEK;                     */
/*                      }                                           */
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

	    if (pflag && (*(p + 1) == '\b') &&
		    (*p == '_') && (*(p + 2) != '\n')) {
		outchar (*(p + 2) & BYTE);
		outchar ('\b');
		outchar (*p & BYTE);
		p = p + 3;
		pcp++;
	    }
	    else {
		outchar (*p & BYTE);
		if (*p++ == '\b')
		    pcp--;
		else
		    pcp++;
	    }
	}
    }
}

incr () {                       /* ����������� ������  �� ���� ������ */
    store (ll++);               /* ��������� ������� ������ � ����������
				   ��������� */
    if (ll > llh)
	llh = ll;
    if (ll >= mustwr && page[ll % PL]) {/* ������������ ������ */
	emit (page[ll % PL], ll - PL);/* �������� ����� ������ */
	mustwr++;               /* ����� ������ �� ��������� ������ */
	free (page[ll % PL]);   /* ���������� ����� � ������ */
	page[ll % PL] = 0;
    }
    fetch (ll);                 /* ������� ��������� ������ */
}

decr () {                       /* ����������� ����� �� ���� ������ */
    if (ll > mustwr - PL) {     /* ���� ���� ���� ��������� */
	store (ll--);           /* ��������� ������� */
	fetch (ll);             /* ������� ���������� */
    }
}

outchar (c)                     /* �������������� ����� � ������ -t */
char    c;
{

    char   *pw;                 /* ������� ���������� */

    if (!tflag) {               /* ��� ����� -t */
	putchar (c);
	return;
    }
    if (c == CLOSE) {
	pb = pc;
	pc = pf;
	pf = 0;
	outline (pb, pc, pf);
	return;
    }
    *pp++ = c;
    if (c != '\n')
	return;
    else {                      /* ����� ������ */
	*pp++ = 0;
	switch (ns) {
	    case 0:
		pp = pf;
		ns++;
		break;
	    case 1:
		pw = pb;
		pb = 0;
		outline (pb, pc, pf);
		pb = pc;
		pc = pf;
		pf = pw;
		pp = pf;
		ns++;
		break;
	    case 2:
		outline (pb, pc, pf);
		pw = pb;
		pb = pc;
		pc = pf;
		pf = pw;
		pp = pf;
		break;
	}
    }
}
outline (pb, pc, pf)
char   *pb,                     /* ��������� �� ������� ������ */
       *pc,                     /* ��������� �� ���������� ������ */
       *pf;                     /* ��������� �� ����������� ������ */
{
    char   *pw,                 /* ��������� �� ������� ������ */
	   *pw1,                /* ��������� �� ���������� ������ */
	   *pw2;                /* ��������� �� ����������� ������ */

    int     nc,                 /* ����� ������� � ������� ������ */
	    np;                 /* ����� ������� ��� ������ ������� ������
				   */

    pw = pc;
    pw1 = pb;
    pw2 = pf;
    np = 0;
    while (*pw) {
	nc++;
	np++;
	if (*pw++ == '\b') {    /* ��� BS (backspace) */
	    np -= 2;
	    if ((*(pw - 2) == MINUS) && (*pw == VERT)) {
		if (!np || (*(pw + 1) == '\n')) {/* ������� ������� */
		    if (compar (np + 1, pw1) && compar (np + 1, pw2)) {
			compres (VERT, pw - 2);
			pw = pw - 2;
		    }
		    else {
			compres (MINUS, pw - 2);
			pw = pw - 2;
		    }
		    continue;
		}
		if (!pb) {      /* ������� ������ */
		    if (compar (np + 1, pw2)) {
			compres (MINUS, pw - 2);
			pw = pw - 2;
		    }
		}
		else
		    if (pb && pf)/* �� ������� � �� ������ ������ */
			if (compar (np + 1, pw1) && compar (np + 1, pw2)) {
			    compres (PLUS, pw - 2);
			    pw = pw - 2;
			}
			else {
			    compres (MINUS, pw - 2);
			    pw = pw - 2;
			}
		    else        /* ������ ������ */
			if (compar (np + 1, pw1)) {
			    compres (MINUS, pw - 2);
			    pw = pw - 2;
			}
	    }
	    else
		continue;
	}
	else
	    continue;
    }
    while (*pc)
	putchar (*pc++);
}
compres (c, pc)
char    c;
char   *pc;
{
    char   *pr,                 /* ��������� ��� ������ */
	   *pw;                 /* ��������� ��� ������ */
    pr = pw = pc;
    *pw++ = c;
    pr += 3;
    while (*pr)
	*pw++ = *pr++;
    *pw = 0;
}
compar (np, pc)
int     np;                     /* ����� ������� ������� */
char   *pc;                     /* ��������� �� ������, � ������� ����
				   ������ */
{
    int     np1;
    char   *pw2;
    np1 = 0;
    pw2 = pc;
    while ((np1 != np) && *pw2) {
	if (*pw2++ == '\b')
	    np1 -= 2;
	np1++;
    }
    if (!*pw2)
	return (0);
    if (*(pw2 - 1) == VERT)
	return (1);
    return (0);
}

dummy () {                      /* �������� */
}
