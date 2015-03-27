/*
 * Mail -- a mail program
 *
 * Handle name lists.
 *
 * $Log: names.c,v $
 * Revision 1.24  1991/07/22  16:36:47  ache
 * Port to Borland C
 *
 * Revision 1.23  1991/07/19  20:00:14  ache
 * dOBAWLENA RASSYLKA SOOB]ENIJ W TELEKONFERENCII
 *
 * Revision 1.22  1991/04/26  16:47:41  ache
 * dOBAWLENA PEREDA^A FLAGA DLQ KODIROWKI wOLAP@K.
 *
 * Revision 1.21  1991/01/25  18:04:45  ache
 * uBRANY STARYE (4.1) SIGNALY
 *
 * Revision 1.20  90/12/08  20:13:15  ache
 * dOBAWLENA OBRABOTKA KAWY^EK
 * 
 * Revision 1.19  90/12/07  14:00:24  ache
 * pRAWLENA OBRABOTKA WREMENNYH FAJLOW I MELO^I
 * 
 * Revision 1.18  90/11/11  20:06:34  ache
 * iSPRAWLENO RASPOZNAWANIE IMEN FAJLOW
 * 
 * Revision 1.17  90/11/02  20:02:19  ache
 * Splitting long argument list.
 * 
 * Revision 1.16  90/10/16  09:13:08  ache
 * ~UTX USKORENA OBRABOTKA >From
 * 
 * Revision 1.15  90/10/13  20:28:17  ache
 * handling From
 * 
 * Revision 1.14  90/09/21  21:59:29  ache
 * MS-DOS extends + some new stuff
 * 
 * Revision 1.13  90/09/13  13:20:07  ache
 * MS-DOS & Unix together...
 * 
 * Revision 1.12  90/08/23  16:50:32  avg
 * wYKINUTA ZABYTAQ OTLADO^NAQ WYDA^A.
 * 
 * Revision 1.11  90/08/20  00:36:07  avg
 * Some patches.
 * 
 * Revision 1.10  90/08/10  13:47:29  avg
 * dOBAWLEN KL@^ -x DLQ PEREDA^I BOLX[IH FAJLOW.
 * 
 * Revision 1.9  90/08/08  17:14:23  avg
 * SENDMAIL->NETMAIL
 * 
 * Revision 1.8  90/08/07  18:22:03  avg
 * PRAWLENA O[IBKA PRI TRANSLQCII BEZ PMCS\
 * 
 * Revision 1.7  90/06/26  17:18:10  avg
 * sDELANA OBRABOTKA WLOVENNYH () W KOMMENTARIQH PO RFC822.
 * 
 * Revision 1.6  90/05/31  23:18:29  avg
 * iSPRAWLENA O[IBKA - POTERQLSQ KONEC STROKI PRI PEREHODE OT
 * ctime K udate.
 * 
 * Revision 1.5  90/05/31  19:47:50  avg
 * pRAWLENA OBRABOTKA Internet-OWSKIH IMEN.
 * 
 * Revision 1.4  90/04/30  19:44:34  avg
 * pO^INEN REVIM rmail
 * 
 * Revision 1.3  88/07/23  20:36:08  ache
 * rUSSKIE DIAGNOSTIKI
 * 
 * Revision 1.2  88/01/11  12:26:32  avg
 * dOBAWLENY KUSKI DLQ RABOTY S PMCS.
 * u rcsid POSTAWLENY KOMMENTARII NOXSTR.
 * 
 * Revision 1.1  87/12/25  16:00:11  avg
 * Initial revision
 *
 */

#include "rcv.h"
#ifdef  MSDOS
#include    <process.h>
#endif


/*
 * Allocate a single element of a name list,
 * initialize its name field to the passed
 * name and return it.
 */

struct name *
nalloc(str)
	char str[];
{
	register struct name *np;

	np = (struct name *) salloc(sizeof *np);
	np->n_flink = NIL;
	np->n_blink = NIL;
	np->n_type = -1;
	np->n_name = savestr(str);
	return(np);
}

/*
 * Find the tail of a list and return it.
 */

struct name *
tailof(name)
	struct name *name;
{
	register struct name *np;

	np = name;
	if (np == NIL)
		return(NIL);
	while (np->n_flink != NIL)
		np = np->n_flink;
	return(np);
}

/*
 * Extract a list of names from a line,
 * and make a list of names from it.
 * Return the list or NIL if none found.
 */

