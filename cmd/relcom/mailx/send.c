#include "rcv.h"
#ifndef M_XENIX
#ifdef VMUNIX
#include <sys/wait.h>
#else
#ifndef MSDOS
/* #include <wait.h>   */
#else
#ifndef __TURBOC__
#define freemem _dos_freemem
#define allocmem _dos_allocmem
#define ALLOCOK 0
#else
#define ALLOCOK (-1)
#endif
#endif  /*MSDOS*/
#endif  /*VMUNIX*/
#endif  /*M_XENIX*/
#include <sys/stat.h>

/*
 * Mail -- a mail program
 *
 * Mail to others.
 *
 * $Log: send.c,v $
 * Revision 1.36  1991/07/22  16:36:47  ache
 * Port to Borland C
 *
 * Revision 1.35  1991/07/19  20:00:14  ache
 * dOBAWLENA RASSYLKA SOOB]ENIJ W TELEKONFERENCII
 *
 * Revision 1.34  1991/07/17  19:55:29  ache
 * wait.h non needed in XENIX
 *
 * Revision 1.33  1991/04/28  14:29:43  ache
 * asa damaged MSDOS
 *
 * Revision 1.32  1991/04/19  22:49:53  asa
 * iZMENENIQ DLQ dEMOS 32
 *
 * Revision 1.31  1991/02/07  01:44:26  ache
 * dLQ dOS DOBAWLENA PROWERKA, DOSTATO^NO LI PAMQTI
 * DLQ ZAPUSKA sendmail
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
 * Revision 1.27  90/12/23  21:27:21  ache
 * bUFERIZACIQ IO PO 16 K
 * 
 * Revision 1.26  90/12/22  22:54:33  ache
 * sORTIROWKA + WYDA^A fio
 * 
 * Revision 1.25  90/12/07  07:15:31  ache
 * pEREDELANA OBRABOTKA WREMENNYH FAJLOW I MELO^I
 * 
 * Revision 1.24  90/12/03  03:01:33  ache
 * w DOSE: PEREHODIT W DOMA[NIJ KATALOG, PREVDE ^EM POZWATX sendmail
 * 
 * Revision 1.23  90/11/07  17:57:38  ache
 * pRI resent NE NADO signature
 * 
 * Revision 1.22  90/11/02  20:02:47  ache
 * Splitting long argument list.
 * 
 * Revision 1.21  90/10/16  09:13:38  ache
 * ~UTX USKORENA OBRABOTKA >From
 * 
 * Revision 1.20  90/10/13  20:29:01  ache
 * handling 'From '
 *
 * Revision 1.19  90/10/04  03:38:41  ache
 * Now 48 -- magic number form DOS sendmail -- remote spooled
 * 
 * Revision 1.18  90/09/29  18:22:46  ache
 * <ctype.h> kicked out...
 * 
 * Revision 1.17  90/09/26  22:15:52  ache
 * Signature added like in INEWS
 * 
 * Revision 1.16  90/09/21  22:00:25  ache
 * MS-DOS extends + some new stuff
 * 
 * Revision 1.15  90/09/13  13:20:31  ache
 * MS-DOS & Unix together...
 * 
 * Revision 1.14  90/09/08  13:39:53  avg
 * set record IGNORIRUETSQ ESLI WWOD NE S TERMINALA.
 * 
 * Revision 1.13  90/08/16  18:52:13  avg
 * *** empty log message ***
 * 
 * Revision 1.12  90/08/16  17:37:25  avg
 * w built-in pager DOBAWLENA KOMANDA d.
 * 
 * Revision 1.11  90/08/16  17:32:15  avg
 * dOBAWLENA KOMANDA forward.
 * 
 * Revision 1.10  90/08/15  19:45:19  avg
 * wDELAN WSTROENNYJ pager.
 *
 * Revision 1.9  90/08/10  13:47:23  avg
 * dOBAWLEN KL@^ -x DLQ PEREDA^I BOLX[IH FAJLOW.
 * 
 * Revision 1.8  90/08/08  17:14:40  avg
 * SENDMAIL->NETMAIL
 * 
 * Revision 1.7  90/05/31  23:19:43  avg
 * iSPRAWLENA O[IBKA - POTERQLSQ KONEC STROKI PRI PEREHODE OT
 * ctime K udate.
 *
 * Revision 1.6  90/05/31  19:49:40  avg
 * nOWAQ WERSIQ gethostname DLQ XENIXA.
 * 
 * Revision 1.5  90/05/18  14:22:48  avg
 * dOBAWLENA WOZMOVNOSTX UBIRATX PE^ATX STROKI 'From ...'
 * PRI POMO]I KOMANDY 'dis -from'.
 * 
 * Revision 1.4  90/04/30  19:44:44  avg
 * pO^INEN REVIM rmail
 *
 * Revision 1.3  88/07/23  20:38:04  ache
 * rUSSKIE DIAGNOSTIKI
 * 
 * Revision 1.2  88/01/11  12:33:30  avg
 * pOPRAWLENA ZAPISX W FAJL ISHODQ]EJ PO^TY, DATA WSEGDA TEPERX
 * ZAPISYWAETSQ W ANGL. FORMATE, DOBAWLENY NOXSTR U rcsid.
 * 
 * Revision 1.1  87/12/25  16:00:39  avg
 * Initial revision
 *
 */

