#include "rcv.h"

/*
 * Mail -- a mail program
 *
 * Lexical processing of commands.
 *
 * $Log: lex.c,v $
 * Revision 1.17  1991/07/22  16:36:47  ache
 * Port to Borland C
 *
 * Revision 1.16  1991/04/19  22:48:42  asa
 * iZMENENIQ DLQ dEMOS 32
 *
 * Revision 1.15  1991/01/25  18:04:45  ache
 * uBRANY STARYE (4.1) SIGNALY
 *
 * Revision 1.14  1991/01/19  15:38:23  ache
 * UBRANY BUFERA 16K, KAK NE OPRAWDAW[IE DOWERIQ NARODA
 *
 * Revision 1.13  90/12/23  21:12:05  ache
 * bUFERIZACIQ IO PO 16 k
 * 
 * Revision 1.12  90/12/22  22:53:39  ache
 * sORTIROWKA + WYDA^A fio
 * 
 * Revision 1.11  90/12/07  07:15:16  ache
 * pEREDELANA OBRABOTKA WREMENNYH FAJLOW I MELO^I
 * 
 * Revision 1.10  90/10/04  04:07:10  ache
 * iSPRAWLENA OBRABOTKA #
 * 
 * Revision 1.9  90/09/21  21:58:57  ache
 * MS-DOS extends + some new stuff
 * 
 * Revision 1.8  90/09/13  13:19:30  ache
 * MS-DOS & Unix together...
 * 
 * Revision 1.7  90/06/14  13:44:37  avg
 * dOBAWLENO OBNULENIE uglyfromflag.
 * (PERED WYPOLNENIEM KOMANDY).
 * 
 * Revision 1.6  90/04/20  19:16:55  avg
 * pRIKRU^ENO POD System V
 * 
 * Revision 1.5  88/07/24  15:11:17  ache
 * pRI source TEPERX NE WYDAETSQ PO 10 RAZ PRIGLA[ENIE
 * 
 * Revision 1.4  88/07/23  22:09:17  ache
 * fORMAT WYDA^I
 * 
 * Revision 1.3  88/07/23  20:34:18  ache
 * rUSSKIE DIAGNOSTIKI
 * 
 * Revision 1.2  88/01/11  12:42:41  avg
 * dOBAWLEN NOXSTR U rcsid.
 * 
 * Revision 1.1  87/12/25  15:59:42  avg
 * Initial revision
 * 
 */

/*NOXSTR
static char rcsid[] = "$Header: /usr/src/Relcom/relcom/mailx/RCS/lex.c,v 1.17 1991/07/22 16:36:47 ache Exp $";
YESXSTR*/

#ifndef MSDOS
sigtype hangup();
#endif
#ifdef  VMUNIX
sigtype contin();
#endif
sigtype stop();

char    *prompt = "& ";

/*
 * Set up editing on the given file name.
 * If isedit is true, we are considered to be editing the file,
 * otherwise we are reading our mail which has signficance for
 * mbox and so forth.
 */

setfile(name, isedit)
	char *name;
{
	FILE *ibuf;
	int i;
	static int shudclob;
	static char efile[128];
	extern char tempMesg[];

	if ((ibuf = fopen(name, "r")) == NULL)
		return(-1);

	/*
	 * Looks like all will be well.  We must now relinquish our
	 * hold on the current set of stuff.  Must hold signals
	 * while we are reading the new file, else we will ruin
	 * the message[] data structure.
	 */

	holdsigs();
	if (shudclob) {
		if (edit)
			edstop();
		else
			quit();
	}

	/*
	 * Copy the messages into /tmp
	 * and set pointers.
	 */

	readonly = 0;
	if ((i = open(name, 1)) < 0)
		readonly++;
	else
		close(i);
	if (shudclob)
		TmpDel(tf);
	shudclob = 1;
	edit = isedit;
	strncpy(efile, name, 128);
	editfile = efile;
	if (name != mailname)
		strcpy(mailname, name);
	mailsize = fsize(ibuf);
#ifdef  MSDOS
	maketemp(tempMesg);
#endif
	if (   fclear(tempMesg) < 0
		|| (tf = TmpOpen(tempMesg, "a+")) == NULL
	   ) {
		perror(tempMesg);
		remove(tempMesg);
		exit(1);
	}
	setptr(ibuf);
	fclose(ibuf);
	setmsize(msgCount);
	relsesigs();
	sawcom = 0;
	return(0);
}

