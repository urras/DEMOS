/*
 *  Internet to UUCP Address Convertor & Router
 *
 *  Copyright 1990, DEMOS cooperative, Moscow, USSR
 *  All rights reserved.
 *
 *  The copyright above and this notice must be preserved in all
 *  copies of this source code. The copyright above does not
 *  evidence any actual or intended publication of this source
 *  code.
 *
 *  This is unpublished proprietary trade secret source code of
 *  DEMOS cooperative. This source code may not be copied,
 *  disclosed, distributed, demonstrated or licensed except as
 *  expressly authorized by DEMOS.
 *
 * $Log: uumail.c,v $
 * Revision 2.30  1991/03/17  10:21:40  dvv
 *   `#c,%-b% *+ng  uux -a '@' ' ,%-o%bao -  '%'.
 *
 * Revision 2.29  1991/02/16  18:08:54  avg
 * Now default routes have lower priority than explicit ones.
 *
 * Revision 2.28  91/01/12  17:18:16  avg
 * %/%`l -%./.'- --k% (,%-  , h(- " UUCP -% a" +(" nbao -  uux,
 *   /`.ab. .bak+ nbao.
 * 
 * Revision 2.27  91/01/10  21:42:58  avg
 * .! "+%-. .b`%' -(% +.* +l-.#. (,%-( " bang path ( /./` "+%-.
 * a+(o-(%  $`%a." a /cb%, "($  aaa!%bbb
 * 
 * Revision 2.26  91/01/05  23:52:02  avg
 * .! "+%-. /`%.!` '." -(% " bang path /`( /%`%$ g% c d+ #., F.
 * 
 * Revision 2.25  90/12/12  21:53:00  avg
 * .! "+%-. "k$(` -(% (,%-( , h(-k-(ab.g-(*  a..!i%-(o ('
 * UU_MACHINE %a+( -%b remote from...
 * 
 * Revision 2.24  90/12/10  17:08:52  avg
 * .! "+%- "k'." batchmail ( *+ng -a /`( "k'."% uux.
 * 
 * Revision 2.23  90/11/15  18:36:43  avg
 * .! "+%-. .b*($k" -(% +.* +l-.#. $.,%- .
 * 
 * Revision 2.22  90/11/12  19:11:33  avg
 * `( .bak+*%  $`%a !%`%bao (' /.+o Sender,   -% (' From
 * %a+( %abl .! .
 * 
 * Revision 2.21  90/11/09  23:13:28  avg
 * Added convertion from bang path to Internet path.
 * 
 * Revision 2.20  90/11/04  02:18:51  ache
 * #endif corrected
 * 
 * Revision 2.19  90/09/30  18:02:43  avg
 * .! "+%-  .!` !.b*  -%a*.+l*(e  $`%a b." " .forward
 * 
 * Revision 2.18  90/09/26  23:14:08  avg
 * .! "+%-  .!` !.b*  Return-Receipt-To.
 * 
 * Revision 2.17  90/09/26  20:17:46  avg
 * .! "+%-. a./.ab "+%-(% /cab.#. a. *.
 * 
 * Revision 2.16  90/09/22  21:50:17  avg
 * Aliases of form *: ...
 * 
 * Revision 2.15  90/09/09  23:07:16  avg
 * .! "+%- , `h`cb '*'.
 * 
 * Revision 2.14  90/09/08  22:55:46  avg
 * .! "+%-. /`(/(ak" -(% +.* +l-.#. uucp-h-.#. (,%-( " b !+(fc.
 * 
 * Revision 2.13  90/09/05  16:54:51  avg
 * Added form of pattern  x.y.z|  (matches ONLY if z present).
 * 
 * Revision 2.12  90/09/03  22:55:13  avg
 * *** empty log message ***
 * 
 * Revision 2.11  90/09/03  20:59:46  avg
 * o ' !k+ $.! "+obl .uucp * +.* +l-k, link- ,!
 * n
 * 
 * Revision 2.10  90/09/01  15:20:19  avg
 * Added a search in sorted text database feature.
 * 
 * Revision 2.9  90/08/26  19:19:24  avg
 * Added /F mailer flag (suppress local UUCP part in From... header).
 * Hacked up the joinaddr routine.
 * 
 * Revision 2.8  90/08/24  15:39:47  avg
 * .! "+%-  e(b` o .!` l.b*  $.,%-  .uucp ( /` "+%- d.`, b $ bk
 * " ARPA.
 * 
 * Revision 2.7  90/08/20  22:42:57  avg
 * .! "+%- d+ # -r * uux-c (.b,%-o%bao c* ' -(%, /I " Systems).
 * 
 * Revision 2.6  90/08/18  15:53:49  avg
 * .g(-%-. a` "-%-(% a h !+.- ,( " `%&(,% manual-routing
 * ` "+%-  .!` !.b*  *.-ab`c*f() a%b%c@d
 * 
 * Revision 2.5  90/08/17  18:40:38  avg
 * /usr/lib/localmail --> /usr/lib/relcom/localmail
 * 
 * Revision 2.4  90/08/13  16:56:21  avg
 * Lowering letters of user address in auto mode was added.
 * 
 * Revision 2.3  90/08/10  13:47:55  avg
 * .! "+%- (/cab.)) *+ng -o...
 * 
 * Revision 2.2  90/07/29  23:07:36  avg
 *  aa." o /` "*  ,%+*(e /+ne.
 * 
 * Revision 2.1  90/07/29  20:51:35  avg
 * .! "+%-  ` !.b  a b !+(f ,( " d.`, b%  "b., b(g%a*.#. , `h`cb(' b.` .
 * %/%`l uumail /.$$%`&(" %b a%`"(a-k% /`.#` ,,k, "k'k" o (e * *
 * 	uu-router -r .b*c$ 
 * .! "+%- a!.` ab b(ab(*( /. /%`%$ g , /.gb."ke a..!i%-().
 * 
 * Revision 1.29  90/07/19  17:41:28  avg
 * ` "+%-  .h(!*  " ifdef-%.
 * 
 * Revision 1.28  90/07/02  00:33:41  avg
 * $%+ - d+ # mailer-  /S - strip host.domain part.
 * 
 * Revision 1.27  90/06/30  22:34:25  avg
 * .! "+%-  !.+%% /.$`.!- o $( #-.ab(*  -%".',.&-.ab( ` aak+*(
 * +.* +l-k, /.+l'." b%+o,.
 * $%+ -  /`."%`*  g(a+  /%`%ak+.* $+o /`%$.b"` i%-(o ' f(*+(" -(o.
 * 
 * Revision 1.26  90/06/30  17:42:54  avg
 * Fixed the bug which appears when the message has no RFC From: line.
 * 
 * Revision 1.24  90/06/26  15:53:42  avg
 * %/%`l .!` ! bk" %b "+.&%--k% () " /.+oe ' #.+."* .
 * 
 * Revision 1.23  90/06/21  23:17:38  avg
 * .! "+%-. /.+% Date " Error Report.
 * 
 * Revision 1.22  90/06/20  16:07:26  avg
 * ` "+%-  .h(!*  " alias- e - uumail -% ' e.$(+ -  aliask
 * $+o (,%- "($  user@localhost.
 * 
 * Revision 1.21  90/06/19  22:30:12  avg
 * %/%`l uumail 
 -% /%`%$ %b d+ # -r - mb. .g%-l ab` -- o hbc* .
 * 
 * Revision 1.20  90/06/19  13:35:40  avg
 * .! "+%-. ' *`kb(% +(h-(e d )+.".
 * %,-.#. (',%-%-  ab` b%#(o ag%b  *.`%++of((.
 * 
 * Revision 1.19  90/06/18  16:51:14  avg
 * ` "+%-  /%`%$ g   `#c,%-b." $+o uux.
 * 
 * Revision 1.18  90/06/17  19:55:05  avg
 * *** empty log message ***
 *       ALIAS
 * Revision 1.17  90/06/17  19:50:46  avg
 *  !k" + c$ +(bl \n (' *.-f  $c` f*.) ab`.*( From ...
 * 
 * Revision 1.16  90/06/17  19:44:44  avg
 * %,-.#. /` "+%-  $( #-.ab(*  - b%/%`l uumail .b+(g %b o"-k%
 * uucp-h-k%  $`%a  .b .ab +l-ke.
 * 
 * Revision 1.15  90/06/17  19:38:35  avg
 * ` "+%-  / `  ,%+*(e /+nh%*.
 * 
 * Revision 1.14  90/06/17  17:50:15  avg
 * ` "+%-  .h(!*  /`( a*+%(" -(( $+(--ke $+(--ke  $`%a.".
 * 
 * Revision 1.13  90/06/12  22:27:13  avg
 * $%+ -. /`.a%* -(% +.* +l-.#.  $`%a  /.ak+ b%+o.
 * 
 * 
 * Revision 1.12  90/06/11  14:56:13  avg
 * a+( -%b sendmail- , b. $+o /`(%,  .h(!.* b%/%`l (a/.+l'c%bao
 * rmail.
 * 
 * Revision 1.11  90/06/11  14:49:33  avg
 * ` "+%-  .!` !.b*  #+c/.) ab`.*( 'From'.
 * .! "+o%bao Received: by ....
 * +cgh%-  $( #-.ab(*  .h(!.*.
 * 
 * Revision 1.10  90/06/10  18:47:05  avg
 * %/%`l $+o .bak+*( .h(!.* ( +.* +l-.) /.gbk "k'k" nbao ` '-k%
 * mailer-k.
 * 
 * Revision 1.9  90/06/08  21:35:43  avg
 * %/%`l uumail /`( /.ak+*% a..!i%-(o g%`%' uucp "a%#$  $.! "+o%b
 * ab`.*c From ... remote.
 * 
`.,% b.#., ./(a b%+l , `h`cb  @ $+o +.* +l-.) , h(-k $.! "+o%bao
 * "a%#$  /. c,.+g -(n.
 * (Internet-."a*.% (,o , h(-k $.+&-. !kbl " d )+% /etc/myhostname
 * %a+( -%b *., -$k hostname).
 * 
 * Revision 1.8  90/06/08  16:21:49  avg
 * .! "+%-  ".',.&-.abl .bak+*( a..!i%-() .! .h(!* e
 * !%' *+ng  -r.
 * 
 * Revision 1.7  90/06/08  15:31:20  avg
 * .! "+%-  .!` !.b*  alias-.".
 * 
 * Revision 1.6  90/06/05  20:16:17  avg
 * ` "*( $+o 286.
 * 
 * Revision 1.5  90/06/05  18:05:10  dvv
 * -#+()a*().
 * 
 * Revision 1.4  90/06/05  17:49:23  dvv
 * .! "+%-  /` "(+l- o .b` !.b*  /+.e(e *.$." ".'"` b 
 * uux.
 * 
 * Revision 1.3  90/06/05  16:44:45  avg
 * !` - (/.* ) *+ng -h  ( $.! "+%-  .!` .b*  o"-.#. /cb( " d.`,%
 * @a,@b,@c:d@e		( - +.#(g-. @a:@b:@c:d@e)
 * 
 * Revision 1.2  90/06/02  22:04:02  avg
 * Addresses given to remote rmail now are enclosed in parenteses
 * to avoid their interpreting by uux.
 * 
 * Revision 1.2  90/06/02  21:57:45  avg
 * ` "+%- "k'." uux b *, gb. "  `#c,%-b e rmail  $`%a  !c$cb
 * c* 'k" blao " a*.!* e $+o .b,%-k a/%f( +l-.#. '- g%-(o ! $+o uux.
 * 
 * Revision 1.1  90/05/23  21:20:48  avg
 * Initial revision
 * 
 *
 *----------------------------------------------------------------------
 *
 * Mailing through UUCP with address conversion from
 * Internet address to UUCP path.
 *
 * Usage:   uumail address...
 *
 * Files:
 * >>>  /$Relcom/uucp.routes
 *
 *      This file contains records of following format:
 *
 *           Internet-Hostname-Pattern <spaces> UUCP-Relay-Host
 *  or       * <spaces> UUCP-Relay-Host    (default route)
 *
 *      Internet hostname pattern is composed from list
 *      of dot-separated patterns which may be:
 *
 *      a) ordinary word       - requires exact pattern matching
 *      b) asterisk (*)        - matches any non-empty sequence of words
 *      c) question mark (?)   - matches a word
 *      d) form word,..,word   - matches a word from the given list
 *      e) form !word,..,word  - matches any word which is NOT in given list
 *
 *      UUCP Relay name must looks like UUCP path.
 *
 * >>>  $Relcom/uucp.froutes                   | NOT IMPLEMENTED NOW
 *      Freezed uuroutes for faster access.     |
 *
 * >>>  $Lib/aliases
 *      System-wide alias database in sendmail format.
 *
 * >>>  user-home/.forward
 *      User personal mail forwarding.
 */

