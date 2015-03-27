/*
 * Mail -- a mail program
 *
 * Collect input from standard input, handling
 * ~ escapes.
 *
 * $Log: collect.c,v $
 * Revision 1.33  1991/07/22  16:36:47  ache
 * Port to Borland C
 *
 * Revision 1.32  1991/07/19  20:00:14  ache
 * dOBAWLENA RASSYLKA SOOB]ENIJ W TELEKONFERENCII
 *
 * Revision 1.31  1991/02/03  15:24:56  ache
 * uWELI^EN RAZMER DLQ uucppath
 *
 * Revision 1.30  1991/01/25  18:50:28  ache
 * zAPROS Cc: PO askcc PERESTAWLEN IZ send.c W collect.c
 *
 * Revision 1.29  1991/01/25  18:04:45  ache
 * uBRANY STARYE (4.1) SIGNALY
 *
 * Revision 1.28  1991/01/19  15:38:23  ache
 * UBRANY BUFERA 16K, KAK NE OPRAWDAW[IE DOWERIQ NARODA
 *
 * Revision 1.27  90/12/23  21:52:20  ache
 * bUFERIZACIQ IO PO 16 K
 *
 * Revision 1.26  90/12/08  20:53:20  ache
 * dOBAWLENO Abort, ... or Send?
 * 
 * Revision 1.25  90/12/07  07:14:08  ache
 * pEREDELANA OBRABOTKA WREMENNYH FAJLOW I MELO^I
 * 
 * Revision 1.24  90/10/25  01:14:09  ache
 * setjmp PERESTAWLEN WY[E.
 *
 * Revision 1.23  90/10/16  09:12:56  ache
 * ~UTX USKORENA OBRABOTKA >From
 * 
 * Revision 1.22  90/10/13  22:53:36  ache
 * ~> escape corrected.
 *
 * Revision 1.21  90/10/13  20:23:44  ache
 * Deleting 'From '.
 * 
 * Revision 1.20  90/10/09  01:13:03  ache
 * dOBAWLENA PODSKAZKA.
 * 
 * Revision 1.19  90/09/26  22:16:23  ache
 * Signature added like in INEWS
 * 
 * Revision 1.18  90/09/21  21:58:16  ache
 * MS-DOS extends + some new stuff
 * 
 * Revision 1.17  90/09/13  13:18:41  ache
 * MS-DOS & Unix together...
 * 
 * Revision 1.16  90/09/08  13:39:13  avg
 * dOBAWLENA KOMANDA ~>.
 *
 * Revision 1.15  90/08/17  18:22:36  avg
 * dOBAWLENO UDALENIE .b - FAJLOW POSLE REDAKTIROWANIQ.
 *
 * Revision 1.14  90/07/15  20:14:23  avg
 * pRI OBRABOTKE remote from TEPERX @ W LOKALXNOJ ^ASTI IMENI
 * BUDET PREOBRAZOWYWATXSQ W %.
 * 
 * Revision 1.13  90/06/30  23:07:45  avg
 * pOMIMO WSEGO PRO^EGO, PRAWLENA F-Q transmit NA PREDMET
 * DOBAWLENIQ > PERED From (NIKOGDA NE POME[AET).
 *
 * Revision 1.12  90/06/30  22:45:43  avg
 * pO NASTOQTELXNYM PROSXBAM kOLI sAUHA SDELANO ~M KAK SINONIM ~f.
 * 
 * Revision 1.11  90/05/31  22:17:45  avg
 * dOBAWLENA OBRABOTKA CTRL/C PRI WWODE POLEJ ZAGOLOWKA.
 * 
 * Revision 1.10  90/05/31  19:46:39  avg
 * pRAWLEN NABOR WYDAWAEMYH POLEJ ZAGOLOWKOW.
 * 
 * Revision 1.9  90/04/30  19:44:12  avg
 * pO^INEN REVIM rmail
 * 
 * Revision 1.8  90/04/20  19:16:39  avg
 * pRIKRU^ENO POD System V
 *
 * Revision 1.7  88/08/23  16:37:18  avg
 * sDELAN DWUQZY^NYJ help I PRAWLENY RUSSKIE DIAGNOSTIKI.
 *
 * Revision 1.6  88/07/23  22:11:23  ache
 * o[IBKA F FORMATE
 *
 * Revision 1.5  88/07/23  20:31:45  ache
 * rUSSKIE DIAGNOSTIKI
 *
 * Revision 1.4  88/07/04  21:23:33  ache
 * pERED ~q (collrub) POSTAWLEN sighold
 *
 * Revision 1.3  88/07/04  21:02:12  ache
 * oBRABOTKA SIGQUIT I SNQTIE/WOSSTANOWLENIE hold PERED
 * grabh ESLI NE TIOCSTI
 *
 * Revision 1.2  88/01/11  12:31:34  avg
 * tEPERX tilde escapes NE RABOTA@T TOLXKO TOGDA, KOGDA WWOD PROISHODIT NE
 * S TERMINALA (A NE PRI NALI^II KL@^A -r, KAK BYLO).
 *
 * Revision 1.1  87/12/25  15:58:47  avg
 * Initial revision
 *
 */