#ifdef  NETMAIL
extern char sendprog[];
#endif
#ifdef	MSDOS
#include	<dos.h>
#include	<process.h>
#include	<errno.h>
extern SpoolRemote;
#define SKBSIZE 60  /* Memory(KB) needed for sendmail.exe */
#endif
void fmt();
static FILE *infix();

/*
 * Send message described by the passed pointer to the
 * passed output buffer.  Return -1 on error, but normally
 * the number of lines written.  Adjust the status: field
 * if need be.  If SF_DOIGN is set, suppress ignored header fields.
 * If SF_PAGER is set, emulate a MORE with 20 lines per screeen.
 */
long
send(mailp, obuf, sflags)
	struct message *mailp;
	FILE *obuf;
{
	register struct message *mp;
	register int t;
	long c;
	FILE *ibuf;
	char line[LINESIZE], field[BUFSIZ];
	long lc;
	int ishead, infld, fline, dostat, linecnt;
	char *cp, *cp2;
	int cc, fc;
	extern crt_lines;

	linecnt = (value("quiet") == NOSTR);    /* Message %d: */
again:  mp = mailp;
	ibuf = setinput(mp);
	c = mp->m_size;
	ishead = 1;
	dostat = 1;
	linecnt--;      /* because of Status: field */
	infld = 0;
	fline = 1;
	lc = 0;
	while (c > 0) {
		if (fgets(line, sizeof(line), ibuf) == NOSTR)
			return(-1L);
		c -= strlen(line);
#ifdef  MSDOS
		c--;
#endif
		lc++;
		if (ishead) {
			/*
			 * First line is the From line, so no headers
			 * there to worry about
			 */
			if (fline) {
				/* fline = 0; do it below... */
				/* Skip the first From line */
				if((sflags & SF_DOIGN) && isign("-from")) {
					fline = 0;
					continue;
				}
				goto writeit;
			}
			/*
			 * If line is blank, we've reached end of
			 * headers, so force out status: field
			 * and note that we are no longer in header
			 * fields
			 */
			if (line[0] == '\n') {
				if (dostat) {
					statusput(mailp, obuf, sflags & SF_DOIGN);
					dostat = 0;
				}
				ishead = 0;
				goto writeit;
			}
			/*
			 * If this line is a continuation (via space or tab)
			 * of a previous header field, just echo it
			 * (unless the field should be ignored).
			 */
			if (infld && isspace(line[0])) {
				if ((sflags & SF_DOIGN) && isign(field)) continue;
				goto writeit;
			}
			infld = 0;
			/*
			 * If we are no longer looking at real
			 * header lines, force out status:
			 * This happens in uucp style mail where
			 * there are no headers at all.
			 */
			if (!headerp(line)) {
				if (dostat) {
					statusput(mailp, obuf, sflags & SF_DOIGN);
					dostat = 0;
				}
				putc('\n', obuf);
				ishead = 0;
				goto writeit;
			}
			infld++;

			/*
			 * Pick up the header field.
			 * If it is an ignored field and
			 * we care about such things, skip it.
			 */
			cp = line;
			cp2 = field;
			while (*cp && *cp != ':' && !isspace(*cp))
				*cp2++ = *cp++;
			*cp2 = 0;
			if ((sflags & SF_DOIGN) && isign(field))
				continue;

			/*
			 * If the field is "status," go compute and print the
			 * real Status: field
			 */
			if (icequal(field, "status")) {
				if (dostat) {
					linecnt += statusput(mailp, obuf, (sflags & SF_DOIGN));
					dostat = 0;
				}
				continue;
			}
		}
writeit:
		/*
		 * Built-in pager
		 */
		if( (sflags & SF_PAGER) && ++linecnt >= crt_lines-1 ) {
			linecnt = 0;
promptagain:            fputs(ediag("?More? ", "?dALX[E? "), obuf);
			fflush(obuf);
			if ((cc = getchar()) != '\n')
				while( (fc = getchar()) != EOF && fc != '\n' )
					;
			if( cc == 'q' || cc == 'Q' || cc == EOF )
				goto flushall;
			if( cc == 'g' || cc == 'G' ) {
				if (value("quiet") == NOSTR) {
					fprintf(obuf, ediag("Message %2d:\n",
							    "sOOB]ENIE %2d:\n"),
					     mp - &message[0] + 1);
					linecnt = 1;
				}
				goto again;
			}
			if( cc == 'd' || cc == 'D' )
				linecnt += crt_lines/2;
			if( cc == '?' ) {
				fputs(ediag("\n\tCR or Enter  - show next screen.\n",
					    "\n\twk ILI Enter - POKAZATX SLEDU@]IJ \KRAN.\n"), obuf);
				fputs(ediag("\tg            - show the message from the beginning.\n",
					    "\tg            - POKAZATX SOOB]ENIE SNA^ALA.\n"), obuf);
				fputs(ediag("\td            - show next half-screen.\n",
					    "\td            - POKAZATX SLEDU@[IE POL-\KRANA.\n"), obuf);
				fputs(ediag("\tq            - quit printing message.\n",
						"\tq            - ZAKON^ITX PROSMOTR SOOB]ENIQ.\n"), obuf);
				fputs(ediag("\t?            - prints this help.\n\n",
					    "\t?            - WYDAET \TU PODSKAZKU.\n\n"), obuf);
				goto promptagain;
			}
		}
		if (!fline && *line == 'F' && strncmp(line, "From ", 5) == 0)
			putc('>', obuf);
		fline = 0;
		fputs(line, obuf);
		if (ferror(obuf))
			return(-1L);
	}
flushall:
	(void) fflush(obuf);
	if (ferror(obuf))
		return(-1L);
	if (ishead && (mailp->m_flag & MSTATUS))
		printf(ediag(
"failed to fix up status field\n",
"NEUDA^A S OBRABOTKOJ POLQ status\n"));
	return(lc);
}