#include "serv.h"

/*
 * If you want to live without SENDMAIL you must remove
 * #define SENDMAIL and add line "set sendmail=$Relcom/uumail"
 * to $relcom/Mail.rc.
 */
#include "../Dispose.h"
#ifndef NOSM            /* From the command line ... */
#define SENDMAIL
#endif

/* # define ALIASES "aliases"      * DEBUGGING */

#include <stdio.h>
#include <sysexits.h>
#include <sys/signal.h>

#ifdef SENDMAIL
# define MAILER R_SMAILER
# define ERROR_MAILER R_SMAILER
#else
# define MAILER XX_LOCALMAIL
# define ERROR_MAILER "rmail"
# define ALIASES M_ALIASES
#endif

#define BATCHMAILER     R_BATCH

#define LOGFILE         R_STAT   /* Mail logfile, comment it if you want to prohibit logging */

#define HOPLIMIT        20      /* Limit of host-to-host hops */

#ifndef lint
static char rcsid[] = "$Header: uumail.c 2.30 1991/03/17 10:21:40 Exp $";
#endif

/*
 * Remove '-h %d' if you want to live without hop counters
 */
/* #define UUX     "uux - %s %s!rmail -h %d" */
#define UUX     "uux - %s -a %s %s!rmail "

struct MRQ {
	char   *sendto;         /* Host to send to */
	char   *modaddr;        /* Modifyed Dst Address */
	struct MRQ *next;       /* Next Request */
	char   mflags;          /* Flags of mailer */
};

struct MRQ *convert();

int debug;
char *reply_to = NULL;
char *routes = NULL;
char *spooldir = NULL;
char  replyaddr[300];           /* Address from 'From:' ARPA-style field */
char  tmpreplyaddr[300];
char  uucppath[300];            /* Address from ugly 'From ... ' UUCP line */
char  fromline[300];            /* Ugly UUCP  From ... line generated */
char  sfromline[300];           /* Ugly UUCP  From ... line generated */
char  mname[512];

char *udate(), *adate();
char *uufix();

char *error_codes[] = {
	"command line usage error",
	"data format error",
	"cannot open input",
	"addressee unknown",
	"host name unknown",
	"service unavailable",
	"internal software error",
	"system error",
	"critical OS file missing",
	"can't create output file",
	"input/output error",
	"temp failure; retry",
	"remote error in protocol",
	"permission denied"
};

/*
 * Delivering lists
 */
struct MRQ *mrqlist, *errlist, *locallist, *amblist;

#define  UUNAME  "uuname -l"

#define  UUNAMES "uuname"

#define tolower(c)      (('A'<=(c)&&(c)<='Z') ? (c)+040 : (c))

#ifdef M_SYSV
#define index  strchr
#define rindex strrchr
#endif
extern char *index(), *rindex(), *getlogin();

char uuname[100];       /* local uucp name */

char localpart[50];     /* Local user name */

#ifdef AUTOMATIC_ROUTING
char *SystemsFile = SYSTEMFILE;
char RoutesFile[250];
static int firsttime = 1;
#else
char *SystemsFile = NULL;
#endif

char errors_to[200] = "postmaster";
int  set_errors_to = 0;
char ret_receipt[200] = "";
char rstamp[200];       /* Received by: stamp string */

char *inet2bang();

struct SERVICE {
	struct SERVICE *serv_next;
	char            serv_name[1];   /* Name of the service */
} *services = NULL;

/* Mailer flags */
#define MF_STRIP        01      /* 'S' - strip host.domain part */
#define MF_QUEUE        02      /* 'I' - call cico immediately (inverse) */
#define MF_SFROM        04      /* 'F' - short form of From... line */
#define MF_BATCH        010     /* 'B' - send via batchmail */


