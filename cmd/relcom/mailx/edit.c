#include "rcv.h"
#include <stdio.h>
#include <sys/stat.h>
#ifdef  MSDOS
#include        <process.h>
#endif

/*
 * Mail -- a mail program
 *
 * Perform message editing functions.
 *
 * $Log: edit.c,v $
 * Revision 1.14  1991/07/22  16:36:47  ache
 * Port to Borland C
 *
 * Revision 1.13  1991/01/25  18:04:45  ache
 * uBRANY STARYE (4.1) SIGNALY
 *
 * Revision 1.12  1991/01/19  15:38:23  ache
 * UBRANY BUFERA 16K, KAK NE OPRAWDAW[IE DOWERIQ NARODA
 *
 * Revision 1.11  90/12/23  21:12:25  ache
 * bUFERIZACIQ IO PO 16 k
 * 
 * Revision 1.10  90/12/22  22:54:12  ache
 * sORTIROWKA + WYDA^A fio
 * 
 * Revision 1.9  90/12/07  07:14:56  ache
 * pEREDELANA OBRABOTKA WREMENNYH FAJLOW I MELO^I
 * 
 * Revision 1.8  90/10/16  09:03:01  ache
 * wWEDENO AWTOISPRAWLENIE NEKORREKTNYH SOOB]ENIJ.
 * 
 * Revision 1.7  90/10/13  20:26:09  ache
 * handling From
 * 
 * Revision 1.6  90/09/21  21:58:34  ache
 * MS-DOS extends + some new stuff
 * 
 * Revision 1.5  90/09/13  13:19:01  ache
 * MS-DOS & Unix together...
 * 
 * Revision 1.4  90/08/17  18:22:11  avg
 * dOBAWLENO UDALENIE .b - FAJLOW POSLE REDAKTIROWANIQ.
 * 
 * Revision 1.3  88/07/23  20:32:36  ache
 * rUSSKIE DIAGNOSTIKI
 * 
 * Revision 1.2  88/01/11  12:41:10  avg
 * dOBAWLEN NOXSTR U rcsid.
 * 
 * Revision 1.1  87/12/25  15:59:11  avg
 * Initial revision
 * 
 */

/*NOXSTR
static char rcsid[] = "$Header: /usr/src/Relcom/relcom/mailx/RCS/edit.c,v 1.14 1991/07/22 16:36:47 ache Exp $";
YESXSTR*/
extern char *udate();
extern char tempMesg[];

/*
 * Edit a message list.
 */

editor(msgvec)
	int *msgvec;
{
	char *edname;

	if ((edname = value("EDITOR")) == NOSTR)
		edname = EDITOR;
	return(edit1(msgvec, edname));
}

/*
 * Invoke the visual editor on a message list.
 */

visual(msgvec)
	int *msgvec;
{
	char *edname;

	if ((edname = value("VISUAL")) == NOSTR)
		edname = VISUAL;
	return(edit1(msgvec, edname));
}

/*
 * Edit a message by writing the message into a funnily-named file
 * (which should not exist) and forking an editor on it.
 * We get the editor from the stuff above.
 */