/*
 * Test if the passed line is a header line, RFC 733 style.
 */
headerp(line)
	register char *line;
{
	register char *cp = line;

	while (*cp && !isspace(*cp) && *cp != ':')
		cp++;
	while (*cp && isspace(*cp))
		cp++;
	return(*cp == ':');
}

/*
 * Output a reasonable looking status field.
 * But if "status" is ignored and doign, forget it.
 */
statusput(mp, obuf, doign)
	register struct message *mp;
	register FILE *obuf;
{
	char statout[3];

	if (doign && isign("status"))
		return 0;
	if ((mp->m_flag & (MNEW|MREAD)) == MNEW)
		return 0;
	if (mp->m_flag & MREAD)
		strcpy(statout, "R");
	else
		strcpy(statout, "");
	if ((mp->m_flag & MNEW) == 0)
		strcat(statout, "O");
	fprintf(obuf, "Status: %s\n", statout);
	return 1;
}


/*
 * Interface between the argument list and the mail1 routine
 * which does all the dirty work.
 */

mail(people)
	char **people;
{
	register char *cp2;
	register int s;
	char *buf, **ap;
	struct header head;

	for (s = 0, ap = people; *ap != (char *) -1; ap++)
		s += strlen(*ap) + 1;
	buf = salloc(s+1);
	cp2 = buf;
	for (ap = people; *ap != (char *) -1; ap++) {
		cp2 = copy(*ap, cp2);
		*cp2++ = ' ';
	}
	if (cp2 != buf)
		cp2--;
	*cp2 = '\0';
	head.h_to = buf;
	if (sflag != NOSTR)
		head.h_subject = savestr(sflag);
	else
		head.h_subject = NOSTR;
	head.h_cc = NOSTR;
	head.h_bcc = NOSTR;
	head.h_refs = NOSTR;
	head.h_newsgroups = NOSTR;
	head.h_distribution = NOSTR;
	head.h_keywords = NOSTR;
	head.h_summary = NOSTR;
	head.h_seq = 0;
	head.h_resent = 0;
	mail1(&head);
	return(0);
}