main(ac, av)
char **av;
{
	struct MRQ *mr, *newmr, *xmrq, *slist, *sscan;
	register c;
	int i, reccnt;
	char savedFrom[512];
	char erb[512], *mailer;
	FILE *of, *inf = stdin;
	int hopcnt = 0;
	register char *p, *q;
	char *r, *getenv();
	int progret;
	int uglyfrom = 0;
	int rfcfrom = 0;
	int non_local;
	int ambflag = 0;
	int remote;
	int verbose = 0;
	long msgsize;
	int  sendergot, sprio, line_read;
	int  queue_request, long_from, batch_mail;
#define temphn fromline
	struct SERVICE *serv;

	if( ac < 2 ) {
usage:          fprintf(stderr, "Usage: %s [-D] [-i] [-r remote_from] [-R table] [-M mailer]\n\t[-v] [-s spooldir] [-S sysfile] [-h hopcount] address...\n", av[0]);
		exit(EX_USAGE);
	}

	/*
	 * Close excessive file descriptors
	 */
	for( i = 3 ; i < 30 ; i++ )
		close(i);

#ifdef SENDMAIL
	/*
	 * RMAIL mode operation
	 */
	if( (p = rindex(av[0], '/')) == NULL )
		p = av[0];
	else
		p++;
	remote = (p[0] == 'r');
#endif

	/*
	 * Parse options
	 */
	i = 1;

	hopcnt = -1;
	mailer = NULL;
	while( i < ac && av[i][0] == '-' ) {
		switch( av[i][1] ) {
		    case 'D':                   /* debug option */
			debug++;
			break;

		    case 'r':                   /* remote from ... */
			if( av[i][2] )
				reply_to = av[i]+2;
			else
				reply_to = av[++i];
			break;

		    case 'R':                   /* routing file located at ... */
			if( !debug )
				break;
			if( av[i][2] )
				routes = av[i]+2;
			else
				routes = av[++i];
			break;

		    case 'M':                   /* local mailer */
			if( !debug )
				break;
			if( av[i][2] )
				mailer = av[i]+2;
			else
				mailer = av[++i];
			break;

		    case 'S':                   /* Systems file */
			if( !debug )
				break;
			if( av[i][2] )
				SystemsFile = av[i]+2;
			else
				SystemsFile = av[++i];
			break;

		    case 's':                   /* routes spooling directory */
			if( !debug )
				break;
			if( av[i][2] )
				spooldir = av[i]+2;
			else
				spooldir = av[++i];
			break;

		    case 'i':           /* Ignore signals */
			(void) signal(SIGHUP,  SIG_IGN);
			(void) signal(SIGINT,  SIG_IGN);
			(void) signal(SIGQUIT, SIG_IGN);
			(void) signal(SIGTERM, SIG_IGN);
			break;

		    case 'h':           /* Hop count */
			if( av[i][2] )
				hopcnt = atoi(av[i]+2);
			else
				hopcnt = atoi(av[++i]);
			break;

		    case 'o':
		    case 'm':
			break;  /* Ignore it */

		    case 'v':
			verbose++;
			break;

		    default:
			goto usage;
		}
		i++;
	}
	if( i == ac )
		goto usage;
	if( routes == NULL )
		routes = ROUTES;
	if( spooldir == NULL )
		spooldir = ROUTES_DIR;
#ifdef AUTOMATIC_ROUTING
	sprintf(RoutesFile,  "%s/%s", spooldir, "Routes");
#endif

	/*
	 * Get a local UUCP hostname
	 */
	if( (inf = popen(UUNAME, "r")) != NULL ) {
		fgets(uuname, sizeof uuname, inf);
		pclose(inf);
		if( (p = index(uuname, '\n')) != NULL )
			*p = '\0';
	} else
		strcpy(uuname, "foobar");

	gethost(temphn, sizeof temphn);
	sprintf(rstamp, "Received: by %s; %s\n", temphn, adate());

	/*
	 * Read list of services
	 */
	if( (of = fopen(SERV_LIST, "r")) != NULL ) {
		while( fgets(erb, sizeof erb, of) != NULL ) {
			erb[sizeof erb - 1] = '\0';
			if( (p = index(erb, '\n')) != NULL )
				*p = '\0';
			p = erb;
			while( *p == ' ' || *p == '\t' )
				p++;
			if( *p == '#' || *p == '\0' )
				continue;
			serv = (struct SERVICE *)malloc(
						     sizeof(struct SERVICE) +
						     strlen(p));
			if( serv == NULL ) {
				errmsg("No memory for list of services.\n");
				exit(EX_SOFTWARE);
			}
			strcpy(serv->serv_name, p);
			serv->serv_next = services;
			services = serv;
		}
		fclose(of);
	}

	/*
	 * Read routing table
	 */
#ifdef MANUAL_ROUTING
	readroute();
#ifdef AUTOMATIC_ROUTING
	firsttime = 0;
	if( rsystems() )
		firsttime = 2;
#endif /* AUTOMATIC_ROUTING */
#else
#ifdef AUTOMATIC_ROUTING
	if( rsystems() ) {
		errmsg("Cannot open %s\n", SystemsFile);
		exit(EX_SOFTWARE);
	}
#endif /* AUTOMATIC_ROUTING */
#endif /* MANUAL_ROUTING */

	/*
	 * Collect addresses
	 */
	errlist = NULL;
	locallist = NULL;
	amblist = NULL;
	for( mrqlist = NULL ; i < ac ; i++ )
		convert(av[i]);

	/*
	 * Copy stdin in temp file
	 */
	sprintf(erb, "/tmp/URO%06d", getpid());
	if( (of  = fopen(erb, "w")) == NULL ||
	    (inf = fopen(erb, "r")) == NULL ) {
		i = EX_TEMPFAIL;
		inf = stdin;
		goto DeliveryError;
	}
	unlink(erb);         /* Unix Kludge */

	/*
	 * Try to grab reply address from the message header
	 * field From:
	 */
	uucppath[0] = 0;
	savedFrom[0] = 0;
	reccnt = 0;
	sendergot = 0;
	while( fgets(erb, sizeof erb, stdin) != NULL ) {
		if( uglyfrom == 0 ) {
			if( !strncmp(erb, "From ", 5) ) {
				strcpy(savedFrom, erb);
				if( (p = index(erb, '\n')) != NULL )
					*p = '\0';      /* strip out NL */
				uglyfrom = 1;

				/*
				 * Is there a "remote from" kludge?
				 */
				q = erb+5;
				while( *q == ' ' ) q++;
				p = index(q, ' ');
				if( p == NULL ) {
					/* No date ... strange feeling */
					strcpy(uucppath, q);
					continue;
				}
				*p++ = '\0';    /* cut string after name */
				while( *p && strncmp(p, " remote from ", 13) ) p++;
				if( *p == '\0' ) {
					/* No remote-from */
					strcpy(uucppath, q);

				} else {
					p += 13;
					while( *p == ' ' ) p++;

					strcpy(uucppath, p);
					if( (p = index(uucppath, ' ')) != NULL )
						*p = '\0';

					strcat(uucppath, "!");
					strcat(uucppath, q);
				}
				if( debug )
					printf("UUCP reply path collected: '%s'\n", uucppath);
				continue;       /* Eliminate old-styled From */
			}
		}
		fputs(erb, of);
next_header_line:

		/* End of header? */
		if( erb[0] == '\n' || erb[0] == '\r' || erb[0] == '\0' )
			goto cont_copy;
		if( fieldcmp(erb, "received") ) {
			reccnt++;
			continue;
		}
		if( fieldcmp(erb, "return-receipt-to") ) {
			i = 18;
			sprio = -1;
			goto parsefrom1;
		}
		if( fieldcmp(erb, "from") ) {
			i = 5;
			sprio = 3;
			goto parsefrom;
		}
		if( fieldcmp(erb, "sender") ) {
			i = 7;
			sprio = 4;
			goto parsefrom;
		}
		if( fieldcmp(erb, "reply-to") ) {
			i = 9;
			sprio = 1;
			goto parsefrom;
		}
		if( fieldcmp(erb, "resent-from") ) {
			i = 12;
			sprio = 5;
			goto parsefrom;
		}
		if( fieldcmp(erb, "resent-sender") ) {
			i = 14;
			sprio = 6;
			goto parsefrom;
		}
		if( fieldcmp(erb, "resent-reply-to") ) {
			i = 16;
			sprio = 2;

		parsefrom:
			if( sendergot >= sprio )
				continue;
		parsefrom1:

			/*
			 * Parse ugly RFC 822  Name <addr>  form
			 */
			rfcfrom = 1;
			if( (p = index(erb+i, '<')) == NULL )
				p = erb+i;
			else
				p++;
			i = 0;          /* comment flag */
			q = tmpreplyaddr;
		rfc_cont:
			for( ; *p != '\n' && *p != '\0' ; p++ ) {
				if( *p == ' ' || *p == '\t' )
					continue;
				if( *p == '(' ) {
					i++;
					continue;
				}
				if( i ) {
					if( *p == ')' )
						i--;
					continue;
				}
				if( *p == '>' )
					break;
				*q++ = *p;
			}
			line_read = 0;
			if( *p == '\n' &&
			    fgets(erb, sizeof erb, stdin) != NULL ) {
				fputs(erb, of);
				if( erb[0] == ' ' || erb[0] == '\t' ) {
					p = erb;
					goto rfc_cont;  /* RFC-style line continuation */
				}
				line_read++;
			}
			*q = '\0';
			if( q != tmpreplyaddr ) {
				if( sprio < 0 ) {
					strcpy(ret_receipt, tmpreplyaddr);
					if( debug )
						printf("Return-Receipt address collected: '%s'\n", tmpreplyaddr);
				} else {
					sendergot = sprio;
					strcpy(replyaddr, tmpreplyaddr);
					if( reply_to == NULL )
						reply_to = replyaddr;
					if( debug )
						printf("Reply address collected: '%s'\n", tmpreplyaddr);
				}
			}
			if( line_read )
				goto next_header_line;
		}
	}
cont_copy:
	while( (i = getchar()) != EOF )
		putc(i, of);
	msgsize = ftell(of);
	fclose(of);

	/*
	 * No information about sender - do something valuable
	 */
	if( reply_to != NULL ) {
		while( *reply_to == ' ' || *reply_to == '\t' )
			reply_to++;
	}
	if( reply_to == NULL || *reply_to == '\0' ) {
		if( uucppath[0] == '\0' ) {
			strcpy(uucppath, "uucp@");
			strcat(uucppath, temphn);
		}
		reply_to = uucppath;
	}

	/*
	 * Check for UUCPPATH
	 */
	if( uucppath[0] == '\0' )
		strcpy(uucppath, reply_to);

	r = getenv("UU_MACHINE");
	if( remote && r != NULL ) {
		strcpy(fromline, uucppath);
		if( (p = index(fromline, '!')) == NULL ) {
			p = fromline;
			while( (p = index(p, '@')) != NULL )
				*p = '%';
			sprintf(uucppath, "%s!%s", r, fromline);
		} else {
			*p = '\0';
			if( strcmp(r, fromline) ) {
				*p = '!';
				sprintf(uucppath, "%s!%s", r, fromline);
			}
		}
	}

	/*
	 * Generate an ugly old-fashioned From ... remote from line
	 */
	sprintf(fromline,  "From %s %s remote from %s\n",
			   uucppath, udate(), uuname);
	if( (p = index(fromline, '@')) != NULL )
		*p = '%';
	sprintf(sfromline, "From %s %s remote from %s\n",
			   inet2bang(reply_to), udate(), uuname);
	if( debug ) {
		printf("FULL    FROM_: '%s'\n", fromline);
		printf("REDUCED FROM_: '%s'\n", sfromline);
	}

	/*
	 * Check if reply_to is a local address
	 */
	non_local = 0;
	if( index(reply_to, '!') != NULL || nonlocal(reply_to) )
		non_local++;

	/*
	 * Check the number of hops
	 */
	if( hopcnt > reccnt )
		reccnt = hopcnt;
	if( reccnt > HOPLIMIT ) {
		i = EX_PROTOCOL;
		goto DeliveryError;
	}
	reccnt = 0;

	/*
	 * Send mail out
	 */
	while( mrqlist != NULL ) {
		/*
		 * Collect mail for host given
		 */
		xmrq = NULL;
		mr = mrqlist->next;
		mrqlist->next = NULL;
		slist = mrqlist;
		while( mr != NULL ) {
			newmr = mr->next;
			if( !strcmp(slist->sendto, mr->sendto) ) {
				/*
				 * Eliminate duplicate addresses
				 */
				for( sscan = xmrq ; sscan != NULL ; sscan = sscan->next ) {
					if( !strcmp(sscan->modaddr, mr->modaddr) )
						goto dupaddr;
				}
				mr->next = slist;
				slist = mr;
		dupaddr:        ;
			} else {
				mr->next = xmrq;
				xmrq = mr;
			}
			mr = newmr;
		}
		mrqlist = xmrq;

		/*
		 * Send mail out
		 */
		if( slist->sendto[0] == '@' && slist->sendto[1] == '\0' ) {
#ifndef SENDMAIL
			errmsg("Attempt to deliver message via sendmail - addr %s\n", slist->modaddr);
			i = EX_SOFTWARE;
			goto DeliveryError;
#else /* SENDMAIL */
			i = 1;
			p = "sendmail";
			sprintf(erb, "%s -r %s", MAILER, uucppath);
#endif /* SENDMAIL */
		} else {
			i = 0;
			p = "uucp";
			queue_request = 1;
			long_from = 1;
			batch_mail = 1;
			for( mr = slist ; mr != NULL ; mr = mr->next ) {
				if( !(mr->mflags & MF_QUEUE) )
					queue_request = 0;
				if( mr->mflags & MF_SFROM )
					long_from = 0;
				if( !(mr->mflags & MF_BATCH) )
					batch_mail = 0;
			}
			if( batch_mail ) {
				sprintf(erb, "%s %s", BATCHMAILER, slist->sendto);
			} else {
				sprintf(erb, UUX, queue_request ? "-r" : "",
					     uufix(reply_to, 1, 1),
					     slist->sendto, hopcnt+1);
			}
		}
		strcpy(mname, erb);
		for( mr = slist ; mr != NULL ; mr = mr->next ) {
			/*
			 * Enclose address in parenteses because of
			 * foolish interpretation of !s by uux.
			 */
			strcat(erb, " ");
			strcat(erb, batch_mail? mr->modaddr :
						uufix(mr->modaddr, i, 0));
		}
		if( debug ) {
			printf("Call %s as '%s'\n", p, erb);
			continue;
		}
		if( verbose )
			fprintf(stderr, "uumail: sending via %s%s%s\n", p, i ? "" : " to",
									   i ? "" : slist->sendto);
		if( (of = popen(erb, "w")) == NULL ) {
			i = EX_OSERR;
			goto DeliveryError;
		}

		/* Add this ugly line for uucp */
		if( !i ) {
			fputs(long_from? fromline : sfromline, of);
#ifdef SENDMAIL
			if( remote )
#endif
				fputs(rstamp, of);
		}

		/*
		 * Copy stdin to output (may be N times)
		 */
		fseek(inf, 0l, 0);      /* rewind it */
		while( (c = getc(inf)) != EOF )
			putc(c, of);
		progret = pclose(of);
		if( (progret&0x00FF) != 0 ) {
			i = EX_OSERR;
			goto DeliveryError;
		}
		progret = (progret>>8) & 0xFF;
		if( progret != 0
#ifdef SENDMAIL
		    && !( i && /* was sent using sendmail */
			  EX__BASE <= progret && progret < 100 )
			  /* sendmail will generate an error reply
			     automagically */
#endif /* SNEDMAIL */
				 ) {
			i = progret;
			goto DeliveryError ;
		}
#ifdef LOGFILE
		for( mr = slist ; mr != NULL ; mr = mr->next ) {
			if( i ) /* sendmail */
				logmail(uucppath, mr->modaddr, msgsize);
			else {  /* uux */
				sprintf(erb, "%s!%s", slist->sendto, mr->modaddr);
				while( (p = index(erb, '@')) != NULL )
					*p = '%';
				logmail(uucppath, erb, msgsize);
			}
		}
#endif /* LOGFILE */
	}

	/*
	 * Send Local Mail
	 */
	if( locallist != NULL ) {
		slist = NULL;
		for( mr = locallist ; mr != NULL ; mr = newmr ) {
			newmr = mr->next;

			/* Try to find a service */
			for( serv = services ; serv != NULL ; serv = serv->serv_next ) {
				if( !strxcmp(serv->serv_name, mr->modaddr) ) {
					sprintf(erb, "%s/%s -r %s", SERV_DIR, serv->serv_name, uucppath);
					goto sendlocal;
				}
			}

			sprintf(erb, "%s", (mailer==NULL)?MAILER:mailer);
			if( non_local || rfcfrom == 0 ) {
				strcat(erb, " -r ");
				strcat(erb, uucppath);
			}
			strcpy(mname, erb);
			strcat(erb, " ");
			strcat(erb, mr->modaddr);
		sendlocal:
			if( debug ) {
				printf("LOCAL MAIL: '%s'\n", erb);
				continue;
			}
			if( verbose )
				fprintf(stderr, "uumail: sending via localmail\n");
			if( (of = popen(erb, "w")) == NULL ) {
				i = EX_OSERR;
				while( mr != NULL ) {
					mr->next = slist;
					slist = mr;
				}
				goto DeliveryError;
			}
			fseek(inf, 0l, 0);
#ifndef SENDMAIL
			fputs(rstamp, of);
#endif
			while( (c = getc(inf)) != EOF )
				putc(c, of);
			progret = pclose(of);
			if( (progret&0x00FF) != 0 ) {
				i = EX_OSERR;
				mr->next = slist;
				slist = mr;
				continue;
			}
			progret = (progret>>8) & 0xFF;
			if( progret != 0
#ifdef SENDMAIL
			    && !(EX__BASE <= progret && progret < 100)
				  /* sendmail will generate an error reply
				     automagically */
#endif /* SNEDMAIL */
					  ) {
				i = progret;
				mr->next = slist;
				slist = mr;
			}
#ifdef LOGFILE
			logmail(uucppath, mr->modaddr, msgsize);
#endif
		}
		if( slist != NULL )
			goto DeliveryError;

#ifndef SENDMAIL
		/*
		 * Report receipt
		 */
		if( ret_receipt[0] != '\0' ) {
			/*
			 * Call error mailer
			 */
			sprintf(erb, "%s %s", ERROR_MAILER, ret_receipt);
			if( debug ) {
				printf("SEND RETURN RECEIPT REPLY as '%s'\n", erb);
				of = stdout;
			} else {
				if( (of = popen(erb, "w")) == NULL )
					exit(i);
			}
			gethost(erb, sizeof erb);
			fprintf(of, "From uucp %s\n", udate());
			fprintf(of, "From: uucp@%s (UUCP Routing Service)\n", erb);
			fprintf(of, "To: %s\n", ret_receipt);
			fprintf(of, "Date: %s\n", adate());
			fprintf(of, "Subject: Return Receipt\n\n");
			fprintf(of, "Your message delivered to");
			for( mr = locallist ; mr != NULL ; mr = mr->next )
				fprintf(of, " %s", mr->modaddr);
			fprintf(of, ".\n\n---------- The header follows ------------\n");
			fseek(inf, 0l, 0);
			while( fgets(erb, sizeof erb, inf) != NULL ) {
				if( erb[0] == '\n' )  /* End of header */
					break;
				if( !strncmp(erb, "From ", 5) )
					putc('>', of);
				fputs(erb, of);
			}
			if( !debug )
				pclose(of);
		}
#endif /* NEW */
	}

	/*
	 * Send User Errors
	 */
	if( errlist == NULL && amblist == NULL )
		exit(0);
	if( errlist != NULL )
		i = EX_NOHOST;
	else {
		i = EX_PROTOCOL;
		ambflag++;
	}

	/*
	 * Send Error Message to the originator of mail
	 */
DeliveryError:
	if( !non_local && localpart[0] != '\0' )
		reply_to = localpart;

	/*
	 * Check User name in order to avoid loops
	 */
	strcpy(erb, reply_to);

	/* Strip near Internet machine name */
	if( (p = rindex(erb, '@')) != NULL ) {
		*p = '\0';
		if( (p = rindex(erb, ':')) != NULL )
			p++;
		else
			p = erb;
	} else
		p = erb;

	/* Strip UUCP machines */
	if( (q = rindex(p, '!')) != NULL )
		p = q+1;

	/* Strip for Internet machine name */
	if( (q = rindex(p, '%')) != NULL )
		*q = '\0';

	/* Check the name */
	if( !strxcmp(p, "uucp") || !strxcmp(p, "MAILER-DEMON") ||
				   !strxcmp(p, "MAILER-DAEMON") ) {
		if( debug )
			printf("REDIRECT ERRORS TO root\n");
		reply_to = "postmaster"; /* serg */
	}

	/*
	 * Call error mailer
	 */
	sprintf(erb, set_errors_to? "%s %s %s" : "%s %s",
		     ERROR_MAILER, reply_to, errors_to);
	if( debug ) {
		printf("SEND ERR REPLY as '%s'\n", erb);
		of = stdout;
	} else {
		if( (of = popen(erb, "w")) == NULL )
			exit(i);
	}
	gethost(erb, sizeof erb);
#ifndef SENDMAIL
	fprintf(of, "From uucp %s\n", udate());
#endif
	fprintf(of, "From: uucp@%s (UUCP Routing Service)\n", erb);
	fprintf(of, "To: %s\n", reply_to);
	if( set_errors_to )
		fprintf(of, "To: %s\n", errors_to);
	fprintf(of, "Date: %s\n", adate());
	if( i == EX_PROTOCOL && reccnt ) {
		fprintf(of, "Subject: Troubles routing the message\n\n");
		fprintf(of, "Hops count exeeded %d, probably misspelled target address.", HOPLIMIT);
	} else if( i == EX_PROTOCOL && ambflag ) {
		fprintf(of, "Subject: Troubles routing the message\n\n");
		fprintf(of, "Can not choose a precise route to the following address%s:\n\n", (amblist->next==NULL)?"":"es");
		for( mr = amblist ; mr != NULL ; mr = mr->next )
			fprintf(of, "\t\t%s\n", mr->modaddr);
		fprintf(of, "\nYou should specify the full name of the target machine(s).");
	} else if( i == EX_NOHOST ) {
		fprintf(of, "Subject: Error routing the message\n\n");
		fprintf(of, "Could not find a route for a message to the following address%s:\n\n", (errlist->next==NULL)? "":"es");
		for( mr = errlist ; mr != NULL ; mr = mr->next ) {
			if( mr->sendto[0] != '\0' && mr->sendto[0] != '@' )
				fprintf(of, "\t\t%s!%s\n", mr->sendto, mr->modaddr);
			else
				fprintf(of, "\t\t%s\n", mr->modaddr);
		}
	} else {
		fprintf(of, "Subject: Troubles delivering the message\n\n");
		fprintf(of, "Could not deliver a message to the following address%s:\n\n", (slist->next==NULL)? "":"es");
		for( mr = slist ; mr != NULL ; mr = mr->next ) {
			if( mr->sendto[0] != '\0' && mr->sendto[0] != '@' )
				fprintf(of, "\t\t%s!%s\n", mr->sendto, mr->modaddr);
			else
				fprintf(of, "\t\t%s\n", mr->modaddr);
		}
		if( i != EX_TEMPFAIL ) {
			fprintf(of, "\nMailer <%s> has\nreturned code %d", mname, i);
			if( i < EX__BASE || i > EX_NOPERM )
				fprintf(of, ".\n");
			else
				fprintf(of, " (%s).\n", error_codes[i-EX__BASE]);
		} else {
			errmsg("Temp file creation failure\n");
			fprintf(of, "\nError creating temporary file in /tmp.\n");
		}
	}
	fprintf(of, "\n\n\t--------- The unsent message follows -----------\n");
	if( savedFrom[0] != 0 )
		fputs(savedFrom, of);
	fseek(inf, 0l, 0);
	while( fgets(erb, sizeof erb, inf) != NULL ) {
		if( !strncmp(erb, "From ", 5) )
			putc('>', of);
		fputs(erb, of);
	}
	fclose(of);             /* Used fclose instead of pclose because
				 * we don't want to wait for error message
				 * delivering.
				 */
	exit(0);
}

