#include "rcv.h"
#include <sys/stat.h>
#include <errno.h>
#ifdef  MSDOS
#include	<fcntl.h>
#endif

/*
 * Mail -- a mail program
 *
 * File I/O.
 *
 * $Log: fio.c,v $
 * Revision 1.18  1991/07/22  16:36:47  ache
 * Port to Borland C
 *
 * Revision 1.17  1991/04/26  17:01:10  ache
 * Cosmetic changes
 *
 * Revision 1.16  1991/04/19  22:45:31  asa
 * iZMENENIQ DLQ dEMOS 32
 *
 * Revision 1.15  1991/01/25  18:04:45  ache
 * uBRANY STARYE (4.1) SIGNALY
 *
 * Revision 1.14  1991/01/19  15:38:23  ache
 * UBRANY BUFERA 16K, KAK NE OPRAWDAW[IE DOWERIQ NARODA
 *
 * Revision 1.13  90/12/23  21:12:10  ache
 * bUFERIZACIQ IO PO 16 k
 *
 * Revision 1.12  90/12/22  22:52:58  ache
 * sORTIROWKA + WYDA^A fio
 * 
 * Revision 1.11  90/12/07  14:12:32  ache
 * pRAWLENA OBRABOTKA WREMENNYH FAJLOW I MELO^I
 * 
 * Revision 1.10  90/11/11  20:05:26  ache
 * iSPRAWLENO RASPOZNAWANIE IMEN FAJLOW
 * 
 * Revision 1.9  90/10/16  09:02:10  ache
 * wWEDENO AWTOISPRAWLENIE NEKORREKTNYH SOOB]ENIJ.
 * 
 * Revision 1.8  90/10/13  20:26:46  ache
 * O_BINARY for DOS
 * 
 * Revision 1.7  90/09/21  21:58:39  ache
 * MS-DOS extends + some new stuff
 * 
 * Revision 1.6  90/09/13  13:19:05  ache
 * MS-DOS & Unix together...
 * 
 * Revision 1.5  90/06/26  17:13:02  avg
 * tEPERX DLQ OBRABOTKI expand WSEGDA WYZYWAETSQ /bin/sh.
 * 
 * Revision 1.4  88/07/23  20:32:50  ache
 * rUSSKIE DIAGNOSTIKI
 * 
 * Revision 1.3  88/02/19  16:05:33  avg
 * dLQ \KONOMII PAMQTI W POLE m_size ZAMENEN long NA unsigned
 * DLQ MA[IN TIPA pdp11.
 *
 * Revision 1.2  88/01/11  12:26:01  avg
 * dOBAWLENY KUSKI DLQ RABOTY S PMCS.
 * u rcsid POSTAWLENY KOMMENTARII NOXSTR.
 *
 * Revision 1.1  87/12/25  15:59:14  avg
 * Initial revision
 *
 */

/*NOXSTR
static char rcsid[] = "$Header: /usr/src/Relcom/relcom/mailx/RCS/fio.c,v 1.18 1991/07/22 16:36:47 ache Exp $";
YESXSTR*/
extern char tempSet[], tempMesg[];
extern char *udate();
static int append();
static void makemessage();
extern long GetDate();
int SortByDate(), SortBySubject(), SortByAuthor();
/*
 * Set up the input pointers while copying the mail file into
 * /tmp.
 */