/*
 * Send mail to a bunch of user names.  The interface is through
 * the mail1 routine below.
 */

sendmail(str)
	char *str;
{
	struct header head;

	if (blankline(str))
		head.h_to = NOSTR;
	else
		head.h_to = str;
	head.h_subject = NOSTR;
	head.h_cc = NOSTR;
	head.h_bcc = NOSTR;
	head.h_refs = NOSTR;
	head.h_newsgroups = NOSTR;
	head.h_distribution = NOSTR;
	head.h_keywords = NOSTR;
	head.h_summary = NOSTR;
	head.h_seq = 0;
	head.h_resent = 0;
	mail1(&head);
	return(0);
}

/*
 * Forward the current message
 * to the given list of addressees.
 */
Forward(str)
	char *str;
{
	struct header head;

	if( blankline(str) ) {
		printf(ediag("No recipients specified.\n",
			     "nE UKAZANY ADRESATY.\n"));
		return 1;
	}
	head.h_to = str;
	head.h_subject = NOSTR;
	head.h_cc = NOSTR;
	head.h_bcc = NOSTR;
	head.h_refs = NOSTR;
	head.h_newsgroups = NOSTR;
	head.h_distribution = NOSTR;
	head.h_keywords = NOSTR;
	head.h_summary = NOSTR;
	head.h_seq = 0;
	head.h_resent = 1;
	mail1(&head);
	return(0);
}

/*
 * Collect the forwarded message and put it into the temp. file
 */
FILE *fcollect(hp)
{
	extern char tempMail[];
	FILE *buf, *inp;
	register struct message *mp;
	char line[LINESIZE];
	int msg, firstl, inhdr, ignfield, t;
	long bcnt;
	char c;

	/*
	 * Scan for applicable message
	 */
	msg = first(0, MMNDEL);
	if( msg == (int)NULL ) {
		printf(ediag("No applicable messages\n",
			     "nET PODHODQ]IH SOOB]ENIJ\n"));
		return NULL;
	}
	mp = &message[msg-1];

	/*
	 * Create temp. file
	 */
	buf = NULL;
#ifdef  MSDOS
	maketemp(tempMail);
#endif
	if (   fclear(tempMail) < 0
		|| (buf = TmpOpen(tempMail, "a+")) == NULL
	   ) {
		perror(tempMail);
		remove(tempMail);
		return NULL;
	}

	/*
	 * Copy the message into temp. file
	 */
	bcnt = mp->m_size;
	inp = setinput(mp);
	firstl = 1;
	inhdr = 1;
	ignfield = 0;
	while (bcnt > 0) {
		if (fgets(line, sizeof(line), inp) == NOSTR)
			goto err;
		bcnt -= strlen(line);
#ifdef  MSDOS
		bcnt--;
#endif
		if( firstl ) {  /* Ugly From header line */
			firstl = 0;
			continue;
		}
		if( inhdr ) {
			if( line[0] == '\n' )
				inhdr = 0;
			else if( isspace(line[0]) ) {
				if( ignfield )
					continue;
			} else {
				ignfield = 0;
				c = line[7];
				line[7] = '\0';
				if( icequal(line, "resent-") ||
				    icequal(line, "status:") ) {
					ignfield++;
					continue;
				}
				line[7] = c;
			}
		}
		if (*line == 'F' && strncmp("From ", line, 5) == 0)
			putc('>', buf);
		fputs(line, buf);
		if (ferror(buf))
			goto err;
	}
	(void) fflush(buf);
	if (ferror(buf))
		goto err;
	rewind(buf);

	return buf;
err:
	TmpDel(buf);

	return NULL;
}

/*
 * Mail a message on standard input to the people indicated
 * in the passed header.  (Internal interface).
 */

mail1(hp)
	struct header *hp;
{
	register char *cp;
	int pid, i, s, p, gotcha;
	char **namelist, *deliver;
	struct name *to, *np;
	struct stat sbuf;
	FILE *mtf, *postage, *fbuf;
	int remote = (rflag != NOSTR) || rmail;
	char **t;
	extern int filetransfer;
	char linebuf[LINESIZE];
#ifdef	MSDOS
	unsigned segbuf;
#endif