/*
 * Interpret user commands one by one.  If standard input is not a tty,
 * print no prompt.
 */

int     *msgvec;

void
commands()
{
	int eofloop, shudprompt;
	register int n;
	char linebuf[LINESIZE];
	extern int uglyfromflag;

# ifdef VMUNIX
	sigset(SIGCONT, SIG_DFL);
# endif
	if (rcvmode && !sourcing) {
		if (signal(SIGINT, SIG_IGN) != SIG_IGN)
			signal(SIGINT, stop);
#ifndef MSDOS
		if (signal(SIGHUP, SIG_IGN) != SIG_IGN)
			signal(SIGHUP, hangup);
#endif
	}
	for (;;) {
		setexit();

		/*
		 * Print the prompt, if needed.  Clear out
		 * string space, and flush the output.
		 */

		if (!rcvmode && !sourcing)
			return;
		eofloop = 0;
top:
		shudprompt = intty && !sourcing;
		if (shudprompt) {
			printf(prompt);
			flush();
# ifdef VMUNIX
			sigset(SIGCONT, contin);
# endif
		} else
			flush();
		sreset();

		/*
		 * Read a line of commands from the current input
		 * and handle end of file specially.
		 */

		n = 0;
		for (;;) {
			uglyfromflag = 0;
			if (readline(input, &linebuf[n]) <= 0) {
				if (n != 0)
					break;
				if (loading)
					return;
				if (sourcing) {
					unstack();
					goto more;
				}
				if (value("ignoreeof") != NOSTR && shudprompt) {
					if (++eofloop < 25) {
						printf(ediag(
"Use \"quit\" to quit.\n",
"iSPOLXZUJTE \"quit\" DLQ WYHODA.\n"));
						goto top;
					}
				}
				if (edit)
					edstop();
				return;
			}
			if ((n = strlen(linebuf)) == 0)
				break;
			n--;
#ifndef MSDOS
			if (linebuf[n] != '\\')
				break;
			linebuf[n++] = ' ';
#else
			break;
#endif
		}
# ifdef VMUNIX
		sigset(SIGCONT, SIG_DFL);
# endif
		if (execute(linebuf, 0)) {
			signal(SIGINT, SIG_IGN);
			return;
		}
more:           ;
	}
}

/*
 * Execute a single command.  If the command executed
 * is "quit," then return non-zero so that the caller
 * will know to return back to main, if he cares.
 * Contxt is non-zero if called while composing mail.
 */