struct name *
extract(line, ntype)
	char line[];
{
	register char *cp;
	register struct name *top, *np, *t;
	char nbuf[BUFSIZ], abuf[BUFSIZ], pbuf[BUFSIZ];
	char *bline, *dp;

	if (line == NOSTR || strlen(line) == 0)
		return(NIL);
	top = NIL;
	np = NIL;

	/*
	 * Try to detect line of form   .... <address> ....
	 */
	if( (cp = index(line, '<')) != NULL &&
	    *(bline = cp+1) != '\0'         &&
	    (cp = index(bline, '>')) != NULL ) {
		dp = pbuf;
		while( bline < cp )
			*dp++ = *bline++;
		*bline = '\0';
		cp = pbuf;
	} else
		cp = line;

	/*
	 * Process list of addresses
	 */
	while ((cp = yankword(cp, nbuf)) != NOSTR) {
		if (np != NIL && equal(nbuf, "at")) {
			strcpy(abuf, nbuf);
			if ((cp = yankword(cp, nbuf)) == NOSTR) {
				strcpy(nbuf, abuf);
				goto normal;
			}
			strcpy(abuf, np->n_name);
			stradd(abuf, '@');
			strcat(abuf, nbuf);
			np->n_name = savestr(abuf);
			continue;
		}
normal:
		t = nalloc(nbuf);
		t->n_type = ntype;
		if (top == NIL)
			top = t;
		else
			np->n_flink = t;
		t->n_blink = np;
		np = t;
	}
	return(top);
}

/*
 * Turn a list of names into a string of the same names.
 */
char *
detract(np, ntype)
	register struct name *np;
{
	register int s;
	register char *cp, *top;
	register struct name *p;
	register int comma;

	comma = ntype & GCOMMA;
	if (np == NIL)
		return(NOSTR);
	ntype &= ~GCOMMA;
	s = 0;
	if (debug && comma)
		fprintf(stderr, ediag(
"detract asked to insert commas\n",
"detract ZAPRA[IWAET WSTAWKU ZAPQTYH\n"));
	for (p = np; p != NIL; p = p->n_flink) {
		if (ntype && (p->n_type & GMASK) != ntype)
			continue;
		s += strlen(p->n_name) + 1;
		if (comma)
			s++;
	}
	if (s == 0)
		return(NOSTR);
	s += 2;
	top = salloc(s);
	cp = top;
	for (p = np; p != NIL; p = p->n_flink) {
		if (ntype && (p->n_type & GMASK) != ntype)
			continue;
		cp = copy(p->n_name, cp);
		if (comma && p->n_flink != NIL)
			*cp++ = ',';
		*cp++ = ' ';
	}
	*--cp = 0;
	if (comma && *--cp == ',')
		*cp = 0;
	return(top);
}

/*
 * Grab a single word (liberal word)
 * Throw away things between ()'s.
 */
char *
yankword(ap, wbuf)
	char *ap, wbuf[];
{
	register char *cp, *cp2;
	int level;
	char q;

	do {
		for (cp = ap; *cp && any(*cp, " \t,"); cp++)
			;
		if (*cp == '(') {
			level = 1;
			while( *cp && level ) {
				switch( *cp++ ) {
				    case ')': level--; break;
				    case '(': level++; break;
				}
			}
			if (*cp)
				cp++;
		}
		if (*cp == '\0')
			return(NOSTR);
	} while (any(*cp, " \t,("));
	cp2 = wbuf;
	if (*cp == '"' || *cp == '\'') {
		q = *cp++;
		while (*cp && *cp != q) {
			if (*cp == '\\' && *(cp + 1) == q)
				cp++;
			*cp2++ = *cp++;
		}
		if (*cp == q)
			cp++;
	}
	else {
		for ( ; *cp && !any(*cp, " \t,(\"'"); *cp2++ = *cp++)
			;
	}
	*cp2 = '\0';
	return(cp);
}

/*
 * Verify that all the users in the list of names are
 * legitimate.  Bitch about and delink those who aren't.
 */