void
setptr(ibuf)
	FILE *ibuf;
{
	register int c;
	register char *cp, *cp2;
	register int count;
	long s, l;
	off_t offset;
	char linebuf[LINESIZE];
	char wbuf[LINESIZE];
	int maybe, flag, inhead, firstline;
	FILE *mestmp;
	struct message this;

#ifdef  MSDOS
	maketemp(tempSet);
#endif
	if (   fclear(tempSet) < 0
#ifndef MSDOS
		|| (mestmp = TmpOpen(tempSet, "a+")) == NULL
#else
		|| (mestmp = TmpOpen(tempSet, "a+b")) == NULL
#endif
	   ) {
		perror(tempSet);
		remove(tempSet);
		exit(1);
	}
	if (fseek (tf, (off_t)0, 2) != 0) {
		ioerror(tempMesg, 2);
		panic("Temp file seeking");
	}
	msgCount = 0;
	offset = 0;
	s = 0;
	l = 0;
	maybe = 1;
	inhead = 0;
	firstline = 1;
	flag = MUSED|MNEW;
	for (;;) {
		cp = linebuf;
		c = getc(ibuf);
		while (c != EOF && c != '\n') {
			if (cp - linebuf >= LINESIZE - 1) {
				ungetc(c, ibuf);
				break;
			}
			*cp++ = c;
			c = getc(ibuf);
		}
		*cp = '\0';
		if (cp == linebuf && c == EOF) {
			if (!maybe) {
			       fprintf(stderr, ediag("WARNING: wrong ended mailbox -- corrected\n",
						     "preduprevdenie: NEWERNO ZAKON^ENYJ PO^TOWYJ Q]IK -- ISPRAWLENO\n"));
				   putc('\n', tf);
#ifdef  MSDOS
				   s++;
#endif
			       s++;
				   l++;
			}
			(void) fflush(tf);
			if (ferror(tf)) {
				ioerror(tempMesg, 1);
				panic("Can't made temp file");
			}
			this.m_flag = flag;
			flag = MUSED|MNEW;
			this.m_offset = offset;
			this.m_size = s;
			this.m_lines = l;
			if (append(&this, mestmp)) {
				ioerror(tempSet, 1);
				panic("Can't append mesg header");
			}
			makemessage(mestmp);
			TmpDel(mestmp);
			return;
		}
		count = cp - linebuf + 1;
#ifdef  MSDOS
		if (c == '\n')
			count++;
#endif
		if (maybe && linebuf[0] == 'F' && ishead(linebuf)) {
			firstline = 0;
			inhead = 1;
			this.m_lines = l;
			l = 0;
	Header:
			msgCount++;
			this.m_flag = flag;
			flag = MUSED|MNEW;
			this.m_offset = offset;
			this.m_size = s;
			s = 0;
			if (append(&this, mestmp)) {
				ioerror(tempSet, 1);
				panic("Can't append mesg header");
			}
			if (firstline) {
				firstline = 0;
				goto BadFrom;
			}
		}
		else if (firstline) {
			char buf[80];

			fprintf(stderr, ediag("WARNING: wrong started mailbox -- corrected\n",
					      "preduprevdenie: NEWERNO NA^ATYJ PO^TOWYJ Q]IK -- ISPRAWLENO\n"));
			(void) sprintf(buf, "From postmaster %s\n\n", udate());
			count += strlen(buf);
#ifdef  MSDOS
			count += 2;
#endif
			inhead = 0;
			this.m_lines = 0;
			l += 2;
			fputs(buf, tf);
			goto Header;
		}
		else
	BadFrom:
		if (linebuf[0] == 'F' && strncmp("From ", linebuf, 5) == 0) {
			putc('>', tf);
			count++;
		}

		cp[0] = '\n';
		cp[1] = '\0';
		fputs(linebuf, tf);
		if (ferror(tf)) {
			ioerror(tempMesg, 1);
			panic("Can't write mesg to temp");
		}
		cp[0] = '\0';

		if (linebuf[0] == '\0')
			inhead = 0;
		if (inhead && index(linebuf, ':')) {
			cp = linebuf;
			cp2 = wbuf;
			while (isalpha(*cp))
				*cp2++ = *cp++;
			*cp2 = '\0';
			if (icequal(wbuf, "status")) {
				cp = index(linebuf, ':');
				if (index(cp, 'R'))
					flag |= MREAD;
				if (index(cp, 'O'))
					flag &= ~MNEW;
				inhead = 0;
			}
		}
		offset = ftell(tf);
		s += count;
		l++;
		maybe = (linebuf[0] == '\0');
	}
}

/*
 * Drop the passed line onto the passed output buffer.
 * If a write error occurs, return -1, else the count of
 * characters written, including the newline.
 */

putline(obuf, linebuf)
	FILE *obuf;
	char *linebuf;
{
	register int c;

	c = strlen(linebuf);
	fputs(linebuf, obuf);
	putc('\n', obuf);
	if (ferror(obuf))
		return(-1);
	return(c+1);
}

/*
 * Quickly read a line from the specified input into the line
 * buffer; return characters read.
 */