/*NOXSTR
static char rcsid[] = "$Header: /usr/src/Relcom/relcom/mailx/RCS/collect.c,v 1.33 1991/07/22 16:36:47 ache Exp $";
YESXSTR*/

#include "rcv.h"
#include <sys/stat.h>
#ifdef  MSDOS
#include        <process.h>
#endif

#ifdef TIOCSETA /* Demos2 Only */
#undef TIOCSTI
#endif

/*
 * Read a message from standard output and return a read file to it
 * or NULL on error.
 */

/*
 * The following hokiness with global variables is so that on
 * receipt of an interrupt signal, the partial message can be salted
 * away on dead.letter.  The output file must be available to flush,
 * and the input to read.  Several open files could be saved all through
 * Mail if stdio allowed simultaneous read/write access.
 */

sigtype collrub (), intack ();
#ifndef MSDOS
sigtype collintsig(), collhupsig(), collquitsig();
#else
sigtype collintsig(), collquitsig();
#endif
# ifdef VMUNIX
sigtype collcont ();
# endif

static  sigtype  (*savesig)();           /* Previous SIGINT value */
#ifndef MSDOS
static  sigtype (*savehup)();           /* Previous SIGHUP value */
#endif
static  sigtype (*savequit)();          /* Previous SIGQUIT value */
# ifdef VMUNIX
static  sigtype (*savecont)();          /* Previous SIGCONT value */
# endif
static  int     hf;                     /* Ignore interrups */
	int     hadintr;                /* Have seen one SIGINT so far */

static  jmp_buf coljmp;                 /* To get back to work */
extern  char tmp[], helpt[], rhelpt[];
static  FILE *buf;

FILE *
collect(hp)
	struct header *hp;
{
	FILE *fbuf;
	long lc, cc;
	int escape, eof;
	register int c, t, i;
	char linebuf[LINESIZE], *cp, *p1;
	extern char tempMail[];
	int notify();

	if( debug && (rmail || rflag != NOSTR) )
		printf("collect: rmail=%d rflag=%s\n", rmail, (rflag==NOSTR)?"(null)":rflag);
	noreset++;
	buf = NULL;
	if (value("ignore") != NOSTR)
		hf = 1;
	else
		hf = 0;
	hadintr = 0;
	savesig = signal(SIGINT, SIG_IGN);
#ifndef MSDOS
	savehup = signal(SIGHUP, SIG_IGN);
#endif
	savequit = signal(SIGQUIT, SIG_IGN);
# ifdef VMUNIX
	savecont = sigset(SIGCONT, collcont);
# endif
#ifdef  MSDOS
	maketemp(tempMail);
#endif
	if (   fclear(tempMail) < 0
		|| (buf = TmpOpen(tempMail, "a+")) == NULL
	   ) {
		perror(tempMail);
		remove(tempMail);
		goto err;
	}

	/*
	 * If we are going to prompt for a subject,
	 * refrain from printing a newline after
	 * the headers (since some people mind).
	 */