/*
 * Compare the beginning of line with the field name
 * ignoring the case. Returns 1 if matched.
 */
fieldcmp(line, fname)
	register char *line;
	register char *fname;
{
	register char c;

	while( *fname ) {
		c = *line++;
		if( c == '\0' )
			return 0;
		if( 'A' <= c && c <= 'Z' )
			c |= 040;
		if( c != *fname++ )
			return 0;
	}
	return (*line == ':');
}

int mailer_flags;
#ifdef ALIASES
static  user_exist;     /* set by aliases */
#endif

/*
 * Convert address
 */
struct MRQ *convert(addr)
	char *addr;
{
	register char *p, *q;
	char *sp, *r;
	struct MRQ *mr;
	char    modaddr[400], sendto[400], buf[400], c;
	int     i, cannot, local, ambflag;
	struct SERVICE *serv;

	cannot = local = ambflag = 0;
	mailer_flags = 0;

	/*
	 * Scan for @
	 */
	if( (p = rindex(addr, '@')) != NULL )
		goto Internet;
Local:  if( (p = index(addr, '!')) == NULL ) {
		if( (p = rindex(addr, '%')) != NULL ) {
			*p = '@';
			goto Internet;
		}

		/*
		 * Local address given; send to local address
		 */
		strcpy(modaddr, addr);           /* Copy to modaddr */
		if( debug )
			printf("Processing Local address: '%s'\n", modaddr);
#ifdef ALIASES
		if( modaddr[0] == '\\' ) {
			/* Do not alias */
			strcpy(buf, &modaddr[1]);
			strcpy(modaddr, buf);
		} else {
			if( aliasit(modaddr, 0) )
				return;
			/* Try to find a service */
			for( serv = services ; serv != NULL ; serv = serv->serv_next ) {
				if( !strxcmp(serv->serv_name, modaddr) ) {
					goto localok;
				}
			}

			if( !user_exist ) {
				if( aliasit(modaddr, 1) )
					return;
			}
		}
localok:
#endif
		local++;
		sendto[0] = '\0';
		goto savestr;
	}

	/* p points to the first '!' */

	/*
	 * Cut off the name of local node
	 */
	*p = '\0';
	if( !strxcmp(uuname, addr) ) {
		*p++ = '!';
		addr = p;
		goto Local;
	}
	*p = '!';

	/*
	 * Check for the ugly form abc.def.ghi!blah!blah
	 * and convert it into blah!blah@abc.def.ghi
	 */
	*p = '\0';
	if( index(addr, '.') != NULL ) {
		if( debug )
			printf("Convert UGLY address: '%s'\n", addr);
		strcpy(buf, p+1);
		strcat(buf, "@");
		strcat(buf, addr);
		strcpy(addr, buf);

		if( debug )
			printf("     ---> to Internet '%s'\n", addr);
		p = rindex(addr, '@');
		goto Internet;
	}
	*p = '!';

	/*
	 * UUCP-style addresses
	 */
	if( debug )
		printf("Processing UUCP address: '%s'\n", addr);

	sp = p;

	/*
	 * Try to find the host xxx.UUCP
	 */
	*p = '\0';
	strcpy(buf, addr);
	strcat(buf, ".uucp");
	*p = '!';
	switch( lookup(buf, sendto) ) {
	    case 5:             /* Local UUCP link */
		if( debug )
			printf("Processing local .UUCP\n");
		p = addr;
		q = sendto;
		while( p < sp )         /* Copy prefix to sendto */
			*q++ = *p++;
		*q = '\0';
		strcpy(modaddr, ++p);   /* Copy tail to modaddr */
		if( index(modaddr, '!') == NULL ) {
			if( (q = rindex(modaddr, '%')) != NULL )
				*q = '@';
		}
		break;

	    case 0:             /* NO .UUCP MATCHED */
		if( debug )
			printf("UUCP Lookup Failed -- Generate Error\n");
		strcpy(modaddr, addr);
		cannot++;
		sendto[0] = '\0';
		break;

	    case 1:
		strcpy(modaddr, addr);
		if( sendto[0] == '*' && sendto[1] == '\0' ) {
			sendto[0] = '@';
			break;
		}
		joinpath(modaddr, sendto);
		break;

	    case 2:     /* No User/Domain part there ! */
		strcpy(modaddr, addr);
		joinpath(modaddr, sendto);
		break;

	    case 3:
		p = rindex(addr, '!');  /* can not be NULL */
		p++;
		strcpy(addr, p);
		goto Local;     /* process as local address */

	    case 4:
		ambflag++;
		strcpy(modaddr, addr);
		sendto[0] = '\0';
		break;
	}
	goto savestr;

	/*
	 * Internet-style addresses
	 */
Internet:
	if( debug )
		printf("Processing Internet address: '%s'\n", addr);

	/*
	 * Try to parse explicit path
	 */
	sp = p;
TryAgain:
	if( (p = index(addr, ':')) != NULL ) {
		if( debug )
			printf("\tPath Specified\n");
		for( q = p ; q > addr ; q-- ) {
			if( *q == ',' ) {
				p = q;
				break;
			}
		}
		if( *addr == '@' ) {
			/*
			 * Internet relay host address
			 */
			sp = addr+1;
			*p = '\0';
			addr = p+1;
			switch( lookup(sp, sendto) ) {
				case 1:
					strcpy(modaddr, addr);
					if( sendto[0] == '*' && sendto[1] == '\0' ) {
						sendto[0] = '@';
						goto savestr;  /* Do not join addrs */
					}
					goto ComPath;
				case 2:
					strcpy(modaddr, addr);
					goto ComPath;
				case 0:
				case 5:
					if( debug )
						printf("Failed to parse explicit route: discard it\n");
				case 3:
					goto TryAgain;          /* strip local host */
				case 4:
					ambflag++;
			}
		}
		if( debug )
			printf("\tHideous Path!!!\n");
		strcpy(modaddr, addr);
		cannot++;
		sendto[0] = '\0';
		goto savestr;
	}

	/*
	 * Simple Internet Address
	 * Extract the host/domain part and try to
	 * get route for it
	 *
	 * zDESX BYLO BY NEPLOHO SDELATX PEREKRU^IWANIE:
	 *      abc@def!ghi.uucp ---> ghi!abc@def.uucp
	 */
	switch( lookup(sp+1, sendto) ) {
	    case 4:
		ambflag++;
		strcpy(modaddr, addr);
		sendto[0] = '\0';
		break;

	    case 3:
		strncpy(modaddr, addr, sp-addr);
		modaddr[sp-addr] = '\0';
		strcpy(addr, modaddr);
		goto Local;     /* process as local address */

	    case 1:             /* Save host/domain name */
		strcpy(modaddr, addr);
		if( sendto[0] == '*' && sendto[1] == '\0' ) {
			sendto[0] = '@';
			break;  /* Do not join addrs */
		}
		goto ComPath;

	    case 2:             /* Strip out host/domain name */
		strncpy(modaddr, addr, sp-addr);
		modaddr[sp-addr] = '\0';
	    ComPath:
		joinpath(modaddr, sendto);
		break;

	    case 5:             /* Local UUCP link */
		if( debug )
			printf("Processing local .UUCP\n");
		strcpy(buf, sp+1);
		for( q = buf ; *q != '\0' ; q++ );
		q[-5] = '\0';   /* remove .uucp */
		*sp = '\0';
		if( (p = index(buf, '!')) != NULL ) {
			*p++ = '\0';
			strcpy(modaddr, p);
			strcat(modaddr, "!");
			strcat(modaddr, addr);
		} else
			strcpy(modaddr, addr);
		strcpy(sendto, buf);
		*sp = '@';
		break;

	    case 0:             /* Route Not Found */
		if( debug )
			printf("Lookup Failed -- Generate Error\n");
		strcpy(modaddr, addr);
		cannot++;
		sendto[0] = '\0';
	}

	/*
	 * Save strings
	 */
savestr:
	mr = (struct MRQ *)malloc(sizeof(struct MRQ));
	if( mr == NULL ) {
nomem:          errmsg("Not enough core to save msg header\n");
		exit(EX_OSERR);
	}
	if( (p = (char *)malloc(strlen(modaddr)+1)) == NULL )
		goto nomem;
	strcpy(p, modaddr);
	mr->modaddr = p;
	if( (p = (char *)malloc(strlen(sendto)+1)) == NULL )
		goto nomem;
	strcpy(p, sendto);
	mr->sendto = p;

	/*
	 * Put message into queue
	 */
	if( ambflag ) {
		if( debug )
			printf("Ambigious routing: %s\n", modaddr);
		mr->next = amblist;
		amblist = mr;
	} else if( cannot ) {
		mr->next = errlist;
		errlist = mr;
	} else if( local ) {
		mr->next = locallist;
		locallist = mr;
	} else {
		mr->next = mrqlist;
		mrqlist = mr;
	}
	mr->mflags = mailer_flags;
	return;
}