struct name *
verify(names)
	struct name *names;
{
	register struct name *np, *top, *t, *x;
	register char *cp;

#ifdef NETMAIL
	return(names);
#else
	top = names;
	np = names;
	while (np != NIL) {
		if (np->n_type & GDEL) {
			np = np->n_flink;
			continue;
		}
		for (cp = "!:@^"; *cp; cp++)
			if (any(*cp, np->n_name))
				break;
		if (*cp != 0) {
			np = np->n_flink;
			continue;
		}
		cp = np->n_name;
		while (*cp == '\\')
			cp++;
		if (equal(cp, "msgs") ||
		    getuserid(cp) != -1) {
			np = np->n_flink;
			continue;
		}
		fprintf(stderr, ediag(
"Can't send to %s\n",
"nELXZQ POSLATX %s\n"),
np->n_name);
		senderr++;
		if (np == top) {
			top = np->n_flink;
			if (top != NIL)
				top->n_blink = NIL;
			np = top;
			continue;
		}
		x = np->n_blink;
		t = np->n_flink;
		x->n_flink = t;
		if (t != NIL)
			t->n_blink = x;
		np = t;
	}
	return(top);
#endif
}

/*
 * For each recipient in the passed name list with a /
 * in the name, append the message to the end of the named file
 * and remove him from the recipient list.
 *
 * Recipients whose name begins with | are piped through the given
 * program and removed.
 */

struct name *
outof(names, fo, hp)
	struct name *names;
	FILE *fo;
	struct header *hp;
{
	register int c;
	register struct name *np, *top, *t, *x;
	char *date, *fname, *shell, *udate();
	FILE *fout, *fin;
	int ispipe, s, pid;
	extern char tempEdit[];
	char line[LINESIZE];
	int remote = rflag != NOSTR || rmail;
#ifdef PMCS
	int isproject;
	long HdrOffset, ftell();
#endif

	top = names;
	np = names;
	date = udate();
	while (np != NIL) {
#ifdef PMCS
		HdrOffset = 0l;
#endif
		if (!isfileaddr(np->n_name) && np->n_name[0] != '|'
#ifdef PMCS
					    && np->n_name[0] != '='
#endif
								    ) {
			np = np->n_flink;
			continue;
		}
		ispipe    = np->n_name[0] == '|';
#ifdef PMCS
		isproject = np->n_name[0] == '=';
		if (ispipe || isproject)
#else
		if (ispipe)
#endif
			fname = np->n_name+1;
		else
			fname = expand(np->n_name);

		/*
		 * See if we have copied the complete message out yet.
		 * If not, do so.
		 */

		if (image == NULL) {
#ifdef  MSDOS
			maketemp(tempEdit);
#endif
			if (   fclear(tempEdit) < 0
				|| (image = TmpOpen(tempEdit, "a+")) == NULL
			   ) {
				perror(tempEdit);
				remove(tempEdit);
				senderr++;
				goto cant;
			}
			if( !remote ) {
				fprintf(image, "From %s %s\n", myname, date);
#ifdef PMCS
				HdrOffset = ftell(image);
#endif
				puthead(hp, image, GMASK|GNL);
			}
			rewind(fo);
			while (fgets(line, sizeof(line), fo) != NOSTR) {
				if (*line == 'F' && strncmp("From ", line, 5) == 0)
					putc('>', image);
				fputs(line, image);
			}
			putc('\n', image);
			fflush(image);
			if (ferror(image))
				ioerror(tempEdit, 1);
		}

		/*
		 * Now either copy "image" to the desired file
		 * or give it as the standard input to the desired
		 * program as appropriate.
		 */

#ifndef PMCS
		if(ispipe) {
#else
		if(ispipe || isproject) {
#endif
#ifdef  MSDOS
			if ((shell = value("SHELL")) == NULL)
				if ((shell = value("COMSPEC")) == NULL)
					shell = SHELL;
			c = dup (fileno(stdin));
			close(fileno(stdin));
			rewind(image);
			dup(fileno(image));
			s = spawnlp (P_WAIT, shell, "mail-pipe", "/C", fname, NULL);
			if (s < 0) {
				perror(shell);
				dup2 (c, fileno(stdin));
				senderr++;
				goto cant;
			}
			dup2 (c, fileno(stdin));
#else   /* not MSDOS */
			wait(&s);
			rewind(image);
			switch (pid = fork()) {
			case 0:
				sigsys(SIGHUP, SIG_IGN);
				sigsys(SIGINT, SIG_IGN);
				sigsys(SIGQUIT, SIG_IGN);
				close(0);
				dup(fileno(image));
				close(fileno(image));
#ifdef PMCS
				if (isproject) {
					lseek(0, HdrOffset, 0);
					if((shell = value("projmailer")) == NOSTR)
						shell = PROJMAILER;
					execlp(shell, shell, fname, rflag? rflag : myname, NULL);
					perror(shell);
					exit(1);
				}
#endif
				if ((shell = value("SHELL")) == NOSTR)
					shell = SHELL;
				execlp(shell, shell, "-c", fname, NULL);
				perror(shell);
				exit(1);
				break;

			case -1:
				perror("fork");
				senderr++;
				goto cant;
			}
#endif  /* not MSDOS */
		}
		else {
			if ((fout = fopen(fname, "a")) == NULL) {
				perror(fname);
				senderr++;
				goto cant;
			}
			rewind(image);
			while ((c = getc(image)) != EOF)
				putc(c, fout);
			fflush(fout);
			if (ferror(fout))
				senderr++, ioerror(fname, 1);
			fclose(fout);
		}

cant:

		/*
		 * In days of old we removed the entry from the
		 * the list; now for sake of header expansion
		 * we leave it in and mark it as deleted.
		 */

#ifdef CRAZYWOW
		if (np == top) {
			top = np->n_flink;
			if (top != NIL)
				top->n_blink = NIL;
			np = top;
			continue;
		}
		x = np->n_blink;
		t = np->n_flink;
		x->n_flink = t;
		if (t != NIL)
			t->n_blink = x;
		np = t;
#endif

		np->n_type |= GDEL;
		np = np->n_flink;
	}

	TmpDel(image);

	return(top);
}

/*
 * Determine if the passed address is a local "send to file" address.
 * If any of the network metacharacters precedes any slashes, it can't
 * be a filename.  We cheat with .'s to allow path names like ./...
 */
isfileaddr(name)
	char *name;
{
	register char *cp;
	extern char *metanet;

	if (any('@', name))
		return(0);
	if (*name == '+')
		return(1);
#ifdef	MSDOS
	if (name[0] && name[1] == ':')
		return(1);
#endif
	for (cp = name; *cp; cp++) {
		if (any(*cp, metanet))
			return(0);
		if (*cp == '/'
#ifdef	MSDOS
			|| *cp == SEPCHAR
#endif
		   )
			return(1);
	}
	return(0);
}

/*
 * Map all of the aliased users in the invoker's mailrc
 * file and insert them into the list.
 * Changed after all these months of service to recursively
 * expand names (2/14/80).
 */

struct name *
usermap(names)
	struct name *names;
{
	register struct name *new, *np, *cp;
	struct grouphead *gh;
	register int metoo;

