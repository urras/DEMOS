delivermail/                                                                                           775       3       2            0  4717517421   6234                                                                                                                                                                                                                                                                                                                                                                      delivermail/READ_ME                                                                                    660      17       2        13603  2627444444   7406                                                                                                                                                                                                                                                                                                                                                                      This directory contains the files for delivermail.  On some
installations it may also contain the source files for
auxiliary mailers.

The approximate description of each file is as follows:

TO_BE_DONE	New features to be put in (maybe) at some time.
addr.c		Routines to manipulate addresses and address
		lists.
alias.c		Does name aliasing in all forms.
alias.c.save	An old version which has a tentative (and later
		rejected) method of mail forwarding.
arpa-mailer.8	Documentation for arpa.c.
arpa.c		The auxiliary mailer for the ARPANET.
arpadate.c	A subroutine used by delivermail & arpa.c which
		creates ARPANET standard dates.
bmove.c		A core-to-core move.  This is in a separate file
		because it can easily be replaced by a blindingly
		efficient version on Vaxes.
conf.c		The configuration file.  This uses compilation flags
		from <whoami.h> to figure out what to do.
conf.skel.c	A stripped version of conf.c; if you are forced to
		build a configuration file from scratch, you are
		probably better off starting with this one.
deliver.c	Routines to deliver mail, including queueing up
		for eventual delivery & actual sending.
delivermail.8	Documentation for delivermail.
dlvrmail.h	Main header file for delivermail.
err.c		Routines to print error messages.
logmsg.3	Documentation for the message logging routines.
mail-dm.c	Example of the code in server ftp for the Arpanet
		software.
mailaliases.5	Documentation for the alias file used by delivermail.
main.c		The main routine to delivermail.  This file also
		contains some miscellaneous routines.
makefile	The makefile used here.
maketemp.c	The routine that actually reads the mail into a temp
		file.  It also does a certain amount of parsing of
		the header, etc.
matchhdr.c	A routine used by delivermail & arpa.c (& others?)
		which performs a match on ARPANET format header items.
parse.c		The routines which do address parsing.
savemail.c	Routines which save the letter on processing errors.
sysexits.c	List of error messages associated with error codes
		in sysexits.h.
sysexits.h	List of exit statuses that could someday become a
		standard.  In any case, they are what delivermail &
		a handful of other programs use.
useful.h	A header file containing a few things that appear to
		be useful for even the most various programs.
userinfo.5	Tentative documentation for the proposed (and rejected)
		user info database.
util.c		Some general purpose routines used by delivermail.
v6-mail.c	Local mailer under version 6.  This is included to
		permit you to see the sorts of modifications that
		had to take place.
vax-mail.c	Local mailer under VM/UNIX (similar for V7).  This
		is also included as an example.  This is more complex
		than v6-mail.c because it has to be able to deal with
		UUCP mail also.  Beware of local hacks that will not
		apply to your installation.
version.c	A file defining the version.  This gets bumped every
		time a new version is installed (theoretically).

[This list was current as of 10/27/80]

The configuration table distributed here should work with a vanilla
system.  It will try to figure out who you are from whoami.h.  It
depends on:
	BERKELEY -- if set, does all the Berkeley-specific stuff.
		Other local compilation flags provide machine-
		specific definition.
	V6 -- set this if you are on version 6 system.
	sysname -- a defined constant which is your name on the
		UUCP net.  It is assumed that you are on the UUCP
		net.

There are two tables in conf.c which are critical.  The first is
the Mailers table.  This describes each mailer available to your
system.  The second is the parse table, describing how to decide
what net an address applies to, etc.  The comments in the code
should explain how to set these up.  If you do not have any more
nets than berknet, uucp, and arpanet, and if the mailers for these
are in the standard places, you probably won't have to touch the
Mailers table.

You will also have to create a new makefile.  There are only two
basic types: for a v6 system & for a v7 system.  All pathnames &
system dependent stuff should be in conf.c.  The makefile distributed
is for our CSVAX site, and should work for you.  You may want to
change some of the compile flags as listed in the makefile; these
control things like logging, hashed alias tables, and debugging
info.

You will have to change your equivalent of /bin/mail to take a
"-d" flag.  If this flag is not asserted, /bin/mail should just
turn itself into /etc/delivermail (this is probably in a routine
called "bulkmail" or "sendmail").  The code looks like something
like:

# ifdef DELIVERMAIL
	{search argv for a -d flag}
	if (!dflag)
	{
		argv[0] = "-delivermail";
		argv[argc] = 0;
		execv(DELIVERMAIL, argv);
		perror(DELIVERMAIL);
	}
	{ignore -d flag in further processing}
# endif

This is actually somewhat oversimplified.  See the example mailers
for something more realistic.  Besides the mailers in this directory,
see ../berknet/v6mail.c.

Argv[0] should begin with a "-" to indicate that it is being called
from /bin/mail.

If you are running server ftp on arpanet, you will have to change
the incoming mail routine to accept mail for any address (rather than
checking to see that the user exists).  Instead of writing the
mail in some non-standard mailbox, pipe the mail to /etc/delivermail
instead (include the -a flag).  Delivermail will mail back a message
if the letter can't be sent.  Also, I have changed the "success"
message to read "Mail accepted" rather than "Mail delivered", since
the mere fact that delivermail chewed up the mail does not mean that
it has been delivered.  Our "mail-dm.c" is given as an example.

Sometimes (particularly in Arpa-land) mail comes from a person who
doesn't really exist.  Some mail will be returned to them from a
pseudo-user named ~MAILER~DAEMON~, which will of course be returned.
But ~MAILER~DAEMON~ does not exist, so it is returned.... and you have
a loop.  To break this, every site should have the entry:
	~MAILER~DAEMON~:root
in their aliases file.
ce
the mere fact that delivermail chewed up the mail does not mean that
it has been delivered.  Our "mail-dm.c" is given as adelivermail/TO_BE_DONE                                                                                 660      17       2         2434  2470111534   7752                                                                                                                                                                                                                                                                                                                                                                        ***** Change sendto & aliasdbm to know about quoted commas.
	Currently, quoted commas delimit aliases when they should be
	ignored.

   ****	Collect mail together that can be sent all at once, i.e.,
	create equivalence classes of hosts.  This could end up needing
	great knowledge about network topology, but most errors would
	end up being inefficient although still ok.

    *** Put host aliasing into the users file, e.g.,
	``vax135!:research!vax135!''   This would simplify
	configuration considerably, but it might not be worth it.
	For example, this is too sensitive to be in a file that
	is writable by all users.

     ** Collect all names that are local in advance, and send
	them all off to /bin/mail at once -- fork before sending so
	that we can return status quickly.

     ** Handle :Include: specifications.

      * (Somehow) have "From" person modified so that if an alias exists
	for the person, it will appear post-aliased rather than pre-aliased,
	especially (perhaps only?) in ARPANET mail.  For example, if there
	exists an alias "fabry->cory:fabry", then if mail comes in from
	"cory:fabry", make it appear as though it were from "fabry".
	This should probably be in Mail (put "From" and "Date" lines in)
	or in the arpa mailer.

      * Do aliasing on mail being returned also (?).
le :Include: specifications.

      * (Somehow) have "From" person modified so that if an alias exists
	for the person, it will appear post-aliased rather than pre-aliased,
	especially (perhaps only?) in ARPANET mail.  For exampdelivermail/addr.c                                                                                     660      17       2         4302  2470111535   7405                                                                                                                                                                                                                                                                                                                                                                      # include "dlvrmail.h"

static char SccsId[] = "@(#)addr.c	2.1	11/5/80";

/*
**  PUTONQ -- put an address node on the end of a queue
**
**	Parameters:
**		a -- the address to put on the queue.
**		q -- the queue to put it on.
**
**	Returns:
**		none
**
**	Side Effects:
**		none
**
**	Called By:
**		alias
**		recipient
*/

putonq(a, q)
	register addrq *a;
	register addrq *q;
{
	if (q->q_prev == NULL)
	{
		q->q_prev = q->q_next = a;
		a->q_prev = NULL;
	}
	else
	{
		a->q_prev = q->q_prev;
		q->q_prev->q_next = a;
		q->q_prev = a;
	}
	a->q_next = NULL;
}
/*
**  TKOFFQ -- remove address node from queue
**
**	Takes a node off of a queue, from anyplace in the queue.
**
**	Parameters:
**		a -- the node to remove.
**		q -- the queue to remove it from.
**
**	Returns:
**		none
**
**	Side Effects:
**		none
**
**	Called By:
**		alias
*/

tkoffq(a, q)
	register addrq *a;
	register addrq *q;
{
	if (a->q_prev != NULL)
		a->q_prev->q_next = a->q_next;
	else
		q->q_next = a->q_next;
	if (a->q_next != NULL)
		a->q_next->q_prev = a->q_prev;
	else
		q->q_prev = a->q_prev;
}
/*
**  SAMEADDR -- Determine if tow addresses are the same
**
**	This is not just a straight comparison -- if the mailer doesn't
**	care about the host we just ignore it, etc.
**
**	Parameters:
**		a, b -- pointers to the internal forms to compare.
**		wildflg -- if TRUE, 'a' may have no user specified,
**			in which case it is to match anything.
**
**	Returns:
**		TRUE -- they represent the same mailbox.
**		FALSE -- they don't.
**
**	Side Effects:
**		none.
**
**	Called By:
**		recipient
**		alias
*/

bool
sameaddr(a, b, wildflg)
	register addrq *a;
	register addrq *b;
	bool wildflg;
{
	/* if they don't have the same mailer, forget it */
	if (a->q_mailer != b->q_mailer)
		return (FALSE);

	/* if the user isn't the same, we can drop out */
	if ((!wildflg || a->q_user[0] != '\0') && strcmp(a->q_user, b->q_user) != 0)
		return (FALSE);

	/* if the mailer ignores hosts, we have succeeded! */
	if (flagset(M_NOHOST, a->q_mailer->m_flags))
		return (TRUE);

	/* otherwise compare hosts (but be careful for NULL ptrs) */
	if (a->q_host == NULL || b->q_host == NULL)
		return (FALSE);
	if (strcmp(a->q_host, b->q_host) != 0)
		return (FALSE);

	return (TRUE);
}
_mailer != b->q_mailer)
		return (FALSE);

	/* if the user isn't the same, we can drop out */
	if ((!wildflg || a->q_user[0] != '\0') && strcmp(a->q_user, b->q_user) != 0)
		return (FALSE);

	/* if the mailer ignores hosts, we have succeeded! */
	if (flagset(M_NOHOST, a->q_mailer->m_flags))
		return (TRUE);

	/* othedelivermail/alias.c                                                                                    660      17       2        13754  2470111535   7617                                                                                                                                                                                                                                                                                                                                                                      # include <stdio.h>
# include <ctype.h>
# include <pwd.h>
# include "dlvrmail.h"

static char SccsId[] = "@(#)alias.c	2.2	12/6/80";

/*
**  ALIAS -- Compute aliases.
**
**	Scans the file ALIASFILE for a set of aliases.
**	If found, it arranges to deliver to them by inserting the
**	new names onto the SendQ queue.  Uses libdbm database if -DDBM.
**
**	Parameters:
**		none
**
**	Returns:
**		none
**
**	Side Effects:
**		Aliases found on SendQ are removed and put onto
**		AliasQ; replacements are added to SendQ.  This is
**		done until no such replacement occurs.
**
**	Defined Constants:
**		MAXRCRSN -- the maximum recursion depth.
**
**	Called By:
**		main
**
**	Files:
**		ALIASFILE -- the mail aliases.  The format is
**			a series of lines of the form:
**				alias:name1,name2,name3,...
**			where 'alias' expands to all of
**			'name[i]'.  Continuations begin with
**			space or tab.
**		ALIASFILE.pag, ALIASFILE.dir: libdbm version
**			of alias file.  Keys are aliases, datums
**			(data?) are name1,name2, ...
**
**	Notes:
**		If NoAlias (the "-n" flag) is set, no aliasing is
**			done.
**
**	Deficiencies:
**		It should complain about names that are aliased to
**			nothing.
**		It is unsophisticated about line overflows.
*/


# define MAXRCRSN	10

#ifdef DBM
typedef struct {char *dptr; int dsize;} datum;
datum lhs, rhs;
extern datum fetch();
#endif DBM

alias()
{
	register addrq *q;
	addrq *q2;
	FILE *af;
	char line[MAXLINE+1];
	register char *p;
	extern int errno;
	bool didalias;
	bool gotmatch;
	auto addrq al;
	extern bool sameaddr();
	extern addrq *parse();

	if (NoAlias)
		return;
# ifdef DEBUG
	if (Debug)
		printf("--- alias ---\n");
# endif

	/* open alias file if not already open */
#ifndef DBM
# ifdef DEBUG
	if (Debug && (af = fopen("mailaliases", "r")) != NULL)
		printf(" [using local alias file]\n");
	else
# endif
	if ((af = fopen(ALIASFILE, "r")) == NULL)
	{
# ifdef DEBUG
		if (Debug)
			printf("Can't open %s\n", ALIASFILE);
# endif
		errno = 0;
		return;
	}
#else DBM
	dbminit(ALIASFILE);
#endif DBM

#ifndef DBM
	/*
	**  Scan alias file.
	**	If we find any user that any line matches any user, we
	**	will send to the line rather than to the user.
	**
	**	We pass through the file several times.  Didalias tells
	**	us if we took some alias on this pass through the file;
	**	when it goes false at the top of the loop we don't have
	**	to scan any more.  Gotmatch tells the same thing, but
	**	on a line-by-line basis; it is used for processing
	**	continuation lines.
	*/

	do
	{
		didalias = FALSE;
		gotmatch = FALSE;
		rewind(af);
		while (fgets(line, sizeof line, af) != NULL)
		{
			/* comments begin with `#' */
			if (line[0] == '#')
				continue;

			/* check for continuation lines */
			if (isspace(line[0]))
			{
				if (gotmatch)
				{
					sendto(line, 1);
				}
				continue;
			}
			gotmatch = FALSE;

			/*
			**  Check to see if this pseudonym exists in SendQ.
			**	Turn the alias into canonical form.
			**	Then scan SendQ until you do (or do not)
			**	find that address.
			*/

			/*  Get a canonical form for the alias. */
			for (p = line; *p != '\0' && *p != ':' && *p != '\n'; p++)
				continue;
			if (*p == '\0' || *p == '\n')
			{
			 syntaxerr:
				syserr("Bad alias line `%s'", line);
				continue;
			}
			*p++ = '\0';
			if (parse(line, &al, -1) == NULL)
			{
				*--p = ':';
				goto syntaxerr;
			}

			/* if already in AliasQ don't realias */
			for (q = &AliasQ; (q = nxtinq(q)) != NULL; )
			{
				if (sameaddr(&al, q, TRUE))
					break;
			}
			if (q != NULL)
				continue;

			/*  Scan SendQ for that canonical form. */
			for (q = &SendQ; (q = nxtinq(q)) != NULL; )
			{
				if (sameaddr(&al, q, TRUE))
					break;
			}
			if (q != NULL)
			{
				/*
				**  Match on Alias.
				**	Deliver to the target list.
				**	Remove the alias from the send queue
				**	  and put it on the Alias queue.
				*/

# ifdef DEBUG
				if (Debug)
					printf("%s (%s, %s) aliased to %s (%s,%s,%s)\n",
					    q->q_paddr, q->q_host, q->q_user,
					    p, al.q_paddr, al.q_host, al.q_user);
# endif
				tkoffq(q, &SendQ);
				didalias++;
				gotmatch++;
				sendto(p, 1);
				putonq(q, &AliasQ);
			}
		}
	} while (didalias);
	fclose(af);
#else DBM
	/*
	**  Scan SendQ
	**	We only have to do this once, since anything we alias
	**	to is being put at the end of the queue we are
	**	scanning.
	**	If the alias on SendQ is also (already) on AliasQ, we
	**	have an alias such as:
	**		eric:eric,i:eric
	**	In this case we have already done this alias once, and
	**	we don't want to do it again (for obvious reasons!).
	*/

	for (q2 = nxtinq(&SendQ); q2 != NULL; )
	{
		/* if already in AliasQ, don't realias */
		for (q = &AliasQ; (q = nxtinq(q)) != NULL; )
		{
			if (sameaddr(q, q2, TRUE))
				break;
		}
		if (q != NULL)
		{
			q2 = nxtinq(q2);
			continue;
		}

		/* save ptr to next address */
		q = q2;
		q2 = nxtinq(q);

		/* only alias local users */
		if (q->q_mailer != &Mailer[0])
			continue;

		/* create a key for fetch */
		lhs.dptr = q->q_user;
		lhs.dsize = strlen(q->q_user) + 1;
		rhs = fetch(lhs);

		/* find this alias? */
		p = rhs.dptr;
		if (p == NULL)
			continue;

		/*
		**  Match on Alias.
		**	Deliver to the target list.
		**	Remove the alias from the send queue
		**	  and put it on the Alias queue.
		*/

# ifdef DEBUG
		if (Debug)
			printf("%s (%s, %s) aliased to %s\n",
			    q->q_paddr, q->q_host, q->q_user, p);
# endif
		tkoffq(q, &SendQ);
		sendto(p, 1);
		putonq(q, &AliasQ);

		/* if our last entry had an alias, process them */
		if (q2 == NULL)
			q2 = nxtinq(&SendQ);
	}
#endif DBM
}
/*
**  FORWARD -- Try to forward mail
**
**	This is similar but not identical to aliasing.
**
**	Currently it is undefined, until the protocol for userinfo
**	databases is finalized.
**
**	Parameters:
**		user -- the name of the user who's mail we
**			would like to forward to.
**
**	Returns:
**		TRUE -- we have forwarded it somewhere.
**		FALSE -- not forwarded; go ahead & deliver.
**
**	Side Effects:
**		New names are added to SendQ.
**
**	Called By:
**		recipient
*/

bool
forward(user)
	addrq *user;
{
	return (FALSE);
}
 Try to forward maildelivermail/aliases.5                                                                                  660      17       2         1176  2470111536  10045                                                                                                                                                                                                                                                                                                                                                                      .TH ALIASES 5 12/31/79
.SH NAME
aliases \- aliases file for delivermail
.SH SYNOPSIS
/usr/lib/aliases
.SH DESCRIPTION
This file describes user id aliases
that will be used
by
.I /etc/delivermail.
It is formatted as a series of lines
of the form
.in +0.5i
name:addr1,addr2,...addrn
.in
The
.I name
is the name to alias,
and the
.I addri
are the addresses to send the message to.
Lines beginning with white space
are continuation lines.
Lines beginning with `\|#\|'
are comments.
.PP
Aliasing occurs only on local names.
Loops can not occur,
since no message will be sent to any person
more than once.
.SH SEE\ ALSO
delivermail(8)
.SH BUGS
e describes user id aliases
that will be used
by
.I /etc/delivermail.
It is formatted as a series of lines
of the form
.in +0.5i
name:addr1,addr2,...addrn
.in
The
.I name
is the name to alias,
and the
.I addri
are the addresses to send the message to.
Lines beginning with white space
are continuation lines.
Lines beginning with `\|#\|'
are comments.
.PP
Aliasing occurs only on local delivermail/arpa-mailer.8                                                                              660      17       2         1531  2470111537  10615                                                                                                                                                                                                                                                                                                                                                                      .TH ARPA-MAILER 8 12/31/79
.SH NAME
arpa-mailer \- deliver mail to the
.SM ARPANET
.SH SYNOPSIS
/usr/lib/mailers/arpa
.I from-address
.I to-host
.I to-user
.SH DESCRIPTION
.I Arpa-mailer
queues the letter found on its standard input
for delivery to the host and user specified.
The actual delivery will be performed by the
.SM ARPANET
mailer daemon.
.PP
If the letter does not appear to have a full
.SM ARPANET
header,
.I arpa-mailer
will insert ``Date:'' and ``From:''
fields in the proper format.
The ``From:'' person
is determined by the from-address argument,
with colons translated to periods
and ``@Berkeley'' appended.
The from-address argument
is also used by the
mailer daemon
to return the mail to you
if there is a problem at the receiving host.
.SH FILES
.RI /dev/net/ to-host
.br
/usr/spool/netmail/*
.SH SEE\ ALSO
delivermail(8), mailer(ARPA)
mon.
.PP
If the letter does not appear to have a full
.SM ARPANET
header,
.I arpa-mailer
will insert ``Date:'' and ``From:''
fields in the proper format.
The ``From:''delivermail/arpa.c                                                                                     660      17       2        25215  2470111540   7440                                                                                                                                                                                                                                                                                                                                                                      # include	<stdio.h>
# include	<ctype.h>
# include	<signal.h>
# include	<sysexits.h>
# include	"useful.h"

static char SccsId[] = "@(#)arpa.c	2.2	1/11/81";
char Version[] = "@(#)Arpa-mailer version 2.2 of 1/11/81";


/*
**  ARPA MAILER -- Queue ARPANET mail for eventual delivery
**
**	The standard input is stuck away in the outgoing arpanet
**	mail queue for delivery by the true arpanet mailer.
**
**	Usage:
**		/usr/lib/mailers/arpa from host user
**
**	Positional Parameters:
**		from -- the person sending the mail.
**		host -- the host to send the mail to.
**		user -- the user to send the mail to.
**
**	Flags:
**		-T -- debug flag.
**
**	Files:
**		/usr/spool/netmail/* -- the queue file.
**
**	Return Codes:
**		0 -- all messages successfully mailed.
**		2 -- user or host unknown.
**		3 -- service unavailable, probably temporary
**			file system condition.
**		4 -- syntax error in address.
**
**	Compilation Flags:
**		SPOOLDIR -- the spool directory
**
**	Compilation Instructions:
**		cc -n -O -s arpa-mailer.c -o arpa-mailer -lX
**		chmod 755 arpa-mailer
**		mv arpa-mailer /usr/lib/mailers/arpa
**
**	Author:
**		Eric Allman, UCB/INGRES (eric@berkeley)
*/

# define SPOOLDIR	"/usr/spool/netmail"




char	*From;			/* person sending this mail */
char	*To;			/* current "To:" person */
int	State;			/* the current state (for exit codes) */
# ifdef DEBUG
bool	Tflag;			/* -T given */
# endif DEBUG
char	FromHost[200];		/* string to prepend to addresses */
/*
**  MAIN -- Main program for arpa mailer
**
**	Processes arguments, and calls sendmail successively on
**	the To: list.
**
**	Algorithm:
**		Scan for debug flag.
**		Catch interrupt signals.
**		Collect input file name and from person.
**		If more than one person in the to list, and
**			if the input file is not a real file,
**			collect input into a temp file.
**		For each person in the to list
**			Send to that person.
**
**	Parameters:
**		argc
**		argv -- as usual
**
**	Returns:
**		via exit
**
**	Side Effects:
**		Mail gets sent.
**
**	Called By:
**		/etc/delivermail
**
**	Author:
**		Eric Allman UCB/INGRES.
*/

main(argc, argv)
	int argc;
	char **argv;
{
	register int i;
	register char *p;
	register int ifd;
	char buf[512];
	extern int finis();
	extern char *locv();
	register char *q;
	char *lastmark;

	State = 3;
	if (signal(SIGINT, SIG_IGN) != SIG_IGN)
		signal(SIGINT, finis);
	
	/* process flags */
	argv[argc] = 0;
# ifdef DEBUG
	if (strcmp(argv[1], "-T") == 0)
	{
		Tflag++;
		argv++;
		argc--;
		printf("%s\n", Version);
	}
# endif DEBUG

	if (argc != 4)
	{
		rexit (EX_SOFTWARE);
	}
	
	/* decode parameters */
	From = argv[1];
	lastmark = &FromHost[-1];
	for (p = From, q = FromHost; (*q = *p) != '\0'; p++, q++)
	{
		if (*p == ':')
			*q = *p = '.';
		if (*q == '.' || *q == '!' || *q == '@')
			lastmark = q;
	}
	lastmark[1] = '\0';

	/* start sending mail */
	State = sendmail(argv[2], argv[3]);

	/* all done, clean up */
	finis();
}
/*
**  FINIS -- Finish up, remove temp files, etc.
**
**	This does basic cleanup on interrupt, error, or
**	normal termination.  It uses "State" to tell which
**	is happening.
**
**	Parameters:
**		none
**
**	Returns:
**		none
**
**	Side Effects:
**		Exit(State).
**
**	Called By:
**		interrupt signal.
**		main
*/

finis()
{
	rexit(State);
}

/*
** REXIT -- exit, reporting error code if -T given
**
**	Parameters:
**		e -- error code to exit with; see sysexits.h
**
**	Returns:
**		none
**
**	Side Effects:
**		Exit(e).
**
**	Called By:
**		main
**		finis
**		sendmail
*/
rexit(e)
{

# ifdef DEBUG
	if (Tflag)
		fprintf(stderr, "arpa-mail: return code %d\n", e);
# endif
	exit(e);
}
/*
**  SENDMAIL -- Queue up mail for the arpanet mailer.
**
**	The mail is inserted with proper headers into the
**	arpanet queue directory.
**
**	Algorithm:
**		decode "to" address
**			if error, exit.
**		create a spool file name.
**		output the header information to spool file,
**		  separate names in To:, CC: fields with commas.
**		copy the mail to the spool file.
**
**	Parameters:
**		host -- the host to send to.
**		user -- the user to send to.
**
**	Returns:
**		none
**
**	Side Effects:
**		the mail is copied into a file in the network
**			queue directory (/usr/spool/netmail).
**
**	Called By:
**		main
*/

sendmail(host, user)
	char *host;
	char *user;
{
	char spoolfile[50];	/* gets the spool file name */
	register int i;
	register char *p;
	static int callnum;	/* for the final letter on spoolfile */
	char buf[512];
	register FILE *sfp;	/* spool file */
	register int c;
	extern char *matchhdr();

	/* verify that the host exists */
#ifndef TESTING
	strcpy(buf, "/dev/net/");
	strcat(buf, host);
	if (host[0] == '\0' || access(buf, 0) < 0)
		return (EX_NOHOST);
#endif TESTING

	/*
	**  Create spool file name.
	**	Format is "username000nnX", where username is
	**	padded on the right with zeros and nn (the process
	**	id) is padded on the left with zeros; X is a unique
	**	sequence character.
	*/

# ifdef DEBUG
	if (Tflag)
		strcpy(spoolfile, "test.out");
# endif DEBUG
	else
		sprintf(spoolfile, "%s/arpamail%05d%c", SPOOLDIR, getpid(), 'a' + callnum++);

	/* create spool file */
	sfp = fopen(spoolfile, "w");
	if (sfp == NULL)
	{
	spoolerr:
		return (EX_OSERR);
	}
# ifdef DEBUG
	if (!Tflag)
# endif DEBUG
		chmod(spoolfile, 0400);

	/*
	** Output mailer control lines.
	**	These lines are as follows:
	**		/dev/net/<hostname> {target host}
	**		user-name {at target host}
	**		/mnt/eric {pathname of sender; not used}
	**		eric {name of user who is sending}
	*/

	fputs(buf, sfp);
	fputs("\n", sfp);
	fputs(user, sfp);
	fputs("\n\n", sfp);
	fputs(From, sfp);
	fputs("\n", sfp);

	/*
	**  Output the mail
	**	Check the first line for the date.  If not found,
	**	assume the message is not in arpanet standard format
	**	and output a "Date:" and "From:" header.
	*/

	if (fgets(buf, sizeof buf, stdin) == NULL)
	{
		/* no message */
		unlink(spoolfile);
		return (EX_OK);
	}
	if (strncmp("From ", buf, 5) == 0)
	{
		/* strip Unix "From" line */
		/* should save the date here */
		fgets(buf, sizeof buf, stdin);
	}
	if (matchhdr(buf, "date") == NULL)
		putdate(sfp);
	if (!ishdr(buf))
	{
		putc('\n', sfp);
		goto hdrdone;
	}

	/*
	** At this point, we have a message with REAL headers.
	** We look at each head line and insert commas if it
	** is a To: or Cc: field.
	*/

	do
	{
		if (!ishdr(buf))
			break;
		if (!matchhdr(buf, "to") && !matchhdr(buf, "cc"))
		{
			fputs(buf, sfp);
			continue;
		}
		/* gotcha! */
		rewrite(buf, 1, sfp);
		while (isspace(c = peekc(stdin)) && c != '\n')
		{
			fgets(buf, BUFSIZ, stdin);
			rewrite(buf, 0, sfp);
		}
	} while (fgets(buf, BUFSIZ, stdin) != NULL);

hdrdone:
	/* output the rest of the header & the body of the letter */
	do
	{
		fputs(buf, sfp);
		if (ferror(sfp))
			goto spoolerr;
	} while (fgets(buf, sizeof buf, stdin) != NULL);

	/* all done! */
	fclose(sfp);
	return (EX_OK);
}
/*
**  REWRITE -- Output header line with needed commas.
**
**	Parameters:
**		buf -- header line
**		first -- true if this is not a continuation
**
**	Returns:
**		none
**
**	Side effects:
**		The contents of buf is copied onto the spool file with
**		with the right commas interlaced
**
**	Called by:
**		sendmail
*/

