#define MAIN_INIT
#include "rcv.h"
#include <sys/stat.h>

/*
 * Mail -- a mail program
 *
 * Startup -- interface with user.
 *
 * $Log: main.c,v $
 * Revision 1.12  1991/07/22  16:36:47  ache
 * Port to Borland C
 *
 * Revision 1.11  1991/01/25  18:04:45  ache
 * uBRANY STARYE (4.1) SIGNALY
 *
 * Revision 1.10  90/11/11  20:06:29  ache
 * iSPRAWLENO RASPOZNAWANIE IMEN FAJLOW
 * 
 * Revision 1.9  90/09/21  21:59:20  ache
 * MS-DOS extends + some new stuff
 * 
 * Revision 1.8  90/09/13  13:19:55  ache
 * MS-DOS & Unix together...
 * 
 * Revision 1.7  90/08/10  13:46:51  avg
 * dOBAWLEN KL@^ -x DLQ PEREDA^I BOLX[IH FAJLOW.
 * 
 * Revision 1.6  90/06/30  15:09:28  avg
 * tEPERX user-NYJ .mailrc NE GRUZITSQ W REVIME rmail.
 * 
 * Revision 1.5  90/04/30  19:44:31  avg
 * pO^INEN REVIM rmail
 * 
 * Revision 1.4  90/04/20  19:17:09  avg
 * pRIKRU^ENO POD System V
 * 
 * Revision 1.3  88/07/23  20:35:42  ache
 * rUSSKIE DIAGNOSTIKI
 * 
 * Revision 1.2  88/01/11  12:43:58  avg
 * dOBAWLEN NOXSTR U rcsid.
 * 
 * Revision 1.1  87/12/25  16:00:06  avg
 * Initial revision
 * 
 */

/*NOXSTR
static char rcsid[] = "$Header: /usr/src/Relcom/relcom/mailx/RCS/main.c,v 1.12 1991/07/22 16:36:47 ache Exp $";
YESXSTR*/

jmp_buf hdrjmp;

/*
 * Find out who the user is, copy his mail file (if exists) into
 * /tmp/Rxxxxx and set up the message pointers.  Then, print out the
 * message headers and read user commands.
 *
 * Command line syntax:
 *      Mail [ -i ] [ -r address ] [ -h number ] [ -f [ name ] ]
 * or:
 *      Mail [ -i ] [ -r address ] [ -h number ] people ...
 */

int     _ediag = EDIAG_E;
extern char master[];

int 	filetransfer = 0;
#ifdef	MSDOS
extern char *strdup();
char *argv0;
#endif