/*
 * Join the path and the explicit route
 */
joinpath(adr, path)
	char adr[];
	char path[];
{
	char buf[400];
	register char *p, *q;
	char c, *r;

	if( debug )
		printf("JOIN: ADR: '%s'  PATH: '%s'\n", adr, path);

	/*
	 * Turn all @s to %s in destination address
	 *  !s --> DELs
	 */
	while( (p = index(adr, '@')) != NULL )
		*p = '%';
	while( (p = index(adr, '!')) != NULL )
		*p = 0177;

	/*
	 * First - copy a UUCP part of path
	 */
	p = path;
	while( *p == '@' && (q = index(p, ':')) != NULL )
		p = q;          /* skip local explicit route */
	if( (q = rindex(p, '!')) != NULL ) {
		c = *++q;
		*q = '\0';
		strcpy(buf, path);
		*q = c;
	} else {
		q = path;
		buf[0] = '\0';
	}

	/*
	 * If there is a remote-Internet part in path - join to it
	 */
	if( index(path, '%') != NULL && index(adr, '%') != NULL &&
	    (index(path, '@') != NULL || index(path, '!') != NULL) ) {
		if( (p = index(q, '@')) != NULL )
			*p = '\0';
		if( debug )
			printf("remote Internet part is present, q='%s'\n", q);
		strcat(buf, adr);
		strcat(buf, q);
		if( p != NULL )
			*p = '@';
		q = (p == NULL) ? "" : p-1;
	} else

	/*
	 * Append the destination address & tail of path
	 */
		strcat(buf, adr);
	strcat(buf, q);
	if( debug )
		printf("JOIN ADR+PATH: '%s'\n", buf);

	/*
	 * If there are local Internet part - return whole addr
	 */
	if( index(buf, '@') != NULL ) {
		p = buf;
		strcpy(path, "@");      /* Send-through-SMTP flag */
	} else {
		/*
		 * Split the local UUCP path
		 */
		if( (p = index(buf, '!')) != NULL ) {
			*p = '\0';
			strcpy(path, buf);
			p++;
		} else {
			p = buf;
			strcpy(path, "@");
		}
		if( index(p, '!') == NULL ) {
			if( (q = rindex(p, '%')) != NULL )
				*q = '@';
		}
	}
	while( (q = index(p, 0177)) != NULL )
		*q = '!';
	/*
	 * Remove duplicated UUCP entries
	 */
	if( (q = index(p, '!')) != NULL   &&
	    (r = index(q+1, '!')) != NULL &&
	    (q-p) == (r-(q+1))            &&
	    !strncmp(p, q+1, q-p) )
		p = q+1;
	strcpy(adr, p);
	if( debug )
		printf("JOIN: MODADR: '%s'  SENDTO: '%s'\n", adr, path);
}

#ifdef MANUAL_ROUTING

struct RENT {                           /* Route Table Entry */
	struct RENT    *next;    /* Next Table Entry */
	short           mflags;  /* Mailer flags */
	char           *dest;    /* Destination address */
	char           *patt[1]; /* References to words */
}
	*routetab = NULL,         /* Table of routes */
	*tailoftab = NULL;        /* Tail of table */

static int      mr_enabled = 0;

/*
 * Read routing table
 */
readroute()
{
	register c;
	FILE *rt;
	char    iadr[256];
	char    route[256];
	char   **wp;
	int     nwords, cflags;
	int     mflags;
	char    *p;
	int     lc, ial, rol;
	struct RENT *rp;

	if( (rt = fopen(routes, "r")) == NULL )
		return;

	/*
	 * Scan table
	 */
	lc = 1;
	while( (c = getc(rt)) != EOF ) {
		/* skip blanks */
		if( c == ' ' || c == '\t' )
			continue;

		if( c == '\n' ) {
			lc++;
			continue;
		}

		/* skip comments */
		if( c == '#' ) {
			while( (c = getc(rt)) != EOF && c != '\n' ) ;
			lc++;
			continue;
		}

		/* read Internet address pattern */
		p = iadr;
		do {
			*p++ = tolower(c);
			c = getc(rt);
		} while( c != ' ' && c != '\t' && c != EOF && c != '\n' );
		*p = '\0';
		while( c == ' ' || c == '\t' )
			c = getc(rt);
		if( c == '\n' || c == EOF ) {
			errmsg("Missed destination field in line %d of manual routing table\n", lc);
			exit(EX_OSFILE);
		}

		/* read UUCP destination field */
		p = route;
		cflags = 0;
		mflags = 0;
		do {
			if( !cflags ) {
				if( c == '/' )
					cflags++;
				else
					*p++ = tolower(c);
			} else {
				if( c == 's' || c == 'S' )
					mflags |= MF_STRIP;
			}
			c = getc(rt);
		} while( c != ' ' && c != '\t' && c != EOF && c != '\n' );
		*p = '\0';
		while( c != '\n' && c != EOF )
			c = getc(rt);

		/* count dots in Internet address pattern */
		p = iadr;
		nwords = 1;
		while( *p )
			if( *p++ == '.' )
				nwords++;

		/* allocate memory for strings */
		ial = strlen(iadr)  + 1;
		rol = strlen(route) + 1;
		p = (char *)malloc(ial + rol + sizeof(struct RENT) +
					       sizeof(char *) * nwords);
		if( p == NULL ) {
		NoMem:  errmsg("Not enough core for routing tables\n");
			exit(EX_OSERR);
		}
		rp = (struct RENT *)(p + ial + rol);
		rp->mflags = mflags;
		if( routetab == NULL ) {
			routetab = tailoftab = rp;
			rp->next = NULL;
		} else {
			tailoftab->next = rp;
			rp->next = NULL;
			tailoftab = rp;
		}
		strcpy(rp->dest = p, route);
		p += rol;
		strcpy(p, iadr);
		wp = rp->patt;
		while( nwords-- ) {
			*wp = p;
			while( *p != '.' && *p )
				p++;
			*p++ = '\0';
			checkword(*wp++, lc);
		}
		*wp = NULL;

		/* go to next line */
		lc++;
		if( c == EOF )
			break;
	}

	/*
	 * Local delivery for current hostname must be done always.
	 */
	gethost(iadr, sizeof iadr);
	for( nwords = 1, p = iadr ; *p ; p++ )
		if( *p == '.' )
			nwords++;
	ial = strlen(iadr) + 1;
	p = (char *)malloc(ial + 2 + sizeof(struct RENT) +
				     sizeof(char *) * nwords);
	if( p == NULL )
		goto NoMem;
	rp = (struct RENT *)(p + ial + 2);
	if( routetab == NULL ) {
		routetab = tailoftab = rp;
		rp->next = NULL;
	} else {
		tailoftab->next = rp;
		rp->next = NULL;
		tailoftab = rp;
	}
	strcpy(rp->dest = p, "@");      /* Local Delivery */
	p += 2;
	strcpy(p, iadr);
	wp = rp->patt;
	while( nwords-- ) {
		*wp++ = p;
		while( *p != '.' && *p )
			*p++ = tolower(*p);
		*p++ = '\0';
	}
	*wp = NULL;
	mr_enabled = 1;

	/*
	 * *.uucp should fail
	 */
	rp = (struct RENT *)malloc( sizeof(struct RENT) + 2*sizeof(char *) );
	if( rp == NULL )
		goto NoMem;
	rp->next = routetab;
	routetab = rp;
	rp->mflags = 0;
	rp->dest = "-";
	rp->patt[0] = "*";
	rp->patt[1] = "uucp";
	rp->patt[2] = NULL;

	/*
	 * Print debugging info
	 */
	if( debug ) {
		printf("-------- ROUTING TABLE --------\n");
		printf("DESTINATION     ADDRESS PATTERN\n");
		for( rp = routetab ; rp != NULL ; rp = rp->next ) {
			printf("%-15.15s", rp->dest );
			for( wp = rp->patt ; *wp != NULL ; wp++ )
				printf(" %s", *wp);
			printf("\n");
		}
		printf("-------------------------------\n");
	}
}

/*
 * Check a pattern word
 */
checkword(pp, line)
	char *pp;
{
	register char *p = pp;
	register char c;
	char oc;

	if( *p == '*' || *p == '?' ) {
		if( p[1] != '\0' )
			goto Error;
		return;
	}
	oc = ',';
	if( *p == '!' && *++p == '\0' )
		goto Error;
	for( ; c = *p++ ; oc = c ) {
		if( 'a' <= c && c <= 'z' )
			continue;
		if( '0' <= c && c <= '9' )
			continue;
		switch( c ) {
		    default:
			goto Error;
		    case ',':
			if( oc == ',' )
				goto Error;
		    case '-':
		    case '_':
		    case '=':
		    case '+':   ;
		}
	}
	if( oc != ',' )
		return;
Error:
	errmsg("Illegal pattern '%s' in line %d\n", pp, line);
	exit(EX_USAGE);
}

/*
 * Try to match pattern and count the score
 * Returns -1 if matching is impossible
 */