rewrite(buf, first, spf)
	char buf[];
	register FILE *spf;
{
	register char *cp;
	register int c;
	char word[BUFSIZ], word2[BUFSIZ];
	char *gword();
	static char wsep[] = ", ";

	cp = buf;
	if (first)
	{
		while (*cp != ':' && *cp)
			putc(*cp++, spf);
		if (*cp == ':')
		{
			fputs(": ", spf);
			cp++;
		}
	}
	else
		while (*cp && isspace(*cp))
			putc(*cp++, spf);
	cp = gword(word, cp);
	if (strlen(word) == 0)
	{
		putc('\n', spf);
		goto test;
	}
	for (;;)
	{
		cp = gword(word2, cp);
		if (strlen(word2) == 0)
		{
			putaddr(word, spf);
			break;
		}
		if (strcmp(word2, "%") == 0)
			word2[0] = '@';
		if (strcmp(word2, "@") && strcmp(word2, "at"))
		{
			putaddr(word, spf);
			fputs(wsep, spf);
			strcpy(word, word2);
			continue;
		}
		fputs(word, spf);
		if (word2[0] == '@')
			putc('@', spf);
		else
			fputs(" at ", spf);
		cp = gword(word, cp);
		fputs(word, spf);
		cp = gword(word, cp);
		if (strlen(word))
			fputs(wsep, spf);
	}

test:
	c = peekc(stdin);
	if (isspace(c) && c != '\n')
		fputs(",\n", spf);
	else
		putc('\n', spf);
}
/*
**  PUTADDR -- output address onto file
**
**	Putaddr prepends the network header onto the address
**	unless one already exists.
**
**	Parameters:
**		name -- the name to output.
**		fp -- the file to put it on.
**
**	Returns:
**		none.
**
**	Side Effects:
**		name is put onto fp.
*/

putaddr(name, fp)
	char *name;
	FILE *fp;
{
	register char *p;

	if (strlen(name) == 0)
		return;
	for (p = name; *p != '\0' && *p != ':' && *p != '.' && *p != '@' &&
	     *p != '!' && *p != '^'; p++)
		continue;
	if (*p == ':')
		*p = '.';
	else if (*p == '\0')
		fputs(FromHost, fp);
	fputs(name, fp);
	if (*p != '@')
		fputs("@Berkeley", fp);
}
/*
**  PEEKC -- peek at next character in input file
**
**	Parameters:
**		fp -- stdio file buffer
**
**	Returns:
**		the next character in the input or EOF
**
**	Side effects:
**		None.
**
**	Called by:
**		sendmail
**		rewrite
*/
peekc(fp)
	register FILE *fp;
{
	register int c;

	c = getc(fp);
	ungetc(c, fp);
	return(c);
}

/*
**  GWORD -- get the next liberal word from a string
**
**	Parameters:
**		buf -- place to put scanned word
**		p -- place to start looking for word
**
**	Returns:
**		updated value of p or 0 if no more left after this
**
**	Side effects:
**		buf gets the liberal word scanned.
**		buf will be length 0 if there is no more input,
**		or if p was passed as 0
**
**	Called by:
**		rewrite
*/
char *
gword(buf, p)
	char buf[];
	register char *p;
{
	register char *sp, *dp;

	strcpy(buf, "");
	if (p == 0)
		return(0);
	sp = p;
	while (*sp && (isspace(*sp) || *sp == ','))
		sp++;
	dp = buf;
	if (*sp != '%' && *sp != '@')
	{
		while (*sp && !isspace(*sp) && *sp != ',' && *sp != '%' && *sp != '@')
			*dp++ = *sp++;
	}
	else
		*dp++ = *sp++;
	*dp = 0;
	if (*sp == 0)
		return(0);
	return(sp);
}
/*
**  ISHDR -- see if the passed line is a ARPA style header line
**
**	Parameters:
**		buf -- header line
**
**	Returns:
**		non-zero if the line is a header line, else zero
**
**	Side effects:
**		none
**
**	Called by:
**		sendmail
*/
ishdr(buf)
	char buf[];
{
	register char *p;

	p = buf;
	if (isspace(*p))
		p = 0;
	else
	{
		while (*p != ':' && !isspace(*p))
			p++;
		while (isspace(*p))
			p++;
		if (*p != ':')
			p = 0;
	}
	return(p != 0);
}
/*
**  PUTDATE -- Put the date & from field into the message.
**
**	Parameters:
**		fp -- file to put them onto.
**
**	Returns:
**		none
**
**	Side Effects:
**		output onto fp.
**
**	Called By:
**		sendmail
*/

putdate(fp)
	register FILE *fp;
{
	register char *p;

	fputs("Date: ", fp);
	fputs(arpadate(), fp);

	/* output from field */
	fputs("\nFrom: ", fp);
	fputs(From, fp);
	fputs(" at Berkeley\n", fp);
}
isspace(*p))
			p++;
		while (isspace(*p))
			p++;
		if (*p != ':')
			p = 0;
	}
	return(p != 0);
}
/*
**  PUTDATE -- Put the date & from field into the message.
**
**	Parameters:
**		fp -- file to put them onto.
**
**	Returns:
**		none
**
**	Side Effects:
**		output onto fp.
**
**	Called By:
**		sendmail
*/

