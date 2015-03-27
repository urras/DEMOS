#include "rcv.h"
#include <sys/stat.h>

/*
 * Rcv -- receive mail rationally.
 *
 * Termination processing.
 *
 * $Log: quit.c,v $
 * Revision 1.10  1991/07/22  16:36:47  ache
 * Port to Borland C
 *
 * Revision 1.9  1991/01/19  15:38:23  ache
 * UBRANY BUFERA 16K, KAK NE OPRAWDAW[IE DOWERIQ NARODA
 *
 * Revision 1.8  90/12/23  21:09:40  ache
 * bUFERIZACIQ IO PO 16 k
 * 
 * Revision 1.7  90/12/07  07:15:28  ache
 * pEREDELANA OBRABOTKA WREMENNYH FAJLOW I MELO^I
 * 
 * Revision 1.6  90/09/22  20:09:50  avg
 * int p ---> long p
 * 
 * Revision 1.5  90/09/21  22:00:19  ache
 * MS-DOS extends + some new stuff
 * 
 * Revision 1.4  90/09/13  17:50:31  ache
 * MS-DOS & Unix together...
 * 
 * Revision 1.3  88/07/23  20:37:41  ache
 * rUSSKIE DIAGNOSTIKI
 * 
 * Revision 1.2  88/01/11  12:45:02  avg
 * dOBAWLEN NOXSTR U rcsid.
 * 
 * Revision 1.1  87/12/25  16:00:32  avg
 * Initial revision
 * 
 */

static void writeback();


/*
 * Save all of the undetermined messages at the top of "mbox"
 * Save all untouched messages back in the system mailbox.
 * Remove the system mailbox, if none saved there.
 */
void
quit()
{
	int mcount, p, modify, autohold, anystat, holdbit, nohold;
	long lp;
	FILE *ibuf = NULL, *obuf, *fbuf, *rbuf = NULL, *readstat;
	register struct message *mp;
	register int c;
	extern char tempQuit[], tempResid[];
	struct stat minfo;
	char *id;

	/*
	 * If we are read only, we can't do anything,
	 * so just return quickly.
	 */

	if (readonly)
		return;
	/*
	 * See if there any messages to save in mbox.  If no, we
	 * can save copying mbox to /tmp and back.
	 *
	 * Check also to see if any files need to be preserved.
	 * Delete all untouched messages to keep them out of mbox.
	 * If all the messages are to be preserved, just exit with
	 * a message.
	 *
	 * If the luser has sent mail to himself, refuse to do
	 * anything with the mailbox, unless mail locking works.
	 */

	lock(mailname);
#ifndef CANLOCK
	if (selfsent) {
		printf(ediag("You have new mail.\n",
			     "u WAS ESTX NOWAQ PO^TA.\n"));
		unlock();
		return;
	}
#endif
	ibuf = rbuf = NULL;
	if (stat(mailname, &minfo) >= 0 && minfo.st_size > mailsize) {
		printf(ediag(
"New mail has arrived.\n",
"pRI[LA NOWAQ PO^TA.\n"));
#ifdef  MSDOS
		maketemp(tempResid);
#endif
		if (   fclear(tempResid) < 0
			|| (rbuf = TmpOpen(tempResid, "a+")) == NULL
		   ) {
			perror(tempResid);
			remove(tempResid);
		}
		fbuf = fopen(mailname, "r");
		if (rbuf == NULL || fbuf == NULL)
			goto newmail;
#ifdef APPEND
		fseek(fbuf, mailsize, 0);
		while ((c = getc(fbuf)) != EOF)
			putc(c, rbuf);
#else
		lp = minfo.st_size - mailsize;
		while (lp-- > 0) {
			c = getc(fbuf);
			if (c == EOF)
				goto newmail;
			putc(c, rbuf);
		}
#endif
		fclose(fbuf);
	}

	/*
	 * Adjust the message flags in each message.
	 */

	anystat = 0;
	autohold = value("hold") != NOSTR;
	holdbit = autohold ? MPRESERVE : MBOX;
	nohold = MBOX|MSAVED|MDELETED|MPRESERVE;
	if (value("keepsave") != NOSTR)
		nohold &= ~MSAVED;
	for (mp = &message[0]; mp < &message[msgCount]; mp++) {
		if (mp->m_flag & MNEW) {
			mp->m_flag &= ~MNEW;
			mp->m_flag |= MSTATUS;
		}
		if (mp->m_flag & MSTATUS)
			anystat++;
		if ((mp->m_flag & MTOUCH) == 0)
			mp->m_flag |= MPRESERVE;
		if ((mp->m_flag & nohold) == 0)
			mp->m_flag |= holdbit;
	}
	modify = 0;
	if (Tflag != NOSTR) {
		if ((readstat = fopen(Tflag, "w")) == NULL)
			Tflag = NOSTR;
	}
	for (c = 0, p = 0, mp = &message[0]; mp < &message[msgCount]; mp++) {
		if (mp->m_flag & MBOX)
			c++;
		if (mp->m_flag & MPRESERVE)
			p++;
		if (mp->m_flag & MODIFY)
			modify++;
		if (Tflag != NOSTR && (mp->m_flag & (MREAD|MDELETED)) != 0) {
			id = hfield("article-id", mp);
			if (id != NOSTR)
				fprintf(readstat, "%s\n", id);
		}
	}
	if (Tflag != NOSTR)
		fclose(readstat);
	if (p == msgCount && !modify && !anystat) {
		if (p == 1)
			printf(ediag(
"Held 1 message in %s\n",
"1 SOOB]ENIE OSTAETSQ W %s\n"),
mailname);
		else if (p > 0)
			printf(ediag(
"Held %d messages in %s\n",
"%d SOOB]ENIJ OSTA@TSQ W %s\n"),
p, mailname);
		TmpDel(rbuf);
		unlock();
		return;
	}
	if (c == 0) {
		if (p != 0) {
			writeback(rbuf);
			unlock();
			return;
		}
		goto cream;
	}

	/*
	 * Create another temporary file and copy user's mbox file
	 * darin.  If there is no mbox, copy nothing.
	 * If he has specified "append" don't copy his mailbox,
	 * just copy saveable entries at the end.
	 */

	mcount = c;
	if (value("append") == NOSTR) {
#ifdef  MSDOS
		maketemp(tempQuit);
#endif
		if (   fclear(tempQuit) < 0
			|| (ibuf = TmpOpen(tempQuit, "a+")) == NULL
		   ) {
			perror(tempQuit);
			remove(tempQuit);
			TmpDel(rbuf);
			unlock();
			return;
		}
		if ((fbuf = fopen(mbox, "r")) != NULL) {
			while ((c = getc(fbuf)) != EOF)
				putc(c, ibuf);
			fclose(fbuf);
		}
		if (ferror(ibuf)) {
			ioerror(tempQuit, 1);
			TmpDel(ibuf);
			TmpDel(rbuf);
			unlock();
			return;
		}
		if (   fclear(mbox) < 0
			|| (obuf = fopen(mbox, "a")) == NULL
		   ) {
			perror(mbox);
			remove(mbox);
			TmpDel(ibuf);
			TmpDel(rbuf);
			unlock();
			return;
		}
	}
	else if ((obuf = fopen(mbox, "a")) == NULL) {
			perror(mbox);
			TmpDel(rbuf);
			unlock();
			return;
		}
	for (mp = &message[0]; mp < &message[msgCount]; mp++)
		if (mp->m_flag & MBOX)
			if (send(mp, obuf, 0) < 0) {
				ioerror(mbox, 1);
				fclose(obuf);
				TmpDel(ibuf);
				TmpDel(rbuf);
				unlock();
				return;
			}

	/*
	 * Copy the user's old mbox contents back
	 * to the end of the stuff we just saved.
	 * If we are appending, this is unnecessary.
	 */

	if (value("append") == NOSTR) {
		rewind(ibuf);
		while ((c = getc(ibuf)) != EOF) {
			putc(c, obuf);
			if (ferror(obuf))
				goto Oerr;
		}
		TmpDel(ibuf);
		ibuf = NULL;
		fflush(obuf);
	}
	if (ferror(obuf)) {
	Oerr:
		ioerror(mbox, 1);
		fclose(obuf);
		TmpDel(ibuf);
		TmpDel(rbuf);
		unlock();
		return;
	}
	fclose(obuf);
	if (mcount == 1)
		printf(ediag(
"Saved 1 message in mbox\n",
"1 SOOB]ENIE ZAPISYWAETSQ W mbox\n"));
	else if (mcount > 0)
		printf(ediag(
"Saved %d messages in mbox\n",
"%d SOOB]ENIJ ZAPISYWA@TSQ W mbox\n"),
mcount);

	/*
	 * Now we are ready to copy back preserved files to
	 * the system mailbox, if any were requested.
	 */

	if (p != 0) {
		writeback(rbuf);
		unlock();
		return;
	}

	/*
	 * Finally, remove his /usr/mail file.
	 * If new mail has arrived, copy it back.
	 */

cream:
	if (rbuf != NULL) {
		if (   fclear(mailname) < 0
			|| (fbuf = fopen(mailname, "a")) == NULL
		   )
			goto newmail;
		rewind(rbuf);
		while ((c = getc(rbuf)) != EOF)
			putc(c, fbuf);
		fclose(fbuf);
		TmpDel(rbuf);
		alter(mailname);
		unlock();
		return;
	}
	demail();
	unlock();
	return;

newmail:
	printf(ediag("Thou hast new mail.\n","wAM ESTX NOWAQ PO^TA.\n"));
	TmpDel(rbuf);
	TmpDel(ibuf);
	unlock();
	return;
}