int match(w, p)
	char **w, **p;
{
	int i, inverse;
	static level = 0;
	char **ww, **wdp;
	register char *a, *b;

	level++;
	if( *w == NULL ) {      /* No more words */
		i = 0;
		while( *p != NULL )
			i++, p++;
		if( debug )
			printf("(%d) No more words (lost=%d)\n", level--, i);
		return -i*2;    /* -2 per each word at the tail */
	}
	if( *p == NULL ) {
		if( debug )
			printf("(%d) END of pattern (FAIL)\n", level--);
		return -10000;
	}
	if( **p == '*' ) {
		ww = w;
		while( *w != NULL ) {
			if( debug ) {
				printf("(%d) Match '", level);
				for( wdp = ww ; wdp <= w ; wdp++ )
					printf((wdp == ww) ? "%s" : ".%s", *wdp);
				printf("' with '*'\n");
			}
			i = match(w+1, p+1);
			if( i > -5000 )
				break;
			w++;
		}
		level--;
		return i + 7*(w-ww) + 9;    /* 7 pts per each word + 9 pts for the first */
	}
	if( **p == '?' ) {      /* Match one word */
		if( debug )
			printf("(%d) Match '%s' with '?'\n", level--, *w);
		return match(w+1, p+1) + 10;
	}

	if( debug )
		printf("(%d) Matching '%s' with '%s' ", level--, *w, *p);
	inverse = 0;
	b = *p;
	if( *b == '!' )
		inverse++, b++;
	for(;;) {
		a = *w;
		while( *a && *a == *b ) a++, b++;
		if( (*b == ',' || *b == '\0') && *a == '\0' ) {   /* OK */
			if( inverse ) {
				if( debug )
					printf("FAILS\n");
				return -10000;
			} else {
				if( debug )
					printf("OK\n");        /* Exact matching */
				return match(w+1, p+1) + 100;
			}
		}
		while( *b && *b != ',' ) b++;           /* Scan to next */
		if( *b == '\0' ) {
			if( inverse ) {
				if( debug )
					printf("OK\n");
				return match(w+1, p+1) + 10;            /* Like ? */
			} else {
				if( debug )
					printf("FAILS\n");
				return -10000;
			}
		}
		b++;
	}
}
#endif /* MANUAL_ROUTING */

/*
 * Look up for Internet address
 *
 *      0 - Lookup Failed
 *      1 - OK
 *      2 - OK, stripping host/domain part is wanted
 *      3 - Local Address
 *      4 - ambig. address
 *      5 - localhost.UUCP
 */
int lookup(src, dst)
	char *src;
	char *dst;
{
	char    addr[256];
	char   *words[50];
	int     nwords, c, score, maxscore;
	char  **wp;
	register char *p, *q;
	struct RENT *rp, *bestp;

	if( debug )
		printf("Lookup for host '%s'\n", src);

	/*
	 * Convert name into lower case
	 */
	p = src;
	q = addr;
	while( c = *p++ )
		*q++ = tolower(c);
	*q = '\0';

	/* Try to route automatically */
#ifdef AUTOMATIC_ROUTING
	if( (c = findroute(src, dst)) != 0 )
		return c;
#endif /* AUTOMATIC_ROUTING */

	/* Try to use manual routing table */
#ifdef MANUAL_ROUTING
	if( !mr_enabled )       /* There are no MANUAL routing table */
		return 0;

	/*
	 * Split source address into words
	 */
	p = addr;
	wp = words;
	for(;;) {
		nwords++;
		*wp++ = p;
		while( *p && *p != '.' ) p++;
		if( *p == '\0' )
			break;
		*p++ = '\0';
	}
	*wp = NULL;
	if( debug )  {
		printf("> ");
		for( wp = words ; *wp != NULL ; wp++ )
			printf(" %s", *wp);
		printf("\n");
	}

	/*
	 * Scan through patterns and get the best hit
	 */
	maxscore = -5000;
	bestp = NULL;
	for( rp = routetab ; rp != NULL ; rp = rp->next ) {
		/*
		 * Try to match with pattern
		 */
		if( debug )  {
			printf("-------- compare with ");
			for( wp = rp->patt ; *wp != NULL ; wp++ )
				printf(" %s", *wp);
			printf("\n");
		}
		score = match(words, rp->patt);
		if( debug )
			printf("-------- score=%d\n", score);

		/*
		 * Record the best score
		 */
		if( score > maxscore ) {
			bestp = rp;
			maxscore = score;
		}
	}

	/*
	 * If found then return relay/destination address
	 */
	if( bestp == NULL )
		return 0;
	if( debug )
		printf("The best route is to: '%s'\n", bestp->dest);
	if( bestp->dest[0] == '-' && bestp->dest[1] == '\0' )
		return 0;       /* Fail */
	if( bestp->dest[0] == '@' && bestp->dest[1] == '\0' )
		return 3;       /* Local */
	if( bestp->dest[0] == '*' && bestp->dest[1] == '\0' )
		return 1;       /* Found */
	if( bestp->dest[0] == '#' && bestp->dest[1] == '\0' )
		return 5;       /* locallink.uucp */
	strcpy(dst, bestp->dest);
	strcat(dst, "!");               /* Mark an UUCP path */
	return (bestp->mflags & MF_STRIP)? 2 : 1;
#else
	return 0;
#endif  /* MANUAL_ROUTING */
}


#ifdef ALIASES
#include <pwd.h>

FILE *af;

/*
 * Processing of aliases
 */
int aliasit(addr, wildcard)
	char *addr;
{
	long savepos;
	static int level = 0;
	register c;
	char  nbuf[100], wbuf[100], *p, *q;
	int skipf;
	FILE *forw;
	struct passwd *pw, *getpwnam();

	if( ++level > 20 ) {
		if( debug )
			printf("Loop in aliases (%s)\n", addr);
		--level;
		return 1;       /* do not deliver to looping alias */
	}

	/*
	 * First - open the aliases file
	 */
	if( af == (FILE *)1 )
		goto checkforward;
	if( af == NULL ) {
		if( (af = fopen(ALIASES, "r")) == NULL ) {
			af = (FILE *)1; /* don't try to open it again */
			goto checkforward;
		}
	}
	savepos = ftell(af);            /* get current position */
	fseek(af, 0l, 0);               /* rewind it */

	/*
	 * Scan for given name
	 */
	skipf = 0;
	p = nbuf;
	while( (c = getc(af)) != EOF ) {
		if( c == ' ' || c == '\t' ) {
			if( skipf )
				goto skipentry;
			continue;
		}
		if( c == '#' ) {
			while( (c = getc(af)) != EOF && c != '\n' ) ;
			if( c == EOF )
				goto rewind;
			p = nbuf;
			continue;
		}
		if( c == '\n' ) {
			p = nbuf;
			continue;
		}
		skipf = 0;
		if( c == ':' ) {        /* Name collected */
			*p = '\0';
			if( ( wildcard && !strcmp(nbuf, "*")) ||
			    (!wildcard && !strxcmp(nbuf, addr)) ) {
				/*
				 * Alias found
				 */
				for(;;) {
					p = nbuf;
					while( (c = getc(af)) != ',' && c != '\n' && c != EOF ) {
						if( c == '#' ) {
							while( (c = getc(af)) != EOF && c != '\n' ) ;
							break;
						}
						if( c == ' ' || c == '\t' )
							continue;
						if( p > &nbuf[sizeof nbuf - 1] )
							continue;
						*p++ = c;
					}
					if( p != nbuf ) {
						*p = '\0';
						if( wildcard ) {
							p = index(nbuf, '$');
							if( p != NULL ) {
								*p = '\0';
								strcpy(wbuf, nbuf);
								strcat(wbuf, addr);
								strcat(wbuf, ++p);
								strcpy(nbuf, wbuf);
							}
						}
						convert(nbuf);
					}
				clp:    if( c == EOF )
						break;
					if( c == '\n' ) {
						c = getc(af);
						if( c == '#' ) {
							while( (c = getc(af)) != EOF && c != '\n' ) ;
							goto clp;
						}
						if( c != ' ' && c != '\t' )
							break;
					}
				}
				fseek(af, savepos, 0);
				--level;
				return 1;
			}

			/*
			 * Skip until next entry
			 */
			skipf++;
		skipentry:
			while( (c = getc(af)) != EOF && c != '\n' ) ;
			if( c == EOF )
				goto rewind;
			p = nbuf;
			continue;
		}

		/* letter */
		if( p < &nbuf[sizeof nbuf - 1] )
			*p++ = c;
	}

	/* Not found */
rewind: fseek(af, savepos, 0);

	/*
	 * Look for user .forward file
	 */
checkforward:
	if( wildcard ) {
		--level;
		return 0;
	}

	setpwent();
	user_exist = 1;
	if( (pw = getpwnam(addr)) == NULL ) {
		--level;
		user_exist = 0;
		return 0;       /* true local name */
	}

	nbuf[0] = '\0';                 /* handle root dir */
	if( strcmp(pw->pw_dir, "/") )
		strcpy(nbuf, pw->pw_dir);
	strcat(nbuf, "/");
	strcat(nbuf, ".forward");
	if( (forw = fopen(nbuf, "r")) == NULL ) {
		--level;
		return 0;
	}
	while( fgets(nbuf, sizeof nbuf - 1, forw) != NULL ) {
		nbuf[sizeof nbuf - 1] = '\0';
		if( (p = index(nbuf, '\n')) != NULL )
			*p = '\0';
		p = nbuf;
		while( *p == ' ' || *p == '\t' ) p++;
		if( *p == '\0' )
			continue;
		q = p;
		while( *p != '\0' ) {
			while( *q != '\0' && *q != ' ' &&
			       *q != '\t' && *q != ',' ) q++;
			while( *q == ' ' || *q == '\t' ) q++;
			if( *q != '\0' ) {
				*q++ = '\0';
				while( *q == ' ' || *q == '\t' || *q == ',' ) q++;
			}
			convert(p);
			p = q;
		}
	};
	fclose(forw);
	--level;
	return 1;
}
#endif /* ALIASES */

/*
 * Get a current host name for replies
 */
gethost(buf, len)
	char buf[];
{
	int     fd[2];
	int     l, rlen;
	static char hn[80] = "";
	int     istream;
	char *p;

	if( hn[0] != '\0' ) {
		strncpy(buf, hn, len);
		return;
	}

	istream = open("/etc/myhostname", 0);
	if( istream >= 0 ) {
		(void) read(istream, hn, 79);
		close( istream );
		goto got;
	}

	pipe(fd);
	if( fork() == 0 ) {
		/* New proc */
		close(fd[0]);
		close(0);
		close(1);
		close(2);
		open("/dev/null", 0);
		dup(fd[1]);
		dup(fd[1]);
		close(fd[1]);

		execl("/usr/bin/hostname", "hostname", 0);
		execl("/bin/hostname", "hostname", 0);
		execl("/etc/hostname", "hostname", 0);
		exit(1);
	}
	close(fd[1]);
	rlen = read(fd[0], hn, 79);
	close(fd[0]);
	wait(&l);
	if( l != 0 || rlen <= 0 ) {     /* Error */
		strcpy(hn, uuname);
		strcat(hn, ".UUCP");
	}
got:    hn[79] = '\0';
	if( (p = index(hn, '\n')) != NULL )
		*p = '\0';
	/* Ugh! */
	strncpy(buf, hn, len);
}


#ifdef LOGFILE

#include <time.h>
/*
 * Write the log record
 */
logmail(from, to, size)
	char *from;
	char *to;
	long size;
{
	int fd;
	char buf[300];
	long now;
	struct tm *t;

	if( (fd = open(LOGFILE, 1)) == -1 )
		return;
	time(&now);
	t = localtime(&now);
	sprintf(buf, "%s\t%s\t%ld\t%02d:%02d:%02d %d.%d.%02d\n", from, to,
		     size, t->tm_hour, t->tm_min, t->tm_sec,
			   t->tm_mday, t->tm_mon + 1, t->tm_year % 100);
	lseek(fd, 0l, 2);
	write(fd, buf, strlen(buf));
	close(fd);
}
#endif

/*
 * Check if an Internet address is local one
 */