putdate(fp)
	register FILE *fp;
{
	register char *p;

	fputdelivermail/arpadate.c                                                                                 660      17       2         1551  2470111540  10253                                                                                                                                                                                                                                                                                                                                                                      # include <time.h>

static char SccsId[] = "@(#)arpadate.c	2.1	11/5/80";

/*
**  ARPADATE -- Create date in ARPANET format
**
**	Parameters:
**		none
**
**	Returns:
**		pointer to an ARPANET date field
**
**	Side Effects:
**		none
**
**	WARNING:
**		date is stored in a local buffer -- subsequent
**		calls will overwrite.
*/

char *
arpadate()
{
	register char *ud;	/* the unix date */
	long t;
	extern struct tm *localtime();
	register char *p;
	static char b[40];
	extern char *ctime();

	time(&t);
	ud = ctime(&t);

	ud[3] = ud[7] = ud[10] = ud[19] = ud[24] = '\0';
	p = &ud[8];		/* 16 */
	if (*p == ' ')
		p++;
	strcpy(b, p);
	strcat(b, " ");
	strcat(b, &ud[4]);	/* Sep */
	strcat(b, " ");
	strcat(b, &ud[20]);	/* 1979 */
	strcat(b, " ");
	strcat(b, &ud[11]);	/* 01:03:52 */
	if (localtime(&t)->tm_isdst)
		strcat(b, "-PDT");
	else
		strcat(b, "-PST");
	return (b);
}
 *ud;	/* the unix date */
	long t;
	extern struct tm *localtime();
	register char *p;
	static char b[40];
	extern char *ctime();

	time(&t);
	ud = ctimdelivermail/bmove.11.s                                                                                 660      17       2         3400  2470111541  10036                                                                                                                                                                                                                                                                                                                                                                      /
/  BMOVE -- block move
/
/	@(#)bmove.11.s	2.1	11/5/80
/
/	This is a highly optimized version of the old C-language
/	bmove routine; it's function (should be) identical.
/	It uses a fancy algorithm to move words instead of bytes
/	whenever possible.
/
/	In C the routine is:
/		char *bmove(a, b, l)
/		char	*a, *b;
/		int	l;
/		{
/			register int	n;
/			register char	*p, *q;
/			p = a;
/			q = b;
/			n = l;
/			while (n--)
/				*q++ = *p++;
/			return (q);
/		}
/
/	Parameters:
/		a [4(sp)] -- source area
/		b [6(sp)] -- target area
/		l [10(sp)] -- byte count
/
/	Returns:
/		Pointer to end of target area.
/
/	History:
/		3/14/79 [rse] -- added odd to odd case
/		2/9/78 [bob] -- converted from "C"
/
/

.globl	_bmove

_bmove:
	mov	r2,-(sp)	/ save r2
	mov	4(sp),r1	/ get src address
	mov	6(sp),r0	/ get dst address

	/ determine whether to use word or byte move
	mov	r0,r2		/ r2 will reflect the three cases
	bic	$177776,r2	/ keep only last bit of dst
	ror	4(sp)		/ get least significant bit of src
	adc	r2		/ add it in.
	beq	wordm		/ both on even boundary
	dec	r2		/ check for odd case
	bgt	wordodd		/ both on odd boundary

	mov	10(sp),r2	/ get count
	beq	done
bytem:
	movb	(r1)+,(r0)+	/ copy next byte
	sob	r2,bytem	/ branch until done
	br	done

wordm:
	mov	10(sp),r2	/ get count
wordt:
	beq	done
	asr	r2		/ get word count
	bcs	odd		/ count was odd
even:
	mov	(r1)+,(r0)+	/ copy word
	sob	r2,even		/ more to do if non-zero
	br	done

wordodd:
	mov	10(sp),r2	/ get count
	beq	done
	movb	(r1)+,(r0)+	/ copy byte
	dec	r2		/ dec count
	br	wordt		/ now treat as an even word move

odd:
	beq	odd2		/ special case of count = 1
odd1:
	mov	(r1)+,(r0)+	/ copy word
	sob	r2,odd1		/ continue
odd2:
	movb	(r1)+,(r0)+	/ count was odd. do last one

done:
	mov	(sp)+,r2	/ restore r2
	rts	pc		/ return
t count
wordt:
	beq	done
	asr	r2		/ get word count
	bcs	odd		/ count was odd
even:
	mov	(r1)+,(r0)+	/ copy word
	sob	r2,even		/ more to do if non-zero
	br	done

wordodd:
	mov	10(sp),r2	/ get count
	beq	done
	movb	(r1)+,(r0)+	/ copy byte
	dec	r2		/ dec coundelivermail/bmove.c                                                                                    660      17       2          172  2470111541   7561                                                                                                                                                                                                                                                                                                                                                                      /* @(#)bmove.c	2.1	11/5/80 */

bmove(s, d, l)
	register char *s, *d;
	register int l;
{
	while (l-- > 0)
		*d++ = *s++;
}
ord
	sob	r2,odd1		/ continue
odd2:
	movb	(r1)+,(r0)+	/ count was odd. do last one

done:
	mov	(sp)+,r2	/ restore r2
	rts	pc		/ return
t count
wordt:
	beq	done
	asr	r2		/ get word count
	bcs	odd		/ count was odd
even:
	mov	(r1)+,(r0)+	/ copy word
	sob	r2,even		/ more to do if non-zero
	br	done

wordodd:
	mov	10(sp),r2	/ get count
	beq	done
	movb	(r1)+,(r0)+	/ copy byte
	dec	r2		/ dec coundelivermail/bmove.vax.s                                                                                660      17       2          225  2470111542  10376                                                                                                                                                                                                                                                                                                                                                                      #
#  BMOVE.S -- optimized block move routine.
#
#	@(#)bmove.vax.s	2.1	11/5/80
#
.globl	_bmove
_bmove:
	.word	0x0030
	movc3	12(ap),*4(ap),*8(ap)
	ret
e
odd2:
	movb	(r1)+,(r0)+	/ count was odd. do last one

done:
	mov	(sp)+,r2	/ restore r2
	rts	pc		/ return
t count
wordt:
	beq	done
	asr	r2		/ get word count
	bcs	odd		/ count was odd
even:
	mov	(r1)+,(r0)+	/ copy word
	sob	r2,even		/ more to do if non-zero
	br	done

wordodd:
	mov	10(sp),r2	/ get count
	beq	done
	movb	(r1)+,(r0)+	/ copy byte
	dec	r2		/ dec coundelivermail/conf.c                                                                                     660      17       2        27411  3130024051   7434                                                                                                                                                                                                                                                                                                                                                                      # include <stdio.h>
# include <pwd.h>
# include "dlvrmail.h"

/*
**  CONF.C -- Delivermail Configuration Tables.
**
**	Defines the configuration of this installation.
**
**	Compilation Flags:
**		HASARPA -- set if this machine has a connection to
**			the Arpanet.
**		HASUUCP -- set if this machine has a connection to
**			the UUCP network.
**		NETV6MAIL -- set if you want to use "v6mail" that
**			comes with the Berkeley network.  Normally
**			/bin/mail will work fine, but around Berkeley
**			we use v6mail because it is a "fixed target".
**		V6 -- running on a version 6 system.  This determines
**			whether to define certain routines between
**			the two systems.  If you are running a funny
**			system, e.g., V6 with long tty names, this
**			should be checked carefully.
**		DUMBMAIL -- set if your /bin/mail doesn't have the
**			-d flag.
**
**	Configuration Variables:
**		ArpaHost -- the arpanet name of the host through
**			which arpanet mail will be sent.
**		MyLocName -- the name of the host on a local network.
**			This is used to disambiguate the contents of
**			ArpaHost among many hosts who may be sharing
**			a gateway.
**		DaemonName -- the name of this agent for use in
**			error messages, typically "~MAILER~DAEMON~"
**			at this host on the local net.
**		ArpaLocal -- a list of local names for this host on
**			the arpanet.  Only functional if HASARPA set.
**		UucpLocal -- ditto for the Arpanet.
**		BerkLocal -- ditto for the Berknet.
**		Mailer -- a table of mailers known to the system.
**			The fields are:
**			- the pathname of the mailer.
**			- a list of flags describing the properties
**			  of this mailer:
**			   M_FOPT -- if set, the mailer has a picky "-f"
**				option.  In this mode, the mailer will
**				only accept the "-f" option if the
**				sender is actually "root", "network",
**				and possibly (but not necessarily) if
**				the -f argument matches the real sender.
**				The effect is that if the "-f" option
**				is given to delivermail then it will be
**				passed through (as arguments 1 & 2) to
**				the mailer.
**			   M_ROPT -- identical to M_FOPT, except uses
**				-r instead.
**			   M_QUIET -- if set, don't print a message if
**				the mailer returns bad status.
**			   M_RESTR -- if set, this mailer is restricted
**				to use by "daemon"; otherwise, we do a
**				setuid(getuid()) before calling the
**				mailer.
**			   M_HDR -- if set, the mailer wants us to
**				insert a UNIX "From" line before
**				outputing.
**			   M_FHDR -- if set, the header that we
**				generate will be used literally, so
**				we must force it to be correct.  The
**				effect is that we generate a header
**				even if one exists.
**			   M_NOHOST -- if set, this mailer doesn't care
**				about the host part (e.g., the local
**				mailer).
**			   M_STRIPQ -- if set, strip quote (`"')
**				characters out of parameters as you
**				transliterate them into the argument
**				vector.  For example, the local mailer
**				is called directly, so these should be
**				stripped, but the program-mailer (i.e.,
**				csh) should leave them in.
**			- an exit status to use as the code for the
**			  error message print if the mailer returns
**			  something we don't understand.
**			- A list of names that are to be considered
**			  "local" (and hence are stripped off) for
**			  this mailer.
**			- An argument vector to be passed to the
**			  mailer with the following substitutions:
**			   $f - the from person name.
**			   $u - the target user name.
**			   $h - the target user host.
**			   $c - the hop count.
**			>>>>>>>>>> Entry zero must be for the local
**			>> NOTE >> mailer and entry one must be for
**			>>>>>>>>>> the shell.
**		ParseTab -- a table driving the parsing process.  Each
**			entry contains:
**			- a character that will trigger this entry.
**			- an index into the Mailer table.
**			- a word of flags, described in dlvrmail.h.
**			- an argument.  If we have P_MAP, it is the
**			  character to turn the trigger character into.
**			  If we have P_MOVE, it is the site to send it
**			  to, using the mailer specified above.
**			This table will almost certainly have to be
**			changed on your site if you have anything more
**			than the UUCP net.
*/




static char SccsId[] = "@(#)conf.c	2.4	1/10/81";


bool	UseMsgId = FALSE;	/* don't put message id's in anywhere */

# include <whoami.h>		/* definitions of machine id's at berkeley */

# ifdef BERKELEY

char	*ArpaHost = "Berkeley";	/* host name of gateway on Arpanet */

# else BERKELEY

char	*ArpaHost = "[unknown]";
char	*MyLocName = sysname;
# define HASUUCP		/* default to having UUCP net */
char	*UucpLocal[] = { sysname, NULL };

# endif BERKELEY


/* Specific Configurations for Berkeley Machines */

/* Berkeley people: mail changes to csvax:eric or they will be lost! */

# ifdef ING70
static char	*BerkLocal[] = { "i", "ingres", "ing70", NULL };
char		*ArpaLocal = { "berkeley", "ucb", NULL };
char		*MyLocName = "Ing70";
char		*DaemonName = "Ing70:~MAILER~DAEMON~";
# define HASARPA
# define V6
# endif ING70

# ifdef INGVAX
static char	*BerkLocal[] = { "j", "ingvax", NULL };
char		*MyLocName = "IngVax";
char		*DaemonName = "IngVax:~MAILER~DAEMON~";
# endif INGVAX

# ifdef CSVAX
static char	*BerkLocal[] = { "v", "csvax", "vax", NULL };
static char	*UucpLocal[] = { "ucbvax", "ernie", NULL };
char		*MyLocName = "CSVAX";
char		*DaemonName = "CSVAX:~MAILER~DAEMON~";
# define HASUUCP
# define NETV6MAIL
# endif CSVAX

# ifdef CORY
static char	*BerkLocal[] = { "y", "cory", NULL };
char		*MyLocName = "Cory";
char		*DaemonName = "Cory:~MAILER~DAEMON~";
# endif CORY

# ifdef IMAGE
/* untested */
static char	*BerkLocal[] = { "m", "image", NULL };
char		*MyLocName = "Image";
char		*DaemonName = "Image:~MAILER~DAEMON~";
# define V6
# endif IMAGE

# ifdef ESVAX
static char	*BerkLocal[] = { "o", "esvax", NULL };
char		*MyLocName = "ESVAX";
char		*DaemonName = "ESVAX:~MAILER~DAEMON~";
# endif ESVAX

# ifdef EECS40
/* untested */
static char	*BerkLocal[] = { "z", "eecs40", NULL };
char		*MyLocName = "EECS40";
char		*DaemonName = "EECS40:~MAILER~DAEMON~";
# define V6
# endif EECS40


# ifndef HASARPA
# define ArpaLocal	NULL
# endif HASARPA

# ifndef HASUUCP
# define UucpLocal	NULL
# endif HASUUCP


struct mailer Mailer[] =
{
	/* local mail -- must be #0 */
	{
# ifdef NETV6MAIL
		"/usr/net/bin/v6mail",
# else
		"/bin/mail",
# endif
		M_ROPT|M_NOHOST|M_STRIPQ,	EX_NOUSER,	NULL,
		{ "...local%mail", "-d", "$u", NULL }
	},
	/* pipes through programs -- must be #1 */
	{
		"/bin/csh",
		M_HDR|M_FHDR|M_NOHOST,		EX_UNAVAILABLE,	NULL,
		{ "...prog%mail", "-fc", "$u", NULL }
	},
	/* local berkeley mail */
	{
		"/usr/net/bin/sendberkmail",
		M_FOPT|M_HDR|M_STRIPQ,		EX_UNAVAILABLE,	BerkLocal,
		{ "...berk%mail", "-m", "$h", "-t", "$u", "-h", "$c", NULL }
	},
	/* arpanet mail */
	{
		"/usr/lib/mailers/arpa",
		M_STRIPQ,			0,		ArpaLocal,
		{ "...arpa%mail", "$f", "$h", "$u", NULL }
	},
	/* uucp mail (cheat & use Bell's v7 mail) */
	{
		"/bin/mail",
		M_ROPT|M_STRIPQ,		EX_NOUSER,	UucpLocal,
# ifdef DUMBMAIL
		{ "...uucp%mail", "$h!$u", NULL }
# else
		{ "...uucp%mail", "-d", "$h!$u", NULL }
# endif DUMBMAIL
	},
};

# define M_LOCAL	0
# define M_BERK		2
# define M_ARPA		3
# define M_UUCP		4



# ifdef BERKELEY
struct parsetab ParseTab[] =
{
	':',	M_BERK,		P_ONE,				NULL,
# ifdef HASARPA
	'@',	M_ARPA,		P_HLAST|P_USR_UPPER,		NULL,
# else
	'@',	M_BERK,		P_HLAST|P_USR_UPPER|P_MOVE,	"ing70",
# endif HASARPA
	'^',	-1,		P_MAP,				"!",
# ifdef HASUUCP
	'!',	M_UUCP,		0,				NULL,
# else
	'!',	M_BERK,		P_MOVE,				"csvax",
# endif HASUUCP
	'.',	-1,		P_MAP|P_ONE,			":",
	'\0',	M_LOCAL,	P_MOVE,				"",
};
# else BERKELEY
struct parsetab ParseTab[] =
{
# ifdef HASARPA
	'@',	M_ARPA,		P_HLAST|P_USR_UPPER,		NULL,
# endif HASARPA
# ifdef HASUUCP
	'^',	-1,		P_MAP,				"!",
	'!',	M_UUCP,		0,				NULL,
# endif HASUUCP
	'\0',	M_LOCAL,	P_MOVE,				"",
};
# endif BERKELEY
/*
**  GETNAME -- Get the current users login name.
**
**	This is in config.c because it is somewhat machine dependent.
**	Examine it carefully for your installation.
**
**	Algorithm:
**		See if the person is logged in.  If so, return
**			the name s/he is logged in as.
**		Look up the user id in /etc/passwd.  If found,
**			return that name.
**		Return NULL.
**
**	Parameters:
**		none
**
**	Returns:
**		The login name of this user.
**		NULL if this person is noone.
**
**	Side Effects:
**		none
**
**	Called By:
**		main
*/

char *
getname()
{
	register char *p;
	register struct passwd *w;
	extern struct passwd *getpwuid();
	static char namebuf[9];

# ifdef V6
	w = getpwuid(getuid() & 0377);
# else
	w = getpwuid(getuid());
# endif V6
	if (w != NULL)
	{
		strcpy(namebuf, w->pw_name);
		return (namebuf);
	}
	return (NULL);
}

# ifdef V6
/*
**  TTYPATH -- Get the path of the user's tty -- Version 6 version.
**
**	Returns the pathname of the user's tty.  Returns NULL if
**	the user is not logged in or if s/he has write permission
**	denied.
**
**	Parameters:
**		none
**
**	Returns:
**		pathname of the user's tty.
**		NULL if not logged in or write permission denied.
**
**	Side Effects:
**		none.
**
**	WARNING:
**		Return value is in a local buffer.
**
**	Called By:
**		savemail
*/

# include <sys/types.h>
# include <sys/stat.h>

char *
ttypath()
{
	struct stat stbuf;
	register int i;
	static char pathn[] = "/dev/ttyx";
	extern int errno;

	/* compute the pathname of the controlling tty */
	if ((i = ttyn(2)) == 'x' && (i = ttyn(1)) == 'x' && (i = ttyn(0)) == 'x')
	{
		errno = 0;
		return (NULL);
	}
	pathn[8] = i;

	/* see if we have write permission */
	if (stat(pathn, &stbuf) < 0 || !flagset(02, stbuf.st_mode))
	{
		errno = 0;
		return (NULL);
	}

	/* see if the user is logged in */
	if (getlogin() == NULL)
		return (NULL);

	/* looks good */
	return (pathn);
}
/*
**  FDOPEN -- Open a stdio file given an open file descriptor.
**
**	This is included here because it is standard in v7, but we
**	need it in v6.
**
**	Algorithm:
**		Open /dev/null to create a descriptor.
**		Close that descriptor.
**		Copy the existing fd into the descriptor.
**
**	Parameters:
**		fd -- the open file descriptor.
**		type -- "r", "w", or whatever.
**
**	Returns:
**		The file descriptor it creates.
**
**	Side Effects:
**		none
**
**	Called By:
**		deliver
**
**	Notes:
**		The mode of fd must match "type".
*/

FILE *
fdopen(fd, type)
	int fd;
	char *type;
{
	register FILE *f;

	f = fopen("/dev/null", type);
	close(fileno(f));
	fileno(f) = fd;
	return (f);
}
/*
**  INDEX -- Return pointer to character in string
**
**	For V7 compatibility.
**
**	Parameters:
**		s -- a string to scan.
**		c -- a character to look for.
**
**	Returns:
**		If c is in s, returns the address of the first
**			instance of c in s.
**		NULL if c is not in s.
**
**	Side Effects:
**		none.
*/

index(s, c)
	register char *s;
	register char c;
{
	while (*s != '\0')
	{
		if (*s++ == c)
			return (--s);
	}
	return (NULL);
}
# endif V6

# ifndef V6
/*
**  TTYPATH -- Get the path of the user's tty -- Version 7 version.
**
**	Returns the pathname of the user's tty.  Returns NULL if
**	the user is not logged in or if s/he has write permission
**	denied.
**
**	Parameters:
**		none
**
**	Returns:
**		pathname of the user's tty.
**		NULL if not logged in or write permission denied.
**
**	Side Effects:
**		none.
**
**	WARNING:
**		Return value is in a local buffer.
**
**	Called By:
**		savemail
*/

# include <sys/types.h>
# include <sys/stat.h>

char *
ttypath()
{
	struct stat stbuf;
	register char *pathn;
	extern int errno;
	extern char *ttyname();

	/* compute the pathname of the controlling tty */
	if ((pathn = ttyname(2)) == NULL && (pathn = ttyname(1)) == NULL && (pathn = ttyname(0)) == NULL)
	{
		errno = 0;
		return (NULL);
	}

	/* see if we have write permission */
	if (stat(pathn, &stbuf) < 0 || !flagset(02, stbuf.st_mode))
	{
		errno = 0;
		return (NULL);
	}

	/* see if the user is logged in */
	if (getlogin() == NULL)
		return (NULL);

	/* looks good */
	return (pathn);
}
# endif V6
er char *pathn;
	extern int errno;
	extern char *ttyname();

	/* compute the pathname of the controlling tty */
	if ((pathn = ttyname(2)) == NULL && (pathn = ttyname(1)) == NULL && (pathn = ttyname(0)) == NULL)
	{
		errno = 0;
		return (NULL);
	}
delivermail/conf.skel.c                                                                                660      17       2        23325  3130024113  10370                                                                                                                                                                                                                                                                                                                                                                      # include <stdio.h>
# include <pwd.h>
# include "dlvrmail.h"

/*
**  CONF.C -- Delivermail Configuration Tables.
**
**	Defines the configuration of this installation.
**
**	Compilation Flags:
**		HASARPA -- set if this machine has a connection to
**			the Arpanet.
**		HASUUCP -- set if this machine has a connection to
**			the UUCP network.
**		NETV6MAIL -- set if you want to use "v6mail" that
**			comes with the Berkeley network.  Normally
**			/bin/mail will work fine, but around Berkeley
**			we use v6mail because it is a "fixed target".
**		V6 -- running on a version 6 system.  This determines
**			whether to define certain routines between
**			the two systems.  If you are running a funny
**			system, e.g., V6 with long tty names, this
**			should be checked carefully.
**		DUMBMAIL -- set if your /bin/mail doesn't have the
**			-d flag.
**
**	Configuration Variables:
**		ArpaHost -- the arpanet name of the host through
**			which arpanet mail will be sent.
**		MyLocName -- the name of the host on a local network.
**			This is used to disambiguate the contents of
**			ArpaHost among many hosts who may be sharing
**			a gateway.
**		ArpaLocal -- a list of local names for this host on
**			the arpanet.  Only functional if HASARPA set.
**		UucpLocal -- ditto for the Arpanet.
**		BerkLocal -- ditto for the Berknet.
**		Mailer -- a table of mailers known to the system.
**			The fields are:
**			- the pathname of the mailer.
**			- a list of flags describing the properties
**			  of this mailer:
**			   M_FOPT -- if set, the mailer has a picky "-f"
**				option.  In this mode, the mailer will
**				only accept the "-f" option if the
**				sender is actually "root", "network",
**				and possibly (but not necessarily) if
**				the -f argument matches the real sender.
**				The effect is that if the "-f" option
**				is given to delivermail then it will be
**				passed through (as arguments 1 & 2) to
**				the mailer.
**			   M_ROPT -- identical to M_FOPT, except uses
**				-r instead.
**			   M_QUIET -- if set, don't print a message if
**				the mailer returns bad status.
**			   M_RESTR -- if set, this mailer is restricted
**				to use by "daemon"; otherwise, we do a
**				setuid(getuid()) before calling the
**				mailer.
**			   M_HDR -- if set, the mailer wants us to
**				insert a UNIX "From" line before
**				outputing.
**			   M_NOHOST -- if set, this mailer doesn't care
**				about the host part (e.g., the local
**				mailer).
**			   M_STRIPQ -- if set, strip quote (`"')
**				characters out of parameters as you
**				transliterate them into the argument
**				vector.  For example, the local mailer
**				is called directly, so these should be
**				stripped, but the program-mailer (i.e.,
**				csh) should leave them in.
**			- an exit status to use as the code for the
**			  error message print if the mailer returns
**			  something we don't understand.
**			- A list of names that are to be considered
**			  "local" (and hence are stripped off) for
**			  this mailer.
**			- An argument vector to be passed to the
**			  mailer with the following substitutions:
**			   $f - the from person name.
**			   $u - the target user name.
**			   $h - the target user host.
**			   $c - the hop count.
**			>>>>>>>>>> Entry zero must be for the local
**			>> NOTE >> mailer and entry one must be for
**			>>>>>>>>>> the shell.
**		ParseTab -- a table driving the parsing process.  Each
**			entry contains:
**			- a character that will trigger this entry.
**			- an index into the Mailer table.
**			- a word of flags, described in dlvrmail.h.
**			- an argument.  If we have P_MAP, it is the
**			  character to turn the trigger character into.
**			  If we have P_MOVE, it is the site to send it
**			  to, using the mailer specified above.
**			This table will almost certainly have to be
**			changed on your site if you have anything more
**			than the UUCP net.
*/




static char SccsId[] = "@(#)conf.c	1.10	10/21/80";

/************ BEGIN CONFIGURATION SECTION ************/

bool	UseMsgId = FALSE;	/* don't put message id's in anywhere */

# include <whoami.h>		/* definitions of machine id's at berkeley */

char	*ArpaHost = "[unknown]";
char	*MyLocName = sysname;
# define HASUUCP		/* default to having UUCP net */
char	*UucpLocal[] = { sysname, NULL };
/* if you define HASARPA you must include a declaration for ArpaLocal */

/************ END CONFIGURATION SECTION ************/

# ifndef HASARPA
# define ArpaLocal	NULL
# endif HASARPA

# ifndef HASUUCP
# define UucpLocal	NULL
# endif HASUUCP

# ifndef HASBERK
# define BerkLocal	NULL
# endif HASBERK


struct mailer Mailer[] =
{
	/* local mail -- must be #0 */
	{
# ifdef NETV6MAIL
		"/usr/net/bin/v6mail",
# else
		"/bin/mail",
# endif
		M_ROPT|M_NOHOST|M_STRIPQ,	EX_NOUSER,	NULL,
		{ "...local%mail", "-d", "$u", NULL }
	},
	/* pipes through programs -- must be #1 */
	{
		"/bin/csh",
		M_HDR|M_NOHOST,			EX_UNAVAILABLE,	NULL,
		{ "...prog%mail", "-fc", "$u", NULL }
	},
	/* local berkeley mail */
	{
		"/usr/net/bin/sendberkmail",
		M_FOPT|M_HDR|M_STRIPQ,		EX_UNAVAILABLE,	BerkLocal,
		{ "...berk%mail", "-m", "$h", "-t", "$u", "-h", "$c", NULL }
	},
	/* arpanet mail */
	{
		"/usr/lib/mailers/arpa",
		M_STRIPQ,			0,		ArpaLocal,
		{ "...arpa%mail", "$f", "$h", "$u", NULL }
	},
	/* uucp mail (cheat & use Bell's v7 mail) */
	{
		"/bin/mail",
		M_ROPT|M_STRIPQ,		EX_NOUSER,	UucpLocal,
# ifdef DUMBMAIL
		{ "...uucp%mail", "$h!$u", NULL }
# else
		{ "...uucp%mail", "-d", "$h!$u", NULL }
# endif DUMBMAIL
	},
};

# define M_LOCAL	0
# define M_BERK		2
# define M_ARPA		3
# define M_UUCP		4



struct parsetab ParseTab[] =
{
# ifdef HASARPA
	'@',	M_ARPA,		P_HLAST|P_USR_UPPER,		NULL,
# endif HASARPA
# ifdef HASUUCP
	'^',	-1,		P_MAP,				"!",
	'!',	M_UUCP,		0,				NULL,
# endif HASUUCP
	'\0',	M_LOCAL,	P_MOVE,				"",
};
/*
**  GETNAME -- Get the current users login name.
**
**	This is in config.c because it is somewhat machine dependent.
**	Examine it carefully for your installation.
**
**	Algorithm:
**		See if the person is logged in.  If so, return
**			the name s/he is logged in as.
**		Look up the user id in /etc/passwd.  If found,
**			return that name.
**		Return NULL.
**
**	Parameters:
**		none
**
**	Returns:
**		The login name of this user.
**		NULL if this person is noone.
**
**	Side Effects:
**		none
**
**	Called By:
**		main
*/

char *
getname()
{
	register char *p;
	register struct passwd *w;
	extern struct passwd *getpwuid();
	static char namebuf[9];

# ifdef V6
	w = getpwuid(getuid() & 0377);
# else
	w = getpwuid(getuid());
# endif V6
	if (w != NULL)
	{
		strcpy(namebuf, w->pw_name);
		return (namebuf);
	}
	return (NULL);
}

# ifdef V6
/*
**  TTYPATH -- Get the path of the user's tty -- Version 6 version.
**
**	Returns the pathname of the user's tty.  Returns NULL if
**	the user is not logged in or if s/he has write permission
**	denied.
**
**	Parameters:
**		none
**
**	Returns:
**		pathname of the user's tty.
**		NULL if not logged in or write permission denied.
**
**	Side Effects:
**		none.
**
**	WARNING:
**		Return value is in a local buffer.
**
**	Called By:
**		savemail
*/

# include <sys/types.h>
# include <sys/stat.h>

char *
ttypath()
{
	struct stat stbuf;
	register int i;
	static char pathn[] = "/dev/ttyx";
	extern int errno;

	/* compute the pathname of the controlling tty */
	if ((i = ttyn(2)) == 'x' && (i = ttyn(1)) == 'x' && (i = ttyn(0)) == 'x')
	{
		errno = 0;
		return (NULL);
	}
	pathn[8] = i;

	/* see if we have write permission */
	if (stat(pathn, &stbuf) < 0 || !flagset(02, stbuf.st_mode))
	{
		errno = 0;
		return (NULL);
	}

	/* see if the user is logged in */
	if (getlogin() == NULL)
		return (NULL);

	/* looks good */
	return (pathn);
}
/*
**  FDOPEN -- Open a stdio file given an open file descriptor.
**
**	This is included here because it is standard in v7, but we
**	need it in v6.
**
**	Algorithm:
**		Open /dev/null to create a descriptor.
**		Close that descriptor.
**		Copy the existing fd into the descriptor.
**
**	Parameters:
**		fd -- the open file descriptor.
**		type -- "r", "w", or whatever.
**
**	Returns:
**		The file descriptor it creates.
**
**	Side Effects:
**		none
**
**	Called By:
**		deliver
**
**	Notes:
**		The mode of fd must match "type".
*/

FILE *
fdopen(fd, type)
	int fd;
	char *type;
{
	register FILE *f;

	f = fopen("/dev/null", type);
	close(fileno(f));
	fileno(f) = fd;
	return (f);
}
/*
**  INDEX -- Return pointer to character in string
**
**	For V7 compatibility.
**
**	Parameters:
**		s -- a string to scan.
**		c -- a character to look for.
**
**	Returns:
**		If c is in s, returns the address of the first
**			instance of c in s.
**		NULL if c is not in s.
**
**	Side Effects:
**		none.
*/

index(s, c)
	register char *s;
	register char c;
{
	while (*s != '\0')
	{
		if (*s++ == c)
			return (--s);
	}
	return (NULL);
}
# endif V6

# ifndef V6
/*
**  TTYPATH -- Get the path of the user's tty -- Version 7 version.
**
**	Returns the pathname of the user's tty.  Returns NULL if
**	the user is not logged in or if s/he has write permission
**	denied.
**
**	Parameters:
**		none
**
**	Returns:
**		pathname of the user's tty.
**		NULL if not logged in or write permission denied.
**
**	Side Effects:
**		none.
**
**	WARNING:
**		Return value is in a local buffer.
**
**	Called By:
**		savemail
*/

# include <sys/types.h>
# include <sys/stat.h>

char *
ttypath()
{
	struct stat stbuf;
	register char *pathn;
	extern int errno;
	extern char *ttyname();

	/* compute the pathname of the controlling tty */
	if ((pathn = ttyname(2)) == NULL && (pathn = ttyname(1)) == NULL && (pathn = ttyname(0)) == NULL)
	{
		errno = 0;
		return (NULL);
	}

	/* see if we have write permission */
	if (stat(pathn, &stbuf) < 0 || !flagset(02, stbuf.st_mode))
	{
		errno = 0;
		return (NULL);
	}

	/* see if the user is logged in */
	if (getlogin() == NULL)
		return (NULL);

	/* looks good */
	return (pathn);
}
# endif V6
er char *pathn;
	extern int errno;
	extern char *ttyname();

	/* compute the pathname of the controlling tty */
	if ((pathn = ttyname(2)) == NULL && (pathn = ttyname(1)) == NULL && (pathn = ttyname(0)) == NULL)
	{
		errno = 0;
		return (NULL);
	}

	/* see if we have write permission */
	if (stat(padelivermail/deliver.c                                                                                  660      17       2        36537  3127450635  10173                                                                                                                                                                                                                                                                                                                                                                      # include <stdio.h>
# include <pwd.h>
# include <signal.h>
# include <sys/file.h>
# include "dlvrmail.h"
# ifdef LOG
# include <log.h>
# endif LOG

static char SccsId[] = "@(#)deliver.c	2.5	2/5/81";

/*
**  DELIVER -- Deliver a message to a particular address.
**
**	Algorithm:
**		Compute receiving network (i.e., mailer), host, & user.
**		If local, see if this is really a program name.
**		Build argument for the mailer.
**		Create pipe through edit fcn if appropriate.
**		Fork.
**			Child: call mailer
**		Parent: call editfcn if specified.
**		Wait for mailer to finish.
**		Interpret exit status.
**
**	Parameters:
**		to -- the address to deliver the message to.
**		editfcn -- if non-NULL, we want to call this function
**			to output the letter (instead of just out-
**			putting it raw).
**
**	Returns:
**		zero -- successfully delivered.
**		else -- some failure, see ExitStat for more info.
**
**	Side Effects:
**		The standard input is passed off to someone.
**
**	WARNING:
**		The standard input is shared amongst all children,
**		including the file pointer.  It is critical that the
**		parent waits for the child to finish before forking
**		another child.
**
**	Called By:
**		main
**		savemail
**
**	Files:
**		standard input -- must be opened to the message to
**			deliver.
*/

deliver(to, editfcn)
	addrq *to;
	int (*editfcn)();
{
	register struct mailer *m;
	char *host;
	char *user;
	extern struct passwd *getpwnam();
	char **pvp;
	extern char **buildargv();
	auto int st;
	register int i;
	register char *p;
	int pid;
	int pvect[2];
	extern FILE *fdopen();
	extern int errno;
	FILE *mfile;
	extern putheader();
	extern pipesig();
	extern bool GotHdr;
	extern char *index();

	/*
	**  Compute receiving mailer, host, and to addreses.
	**	Do some initialization first.  To is the to address
	**	for error messages.
	*/

	To = to->q_paddr;
	m = to->q_mailer;
	user = to->q_user;
	host = to->q_host;
	Errors = 0;
	errno = 0;
# ifdef DEBUG
	if (Debug)
		printf("deliver(%s [%d, `%s', `%s'])\n", To, m, host, user);
# endif DEBUG

	/*
	**  Remove quote bits from user/host.
	*/

	for (p = user; (*p++ &= 0177) != '\0'; )
		continue;
	if (host != NULL)
		for (p = host; (*p++ &= 0177) != '\0'; )
			continue;
	
	/*
	**  Strip quote bits from names if the mailer wants it.
	*/

	if (flagset(M_STRIPQ, m->m_flags))
	{
		stripquotes(user);
		stripquotes(host);
	}

	/*
	**  See if this user name is "special".
	**	If the user is a program, diddle with the mailer spec.
	**	If the user name has a slash in it, assume that this
	**		is a file -- send it off without further ado.
	**		Note that this means that editfcn's will not
	**		be applied to the message.
	*/

	if (m == &Mailer[0])
	{
		if (*user == '|')
		{
			user++;
			m = &Mailer[1];
		}
		else
		{
			if (index(user, '/') != NULL)
			{
				i = mailfile(user);
				giveresponse(i, TRUE, m);
				return (i);
			}
		}
	}

	/*
	**  See if the user exists.
	**	Strictly, this is only needed to print a pretty
	**	error message.
	**
	**	>>>>>>>>>> This clause assumes that the local mailer
	**	>> NOTE >> cannot do any further aliasing; that
	**	>>>>>>>>>> function is subsumed by delivermail.
	*/

	if (m == &Mailer[0])
	{
		if (getpwnam(user) == NULL)
		{
			giveresponse(EX_NOUSER, TRUE, m);
			return (EX_NOUSER);
		}
	}

	/*
	**  If the mailer wants a From line, insert a new editfcn.
	*/

	if (flagset(M_HDR, m->m_flags) && editfcn == NULL && (!GotHdr || flagset(M_FHDR, m->m_flags)))
		editfcn = putheader;

	/*
	**  Call the mailer.
	**	The argument vector gets built, pipes through 'editfcn'
	**	are created as necessary, and we fork & exec as
	**	appropriate.  In the parent, we call 'editfcn'.
	*/

	pvp = buildargv(m->m_argv, m->m_flags, host, user, From.q_paddr);
	if (pvp == NULL)
	{
		usrerr("name too long");
		return (-1);
	}
	rewind(stdin);

	/* create a pipe if we will need one */
	if (editfcn != NULL && pipe(pvect) < 0)
	{
		syserr("pipe");
		return (-1);
	}
# ifdef VFORK
	pid = vfork();
# else
	pid = fork();
# endif
	if (pid < 0)
	{
		syserr("Cannot fork");
		if (editfcn != NULL)
		{
			close(pvect[0]);
			close(pvect[1]);
		}
		return (-1);
	}
	else if (pid == 0)
	{
		/* child -- set up input & exec mailer */
		/* make diagnostic output be standard output */
		close(2);
		dup(1);
		signal(SIGINT, SIG_IGN);
		if (editfcn != NULL)
		{
			close(0);
			if (dup(pvect[0]) < 0)
			{
				syserr("Cannot dup to zero!");
				_exit(EX_OSERR);
			}
			close(pvect[0]);
			close(pvect[1]);
		}
		if (!flagset(M_RESTR, m->m_flags))
			setuid(getuid());
# ifdef LOG
		closelog();
# endif LOG
# ifndef VFORK
		/*
		 * We have to be careful with vfork - we can't mung up the
		 * memory but we don't want the mailer to inherit any extra
		 * open files.  Chances are the mailer won't
		 * care about an extra file, but then again you never know.
		 * Actually, we would like to close(fileno(pwf)), but it's
		 * declared static so we can't.  But if we fclose(pwf), which
		 * is what endpwent does, it closes it in the parent too and
		 * the next getpwnam will be slower.  If you have a weird mailer
		 * that chokes on the extra file you should do the endpwent().
		 */
		endpwent();
# endif
		execv(m->m_mailer, pvp);
		/* syserr fails because log is closed */
		/* syserr("Cannot exec %s", m->m_mailer); */
		printf("Cannot exec %s\n", m->m_mailer);
		fflush(stdout);
		_exit(EX_UNAVAILABLE);
	}

	/* arrange to write out header message if error */
	if (editfcn != NULL)
	{
		close(pvect[0]);
		signal(SIGPIPE, pipesig);
		mfile = fdopen(pvect[1], "w");
		(*editfcn)(mfile);
		fclose(mfile);
	}

	/*
	**  Wait for child to die and report status.
	**	We should never get fatal errors (e.g., segmentation
	**	violation), so we report those specially.  For other
	**	errors, we choose a status message (into statmsg),
	**	and if it represents an error, we print it.
	*/

	while ((i = wait(&st)) > 0 && i != pid)
		continue;
	if (i < 0)
	{
		syserr("wait");
		return (-1);
	}
	if ((st & 0377) != 0)
	{
		syserr("%s: stat %o", pvp[0], st);
		ExitStat = EX_UNAVAILABLE;
		return (-1);
	}
	i = (st >> 8) & 0377;
	giveresponse(i, TRUE, m);
	return (i);
}
/*
**  GIVERESPONSE -- Interpret an error response from a mailer
**
**	Parameters:
**		stat -- the status code from the mailer (high byte
**			only; core dumps must have been taken care of
**			already).
**		force -- if set, force an error message output, even
**			if the mailer seems to like to print its own
**			messages.
**		m -- the mailer descriptor for this mailer.
**
**	Returns:
**		none.
**
**	Side Effects:
**		Errors may be incremented.
**		ExitStat may be set.
**
**	Called By:
**		deliver
*/

giveresponse(stat, force, m)
	int stat;
	int force;
	register struct mailer *m;
{
	register char *statmsg;
	extern char *SysExMsg[];
	register int i;
	extern int N_SysEx;
	extern long MsgSize;
	char buf[30];

	i = stat - EX__BASE;
	if (i < 0 || i > N_SysEx)
		statmsg = NULL;
	else
		statmsg = SysExMsg[i];
	if (stat == 0)
		statmsg = "ok";
	else
	{
		Errors++;
		if (statmsg == NULL && m->m_badstat != 0)
		{
			stat = m->m_badstat;
			i = stat - EX__BASE;
# ifdef DEBUG
			if (i < 0 || i >= N_SysEx)
				syserr("Bad m_badstat %d", stat);
			else
# endif DEBUG
			statmsg = SysExMsg[i];
		}
		if (statmsg == NULL)
			usrerr("unknown mailer response %d", stat);
		else if (force || !flagset(M_QUIET, m->m_flags))
			usrerr("%s", statmsg);
	}

	/*
	**  Final cleanup.
	**	Log a record of the transaction.  Compute the new
	**	ExitStat -- if we already had an error, stick with
	**	that.
	*/

	if (statmsg == NULL)
	{
		sprintf(buf, "error %d", stat);
		statmsg = buf;
	}

# ifdef LOG
	logmsg(LOG_INFO, "%s->%s: %ld: %s", From.q_paddr, To, MsgSize, statmsg);
# endif LOG
	setstat(stat);
	return (stat);
}
/*
**  PUTHEADER -- insert the From header into some mail
**
**	For mailers such as 'msgs' that want the header inserted
**	into the mail, this edit filter inserts the From line and
**	then passes the rest of the message through.
**
**	Parameters:
**		fp -- the file pointer for the output.
**
**	Returns:
**		none
**
**	Side Effects:
**		Puts a "From" line in UNIX format, and then
**			outputs the rest of the message.
**
**	Called By:
**		deliver
*/

putheader(fp)
	register FILE *fp;
{
	char buf[MAXLINE + 1];
	long tim;
	extern char *ctime();
	register char *p;
	extern char *index();

	/* output the header part */
	fgets(buf, sizeof buf, stdin);
	if (strncmp(buf, "From ", 5) != 0 || (p = index(&buf[5], ' ')) == NULL)
	{
		time(&tim);
		fprintf(fp, "From %s %s", From.q_paddr, ctime(&tim));
		fputs(buf, fp);
	}
	else
		fprintf(fp, "From %s %s", From.q_paddr, &p[1]);

	/* output the body */
	while (!ferror(fp) && fgets(buf, sizeof buf, stdin) != NULL)
		fputs(buf, fp);
	if (ferror(fp))
	{
		syserr("putheader: write error");
		setstat(EX_IOERR);
	}
}
/*
**  PIPESIG -- Handle broken pipe signals
**
**	This just logs an error.
**
**	Parameters:
**		none
**
**	Returns:
**		none
**
**	Side Effects:
**		logs an error message.
*/

pipesig()
{
	syserr("Broken pipe");
	signal(SIGPIPE, SIG_IGN);
}
/*
**  SENDTO -- Designate a send list.
**
**	The parameter is a comma-separated list of people to send to.
**	This routine arranges to send to all of them.
**
**	Parameters:
**		list -- the send list.
**		copyf -- the copy flag; passed to parse.
**
**	Returns:
**		none
**
**	Side Effects:
**		none.
**
**	Called By:
**		main
**		alias
*/

sendto(list, copyf)
	char *list;
	int copyf;
{
	register char *p;
	register char *q;
	register char c;
	addrq *a;
	extern addrq *parse();
	bool more;

	/* more keeps track of what the previous delimiter was */
	more = TRUE;
	for (p = list; more; )
	{
		/* find the end of this address */
		q = p;
		while ((c = *p++) != '\0' && c != ',' && c != '\n')
			continue;
		more = c != '\0';
		*--p = '\0';
		if (more)
			p++;

		/* parse the address */
		if ((a = parse(q, (addrq *) NULL, copyf)) == NULL)
			continue;

		/* arrange to send to this person */
		recipient(a, &SendQ);
	}
	To = NULL;
}
/*
**  RECIPIENT -- Designate a message recipient
**
**	Saves the named person for future mailing.
**
**	Designates a person as a recipient.  This routine
**	does the initial parsing, and checks to see if
**	this person has already received the mail.
**	It also supresses local network names and turns them into
**	local names.
**
**	Parameters:
**		a -- the (preparsed) address header for the recipient.
**		targetq -- the queue to add the name to.
**
**	Returns:
**		none.
**
**	Side Effects:
**		none.
**
**	Called By:
**		sendto
**		main
*/

recipient(a, targetq)
	register addrq *a;
	addrq *targetq;
{
	register addrq *q;
	register struct mailer *m;
	register char **pvp;
	extern char *xalloc();
	extern bool forward();
	extern int errno;
	extern bool sameaddr();

	To = a->q_paddr;
	m = a->q_mailer;
	errno = 0;
# ifdef DEBUG
	if (Debug)
		printf("recipient(%s)\n", To);
# endif DEBUG

	/*
	**  Look up this person in the recipient list.  If they
	**  are there already, return, otherwise continue.
	*/

	if (!ForceMail)
	{
		for (q = &SendQ; (q = nxtinq(q)) != NULL; )
			if (sameaddr(q, a, FALSE))
			{
# ifdef DEBUG
				if (Debug)
					printf("(%s in SendQ)\n", a->q_paddr);
# endif DEBUG
				return;
			}
		for (q = &AliasQ; (q = nxtinq(q)) != NULL; )
			if (sameaddr(q, a, FALSE))
			{
# ifdef DEBUG
				if (Debug)
					printf("(%s in AliasQ)\n", a->q_paddr);
# endif DEBUG
				return;
			}
	}

	/*
	**  See if the user wants hir mail forwarded.
	**	`Forward' must do the forwarding recursively.
	*/

	if (m == &Mailer[0] && !NoAlias && targetq == &SendQ && forward(a))
		return;

	/*
	**  Put the user onto the target queue.
	*/

	if (targetq != NULL)
	{
		putonq(a, targetq);
	}

	return;
}
/*
**  BUILDARGV -- Build an argument vector for a mail server.
**
**	Using a template defined in config.c, an argv is built.
**	The format of the template is already a vector.  The
**	items of this vector are copied, unless a dollar sign
**	is encountered.  In this case, the next character
**	specifies something else to copy in.  These can be
**		$f	The from address.
**		$h	The host.
**		$u	The user.
**		$c	The hop count.
**	The vector is built in a local buffer.  A pointer to
**	the static argv is returned.
**
**	Parameters:
**		tmplt -- a template for an argument vector.
**		flags -- the flags for this server.
**		host -- the host name to send to.
**		user -- the user name to send to.
**		from -- the person this mail is from.
**
**	Returns:
**		A pointer to an argv.
**
**	Side Effects:
**		none
**
**	WARNING:
**		Since the argv is staticly allocated, any subsequent
**		calls will clobber the old argv.
**
**	Called By:
**		deliver
*/

char **
buildargv(tmplt, flags, host, user, from)
	char **tmplt;
	int flags;
	char *host;
	char *user;
	char *from;
{
	register char *p;
	register char *q;
	static char *pv[MAXPV+1];
	char **pvp;
	char **mvp;
	static char buf[512];
	register char *bp;
	char pbuf[30];

	/*
	**  Do initial argv setup.
	**	Insert the mailer name.  Notice that $x expansion is
	**	NOT done on the mailer name.  Then, if the mailer has
	**	a picky -f flag, we insert it as appropriate.  This
	**	code does not check for 'pv' overflow; this places a
	**	manifest lower limit of 4 for MAXPV.
	*/

	pvp = pv;
	bp = buf;

	*pvp++ = tmplt[0];

	/* insert -f or -r flag as appropriate */
	if (flagset(M_FOPT|M_ROPT, flags) && FromFlag)
	{
		if (flagset(M_FOPT, flags))
			*pvp++ = "-f";
		else
			*pvp++ = "-r";
		*pvp++ = From.q_paddr;
	}

	/*
	**  Build the rest of argv.
	**	For each prototype parameter, the prototype is
	**	scanned character at a time.  If a dollar-sign is
	**	found, 'q' is set to the appropriate expansion,
	**	otherwise it is null.  Then either the string
	**	pointed to by q, or the original character, is
	**	interpolated into the buffer.  Buffer overflow is
	**	checked.
	*/

	for (mvp = tmplt; (p = *++mvp) != NULL; )
	{
		if (pvp >= &pv[MAXPV])
		{
			syserr("Too many parameters to %s", pv[0]);
			return (NULL);
		}
		*pvp++ = bp;
		for (; *p != '\0'; p++)
		{
			/* q will be the interpolated quantity */
			q = NULL;
			if (*p == '$')
			{
				switch (*++p)
				{
				  case 'f':	/* from person */
					q = from;
					break;

				  case 'u':	/* user */
					q = user;
					break;

				  case 'h':	/* host */
					q = host;
					break;

				  case 'c':	/* hop count */
					sprintf(pbuf, "%d", HopCount);
					q = pbuf;
					break;
				}
			}

			/*
			**  Interpolate q or output one character
			**	Strip quote bits as we proceed.....
			*/

			if (q != NULL)
			{
				while (bp < &buf[sizeof buf - 1] && (*bp++ = *q++) != '\0')
					continue;
				bp--;
			}
			else if (bp < &buf[sizeof buf - 1])
				*bp++ = *p;
		}
		*bp++ = '\0';
		if (bp >= &buf[sizeof buf - 1])
			return (NULL);
	}
	*pvp = NULL;

# ifdef DEBUG
	if (Debug)
	{
		printf("Interpolated argv is:\n");
		for (mvp = pv; *mvp != NULL; mvp++)
			printf("\t%s\n", *mvp);
	}
# endif DEBUG

	return (pv);
}
/*
**  MAILFILE -- Send a message to a file.
**
**	Parameters:
**		filename -- the name of the file to send to.
**
**	Returns:
**		The exit code associated with the operation.
**
**	Side Effects:
**		none.
**
**	Called By:
**		deliver
*/

mailfile(filename)
	char *filename;
{
	char buf[MAXLINE];
	register FILE *f;
	auto long tim;
	extern char *ctime();

	if (access(filename, FACCESS_WRITE) < 0)
		return (EX_CANTCREAT);
	f = fopen(filename, "a");
	if (f == NULL)
		return (EX_CANTCREAT);
	
	/* output the timestamp */
	time(&tim);
	fprintf(f, "From %s %s", From.q_paddr, ctime(&tim));
	rewind(stdin);
	while (fgets(buf, sizeof buf, stdin) != NULL)
	{
		fputs(buf, f);
		if (ferror(f))
		{
			fclose(f);
			return (EX_IOERR);
		}
	}
	fputs("\n", f);
	fclose(f);
	return (EX_OK);
}
me;
{
	char buf[MAXLINE];
	register FILE *f;
	auto long tim;
	extern char *ctime();

	if (access(filename, FACCESS_WRITE) < 0)
		return (EX_CANTCREAT);
	f = fopedelivermail/delivermail.8                                                                              660      17       2        12274  2470111550  10741                                                                                                                                                                                                                                                                                                                                                                      .TH DELIVERMAIL 8
.SH NAME
delivermail \- deliver mail to arbitrary people
.SH SYNOPSIS
.B /etc/delivermail
[
.BR \- [ fr ]
.I address
] [
.B \-a
] [
.BR \-e [ empqw ]
] [
.B \-n
] [
.B \-m
] [
.B \-s
] [
.B \-i
] [
.B \-h
.I N
] address ...
.SH DESCRIPTION
.I Delivermail
delivers a letter to one or more people,
routing the letter over whatever networks
are necessary.
.I Delivermail
will do inter-net forwarding as necessary
to deliver the mail to the correct place.
.PP
.I Delivermail
is not intended as a user interface routine;
it is expected that other programs will provide user-friendly
front ends,
and
.I delivermail
will be used only to deliver pre-formatted messages.
.PP
.I Delivermail
reads its standard input
up to a control-D
or a single dot
and sends a copy of the letter found there
to all of the addresses listed.
If the
.B \-i
flag is given,
single dots are ignored.
It determines the network to use
based on the syntax of the addresses.
Addresses containing the character `@'
or the word ``at''
are sent to the
.SM ARPANET;
addresses containing `!'
are sent to the
.SM UUCP
net,
and addresses containing `:' or `.'
are sent to the Berkeley network.
Other addresses are assumed to be local.
.PP
Local addresses are looked up in the file
.I /usr/lib/mailaliases
and aliased appropriately.
Aliasing can be prevented by preceeding the address
with a backslash or using the
.B \-n
flag.
Normally the sender is not included in any alias
expansions, e.g.,
if `john' sends to `group',
and `group' includes `john' in the expansion,
then the letter will not be delivered to `john'.
The
.B \-m
flag disables this suppression.
.PP
.I Delivermail
computes the person sending the mail
by looking at your login name.
The ``from'' person
can be explicitly specified by using the
.B \-f
flag;
or, if the
.B \-a
flag is given,
delivermail looks in the body of the message
for a ``From:'' or ``Sender:''
field in
.SM ARPANET
format.
The
.B \-f
and
.B \-a
flags can be used
only by the special users
.I root
and
.I network,
or if the person you are trying to become
is the same as the person you are.
The
.B \-r
flag is entirely equivalent to the
.B \-f
flag; it is provided for ease of interface only.
.PP
The
.BI \-e x
flag controls the disposition of error output,
as follows:
.TP 3n
.B e
Print errors on the standard output,
and echo a copy of the message when done.
It is assumed that a network server will
return the message back to the user.
.TP
.B m
Mail errors back to the user.
.TP
.B p
Print errors on the standard output.
.TP
.B q
Throw errors away;
only exit status is returned.
.TP
.B w
Write errors back to the user's terminal,
but only if the user is still logged in
and write permission is enabled;
otherwise errors are mailed back.
.LP
If the error is not mailed back,
and if the mail originated on the machine where the error occurred,
the letter is appended to the file
.I dead.letter
in the sender's home directory.
.PP
If the first character of the user name
is a vertical bar,
the rest of the user name is used as the name of a program
to pipe the mail to.
It may be necessary to quote the name of the user
to keep
.I delivermail
from supressing the blanks from between arguments.
.PP
The message is normally editted to eliminate ``From''
lines that might confuse other mailers.
In particular,
``From'' lines in the header are deleted,
and ``From'' lines in the body are prepended by `>'.
The
.B \-s
flag saves ``From'' lines in the header.
.PP
The
.B \-h
flag gives a ``hop-count'', i.e.,
a measure of how many times this message
has been processed by
.I delivermail
(presumably on different machines).
Each time
.I delivermail
processes a message,
it increases the hop-count by one;
if it exceeds 30
.I delivermail
assumes that an alias loop has occured
and it aborts the message.
The hop-count defaults to zero.
.PP
.I Delivermail
returns an exit status
describing what it did.
The codes are defined in
.IR mailexits.h :
.ta 3n +\w'EX_UNAVAILABLE'u+3n
.de XX
.ti \n(.iu
..
.in +\w'EX_UNAVAILABLE'u+6n
.XX
0	EX_OK	Succesful completion on all addresses.
.XX
2	EX_NOUSER	User name not recognized.
.XX
3	EX_UNAVAILABLE	Catchall meaning necessary resources
were not available.
.XX
4	EX_SYNTAX	Syntax error in address.
.XX
5	EX_SOFTWARE	Internal software error,
including bad arguments.
.XX
6	EX_OSERR	Temporary operating system error,
such as ``cannot fork''.
.XX
7	EX_NOHOST	Host name not recognized.
.DT
.PP
.SH FILES
.in +10
.ti -10
/usr/lib/mailaliases \- to alias names
.ti -10
/bin/mail \- to deliver local mail
.ti -10
/usr/net/bin/sendmail \- to deliver Berkeley mail
.ti -10
/usr/lib/mailers/arpa \- to deliver
.SM ARPANET
mail
.ti -10
/usr/lib/mailers/uucp \- to deliver
.SM UUCP
mail
.ti -10
/tmp/mail* \- temp file
.ti -10
/tmp/xscript* \- saved transcript
.ti -10
/dev/log \- to log status (optional)
.in -10
.SH SEE\ ALSO
mail(1), Mail(UCB), arpa-mailer(8), uucp-mailer(8),
mailaliases(5), userinfo(5)
.SH BUGS
.I Delivermail
sends one copy of the letter
to each user;
it should send one copy
of the letter to each host
and distribute to multiple users there
whenever possible.
.PP
.I Delivermail
assumes the addresses can be represented as one word.
This is incorrect according to the
.SM ARPANET
mail protocol RFC 733 (NIC 41952),
but is consistant with the real world.
10
/dev/log \- to log status (optional)
.in -10
.SH SEE\ ALSO
mail(1), Mail(UCB), arpa-mailer(8), uucp-mailer(8),
mailaliases(5), userinfo(5)
.SH BUGS
.I Delivermail
sends one copy of the letter
to each user;
it should send one copy
of the letter to each host
and distribute to multiple users there
whenever possible.
.PP
.Idelivermail/dlvrmail.h                                                                                 660      17       2        10372  4025451666  10350                                                                                                                                                                                                                                                                                                                                                                      /*
**  DLVRMAIL.H -- Global definitions for delivermail.
**
**	Most of these are actually allocated in globals.c
**
**	@(#)dlvrmail.h	2.2	11/21/80
*/




# include "useful.h"

/*
**  Manifest constants.
*/

# define MAXLINE	256	/* maximum line length */
# define MAXNAME	128	/* maximum length of a name */
# define MAXFIELD	2500	/* maximum total length of a header field */
# define MAXPV		15	/* maximum # of parms to mailers */
# define MAXHOP		30	/* maximum value of HopCount */
# define ALIASFILE	"/usr/lib/aliases"	/* location of alias file */





/*
**  Mailer definition structure.
**	Every mailer known to the system is declared in this
**	structure.  It defines the pathname of the mailer, some
**	flags associated with it, and the argument vector to
**	pass to it.  The flags are defined in conf.c
**
**	The argument vector is expanded before actual use.  Every-
**	thing is passed through except for things starting with "$".
**	"$x" defines some interpolation, as described in conf.c
**	"$x" where x is unknown expands to "x", so use "$$" to get "$".
*/

struct mailer
{
	char	*m_mailer;	/* pathname of the mailer to use */
	short	m_flags;	/* status flags, see below */
	short	m_badstat;	/* the status code to use on unknown error */
	char	**m_local;	/* list of local names for this host */
	char	*m_argv[MAXPV];	/* template argument vector */
};

# define M_FOPT		0001	/* mailer takes picky -f flag */
# define M_ROPT		0002	/* mailer takes picky -r flag */
# define M_QUIET	0004	/* don't print error on bad status */
# define M_RESTR	0010	/* must be daemon to execute */
# define M_HDR		0020	/* insert From line */
# define M_NOHOST	0040	/* ignore host in comparisons */
# define M_STRIPQ	0100	/* strip quote characters from user/host */
# define M_FHDR		0200	/* force good From line */

extern struct mailer Mailer[];


/*
**  Address structure.
**	Addresses are stored internally in this structure.
*/

struct address
{
	char		*q_paddr;	/* the printname for the address */
	char		*q_user;	/* user name */
	char		*q_host;	/* host name */
	struct mailer	*q_mailer;	/* mailer to use */
	struct address	*q_next;	/* chain */
	struct address	*q_prev;	/* back pointer */
};

typedef struct address addrq;

/* some other primitives */
# define nxtinq(q)	((q)->q_next)
# define clearq(q)	(q)->q_next = (q)->q_prev = NULL

extern addrq SendQ;		/* queue of people to send to */
extern addrq AliasQ;		/* queue of people that are aliases */


/*
**  Parse structure.
**	This table drives the parser which determines the network
**	to send the mail to.
*/

struct parsetab
{
	char	p_char;		/* trigger character */
	char	p_mailer;	/* the index of the mailer to call */
	short	p_flags;	/* see below */
	char	*p_arg;		/* extra info needed for some flags */
};

# define P_MAP		0001	/* map p_char -> p_arg[0] */
# define P_HLAST	0002	/* host is last, & right associative */
# define P_ONE		0004	/* can only be one p_char in addr */
# define P_MOVE		0010	/* send untouched to host p_arg */
# define P_USR_UPPER	0020	/* don't map UPPER->lower in user names */
# define P_HST_UPPER	0040	/* don't map UPPER->lower in host names */




/*
**  Global variables.
*/

extern bool	ArpaFmt;	/* if set, message is in arpanet fmt */
extern bool	FromFlag;	/* if set, "From" person is explicit */
extern bool	Debug;		/* if set, debugging info */
extern bool	MailBack;	/* mail back response on error */
extern bool	BerkNet;	/* called from BerkNet */
extern bool	WriteBack;	/* write back response on error */
extern bool	NoAlias;	/* if set, don't do any aliasing */
extern bool	ForceMail;	/* if set, mail even if already got a copy */
extern bool	MeToo;		/* send to the sender also */
extern bool	UseMsgId;	/* put msg-id's in all msgs [conf.c] */
extern bool	IgnrDot;	/* don't let dot end messages */
extern bool	SaveFrom;	/* save leading "From" lines */
extern int	Errors;		/* set if errors */
extern int	ExitStat;	/* exit status code */
extern char	InFileName[];	/* input file name */
extern char	Transcript[];	/* the transcript file name */
extern char	MsgId[];	/* the message id for this message */
extern addrq	From;		/* the person it is from */
extern char	*To;		/* the target person */
extern int	HopCount;	/* hop count */


# include	<sysexits.h>

# define flagset(bits, word)	((bits) & (word))
# define setstat(s)		{ if (ExitStat == EX_OK) ExitStat = s; }
/
extern int	Errors;		/* set if errors */
extern int	ExitStat;	/* exit status code */
extern char	InFileName[];	/* input file name */
extern char	Transcript[];	/* the transcript file name */
extern char	MsgId[];	/* the message id for this message */
extern addrqdelivermail/err.c                                                                                      660      17       2         3137  2470111554   7271                                                                                                                                                                                                                                                                                                                                                                      # include <stdio.h>
# include "dlvrmail.h"
# ifdef LOG
# include <log.h>
# endif LOG

static char	SccsId[] = "@(#)err.c	2.2	1/10/81";

/*
**  SYSERR -- Print error message.
**
**	Prints an error message via printf to the diagnostic
**	output.  If LOG is defined, it logs it also.
**
**	Parameters:
**		f -- the format string
**		a, b, c, d, e -- parameters
**
**	Returns:
**		-1 always
**
**	Side Effects:
**		increments Errors.
**		sets ExitStat.
*/

/*VARARGS1*/
syserr(fmt, a, b, c, d, e)
	char *fmt;
{
	extern int errno;
	static char errbuf[MAXLINE+1];
	register char *p;
	extern char *sys_errlist[];
	extern int sys_nerr;

	sprintf(errbuf, fmt, a, b, c, d, e);
	if (errno != 0)
	{
		p = &errbuf[strlen(errbuf)];
		if (errno < sys_nerr && errno > 0)
			sprintf(p, ": %s", sys_errlist[errno]);
		else
			sprintf(p, ": error %d", errno);
	}
	printf("delivermail: %s\n", errbuf);
	fflush(stdout);
	Errors++;

	/* determine exit status if not already set */
	if (ExitStat == EX_OK)
	{
		if (errno == 0)
			ExitStat = EX_SOFTWARE;
		else
			ExitStat = EX_OSERR;
	}

# ifdef LOG
	logmsg(LOG_ERR, "%s->%s: %s", From.q_paddr, To, errbuf);
# endif LOG
	errno = 0;
	return (-1);
}
/*
**  USRERR -- Signal user error.
**
**	This is much like syserr except it is for user errors.
**
**	Parameters:
**		fmt, a, b, c, d -- printf strings
**
**	Returns:
**		-1
**
**	Side Effects:
**		increments Errors.
*/

/*VARARGS1*/
usrerr(fmt, a, b, c, d, e)
	char *fmt;
{
	extern char SuprErrs;

	if (SuprErrs)
		return;

	Errors++;
	if (To != NULL)
		printf("%s... ", To);
	printf(fmt, a, b, c, d, e);
	printf("\n");
	fflush(stdout);
	return (-1);
}
r, To, errbuf);
# endif LOG
	errno = 0;
	return (-1);
}
/*
**  USRERR -- Signal user error.
**
**	This is much like syserr except it is for user errors.
**
**	Parameters:
**		fmt, a, b, c, d -- printf strings
**
**	Returns:
**		-1
**
**	Side Effects:
**		increments Errors.
*/

/*VARARGS1*/
usrerr(fmt, a, b, c, d, e)
	char *fmt;
{
	extern char SuprErrs;

	if (SuprErrs)
		return;

	Errors++;
	if (To != NULL)
		prindelivermail/logmsg.3                                                                                   660      17       2         3326  2470111555   7712                                                                                                                                                                                                                                                                                                                                                                      .TH LOGMSG 3 12/31/79
.SH NAME
logmsg \- log a message on the log device
.SH SYNOPSIS
# include <log.h>

logmsg(pri, fmt, args)
.br
char *fmt;

initlog(name, flags, mode)
.br
char *name;
.SH DESCRIPTION
.I Logmsg
acts like a simple printf
into a log file.
Formats are limited to
``%s'', ``%d'', ``%c'', and ``%o'';
no modifiers
(such as ``%.4s'')
are allowed.
In addition,
``%m'' will print out the value of
.I errno
in symbolic form.
.I Logmsg
will append a newline to the message
if there is not one already,
and writes the message in one physical write
to keep it from being fragmented by the system.
.PP
Messages have priorities between one and nine,
specified by
.I pri.
Legal priorities are described in syslog(8).
.PP
The
.I mode
parameter describes where to log the output.
.IP LOG_NULL \w'LOG_CLOSE'u+2n
Don't change anything.
This will cause the log to be output
on file descriptor 2.
.IP LOG_SOUT
Merges standard and diagnostic outputs
and logs on the diagnostic output.
This should only be used by programs
that have
.I only
log output \- such as daemons.
.IP LOG_DIAG
Opens the diagnostic output to be the log device
and logs there.
.IP LOG_INDEP
Opens a seperate file descriptor for the log.
.IP LOG_CLOSE
Closes the log.
.PP
The
.I flags
parameter arranges for special features.
These options can be ORed together.
.IP LOG_PID \w'LOG_COOLIT'u+2n
Log the process id with each message.
.IP LOG_TIME
Log the current time-of-day with each message.
Since
.I syslog
does this automatically,
this is only useful when outputing directly to a terminal
or file.
(Actually, this is not currently supported.)
.IP LOG_COOLIT
Supresses output of priority tags on messages.
(Not supported.)
.SH FILES
/dev/log \- the log file
.SH SEE\ ALSO
syslog(8)
P
The
.I flags
parameter arranges for special features.
These options can be ORed together.
.IP LOG_PID \w'LOG_COOLIT'u+2n
Log the process id with each message.
.IP LOG_TIME
Log the current time-of-day with each message.
Since
.I syslog
does this automatically,
this is only useful when outputing ddelivermail/mail-dm.c                                                                                  660      17       2        20061  2470111560  10031                                                                                                                                                                                                                                                                                                                                                                      #include <arpa/netopen.h>
#include "srvrftp.h"
#include <statbuf.h>
#include <arpa/hostnames.h>
#include <io_buf.h>
#include <arpa/mail.h>
#include <ident.h>
#include <signal.h>
#include <log.h>
extern int fout;
/*
Name:
	mail

Function:
	handle the MAIL <user> command over the command connection

Algorithm:
	see if we have a known user

	if mailbox file can't be gotten
		return
	tell him it is ok to go ahead with mail

	while he doesn't type a period
		read and write data
	say completed

Parameters:
	username in arg

Returns:
	nothing

Globals:
	arg
	username=

Calls:
	strmove
	getuser
	loguser
	openmail
	closemail
	getline
	chown (sys)
	time (sys)
	printf (sys)
	getch	(util)
	putch	(util)

Called by:
	main thru command array

History:
	initial coding 		Mark Kampe UCLA-ATS
	modified 4/13/76 by S. F. Holmgren for Illinois version
	modified 6/30/76 by S. F. Holmgren to call getmbox
	modified 10/18/76 by J. S. Kravitz to improve net mail header
	chown removed by R. Balocca @ CAC, Sunday 1977 February 20
	getline removed and limit on line length removed by using
	getch and putch added by R. Balocca @ CAC, 1977 March 8 Tuesday
	Fixed oversight in above (forgot to translate <crlf> to <lf>)
		1977 March 10 Thursday by Rick Balocca @ CAC
	Added openmail & closemail, added logging, and fixed several
		bugs on or about 12/21/79 by Eric Allman, UCB/INGRES.
	Changed to always accept mail -- bad mail will be sent back --
		1/9/80 by Eric Allman, UCB/INGRES.
*/
#define gt (c = getch())
mail()
{
	register char *p;	/* general use */
	register int c;
	int i;

	/* extern struct io_buf obuf; */

	/* get to open mailbox file descriptor */
	fflush(&fout);
	if( (fout = openmail(arg)) < 0 )
		return;

	/* obuf.unused = 0;	obuf.addr = 0;	/* fake a fcreat */

	/* say its ok to continue */
	netreply( "350 Enter mail, end with a line containing only `.'\r\n" );

	for(;;)				/* while no error or <crlf>.<crlf> */
	{
		/* we are at beginning of line */

		if(gt=='.')			/*"."*/
		{
			if(gt=='\r')			/*".\r"*/
			{
				if(gt=='\n')			/*".\r\n"*/
				{
					/* end of message */
					break;
				}
				else
				{				/*".\r"c*/
					putch('.');
					putch('\r');
				}
			}
			else				/*"."c"*/
				putch('.');
		}
								/*"-"*/
					/* c */
		for(;;)
		{
			for(; c != '\r'; gt)
			{
				if( c < 0 )
				{
					/* fflush(&obuf); */
					/* write(obuf.fid, "\n***** Sender aborted connection *****\n", 39); */
					fflush(&fout);
					write(fout, "\n***** Sender aborted connection *****\n", 39);
					goto out;
				}
				else
					putch(c);
			}
	
						/*"\r"*/
			if( gt == '\n' )
			{			/*"\r\n"*/
crlf:
				putch('\n');
				break;
			}
			else
			{			/*"\r"c*/
crc:
				putch('\r');
				if(c=='\0')
					gt;	/* "\r\0" */
						/* is arpa escape for "\r" */
			}
		}
	}

out:
	fflush(&fout);
	if (closemail(fout) >= 0)
		netreply("256 Mail accepted\r\n");
}

/*
Name:
	datamail

Function:
	handle the MLFL command

Algorithm:
	fork
		make sure we have a valid user
			say bad user and exit
		send sock command
		open data connection
		get open mailbox file descriptor
		call rcvdata to receive mail

Parameters:
	username in arg

Returns:
	nothing

Globals:
	arg

Calls:
	fork (sys)
	strmove
	netreply
	sendsock
	dataconnection
	getmbox
	rcvdata
	printf (sys)
	time (sys)

Called by:
	main thru command array 

History:
	initial coding 4/13/76 by S. F. Holmgren
	modified 10/18/76 by J. S. Kravitz to put net mail header
	chown removed by R. Balocca @ CAC, Sunday 1977 February 20
*/
datamail()
{
	register netdata;
	/* register mboxfid; */
	register int i;

	i = fork();
	if (i < 0)
	{
		netreply("455 Mail server temporarily unavailable\r\n");
		return;
	}
	else if (i == 0)
	{
		fflush(&fout);
		if ((fout = openmail(arg)) < 0)
			exit(3);

		/* send sock command */
		sendsock( U4 );

		/* open data connection */
		netdata = dataconnection( U4 );

		/* say its ok to proceed */
		numreply( NUM250 );

		/* get data from net connection and copy to mail file */
		/* rcvdata( netdata,mboxfid ); */
		if (rcvdata(netdata, fout) < 0)
			exit(1);

		/* close the mail, see if ok; if so say ok */
		fflush(&fout);
		if (closemail(fout) >= 0)
			numreply( NUM252 );

		exit( 0 );
	}
}
/*
**  OPENMAIL -- Open a channel to the mail server
**
**	Gets the mail server started up ready to handle our
**	mail.
**
**	Algorithm:
**		See if the user is specified.
**			If not, send to user "root".
**		See if the user exists.
**			If not, signal error 450 and return.
**		Fork.
**		Create a pipe
**			Signal "unavailable" and exit on failure.
**		Fork.
**			Signal "unavailable" and exit on failure
**			In child:
**				Call mailer: /etc/delivermail is preferred.
**			In parent:
**				Avoid pipe signals in case delivermail dies.
**				Save the childs pid.
**				Return file descriptor.
**
**	Notes:
**		The check to see if the user actually exists should
**		go away so that we can do real mail forwarding.
**
**	Parameters:
**		who -- the user to send the mail to.
**
**	Returns:
**		File descriptor to send mail to.
**		-1 on failure.
**
**	Side Effects:
**		Forks /etc/delivermail or /bin/mail or /usr/bin/mail.
**		Becomes "network" in the child.
**
**	Requires:
**		strmove
**		getuser
**		netreply
**		pipe (sys)
**		fork (sys)
**		close (sys)
**		dup (sys)
**		execl (sys)
**		signal (sys)
**		exit (sys)
**
**	Called By:
**		mail
**		datamail
**
**	History:
**		1/9/80 -- Added 050 & 455 reply messages if execl's
**			fail.  Eric Allman UCB/INGRES.
**		11/26/79 -- Modified to map upper case to lower
**			case.  Eric Allman UCB/INGRES.
**		11/10/79 -- Written by Eric Allman UCB/INGRES
**		3/6/80 -- Dropped case mapping; delivermail does
**			that now.  EPA UCB/INGRES.
*/

int Mail_pid;
char *Mail_user;

openmail(who)
	char *who;
{
	register char *w;
	register int i;
	int pvect[2];
	register char *p;

	w = who;
	if (w == 0)
		w = "root";
/*
	else
	{
		for (p = w; *p != '\0'; p++)
		{
			if (*p >= 'A' && *p <= 'Z')
				*p =- 'A' - 'a';
		}
	}
*/
	Mail_user = w;

	/* see if the user exists */
	strmove(w, username);
/*
	if (getuser(0) == 0)
	{
		netreply("450 User unknown\r\n");
		return (-1);
	}
*/

	/* try to get a pipe to the mailer */
	if (pipe(pvect) < 0)
	{
	  unavailable:
		netreply("455 Mail server temporarily unavailable\r\n");
		return (-1);
	}

	/* fork */
	i = fork();
	if (i < 0)
	{
		/* failure */
		close(pvect[0]);
		close(pvect[1]);
		goto unavailable;
	}
	else if (i == 0)
	{
		/* child */
		close(pvect[1]);
		close(0);
		dup(pvect[0]);
		close(pvect[0]);
		setuid(NETUID);

		/* try to call something to deliver the mail */
		execl("/etc/delivermail", "delivermail", "-em", "-a", w, 0);
		netreply("050 Not using normal mail server, beware!\r\n");
		execl("/bin/mail", "mail", w, 0);
		execl("/usr/bin/mail", "mail", w, 0);

		/* doesn't seem to be anything around */
		netreply("455 Mail server unavailable\r\n");
		exit(3);
	}

	/* else parent */
	signal(SIGPIPE, SIG_IGN);
	Mail_pid = i;
	close(pvect[0]);
	return (pvect[1]);
}
/*
**  CLOSEMAIL -- Close the mail file and get actual status
**
**	The mail file is closed.
**
**	Algorithm:
**		Wait for the mailer to die.
**			If it wasn't there, be non-comittal.
**		If it died a violent death, give error.
**
**	Parameters:
**		fd -- the file descriptor of the mail file.
**
**	Returns:
**		none.
**
**	Side Effects:
**		mailer is soaked up.
**
**	Requires:
**		close (sys)
**		wait (sys)
**
**	Called By:
**		mail
**		datamail
**
**	History:
**		1/9/80 -- Changed to not check for errors in mailing,
**			since these will be mailed back.
**		11/10/79 -- Written by Eric Allman UCB/INGRES.
*/

closemail(fd)
	int fd;
{
	auto int st;
	register int i;

	/* close the pipe -- mail should go away */
	close(fd);

	/* wait for its body */
	while ((i = wait(&st)) != Mail_pid)
	{
		if (i < 0)
		{
			/* how did this happen? */
			logmsg(LOG_ERR, "mail from host %d to %s: no child",
			    openparams.o_frnhost & 0377, Mail_user);
			goto unavailable;
		}
	}

	/* 'st' is now the status of the mailer */
	if ((st & 0377) != 0)
	{
		logmsg(LOG_ERR, "mail from host %d to %s: status %o",
		    openparams.o_frnhost & 0377, Mail_user, st);
unavailable:
		netreply("455 Mail not delivered -- local system error\r\n");
		return (-1);
	}

	return (0);
}
 */
	while ((i = wait(&st)) != Mail_pid)
	{
		if (i < 0)
		{
			/* how did this happen? */
			logmsg(LOG_ERR, "mail from host %d to %s: no child",
			    openparams.o_frnhost & 0377, Mail_user);
			goto unavailable;
		}
	}

	/* 'st' is now the status of the mailer */
	if ((st & 0377) != 0)
	{
		logmsg(LOG_ERR, "mail from host %d to %s: status %o",
		    openparams.o_frnhost & 0377, Mail_user, st);
unavailable:
		netreply("455 Mail not delivered -- local systedelivermail/main.c                                                                                     660      17       2        26035  2470111561   7445                                                                                                                                                                                                                                                                                                                                                                      # include <stdio.h>
# include <signal.h>
# include <ctype.h>
# include "dlvrmail.h"
# ifdef LOG
# include <log.h>
# endif LOG

static char	SccsId[] = "@(#)main.c	2.3	1/10/81";

/*
**  DELIVERMAIL -- Deliver mail to a set of destinations
**
**	This is the basic mail router.  All user mail programs should
**	call this routine to actually deliver mail.  Delivermail in
**	turn calls a bunch of mail servers that do the real work of
**	delivering the mail.
**
**	Delivermail is driven by tables defined in config.c.  This
**	file will be different from system to system, but the rest
**	of the code will be the same.  This table could be read in,
**	but it seemed nicer to have it compiled in, since deliver-
**	mail will potentially be exercised a lot.
**
**	Usage:
**		/etc/delivermail [-f name] [-a] [-q] [-v] [-n] [-m] addr ...
**
**	Positional Parameters:
**		addr -- the address to deliver the mail to.  There
**			can be several.
**
**	Flags:
**		-f name		The mail is from "name" -- used for
**				the header in local mail, and to
**				deliver reports of failures to.
**		-r name		Same as -f; however, this flag is
**				reserved to indicate special processing
**				for remote mail delivery as needed
**				in the future.  So, network servers
**				should use -r.
**		-a		This mail should be in ARPANET std
**				format (not used).
**		-n		Don't do aliasing.  This might be used
**				when delivering responses, for
**				instance.
**		-d		Run in debug mode.
**		-em		Mail back a response if there was an
**				error in processing.  This should be
**				used when the origin of this message
**				is another machine.
**		-ew		Write back a response if the user is
**				still logged in, otherwise, act like
**				-em.
**		-eq		Don't print any error message (just
**				return exit status).
**		-ep		(default)  Print error messages
**				normally.
**		-ee		Send BerkNet style errors.  This
**				is equivalent to MailBack except
**				that it has gives zero return code
**				(unless there were errors during
**				returning).  This used to be
**				"EchoBack", but you know how the old
**				software bounces.
**		-m		In group expansion, send to the
**				sender also (stands for the Mail metoo
**				option.
**		-i		Do not terminate mail on a line
**				containing just dot.
**		-s		Save UNIX-like "From" lines on the
**				front of messages.
**
**	Return Codes:
**		As defined in <sysexits.h>.
**
**		These codes are actually returned from the auxiliary
**		mailers; it is their responsibility to make them
**		correct.
**
**	Compilation Flags:
**		LOG -- if set, everything is logged.
**
**	Compilation Instructions:
**		cc -c -O main.c config.c deliver.c parse.c
**		cc -n -s *.o -lS
**		chown root a.out
**		chmod 755 a.out
**		mv a.out delivermail
**
**	Deficiencies:
**		It ought to collect together messages that are
**			destined for a single host and send these
**			to the auxiliary mail server together.
**		It should take "user at host" as three separate
**			parameters and combine them into one address.
**
**	Author:
**		Eric Allman, UCB/INGRES
*/





bool	ArpaFmt;	/* mail is expected to be in ARPANET format */
bool	FromFlag;	/* from person is explicitly specified */
bool	Debug;		/* run in debug mode */
bool	MailBack;	/* mail back response on error */
bool	BerkNet;	/* called from BerkNet */
bool	WriteBack;	/* write back response on error */
bool	HasXscrpt;	/* if set, the transcript file exists */
bool	NoAlias;	/* don't do aliasing */
bool	ForceMail;	/* mail even if already sent a copy */
bool	MeToo;		/* send to the sender also if in a group expansion */
bool	SaveFrom;	/* save From lines on the front of messages */
bool	IgnrDot;	/* if set, ignore dot when collecting mail */
bool	SuprErrs;	/* supress errors if set */
int	Errors;		/* count of errors */
char	InFileName[] = "/tmp/mailtXXXXXX";
char	Transcript[] = "/tmp/mailxXXXXXX";
addrq	From;		/* the from person */
char	*To;		/* the target person */
int	HopCount;	/* hop count */
int	ExitStat;	/* the exit status byte */
addrq	SendQ;		/* queue of people to send to */
addrq	AliasQ;		/* queue of people who turned out to be aliases */






main(argc, argv)
	int argc;
	char **argv;
{
	register char *p;
	extern char *maketemp();
	extern char *getname();
	extern int finis();
	extern addrq *parse();
	register addrq *q;
	extern char Version[];
	extern int errno;
	char *from;
	register int i;
	typedef int (*fnptr)();
	char nbuf[MAXLINE];

	if (signal(SIGINT, SIG_IGN) != SIG_IGN)
		signal(SIGINT, finis);
	signal(SIGTERM, finis);
	setbuf(stdout, (char *) NULL);
# ifdef LOG
	openlog("delivermail", 0);
# endif LOG
# ifdef DEBUG
# ifdef DEBUGFILE
	if ((i = open(DEBUGFILE, 1)) > 0)
	{
		lseek(i, 0L, 2);
		close(1);
		dup(i);
		close(i);
		Debug++;
	}
# endif DEBUGFILE
# endif
	errno = 0;
	from = NULL;

	/*
	** Crack argv.
	*/

	while (--argc > 0 && (p = *++argv)[0] == '-')
	{
		switch (p[1])
		{
		  case 'r':	/* obsolete -f flag */
		  case 'f':	/* from address */
			p += 2;
			if (*p == '\0')
			{
				p = *++argv;
				if (--argc <= 0 || *p == '-')
				{
					syserr("No \"from\" person");
					argc++;
					argv--;
					break;
				}
			}
			if (from != NULL)
			{
				syserr("More than one \"from\" person");
				break;
			}
			from = p;
			break;

		  case 'h':	/* hop count */
			p += 2;
			if (*p == '\0')
			{
				p = *++argv;
				if (--argc <= 0 || *p < '0' || *p > '9')
				{
					syserr("Bad hop count (%s)", p);
					argc++;
					argv--;
					break;
				}
			}
			HopCount = atoi(p);
			break;

		  case 'e':	/* error message disposition */
			switch (p[2])
			{
			  case 'p':	/* print errors normally */
				break;	/* (default) */

			  case 'q':	/* be silent about it */
				freopen("/dev/null", "w", stdout);
				break;

			  case 'm':	/* mail back */
				MailBack++;
				openxscrpt();
				break;

			  case 'e':	/* do berknet error processing */
				BerkNet++;
				openxscrpt();
				break;

			  case 'w':	/* write back (or mail) */
				WriteBack++;
				openxscrpt();
				break;
			}
			break;

# ifdef DEBUG
		  case 'd':	/* debug */
			Debug++;
			printf("%s\n", Version);
			break;
# endif DEBUG
		
		  case 'n':	/* don't alias */
			NoAlias++;
			break;

		  case 'm':	/* send to me too */
			MeToo++;
			break;

		  case 'i':	/* don't let dot stop me */
			IgnrDot++;
			break;

		  case 'a':	/* arpanet format */
			ArpaFmt++;
			break;
		
		  case 's':	/* save From lines in headers */
			SaveFrom++;
			break;

		  default:
			/* at Eric Schmidt's suggestion, this will not be an error....
			syserr("Unknown flag %s", p);
			... seems that upward compatibility will be easier. */
			break;
		}
	}

	if (from != NULL && ArpaFmt)
		syserr("-f and -a are mutually exclusive");

	/*
	** Get a temp file.
	*/

	p = maketemp();
	if (from == NULL)
		from = p;
# ifdef DEBUG
	if (Debug)
		printf("Message-Id: <%s>\n", MsgId);
# endif DEBUG

	/*
	**  Figure out who it's coming from.
	**	Under certain circumstances allow the user to say who
	**	s/he is (using -f or -r).  These are:
	**	1.  The user's uid is zero (root).
	**	2.  The user's login name is "network" (mail from
	**	    a network server).
	**	3.  The user's login name is "uucp" (mail from the
	**	    uucp network).
	**	4.  The address the user is trying to claim has a
	**	    "!" character in it (since #3 doesn't do it for
	**	    us if we are dialing out).
	**	A better check to replace #3 & #4 would be if the
	**	effective uid is "UUCP" -- this would require me
	**	to rewrite getpwent to "grab" uucp as it went by,
	**	make getname more nasty, do another passwd file
	**	scan, or compile the UID of "UUCP" into the code,
	**	all of which are reprehensible.
	**
	**	Assuming all of these fail, we figure out something
	**	ourselves.
	*/

	errno = 0;
	p = getname();
	if (p == NULL || p[0] == '\0')
	{
		syserr("Who are you? (uid=%d)", getuid());
		finis();
	}
	errno = 0;
	if (from != NULL)
	{
		if (strcmp(p, "network") != 0 && strcmp(p, "uucp") != 0 &&
		    index(from, '!') == NULL && getuid() != 0)
		{
			/* network sends -r regardless (why why why?) */
			/* syserr("%s, you cannot use the -f flag", p); */
			from = NULL;
		}
	}
	if (from == NULL || from[0] == '\0')
		from = p;
	else
		FromFlag++;
	SuprErrs = TRUE;
	if (parse(from, &From, 0) == NULL)
	{
		/* too many arpanet hosts generate garbage From addresses ....
		syserr("Bad from address `%s'", from);
		.... so we will just ignore this address */
		from = p;
		FromFlag = FALSE;
	}
	SuprErrs = FALSE;

# ifdef DEBUG
	if (Debug)
		printf("From person = \"%s\"\n", From.q_paddr);
# endif DEBUG

	if (argc <= 0)
		usrerr("Usage: /etc/delivermail [flags] addr...");

	/*
	**  Process Hop count.
	**	The Hop count tells us how many times this message has
	**	been processed by delivermail.  If it exceeds some
	**	fairly large threshold, then we assume that we have
	**	an infinite forwarding loop and die.
	*/

	if (++HopCount > MAXHOP)
		syserr("Infinite forwarding loop (%s->%s)", From.q_paddr, *argv);

	/*
	** Scan argv and deliver the message to everyone.
	*/

	for (; argc-- > 0; argv++)
	{
		p = argv[1];
		if (argc >= 2 && p[2] == '\0' &&
		    (p[0] == 'a' || p[0] == 'A') &&
		    (p[1] == 't' || p[1] == 'T'))
		{
			if (strlen(argv[0]) + strlen(argv[2]) + 2 > sizeof nbuf)
			{
				usrerr("address overflow");
				p = argv[0];
			}
			else
			{
				strcpy(nbuf, argv[0]);
				strcat(nbuf, "@");
				strcat(nbuf, argv[2]);
				p = nbuf;
				argv += 2;
				argc -= 2;
			}
		}
		else
			p = argv[0];
		sendto(p, 0);
	}

	/* if we have had errors sofar, drop out now */
	if (Errors > 0 && ExitStat == EX_OK)
		ExitStat = EX_USAGE;
	if (ExitStat != EX_OK)
		finis();

	/*
	**  See if we have anyone to send to at all.
	*/

	if (nxtinq(&SendQ) == NULL && ExitStat == EX_OK)
	{
		syserr("Noone to send to!");
		ExitStat = EX_USAGE;
		finis();
	}

	/*
	**  Do aliasing.
	**	First arrange that the person who is sending the mail
	**	will not be expanded (unless explicitly requested).
	*/

	if (!MeToo)
		recipient(&From, &AliasQ);
	To = NULL;
	alias();
	if (nxtinq(&SendQ) == NULL && ExitStat == EX_OK)
	{
/*
		syserr("Vacant send queue; probably aliasing loop");
		ExitStat = EX_SOFTWARE;
		finis();
*/
		recipient(&From, &SendQ);
	}

	/*
	**  Actually send everything.
	*/

	for (q = &SendQ; (q = nxtinq(q)) != NULL; )
		deliver(q, (fnptr) NULL);

	/*
	** All done.
	*/

	finis();
}
/*
**  FINIS -- Clean up and exit.
**
**	Parameters:
**		none
**
**	Returns:
**		never
**
**	Side Effects:
**		exits delivermail
**
**	Called By:
**		main
**		via signal on interrupt.
**
**	Deficiencies:
**		It may be that it should only remove the input
**			file if there have been no errors.
*/

finis()
{
	/* mail back the transcript on errors */
	if (ExitStat != EX_OK)
		savemail();

	if (HasXscrpt)
		unlink(Transcript);
	unlink(InFileName);
	exit(ExitStat);
}
/*
**  OPENXSCRPT -- Open transcript file
**
**	Creates a transcript file for possible eventual mailing or
**	sending back.
**
**	Parameters:
**		none
**
**	Returns:
**		none
**
**	Side Effects:
**		Turns the standard output into a special file
**			somewhere.
**
**	Called By:
**		main
*/

openxscrpt()
{
	mktemp(Transcript);
	HasXscrpt++;
	if (freopen(Transcript, "w", stdout) == NULL)
		syserr("Can't create %s", Transcript);
	chmod(Transcript, 0600);
	setbuf(stdout, (char *) NULL);
}
);
	exit(ExitStat);
}
/*
**  OPENXSCRPT -- Open transcript file
**
**	Creates a transcript file for possible eventual mailing or
**	sending back.
**
**	Parameters:
**		none
**
**	Returns:
**		none
**
**	Side Effects:
**		Turns the standard output into a special file
**			somewhere.
**
**	Called By:
**		main
*/

openxscrpt()
{
	mktemp(Transcript);
	HasXscrpt++;
	if (freopen(Transcript, "w", stdout) == NULL)
		syserr("Can't create %s", Transcript);
	chmod(Transcript, 0600);
	setbdelivermail/makefile                                                                                   660      17       2         5332  2470111562  10033                                                                                                                                                                                                                                                                                                                                                                      #
#  VAX Version
#
#	@(#)makefile	2.2	12/6/80
#
#	Compilation Flags (in CFLAGS):
#		-DDEBUG -- compile in debug stuff.  This is not enabled
#			unless the debug flag is given, so the only
#			advantage in not including this is that the
#			binaries will be smaller.
#		-DLOG -- include log information.  This is probably
#			only useful on systems that include the logger.
#		-DVFORK -- use 'vfork' instead of 'fork'.
#		-DDBM -- use the dbm package instead of scanning the
#			ALIASFILE sequentially.  LIBS must include
#			-ldbm for this to work.
#

LIBS=	-ldbm
TARG=	$(DESTDIR)/etc

OBJS1=	conf.o main.o maketemp.o parse.o alias.o deliver.o \
	savemail.o addr.o err.o
OBJS2=	matchhdr.o sysexits.o util.o bmove.o
SRCS=	useful.h dlvrmail.h \
	conf.c deliver.c main.c parse.c err.c alias.c savemail.c addr.c \
	matchhdr.c sysexits.c util.c bmove.c bmove.11.s bmove.vax.s \
	arpa.c arpadate.c version.c maketemp.c \
	newaliases.c praliases.c
ALL=	delivermail newaliases arpa

CHOWN=	-echo chown
CHMOD=	chmod
CFLAGS=	-O -DDBM -DVFORK -DDEBUG
LDFLAGS=
AR=	-ar
ARFLAGS=rvu
LINT=	lint
LINTFLAGS=-bxa
XREF=	csh /usr/bin/xref
CP=	cp

GET=	sccs get
DELTA=	sccs delta
REL=

ROOT=	root
OBJMODE=755

delivermail: $(OBJS1) $(OBJS2) version.o
	$(CC) $(LDFLAGS) -o delivermail version.o $(OBJS1) $(OBJS2) $(LIBS)
	$(CHMOD) $(OBJMODE) delivermail
	size delivermail; ls -l delivermail

install: all
	$(CP) delivermail $(TARG)/delivermail
	install -s newaliases $(DESTDIR)/usr/ucb

version:
	@rm -f SCCS/p.version.c version.c
	@$(GET) $(REL) -e -s SCCS/s.version.c
	@$(DELTA) -s SCCS/s.version.c
	@$(GET) -t SCCS/s.version.c

$(OBJS1): dlvrmail.h

dlvrmail.h util.o: useful.h

all: $(ALL)

#
#  Auxiliary stuff
#

clean:
	rm -f core delivermail arpa uucp a.out xref
	rm -f *.o

sources: $(SRCS)

$(SRCS):
	$(GET) $(REL) SCCS/s.$@

archive:
	${AR} ${ARFLAGS} delivermail.a READ_ME TO_BE_DONE makefile makefl.* *.h *.c *.s makeversion *.[123456789u]

print: xref
	@ls -l | pr -h "delivermail directory"
	@pr -h "cross reference listing" xref
	@size *.o | pr -h "object code sizes"
	@pr *.h *.[cs]

lint:
	$(LINT) $(LINTFLAGS) $(SRCS)

xref: *.c
	${XREF} *.c > xref

#
#  Data base maintenance routines
#
AOBJS=	newaliases.o parse.o conf.o util.o

newaliases: $(AOBJS)
	$(CC) $(LDFLAGS) $(AOBJS) -o newaliases $(LIBS)

praliases: praliases.c
	$(CC) $(CFLAGS) praliases.c -o praliases $(LIBS)

newaliases.o praliases.o: dlvrmail.h

#
#  Auxiliary mailers
#

arpa: arpa.o matchhdr.o arpadate.o
	$(CC) $(LDFLAGS) -o arpa arpa.o matchhdr.o arpadate.o $(LIBS)
	${CHMOD} ${OBJMODE} arpa
	size arpa; ls -l arpa

uucp: uucp.o
	${CC} ${LDFLAGS} -o uucp uucp.o ${LIBS}
	${CHMOD} ${OBJMODE} uucp
	size uucp; ls -l uucp

mail:	mail.o getname.o
	$(CC) $(LDFLAGS) -o mail mail.o getname.o
	size mail; ls -l mail
 $(AOBJS) -o newaliases $(LIBS)

praliases: praliases.c
	$(CC) $(CFLAGS) praliases.c -o praliases $(LIBS)

newaliases.o praliases.o: dlvrmail.h

#
#  Auxiliary mailers
#

arpa: arpa.o matchhdr.o arpadate.o
	$(CC) $(LDFLAGS) -o arpa arpa.o matchhdr.o arpadate.o $(LIBS)
	${CHMOD} ${OBJMODE} arpadelivermail/makefl.ing70                                                                               660      17       2         3742  2470111562  10443                                                                                                                                                                                                                                                                                                                                                                      #
#  INGRES 11/70 Version
#
#	@(#)makefl.ing70	2.1	11/5/80
#

LIBS=	-lX -lS
TARG=	/etc

OBJS1=	conf.o main.o maketemp.o parse.o alias.o deliver.o \
	savemail.o addr.o err.o
OBJS2=	matchhdr.o sysexits.o util.o bmove.o
SRCS=	useful.h dlvrmail.h \
	conf.c deliver.c main.c parse.c err.c alias.c savemail.c addr.c \
	matchhdr.c sysexits.c util.c bmove.c bmove.11.s bmove.vax.s \
	arpa.c arpadate.c version.c maketemp.c

CHOWN=	-echo chown
CHMOD=	chmod
CFLAGS=	-O -DDEBUG -DLOG
LDFLAGS=-n
AR=	-ar
ARFLAGS=rvu
LINT=	lint
LINTFLAGS=-bxa
XREF=	csh /usr/bin/xref
CP=	cp

GET=	sccs get
DELTA=	sccs delta
REL=

ROOT=	root
OBJMODE=755

delivermail: $(OBJS1) $(OBJS2) version.o
	$(CC) $(LDFLAGS) -o delivermail version.o $(OBJS1) $(OBJS2) $(LIBS)
	$(CHMOD) $(OBJMODE) delivermail
	size delivermail; ls -l delivermail

install: $(OBJS1) $(OBJS2) install1 delivermail
	$(CP) delivermail $(TARG)/delivermail

install1:
	@rm -f SCCS/p.version.c version.c
	@$(GET) $(REL) -e -s SCCS/s.version.c
	@$(DELTA) -s SCCS/s.version.c
	@$(GET) -t SCCS/s.version.c

main.o: main.c
	$(CC) $(CFLAGS) -DDEBUGFILE=\"/mnt/eric/DEBUG.DLVRML\" -c $<

$(OBJS1): dlvrmail.h

dlvrmail.h util.o: useful.h

#
#  Auxiliary stuff
#

clean:
	rm -f core delivermail arpa uucp a.out xref version.c
	rm -f *.o

sources: $(SRCS)

$(SRCS):
	$(GET) $(REL) SCCS/s.$@

archive:
	${AR} ${ARFLAGS} delivermail.a READ_ME TO_BE_DONE makefile makefl.* *.h *.c *.s makeversion *.[123456789u]

print: xref
	@ls -l | pr -h "delivermail directory"
	@pr -h "cross reference listing" xref
	@size *.o | pr -h "object code sizes"
	@pr *.h *.[cs]

lint:
	$(LINT) $(LINTFLAGS) $(SRCS)

xref: *.c
	${XREF} *.c > xref

#
#  Auxiliary mailers
#

arpa: arpa.o matchhdr.o arpadate.o
	$(CC) $(LDFLAGS) -o arpa arpa.o matchhdr.o arpadate.o $(LIBS)
	${CHMOD} ${OBJMODE} arpa
	size arpa; ls -l arpa

uucp: uucp.o
	${CC} ${LDFLAGS} -o uucp uucp.o ${LIBS}
	${CHMOD} ${OBJMODE} uucp
	size uucp; ls -l uucp

mail:	mail.o getname.o
	$(CC) $(LDFLAGS) -o mail mail.o getname.o
	size mail; ls -l mail
erence listing" xref
	@size *.delivermail/makefl.vax                                                                                 660      17       2         3602  2470111562  10310                                                                                                                                                                                                                                                                                                                                                                      #
#  VAX Version
#
#	@(#)makefl.vax	2.1	11/5/80
#

LIBS=
TARG=	/etc

OBJS1=	conf.o main.o maketemp.o parse.o alias.o deliver.o \
	savemail.o addr.o err.o
OBJS2=	matchhdr.o sysexits.o util.o bmove.o
SRCS=	useful.h dlvrmail.h \
	conf.c deliver.c main.c parse.c err.c alias.c savemail.c addr.c \
	matchhdr.c sysexits.c util.c bmove.c bmove.11.s bmove.vax.s \
	arpa.c arpadate.c version.c maketemp.c

CHOWN=	-echo chown
CHMOD=	chmod
CFLAGS=	-O -DDEBUG -I.
LDFLAGS=-n -s
AR=	-ar
ARFLAGS=rvu
LINT=	lint
LINTFLAGS=-bxa
XREF=	csh /usr/bin/xref
CP=	cp

GET=	sccs get
DELTA=	sccs delta
REL=

ROOT=	root
OBJMODE=755

delivermail: $(OBJS1) $(OBJS2) version.o
	$(CC) $(LDFLAGS) -o delivermail version.o $(OBJS1) $(OBJS2) $(LIBS)
	$(CHMOD) $(OBJMODE) delivermail
	size delivermail; ls -l delivermail

install: $(OBJS1) $(OBJS2) install1 delivermail
	$(CP) delivermail $(TARG)/delivermail

install1:
	@rm -f SCCS/p.version.c version.c
	@$(GET) $(REL) -e -s SCCS/s.version.c
	@$(DELTA) -s SCCS/s.version.c
	@$(GET) -t SCCS/s.version.c

$(OBJS1): dlvrmail.h

dlvrmail.h util.o: useful.h

#
#  Auxiliary stuff
#

clean:
	rm -f core delivermail arpa uucp a.out xref version.c
	rm -f *.o

sources: $(SRCS)

$(SRCS):
	$(GET) $(REL) SCCS/s.$@

archive:
	${AR} ${ARFLAGS} delivermail.a READ_ME TO_BE_DONE makefile makefl.* *.h *.c *.s makeversion *.[123456789u]

print: xref
	@ls -l | pr -h "delivermail directory"
	@pr -h "cross reference listing" xref
	@size *.o | pr -h "object code sizes"
	@pr *.h *.[cs]

lint:
	$(LINT) $(LINTFLAGS) $(SRCS)

xref: *.c
	${XREF} *.c > xref

#
#  Auxiliary mailers
#

arpa: arpa.o matchhdr.o arpadate.o
	$(CC) $(LDFLAGS) -o arpa arpa.o matchhdr.o arpadate.o $(LIBS)
	${CHMOD} ${OBJMODE} arpa
	size arpa; ls -l arpa

uucp: uucp.o
	${CC} ${LDFLAGS} -o uucp uucp.o ${LIBS}
	${CHMOD} ${OBJMODE} uucp
	size uucp; ls -l uucp

mail:	mail.o getname.o
	$(CC) $(LDFLAGS) -o mail mail.o getname.o
	size mail; ls -l mail
erence listing" xref
	@size *.o | pr -h "object code sizes"
	@pr *.h *.[cs]

lint:
	$(LINT) $(LINTFLAGS) $(SRCS)

xref: *.c
	$delivermail/maketemp.c                                                                                 660      17       2        12140  2470111562  10315                                                                                                                                                                                                                                                                                                                                                                      # include <stdio.h>
# include <ctype.h>
# include <errno.h>
# include "dlvrmail.h"

static char	SccsId[] = "@(#)maketemp.c	2.3	12/6/80";

/*
**  MAKETEMP -- read & parse message header & make temp file.
**
**	Creates a temporary file name and copies the standard
**	input to that file.  While it is doing it, it looks for
**	"From:" and "Sender:" fields to use as the from-person
**	(but only if the -a flag is specified).  It prefers to
**	to use the "Sender:" field.
**
**	MIT seems to like to produce "Sent-By:" fields instead
**	of "Sender:" fields.  We used to catch this, but it turns
**	out that the "Sent-By:" field doesn't always correspond
**	to someone real ("___057", for instance), as required by
**	the protocol.  So we limp by.....
**
**	Parameters:
**		none
**
**	Returns:
**		Name of temp file.
**
**	Side Effects:
**		Temp file is created and filled.
**
**	Called By:
**		main
**
**	Notes:
**		This is broken off from main largely so that the
**		temp buffer can be deallocated.
*/

char	MsgId[MAXNAME];		/* message-id, determined or created */
long	MsgSize;		/* size of message in bytes */
bool	GotHdr;			/* if set, "From ..." line exists */

char *
maketemp()
{
	register FILE *tf;
	char buf[MAXFIELD+1];
	static char fbuf[sizeof buf];
	extern char *prescan();
	extern char *matchhdr();
	register char *p;
	register bool inheader;
	bool firstline;
	char c;
	extern int errno;

	/*
	**  Create the temp file name and create the file.
	*/

	mktemp(InFileName);
	close(creat(InFileName, 0600));
	if ((tf = fopen(InFileName, "w")) == NULL)
	{
		syserr("Cannot create %s", InFileName);
		return (NULL);
	}

	/*
	**  Copy stdin to temp file & do message editting.
	**	From person gets copied into fbuf.  At the end of
	**	this loop, if fbuf[0] == '\0' then there was no
	**	recognized from person in the message.  We also
	**	save the message id in MsgId.  The
	**	flag 'inheader' keeps track of whether we are
	**	in the header or in the body of the message.
	**	The flag 'firstline' is only true on the first
	**	line of a message.
	**	To keep certain mailers from getting confused,
	**	and to keep the output clean, lines that look
	**	like UNIX "From" lines are deleted in the header,
	**	and prepended with ">" in the body.
	*/

	inheader = TRUE;
	firstline = TRUE;
	fbuf[0] = '\0';
	while (!feof(stdin) && fgets(buf, sizeof buf, stdin) != NULL)
	{
		if (inheader && isalnum(buf[0]))
		{
			/* get the rest of this field */
			while ((c = getc(stdin)) == ' ' || c == '\t')
			{
				p = &buf[strlen(buf)];
				*p++ = c;
				if (fgets(p, sizeof buf - (p - buf), stdin) == NULL)
					break;
			}
			if (c != EOF)
				ungetc(c, stdin);
		}

		if (!IgnrDot && buf[0] == '.' && (buf[1] == '\n' || buf[1] == '\0'))
			break;

		/* are we still in the header? */
		if ((buf[0] == '\n' || buf[0] == '\0') && inheader)
		{
			inheader = FALSE;
			if (MsgId[0] == '\0')
			{
				makemsgid();
				if (UseMsgId)
					fprintf(tf, "Message-Id: <%s>\n", MsgId);
			}
# ifdef DEBUG
			if (Debug)
				printf("EOH\n");
# endif DEBUG
		}

		/* Hide UNIX-like From lines */
		if (strncmp(buf, "From ", 5) == 0)
		{
			if (!firstline)
			{
				fputs(">", tf);
				MsgSize++;
			}
			else
				GotHdr++;
		}

		if (inheader && !isspace(buf[0]))
		{
			/* find out if this is really a header */
			for (p = buf; *p != ':' && *p != '\0' && !isspace(*p); p++)
				continue;
			while (*p != ':' && isspace(*p))
				p++;
			if (*p != ':')
			{
				inheader = FALSE;
# ifdef DEBUG
				if (Debug)
					printf("EOH?\n");
# endif DEBUG
			}
		}

		if (inheader)
		{
			/* find the sender */
			p = matchhdr(buf, "sender");
			if (p == NULL && fbuf[0] == '\0')
				p = matchhdr(buf, "from");
			if (p != NULL)
				prescan(p, fbuf, &fbuf[sizeof fbuf - 1], '\0');

			/* find the message id */
			p = matchhdr(buf, "message-id");
			if (p != NULL && MsgId[0] == '\0')
				prescan(p, MsgId, &MsgId[sizeof MsgId - 1], '\0');
		}
		MsgSize += strlen(buf);
		fputs(buf, tf);
		firstline = FALSE;
		if (ferror(tf))
		{
			if (errno == ENOSPC)
			{
				freopen(InFileName, "w", tf);
				fputs("\nMAIL DELETED BECAUSE OF LACK OF DISK SPACE\n\n", tf);
				syserr("Out of disk space for temp file");
			}
			else
				syserr("Cannot write %s", InFileName);
			freopen("/dev/null", "w", tf);
		}
	}
	fclose(tf);
	if (MsgId[0] == '\0')
		makemsgid();
	if (freopen(InFileName, "r", stdin) == NULL)
		syserr("Cannot reopen %s", InFileName);
	return (ArpaFmt && fbuf[0] != '\0' ? fbuf : NULL);
}
/*
**  MAKEMSGID -- Compute a message id for this process.
**
**	This routine creates a message id for a message if
**	it did not have one already.  If the MESSAGEID compile
**	flag is set, the messageid will be added to any message
**	that does not already have one.  Currently it is more
**	of an artifact, but I suggest that if you are hacking,
**	you leave it in -- I may want to use it someday if
**	duplicate messages turn out to be a problem.
**
**	Parameters:
**		none.
**
**	Returns:
**		none.
**
**	Side Effects:
**		Stores a message-id into MsgId.
**
**	Called By:
**		maketemp
*/

makemsgid()
{
	auto long t;
	extern char *MyLocName;
	extern char *ArpaHost;

	time(&t);
	sprintf(MsgId, "%ld.%d.%s@%s", t, getpid(), MyLocName, ArpaHost);
}
t does not already have one.  Currently it is more
**	of an artifact, but I suggest that if you are hacking,
**	you leave it in -- I may want to use it someday if
**	duplicate messages turn out to be a problem.
**
**	Parameters:
**		none.
**
**	Returns:
**		none.
**
**	Side Effects:
**		Stores a message-id into MsgId.
**
**	Called By:
**		maketemp
*/

makemsgid()
{
	auto long t;
	extern char *MyLocName;
	extern cdelivermail/matchhdr.c                                                                                 660      17       2         2622  2470111563  10271                                                                                                                                                                                                                                                                                                                                                                      # include <stdio.h>
# include <ctype.h>

static char	SccsId[] = "@(#)matchhdr.c	2.1	11/5/80";

/*
**  MATCHHDR -- Match header line
**
**	Matches a header line in arpanet format (case and white
**	space is ignored).
**
**	This routine is used by arpa-mailer and delivermail.
**
**	Parameters:
**		line -- the line to match against.
**		pat -- the pattern to match against; must be in
**			lower case.
**
**	Returns:
**		address of the 'value' of the pattern (the beginning
**			of the non-white string following the delim).
**		NULL if none found.
**
**	Side Effects:
**		none
**
**	Called By:
**		maketemp
**		sendmail [arpa.c]
**
**	Deficiencies:
**		It doesn't handle folded lines.
*/

char *
matchhdr(line, pat)
	char *line;
	char *pat;
{
	register char *p;
	register char *q;

	for (q = pat, p = line; *q != '\0'; p++, q++)
		if (lower(*p) != *q)
			return (NULL);
	while (isspace(*p))
		p++;
	if (*p != ':')
		return (NULL);
	while (isspace(*++p))
		continue;
	return (*p == '\0' ? NULL : p);
}
/*
**  LOWER -- Convert a character to lower case
**
**	If the argument is an upper case letter, it is converted
**	to a lower case letter, otherwise it is passed through
**	unchanged.
**
**	Parameters:
**		c -- the character to check.
**
**	Returns:
**		c converted to lower case.
**
**	Side Effects:
**		none
**
**	Called By:
**		matchhdr
*/

lower(c)
	register char c;
{
	if (isupper(c))
		c -= 'A' - 'a';
	return (c);
}
		return (NULL);
	while (isspace(*++p))
		continue;
	return (*p == '\0' ? NULL : p);
}
/*
**  LOWER -- Converdelivermail/newaliases.c                                                                               660      17       2         6601  2470111565  10635                                                                                                                                                                                                                                                                                                                                                                      # include <stdio.h>
# include <ctype.h>
# include "dlvrmail.h"

static	char SccsId[] = "@(#)newaliases.c	2.1	11/5/80";

typedef struct { char *dptr; int dsize; } datum;
char *aliases = ALIASFILE;
char dirbuf[100];
char pagbuf[100];
int LineNo;
char *To;
int ExitStat;
int Errors;
# ifdef DEBUG
bool Debug;
# endif DEBUG

main(argc, argv)
	int argc;
	char *argv[];
{
	int f;
	char line[BUFSIZ];
	char line2[MAXLINE];
	register char *p;
	char *cp, *p2;
	char *rhs;
	int naliases, bytes, longest;
	datum key, content;
	bool skipping;
	addrq al, bl;
	extern char *prescan();
	extern addrq *parse();
	bool contin;

# ifdef DEBUG
	if (argc > 1 && strcmp(argv[1], "-T") == 0)
	{
		Debug++;
		argc--;
		argv++;
	}
# endif DEBUG
	if (argc > 1)
		aliases = argv[1];

	strcpy(dirbuf, aliases);
	strcat(dirbuf, ".dir");
	strcpy(pagbuf, aliases);
	strcat(pagbuf, ".pag");
	f = creat(dirbuf, 0666);
	if (f < 0) {
		perror(dirbuf);
		exit(1);
	}
	close(f);
	f = creat(pagbuf, 0666);
	if (f < 0) {
		perror(pagbuf);
		exit(1);
	}
	close(f);
	if (dbminit(aliases) < 0)
		exit(1);
	if (freopen(aliases, "r", stdin) == 0) {
		perror(aliases);
		exit(1);
	}

	/* read and interpret lines */
	LineNo = 0;
	naliases = 0;
	bytes = 0;
	longest = 0;
	skipping = FALSE;
	while (fgets(line, sizeof (line), stdin) != NULL)
	{
		LineNo++;
		switch (line[0])
		{
		  case '#':
		  case '\n':
		  case '\0':
			skipping = FALSE;
			continue;

		  case ' ':
		  case '\t':
			if (!skipping)
				usrerr("Non-continuation line starts with space");
			skipping = TRUE;
			continue;
		}
		skipping = FALSE;

		/* process the LHS */
		for (p = line; *p != '\0' && *p != ':' && *p != '\n'; p++)
			continue;
		if (*p == '\0' || *p == '\n')
		{
		 syntaxerr:
			usrerr("missing colon");
			continue;
		}
		*p++ = '\0';
		if (parse(line, &al, 1) == NULL)
		{
			*--p = ':';
			goto syntaxerr;
		}
		rhs = cp = p;
		contin = FALSE;
		for (;;)
		{
			register char c;

			/* do parsing & compression of addresses */
			c = *p;
			while (c != '\0')
			{
				p2 = p;
				while (*p != '\n' && *p != ',' && *p != '\0')
					p++;
				c = *p;
				*p++ = '\0';
				if (prescan(p2, cp, &line[sizeof line - 1], ',') == NULL)
					continue;
				contin = FALSE;
				if (parse(cp, &bl, -1) != NULL)
					cp += strlen(cp);
				if (c == ',')
				{
					*cp++ = ',';
					contin = TRUE;
				}
			}

			/* see if there should be a continuation line */
			if (!contin)
				break;

			/* read continuation line */
			if (fgets(line2, sizeof (line2), stdin) == NULL)
				break;
			LineNo++;

			if (!isspace(line2[0]))
				usrerr("continuation line missing");

			p = line2;
		}
		if (al.q_mailer != &Mailer[0])
		{
			usrerr("cannot alias non-local names");
			continue;
		}
		naliases++;
		key.dsize = strlen(al.q_user) + 1;
		key.dptr = al.q_user;
		content.dsize = strlen(rhs) + 1;
		if (content.dsize > longest)
			longest = content.dsize;
		content.dptr = rhs;
		bytes += key.dsize + content.dsize;
		if (store(key, content), 0)
		/* if (f = store(key, content)) */
			usrerr("Dbm internal error return %d from store\n", f);
	}
	fprintf(stderr, "%d aliases, %d bytes, longest %d bytes, %d errors\n",
	    naliases, bytes, longest, Errors);

	exit(ExitStat);
}

usrerr(fmt, a, b, c, d, e)
	char *fmt;
{
	Errors++;
	fprintf(stderr, "line %d: ", LineNo);
	fprintf(stderr, fmt, a, b, c, d, e);
	fprintf(stderr, "\n");
	return (-1);
}

syserr(fmt, a, b, c, d, e)
	char *fmt;
{
	return (usrerr(fmt, a, b, c, d, e));
}
	if (store(key, content), 0)
		/* if (f = store(key, content)) */
			usrerr("Dbm internal error return %d from store\n", f);
	}delivermail/nmakefile                                                                                  660      17       2         5024  2470111566  10213                                                                                                                                                                                                                                                                                                                                                                      #
#  VAX Version
#
#	@(#)makefile	1.10	10/28/80
#
#	Compilation Flags (in CFLAGS):
#		-DDEBUG -- compile in debug stuff.  This is not enabled
#			unless the debug flag is given, so the only
#			advantage in not including this is that the
#			binaries will be smaller.
#		-DLOG -- include log information.  This is probably
#			only useful on systems that include the logger.
#		-DVFORK -- use 'vfork' instead of 'fork'.
#		-DDBM -- use the dbm package instead of scanning the
#			ALIASFILE sequentially.  LIBS must include
#			-ldbm for this to work.
#

LIBS=	-ldbm
TARG=	$(DESTDIR)/etc

OBJS1=	conf.o main.o maketemp.o parse.o alias.o deliver.o \
	savemail.o addr.o err.o
OBJS2=	matchhdr.o sysexits.o util.o bmove.o
SRCS=	useful.h dlvrmail.h \
	conf.c deliver.c main.c parse.c err.c alias.c savemail.c addr.c \
	matchhdr.c sysexits.c util.c bmove.c bmove.11.s bmove.vax.s \
	arpa.c arpadate.c version.c maketemp.c \
	newaliases.c
ALL=	delivermail newaliases

CHOWN=	-echo chown
CHMOD=	chmod
CFLAGS=	-O -DDBM -DVFORK -DDEBUG
LDFLAGS=
AR=	-ar
ARFLAGS=rvu
LINT=	lint
LINTFLAGS=-bxa
XREF=	csh /usr/bin/xref
CP=	cp

GET=	sccs get
DELTA=	sccs delta
REL=

ROOT=	root
OBJMODE=755

all: $(ALL)

delivermail: $(OBJS1) $(OBJS2) version.o
	$(CC) $(LDFLAGS) -o delivermail version.o $(OBJS1) $(OBJS2) $(LIBS)
	$(CHMOD) $(OBJMODE) delivermail
	size delivermail; ls -l delivermail

install: $(OBJS1) $(OBJS2) all
	$(CP) delivermail $(TARG)/delivermail
	install -s newaliases $(DESTDIR)/usr/ucb


$(OBJS1): dlvrmail.h

dlvrmail.h util.o: useful.h

#
#  Auxiliary stuff
#

clean:
	rm -f core delivermail arpa uucp a.out xref newaliases
	rm -f *.o

archive:
	${AR} ${ARFLAGS} delivermail.a READ_ME TO_BE_DONE makefile makefl.* *.h *.c *.s makeversion *.[123456789u]

print: xref
	@ls -l | pr -h "delivermail directory"
	@pr -h "cross reference listing" xref
	@size *.o | pr -h "object code sizes"
	@pr *.h *.[cs]

lint:
	$(LINT) $(LINTFLAGS) $(SRCS)

xref: *.c
	${XREF} *.c > xref

#
#  Data base maintenance routines
#
AOBJS=	newaliases.o parse.o conf.o util.o

newaliases: $(AOBJS)
	$(CC) $(LDFLAGS) $(AOBJS) -o newaliases $(LIBS)

praliases: praliases.c
	$(CC) $(CFLAGS) praliases.c -o praliases $(LIBS)

$(AOBJS): dlvrmail.h

#
#  Auxiliary mailers
#

arpa: arpa.o matchhdr.o arpadate.o
	$(CC) $(LDFLAGS) -o arpa arpa.o matchhdr.o arpadate.o $(LIBS)
	${CHMOD} ${OBJMODE} arpa
	size arpa; ls -l arpa

uucp: uucp.o
	${CC} ${LDFLAGS} -o uucp uucp.o ${LIBS}
	${CHMOD} ${OBJMODE} uucp
	size uucp; ls -l uucp

mail:	mail.o getname.o
	$(CC) $(LDFLAGS) -o mail mail.o getname.o
	size mail; ls -l mail
$(CC) $(LDFLAGS) $(AOBJS) -o newaliases $(LIBS)

praliases: praliases.c
	$(CC) $(CFLAGS) praliases.c -o praliases $(LIBS)

$(AOBJS): dlvrmail.h

#
#  Auxiliary mailers
#

arpa: arpa.o matchhdr.o arpadate.o
	$(CC) $(LDFLAGS) -o arpa arpa.o matchhdr.o arpadate.o $(LIBS)
	${CHMOD} ${OBJMODE} arpa
	size arpa; ls -l arpa

uucp: uucp.o
	${CC} ${LDFLAGS} -o uucp uucp.o ${LIBS}
	${CHMOD} ${OBJMODE} uucp
	size uucp; ls -l uucp

mail:	mail.o getname.o
	$(CC) $(LDFLAGS) -o mail mail.o getname.o
	sidelivermail/parse.c                                                                                    660      17       2        24205  2470111567   7636                                                                                                                                                                                                                                                                                                                                                                      # include <stdio.h>
# include <ctype.h>
# include "dlvrmail.h"

static char	SccsId[] = "@(#)parse.c	2.5	1/8/81";

/*
**  PARSE -- Parse an address
**
**	Parses an address and breaks it up into three parts: a
**	net to transmit the message on, the host to transmit it
**	to, and a user on that host.  These are loaded into an
**	addrq header with the values squirreled away if necessary.
**	The "user" part may not be a real user; the process may
**	just reoccur on that machine.  For example, on a machine
**	with an arpanet connection, the address
**		csvax.bill@berkeley
**	will break up to a "user" of 'csvax.bill' and a host
**	of 'berkeley' -- to be transmitted over the arpanet.
**
**	Parameters:
**		addr -- the address to parse.
**		a -- a pointer to the address descriptor buffer.
**			If NULL, a header will be created.
**		copyf -- determines what shall be copied:
**			-1 -- don't copy anything.  The printname
**				(q_paddr) is just addr, and the
**				user & host are allocated internally
**				to parse.
**			0 -- copy out the parsed user & host, but
**				don't copy the printname.
**			+1 -- copy everything.
**
**	Returns:
**		A pointer to the address descriptor header (`a' if
**			`a' is non-NULL).
**		NULL on error.
**
**	Side Effects:
**		none
**
**	Called By:
**		main
**		sendto
**		alias
**		savemail
*/

# define DELIMCHARS	"()<>@!.,;:\\\" \t\r\n"	/* word delimiters */
# define SPACESUB	('.'|0200)		/* substitution for <lwsp> */

addrq *
parse(addr, a, copyf)
	char *addr;
	register addrq *a;
	int copyf;
{
	register char *p;
	register struct parsetab *t;
	extern struct parsetab ParseTab[];
	static char buf[MAXNAME];
	register char c;
	register char *q;
	bool got_one;
	extern char *prescan();
	extern char *xalloc();
	char **pvp;

	/*
	**  Initialize and prescan address.
	*/

	To = addr;
	if (prescan(addr, buf, &buf[sizeof buf], '\0') == NULL)
		return (NULL);

	/*
	**  Scan parse table.
	**	Look for the first entry designating a character
	**		that is contained in the address.
	**	Arrange for q to point to that character.
	**	Check to see that there is only one of the char
	**		if it must be unique.
	**	Find the last one if the host is on the RHS.
	**	Insist that the host name is atomic.
	**	If just doing a map, do the map and then start all
	**		over.
	*/

 rescan:
	got_one = FALSE;
	for (t = ParseTab; t->p_char != '\0'; t++)
	{
		q = NULL;
		for (p = buf; (c = *p) != '\0'; p++)
		{
			/* find the end of this token */
			while (isalnum(c) || c == '-' || c == '_')
				c = *++p;
			if (c == '\0')
				break;

			if (c == t->p_char)
			{
				got_one = TRUE;

				/* do mapping as appropriate */
				if (flagset(P_MAP, t->p_flags))
				{
					*p = t->p_arg[0];
					if (flagset(P_ONE, t->p_flags))
						goto rescan;
					else
						continue;
				}

				/* arrange for q to point to it */
				if (q != NULL && flagset(P_ONE, t->p_flags))
				{
					usrerr("multichar error");
					ExitStat = EX_USAGE;
					return (NULL);
				}
				if (q == NULL || flagset(P_HLAST, t->p_flags))
					q = p;
			}
			else
			{
				/* insist that host name is atomic */
				if (flagset(P_HLAST, t->p_flags))
					q = NULL;
				else
					break;
			}
		}

		if (q != NULL)
			break;
	}

	/*
	**  If we matched nothing cleanly, but we did match something
	**  somewhere in the process of scanning, then we have a
	**  syntax error.  This can happen on things like a@b:c where
	**  @ has a right host and : has a left host.
	**
	**  We also set `q' to the null string, in case someone forgets
	**  to put the P_MOVE bit in the local mailer entry of the
	**  configuration table.
	*/

	if (q == NULL)
	{
		q = "";
		if (got_one)
		{
			usrerr("syntax error");
			ExitStat = EX_USAGE;
			return (NULL);
		}
	}

	/*
	**  Interpret entry.
	**	t points to the entry for the mailer we will use.
	**	q points to the significant character.
	*/

	if (a == NULL)
		a = (addrq *) xalloc(sizeof *a);
	if (copyf > 0)
	{
		p = xalloc((unsigned) strlen(addr) + 1);
		strcpy(p, addr);
		a->q_paddr = p;
	}
	else
		a->q_paddr = addr;
	a->q_mailer = &Mailer[t->p_mailer];

	if (flagset(P_MOVE, t->p_flags))
	{
		/* send the message to another host & retry */
		a->q_host = t->p_arg;
		if (copyf >= 0)
		{
			p = xalloc((unsigned) strlen(buf) + 1);
			strcpy(p, buf);
			a->q_user = p;
		}
		else
			a->q_user = buf;
	}
	else
	{
		/*
		**  Make local copies of the host & user and then
		**  transport them out.
		*/

		*q++ = '\0';
		if (flagset(P_HLAST, t->p_flags))
		{
			a->q_host = q;
			a->q_user = buf;
		}
		else
		{
			a->q_host = buf;
			a->q_user = q;
		}

		/*
		**  Don't go to the net if already on the target host.
		**	This is important on the berkeley network, since
		**	it get confused if we ask to send to ourselves.
		**	For nets like the ARPANET, we probably will have
		**	the local list set to NULL to simplify testing.
		**	The canonical representation of the name is also set
		**	to be just the local name so the duplicate letter
		**	suppression algorithm will work.
		*/

		if ((pvp = a->q_mailer->m_local) != NULL)
		{
			while (*pvp != NULL)
			{
				auto char buf2[MAXNAME];

				strcpy(buf2, a->q_host);
				if (!flagset(P_HST_UPPER, t->p_flags))
					makelower(buf2);
				if (strcmp(*pvp++, buf2) == 0)
				{
					strcpy(buf2, a->q_user);
					p = a->q_paddr;
					if (parse(buf2, a, -1) == NULL)
					{
						To = addr;
						return (NULL);
					}
					To = a->q_paddr = p;
					break;
				}
			}
		}

		/* make copies if specified */
		if (copyf >= 0)
		{
			p = xalloc((unsigned) strlen(a->q_host) + 1);
			strcpy(p, a->q_host);
			a->q_host = p;
			p = xalloc((unsigned) strlen(a->q_user) + 1);
			strcpy(p, a->q_user);
			a->q_user = p;
		}
	}

	/*
	**  Do UPPER->lower case mapping unless inhibited.
	*/

	if (!flagset(P_HST_UPPER, t->p_flags))
		makelower(a->q_host);
	if (!flagset(P_USR_UPPER, t->p_flags))
		makelower(a->q_user);

	/*
	**  Compute return value.
	*/

# ifdef DEBUG
	if (Debug)
		printf("parse(\"%s\"): host \"%s\" user \"%s\" mailer %d\n",
		    addr, a->q_host, a->q_user, t->p_mailer);
# endif DEBUG

	return (a);
}
/*
**  MAKELOWER -- Translate a line into lower case
**
**	Parameters:
**		p -- the string to translate.  If NULL, return is
**			immediate.
**
**	Returns:
**		none.
**
**	Side Effects:
**		String pointed to by p is translated to lower case.
**
**	Called By:
**		parse
*/

makelower(p)
	register char *p;
{
	register char c;

	if (p == NULL)
		return;
	for (; (c = *p) != '\0'; p++)
		if ((c & 0200) == 0 && isupper(c))
			*p = c - 'A' + 'a';
}
/*
**  PRESCAN -- Prescan name and make it canonical
**
**	Scans a name and turns it into canonical form.  This involves
**	deleting blanks, comments (in parentheses), and turning the
**	word "at" into an at-sign ("@").  The name is copied as this
**	is done; it is legal to copy a name onto itself, since this
**	process can only make things smaller.
**
**	This routine knows about quoted strings and angle brackets.
**
**	There are certain subtleties to this routine.  The one that
**	comes to mind now is that backslashes on the ends of names
**	are silently stripped off; this is intentional.  The problem
**	is that some versions of sndmsg (like at LBL) set the kill
**	character to something other than @ when reading addresses;
**	so people type "csvax.eric\@berkeley" -- which screws up the
**	berknet mailer.
**
**	Parameters:
**		addr -- the name to chomp.
**		buf -- the buffer to copy it into.
**		buflim -- the last usable address in the buffer
**			(which will old a null byte).  Normally
**			&buf[sizeof buf - 1].
**		delim -- the delimiter for the address, normally
**			'\0' or ','; \0 is accepted in any case.
**			are moving in place; set buflim to high core.
**
**	Returns:
**		A pointer to the terminator of buf.
**		NULL on error.
**
**	Side Effects:
**		buf gets clobbered.
**
**	Called By:
**		parse
**		maketemp
*/

char *
prescan(addr, buf, buflim, delim)
	char *addr;
	char *buf;
	char *buflim;
	char delim;
{
	register char *p;
	bool space;
	bool quotemode;
	bool bslashmode;
	bool delimmode;
	int cmntcnt;
	int brccnt;
	register char c;
	register char *q;
	extern bool any();

	space = FALSE;
	delimmode = TRUE;
	q = buf;
	bslashmode = quotemode = FALSE;
	cmntcnt = brccnt = 0;
	for (p = addr; (c = *p++) != '\0'; )
	{
		/* chew up special characters */
		*q = '\0';
		if (bslashmode)
		{
			c |= 0200;
			bslashmode = FALSE;
		}
		else if (c == '"')
			quotemode = !quotemode;
		else if (c == '\\')
		{
			bslashmode++;
			continue;
		}
		else if (quotemode)
			c |= 0200;
		else if (c == delim)
			break;
		else if (c == '(')
		{
			cmntcnt++;
			continue;
		}
		else if (c == ')')
		{
			if (cmntcnt <= 0)
			{
				usrerr("Unbalanced ')'");
				return (NULL);
			}
			else
			{
				cmntcnt--;
				continue;
			}
		}
		if (cmntcnt > 0)
			continue;
		else if (isascii(c) && isspace(c) && (space || delimmode))
			continue;
		else if (c == '<')
		{
			if (brccnt < 0)
			{
				usrerr("multiple < spec");
				return (NULL);
			}
			brccnt++;
			delimmode = TRUE;
			space = FALSE;
			if (brccnt == 1)
			{
				/* we prefer using machine readable name */
				q = buf;
				*q = '\0';
				continue;
			}
		}
		else if (c == '>')
		{
			if (brccnt <= 0)
			{
				usrerr("Unbalanced `>'");
				return (NULL);
			}
			else
				brccnt--;
			if (brccnt <= 0)
			{
				brccnt = -1;
				continue;
			}
		}

		/*
		**  Turn "at" into "@",
		**	but only if "at" is a word.
		**	By the way, I violate the ARPANET RFC-733
		**	standard here, by assuming that 'space' delimits
		**	atoms.  I assume that is just a mistake, since
		**	it violates the spirit of the semantics
		**	of the document.....
		*/

		if (delimmode && (c == 'a' || c == 'A') &&
		    (p[0] == 't' || p[0] == 'T') &&
		    (any(p[1], DELIMCHARS) || p[1] <= 040))
		{
			c = '@';
			p++;
		}

		if (delimmode = any(c, DELIMCHARS))
			space = FALSE;

		/* if not a space, squirrel it away */
		if ((!isascii(c) || !isspace(c)) && brccnt >= 0)
		{
			if (q >= buflim-1)
			{
				usrerr("Address too long");
				return (NULL);
			}
			if (space)
				*q++ = SPACESUB;
			*q++ = c;
		}
		space = isascii(c) && isspace(c);
	}
	*q = '\0';
	if (c == '\0')
		p--;
	if (cmntcnt > 0)
		usrerr("Unbalanced '('");
	else if (quotemode)
		usrerr("Unbalanced '\"'");
	else if (brccnt > 0)
		usrerr("Unbalanced '<'");
	else if (buf[0] != '\0')
		return (p);
	return (NULL);
}
;

		/* if not a space, squirrel it away */
		if ((!isascii(c) || !isspace(c)) && brccnt >= 0)
		{
			if (q >= buflim-1)
			{
				usrerr("Address too long");
				return (NULL);
			}
			if (space)
				*q++ = SPACESUB;
			*q++ = c;
		}
		space = isascii(c) && isspace(c);
	}
	*q = '\0';
	if (c == '\0')
		p--;
	if (cmntcnt > 0)
		usrerr("Unbalanced '('");
	else if (quotemode)
		usdelivermail/savemail.c                                                                                 660      17       2        12655  2470111570  10325                                                                                                                                                                                                                                                                                                                                                                      # include <stdio.h>
# include <pwd.h>
# include "dlvrmail.h"

static char	SccsId[] = "@(#)savemail.c	2.2	1/10/81";

/*
**  SAVEMAIL -- Save mail on error
**
**	If the MailBack flag is set, mail it back to the originator
**	together with an error message; otherwise, just put it in
**	dead.letter in the user's home directory (if he exists on
**	this machine).
**
**	Parameters:
**		none
**
**	Returns:
**		none
**
**	Side Effects:
**		Saves the letter, by writing or mailing it back to the
**		sender, or by putting it in dead.letter in her home
**		directory.
**
**		WARNING: the user id is reset to the original user.
*/

savemail()
{
	register struct passwd *pw;
	register FILE *xfile;
	char buf[MAXLINE+1];
	extern errhdr();
	auto addrq to_addr;
	extern struct passwd *getpwnam();
	register char *p;
	register int i;
	auto long tim;
	extern int errno;
	extern char *ttypath();
	extern char *ctime();
	extern addrq *parse();
	static int exclusive;
	extern char *DaemonName;

	if (exclusive++)
		return;

	/*
	**  In the unhappy event we don't know who to return the mail
	**  to, make someone up.
	*/

	if (From.q_paddr == NULL)
	{
		if (parse("root", &From, 0) == NULL)
		{
			syserr("Cannot parse root!");
			ExitStat = EX_SOFTWARE;
			finis();
		}
	}

	/*
	**  If called from Eric Schmidt's network, do special mailback.
	**	Fundamentally, this is the mailback case except that
	**	it returns an OK exit status (assuming the return
	**	worked).
	*/

	if (BerkNet)
	{
		ExitStat = EX_OK;
		MailBack++;
	}

	/*
	**  If writing back, do it.
	**	If the user is still logged in on the same terminal,
	**	then write the error messages back to hir (sic).
	**	If not, set the MailBack flag so that it will get
	**	mailed back instead.
	*/

	if (WriteBack)
	{
		p = ttypath();
		if (p == NULL || freopen(p, "w", stdout) == NULL)
		{
			MailBack++;
			errno = 0;
		}
		else
		{
			xfile = fopen(Transcript, "r");
			if (xfile == NULL)
				syserr("Cannot open %s", Transcript);
			printf("\r\nMessage from %s\r\n", DaemonName);
			printf("Errors occurred while sending mail, transcript follows:\r\n");
			while (fgets(buf, sizeof buf, xfile) && !ferror(stdout))
				fputs(buf, stdout);
			if (ferror(stdout))
				syserr("savemail: stdout: write err");
			fclose(xfile);
		}
	}

	/*
	**  If mailing back, do it.
	**	Throw away all further output.  Don't do aliases, since
	**	this could cause loops, e.g., if joe mails to x:joe,
	**	and for some reason the network for x: is down, then
	**	the response gets sent to x:joe, which gives a
	**	response, etc.  Also force the mail to be delivered
	**	even if a version of it has already been sent to the
	**	sender.
	*/

	if (MailBack || From.q_mailer != &Mailer[0])
	{
		freopen("/dev/null", "w", stdout);
		NoAlias++;
		ForceMail++;

		/* fake up an address header for the from person */
		bmove((char *) &From, (char *) &to_addr, sizeof to_addr);
		if (parse(DaemonName, &From, -1) == NULL)
		{
			syserr("Can't parse myself!");
			ExitStat = EX_SOFTWARE;
			finis();
		}
		i = deliver(&to_addr, errhdr);
		bmove((char *) &to_addr, (char *) &From, sizeof From);
		if (i != 0)
			syserr("Can't return mail to %s", p);
		else
			return;
	}

	/*
	**  Save the message in dead.letter.
	**	If we weren't mailing back, and the user is local, we
	**	should save the message in dead.letter so that the
	**	poor person doesn't have to type it over again --
	**	and we all know what poor typists programmers are.
	*/

	setuid(getuid());
	setgid(getgid());
	setpwent();
	if (From.q_mailer == &Mailer[0] && (pw = getpwnam(From.q_user)) != NULL)
	{
		/* user has a home directory */
		p = pw->pw_dir;
	}
	else
	{
		syserr("Can't return mail to %s (pw=%u)", From.q_paddr, pw);
# ifdef DEBUG
		p = "/usr/tmp";
# else
		p = NULL;
# endif
	}
	if (p != NULL)
	{
		/* we have a home directory; open dead.letter */
		strcpy(buf, p);
		strcat(buf, "/dead.letter");
		xfile = fopen(buf, "a");
		if (xfile == NULL)
			printf("Cannot save mail, sorry\n");
		else
		{
			rewind(stdin);
			errno = 0;
			time(&tim);
			fprintf(xfile, "----- Mail saved at %s", ctime(&tim));
			while (fgets(buf, sizeof buf, stdin) && !ferror(xfile))
				fputs(buf, xfile);
			fputs("\n", xfile);
			if (ferror(xfile))
				syserr("savemail: dead.letter: write err");
			fclose(xfile);
			printf("Letter saved in dead.letter\n");
		}
	}
	else

	/* add terminator to writeback message */
	if (WriteBack)
		printf("-----\r\n");
}
/*
**  ERRHDR -- Output the header for error mail.
**
**	This is the edit filter to error mailbacks.
**
**	Algorithm:
**		Output fixed header.
**		Output the transcript part.
**		Output the original message.
**
**	Parameters:
**		xfile -- the transcript file.
**		fp -- the output file.
**
**	Returns:
**		none
**
**	Side Effects:
**		input from xfile
**		output to fp
**
**	Called By:
**		deliver
*/


errhdr(fp)
	register FILE *fp;
{
	char copybuf[512];
	register int i;
	register int xfile;
	extern int errno;

	if ((xfile = open(Transcript, 0)) < 0)
		syserr("Cannot open %s", Transcript);
	fflush(stdout);
	errno = 0;
	fprintf(fp, "To: %s\n", To);
	fprintf(fp, "Subject: Unable to deliver mail\n");
	fprintf(fp, "\n   ----- Transcript of session follows -----\n");
	fflush(fp);
	while ((i = read(xfile, copybuf, sizeof copybuf)) > 0)
		write(fileno(fp), copybuf, i);
	fprintf(fp, "\n   ----- Unsent message follows -----\n");
	fflush(fp);
	rewind(stdin);
	while ((i = read(fileno(stdin), copybuf, sizeof copybuf)) > 0)
		write(fileno(fp), copybuf, i);
	close(xfile);
	if (errno != 0)
		syserr("errhdr: I/O error");
}

	errno = 0;
	fprintf(fp, "To: %s\n", To);
	fprintf(fp, "Subject: Unable to deliverdelivermail/showdbm.c                                                                                  660      17       2         1463  2470111571  10143                                                                                                                                                                                                                                                                                                                                                                      # include "dlvrmail.h"

static	char sccsid[] = "@(#)showdbm.c 2.1 11/5/80";

typedef struct { char *dptr; int dsize; } datum;
datum	firstkey(), nextkey(), fetch();
char	*filename = ALIASFILE;

main(argc, argv)
	char **argv;
{
	datum content, key;

	if (argc > 2 && strcmp(argv[1], "-f") == 0)
	{
		argv++;
		filename = *++argv;
		argc -= 2;
	}

	if (dbminit(filename) < 0)
		exit(EX_OSFILE);
	argc--, argv++;
	if (argc == 0) {
		for (key = firstkey(); key.dptr; key = nextkey(key)) {
			content = fetch(key);
			printf("\n%s:%s\n", key.dptr, content.dptr);
		}
		exit(EX_OK);
	}
	while (argc) {
		key.dptr = *argv;
		key.dsize = strlen(*argv)+1;
		content = fetch(key);
		if (content.dptr == 0)
			printf("%s: No such key\n");
		else
			printf("\n%s:%s\n", key.dptr, content.dptr);
		argc--, argv++;
	}
	exit(EX_OK);
}
	filename = *++argv;
		argc -= 2;
	}

	if (dbminit(filename) < 0)
		exit(EX_OSFILE);
	argc--, argv++;
	if (argc == 0) {
		for (key = firstkey(); key.dptr; key = nextkey(key)) {
			content = fetch(key);
			delivermail/sysexits.c                                                                                 660      17       2         1042  2470111571  10364                                                                                                                                                                                                                                                                                                                                                                      # include <sysexits.h>

static char	SccsId[] = "@(#)sysexits.c	2.1	11/5/80";

/*
**  SYSEXITS.C -- error messages corresponding to sysexits.h
*/

char	*SysExMsg[] =
{
	/* 64 */	"Bad usage",
	/* 65 */	"Data format error",
	/* 66 */	"Cannot open input",
	/* 67 */	"User unknown",
	/* 68 */	"Host unknown",
	/* 69 */	"Service unavailable",
	/* 70 */	"Internal error",
	/* 71 */	"Operating system error",
	/* 72 */	"System file missing",
	/* 73 */	"Can't create output",
	/* 74 */	"I/O error",
};

int	N_SysEx = sizeof SysExMsg / sizeof SysExMsg[0];
r	SccsId[] = "@(#)sysexits.c	2.1	11/5/80";

/*
**  SYSEXITS.C -- error messages corresponding to sysexits.h
*/

char	*SysExMsg[] =
{
	/* 64 */	"Bad usage",
	/* 65 */	"Data format error",
	/* 66 */	"Cannot open input",
	/* 67 */	"User unknown",
	/* 68 */	"Host unknown",
	/* 69 */	"Service unavailable",
	/* 70 */	"Internal error",
	/* 71 */	"Operating system error",
	/* 72 */	"System file missing",
	/* 73 */	"Can't create output",
	/* 74 */	"I/O error",
};

int	N_SysEx = sizedelivermail/syslog.8                                                                                   660      17       2         6425  2470111572   7751                                                                                                                                                                                                                                                                                                                                                                      .TH SYSLOG 8
.SH NAME
syslog \- log systems messages
.SH SYNOPSIS
/etc/syslog < /dev/logx
.SH DESCRIPTION
.I Syslog
reads its standard input
and logs each line it reads into a set of files
described by the configuration file
/etc/syslog.config.
.I Syslog
configures when it starts up
and whenever it receives a hangup signal.
.PP
Each message is one line.
A message can contain a priority code,
marked by a digit in angle braces
at the beginning of the line.
Priorities are defined in <log.h>, as follows:
.IP LOG_ALERT \w'LOG_WARNING'u+2n
this priority should essentially never
be used.  It applies only to messages that
are so important that every user should be
aware of them, e.g., a serious hardware failure.
.IP LOG_SALERT
messages of this priority should be
issued only when immediate attention is needed
by a qualified system person, e.g., when some
valuable system resource dissappears.  They get
sent to a list of system people.
.IP LOG_EMERG
Emergency messages are not sent to users,
but represent major conditions.  An example
might be hard disk failures.  These could be
logged in a separate file so that critical
conditions could be easily scanned.
.IP LOG_ERR
these represent error conditions, such as soft
disk failures, etc.
.IP LOG_CRIT
such messages contain critical information,
but which can not be classed as errors, for example,
'su' attempts.
Messages of this priority and higher
are typically logged on the system console.
.IP LOG_WARNING
issued when an abnormal condition has been
detected, but recovery can take place.
.IP LOG_NOTICE
something that falls in the class of
"important information"; this class is informational
but important enough that you don't want to throw
it away casually.
Messages without any priority assigned to them
are typically mapped into this priority.
.IP LOG_INFO
information level messages.  These messages
could be thrown away without problems, but should
be included if you want to keep a close watch on
your system.
.IP LOG_DEBUG
it may be useful to log certain debugging
information.  Normally this will be thrown away.
.PP
It is expected that the kernel will not log anything below
LOG_ERR priority.
.PP
The configuration file is in two sections
seperated by a blank line.
The first section defines files that
.I syslog
will log into.
Each line contains
a single digit which defines the lowest priority
(highest numbered priority)
that this file will receive,
an optional asterisk
which guarantees that something gets output
at least every 20 minutes,
and a pathname.
The second part of the file
contains a list of users that will be
informed on SALERT level messages.
For example, the configuration file:
.nf
	5*/dev/tty8
	8/usr/spool/adm/syslog
	3/usr/adm/critical

	eric
	kridle
	birman
.fi
logs all messages of priority 5 or higher
onto the system console,
including timing marks every 20 minutes;
all messages of priority 8 or higher
into the file /usr/spool/adm/syslog;
and all messages of priority 3 or higher
into /usr/adm/critical.
The users ``eric'', ``kridle'', and ``birman''
will be informed on any subalert messages.
.PP
To bring
.I syslog
down,
it should be sent a terminate signal.
It logs that it is going down
and then waits approximately 30 seconds
for any additional messages to come in.
.SH FILES
/etc/syslog.config \- the configuration file
.SH SEE\ ALSO
logmsg(3)
ng marks every 20 minutes;
all messages of priority 8 or higher
into the file /usr/spool/adm/syslog;
and all messages of priority 3 or higher
into /usr/adm/critical.
The users ``eric'', ``kridle'', and ``birman''
will be informed on andelivermail/useful.h                                                                                   660      17       2          251  2470111572   7763                                                                                                                                                                                                                                                                                                                                                                      /*
**  USEFUL.H -- Some useful stuff.
**
**	@(#)useful.h	2.1	11/5/80
*/

# define bool	char
# define TRUE	1
# define FALSE	0

# ifndef NULL
# define NULL	0
# endif NULL
itional messages to come in.
.SH FILES
/etc/syslog.config \- the configuration file
.SH SEE\ ALSO
logmsg(3)
ng marks every 20 minutes;
all messages of priority 8 or higher
into the file /usr/spool/adm/syslog;
and all messages of priority 3 or higher
into /usr/adm/critical.
The users ``eric'', ``kridle'', and ``birman''
will be informed on andelivermail/userinfo.5                                                                                 660      17       2         2523  2470111573  10254                                                                                                                                                                                                                                                                                                                                                                      .TH USERINFO 5 1/22/80
.SH NAME
userinfo \- per-user database of personal information
.SH SYNOPSIS
~/.userinfo
.SH DESCRIPTION
The
.I \&.userinfo
database contains a set of miscellaneous information
about each user.
It is formatted as a sequence of lines
of the form ``field-name: value''.
.I Field-name
must begin in the first column,
but there may be spaces before or after the colon.
Spaces in the value field may be significant or not,
depending on the semantics of the particular field.
Fields may be in any order.
Case in field names is ignored.
.PP
The following fields are defined:
.IP name \w'office-address'u+3n
The user's full name.
.IP office-address
The user's office address.
.IP office-phone
The user's office phone number.
.IP home-address
The home address;
lines of the address are separated with semicolons.
.IP home-phone
The home phone number.
.IP forward-to
The address to forward any incoming mail to.
.IP project
The project that this user is associated with.
.IP advisor
This user's advisor or supervisor.
.IP affiliation
A business or technical affiliation
(for outside users).
.PP
Other fields may be defined as needed.
.SH EXAMPLE
.nf
NAME: Eric Allman
Forward-To: ingres:eric
home-phone: (415) 843-9461
office-address: 380 Cory
office-phone: 2-7520
Project: INGRES
Advisor: Mike Stonebraker
.fi
.SH SEE\ ALSO
finger(1), delivermail(8)
hone number.
.IP forward-to
The address to forward any incoming mail to.
.IP project
The project that this user is associated with.
.IP advisor
This user's advisor or supervdelivermail/util.c                                                                                     660      17       2         3071  2470111573   7454                                                                                                                                                                                                                                                                                                                                                                      # include <sysexits.h>
# include "useful.h"

static char	SccsId[] = "@(#)util.c	2.1	11/5/80";

/*
**  STRIPQUOTES -- Strip quotes & quote bits from a string.
**
**	Runs through a string and strips off unquoted quote
**	characters and quote bits.  This is done in place.
**
**	Parameters:
**		s -- the string to strip.
**
**	Returns:
**		none.
**
**	Side Effects:
**		none.
**
**	Called By:
**		deliver
*/

stripquotes(s)
	char *s;
{
	register char *p;
	register char *q;
	register char c;

	for (p = q = s; (c = *p++) != '\0'; )
	{
		if (c != '"')
			*q++ = c & 0177;
	}
	*q = '\0';
}
/*
**  XALLOC -- Allocate memory and bitch wildly on failure.
**
**	THIS IS A CLUDGE.  This should be made to give a proper
**	error -- but after all, what can we do?
**
**	Parameters:
**		sz -- size of area to allocate.
**
**	Returns:
**		pointer to data region.
**
**	Side Effects:
**		Memory is allocated.
**
**	Called By:
**		lots of people.
*/

char *
xalloc(sz)
	register unsigned int sz;
{
	register char *p;
	extern char *malloc();

	p = malloc(sz);
	if (p == NULL)
	{
		syserr("Out of memory!!");
		exit(EX_UNAVAILABLE);
	}
	return (p);
}
/*
**  ANY -- Return TRUE if the character exists in the string.
**
**	Parameters:
**		c -- the character.
**		s -- the string
**			(sounds like an avant garde script)
**
**	Returns:
**		TRUE -- if c could be found in s.
**		FALSE -- otherwise.
**
**	Side Effects:
**		none.
**
**	Called By:
**		prescan
*/

any(c, s)
	register char c;
	register char *s;
{
	register char c2;

	while ((c2 = *s++) != '\0')
		if (c2 == c)
			return (TRUE);
	return (FALSE);
}
emory!!");
		exit(EX_UNAVAILABLE);
	}
	return (p);
}
/*
**  ANY -- Return TRUE if the character exists in the string.
**
**	Parameters:
**		c -- the character.
**		s -- the string
**			(sounds like an avant garde script)
**
**	Returns:
**		TRUE -- if c could be found in s.
**		FALSE -- otherwise.
**
**	Side Effects:
**		none.
**
**	Called By:
**		prescan
*/

any(c, s)
	register char c;
	register char *s;
{
	register char c2;

	while ((c2 = *s++) != 'delivermail/v6-mail.c                                                                                  660      17       2        21712  2470111575   7776                                                                                                                                                                                                                                                                                                                                                                      #

/*
 * mail command usage
 *	mail [-yn]
 *		prints your mail
 *	mail people
 *		sends standard input to people
 *
 *	mail -r machine user people
 *		sends mail from the network
 *
 * if NOTROOT is defined, don't run as root.
 */

#define	SIGINT		2
#define DIRECT		040000
#define RMAILCMD	"/usr/net/bin/sendmail"
#define GETUID()	(getuid() & 0377)
#define	SPOOLDIR	"/usr/spool/mail/"
#define NOTROOT		$

struct inode {
	char minor;
	char major;
	int inumber;
	int flags;
	char nlinks;
	char uid;
	char gid;
	char size0;
	int size1;
	int addr[8];
	int actime[2];
	int modtime[2];
} inode;

char	lettmp[] "/tmp/maXXXXX";
char	preptmp[] "/tmp/mbXXXXX";
int	pwfil;
int	chew;
int	errs;
char	*strcat(), *strcpy();

main(argc, argv)
char **argv;
{
	register int me;
	extern int fout;
	int uf, delexit();
	char namebuf[20];

	mktemp(lettmp);
	mktemp(preptmp);
	unlink(lettmp);
	unlink(preptmp);
	me = GETUID();
	if (getname(me, namebuf) < 0) {
		printf("Who are you?\n");
		delexit(1);
	}
	if (argc < 2)
		goto hitit;
	for (argc--, argv++; argc > 0 && argv[0][0] == '-'; argc--, argv++)
	switch(argv[0][1]) {
		register char *cp, *np;

		case 'y':
		case 'n':
			argc++, argv--;
hitit:
			printmail(argc, argv, namebuf);
			delexit(0);

		case 'r':
			if (argc < 2)
				continue;
		case 'f':
			if (argc < 1)
				continue;
			if (!equal("network", namebuf) && me != 0) {
				printf("Nice try!\n");
				delexit(1);
			}
			chew++;
			np = namebuf;
			for (cp = argv[1]; *cp; cp++)
				*np++ = *cp;
			if (argv[0][1] == 'r')
			{
				argc--, argv++;
				*np++ = ':';
				for (cp = argv[1]; *cp; cp++)
					*np++ = *cp;
			}
			*np++ = 0;
			argc--, argv++;
			continue;
	}
	if ((signal(SIGINT, 01) & 01) == 0)
		signal(SIGINT, delexit);
	unlink(lettmp);
# ifdef NOTROOT
	fout = creat(lettmp, 0666);
# else
	fout = creat(lettmp, 0600);
# endif
	if (fout < 0) {
		fout = 1;
		perror(lettmp);
		delexit(1);
	}
	argc++, argv--;
	bulkmail(argc, argv, namebuf);
	delexit(0);
}

printmail(argc, argv, name)
char **argv;
char *name;
{
	extern int fin, fout;
	register n, c, f;
	char *mname;

	mname = cat(SPOOLDIR, name);
	if (stat(mname, &inode)>=0 && inode.nlinks==1 &&
		fopen(mname, &fin)>=0 && (c = getchar())) {
		putchar(c);
		getput();
		close(fin);
		c = 'x';
		if (argc<2) {
			if (ttyn(0)!='x') {
				printf("Save?");
				fin = 0;
				c = getchar();
			}
		} else
			c = argv[1][1];
		if (!any(c, "yn"))
			delexit(0);
		if (c == 'y') {
			if (accesss("mbox")) {
				printf("Saved mail in 'mbox'\n");
				prepend(mname, "mbox", GETUID());
				unlink(mname);
			} else
				printf("In wrong directory\n");
		} else
			unlink(mname);
	} else
		printf("No mail.\n");
}

bulkmail(argc, argv, from)
char **argv, *from;
{
	extern int fin, fout;
	register int c;
	register char *cp;
	char linebuf[128];
	int tbuf[2], ttyn1;

	fin = 0;
	(&fin)[1] = 0;
	time(tbuf);
	ttyn1 = ttyn(1);
	if (ttyn1 < 033) {
		ttyn1 =+ 'a' - 1;
		ttyn1 =<< 8;
		ttyn1 =| '^';
	}
	printf("From %s  tty%c  %s", from, ttyn1, ctime(tbuf));

	/*
	 * If delivering mail from the network via mail -r,
	 * Strip the leading line and throw it away, as long
	 * as it begins with "From ..."
	 */

	if (chew) {
		cp = linebuf;
		do {
			c = getchar();
			if (cp - linebuf < 120)
				*cp++ = c;
		} while (c != '\n' && c != 0);
		*cp = '\0';
		if (linebuf[0] != 'F' || linebuf[1] != 'r' ||
		    linebuf[2] != 'o' || linebuf[3] != 'm')
			printf("%s", linebuf);
	}
	getput();
	putchar('\n');
	flush();
	close(fout);
	while (--argc > 0)
		sendto(*++argv);
	delexit(errs);
}

sendto(person)
char *person;
{
	static int saved;
	extern int fout, fin;
	register char *filep;
	register int him;
	int i;

	if ((person[0] == 'i' || person[0] == 'I') && person[1] == ':')
		person += 2;
	for (i = 0; person[i] != '\0'; i++)
	{
		if (person[i] == ':')
		{
			person[i] = '\0';
			if (equal(person, "ing70") || equal(person, "ingres"))
				person += i + 1;
			else
				person[i] = ':';
			break;
		}
	}
	if (person[i] == ':' || equal(person, "msgs"))
	{
		int i = fork();
		int s;

		if (i < 0) {
			perror("fork");
			goto assback;
		}
		if (i == 0) {
			close(0);
			open(lettmp, 0);
			if (any(':', person)) {
				execl(RMAILCMD, "sendmail", person, 0);
				execl("/usr/bin/sendmail", "sendmail", person, 0);
				execl("/bin/sendmail", "sendmail", person, 0);
				perror("sendmail");
			} else {
				execl("/usr/new/msgs", "msgs", "-s", 0);
				execl("/usr/ucb/msgs", "msgs", "-s", 0);
				execl("/usr/bin/msgs", "msgs", "-s", 0);
			}
			exit(12);
		}
		for (;;) {
			register int j = wait(&s);
			if (j == -1)
				goto assback;
			if (j == i)
				break;
		}
		if ((s & 0377) != 0 || (s >> 8) == 12)
			goto assback;
		return;
	}

	if ((him = getuserid(person)) == -1) {
assback:
		fout = 1;
		flush();
		printf("Can't send to %s.\n", person);
		errs++;
		if (ttyn(0)!='x' && saved==0) {
			saved++;
			if (accesss("dead.letter")) {
				printf("Letter saved in 'dead.letter'\n");
				prepend(lettmp, "dead.letter", GETUID());
			} else
				printf("In wrong directory\n");
		}
		return;
	}
	filep = cat(SPOOLDIR, person);
	lock(filep);
	prepend(lettmp, filep, him);
	unlock();
}

prepend(from, to, own)
char *from, *to;
{
	extern int fin, fout;
	register int sig;
	int statb[18];

	if (stat(to, statb) >= 0 && (statb[2] & 060000) != 0) {
		write(2, "Exotic destination\n", 19);
		delexit(1);
	}
	unlink(preptmp);
	if (fcreat(preptmp, &fout) < 0) {
		fout = 1;
		perror("mail");
		delexit(1);
	}
	chmod(preptmp, 0600);
	if (fopen(from, &fin) < 0) {
		close(fout);
		fout = 1;
		perror("mail");
		unlink(preptmp);
		return(0);
	}
	getput();
	close(fin);
	fopen(to, &fin);
	getput();
	close(fin);
	flush();
	close(fout);
	sig = signal(SIGINT, 01);
	unlink(to);
	if (fcreat(to, &fout) < 0) {
		unlink(preptmp);
		fout = 1;
		signal(SIGINT, sig);
		return(0);
	}
# ifdef NOTROOT
	chmod(to, 0666);
# else
	chmod(to, 0600);
	chown(to, own);
# endif
	if(stat(to, &inode) < 0 || inode.nlinks != 1) {
		close(fout);
		fout = 1;
		unlink(preptmp);
		signal(SIGINT, sig);
		return(0);
	}
	if (fopen(preptmp, &fin) < 0) {
		fout = 1;
		perror("mail");
		signal(SIGINT, sig);
		errs++;
		return(0);
	}
	getput();
	flush();
	close(fout);
	close(fin);
	fout = 1;
	signal(SIGINT, sig);
	return(1);
}

delexit(ex)
{
	unlock();
	unlink(lettmp);
	unlink(preptmp);
	exit(ex);
}

equal(as1, as2)
{
	register char *s1, *s2;

	s1 = as1;
	s2 = as2;
	while (*s1++ == *s2)
		if (*s2++ == 0)
			return(1);
	return(0);
}

cat(ap1, ap2)
char *ap1, *ap2;
{
	register char *p1, *p2;
	static char fn[32];

	p1 = ap1;
	p2 = fn;
	while (*p2++ = *p1++);
	p2--;
	p1 = ap2;
	while (*p2++ = *p1++);
	return(fn);
}

getput()
{
	extern int errno;
	register c;

	while(c = getchar()) {
		errno = 0;
		putchar(c);
		if(errno) {
			perror("mail");
			delexit(1);
		}
	}
}

accesss(s1)
{
	if (access(".", 2) != -1 && (stat(s1, &inode)<0 || access(s1, 2)==0))
		return(1);
	return(0);
}

any(c, str)
	char *str;
{
	register char *f;

	f = str;
	while (*f)
		if (c == *f++)
			return(1);
	return(0);
}

char	*maillock	= ".lock";		/* Lock suffix for mailname */
char	*lockname	= "/usr/spool/mail/tmXXXXXX";
char	locktmp[30];				/* Usable lock temporary */
char	curlock[50];				/* Last used name of lock */
int	locked;					/* To note that we locked it */

/*
 * Lock the specified mail file by setting the file mailfile.lock.
 * We must, of course, be careful to unlink the lock file by a call
 * to unlock before we stop.  The algorithm used here is to see if
 * the lock exists, and if it does, to check its modify time.  If it
 * is older than 30 seconds, we assume error and set our own file.
 * Otherwise, we wait for 5 seconds and try again.
 */

lock(file)
char *file;
{
	register int f;
	long age;
	struct inode sbuf;
	long curtime;

	if (file == (char *) 0) {
		printf("Locked = %d\n", locked);
		return(0);
	}
	if (locked)
		return(0);
	strcpy(curlock, file);
	strcat(curlock, maillock);
	strcpy(locktmp, lockname);
	mktemp(locktmp);
	unlink(locktmp);
	for (;;) {
		f = lock1(locktmp, curlock);
		if (f == 0) {
			locked = 1;
			return(0);
		}
		if (stat(curlock, &sbuf) < 0)
			return(0);
		time(&curtime);
		age = * ((long *) sbuf.modtime);
		if (curtime < age + 30) {
			sleep(5);
			continue;
		}
		unlink(curlock);
	}
}

/*
 * Remove the mail lock, and note that we no longer
 * have it locked.
 */

unlock()
{

	if (locked)
		unlink(curlock);
	locked = 0;
}

/*
 * Attempt to set the lock by creating the temporary file,
 * then doing a link/unlink.  If it fails, return -1 else 0
 */

lock1(tempfile, name)
	char tempfile[], name[];
{
	register int fd;

	fd = creat(tempfile, 0);
	if (fd < 0)
		return(-1);
	close(fd);
	if (link(tempfile, name) < 0) {
		unlink(tempfile);
		return(-1);
	}
	unlink(tempfile);
	return(0);
}

/*
 * Concatenate s2 on the end of s1.  S1's space must be large enough.
 * Return s1.
 */

char *
strcat(s1, s2)
register char *s1, *s2;
{
	register os1;

	os1 = s1;
	while (*s1++)
		;
	*--s1;
	while (*s1++ = *s2++)
		;
	return(os1);
}

/*
 * Copy string s2 to s1.  s1 must be large enough.
 * return s1
 */

char *
strcpy(s1, s2)
register char *s1, *s2;
{
	register os1;

	os1 = s1;
	while (*s1++ = *s2++)
		;
	return(os1);
}
name) < 0) {
		unlink(tempfile);
		return(-1);
	}
	unldelivermail/vax-mail.c                                                                                 660      17       2        40330  3207526073  10240                                                                                                                                                                                                                                                                                                                                                                      #ifndef	lint
static char sccsid[] = "@(#)mail.c	4.18 (Berkeley) 9/9/83";
#endif

#include <ctype.h>
#include <stdio.h>
#include <pwd.h>
#include <utmp.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <setjmp.h>
#include <whoami.h>
#include <sysexits.h>

#define DELIVERMAIL	"/etc/delivermail"


/*copylet flags */
	/*remote mail, add rmtmsg */
#define REMOTE	1
	/* zap header and trailing empty line */
#define ZAP	3
#define ORDINARY 2
#define	FORWARD	4
#define	LSIZE	256
#define	MAXLET	300	/* maximum number of letters */
#define	MAILMODE (~0600)		/* mode of created mail */
# ifndef DELIVERMAIL
#define	MSGS	"/usr/ucb/msgs"
#define	RMAIL	"/usr/net/bin/sendberkmail"
#define LOCNAM1	"csvax"
#define LOCNAM2	"ucbvax"
#define LOCNAM3	"vax"
#define LOCNAM4	"v"
# endif

char	line[LSIZE];
char	resp[LSIZE];
struct let {
	long	adr;
	char	change;
} let[MAXLET];
int	nlet	= 0;
char	lfil[50];
long	iop, time();
char	*getenv();
char	*index();
char	lettmp[] = "/tmp/maXXXXX";
char	maildir[] = "/usr/spool/mail/";
char	mailfile[] = "/usr/spool/mail/xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
char	dead[] = "dead.letter";
char	forwmsg[] = " forwarded\n";
FILE	*tmpf;
FILE	*malf;
char	*my_name;
struct	passwd	*getpwuid();
int	error;
int	changed;
int	forward;
char	from[] = "From ";
long	ftell();
int	delete();
char	*ctime();
int	flgf;
int	flgp;
int	delflg = 1;
int	hseqno;
jmp_buf	sjbuf;
int	rmail;
char	hostname[32];

main(argc, argv)
char **argv;
{
	register i;
	char sobuf[BUFSIZ];

	setbuf(stdout, sobuf);
	mktemp(lettmp);
	unlink(lettmp);
	if (my_name == NULL || strlen(my_name) == 0) {
		struct passwd *pwent;
		pwent = getpwuid(getuid());
		if (pwent==NULL)
			my_name = "???";
		else
			my_name = pwent->pw_name;
	}
	if(setjmp(sjbuf)) done();
	for (i = SIGHUP; i <= SIGTERM; i++)
		setsig(i, delete);
	tmpf = fopen(lettmp, "w");
	if (tmpf == NULL) {
		fprintf(stderr, "mail: cannot open %s for writing\n", lettmp);
		done();
	}
	if (argv[0][0] == 'r')
		rmail++;
	if (argv[0][0] != 'r' &&	/* no favors for rmail*/
	   (argc == 1 || argv[1][0] == '-' && !any(argv[1][1], "rhd")))
		printmail(argc, argv);
	else
		sendmail(argc, argv);
	done();
}

setsig(i, f)
int i;
int (*f)();
{
	if(signal(i, SIG_IGN) != SIG_IGN)
		signal(i, f);
}

any(c, str)
	register int c;
	register char *str;
{

	while (*str)
		if (c == *str++)
			return(1);
	return(0);
}

printmail(argc, argv)
char **argv;
{
	int flg, i, j, print;
	char *p, *getarg();
	struct stat statb;

	setuid(getuid());
	cat(mailfile, maildir, my_name);
	if (stat(mailfile, &statb) >= 0
	    && (statb.st_mode & S_IFMT) == S_IFDIR) {
		strcat(mailfile, "/");
		strcat(mailfile, my_name);
	}
	for (; argc>1; argv++, argc--) {
		if (argv[1][0]=='-') {
			if (argv[1][1]=='q')
				delflg = 0;
			else if (argv[1][1]=='p') {
				flgp++;
				delflg = 0;
			} else if (argv[1][1]=='f') {
				if (argc>=3) {
					strcpy(mailfile, argv[2]);
					argv++;
					argc--;
				}
			} else if (argv[1][1]=='r') {
				forward = 1;
			} else if (argv[1][1]=='h') {
				forward = 1;
			} else {
				fprintf(stderr, "mail: unknown option %c\n", argv[1][1]);
				done();
			}
		} else
			break;
	}
	malf = fopen(mailfile, "r");
	if (malf == NULL) {
		fprintf(stdout, "No mail.\n");
		return;
	}
	lock(mailfile);
	copymt(malf, tmpf);
	fclose(malf);
	fclose(tmpf);
	unlock();
	tmpf = fopen(lettmp, "r");

	changed = 0;
	print = 1;
	for (i = 0; i < nlet; ) {
		j = forward ? i : nlet - i - 1;
		if(setjmp(sjbuf)) {
			print=0;
		} else {
			if (print)
				copylet(j, stdout, ORDINARY);
			print = 1;
		}
		if (flgp) {
			i++;
			continue;
		}
		setjmp(sjbuf);
		fprintf(stdout, "? ");
		fflush(stdout);
		if (fgets(resp, LSIZE, stdin) == NULL)
			break;
		switch (resp[0]) {

		default:
			fprintf(stderr, "usage\n");
		case '?':
			print = 0;
			fprintf(stderr, "q\tquit\n");
			fprintf(stderr, "x\texit without changing mail\n");
			fprintf(stderr, "p\tprint\n");
			fprintf(stderr, "s[file]\tsave (default mbox)\n");
			fprintf(stderr, "w[file]\tsame without header\n");
			fprintf(stderr, "-\tprint previous\n");
			fprintf(stderr, "d\tdelete\n");
			fprintf(stderr, "+\tnext (no delete)\n");
			fprintf(stderr, "m user\tmail to user\n");
			fprintf(stderr, "! cmd\texecute cmd\n");
			break;

		case '+':
		case 'n':
		case '\n':
			i++;
			break;
		case 'x':
			changed = 0;
		case 'q':
			goto donep;
		case 'p':
			break;
		case '^':
		case '-':
			if (--i < 0)
				i = 0;
			break;
		case 'y':
		case 'w':
		case 's':
			flg = 0;
			if (resp[1] != '\n' && resp[1] != ' ') {
				printf("illegal\n");
				flg++;
				print = 0;
				continue;
			}
			if (resp[1] == '\n' || resp[1] == '\0') {
				p = getenv("HOME");
				if(p != 0)
					cat(resp+1, p, "/mbox");
				else
					cat(resp+1, "", "mbox");
			}
			for (p = resp+1; (p = getarg(lfil, p)) != NULL; ) {
				malf = fopen(lfil, "a");
				if (malf == NULL) {
					fprintf(stdout, "mail: cannot append to %s\n", lfil);
					flg++;
					continue;
				}
				copylet(j, malf, resp[0]=='w'? ZAP: ORDINARY);
				fclose(malf);
			}
			if (flg)
				print = 0;
			else {
				let[j].change = 'd';
				changed++;
				i++;
			}
			break;
		case 'm':
			flg = 0;
			if (resp[1] == '\n' || resp[1] == '\0') {
				i++;
				continue;
			}
			if (resp[1] != ' ') {
				printf("invalid command\n");
				flg++;
				print = 0;
				continue;
			}
			for (p = resp+1; (p = getarg(lfil, p)) != NULL; )
				if (!sendrmt(j, lfil, "/bin/mail"))	/* couldn't send it */
					flg++;
			if (flg)
				print = 0;
			else {
				let[j].change = 'd';
				changed++;
				i++;
			}
			break;
		case '!':
			system(resp+1);
			printf("!\n");
			print = 0;
			break;
		case 'd':
			let[j].change = 'd';
			changed++;
			i++;
			if (resp[1] == 'q')
				goto donep;
			break;
		}
	}
   donep:
	if (changed)
		copyback();
}

copyback()	/* copy temp or whatever back to /usr/spool/mail */
{
	register i, n, c;
	int new = 0;
	struct stat stbuf;

	signal(SIGINT, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	lock(mailfile);
	stat(mailfile, &stbuf);
	if (stbuf.st_size != let[nlet].adr) {	/* new mail has arrived */
		malf = fopen(mailfile, "r");
		if (malf == NULL) {
			fprintf(stdout, "mail: can't re-read %s\n", mailfile);
			done();
		}
		fseek(malf, let[nlet].adr, 0);
		fclose(tmpf);
		tmpf = fopen(lettmp, "a");
		fseek(tmpf, let[nlet].adr, 0);
		while ((c = fgetc(malf)) != EOF)
			fputc(c, tmpf);
		fclose(malf);
		fclose(tmpf);
		tmpf = fopen(lettmp, "r");
		let[++nlet].adr = stbuf.st_size;
		new = 1;
	}
	malf = fopen(mailfile, "w");
	if (malf == NULL) {
		fprintf(stderr, "mail: can't rewrite %s\n", lfil);
		done();
	}
	n = 0;
	for (i = 0; i < nlet; i++)
		if (let[i].change != 'd') {
			copylet(i, malf, ORDINARY);
			n++;
		}
	fclose(malf);
	if (new)
		fprintf(stdout, "new mail arrived\n");
	unlock();
}

copymt(f1, f2)	/* copy mail (f1) to temp (f2) */
FILE *f1, *f2;
{
	long nextadr;

	nlet = nextadr = 0;
	let[0].adr = 0;
	while (fgets(line, LSIZE, f1) != NULL) {
		if (isfrom(line))
			let[nlet++].adr = nextadr;
		nextadr += strlen(line);
		fputs(line, f2);
	}
	let[nlet].adr = nextadr;	/* last plus 1 */
}

copylet(n, f, type) FILE *f;
{	int ch;
	long	k;
	fseek(tmpf, let[n].adr, 0);
	k = let[n+1].adr - let[n].adr;
	while(k-- > 1L && (ch=fgetc(tmpf))!='\n')
		if(type!=ZAP) fputc(ch,f);
	if(type==REMOTE) {
		gethostname(hostname, sizeof (hostname));
		fprintf(f, " remote from %s\n", hostname);
	}
	else if (type==FORWARD)
		fprintf(f, forwmsg);
	else if(type==ORDINARY)
		fputc(ch,f);
	while(k-->1L)
		fputc(ch=fgetc(tmpf), f);
	if(type!=ZAP || ch!= '\n')
		fputc(fgetc(tmpf), f);
}

isfrom(lp)
register char *lp;
{
	register char *p;

	for (p = from; *p; )
		if (*lp++ != *p++)
			return(0);
	return(1);
}

sendmail(argc, argv)
char **argv;
{
	char truename[100];
	int first;
	register char *cp;
	int gaver = 0;
# ifdef DELIVERMAIL
	char *newargv[1000];
	register char **ap;
	register char **vp;
	int dflag;

	dflag = 0;
	if (argc < 1)
		fprintf(stderr, "puke\n");
	for (vp = argv, ap = newargv + 1; (*ap = *vp++) != 0; ap++)
	{
		if (ap[0][0] == '-' && ap[0][1] == 'd')
			dflag++;
	}
	if (!dflag)
	{
		/* give it to delivermail, rah rah! */
		unlink(lettmp);
		ap = newargv+1;
		if (rmail)
			*ap-- = "-s";
		*ap = "-delivermail";
		execv(DELIVERMAIL, ap);
		perror(DELIVERMAIL);
		exit(EX_UNAVAILABLE);
	}
# endif DELIVERMAIL

	truename[0] = 0;
	line[0] = '\0';

	/*
	 * When we fall out of this, argv[1] should be first name,
	 * argc should be number of names + 1.
	 */

	while (argc > 1 && *argv[1] == '-') {
		cp = *++argv;
		argc--;
		switch (cp[1]) {
		case 'r':
			if (argc <= 0) {
				usage();
				done();
			}
			if (strcmp(my_name, "root") &&
			    strcmp(my_name, "daemon") &&
			    strcmp(my_name, "uucp") &&
			    strcmp(my_name, "network")) {
				usage();
				done();
			}
			gaver++;
			strcpy(truename, argv[1]);
			fgets(line, LSIZE, stdin);
			if (strncmp("From", line, 4) == 0)
				line[0] = '\0';
			argv++;
			argc--;
			break;

		case 'h':
			if (argc <= 0) {
				usage();
				done();
			}
			hseqno = atoi(argv[1]);
			argv++;
			argc--;
			break;

# ifdef DELIVERMAIL
		case 'd':
			break;
# endif DELIVERMAIL
		
		default:
			usage();
			done();
		}
	}
	if (argc <= 1) {
		usage();
		done();
	}
	if (gaver == 0)
		strcpy(truename, my_name);
	/*
	if (argc > 4 && strcmp(argv[1], "-r") == 0) {
		strcpy(truename, argv[2]);
		argc -= 2;
		argv += 2;
		fgets(line, LSIZE, stdin);
		if (strncmp("From", line, 4) == 0)
			line[0] = '\0';
	} else
		strcpy(truename, my_name);
	*/
	time(&iop);
	fprintf(tmpf, "%s%s %s", from, truename, ctime(&iop));
	iop = ftell(tmpf);
	flgf = 1;
	for (first = 1;; first = 0) {
		if (first && line[0] == '\0' && fgets(line, LSIZE, stdin) == NULL)
			break;
		if (!first && fgets(line, LSIZE, stdin) == NULL)
			break;
		if (line[0] == '.' && line[1] == '\n' && isatty(fileno(stdin)))
			break;
		if (isfrom(line))
			fputs(">", tmpf);
		fputs(line, tmpf);
		flgf = 0;
	}
	fputs("\n", tmpf);
	nlet = 1;
	let[0].adr = 0;
	let[1].adr = ftell(tmpf);
	fclose(tmpf);
	if (flgf)
		return;
	tmpf = fopen(lettmp, "r");
	if (tmpf == NULL) {
		fprintf(stderr, "mail: cannot reopen %s for reading\n", lettmp);
		return;
	}
	while (--argc > 0)
		if (!send(0, *++argv, truename))
			error++;
	if (error && safefile(dead)) {
		setuid(getuid());
		malf = fopen(dead, "w");
		if (malf == NULL) {
			fprintf(stdout, "mail: cannot open %s\n", dead);
			fclose(tmpf);
			return;
		}
		copylet(0, malf, ZAP);
		fclose(malf);
		fprintf(stdout, "Mail saved in %s\n", dead);
	}
	fclose(tmpf);
}

sendrmt(n, name, rcmd)
char *name;
char *rcmd;
{
	FILE *rmf, *popen();
	register char *p;
	char rsys[64], cmd[64];
	register local, pid;
	int ret, sts;

	local = 0;
	if (index(name, '^')) {
		while (p = index(name, '^'))
			*p = '!';
		if (strncmp(name, "researc", 7)) {
			strcpy(rsys, "research");
			if (*name != '!')
				--name;
			goto skip;
		}
	}
	if (*name=='!')
		name++;
	for(p=rsys; *name!='!'; *p++ = *name++)
		if (*name=='\0') {
			local++;
			break;
		}
	*p = '\0';
	if ((!local && *name=='\0') || (local && *rsys=='\0')) {
		fprintf(stdout, "null name\n");
		return(0);
	}
skip:
	if ((pid = fork()) == -1) {
		fprintf(stderr, "mail: can't create proc for remote\n");
		return(0);
	}
	if (pid) {
		while ((ret = wait(&sts)) != pid) {
			if (ret == -1)
				return(0);
		}
		return(!sts);
	}
	setuid(getuid());
	if (local)
		sprintf(cmd, "%s %s", rcmd, rsys);
	else {
		if (index(name+1, '!'))
			sprintf(cmd, "uux - %s!rmail \\(%s\\)", rsys, name+1);
		else
			sprintf(cmd, "uux - %s!rmail %s", rsys, name+1);
	}
	if ((rmf=popen(cmd, "w")) == NULL)
		exit(1);
	copylet(n, rmf, local ? (!strcmp(rcmd, "/bin/mail") ? FORWARD : ORDINARY) : REMOTE);
	exit(pclose(rmf) != 0);
}

# ifndef DELIVERMAIL
/*
 * Send mail on the Berkeley network.
 * Sorry Bill, sendrmt() is so awful we just gave up.
 */

sendberkmail(n, name, fromaddr)
	char name[];
	char fromaddr[];
{
	char cmd[200];
	register FILE *cmdf;

	sprintf(cmd, "%s -h %d -f %s -t %s", RMAIL, hseqno, fromaddr, name);
	if ((cmdf = popen(cmd, "w")) == NULL) {
		perror(RMAIL);
		return(0);
	}
	copylet(n, cmdf, ORDINARY);
	pclose(cmdf);
	return(9);
}
# endif

usage()
{
	fprintf(stderr, "Usage: mail [ -f ] people . . .\n");
	error = EX_USAGE;
}

send(n, name, fromaddr)
int n;
char *name;
char *fromaddr;
{
	char file[100];
	register char *p;
	register mask;
	struct passwd *pw, *getpwnam();
	struct stat statb;

# ifndef DELIVERMAIL
	stripfx(LOCNAM1, &name);
	stripfx(LOCNAM2, &name);
	stripfx(LOCNAM3, &name);
	stripfx(LOCNAM4, &name);
	if(*name == ':')
		name++;		/* skip colon in to-name */
	for(p=name; *p!=':' && *p!='!' && *p!='^' &&*p!='\0'; p++)
		;
	/* if(*p == ':') return(sendrmt(n, name, RMAIL)); */
	if (*p == ':')
		return(sendberkmail(n, name, fromaddr));
	else if (*p=='\0' && strcmp(name, "msgs") == 0)
		return(sendrmt(n, "-s", MSGS));
# endif
	for(p=name; *p!='!'&&*p!='^' &&*p!='\0'; p++)
		;
	if (*p == '!'|| *p=='^')
		return(sendrmt(n, name, 0));
	if ((pw = getpwnam(name)) == NULL) {
		fprintf(stdout, "mail: can't send to %s\n", name);
		return(0);
	}
	cat(file, maildir, name);
	if (stat(file, &statb) >= 0 && (statb.st_mode & S_IFMT) == S_IFDIR) {
		strcat(file, "/");
		strcat(file, name);
	}
	mask = umask(MAILMODE);
	if (!safefile(file))
		return(0);
	lock(file);
	malf = fopen(file, "a");
	umask(mask);
	if (malf == NULL) {
		unlock();
		fprintf(stdout, "mail: cannot append to %s\n", file);
		return(0);
	}
	chown(file, pw->pw_uid, pw->pw_gid);
	copylet(n, malf, ORDINARY);
	fclose(malf);
	unlock();
	return(1);
}

delete(i)
{
	setsig(i, delete);
	fprintf(stderr, "\n");
	if(delflg)
		longjmp(sjbuf, 1);
	done();
}

/*
 * Lock the specified mail file by setting the file mailfile.lock.
 * We must, of course, be careful to unlink the lock file by a call
 * to unlock before we stop.  The algorithm used here is to see if
 * the lock exists, and if it does, to check its modify time.  If it
 * is older than 30 seconds, we assume error and set our own file.
 * Otherwise, we wait for 5 seconds and try again.
 */

char	*maillock	= ".lock";		/* Lock suffix for mailname */
char	*lockname	= "/usr/spool/mail/tmXXXXXX";
char	locktmp[30];				/* Usable lock temporary */
char	curlock[50];				/* Last used name of lock */
int	locked;					/* To note that we locked it */

lock(file)
char *file;
{
	register int f;
	struct stat sbuf;
	long curtime;
	int statfailed;

	if (locked || flgf)
		return(0);
	strcpy(curlock, file);
	strcat(curlock, maillock);
	strcpy(locktmp, lockname);
	mktemp(locktmp);
	unlink(locktmp);
	statfailed = 0;
	for (;;) {
		f = lock1(locktmp, curlock);
		if (f == 0) {
			locked = 1;
			return(0);
		}
		if (stat(curlock, &sbuf) < 0) {
			if (statfailed++ > 5)
				return(-1);
			sleep(5);
			continue;
		}
		statfailed = 0;
		time(&curtime);
		if (curtime < sbuf.st_ctime + 30) {
			sleep(5);
			continue;
		}
		unlink(curlock);
	}
}

/*
 * Remove the mail lock, and note that we no longer
 * have it locked.
 */

unlock()
{

	unlink(curlock);
	locked = 0;
}

/*
 * Attempt to set the lock by creating the temporary file,
 * then doing a link/unlink.  If it fails, return -1 else 0
 */

lock1(tempfile, name)
	char tempfile[], name[];
{
	register int fd;

	fd = creat(tempfile, 0);
	if (fd < 0)
		return(-1);
	close(fd);
	if (link(tempfile, name) < 0) {
		unlink(tempfile);
		return(-1);
	}
	unlink(tempfile);
	return(0);
}

done()
{
	if(locked)
		unlock();
	unlink(lettmp);
	unlink(locktmp);
	exit(error);
}

cat(to, from1, from2)
char *to, *from1, *from2;
{
	int i, j;

	j = 0;
	for (i=0; from1[i]; i++)
		to[j++] = from1[i];
	for (i=0; from2[i]; i++)
		to[j++] = from2[i];
	to[j] = 0;
}

char *getarg(s, p)	/* copy p... into s, update p */
register char *s, *p;
{
	while (*p == ' ' || *p == '\t')
		p++;
	if (*p == '\n' || *p == '\0')
		return(NULL);
	while (*p != ' ' && *p != '\t' && *p != '\n' && *p != '\0')
		*s++ = *p++;
	*s = '\0';
	return(p);
}
# ifndef DELIVERMAIL
/*
	stripfx(prefix string, pointer to string)

	takes a ptr to string and compares it to prefix string.
	may be called multiple times
*/
stripfx(pfx, name)
	char *pfx;
	char **name;
{
	register char *cp = *name;

	while (*pfx && (*cp == *pfx || *cp == toupper(*pfx)))
		cp++, pfx++;
	if (*cp != ':' || *pfx != 0)
		return;
	*name = cp;
}
# endif

safefile(f)
	char *f;
{
	struct stat statb;

#ifdef	UCB_SYMLINKS
	if (lstat(f, &statb) < 0)
#else
	if (stat(f, &statb) < 0)
#endif
		return(1);
	if (statb.st_nlink != 1 || (statb.st_mode & S_IFMT) == S_IFLNK) {
		fprintf(stderr, "mail:  %s has more than one link or is a symbolic link\n", f);
		return(0);
	}
	return(1);
}
har *pfx;
	char **name;
{
	register char *cp = *name;

	while (*pfx && (*cp == *pfx || *cp == toupper(*pfx)))
		cp++, pfx++;
	if (*cp != ':' || *pfx != 0)
		return;
	*name = cp;
}
# endif

safefile(f)
	char *f;
{
	struct stat statb;

#ifdef	UCB_SYMLINKS
	if (lstat(f, &statb) < 0)
#else
	if (statdelivermail/version.c                                                                                  660      17       2           72  2470111576  10125                                                                                                                                                                                                                                                                                                                                                                      char	Version[] = "@(#)Delivermail version 2.7 of 2/5/81";
= 1 || (statb.st_mode & S_IFMT) == S_IFLNK) {
		fprintf(stderr, "mail:  %s has more than one link or is a symbolic link\n", f);
		return(0);
	}
	return(1);
}
har *pfx;
	char **name;
{
	register char *cp = *name;

	while (*pfx && (*cp == *pfx || *cp == toupper(*pfx)))
		cp++, pfx++;
	if (*cp != ':' || *pfx != 0)
		return;
	*name = cp;
}
# endif

safefile(f)
	char *f;
{
	struct stat statb;

#ifdef	UCB_SYMLINKS
	if (lstat(f, &statb) < 0)
#else
	if (statdelivermail/newaliases.1                                                                               660      17       2          565  2776662430  10551                                                                                                                                                                                                                                                                                                                                                                      .TH NEWALIASES 1
.UC 4
.SH NAME
newaliases \- rebuild the data base for the mail aliases file
.SH SYNOPSIS
.B newaliases
.SH DESCRIPTION
.I Newalises
rebuilds the random access data base for the mail aliases file
/usr/lib/aliases.
It must be run each time /usr/lib/aliases is changed in order
for the change to take effect.
.SH SEE ALSO
aliases(5), delivermail(8)
.SH BUGS
return;
	*name = cp;
}
# endif

safefile(f)
	char *f;
{
	struct stat statb;

#ifdef	UCB_SYMLINKS
	if (lstat(f, &statb) < 0)
#else
	if (stat                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                );
	return(0);
}

done()
{
	if(locked)
		unlock();
	unlink(lettmp);
	unlink(locktmp);
	exit(error);
}

cat(to, from1, from2)
char *to, *from1, *from2;
{
	int i, j;

	j = 0;
	for (i=0; from1[i]; i++)
		to[j++] = from1[i];
	for (i=0; from2[i]; i++)
		to[j++] = from2[i];
	to[j] = 0;
}

char *getarg(s, p)	/* copy p... into s, update p */
register char *s, *p;
{
	while (*p == ' ' || *p == '\t')
		p++;
	if (*p == '\n' || *p == '\0')
		return(NULL);
	while (*p != ' ' && *p != '\t' && *p != '\n' && *p != '\0')
		*s++ = *p++;
	*s = '\0';
	return(p);
}
# ifndef DELIVERMAIL
/*
	stripfx(prefix string, pointer to string)

	takes a ptr to string and compares it to prefix string.
	may be called multiple times
*/
stripfx(pfx, name)
	char *pfx;
	char **name;
{
	register char *cp = *name;

	while (*pfx && (*cp == *pfx || *cp == toupper(*pfx)))
		cp++, pfx++;
	if (*cp != ':' || *pfx != 0)
		return;
	*name = cp;
}
# endif

safefile(f)
	char *f;
{
	struct stat statb;

#ifdef	UCB_SYMLINKS
	if (lstat(f, &statb) < 0)
#else
	if (stat(f, &statb) < 0)
#endif
		return(1);
	if (statb.st_nlink != 1 || (statb.st_mode & S_IFMT) == S_IFLNK) {
		fprintf(stderr, "mail:  %s has more than one link or is a symbolic link\n", f);
		return(0);
	}
	return(1);
}
har *pfx;
	char **name;
{
	register char *cp = *name;

	while (*pfx && (*cp == *pfx || *cp == toupper(*pfx)))
		cp++, pfx++;
	if (*cp != ':' || *pfx != 0)
		return;
	*name = cp;
}
# endif

safefile(f)
	char *f;
{
	struct stat statb;

#ifdef	UCB_SYMLINKS
	if (lstat(f, &statb) < 0)
#else
	if (statdelivermail/version.c                                                                                  660      17       2           72  2470111576  10125                                                                                                                                                                                                                                                                                                                                                                      char	Version[] = "@(#)Delivermail version 2.7 of 2/5/81";
= 1 || (statb.st_mode & S_IFMT) == S_IFLNK) {
		fprintf(stderr, "mail:  %s has more than one link or is a symbolic link\n", f);
		return(0);
	}
	return(1);
}
har *pfx;
	char **name;
{
	register char *cp = *name;

	while (*pfx && (*cp == *pfx || *cp == toupper(*pfx)))
		cp++, pfx++;
	if (*cp != ':' || *pfx != 0)
		return;
	*name = cp;
}
# endif

safefile(f)
	char *f;
{
	struct stat statb;

#ifdef	UCB_SYMLINKS
	if (lstat(f, &statb) < 0)
#else
	if (stat