	new = NIL;
	np = names;
	metoo = (value("metoo") != NOSTR);
	while (np != NIL) {
		if (np->n_name[0] == '\\') {
			cp = np->n_flink;
			new = put(new, np);
			np = cp;
			continue;
		}
		gh = findgroup(np->n_name);
		cp = np->n_flink;
		if (gh != NOGRP)
			new = gexpand(new, gh, metoo, np->n_type);
		else
			new = put(new, np);
		np = cp;
	}
	return(new);
}

/*
 * Recursively expand a group name.  We limit the expansion to some
 * fixed level to keep things from going haywire.
 * Direct recursion is not expanded for convenience.
 */

struct name *
gexpand(nlist, gh, metoo, ntype)
	struct name *nlist;
	struct grouphead *gh;
{
	struct group *gp;
	struct grouphead *ngh;
	struct name *np;
	static int depth;
	char *cp;

	if (depth > MAXEXP) {
		printf(ediag(
"Expanding alias to depth larger than %d\n",
"sINONIMY RAS[IRQ@TSQ NA GLUBINU BOLX[E ^EM %d\n"),
MAXEXP);
		return(nlist);
	}
	depth++;
	for (gp = gh->g_list; gp != NOGE; gp = gp->ge_link) {
		cp = gp->ge_name;
		if (*cp == '\\')
			goto quote;
		if (strcmp(cp, gh->g_name) == 0)
			goto quote;
		if ((ngh = findgroup(cp)) != NOGRP) {
			nlist = gexpand(nlist, ngh, metoo, ntype);
			continue;
		}
quote:
		np = nalloc(cp);
		np->n_type = ntype;
		/*
		 * At this point should allow to expand
		 * to self if only person in group
		 */
		if (gp == gh->g_list && gp->ge_link == NOGE)
			goto skip;
		if (!metoo && strcmp(cp, myname) == 0)
			np->n_type |= GDEL;
skip:
		nlist = put(nlist, np);
	}
	depth--;
	return(nlist);
}


/*
 * Compute the length of the passed name list and
 * return it.
 */

static
int
lengthof(name)
	struct name *name;
{
	register struct name *np;
	register int c;