	t = GTO|GSUBJECT|GCC|GNGR|GNL;
	if (hp->h_newsgroups != NOSTR)
		t &= ~(GTO|GCC);
	c = 0;
	if (intty && !rmail &&
	    hp->h_subject == NOSTR && value("ask"))
		t &= ~GNL, c++;
	if (hp->h_seq != 0) {
		puthead(hp, stdout, t);
		flush();
	}
	escape = ESCAPE;
	if ((cp = value("escape")) != NOSTR)
		escape = *cp;
	t = 1;
	if (setjmp(coljmp))     /* dOLVEN BYTX ZDESX DLQ WYHODA IZ grabh */
		c = t = 0;
	if (c) {
#ifndef TIOCSTI
		if (savesig != SIG_IGN)
			signal(SIGINT, hf ? intack : collintsig);
#ifndef MSDOS
		if (savehup != SIG_IGN)
			signal(SIGHUP, collhupsig);
#endif
		if (savequit != SIG_IGN)
			signal(SIGQUIT, collquitsig);
#endif
		i = GSUBJECT;
		if (hp->h_newsgroups != NOSTR)
			i |= GNEWS;
		grabh(hp, i);
#ifndef TIOCSTI
		signal(SIGINT, SIG_IGN);
#ifndef MSDOS
		signal(SIGHUP, SIG_IGN);
#endif
		signal(SIGQUIT, SIG_IGN);
# ifdef VMUNIX
		sigset(SIGCONT, collcont);
# endif
#endif
	}
	if (t && intty && outtty && value("quiet") == NOSTR) {
		printf(ediag("'%c?' -- help, ",
			     "'%c?' -- PODSKAZKA, "), escape);
		printf(ediag("%s at the begining of a line -- complete message\n",
			     "%s W NA^ALE STROKI -- ZAWER[ENIE SOOB]ENIQ\n"),
		       value("dot") != NOSTR ? "'.'" :
#ifdef  MSDOS
				"Ctrl+Z<Enter>"
#else
				"CTRL/D"
#endif
		);
		flush();
	}
	eof = 0;
AgainLabel:
	for (;;) {
		if (savesig != SIG_IGN)
			signal(SIGINT, hf ? intack : collintsig);
#ifndef MSDOS
		if (savehup != SIG_IGN)
			signal(SIGHUP, collhupsig);
#endif
		if (savequit != SIG_IGN)
			signal(SIGQUIT, collquitsig);
		flush();
		if (readline(stdin, linebuf) <= 0) {
			if (intty && value("ignoreeof") != NOSTR) {
				if (++eof > 35)
					break;
				printf(ediag(
"Use \".\" to terminate letter\n",
"iSPOLXZUJTE \".\" ^TOBY OKON^ITX PISXMO\n"));
				continue;
			}
			break;
		}
		eof = 0;
		hadintr = 0;
		if (intty && equal(".", linebuf) &&
			(value("dot") != NOSTR || value("ignoreeof") != NOSTR))
			break;

		/*
		 * Process old ugly From ... remote from ...
		 * uucp rmail header line.
		 */
		if( rmail && rflag == NOSTR && *linebuf == 'F' &&
		    strncmp(linebuf, "From ", 5) == 0) {
			register char *p, *q;
			static char remotename[LINESIZE];
#define SPLIT   400     /* must be < LINESIZE */

			p = &linebuf[5];
			q = &remotename[SPLIT];
			while( *p && !isspace(*p) )
				*q++ = *p++;
			*q = '\0';
			q = &remotename[SPLIT];
			while( *p && *p != '\n' ) {
				if( strncmp(p++, "remote from ", 12) )
					continue;
				while( *p ) p++;
				while( --p >= &linebuf[5] && isspace(*p) )
					;
				/* add an uucp part separated by ! */
				*--q = '!';     /* At SPLIT */
				while( p >= &linebuf[5] && !isspace(*p) )
					*--q = *p--;
				/* if there was an Internet address,
				   change all '@'s to '%'s (sigh) */
				for (p = &remotename[SPLIT] + 1; *p; p++)
					if (*p == '@')
						*p = '%';
				break;
			}
			rflag = q;
		}

		if (linebuf[0] != escape || !intty || rflag != NOSTR) {
			if (   fseek(buf, 0L, 2) != 0
				|| (t = putline(buf, linebuf)) < 0) {
				ioerror(tmp, 1);
				goto err;
			}
			continue;
		}
		c = linebuf[1];
		switch (c) {
		default:
			/*
			 * On double escape, just send the single one.
			 * Otherwise, it's an error.
			 */

			if (c == escape) {
				if (   fseek(buf, 0L, 2) != 0
					|| putline(buf, &linebuf[1]) < 0) {
					ioerror(tmp, 1);
					goto err;
				}
				else
					break;
			}
			printf(ediag(
"Unknown tilde escape.\n",
"nEIZWESTNAQ UPRAWLQ@]AQ POSLEDOWATELXNOSTX.\n"));
			break;

		case 'C':
			/*
			 * Dump core.
			 */

			core();
			break;

		case '!':
			/*
			 * Shell escape, send the balance of the
			 * line to sh -c.
			 */

			shell(&linebuf[2]);
			break;

		case ':':
		case '_':
			/*
			 * Escape to command mode, but be nice!
			 */
			execute(&linebuf[2], 1);
			printf(ediag("(continue)\n","(PRODOLVAJTE)\n"));
			break;

		case '.':
			/*
			 * Simulate end of file on input.
			 */
			goto eofl;

		case 'q':
		case 'Q':
AbortLabel:
			/*
			 * Force a quit of sending mail.
			 * Act like an interrupt happened.
			 */

			hadintr++;
			signal(SIGINT, SIG_IGN);
			collrub(SIGINT);
			exit(1);

		case 'h':
			/*
			 * Grab a bunch of headers.
			 */
			if (!outtty) {
				printf(ediag(
"~h: no can do!?\n",
"~h: NELXZQ SDELATX!\n"));
				break;
			}
			i = GTO|GSUBJECT|GCC|GBCC;
			if (hp->h_newsgroups != NOSTR) {
				i |= GNGR|GNEWS;
				i &= ~(GCC|GBCC|GTO);
			}
			grabh(hp, i);
			printf(ediag("(continue)\n","(PRODOLVAJTE)\n"));
			break;

		case 't':
			/*
			 * Add to the To list.
			 */

			hp->h_to = addto(hp->h_to, &linebuf[2]);
			hp->h_seq++;
			break;

		case 's':
			/*
			 * Set the Subject list.
			 */

			cp = &linebuf[2];
			while (any(*cp, " \t"))
				cp++;
			hp->h_subject = savestr(cp);
			hp->h_seq++;
			break;

		case 'c':
			/*
			 * Add to the CC list.
			 */

			hp->h_cc = addto(hp->h_cc, &linebuf[2]);
			hp->h_seq++;
			break;

		case 'b':
			/*
			 * Add stuff to blind carbon copies list.
			 */
			hp->h_bcc = addto(hp->h_bcc, &linebuf[2]);
			hp->h_seq++;
			break;

		case 'd':
			copy(deadletter, &linebuf[2]);
			/* fall into . . . */

		case 'r':
			/*
			 * Invoke a file:
			 * Search for the file name,
			 * then open it and copy the contents to obuf.
			 */

			cp = &linebuf[2];
			while (any(*cp, " \t"))
				cp++;
			if (*cp == '\0') {
				printf(ediag(
"Interpolate what file?\n",
"kAKOJ FAJL WSTAWLQTX?\n"));
				break;
			}
			cp = expand(cp);
			if (cp == NOSTR)
				break;
			if (isdir(cp)) {
				printf(ediag(
"%s: directory\n",
"%s: KATALOG\n"), cp);
				break;
			}
			if ((fbuf = fopen(cp, "r")) == NULL) {
				perror(cp);
				break;
			}
			printf("\"%s\" ", cp);
			flush();
			lc = 0;
			cc = 0;
			if (fseek(buf, 0L, 2) != 0) {
				ioerror(tmp, 2);
				fclose(fbuf);
				goto err;
			}
			while (readline(fbuf, linebuf) > 0) {
				lc++;
				if ((t = putline(buf, linebuf)) < 0) {
					ioerror(tmp, 1);
					fclose(fbuf);
					goto err;
				}
				cc += t;
			}
			fclose(fbuf);
			printf("%ld/%ld\n", lc, cc);
			break;

		case 'u':
			/*
			 * UUENCODE	a file:
			 * Search for the file name,
			 * then open it and copy the contents to obuf.
			 */

			cp = &linebuf[2];
			while (any(*cp, " \t"))
				cp++;
			if (*cp == '\0') {
				printf(ediag(
"Uuencode what file?\n",
"kAKOJ FAJL DLQ uuencode?\n"));
				break;
			}
			for(p1 = cp; *p1 && !any(*p1, " \t"); p1++)
				;
			while (*p1 && any(*p1, " \t"))
				*p1++ = '\0';
			if (*p1 == '\0')
				p1 = cp;
			cp = expand(cp);
			if (cp == NOSTR)
				break;
			if (isdir(cp)) {
				printf(ediag(
"%s: directory\n",
"%s: KATALOG\n"), cp);
				break;
			}
#ifndef MSDOS
			if ((fbuf = fopen(cp, "r")) == NULL) {
#else
			if ((fbuf = fopen(cp, "rb")) == NULL) {
#endif
				perror(cp);
				break;
			}
			printf("\"%s\" ", cp);
			flush();
			lc = 0;
			cc = 0;
			if (fseek(buf, 0L, 2) != 0) {
				ioerror(tmp, 2);
				fclose(fbuf);
				goto err;
			}
			if (!uuencode(p1, fbuf, buf, &lc, &cc)) {
				ioerror(tmp, 1);
				fclose(fbuf);
				goto err;
			}
			fclose(fbuf);
			printf("%ld/%ld\n", lc, cc);
			break;

		case 'w':
			/*
			 * Write the message on a file.
			 */

			cp = &linebuf[2];
			while (any(*cp, " \t"))
				cp++;
			if (*cp == '\0') {
				fprintf(stderr, ediag(
"Write what file!?\n",
"w KAKOJ FAJL PISATX?\n"));
				break;
			}
			if ((cp = expand(cp)) == NOSTR)
				break;
			rewind(buf);
			exwrite(cp, buf, 1);
			break;

		case 'm':
		case 'M':
		case 'f':
		case '>':
			/*
			 * Interpolate the named messages, if we
			 * are in receiving mail mode.  Does the
			 * standard list processing garbage.
			 * If ~f is given, we don't shift over.
			 */

			if (!rcvmode) {
				printf(ediag(
"No messages to send from!?!\n",
"oTKUDA WZQTX SOOB]ENIE?!\n"));
				break;
			}
			cp = &linebuf[2];
			while (any(*cp, " \t"))
				cp++;
			if (forward(cp, buf, c) < 0)
				goto err;
			printf(ediag("(continue)\n","(PRODOLVAJTE)\n"));
			break;

		case '?':
			if ((fbuf = fopen(ediag(helpt,rhelpt), "r")) == NULL) {
				perror(ediag(helpt,rhelpt));
				break;
			}
			while ((t = getc(fbuf)) != EOF)
				putchar(t);
			fclose(fbuf);
			break;

		case 'p':
ListLabel:
			/*
			 * Print out the current state of the
			 * message without altering anything.
			 */
			rewind(buf);
			printf(ediag(
"-------\nMessage contains:\n",
"-------\nsOOB]ENIE SODERVIT:\n"));
			puthead(hp, stdout, GMASK|GNL);
			while ((t = getc(buf)) != EOF)
				putchar(t);
			/*
			 *  Copy out signature file
			 */

			if (   value("autosign") != NOSTR
				&& (fbuf = fopen(signature, "r")) != NULL
			   ) {
				puts("-- ");
				for (t = 0; t < 4; t++)
					if (fgets(linebuf, sizeof(linebuf), fbuf) == NOSTR)
						break;
					else {
						if (*linebuf == 'F' && strncmp("From ", linebuf, 5) == 0)
							putchar('>');
						fputs(linebuf, stdout);
					}
				fclose(fbuf);
			}
			printf(ediag("\n(continue)\n","\n(PRODOLVAJTE)\n"));
			break;

		case '^':
		case '|':
			/*
			 * Pipe message through command.
			 * Collect output as new message.
			 */

			buf = mespipe(buf, &linebuf[2]);
			printf(ediag("(continue)\n","(PRODOLVAJTE)\n"));
			break;
EditLabel:
			c = 'v';
		case 'v':
		case 'e':
			/*
			 * Edit the current message.
			 * 'e' means to use EDITOR
			 * 'v' means to use VISUAL
			 */

			if ((buf = mesedit(buf, c)) == NULL)
				goto err;
			printf(ediag("(continue)\n","(PRODOLVAJTE)\n"));
			break;
		}
	}
eofl:
	if (intty && outtty && !rmail) {
		if (   hp->h_newsgroups == NOSTR
		    && hp->h_cc == NOSTR
		    && value("askcc") != NOSTR
		   )
			grabh(hp, GCC);
		if (value("quiet") == NOSTR) {
			char buf[80];
Ask:
			printf(ediag(
"Abort, Continue, Edit, List or Send? ",
"oTMENITX(a), PRODOLVITX(c), REDAKTIROWATX(e), PROSMOTRETX(l) ILI POSLATX(s)? "));
			flush();
			gets(buf);
			*buf = tolower(*buf);
			switch (*buf) {
			case 'a':
				goto AbortLabel;
			case 'c':
				printf(ediag("(continue)\n","(PRODOLVAJTE)\n"));
				goto AgainLabel;
			case 'e':
				goto EditLabel;
			case 'l':
				goto ListLabel;
			case 's':
			case '\0':
				break;
			default:
				goto Ask;
			}
		}
	}
	rewind(buf);
	signal(SIGINT, savesig);
#ifndef MSDOS
	signal(SIGHUP, savehup);
#endif
	signal(SIGQUIT, savequit);
# ifdef VMUNIX
	sigset(SIGCONT, savecont);
# endif
	noreset = 0;
	return(buf);

err:
	TmpDel(buf);
	signal(SIGINT, savesig);
#ifndef MSDOS
	signal(SIGHUP, savehup);
#endif
	signal(SIGQUIT, savequit);
# ifdef VMUNIX
	sigset(SIGCONT, savecont);
# endif
	noreset = 0;
	return(NULL);
}

/*
 * Write a file, ex-like if f set.
 */

exwrite(name, ibuf, f)
	char name[];
	FILE *ibuf;
{
	register FILE *of;
	long cc;
	long lc;
	char line[LINESIZE];
	struct stat junk;

	if (f) {
		printf("\"%s\" ", name);
		flush();
	}
	if (stat(name, &junk) >= 0 && (junk.st_mode & S_IFMT) == S_IFREG) {
		if (!f)
			fprintf(stderr, "%s: ", name);
		fprintf(stderr, ediag("file exists\n", "FAJL UVE SU]ESTWUET\n"));
		return(-1);
	}
	if ((of = fopen(name, "w")) == NULL) {
		perror(f ? NOSTR : name);
		return(-1);
	}
	lc = 0;
	cc = 0;
	while (fgets(line, sizeof(line), ibuf) != NOSTR) {
		lc++;
		cc += strlen(line);
		if (*line == 'F' && strncmp("From ", line, 5) == 0) {
			putc('>', of);
			cc++;
		}
		fputs(line, of);
		if (ferror(of)) {
	err:
			ioerror(f ? NOSTR : name, 1);
			fclose(of);
			return(-1);
		}
	}
	(void) fflush(of);
	if (ferror(of))
		goto err;
	fclose(of);
	printf("%ld/%ld\n", lc, cc);
	flush();
	return(0);
}

extern char tempEdit[];

/*
 * Edit the message being collected on ibuf and obuf.
 * Write the message out onto some poorly-named temp file
 * and point an editor at it.
 *
 * On return, make the edit file the new temp file.
 */

FILE *
mesedit(buf, c)
	FILE *buf;
{
	int pid, s;
	FILE *fbuf;
	register int t;
	sigtype (*sig)(), (*scont)(), signull(), (*qsig)();
	struct stat sbuf;
	register char *edit;
	char tmpback[100];      /* Editor's .bak file name */

	sig = signal(SIGINT, SIG_IGN);
	qsig = signal(SIGQUIT, SIG_IGN);
# ifdef VMUNIX
	scont = sigset(SIGCONT, signull);
# endif
#ifdef  MSDOS
	maketemp(tempEdit);
#endif
	if (stat(tempEdit, &sbuf) >= 0) {
		printf(ediag("%s: file exists\n",
"%s: FAJL UVE SU]ESTWUET\n"),
tempEdit);
		goto out;
	}
	if (   fclear(tempEdit) < 0
		|| (fbuf = fopen(tempEdit, "a")) == NULL
	   ) {
		perror(tempEdit);
		remove(tempEdit);
		goto out;
	}
	rewind(buf);
	while ((t = getc(buf)) != EOF)
		putc(t, fbuf);
	fflush(fbuf);
	if (ferror(fbuf)) {
		perror(tempEdit);
		remove(tempEdit);
		goto out;
	}
	fclose(fbuf);
	if ((edit = value(c == 'e' ? "EDITOR" : "VISUAL")) == NOSTR)
		edit = c == 'e' ? EDITOR : VISUAL;
#ifndef MSDOS
	pid = vfork();
	if (pid == 0) {
		if (sig != SIG_IGN)
			sigsys(SIGINT, SIG_DFL);
		if (qsig != SIG_IGN)
			sigsys(SIGQUIT, SIG_DFL);
		execlp(edit, edit, tempEdit, NULL);
		perror(edit);
		_exit(1);
	}
	if (pid == -1) {
		perror("fork");
#else
	pid = spawnlp (P_WAIT, edit, "mail-editor", tempEdit, NULL);
	if (pid < 0) {
		perror(edit);
#endif
		remove(tempEdit);
		goto out;
	}
#ifndef MSDOS
	while (wait(&s) != pid)
		;
	if ((s & 0377) != 0) {
#else
	if (pid != 0) {
#endif
		printf(ediag(
"Fatal error in \"%s\"\n",
"fATALXNAQ O[IBKA W \"%s\"\n"),
edit);
		remove(tempEdit);
		goto out;
	}

	/*
	 * Now switch to new file.
	 */

	if ((fbuf = TmpOpen(tempEdit, "a+")) == NULL) {
		perror(tempEdit);
		remove(tempEdit);
		goto out;
	}
	TmpDel(buf);
	buf = fbuf;
	if (fseek(buf, 0L, 2) != 0)
		ioerror(tmp, 2);
out:
	/* remove backfile */
#ifdef	MSDOS
	if ((edit = strrchr(tempEdit, '.')) != NOSTR)
		*edit = '\0';
#endif
	strcpy(tmpback, tempEdit);
#ifndef MSDOS
	strcat(tmpback, ".b");
#else
	strcat(tmpback, ".~");
#endif
	remove(tmpback);

	strcpy(tmpback, tempEdit);
	strcat(tmpback, ".bak");
	remove(tmpback);

# ifdef VMUNIX
	sigset(SIGCONT, scont);
# endif
	signal(SIGINT, sig);
	signal(SIGQUIT, qsig);
	return(buf);
}

/*
 * Pipe the message through the command.
 * Old message is on stdin of command;
 * New message collected from stdout.
 * Sh -c must return 0 to accept the new message.
 */

FILE *
mespipe(buf, cmd)
	FILE *buf;
	char cmd[];
{
	register FILE *n;
	int pid, s;
	sigtype (*savesig)(), (*saveqsig)();
	char *Shell;
#ifdef  MSDOS
	int oo, ii;
#endif
#ifdef  MSDOS
	maketemp(tempEdit);
#endif
	if (   fclear(tempEdit) < 0
		|| (n = TmpOpen(tempEdit, "a+")) == NULL
	   ) {
		perror(tempEdit);
		remove(tempEdit);
		return(buf);
	}
	rewind(buf);
	savesig = signal(SIGINT, SIG_IGN);
	saveqsig = signal(SIGQUIT, SIG_IGN);
	if ((Shell = value("SHELL")) == NULL)
#ifdef  MSDOS
	    if ((Shell = value("COMSPEC")) == NULL)
#endif
		Shell = SHELL;
#ifndef MSDOS
	if ((pid = vfork()) == -1) {
		perror("fork");
		goto err;
	}
	if (pid == 0) {
		/*
		 * stdin = current message.
		 * stdout = new message.
		 */

		close(0);
		dup(fileno(buf));
		close(1);
		dup(fileno(n));
		for (s = 4; s < 15; s++)
			close(s);
		execlp(Shell, Shell, "-c", cmd, NULL);
		perror(Shell);
		_exit(1);
	}
	while (wait(&s) != pid)
		;
	if (s != 0 || pid == -1) {
#else   /* MSDOS */
	/*
	 * stdin = current message.
	 * stdout = new message.
	 */

	ii = dup (fileno(stdin));
	close(fileno(stdin));
	dup(fileno(buf));
	oo = dup (fileno(stdout));
	close(fileno(stdout));
	dup(fileno(n));
	s = spawnlp (P_WAIT, Shell, "mail-pipe", "/C", cmd, NULL);
	if (s < 0) {
		dup2 (ii, fileno(stdin));
		dup2 (oo, fileno(stdout));
		perror (Shell);
		goto err;
	}
	dup2 (ii, fileno(stdin));
	dup2 (oo, fileno(stdout));
	if (s > 0) {
#endif  /* MSDOS */
		fprintf(stderr, ediag(
"\"%s\" failed!?\n",
"\"%s\" ZAWER[ILSQ NEUDA^EJ?\n"), cmd);
		goto err;
	}
	if (fsize(n) == 0) {
		fprintf(stderr, ediag(
"No bytes from \"%s\" !?\n",
"nI^EGO NET IZ \"%s\" !?\n"), cmd);
		goto err;
	}

	/*
	 * Take new files.
	 */
	TmpDel(buf);
	if (fseek(n, 0L, 2) != 0) {
		ioerror(tmp, 2);
		goto err;
	}

	signal(SIGINT, savesig);
	signal(SIGQUIT, saveqsig);

	return(n);

err:
	signal(SIGINT, savesig);
	signal(SIGQUIT, saveqsig);
	TmpDel(n);
	return(buf);
}

/*
 * Interpolate the named messages into the current
 * message, preceding each line with a tab.
 * Return a count of the number of characters now in
 * the message, or -1 if an error is encountered writing
 * the message temporary.  The flag argument is 'm' if we
 * should shift over and 'f' if not.
 */
forward(ms, obuf, f)
	char ms[];
	FILE *obuf;
{
	register int *msgvec, *ip;

	if ((msgvec = (int *) salloc((msgCount+1) * sizeof *msgvec)) == NULL) {
		panic("No memory\n");
		return(-1);
	}
	if (getmsglist(ms, msgvec, 0) < 0)
		return(0);
	if (*msgvec == (int)NULL) {
		*msgvec = first(0, MMNORM);
		if (*msgvec == (int)NULL) {
			printf(ediag(
"No appropriate messages\n",
"nET PODHODQ]IH SOOB]ENIJ\n"));
			return(0);
		}
		msgvec[1] = (int)NULL;
	}
	printf(ediag("Interpolating:","wSTAWKA SOOB]ENI%s:"),
					  (msgvec[1]==(int)NULL)?"Q":"J");
	if (fseek(obuf, 0L, 2) != 0) {
		ioerror(tmp, 2);
		return(-1);
	}
	for (ip = msgvec; *ip != (int)NULL; ip++) {
		touch(*ip);
		printf(" %d", *ip);
		if (transmit(&message[*ip-1], obuf, f) < 0) {
			ioerror(tmp, 1);
			return(-1);
		}
	}
	putchar('\n');
	return(0);
}

/*
 * Send message described by the passed pointer to the
 * passed output buffer.  Insert a tab in front of each
 * line.  Return a count of the characters sent, or -1
 * on error.
 */

long
transmit(mailp, obuf, f)
	struct message *mailp;
	FILE *obuf;
{
	register struct message *mp;
	register int ch;
	long c, n;
	int fcnt = 0;
	FILE *ibuf;
	char line[BUFSIZ];

	mp = mailp;
	ibuf = setinput(mp);
	c = mp->m_size;
	n = c;
	while( fgets(line, sizeof line, ibuf) != NOSTR ) {
		if (*line == 'F' && strncmp(line, "From ", 5) == 0) {
			if( fcnt++ )
				break;  /* Next message is started there */
			if( f != 'm' && f != '>' ) {
				putc('>', obuf);
				n++;
			}
		}
		if( f == 'm' ) {
			putc('\t', obuf);
			n++;
		} else if( f == '>' ) {
			putc('>', obuf);
			n++;
		}
		fputs(line, obuf);
		if (ferror(obuf)) {
			ioerror(tmp, 1);
			return(-1);
		}
	}
	(void) fflush(obuf);
	if (ferror(obuf)) {
		ioerror(tmp, 1);
		return(-1);
	}
	return(n);
}

#ifdef VMUNIX
/*
 * Print (continue) when continued after ^Z.
 */
collcont(s)
{

	printf(ediag("(continue)\n","(PRODOLVAJTE)\n"));
	flush();
}
#endif

/*
 * On interrupt, go here to save the partial
 * message on ~/dead.letter.
 * Then restore signals and execute the normal
 * signal routine.  We only come here if signals
 * were previously set anyway.
 */
sigtype collintsig()
{
	signal(SIGINT, SIG_IGN);
	collrub(SIGINT);
}
#ifndef MSDOS
sigtype collhupsig()
{
	signal(SIGHUP, SIG_IGN);
	collrub(SIGHUP);
}
#endif
sigtype collquitsig()
{
	signal(SIGQUIT, SIG_IGN);
	collrub(SIGQUIT);
}

sigtype collrub(s)
{
	register FILE *dbuf;
	register int c;

	if (s == SIGINT && hadintr == 0) {
		hadintr++;
		clrbuf(stdout);
#ifndef MSDOS
		putchar('\n');
#endif
		printf(ediag(
"(Interrupt -- one more to kill letter)\n",
"(pRERYWANIE -- ^TOBY UNI^TOVITX PISXMO NUVNO E]E ODNO)\n"));
		flush();
		longjmp(coljmp, 1);
	}
	if (s == SIGINT && value("nosave") != NOSTR || fsize(buf) == 0)
		goto done;
	if (   fclear(deadletter) < 0
		|| (dbuf = fopen(deadletter, "a")) == NULL
	   ) {
		remove(deadletter);
		goto done;
	}
	rewind(buf);
	while ((c = getc(buf)) != EOF)
		putc(c, dbuf);
	fclose(dbuf);

done:
	TmpDel(buf);
# ifdef VMUNIX
	sigset(SIGCONT, savecont);
# endif
	if (rcvmode) {
#ifndef MSDOS
		if (s == SIGHUP)
			hangup();
		else
#endif
			stop(s);
	}
	else
		exit(1);
}

/*
 * Acknowledge an interrupt signal from the tty by typing an @
 */

sigtype intack(s)
{

	puts("@");
	flush();
	clearerr(stdin);
}

/*
 * Add a string to the end of a header entry field.
 */

char *
addto(hf, news)
	char hf[], news[];
{
	register char *cp, *cp2, *linebuf;

	if (hf == NOSTR)
		hf = "";
	if (*news == '\0')
		return(hf);
	linebuf = salloc(strlen(hf) + strlen(news) + 2);
	for (cp = hf; any(*cp, " \t"); cp++)
		;
	for (cp2 = linebuf; *cp;)
		*cp2++ = *cp++;
	*cp2++ = ' ';
	for (cp = news; any(*cp, " \t"); cp++)
		;
	while (*cp != '\0')
		*cp2++ = *cp++;
	*cp2 = '\0';
	return(linebuf);
}