execute(linebuf, contxt)
	char linebuf[];
{
	char word[LINESIZE];
	char *arglist[MAXARGC];
	struct cmd *com;
	register char *cp, *cp2;
	register int c;
	int muvec[2];
	int edstop(), e;

	/*
	 * Strip the white space away from the beginning
	 * of the command, then scan out a word, which
	 * consists of anything except digits and white space.
	 *
	 * Handle ! escapes differently to get the correct
	 * lexical conventions.
	 */

	cp = linebuf;
	while (any(*cp, " \t"))
		cp++;
	if (*cp == '!') {
		if (sourcing) {
			printf(ediag(
"Can't \"!\" while sourcing\n",
"nELXZQ \"!\" POKA WYPOLNQETSQ \"source\"\n"));
			unstack();
			return(0);
		}
		shell(cp+1);
		return(0);
	}
	cp2 = word;
	if (*cp == '#')
		*cp2++ = *cp++;
	else while (*cp && !any(*cp, " \t0123456789$#^.:/-+*'\""))
		*cp2++ = *cp++;
	*cp2 = '\0';

	/*
	 * Look up the command; if not found, bitch.
	 * Normally, a blank command would map to the
	 * first command in the table; while sourcing,
	 * however, we ignore blank lines to eliminate
	 * confusion.
	 */

	if (sourcing && equal(word, ""))
		return(0);
	com = lex(word);
	if (com == NONE) {
		printf(ediag(
"Unknown command: \"%s\"\n",
"nEWERNAQ KOMANDA: \"%s\"\n"),
word);
		if (loading)
			return(1);
		if (sourcing)
			unstack();
		return(0);
	}

	/*
	 * See if we should execute the command -- if a conditional
	 * we always execute it, otherwise, check the state of cond.
	 */

	if ((com->c_argtype & F) == 0)
		if (cond == CRCV && !rcvmode || cond == CSEND && rcvmode)
			return(0);

	/*
	 * Special case so that quit causes a return to
	 * main, who will call the quit code directly.
	 * If we are in a source file, just unstack.
	 */

	if (com->c_func == edstop && sourcing) {
		if (loading)
			return(1);
		unstack();
		return(0);
	}
	if (!edit && com->c_func == edstop)
		return(1);

	/*
	 * Process the arguments to the command, depending
	 * on the type he expects.  Default to an error.
	 * If we are sourcing an interactive command, it's
	 * an error.
	 */

	if (!rcvmode && (com->c_argtype & M) == 0) {
		printf(ediag(
"May not execute \"%s\" while sending\n",
"nELXZQ WYPOLNQTX \"%s\" WO WREMQ POSYLKI\n"),
		    com->c_name);
		if (loading)
			return(1);
		if (sourcing)
			unstack();
		return(0);
	}
	if (sourcing && (com->c_argtype & I) != 0) {
		printf(ediag(
"May not execute \"%s\" while sourcing\n",
"nELXZQ WYPOLNQTX \"%s\" WO WREMQ WYPOLNENIQ \"source\"\n"),
		    com->c_name);
		if (loading)
			return(1);
		unstack();
		return(0);
	}
	if (readonly && (com->c_argtype & W) != 0) {
		printf(ediag(
"May not execute \"%s\" -- message file is read only\n",
"nELXZQ WYPOLNQTX \"%s\" -- FAJL SOOB]ENIJ MOVNO TOLXKO ^ITATX\n"),
		   com->c_name);
		if (loading)
			return(1);
		if (sourcing)
			unstack();
		return(0);
	}
	if (contxt && (com->c_argtype & R) != 0) {
		printf(ediag(
"Cannot recursively invoke \"%s\"\n",
"nELXZQ REKURSIWNO WYZYWATX \"%s\"\n"),
com->c_name);
		return(0);
	}
	e = 1;
	switch (com->c_argtype & ~(F|P|I|M|T|W|R)) {
	case MSGLIST:
		/*
		 * A message list defaulting to nearest forward
		 * legal message.
		 */
		if (msgvec == 0) {
			printf(ediag(
"Illegal use of \"message list\"\n",
"nEWERNOE ISPOLXZOWANIE \"SPISKA SOOB]ENIJ\"\n"));
			return(-1);
		}
		if ((c = getmsglist(cp, msgvec, com->c_msgflag)) < 0)
			break;
		if (c  == 0) {
			*msgvec = first(com->c_msgflag,
				com->c_msgmask);
			msgvec[1] = (int)NULL;
		}
		if (*msgvec == (int)NULL) {
			printf(ediag(
"No applicable messages\n",
"nET PODHODQ]IH SOOB]ENIJ\n"));
			break;
		}
		e = (*com->c_func)(msgvec);
		break;

	case NDMLIST:
		/*
		 * A message list with no defaults, but no error
		 * if none exist.
		 */
		if (msgvec == 0) {
			printf(ediag(
"Illegal use of \"message list\"\n",
"nEWERNOE ISPOLXZOWANIE \"SPISKA SOOB]ENIJ\"\n"));
			return(-1);
		}
		if (getmsglist(cp, msgvec, com->c_msgflag) < 0)
			break;
		e = (*com->c_func)(msgvec);
		break;

	case STRLIST:
		/*
		 * Just the straight string, with
		 * leading blanks removed.
		 */
		while (any(*cp, " \t"))
			cp++;
		e = (*com->c_func)(cp);
		break;

	case RAWLIST:
		/*
		 * A vector of strings, in shell style.
		 */
		if ((c = getrawlist(cp, arglist)) < 0)
			break;
		if (c < com->c_minargs) {
			printf(ediag(
"%s requires at least %d arg(s)\n",
"%s PREDPOLAGAET PO MENX[EJ MERE %d ARGUMENTOW\n"),
				com->c_name, com->c_minargs);
			break;
		}
		if (c > com->c_maxargs) {
			printf(ediag(
"%s takes no more than %d arg(s)\n",
"%s WOSPRINIMAET NE BOLX[E ^EM %d ARGUMENTOW\n"),
				com->c_name, com->c_maxargs);
			break;
		}
		e = (*com->c_func)(arglist);
		break;

	case NOLIST:
		/*
		 * Just the constant zero, for exiting,
		 * eg.
		 */
		e = (*com->c_func)(0);
		break;

	default:
		panic(ediag("Unknown argtype","nEIZWESTNYJ TIP ARGUMENTOW"));
	}

	/*
	 * Exit the current source file on
	 * error.
	 */

	if (e && loading)
		return(1);
	if (e && sourcing)
		unstack();
	if (com->c_func == edstop)
		return(1);
	if (value("autoprint") != NOSTR && (com->c_argtype & P) != 0)
		if ((dot->m_flag & MDELETED) == 0) {
			muvec[0] = dot - &message[0] + 1;
			muvec[1] = 0;
			type(muvec);
		}
	if (!sourcing && (com->c_argtype & T) == 0)
		sawcom = 1;
	return(0);
}