freadline(ibuf, linebuf)
	register FILE *ibuf;
	register char *linebuf;
{
	register int c;
	register char *cp;

	c = getc(ibuf);
	cp = linebuf;
	while (c != '\n' && c != EOF) {
		if (c == 0) {
			c = getc(ibuf);
			continue;
		}
		if (cp - linebuf >= BUFSIZ-1) {
			*cp = 0;
			return(cp - linebuf + 1);
		}
		*cp++ = c;
		c = getc(ibuf);
	}
	if (c == EOF && cp == linebuf)
		return(0);
	*cp = 0;
	return(cp - linebuf + 1);
}

/*
 * Read up a line from the specified input into the line
 * buffer.  Return the number of characters read.  Do not
 * include the newline at the end.
 */

readline(ibuf, linebuf)
	FILE *ibuf;
	char *linebuf;
{
	register char *cp;
	register int c;

	do {
		clearerr(ibuf);
		c = getc(ibuf);
		for (cp = linebuf; c != '\n' && c != EOF; c = getc(ibuf)) {
			if (c == 0)
				continue;
			if (cp - linebuf < LINESIZE-2)
				*cp++ = c;
		}
	} while (ferror(ibuf) && ibuf == stdin);
	*cp = 0;
	if (c == EOF && cp == linebuf)
		return(0);
	return(cp - linebuf + 1);
}

/*
 * Return a file buffer all ready to read up the
 * passed message pointer.
 */

FILE *
setinput(mp)
	register struct message *mp;
{
	if (fseek(tf, mp->m_offset, 0) != 0) {
		ioerror(tempSet, 2);
		panic("temporary file seek");
	}
	return(tf);
}

/*
 * Take the data out of the passed ghost file and toss it into
 * a dynamically allocated message structure.
 */

static
void
makemessage(f)
FILE *f;
{
	register struct message *m;
	char *s;
	int (*compar) ();

	if (message != NULL)
		cfree((char *) message);
	m = (struct message *)calloc((unsigned)(msgCount + 1), sizeof *m);
	if (m == NULL) {
		fprintf(stderr, ediag(
"Insufficient memory for %d messages\n",
"nE HWATAET PAMQTI DLQ %d SOOB]ENIJ\n"),
msgCount);
		exit(1);
	}
	message = m;
	dot = message;
	rewind(f);
	if (fread((char *)message, sizeof *m, msgCount + 1, f) != msgCount + 1) {
		ioerror(tempSet, 0);
		panic("Can't read mesg headers");
	}
	for (m = &message[0]; m < &message[msgCount]; m++) {
		m->m_size = (m+1)->m_size;
		m->m_lines = (m+1)->m_lines;
		m->m_flag = (m+1)->m_flag;
	}
	message[msgCount].m_size = 0;
	message[msgCount].m_lines = 0;
	if ((s = value("sort")) != NOSTR) {
		switch (tolower(*s)) {
		case 'd':
			compar = SortByDate;
			break;
		case 's':
			compar = SortBySubject;
			break;
		case 'a':
			compar = SortByAuthor;
			break;
		default:
			fprintf(stderr, ediag("WARNING: bad sort order, must be one of the date, subject, author\n",
					      "preduprevdenie: PORQDOK SORTIROWKI DOLVEN BYTX date, subject ILI Author\n"));
			return;
		}
		qsort((char *)message, msgCount, sizeof *m, compar);
	}
}

/*
 * Append the passed message descriptor onto the temp file.
 * If the write fails, return 1, else 0
 */

static
int
append(mp, f)
FILE *f;
struct message *mp;
{
	return (fwrite((char *) mp, sizeof *mp, 1, f) != 1);
}

#ifndef MSDOS
/*
 * Delete a file, but only if the file is a plain file.
 */

remove(name)
	char name[];
{
	struct stat statb;
	extern int errno;

	if (stat(name, &statb) < 0)
		return(-1);
	if ((statb.st_mode & S_IFMT) != S_IFREG) {
		errno = EISDIR;
		return(-1);
	}
	return(unlink(name));
}
#endif  /* not MSDOS */

/*
 * Terminate an editing session by attempting to write out the user's
 * file from the temporary.  Save any new stuff appended to the file.
 */