	/*
	 * Collect user's mail from standard input.
	 * Get the result as mtf.
	 */

	pid = -1;
	if ((mtf = hp->h_resent ? fcollect(hp) : collect(hp)) == NULL)
		return(-1);
	hp->h_seq = 1;
	if (intty && outtty && !hp->h_resent) {
		printf(ediag("EOT\n","konec pereda~i\n"));
		flush();
	}

	/*
	 *  Copy out signature file
	 */

	if (   !hp->h_resent
	    && value("autosign") != NOSTR
	    && (fbuf = fopen(signature, "r")) != NULL
	   ) {
		fseek(mtf, 0L, 2);
		fputs("-- \n", mtf);
		for (i = 0; i < 4; i++)
			if (fgets(linebuf, sizeof(linebuf), fbuf) == NOSTR)
				break;
			else {
				if (*linebuf == 'F' && strncmp("From ", linebuf, 5) == 0)
					putc('>', mtf);
				fputs(linebuf, mtf);
			}
		fclose(fbuf);
	}

	/*
	 * Now, take the user names from the combined
	 * to and cc lists and do all the alias
	 * processing.
	 */

	senderr = 0;
	to = usermap(cat(extract(hp->h_bcc, GBCC),
	    cat(extract(hp->h_to, GTO), extract(hp->h_cc, GCC))));
	if (to == NIL) {
		printf(ediag(
"No recipients specified\n",
"nE UKAZANY POLU^ATELI\n"));
		goto topdog;
	}

	/*
	 * Look through the recipient list for names with /'s
	 * in them which we write to as files directly.
	 */

	to = outof(to, mtf, hp);
	to = verify(to);
	if (senderr && !remote) {
topdog:
		if (fsize(mtf) != 0) {
			remove(deadletter);
			rewind(mtf);
			exwrite(deadletter, mtf, 1);
		}
	}
	rewind(mtf);
	for (gotcha = 0, np = to; np != NIL; np = np->n_flink)
		if ((np->n_type & GDEL) == 0) {
			gotcha++;
			break;
		}
	to = elide(to);
	mechk(to);

	if (count(to) > 1)
		hp->h_seq++;
	if (hp->h_seq > 0 && !remote) {
		fixhead(hp, to);
		if (fsize(mtf) == 0)
		    if (hp->h_subject == NOSTR)
			printf(ediag(
"No message, no subject; hope that's ok\n",
"nI SOOB]ENIQ, NI TEMY; NAWERNOE TAK I NADO\n"));
		    else
			printf(ediag(
"Null message body; hope that's ok\n",
"pUSTOE SOOB]ENIE; NAWERNOE TAK I NADO\n"));
		if ((mtf = infix(hp, mtf)) == NULL) {
			fprintf(stderr, ediag(
". . . message lost, sorry.\n",
". . . SOOB]ENIE PROPALO, IZWINITE.\n"));
			return(-1);
		}
	}

	if ((cp = value("record")) != NOSTR && !filetransfer && intty)
		savemail(expand(cp), hp, mtf);

	if (!gotcha)
		goto out;