#ifdef  VMUNIX
/*
 * When we wake up after ^Z, reprint the prompt.
 */
sigtype contin(s)
{

	printf(prompt);
	flush();
}
#endif

#ifndef MSDOS
/*
 * Branch here on hangup signal and simulate quit.
 */
sigtype hangup()
{

	holdsigs();
	if (edit) {
		if (setexit())
			exit(0);
		edstop();
	}
	else
		quit();
	exit(0);
}
#endif

/*
 * Set the size of the message vector used to construct argument
 * lists to message list functions.
 */

setmsize(sz)
{

	if (msgvec != NULL)
		cfree(msgvec);
	msgvec = (int *) calloc((unsigned) (sz + 1), sizeof *msgvec);
	if (msgvec == NULL)
		panic(ediag("No memory for mesg list","nET PAMQTI DLQ SPISKA SOOB]ENIJ"));
}

/*
 * Find the correct command in the command table corresponding
 * to the passed command "word"
 */

struct cmd *
lex(word)
	char word[];
{
	register struct cmd *cp;
	extern struct cmd cmdtab[];

	for (cp = &cmdtab[0]; cp->c_name != NOSTR; cp++)
		if (isprefix(word, cp->c_name))
			return(cp);
	return(NONE);
}

/*
 * Determine if as1 is a valid prefix of as2.
 * Return true if yep.
 */

isprefix(as1, as2)
	char *as1, *as2;
{
	register char *s1, *s2;

	s1 = as1;
	s2 = as2;
	while (*s1++ == *s2)
		if (*s2++ == '\0')
			return(1);
	return(*--s1 == '\0');
}

/*
 * The following gets called on receipt of a rubout.  This is
 * to abort printout of a command, mainly.
 * Dispatching here when command() is inactive crashes rcv.
 * Close all open files except 0, 1, 2, and the temporary.
 * The special call to getuserid() is needed so it won't get
 * annoyed about losing its open file.
 * Also, unstack all source files.
 */

int     inithdr;                        /* am printing startup headers */

#ifndef _NFILE
#define _NFILE  20
#endif