/*
 * Preserve all the appropriate messages back in the system
 * mailbox, and print a nice message indicated how many were
 * saved.  On any error, just return -1.  Else return 0.
 * Incorporate the any new mail that we found.
 */
static
void
writeback(res)
	register FILE *res;
{
	register struct message *mp;
	register int p, c;
	FILE *obuf;

	p = 0;
	if (   fclear(mailname) < 0
		|| (obuf = fopen(mailname, "a")) == NULL
	   ) {
		perror(mailname);
		TmpDel(res);
		return;
	}
	if (res != NULL)
		rewind(res);
#ifndef APPEND
	if (res != NULL)
		while ((c = getc(res)) != EOF)
			putc(c, obuf);
#endif
	for (mp = &message[0]; mp < &message[msgCount]; mp++)
		if ((mp->m_flag&MPRESERVE)||(mp->m_flag&MTOUCH)==0) {
			p++;
			if (send(mp, obuf, 0) < 0) {
				ioerror(mailname, 1);
				fclose(obuf);
				TmpDel(res);
				return;
			}
		}
#ifdef APPEND
	if (res != NULL)
		while ((c = getc(res)) != EOF)
			putc(c, obuf);
#endif
	TmpDel(res);
	fflush(obuf);
	if (ferror(obuf)) {
		ioerror(mailname, 1);
		fclose(obuf);
		return;
	}
	fclose(obuf);
	alter(mailname);
	if (p == 1)
		printf(ediag(
"Held 1 message in %s\n",
"1 SOOB]ENIE OSTAETSQ W %s\n"),
mailname);
	else if (p > 0)
		printf(ediag(
"Held %d messages in %s\n",
"%d SOOB]ENIJ OSTAETSQ W %s\n"),
p, mailname);
}