void
edstop()
{
	register int gotcha, c;
	register struct message *mp;
	FILE *obuf, *ibuf, *readstat;
	struct stat statb;
	char *id;
	extern char tempBack[];

	if (readonly)
		return;
	holdsigs();
	if (Tflag != NOSTR) {
		if ((readstat = fopen(Tflag, "w")) == NULL)
			Tflag = NOSTR;
	}
	for (mp = &message[0], gotcha = 0; mp < &message[msgCount]; mp++) {
		if (mp->m_flag & MNEW) {
			mp->m_flag &= ~MNEW;
			mp->m_flag |= MSTATUS;
		}
		if (mp->m_flag & (MODIFY|MDELETED|MSTATUS))
			gotcha++;
		if (Tflag != NOSTR && (mp->m_flag & (MREAD|MDELETED)) != 0) {
			if ((id = hfield("article-id", mp)) != NOSTR)
				fprintf(readstat, "%s\n", id);
		}
	}
	if (Tflag != NOSTR)
		fclose(readstat);
	if (!gotcha || Tflag != NOSTR)
		goto done;
	ibuf = NULL;
	if (stat(editfile, &statb) >= 0 && statb.st_size > mailsize) {
#ifdef  MSDOS
		maketemp(tempBack);
#endif
		if (   fclear(tempBack) < 0
			|| (obuf = TmpOpen(tempBack, "a+")) == NULL
		   ) {
			perror(tempBack);
			remove(tempBack);
			relsesigs();
			reset(0);
		}
		if ((ibuf = fopen(editfile, "r")) == NULL) {
			perror(editfile);
			TmpDel(obuf);
			relsesigs();
			reset(0);
		}
		fseek(ibuf, mailsize, 0);
		while ((c = getc(ibuf)) != EOF)
			putc(c, obuf);
		fclose(ibuf);
		ibuf = obuf;
	}
	printf("\"%s\" ", editfile);
	flush();
	if ((obuf = fopen(editfile, "w")) == NULL) {
		perror(editfile);
		TmpDel(ibuf);
		relsesigs();
		reset(0);
	}
	c = 0;
	for (mp = &message[0]; mp < &message[msgCount]; mp++) {
		if ((mp->m_flag & MDELETED) != 0)
			continue;
		c++;
		if (send(mp, obuf, 0) < 0) {
			ioerror(editfile, 1);
			TmpDel(ibuf);
			fclose(obuf);
			relsesigs();
			reset(0);
		}
	}
	gotcha = (c == 0 && ibuf == NULL);
	if (ibuf != NULL) {
		rewind(ibuf);
		while ((c = getc(ibuf)) != EOF)
			putc(c, obuf);
		TmpDel(ibuf);
	}
	fflush(obuf);
	if (ferror(obuf)) {
		ioerror(editfile, 1);
		relsesigs();
		fclose(obuf);
		reset(0);
	}
	fclose(obuf);
	if (gotcha) {
		remove(editfile);
		printf(ediag("removed\n","UDALEN\n"));
	}
	else
		printf(ediag("complete\n","ZAWER[ENO\n"));
	flush();

done:
	relsesigs();
}

static int sigdepth = 0;                /* depth of holdsigs() */
#ifndef MSDOS
static sigtype (*oldhup)();
#endif
static sigtype (*oldint)();
static sigtype (*oldquit)();
/*
 * Hold signals SIGHUP - SIGQUIT.
 */
holdsigs()
{
	register int i;

	if (sigdepth++ == 0) {
#ifndef MSDOS
		oldhup = signal(SIGHUP, SIG_IGN);
#endif
		oldint = signal(SIGINT, SIG_IGN);
		oldquit = signal(SIGQUIT, SIG_IGN);
	}
}

/*
 * Release signals SIGHUP - SIGQUIT
 */
relsesigs()
{
	register int i;

	if (--sigdepth == 0) {
#ifndef MSDOS
		signal(SIGHUP, oldhup);
#endif
		signal(SIGINT, oldint);
		signal(SIGQUIT, oldquit);
	}
}

/*
 * Empty the output buffer.
 */

clrbuf(buf)
	register FILE *buf;
{
	rewind(buf);
}


/*
 * Flush the standard output.
 */

flush()
{
	fflush(stdout);
	fflush(stderr);
}

/*
 * Determine the size of the file possessed by
 * the passed buffer.
 */

off_t
fsize(iob)
	FILE *iob;
{
	register int f;
	struct stat sbuf;

	fflush(iob);
	f = fileno(iob);
	if (fstat(f, &sbuf) < 0)
		return(0);
	return(sbuf.st_size);
}

/*
 * Take a file name, possibly with shell meta characters
 * in it and expand it by using "sh -c echo filename"
 * Return the file name as a dynamic string.
 */