sigtype stop(s)
{
	register FILE *fp;
#ifdef  MSDOS
	extern char tempEdit[];
#endif

# ifndef VMUNIX
	s = SIGINT;
# endif
	noreset = 0;
	if (!inithdr)
		sawcom++;
	inithdr = 0;
	while (sourcing)
		unstack();
#ifndef MSDOS
	getuserid((char *) -1);
#endif
	TmpDelAll();
#ifdef __TURBOC__
#define _iob _streams
#endif
	for (fp = &_iob[0]; fp < &_iob[_NFILE]; fp++) {
		if (fp == stdin || fp == stdout || fp == stderr)
			continue;
		if (fp == tf)
			continue;
#ifndef MSDOS
		if (fp == pipef) {
			pclose(pipef);
			pipef = NULL;
			continue;
		}
#endif
	}
	clrbuf(stdout);
	printf(ediag("Interrupt\n","pRERYWANIE\n"));
	signal(s, stop);
	reset(0);
}

/*
 * Announce the presence of the current Mail version,
 * give the message count, and print a header listing.
 */

char    *e_greeting       = "Mail. Type ? for help.\n";
char    *r_greeting       = "pO^TA. nABERITE ? DLQ PODSKAZKI.\n";

announce(pr)
{
	int vec[2], mdot;

	if (pr && value("quiet") == NOSTR)
		printf(ediag(e_greeting,r_greeting));
	mdot = newfileinfo();
	vec[0] = mdot;
	vec[1] = 0;
	dot = &message[mdot - 1];
	if (msgCount > 0 && !noheader) {
		inithdr++;
		headers(vec);
		inithdr = 0;
	}
}

/*
 * Announce information about the file we are editing.
 * Return a likely place to set dot.
 */
newfileinfo()
{
	register struct message *mp;
	register int u, n, mdot, d, s;
	char fname[BUFSIZ], zname[BUFSIZ], *ename;

	for (mp = &message[0]; mp < &message[msgCount]; mp++)
		if (mp->m_flag & MNEW)
			break;
	if (mp >= &message[msgCount])
		for (mp = &message[0]; mp < &message[msgCount]; mp++)
			if ((mp->m_flag & MREAD) == 0)
				break;
	if (mp < &message[msgCount])
		mdot = mp - &message[0] + 1;
	else
		mdot = 1;
	s = d = 0;
	for (mp = &message[0], n = 0, u = 0; mp < &message[msgCount]; mp++) {
		if (mp->m_flag & MNEW)
			n++;
		if ((mp->m_flag & MREAD) == 0)
			u++;
		if (mp->m_flag & MDELETED)
			d++;
		if (mp->m_flag & MSAVED)
			s++;
	}
	ename = mailname;
	if (getfold(fname) >= 0) {
		if (strncmp(fname, mailname, strlen(fname)) == 0) {
			sprintf(zname, "+%s", mailname + strlen(fname));
			ename = zname;
		}
	}
	printf("\"%s\": ", ename);
	if (msgCount == 0)
		printf(ediag("no messages","NET SOOB]ENIJ"));
	else if (msgCount == 1)
		printf(ediag("1 message","1 SOOB]ENIE"));
	else if (msgCount > 0)
		printf(ediag("%d messages","%d SOOB]ENIJ"), msgCount);
	if (n > 0)
		printf(ediag(" %d new",(n==1?" 1 NOWOE":" %d NOWYH")), n);
	if (u-n > 0)
		printf(ediag(" %d unread",(u==1?" 1 NEPRO^ITANNOE":" %d NEPRO^ITANNYH")), u);
	if (d > 0)
		printf(ediag(" %d deleted",(d==1?" 1 UDALENNOE":" %d UDALENNYH")), d);
	if (s > 0)
		printf(ediag(" %d saved",(s==1?" 1 ZAPISANNOE":" %d ZAPISANNYH")), s);
	if (readonly)
		printf(ediag(" [Read only]"," [tOLXKO ^TENIE]"));
	printf("\n");
	return(mdot);
}

strace() {}

/*
 * Load a file of user definitions.
 */
void
load(name)
	char *name;
{
	register FILE *in, *oldin;

	if ((in = fopen(name, "r")) == NULL)
		return;
	oldin = input;
	input = in;
	loading = 1;
	sourcing = 1;
	if (debug) fprintf(stderr, "Loading %s\n", name);
	commands();
	loading = 0;
	sourcing = 0;
	input = oldin;
	fclose(in);
}