main(argc, argv)
	char **argv;
{
	register char *ef;
	register int i, argp;
	int mustsend, uflag, f;
	sigtype (*prevint)(), hdrstop();
	FILE *ibuf, *ftat;
#if !defined(MSDOS) && !defined(M_SYSV)
	extern char _sobuf[];
#endif
#ifndef MSDOS
#ifndef M_SYSV
	struct sgttyb tbuf;
#else
	struct termio tbuf;
#endif
#endif

	if(
	    (ef = getenv("MSG")) != NULL &&
	    (*ef == 'r' || *ef == 'R' || *ef == 'R' || *ef == 'r') )
		_ediag = 0;
#ifdef	MSDOS
	argv0 = strdup(argv[0]);
#endif

	/*
	 * Set up a reasonable environment.  We clobber the last
	 * element of argument list for compatibility with version 6,
	 * figure out whether we are being run interactively, set up
	 * all the temporary files, buffer standard output, and so forth.
	 */

	uflag = 0;
	argv[argc] = (char *) -1;
#ifdef  GETHOST
	inithost();
#endif
#ifndef	MSDOS
	mypid = getpid();
#endif
	intty = isatty(0);
	outtty = isatty(1);
#ifndef MSDOS
	if (outtty) {
#ifdef M_SYSV
		ioctl(1, TCGETA, &tbuf);
		baud = tbuf.c_cflag & CBAUD;
#else
		gtty(1, &tbuf);
		baud = tbuf.sg_ospeed;
#endif
	}
	else
		baud = B9600;
#endif
	image = NULL;
#if !defined(MSDOS) && !defined(M_SYSV)
	setbuf(stdout, _sobuf);
#else
	(void) setvbuf(stdout, NULL, _IOFBF, BUFSIZ);
#endif

	/*
	 * Now, determine how we are being used.
	 * We successively pick off instances of -r, -h, -f, and -i.
	 * If called as "rmail" we note this fact for letter sending.
	 * If there is anything left, it is the base of the list
	 * of users to mail to.  Argp will be set to point to the
	 * first of these users.
	 */

	argp = -1;
	mustsend = 0;
	ef = rindex(argv[0], '/');
#ifdef	MSDOS
	if (ef == NOSTR)
		ef = rindex(argv[0], SEPCHAR);
	if (ef == NOSTR && argv[0][0] && argv[0][1] == ':')
		ef = &argv[0][1];
#endif
	ef = (ef==NOSTR) ? argv[0] : ef+1;
	if (argc > 0 && 
			(*ef == 'r'
#ifdef	MSDOS
			|| *ef == 'R'
#endif
		)
	   )
		rmail++;

	ef = NOSTR;
	for (i = 1; i < argc; i++) {

		/*
		 * If current argument is not a flag, then the
		 * rest of the arguments must be recipients.
		 */

		if (*argv[i] != '-') {
			argp = i;
			break;
		}
		switch (argv[i][1]) {
		case 'r':
			/*
			 * Next argument is address to be sent along
			 * to the mailer.
			 */
			if (i >= argc - 1) {
				fprintf(stderr, ediag(
"Address required after -r\n",
"pOSLE -r NUVEN ADRESAT\n"));
				exit(1);
			}
			mustsend++;
			rflag = argv[i+1];
			i++;
			break;

		case 'T':
			/*
			 * Next argument is temp file to write which
			 * articles have been read/deleted for netnews.
			 */
			if (i >= argc - 1) {
				fprintf(stderr, ediag(
"Name required after -T\n",
"pOSLE -T NUVNO IMQ\n"));
				exit(1);
			}
			Tflag = argv[i+1];
			if (fclear(Tflag) < 0) {
				perror(Tflag);
				exit(1);
			}
			i++;
			break;

		case 'u':
			/*
			 * Next argument is person to pretend to be.
			 */
			uflag++;
			if (i >= argc - 1) {
				fprintf(stderr, ediag(
"Missing user name for -u\n",
"pROPU]ENO IMQ POLXZOWATELQ POSLE -u\n"));
				exit(1);
			}
			strcpy(myname, argv[i+1]);
			i++;
			break;

		case 'i':
			/*
			 * User wants to ignore interrupts.
			 * Set the variable "ignore"
			 */
			assign("ignore", "");
			break;

		case 'd':
			debug++;
			break;

		case 'h':
			/*
			 * Specified sequence number for network.
			 * This is the number of "hops" made so
			 * far (count of times message has been
			 * forwarded) to help avoid infinite mail loops.
			 */
			if (i >= argc - 1) {
				fprintf(stderr, ediag(
"Number required for -h\n",
"pOSLE -h NUVNO ^ISLO\n"));
				exit(1);
			}
			mustsend++;
			hflag = atoi(argv[i+1]);
			if (hflag == 0) {
				fprintf(stderr, ediag(
"-h needs non-zero number\n",
"-h NUVEN NE NOLX\n"));
				exit(1);
			}
			i++;
			break;

		case 's':
			/*
			 * Give a subject field for sending from
			 * non terminal
			 */
			if (i >= argc - 1) {
				fprintf(stderr, ediag(
"Subject req'd for -s\n",
"pOSLE -s NUVNA TEMA\n"));
				exit(1);
			}
			mustsend++;
			sflag = argv[i+1];
			i++;
			break;

		case 'f':
			/*
			 * User is specifying file to "edit" with Mail,
			 * as opposed to reading system mailbox.
			 * If no argument is given after -f, we read his
			 * mbox file in his home directory.
			 */
			if (i >= argc - 1)
				ef = mbox;
			else
				ef = argv[i + 1];
			i++;
			break;

		case 'n':
			/*
			 * User doesn't want to source /usr/lib/Mail.rc
			 */
			nosrc++;
			break;

		case 'N':
			/*
			 * Avoid initial header printing.
			 */
			noheader++;
			break;

		case 'v':
			/*
			 * Send mailer verbose flag
			 */
			assign("verbose", "");
			break;

		case 'x':
			/*
			 * Long-file transference flag,
			 * used mainly for sending binary
			 * files using UUENCODE format.
			 */
			filetransfer++;
			break;

		default:
			fprintf(stderr, ediag(
"Unknown flag: %s\n",
"nEWERNYJ KL@^: %s\n"),
argv[i]);
			fprintf(stderr,
"%s: %s [-r from] [-f mailbox] [-u user] [-T tempfile] [-i] [-h hopcnt]\n\
	[-s subject] [-n] [-N] [-v] [-x] [addressee...]\n",
					ediag("Usage", "wYZOW"), argv[0]);
			exit(1);
		}
	}

	/*
	 * Check for inconsistent arguments.
	 */

	if (ef != NOSTR && argp != -1) {
		fprintf(stderr, ediag(
"Cannot give -f and people to send to.\n",
"nELXZQ WMESTE UKAZYWATX -f I KOMU POSYLATX.\n"));
		exit(1);
	}
	if (mustsend && argp == -1) {
		fprintf(stderr, ediag(
"The flags you gave make no sense since you're not sending mail.\n",
"uKAZANNYE FLAGI NE IME@T SMYSLA, T. K. WY NE POSYLAETE PO^TU.\n"));
		exit(1);
	}
	rcvmode = argp == -1;
#ifdef  MSDOS
	configure();
#endif
	tinit();
	input = stdin;
	if (!nosrc)
		load(master);
	if(!rmail)
		load(mailrc);
	if (!rcvmode) {
		mail(&argv[argp]);

		/*
		 * why wait?
		 */

		exit(senderr);
	}

	/*
	 * Ok, we are reading mail.
	 * Decide whether we are editing a mailbox or reading
	 * the system mailbox, and open up the right stuff.
	 */

	if (ef != NOSTR) {
		char *ename;

		edit++;
		ename = expand(ef);
		if (ename != ef) {
			ef = (char *) calloc(1, strlen(ename) + 1);
			strcpy(ef, ename);
		}
		editfile = ef;
		strcpy(mailname, ef);
	}
	if (setfile(mailname, edit) < 0) {
		if (edit)
			perror(mailname);
		else
			fprintf(stderr, ediag(
					"No mail for %s\n",
					"nET PO^TY DLQ %s\n"), myname);
		exit(1);
	}
	if (!edit && !noheader && value("noheader") == NOSTR) {
		if (setjmp(hdrjmp) == 0) {
			if ((prevint = signal(SIGINT, SIG_IGN)) != SIG_IGN)
				signal(SIGINT, hdrstop);
			announce(!0);
			flush();
			signal(SIGINT, prevint);
		}
	}
	if (edit)
		newfileinfo();
/*
 * Wiped out by @VG - I DON'T LIKE IT!
 *      if (!edit && msgCount == 0) {
 *              printf(ediag("No mail\n","nET PO^TY\n"));
 *              flush();
 *              exit(0);
 *      }
 */
	commands();
	if (!edit) {
#ifndef MSDOS
		signal(SIGHUP, SIG_IGN);
#endif
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		quit();
	}
	exit(0);
}

/*
 * Interrupt printing of the headers.
 */
sigtype hdrstop()
{
	signal(SIGINT, SIG_IGN);
	clrbuf(stdout);
#ifndef MSDOS
	putchar('\n');
#endif
	printf(ediag("Interrupt\n","pRERYWANIE\n"));
	flush();
	signal(SIGINT, hdrstop);
	longjmp(hdrjmp, 1);
}