int nonlocal(a)
	char *a;
{
	register char *p, *q;
	char  c, d;
	char  hn[80];

	localpart[0] = '\0';
	if( (p = index(a, '@')) == NULL &&
	    (p = index(a, '%')) == NULL )
		return 0;                       /* surely local address */
	if( index(p+1, '@') != NULL || index(p+1, '%') != NULL )
		return 1;                       /* surely non-local address */
	c = *p;
	*p = '\0';
	strcpy(localpart, a);
	*p++ = c;
	gethost(hn, sizeof hn);
	q = hn;

	/* Compare with the local host name */
	while( *p ) {
		c = *p++; c = tolower(c);
		d = *q++; d = tolower(d);
		if( c != d ) {
			if( c == '\0' && d == '.' )
				return 0;       /* Local address (approximately) */
			localpart[0] = '\0';
			return 1;
		}
	}
	return 0;               /* Local address (exact) */
}

/*
 * Quote the argument for uux (flag=0) or for shell only (flag=1)
 */
char *uufix(s, flag, fixat)
	register char *s;
{
	static char bb[256];
	register char *q;
	int uf = 0, sf = 0;

	for( q = s ; *q ; q++ ) {
		if( *q == '!' || *q == '/' || *q == '~' ) {
			sf = uf = 1;
			break;
		}
		if( ('0' > *q || *q > '9') &&     /* Shell safe chars */
		    ('a' > *q || *q > 'z') &&
		    ('A' > *q || *q > 'Z' ) &&
		    *q != '@' && *q != '.' && *q != '-' &&
		    *q != '+' && *q != '/' && *q != '_' )
			sf = 1;
	}
	if( flag )
		uf = 0;
	q = bb;
	if( sf ) *q++ = '\'';
	if( uf ) *q++ = '(';
	while( *q = *s++ ) {
		if( fixat && *q == '@' ) *q = '%' ;
		q++ ;
	};
	if( uf ) *q++ = ')';
	if( sf ) *q++ = '\'';
	*q = '\0';
	return bb;
}

#ifdef AUTOMATIC_ROUTING
/*
 * Structure of the Systems list
 */
struct SYSTEM {
	struct SYSTEM  *s_next;         /* Next entry */
	char           *s_path;         /* Pathname of the remote system */
	short           s_nor;          /* Number of route */
};

struct DIRSYSTEM {
	struct DIRSYSTEM *ds_next;      /* Next entry */
	char             *ds_name;      /* Name of the system */
	char             *ds_path;      /* Path to the remote system */
	char              ds_flags;     /* Mailer flags */
	char              ds_nice;      /* Additional score penalty */
};

static int      LineNo;         /* Current Line No */
static int      ErrCnt;         /* Error Counter */

static FILE    *rof;    /* Opened Routes file */

int    xatoi();

struct SYSTEM    *syslist  = NULL;  /* List of systems */
struct DIRSYSTEM *dsyslist = NULL;


/*
 * Read the Systems file, returns 1 on fail
 */
rsystems()
{
	FILE *f;
	register char *p, *q;
	char line[300];
	char worka[250];
	struct SYSTEM    *sy;
	struct DIRSYSTEM *dsy;

	/*
	 * Add a local system into the list
	 */
	gethost(line, 200);
	dsy = (struct DIRSYSTEM *)malloc(sizeof(struct DIRSYSTEM));
	if( dsy == NULL )
		goto nomem;
	dsy->ds_name = (char *)malloc(strlen(line)+1);
	if( dsy->ds_name == NULL )
		goto nomem;
	strcpy(dsy->ds_name, line);
	dsy->ds_path = "@";
	dsy->ds_next = dsyslist;
	dsy->ds_flags = 0;
	dsy->ds_nice = 0;
	dsyslist = dsy;

	/*
	 * Add a local UUCP name
	 */
	dsy = (struct DIRSYSTEM *)malloc(sizeof(struct DIRSYSTEM));
	if( dsy == NULL )
		goto nomem;
	dsy->ds_name = (char *)malloc(strlen(uuname)+7);
	if( dsy->ds_name == NULL )
		goto nomem;
	strcpy(dsy->ds_name, uuname);
	strcat(dsy->ds_name, ".uucp|");
	dsy->ds_path = "@";
	dsy->ds_next = dsyslist;
	dsy->ds_flags = 0;
	dsy->ds_nice = 0;
	dsyslist = dsy;

	/*
	 * Add a patterns local-uucp-connection.uucp (they should fail)
	 */
	if( (f = popen(UUNAMES, "r")) != NULL ) {
		while( fgets(line, sizeof line, f) != NULL ) {
			if( (p = index(line, '\n')) != NULL )
				*p = '\0';
			/* skip local name */
			if( !strcmp(line, uuname) )
				continue;
			dsy = (struct DIRSYSTEM *)malloc(sizeof(struct DIRSYSTEM));
			if( dsy == NULL )
				goto nomem;
			p = (char *)malloc(strlen(line)+7);
			if( p == NULL )
				goto nomem;
			strcpy(p, line);
			strcat(p, ".uucp|");
			dsy->ds_name = p;
			dsy->ds_path = "#";
			dsy->ds_next = dsyslist;
			dsy->ds_flags = 0;
			dsy->ds_nice = 1; /* A bit worse than explicitly pointed */
			dsyslist = dsy;
		}
		pclose(f);
	}

	/*
	 * Open the Systems file
	 */
	if( (f = fopen(SystemsFile, "r")) == NULL )
		return 1;
	LineNo = 0;

	/*
	 * Read it line-by-line and throw out comments
	 */
	for(;;) {
		LineNo++;
		line[299] = '\0';
		if( fgets(line, 300, f) == NULL )      /* No more entries */
			break;
		if( line[299] != '\0' ) {
			p = "line too long";
		synterr:
			if( ErrCnt++ <= 5 )
				errmsg("ERR in Systems, line %d: %s\n", LineNo, p);
			continue;
		}
		if( (p = index(line, '\n')) != NULL )
			*p = '\0';      /* Cut LF */
		if( (p = index(line, '#')) != NULL )
			*p = '\0';      /* Strip out comment */
		p = line;
		while( *p == ' ' || *p == '\t' ) p++;   /* Skip leading spaces */

		if( *p == '\0' )
			continue;       /* skip blank line */

		/*
		 * Parse % commands
		 */
		if( *p == '%' ) {
			p++;
			switch( *p++ ) {
			    case 'p':
			    case 'P':
				break;

			    case 'e':
			    case 'E':
				while( *p == ' ' || *p == '\t' ) p++;
				strcpy(errors_to, p);
				if( debug )
					printf("SET-ERRORS-TO\n");
				set_errors_to++;
				break;

			    default:
				p = "illegal % command";
				goto synterr;
			}
			continue;
		}

		/*
		 * Check the type of system
		 */
		if( '0' <= *p && *p <= '9' ) {
			sy = (struct SYSTEM *)malloc(sizeof(struct SYSTEM));
			if( sy == NULL )
				goto nomem;

			/* Read number of route */
			if( (sy->s_nor = xatoi(p)) == -1 ) {
				p = "illegal route #";
				goto synterr;
			}
			while( '0' <= *p && *p <= '9' ) p++;

			/* Skip metric value */
			while( *p == ' ' || *p == '\t' ) p++;
			while( '0' <= *p && *p <= '9' ) p++;

			/* Read path to remote machine */
			while( *p == ' ' || *p == '\t' ) p++;
			if( *p == '\0' ) {
				p = "missed path to remote machine";
				goto synterr;
			}
			q = worka;
			while( *p != '\0' && *p != ' ' && *p != '\t' )
				*q++ = *p++;
			*q++ = '\0';
			sy->s_path = (char *)malloc(q-worka);
			if( sy->s_path == NULL )
				goto nomem;
			strcpy(sy->s_path, worka);
			if( index(sy->s_path, '@') == NULL &&
			    index(sy->s_path, '!') == NULL &&
			    index(sy->s_path, '%') == NULL ) {
				sprintf(worka, "illegal network path (%s)", sy->s_path);
				p = worka;
				goto synterr;
			}

			/* Add the system in the list */
			sy->s_next   = syslist;
			syslist = sy;
			continue;
		}

		/*
		 * Allocate memory for DIRSYSTEM
		 */
		dsy = (struct DIRSYSTEM *)malloc(sizeof(struct DIRSYSTEM));
		if( dsy == NULL )
			goto nomem;

		/*
		 * Fetch host/domain name
		 */
		if( *p == '\0' || *p == ' ' || *p == '\t' ) {
			p = "missed host/domain name";
			goto synterr;
		}
		q = worka;
		while( *p != '\0' && *p != ' ' && *p != '\t' )
			*q++ = *p++;
		*q++ = '\0';
		dsy->ds_name = (char *)malloc(q-worka);
		if( dsy->ds_name == NULL )
			goto nomem;
		strcpy(dsy->ds_name, worka);

		/*
		 * Skip the metric value
		 */
		while( *p == ' ' || *p == '\t' ) p++;
		while( '0' <= *p && *p <= '9' ) p++;
		if( *p != ' ' && *p != '\t' ) {
			p = "bad metric value";
			goto synterr;
		}

		/*
		 * Get the path to remote machine
		 */
		while( *p == ' ' || *p == '\t' ) p++;
		if( *p == '\0' ) {
			p = "missed path to remote";
			goto synterr;
		}
		q = worka;
		while( *p != '\0' && *p != ' ' && *p != '\t' )
			*q++ = *p++;
		*q++ = '\0';
		dsy->ds_flags = MF_QUEUE;
		if( (q = index(worka, '/')) != NULL ) {
			if( q == worka ) {
				p = "missed host/domain name";
				goto synterr;
			}
			*q++ ='\0';
			if( index(q, 'S') != NULL )
				dsy->ds_flags |= MF_STRIP;
			if( index(q, 'I') != NULL )
				dsy->ds_flags &= ~MF_QUEUE;
			if( index(q, 'F') != NULL )
				dsy->ds_flags |= MF_SFROM;
			if( index(q, 'B') != NULL )
				dsy->ds_flags |= MF_BATCH;
		}
		dsy->ds_path = (char *)malloc(strlen(worka)+1);
		if( dsy->ds_path == NULL )
			goto nomem;
		strcpy(dsy->ds_path, worka);
		dsy->ds_next = dsyslist;
		dsy->ds_nice = 0;
		dsyslist = dsy;
	}
	(void) fclose(f);

	if( debug ) {
		printf("---------  DIRSYSTEMS ----------------\n");
		for( dsy = dsyslist ; dsy != NULL ; dsy = dsy->ds_next ) {
			printf("%25s %s", dsy->ds_name, dsy->ds_path);
			if( dsy->ds_flags & MF_STRIP )
				printf("/S");
			if( !(dsy->ds_flags & MF_QUEUE) )
				printf("/I");
			if( dsy->ds_flags & MF_SFROM )
				printf("/F");
			if( dsy->ds_flags & MF_BATCH )
				printf("/B");
			if( dsy->ds_nice )
				printf(" -%d", dsy->ds_nice);
			printf("\n");
		}
		printf("--------------------------------------\n");
	}

	/*
	 * Open Routes file
	 */
	rof = fopen(RoutesFile, "r");
	return 0;
nomem:
	errmsg("No memory for Systems\n");
	exit(EX_SOFTWARE);
}

static char *uawords[15];
static char  uabuf[250];
static int   nuawords;

/*
 * Parse user address
 */
uaparse(a)
	char *a;
{
	register char *p, **wp;

	while( *a && *a == '.' ) a++;   /* Skip leading dots ... */
	strcpy(uabuf, a);
	wp = uawords;
	*wp++ = uabuf;
	p = uabuf;
	while( *p ) {
		if( 'A' <= *p && *p <= 'Z' )    /* upper-->lower */
			*p |= 040;
		if( *p == '.' ) {
			*p = '\0';
			if( wp < &uawords[16] &&
			    p[1] != '\0' && p[1] != '.' )
				*wp++ = &p[1];
		}
		p++;
	}
	nuawords = wp - uawords;
}

/*
 * Compare route with the user supplied address
 *      Returns:        -1 if fails
 *                  score  for match
 *
 *       1 pt  for shifting user pattern to the left side
 *      10 pts for matching a word with wildcard
 *      -4 pts for successful search through db
 *      -5 pts for word matching
 */