char *
expand(name)
	char name[];
{
	char xname[BUFSIZ];
	char cmdbuf[BUFSIZ];
	register int pid, l, rc;
	register char *cp;
	int s, pivec[2];
	sigtype (*sigint)();
	struct stat sbuf;

	if (name[0] == '+' && getfold(cmdbuf) >= 0) {
		cp = copy(cmdbuf, xname);
		copy(name + 1, cp);
		return(expand(savestr(xname)));
	}
#ifdef PMCS
	if (name[0] == '=') {
		if((cp = value("projspool")) == NOSTR)
			cp = PROJSPOOL;
		sprintf(xname, "%s%c%s", cp, SEPCHAR, name + 1);
		return(savestr(xname));
	}
#endif
#ifdef  MSDOS
	return name;
#else
	if (!anyof(name, "~{[*?$`'\"\\"))
		return(name);
	if (pipe(pivec) < 0) {
		perror("pipe");
		return(name);
	}
	sprintf(cmdbuf, "echo %s", name);
	if ((pid = vfork()) == 0) {
		close(pivec[0]);
		close(1);
		dup(pivec[1]);
		close(pivec[1]);
		close(2);
		execl("/bin/sh", "sh", "-c", cmdbuf, NULL);
		_exit(1);
	}
	if (pid == -1) {
		perror("fork");
		close(pivec[0]);
		close(pivec[1]);
		return(NOSTR);
	}
	close(pivec[1]);
	l = read(pivec[0], xname, BUFSIZ);
	close(pivec[0]);
	while (wait(&s) != pid);
		;
	s &= 0377;
	if (s != 0 && s != SIGPIPE) {
		fprintf(stderr, ediag("\"Echo\" failed\n","o[IBKA \"echo\"\n"));
		goto err;
	}
	if (l < 0) {
		ioerror("pipe", 0);
		goto err;
	}
	if (l == 0) {
		fprintf(stderr, ediag(
"\"%s\": No match\n",
"\"%s\": BOLX[E NET\n"),
name);
		goto err;
	}
	if (l == BUFSIZ) {
		fprintf(stderr, ediag(
"Buffer overflow expanding \"%s\"\n",
"pEREPOLNILSQ BUFER WO WREMQ \"%s\"\n"),
name);
		goto err;
	}
	xname[l] = 0;
	for (cp = &xname[l-1]; *cp == '\n' && cp > xname; cp--)
		;
	*++cp = '\0';
	if (any(' ', xname) && stat(xname, &sbuf) < 0) {
		fprintf(stderr, ediag(
"\"%s\": Ambiguous\n",
"\"%s\": nEODNOZNA^NO\n"),
name);
		goto err;
	}
	return(savestr(xname));
#endif  /* not MSDOS */
err:
	return(NOSTR);
}

/*
 * Determine the current folder directory name.
 */
getfold(name)
	char *name;
{
	char *folder, *cp;

	if ((folder = value("folder")) == NOSTR)
		return(-1);
	if (*folder == '/'
#ifdef	MSDOS
		|| *folder == SEPCHAR
		|| folder[0] != '\0' && folder[1] == ':'
#endif
	   )
		cp = copy(folder, name);
	else {
		cp = copy(homedir, name);
		if (cp[-1] != '/'
#ifdef	MSDOS
			&& cp[-1] != SEPCHAR
#endif
		   )
			*cp++ = SEPCHAR;
		cp = copy(folder, cp);
	}
	if (cp[-1] != '/'
#ifdef	MSDOS
		&& cp[-1] != SEPCHAR
#endif
	   )
		*cp++ = SEPCHAR;
	*cp = '\0';
#ifdef	MSDOS
	for (cp = name; *cp; cp++)
		if (*cp == '/')
			*cp = SEPCHAR;
#endif
	return(0);
}

#ifndef _NFILE
#define _NFILE  20
#endif

static struct {
	char *t_name;
	FILE *t_f;
} TmpTable[_NFILE];

static short TmpCount = 0;