	while ((namelist = unpack(&to)) != (char **) NULL) {
		if (debug) {
			printf(ediag(
"Recipients of message:\n",
"pOLU^ATELI SOOB]ENIQ:\n"));
			for (t = namelist; *t != NOSTR; t++)
				printf(" \"%s\"", *t);
			printf("\n");
			flush();
			continue;
		}
#ifndef MSDOS
	/*
	 * Wait, to absorb a potential zombie, then
	 * fork, set up the temporary mail file as standard
	 * input for "mail" and exec with the user list we generated
	 * far above. Return the process id to caller in case he
	 * wants to await the completion of mail.
	 */

#ifdef VMUNIX
#ifdef  pdp11
		while (wait2(&s, WNOHANG) > 0)
#endif
#if defined(vax) || defined(sun)
		while (wait3(&s, WNOHANG, 0) > 0)
#endif
		;
#else
		wait(&s);
#endif
		rewind(mtf);
		pid = fork();
		if (pid == -1) {
			perror("fork");
			remove(deadletter);
			exwrite(deadletter, mtf, 1);
			goto out;
		}
		if (pid == 0) {
#ifdef VMUNIX
			if (remote == 0) {
				sigset(SIGTSTP, SIG_IGN);
				sigset(SIGTTIN, SIG_IGN);
				sigset(SIGTTOU, SIG_IGN);
			}
#endif
			signal(SIGHUP, SIG_IGN);
			signal(SIGINT, SIG_IGN);
			signal(SIGQUIT, SIG_IGN);
			if (!stat(POSTAGE, &sbuf))
				if ((postage = fopen(POSTAGE, "a")) != NULL) {
					fprintf(postage, "%s %d %d\n", myname,
						count(to), fsize(mtf));
					fclose(postage);
				}
			s = fileno(mtf);
			for (i = 3; i < 15; i++)
				if (i != s)
					close(i);
			close(0);
			dup(s);
			close(s);
#ifdef CC
			submit(getpid());
#endif
#ifdef NETMAIL
			if ((deliver = value("sendmail")) == NOSTR)
				deliver = sendprog;
			execv(deliver, namelist);
#endif
			execv(MAIL, namelist);
			perror(MAIL);
			exit(1);
		}
#else   /* MSDOS */
#ifdef NETMAIL
		if ((deliver = value("sendmail")) == NOSTR)
			deliver = sendprog;
#endif
		PushDir(value("HOME"));
		rewind(mtf);
		p = dup (fileno (stdin));
		close (fileno (stdin));
		dup (fileno (mtf));
		if (	allocmem((unsigned)((SKBSIZE * 1024L) >> 4), &segbuf) != ALLOCOK
			 || freemem(segbuf)
		   )
			goto NotStarted;
		s = spawnvp (P_WAIT, deliver, namelist);
		PopDir();
		if (s < 0) {
			int nomem;
		NotStarted:
			nomem = (errno == ENOMEM);
			perror(deliver);
			dup2 (p, fileno (stdin));
			if (nomem)
				fprintf(stderr, ediag(
					"I need %dKb of free memory to run SENDMAIL\n",
					"nUVNO %dkB SWOBODNOJ PAMQTI, ^TOBY ZAPUSTITX SENDMAIL\n"),
					SKBSIZE);
			remove(deadletter);
			exwrite(deadletter, mtf, 1);
			goto out;
		}
		dup2 (p, fileno (stdin));
		if (s == 48) {	/* MAGIC number -- remote spooled */
			SpoolRemote++;
			s = 0;
		}
		pid = 1;
		if (!stat(POSTAGE, &sbuf))
			if ((postage = fopen(POSTAGE, "a")) != NULL) {
				fprintf(postage, "%s %d %d\n", myname,
					count(to), fsize(mtf));
				fclose(postage);
			}
#endif  /* MSDOS */

out:
		if (remote || (value("verbose") != NOSTR)) {
#ifndef MSDOS
			while ((p = wait(&s)) != pid && p != -1)
				;
#endif
			if (s != 0)
				senderr++;
			pid = 0;
		}
	}   /* End While Args */

	TmpDel(mtf);

	return(pid);
}

/*
 * Fix the header by glopping all of the expanded names from
 * the distribution list into the appropriate fields.
 * If there are any ARPA net recipients in the message,
 * we must insert commas, alas.
 */

fixhead(hp, tolist)
	struct header *hp;
	struct name *tolist;
{
	register struct name *nlist;
	register int f;
	register struct name *np;

	for (f = 0, np = tolist; np != NIL; np = np->n_flink)
		if (any('@', np->n_name)) {
			f |= GCOMMA;
			break;
		}

	if (debug && (f & GCOMMA))
		fprintf(stderr, ediag(
"Should be inserting commas in recip lists\n",
"dOLVNY BYTX WSTAWLENY ZAPQTYE W SPISOK POLU^ATELEJ\n"));
	hp->h_to = detract(tolist, GTO|f);
	hp->h_cc = detract(tolist, GCC|f);
}

/*
 * Prepend a header in front of the collected stuff
 * and return the new file.
 */