	if (name == NIL)
		return 0;
	for (c = 0, np = name; np != NIL; np = np->n_flink) {
		if (np->n_type & GDEL)
			continue;
		c++;
	}
	return(c);
}

/*
 * Concatenate the two passed name lists, return the result.
 */

struct name *
cat(n1, n2)
	struct name *n1, *n2;
{
	register struct name *tail;

	if (n1 == NIL)
		return(n2);
	if (n2 == NIL)
		return(n1);
	tail = tailof(n1);
	tail->n_flink = n2;
	n2->n_blink = tail;
	return(n1);
}

/*
 * Unpack the name list onto a vector of strings.
 * Return an error if the name list won't fit.
 */

char **
unpack(np)
	struct name **np;
{
	register char **ap, **top;
	register struct name *n;
	char *cp;
	char hbuf[10];
	int t, extra, metoo, verbose, volapyuk, cnt;
	extern int filetransfer;

	n = *np;
	if ((t = lengthof(n)) == 0)
		return (char **) NULL;

	/*
	 * Compute the number of extra arguments we will need.
	 * We need at least two extra -- one for "mail" and one for
	 * the terminating 0 pointer.  Additional spots may be needed
	 * to pass along -r and -f to the host mailer.
	 */

	extra = 2;
	if (rflag != NOSTR)
		extra += 2;
#ifdef NETMAIL
	extra++;
	metoo = value("metoo") != NOSTR;
	if (metoo)
		extra++;
	verbose = value("verbose") != NOSTR;
	if (verbose)
		extra++;
	volapyuk = value("volapyuk") != NOSTR;
	if (volapyuk)
		extra++;
#ifndef MSDOS
	if (filetransfer)
		extra++;
#endif  /*MSDOS*/
#endif  /*NETMAIL*/
	if (hflag)
		extra += 2;
	top = (char **) salloc((t + extra) * sizeof cp);
	ap = top;
	*ap++ = "send-mail";
	cnt = strlen(*top);
	if (rflag != NOSTR) {
		*ap++ = "-r";
		*ap++ = rflag;
		cnt += 1 + 2 + 1 + strlen(rflag);
	}
#ifdef NETMAIL
	*ap++ = "-i";
	cnt += 1 + 2;
	if (metoo) {
		*ap++ = "-m";
		cnt += 1 + 2;
	}
	if (verbose) {
		*ap++ = "-v";
		cnt += 1 + 2;
	}
	if (volapyuk) {
		*ap++ = "-V";
		cnt += 1 + 2;
	}
#ifndef MSDOS
	if (filetransfer) {
		*ap++ = "-odq";
		cnt += 1 + 4;
	}
#endif  /*MSDOS*/
#endif  /*NETMAIL*/
	if (hflag) {
		*ap++ = "-h";
		sprintf(hbuf, "%d", hflag);
		cnt += 1 + 2 + 1 + strlen(hbuf);
		*ap++ = savestr(hbuf);
	}
	for ( ; n != NIL; n = n->n_flink ) {
		if (n->n_type & GDEL)
			continue;
		cnt += 1 + strlen(n->n_name);
#ifdef	MSDOS
		if (cnt >= 128)
#else
		if (cnt >= 1024)
#endif
		    break;
		*ap++ = n->n_name;
	}
	*np = n;
	*ap = NOSTR;
	return(top);
}

/*
 * See if the user named himself as a destination
 * for outgoing mail.  If so, set the global flag
 * selfsent so that we avoid removing his mailbox.
 */
void
mechk(names)
	struct name *names;
{
	register struct name *np;

	for (np = names; np != NIL; np = np->n_flink)
		if ((np->n_type & GDEL) == 0 && equal(np->n_name, myname)) {
			selfsent++;
			return;
		}
}

/*
 * Remove all of the duplicates from the passed name list by
 * insertion sorting them, then checking for dups.
 * Return the head of the new list.
 */

struct name *
elide(names)
	struct name *names;
{
	register struct name *np, *t, *new;
	struct name *x;