FILE *
TmpOpen(name, mode)
char *name, *mode;
{
	int i;
	FILE *f;
	int first = -1;

	for (i = 0; i < TmpCount; i++) {
		if (TmpTable[i].t_name == NULL) {
			if (first < 0)
				first = i;
		}
#ifdef MSDOS
		else if (strcmp(TmpTable[i].t_name, name) == 0) {
			char buf[100];

			sprintf(buf, "temp name %s already in use!", name);
			_error(buf);
		}
#endif
	}
	if ((f = fopen(name, mode)) == NULL)
		return NULL;
#ifndef MSDOS
	remove(name);
#endif
	if (first < 0)
		first = TmpCount++;
	if ((TmpTable[first].t_name = calloc(strlen(name) + 1, 1)) == NULL)
		_error("no memory!");
	strcpy(TmpTable[first].t_name, name);
	TmpTable[first].t_f = f;

	return f;
}

void
TmpDel(f)
FILE *f;
{
	int i;

	if (f == NULL)
		return;

	for (i = 0; i < TmpCount; i++)
		if (TmpTable[i].t_name != NULL && TmpTable[i].t_f == f) {
			fclose(f);
			if (f == image)
				image = NULL;
			if (f == tf)
				tf = NULL;
#ifdef  MSDOS
			remove(TmpTable[i].t_name);
#endif
			free(TmpTable[i].t_name);
			TmpTable[i].t_name = NULL;
			return;
		}
	_error("temp name not found!");
}


void
TmpDelAll()
{
	int i;

	for (i = 0; i < TmpCount; i++)
		if (   TmpTable[i].t_name != NULL
		    && TmpTable[i].t_f != tf
		   )
			TmpDel(TmpTable[i].t_f);
}

fclear(name)
char *name;
{
	struct stat st;
	int mode;
	int f;

	if (stat(name, &st) < 0)
		mode = 0600;
	else
		mode = st.st_mode;
#ifdef MSDOS
	/* Trick to save hidden bit */
	if (access(name, 0) == -1) {
		if ((f = creat(name, mode)) < 0)
			return -1;
	}
	else {
		if ((f = open(name, O_WRONLY|O_TRUNC|O_BINARY)) < 0)
			return -1;
	}
#else
	if ((f = creat(name, mode)) < 0)
		return -1;
#endif
	if (close(f) < 0)
		return -1;
	return f;
}

ioerror(name, mode)
char *name;
{
	char *s;

	switch (mode) {
	case 0:
		s = ediag("write","^TENIQ");
		break;
	case 1:
		s = ediag("read","ZAPISI");
		break;
	case 2:
		s = ediag("seek","POZICIONIROWANIQ");
		break;
	default:
		s = ediag("program","PROGRAMMY");
		break;
	}
	if (name == NULL)
		fprintf(stderr, ediag(": %s error!\n", ": O[IBKA %s!\n"),
			s);
	else
		fprintf(stderr, ediag("%s: %s error!\n", "%s: O[IBKA %s!\n"),
			name, s);
}

int
SortByDate(a, b)
struct message *a, *b;
{
	long date1, date2;

	date1 = GetDate(a);
	date2 = GetDate(b);
	sreset();

	return (date1 < date2 ? -1 : (date1 > date2 ? 1 : 0));
}

int
SortBySubject(a, b)
struct message *a, *b;
{
	char *s1, *s2;
	int i;

	s1 = hfield("subject", a);
	if (s1 == NOSTR)
		s1 = hfield("subj", a);
	s2 = hfield("subject", b);
	if (s2 == NOSTR)
		s2 = hfield("subj", b);

	if (s1) {
		while (isspace(*s1))
			s1++;
		if (!*s1)
			s1 = NOSTR;
	}
	if (s2) {
		while (isspace(*s2))
			s2++;
		if (!*s2)
			s2 = NOSTR;
	}
	if (s1 == NOSTR && s2 == NOSTR)
		goto Ret;
	if (s1 == NOSTR) {
		sreset();
		return -1;
	}
	if (s2 == NOSTR) {
		sreset();
		return 1;
	}

	if (strncmp(s1, "Re: ", 4) == 0)
		s1 += 4;
	if (strncmp(s2, "Re: ", 4) == 0)
		s2 += 4;

	if ((i = inumlcmp(s1, s2)) != 0) {
		sreset();
		return i;
	}

Ret:
	return SortByDate(a, b);
}

int
SortByAuthor(a, b)
struct message *a, *b;
{
	char *s1, *s2;
	int i;

	s1 = nameof(a, 0);
	s2 = nameof(b, 0);

	if ((i = istrlcmp(s1, s2)) != 0) {
		sreset();
		return i;
	}

	return SortByDate(a, b);
}