static
FILE *
infix(hp, fi)
	struct header *hp;
	FILE *fi;
{
	extern char tempMail[];
	register FILE *nf;
	register int c;

	rewind(fi);
#ifdef  MSDOS
	maketemp(tempMail);
#endif
	if (   fclear(tempMail) < 0
		|| (nf = TmpOpen(tempMail, "a+")) == NULL
	   ) {
		perror(tempMail);
		remove(tempMail);
		TmpDel(fi);
		return NULL;
	}
	puthead(hp, nf, GMASK|GNL);
	while ((c = getc(fi)) != EOF)
		putc(c, nf);
	if (ferror(fi)) {
		ioerror("temp", 0);
		TmpDel(nf);
		TmpDel(fi);
		return NULL;
	}
	fflush(nf);
	if (ferror(nf)) {
		ioerror(tempMail, 1);
		TmpDel(nf);
		TmpDel(fi);
		return NULL;
	}
	TmpDel(fi);
	rewind(nf);

	return(nf);
}

/*
 * Dump the to, subject, cc header on the
 * passed file buffer.
 */

puthead(hp, fo, w)
	struct header *hp;
	FILE *fo;
{
	register int gotcha;

	gotcha = 0;
	if (hp->h_to != NOSTR && (w & GTO))
		fmt(hp->h_resent?"Resent-To: ": "To: ", hp->h_to, fo), gotcha++;
	if (hp->h_newsgroups != NOSTR && (w & GNGR))
		fmt("Newsgroups: ", hp->h_newsgroups, fo), gotcha++;
	if (hp->h_cc != NOSTR && (w & GCC))
		fmt(hp->h_resent?"Resent-Cc: ": "Cc: ", hp->h_cc, fo), gotcha++;
	if (hp->h_bcc != NOSTR && (w & GBCC))
		fmt(hp->h_resent?"Resent-Bcc: ": "Bcc: ", hp->h_bcc, fo), gotcha++;
	if (hp->h_refs != NOSTR && (w & GREFS) && !hp->h_resent)
		fmt("References: ", hp->h_refs, fo), gotcha++;
	if ( w & GADD )
		gotcha += putaddhlines(fo, hp->h_resent);
	if (hp->h_subject != NOSTR && (w & GSUBJECT))
		fprintf(fo, "Subject: %s\n", hp->h_subject), gotcha++;
	if (hp->h_keywords != NOSTR && (w & GNEWS))
		fmt("Keywords: ", hp->h_keywords, fo), gotcha++;
	if (hp->h_summary != NOSTR && (w & GNEWS))
		fmt("Summary: ", hp->h_summary, fo), gotcha++;
	if (hp->h_distribution != NOSTR && (w & GNEWS))
		fmt("Distribution: ", hp->h_distribution, fo), gotcha++;
	if (gotcha && (w & GNL) && !hp->h_resent)
		putc('\n', fo);
	return(0);
}

/*
 * Format the given text to not exceed 72 characters.
 */
void
fmt(str, txt, fo)
	register char *str, *txt;
	register FILE *fo;
{
	register int col;
	register char *bg, *bl, *pt, ch;

	col = strlen(str);
	if (col)
		fprintf(fo, "%s", str);
	pt = bg = txt;
	bl = 0;
	while (*bg) {
		pt++;
		if (++col >72) {
			if (!bl) {
				bl = bg;
				while (*bl && !isspace(*bl))
					bl++;
			}
			if (!*bl)
				goto finish;
			ch = *bl;
			*bl = '\0';
			fprintf(fo, "%s\n    ", bg);
			col = 4;
			*bl = ch;
			pt = bg = ++bl;
			bl = 0;
		}
		if (!*pt) {
finish:
			fprintf(fo, "%s\n", bg);
			return;
		}
		if (isspace(*pt))
			bl = pt;
	}
}

/*
 * Save the outgoing mail on the passed file.
 */

savemail(name, hp, fi)
	char name[];
	struct header *hp;
	FILE *fi;
{
	register FILE *fo;
	char *n, line[LINESIZE];
	extern char *udate();

	if ((fo = fopen(name, "a")) == NULL) {
		perror(name);
		return(-1);
	}
	n = rflag;
	if (n == NOSTR)
		n = myname;
	fprintf(fo, "From %s %s\n", n, udate());
	rewind(fi);
	while (fgets(line, sizeof(line), fi) != NOSTR) {
		if (*line == 'F' && strncmp("From ", line, 5) == 0)
			putc('>', fo);
		fputs(line, fo);
		if (ferror(fo))
			goto err;
	}
	putc('\n', fo);
	(void) fflush(fo);
	if (ferror(fo)) {
	err:
		ioerror(name, 1);
		fclose(fo);
		return(-1);
	}
	fclose(fo);
	return(0);
}