rcmp(adr, subs)
	char *adr;
	char *subs;
{
	int score = 100;
	register char *p, *q;
	char **uw;
	char buf[250], *r;
	extern char *bsearch();
	char fn[100];
	int refuseshift = 0, i;

	if( debug )
		printf("<<< %s >>>\n", adr);
	strcpy(buf, adr);
	subs[0] = '\0';

	/*
	 * Loop by pattern words from the end to the beginning
	 */
	uw = &uawords[nuawords-1];      /* The Last Word */
	p = buf;
	while( *p ) p++;
	if( p[-1] == '|' ) {
		*--p = '\0';
		refuseshift++;
	}

nextword:
	while( p > buf && *p != '.' ) --p;
	if( *p == '.' ) {
		*p = '\0';
		q = p+1;
	} else
		q = p;
	if( debug )
		printf("UWORD '%s' PWORD '%s' ", *uw, q);
	if( q[0] == '*' && q[1] == '\0' ) {
		if( debug )
			printf("WILDCARD\n");
		score += (uw - uawords + 1)*10;
		strcpy(subs, *uw);
		goto ret;
	}
	if( *q == '$' ) {
		sprintf(fn, "%s/%s", SERV_DIR, &q[1]);
		if( (r = bsearch(*uw, fn)) == NULL ) {
			if( debug )
				printf("DB SEARCH FAILED\n");
			goto cmpfail;
		}
		strcpy(subs, r);
		if( debug )
			printf("DB SEARCH OK: '%s'\n", subs);
		i = 3;
		goto accepted;
	}
	if( !strxcmp(*uw, q) ) {
		if( debug )
			printf("ACCEPTED\n");
		i = 5;
accepted:
		refuseshift++;
		score -= i;
		if( --uw < uawords ) {
			if( q == &buf[2] && buf[0] == '*' ) {
				if( debug )
					printf("MATCHING WILDCARD WITH NULL\n");
				score += 10;
				goto ret;

			}
			if( q != buf ) {
				if( debug )
					printf("PATTERN TOO LONG\n");
				return -1;
			}
			goto ret;
		}
		goto nextword;
	}
cmpfail:
	if( q != buf ) {
		if( refuseshift ) {
			if( debug )
				printf("SHIFT REFUSED\n");
			return -1;
		}
		if( debug )
			printf("SHIFT\n");
		score += 1;
		goto nextword;
	}
	if( debug )
		printf("END OF PATTERN\n");
	return -1;

ret:
	return score;
}

/*
 * Compare with ignoring letter case
 */
strxcmp(pat, w)
	register char *pat, *w;
{
	register char c, d;

	while( *pat ) {
		c = *w++;
		if( 'A' <= c && c <= 'Z' )
			c |= 040;
		d = *pat++;
		if( 'A' <= d && d <= 'Z' )
			d |= 040;
		if( d != c )
			return 1;
	}
	return (*w != '\0');
}

/*
 * Find the route to the machine with given name
 *
 *      addr         - a destination address
 *      bestroute    - buffer for placing route in
 * Result:
 *      0       - route not found
 *      1       - OK
 *      2       - OK, stripping host/domain part is wanted
 *      3       - Local Address
 *      4       - ambigious routing
 *      5       - locallink.uucp
 */
int findroute(addr, bestroute)
	char *addr;
	char *bestroute;
{
	struct DIRSYSTEM *dsy;
	struct SYSTEM *sy;
	static char    worka[400];
	static char    rsubst[300], csu[300];
	short   nor, flags;
	int     bestscore, score, ambig, bflags;
	char    *nm, *pt;
	int     nice;

#ifdef MANUAL_ROUTING
	/*
	 * Process initialization
	 */
	if( firsttime ) {
		if( firsttime == 2 )
			return 0;       /* Failure */
		firsttime = 0;
		if( rsystems() ) {
			firsttime = 2;
			return 0;
		}
	}
#endif /* MANUAL_ROUTING */

	/*
	 * Main loop
	 */
	uaparse(addr);
	dsy = dsyslist;
	bestscore = 10000;      /* Huge value */
	bestroute[0] = '\0';    /* Not found */
	ambig = 0;
	LineNo = 0;
	if( rof != NULL )
		rewind(rof);
	for(;;) {
		/*
		 * Try next route
		 */
		if( dsy != NULL ) {
			nm  = dsy->ds_name;
			pt  = dsy->ds_path;
			flags = dsy->ds_flags;
			nice = dsy->ds_nice;
			dsy  = dsy->ds_next;
		} else if( rof != NULL ) {
			if( (nor = grent(worka)) == -1 )
				break;
			if( debug )
				printf("*** NOR=%d\n", nor);
			flags = 0;
			pt = NULL;
			nm = worka;
			nice = 0;
		} else
			break;

		/*
		 * Compare route with the name
		 * and skip if not good
		 */
		if( (score = rcmp(nm, csu)) == -1 )
			continue;
		score += nice;
		if( debug )
			printf("SCORE: %d\n", score);

		/*
		 * Compare score with the best collected and
		 * go out if current score is worse.
		 */
		if( score > bestscore )
			continue;

		/*
		 * Find route's path throug syslist
		 */
		if( pt == NULL ) {
			for( sy = syslist ; sy != NULL ; sy = sy->s_next ) {
				if( sy->s_nor == nor )
					goto found;
			}
			errmsg("cannot find path for route # %d\n", nor);
			continue;
		found:
			pt = sy->s_path;
		}

		/*
		 * Update the BEST score
		 */
		if( score == bestscore && strcmp(pt, bestroute) )
			ambig++;
		else {  /* score < bestscore */
			ambig = 0;
			bestscore = score;
			bflags = flags;
			strcpy(bestroute, pt);
			strcpy(rsubst, csu);
		}
	}

	/*
	 * Report results
	 */
	if( (pt = index(bestroute, '$')) != NULL ) {
		*pt++ = '\0';
		strcpy(worka, bestroute);
		strcat(worka, rsubst);
		strcat(worka, pt);
		strcpy(bestroute, worka);
	}
	mailer_flags = bflags & ~MF_STRIP;
	if( bestroute[0] == '\0' ||
	    (bestroute[0] == '-' && bestroute[1] == '\0') )
		return 0;
	if( ambig )
		return 4;
	if( bestroute[0] == '@' && bestroute[1] == '\0' )
		return 3;
	if( bestroute[0] == '*' && bestroute[1] == '\0' )
		return 1;
	if( bestroute[0] == '#' && bestroute[1] == '\0' )
		return 5;
	return (bflags & MF_STRIP) ? 2 : 1;
}

/*
 * Get an entry from the Routes file
 *      (name is the place to put routes in)
 *      Returns -1 if no more information, otherwise returns
 *      number of route
 */
int grent(name)
	char *name;
{
	char line[300], buf[250];
	static char prevNAME[250] = "";
	static int  prevWORDS = 0;
	register char *p, *q;
	int i, nor;
	int tailwords;  /* N of words from previous entry */

nextline:
	LineNo++;
	line[299] = '\0';
	if( fgets(line, 300, rof) == NULL )       /* No more entries */
		return -1;
	if( line[299] != '\0' ) {
		p = "line too long";
	synterr:
		if( ErrCnt++ <= 5 )
			errmsg("ERR in Routes, line %d: %s\n", LineNo, p);
		goto nextline;
	}
	if( (p = index(line, '\n')) != NULL )
		*p = '\0';      /* Cut LF */
	if( (p = index(line, '#')) != NULL )
		*p = '\0';      /* Strip out comment */

	p = line;
	while( *p == ' ' || *p == '\t' ) p++;   /* Skip leading spaces */

	if( *p == '\0' || *p == '%' )
		goto nextline;  /* skip blank line */

	/*
	 * Read inheritance counter
	 */
	if( '0' > *p || *p > '9' ) {
		p = "no inheritance counter";
		goto synterr;
	}
	tailwords = *p++ - '0';
	if( tailwords > prevWORDS ) {
		p = "too many words inherited";
		goto synterr;
	}

	/*
	 * Fetch host/domain name
	 */
	if( *p == 0 || *p == ' ' || *p == '\t' ) {
		p = "missed host/domain name";
		goto synterr;
	}
	q = buf;
	while( *p != '\0' && *p != ' ' && *p != '\t' )
		*q++ = *p++;
	*q = '\0';

	/*
	 * Fetch number of route
	 */
	while( *p == ' ' || *p == '\t' ) p++;
	if( *p == '-' && (p[1] == ' ' || p[1] == '\t') ) {
		/* Explicit route - oh, yeah, we'he passed thru this one
		   while reading Systems file */
		nor = -1;
	} else if( (nor = xatoi(p)) == -1 ) {
		p = "bad route #";
		goto synterr;
	}

	/*
	 * Count the words
	 */
	for( p = buf, i = 1; *p != '\0' ; p++ )
		if( *p == '.' )
			i++;
	if( i + tailwords > 10 ) {
		p = "too many words";
		goto synterr;
	}
	prevWORDS = i + tailwords;

	/*
	 * Copy tail words to the beginning of f_words
	 */
	if( tailwords > 0 ) {
		p = prevNAME;
		while( *p ) p++;
		while( p > prevNAME && tailwords ) {
			if( *--p == '.' )
				tailwords--;
		}
		if( p == prevNAME )
			*q++ = '.';
		strcpy(q, p);
	}
	strcpy(prevNAME, buf);
	if( nor == -1 )
		goto nextline;
	strcpy(name, buf);
	return nor;
}

/*
 * xatoi - like an ordinary atoi but returns -1 on error
 */
int xatoi(s)
	register char *s;
{
	register int l;

	l = 0;
	while( '0' <= *s && *s <= '9' )
		l = l*10 + (*s++ - '0');
	if( *s != '\0' && *s != ' ' && *s != '\t' )
		return -1;
	return l;
}
#endif /* AUTOMATIC_ROUTING */

/*
 * Put an error message to stderr or mail it to postmaster when
 * in off-line mode.
 */
/*VARARGS 1*/
errmsg(f, a, b, c, d, e)
	char *f;
{
	static FILE *erf = NULL;
	char hn[200];
	char *p;

	if( erf == NULL ) {
		if( isatty(2) )
			erf = stderr;
		else {
			gethost(hn, sizeof hn);
			sprintf(hn, "%s -r uucp@%s %s", ERROR_MAILER, hn, errors_to);
			erf = popen(hn, "w");
			if( erf == NULL )
				erf = stdout;
			else {
				fprintf(erf, "From: uucp@%s (UUCP routing service)\n", hn);
				fprintf(erf, "To: %s\n", errors_to);
				fprintf(erf, "Subject: Troubles collecting routes\n\n");
			}
		}
	}
	fprintf(erf, f, a, b, c, d, e);
	/* Anyway, print errors to stderr */
	if( erf != stdout && erf != stderr )
		fprintf(stderr, f, a, b, c, d, e);
}

/*
 * Convert address from Internet path to bang path (see RFC976)
 */
char *inet2bang(adr)
	char *adr;
{
	char buf[256];
	static char outbuf[256];
	register char *p, *q;

	strcpy(buf, adr);

	outbuf[0] = '\0';

	/* Convert a@b -> b!a */
	while( (p = rindex(buf, '@')) != NULL ) {
		*p++ = '\0';
		strcat(outbuf, p);
		if( index(p, '.') == NULL )
			strcat(outbuf, ".");
		strcat(outbuf, "!");
	}

	/* Copy UUCP part */
	if( (q = rindex(buf, '!')) != NULL ) {
		*q++ = '\0';
		strcat(outbuf, buf);
		strcat(outbuf, "!");
	} else
		q = buf;

	/* Convert a%b -> b!a */
	while( (p = rindex(q, '%')) != NULL ) {
		*p++ = '\0';
		strcat(outbuf, p);
		if( index(p, '.') == NULL )
			strcat(outbuf, ".");
		strcat(outbuf, "!");
	}
	strcat(outbuf, q);
	return outbuf;
}