edit1(msgvec, ed)
	int *msgvec;
	char *ed;
{
	register char *cp, *cp2;
	int *ip, pid, mesg, s, firstline, lastline;
	long ms, lines;
	sigtype (*sigint)(), (*sigquit)();
	FILE *ibuf, *obuf;
	char edname[15], nbuf[10];
	char backedname[20];
	char line[LINESIZE];
	struct message *mp;
	extern char tempEdit[];
	struct stat statb;
	long modtime;

	/*
	 * Deal with each message to be edited . . .
	 */

	for (ip = msgvec; *ip && ip-msgvec < msgCount; ip++) {
		mesg = *ip;
		mp = &message[mesg-1];
		mp->m_flag |= MODIFY;

		/*
		 * Make up a name for the edit file of the
		 * form "Message%d" and make sure it doesn't
		 * already exist.
		 */

		cp = &nbuf[10];
		*--cp = 0;
		while (mesg) {
			*--cp = mesg % 10 + '0';
			mesg /= 10;
		}
#ifdef	MSDOS
		cp2 = copy("M$", edname);
#else
		cp2 = copy("Message", edname);
#endif
		while (*cp2++ = *cp++)
			;
		if (!access(edname, 2)) {
			printf(ediag(
"%s: file exists\n",
"%s: FAJL SU]ESTWUET\n"),
edname);
			goto out;
		}

		/*
		 * Copy the message into the edit file.
		 */

		if (   fclear(edname) < 0
			|| (obuf = fopen(edname, "a")) == NULL
		   ) {
			perror(edname);
			remove(edname);
			goto out;
		}
		if (send(mp, obuf, 0) < 0) {
			ioerror(edname, 1);
			fclose(obuf);
			remove(edname);
			goto out;
		}
		fflush(obuf);
		if (ferror(obuf)) {
			fclose(obuf);
			remove(edname);
			goto out;
		}
		fclose(obuf);

		/*
		 * If we are in read only mode, make the
		 * temporary message file readonly as well.
		 */

		if (readonly)
			chmod(edname, 0400);

		/*
		 * Fork/execl the editor on the edit file.
		 */

		if (stat(edname, &statb) < 0)
			modtime = 0;
		modtime = statb.st_mtime;

		/*
		 * Set signals; locate editor.
		 */
		sigint = signal(SIGINT, SIG_IGN);
		sigquit = signal(SIGQUIT, SIG_IGN);

#ifndef MSDOS
		pid = vfork();
		if (pid == -1) {
			perror("fork");
#else
		pid = spawnlp (P_WAIT, ed, "mail-editor", edname, NULL);
		if (pid < 0) {
			perror(ed);
#endif
			remove(edname);
			goto out;
		}
#ifndef MSDOS
		if (pid == 0) {
			if (sigint != SIG_IGN)
				sigsys(SIGINT, SIG_DFL);
			if (sigquit != SIG_IGN)
				sigsys(SIGQUIT, SIG_DFL);
			execlp(ed, ed, edname, NULL);
			perror(ed);
			_exit(1);
		}
		while (wait(&s) != pid)
			;
#endif
		signal(SIGINT, sigint);
		signal(SIGQUIT, sigquit);
#ifndef MSDOS
		if ((s & 0377) != 0) {
#else
		if (pid != 0) {
#endif
			printf(ediag(
"Fatal error in \"%s\"\n",
"fATALXNAQ O[IBKA W \"%s\"\n"),
ed);
			remove(edname);
			goto out;
		}
		/*
		 * If in read only mode, just remove the editor
		 * temporary and return.
		 */

		if (readonly) {
			remove(edname);
			continue;
		}

		/*
		 * Now copy the message to the end of the
		 * temp file.
		 */

		if (stat(edname, &statb) < 0) {
			perror(edname);
			goto out;
		}
		if (modtime == statb.st_mtime) {
			remove(edname);
			goto out;
		}
		if ((ibuf = TmpOpen(edname, "r")) == NULL) {
			perror(edname);
			remove(edname);
			goto out;
		}
		fseek(tf, 0L, 2);
		mp->m_offset = ftell(tf);
		ms = 0;
		lines = 0;
		firstline = 1;
		lastline = 0;
		while (fgets(line, sizeof(line), ibuf) != NOSTR) {
			if (*line == 'F' && strncmp("From ", line, 5) == 0) {
				if (!firstline) {
					putc('>', tf);
					if (ferror(tf))
						break;
					ms++;
				}
			}
			else if (firstline) {
				char buf[80];
		BadFrom:
				printf(ediag("WARNING: wrong started message -- corrected\n",
					     "preduprevdenie: NEWERNO NA^ATOE SOOB]ENIE -- ISPRAWLENO\n"));
				(void) sprintf(buf, "From postmaster %s\n\n", udate());
				fputs(buf, tf);
				ms += strlen(buf);
#ifdef  MSDOS
				ms += 2;
#endif
				lines += 2;
			}
			firstline = 0;
			lastline = (*line == '\n');
			fputs(line, tf);
			if (ferror(tf))
				break;
			ms += strlen(line);
#ifdef  MSDOS
			if (*line == '\n')
				ms++;
#endif
			lines++;
		}
		if (firstline) {
			line[0] = '\n';
			line[1] = '\0';
			goto BadFrom;
		}
		if (!lastline) {
			printf(ediag("WARNING: wrong ended message -- corrected\n",
				    "preduprevdenie: NEWERNO ZAKON^ENOE SOOB]ENIE -- ISPRAWLENO\n"));
			putc('\n', tf);
			ms++;
#ifdef  MSDOS
			ms++;
#endif
			lines++;
		}
		mp->m_size = ms;
		mp->m_lines = lines;
		(void) fflush(tf);
		if (ferror(tf))
			ioerror(tempMesg, 1);
		TmpDel(ibuf);
	}

	/*
	 * Restore signals and return.
	 */

out:
	/* remove editor's backfile */
#ifdef	MSDOS
	if ((cp = strrchr(edname, '.')) != NOSTR)
		*cp = '\0';
#endif
	strcpy(backedname, edname);
#ifndef MSDOS
	strcat(backedname, ".b");
#else
	strcat(backedname, ".~");
#endif
	remove(backedname);
	strcpy(backedname, edname);
	strcat(backedname, ".bak");
	remove(backedname);
}
