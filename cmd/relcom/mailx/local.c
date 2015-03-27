#

/*
 * Mail -- a mail program
 *
 * Version 7
 *
 * Local routines that are installation dependent.
 *
 * $Log: local.c,v $
 * Revision 1.6  1991/07/22  16:36:47  ache
 * Port to Borland C
 *
 * Revision 1.5  1990/11/11  20:06:27  ache
 * iSPRAWLENO RASPOZNAWANIE IMEN FAJLOW
 *
 * Revision 1.4  90/09/21  21:59:06  ache
 * MS-DOS extends + some new stuff
 * 
 * Revision 1.3  90/09/13  13:19:46  ache
 * MS-DOS & Unix together...
 * 
 * Revision 1.2  88/01/11  12:46:10  avg
 * dOBAWLEN NOXSTR U rcsid.
 * 
 * Revision 1.1  87/12/25  16:01:05  avg
 * Initial revision
 * 
 */

#include "rcv.h"

/*NOXSTR
static char rcsid[] = "$Header: /usr/src/Relcom/relcom/mailx/RCS/local.c,v 1.6 1991/07/22 16:36:47 ache Exp $";
YESXSTR*/

/*
 * Locate the user's mailbox file (ie, the place where new, unread
 * mail is queued).  In Version 7, it is in /usr/spool/mail/name.
 */
void
findmail()
{
	register char *cp, *mdir, *name;

	if ((name = value("MAILBOX")) == NOSTR)
		name = myname;
	else if (isdir(name)) {
		cp = copy(name, mailname);
		if (cp[-1] != '/'
#ifdef	MSDOS
			&& cp[-1] != SEPCHAR
#endif
		   )
			*cp++ = SEPCHAR;
		copy(myname, cp);
		return;
	}
	if ((mdir = value("MAILDIR")) == NOSTR)
		mdir = MAILDIR;
	cp = copy(mdir, mailname);
	if (cp[-1] != '/'
#ifdef	MSDOS
		&& cp[-1] != SEPCHAR
#endif
	   )
		*cp++ = SEPCHAR;
#ifdef	MSDOS
	cp = copy("boxes\\", cp);
#endif
	copy(name, cp);
}

/*
 * Get rid of the queued mail.
 */

demail()
{
	if (   value("keep") != NOSTR
	    || remove(mailname) < 0
	   ) 
		if (fclear(mailname) < 0)
			perror(mailname);
}

/*
 * Discover user login name.
 */

username(uid, namebuf)
	char namebuf[];
{
	register char *np;
#ifndef MSDOS
	extern   char    *getlogin();
#endif

	if (
#ifndef MSDOS
	    uid == getuid() &&
#endif
	    (   (np = value("USER")) != NOSTR
	     || (np = value("LOGNAME")) != NOSTR
#ifndef MSDOS
	     || (np = getlogin()) != NOSTR
#else
	     || (np = value("MAILBOX")) != NOSTR
#endif
	    )
	   ) {
		strncpy(namebuf, np, PATHSIZE - 1);
		namebuf[PATHSIZE - 1] = '\0';
		return(0);
	}
#ifndef MSDOS
	return(getname(uid, namebuf));
#else
	return -1;
#endif
}