	if (names == NIL)
		return(NIL);
	new = names;
	np = names;
	np = np->n_flink;
	if (np != NIL)
		np->n_blink = NIL;
	new->n_flink = NIL;
	while (np != NIL) {
		t = new;
		while (nstrcmp(t->n_name, np->n_name) < 0) {
			if (t->n_flink == NIL)
				break;
			t = t->n_flink;
		}

		/*
		 * If we ran out of t's, put the new entry after
		 * the current value of t.
		 */

		if (nstrcmp(t->n_name, np->n_name) < 0) {
			t->n_flink = np;
			np->n_blink = t;
			t = np;
			np = np->n_flink;
			t->n_flink = NIL;
			continue;
		}

		/*
		 * Otherwise, put the new entry in front of the
		 * current t.  If at the front of the list,
		 * the new guy becomes the new head of the list.
		 */

		if (t == new) {
			t = np;
			np = np->n_flink;
			t->n_flink = new;
			new->n_blink = t;
			t->n_blink = NIL;
			new = t;
			continue;
		}

		/*
		 * The normal case -- we are inserting into the
		 * middle of the list.
		 */

		x = np;
		np = np->n_flink;
		x->n_flink = t;
		x->n_blink = t->n_blink;
		t->n_blink->n_flink = x;
		t->n_blink = x;
	}

	/*
	 * Now the list headed up by new is sorted.
	 * Go through it and remove duplicates.
	 */

	np = new;
	while (np != NIL) {
		t = np;
		while (t->n_flink!=NIL &&
		    icequal(np->n_name,t->n_flink->n_name))
			t = t->n_flink;
		if (t == np || t == NIL) {
			np = np->n_flink;
			continue;
		}

		/*
		 * Now t points to the last entry with the same name
		 * as np.  Make np point beyond t.
		 */

		np->n_flink = t->n_flink;
		if (t->n_flink != NIL)
			t->n_flink->n_blink = np;
		np = np->n_flink;
	}
	return(new);
}

/*
 * Version of strcmp which ignores case differences.
 */

nstrcmp(s1, s2)
	register char *s1, *s2;
{
	register int c1, c2;

	do {
		c1 = (*s1++ & 0377);
		if( (c1 & 0300) || ('A'<c1&&c1<'Z') ) c1 |= 040;
		c2 = (*s2++ & 0377);
		if( (c2 & 0300) || ('A'<c2&&c2<'Z') ) c2 |= 040;
	} while (c1 && c1 == c2);
	return(c1 - c2);
}

/*
 * Put another node onto a list of names and return
 * the list.
 */

struct name *
put(list, node)
	struct name *list, *node;
{
	node->n_flink = list;
	node->n_blink = NIL;
	if (list != NIL)
		list->n_blink = node;
	return(node);
}

/*
 * Determine the number of elements in
 * a name list and return it.
 */

count(np)
	register struct name *np;
{
	register int c = 0;

	while (np != NIL) {
		c++;
		np = np->n_flink;
	}
	return(c);
}

cmpdomain(name, dname)
	register char *name, *dname;
{
	char buf[BUFSIZ];

	strcpy(buf, dname);
	buf[strlen(name)] = '\0';
	return(icequal(name, buf));
}

/*
 * Delete the given name from a namelist, using the passed
 * function to compare the names.
 */
struct name *
delname(np, name, cmpfun)
	register struct name *np;
	char name[];
	int (* cmpfun)();
{
	register struct name *p;

	for (p = np; p != NIL; p = p->n_flink)
		if ((* cmpfun)(p->n_name, name)) {
			if (p->n_blink == NIL) {
				if (p->n_flink != NIL)
					p->n_flink->n_blink = NIL;
				np = p->n_flink;
				continue;
			}
			if (p->n_flink == NIL) {
				if (p->n_blink != NIL)
					p->n_blink->n_flink = NIL;
				continue;
			}
			p->n_blink->n_flink = p->n_flink;
			p->n_flink->n_blink = p->n_blink;
		}
	return(np);
}

/*
 * Call the given routine on each element of the name
 * list, replacing said value if need be.
 */

mapf(np, from)
	register struct name *np;
	char *from;
{
	register struct name *p;

	for (p = np; p != NIL; p = p->n_flink)
		p->n_name = netmap(p->n_name, from);
}

/*
 * Pretty print a name list
 * Uncomment it if you need it.
 */

prettyprint(name)
	struct name *name;
{
	register struct name *np;

	np = name;
	while (np != NIL) {
		fprintf(stderr, "%s(%d) ", np->n_name, np->n_type);
		np = np->n_flink;
	}
	fprintf(stderr, "\n");
}
