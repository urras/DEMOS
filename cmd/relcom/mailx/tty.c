/*
 * Mail -- a mail program
 *
 * Generally useful tty stuff.
 *
 * $Log: tty.c,v $
 * Revision 1.14  1991/07/22  16:36:47  ache
 * Port to Borland C
 *
 * Revision 1.13  1991/07/19  20:00:14  ache
 * dOBAWLENA RASSYLKA SOOB]ENIJ W TELEKONFERENCII
 *
 * Revision 1.12  1991/04/26  17:10:00  ache
 * Cosmetic changes
 *
 * Revision 1.11  1991/04/19  22:50:22  asa
 * iZMENENIQ DLQ dEMOS 32
 *
 * Revision 1.10  1991/01/25  18:18:41  ache
 * ~TOBY POLXZOWATELI BOLX[E NE SPRA[IWALI
 * 	- ~TO TAKOE sUB`EKT: ?
 * Subject, To, Cc I T.P. ZAPRA[IWA@TSQ PO ediag PO RUSSKI
 *
 * Revision 1.9  1991/01/25  18:04:45  ache
 * uBRANY STARYE (4.1) SIGNALY
 *
 * Revision 1.8  90/10/25  01:15:19  ache
 * jmp -> VMUNIX
 * 
 * Revision 1.7  90/10/04  02:28:23  ache
 * if (!isprint(c)) then '$' in readtty
 * 
 * Revision 1.6  90/09/13  13:20:56  ache
 * MS-DOS & Unix together...
 * 
 * Revision 1.5  90/05/31  22:22:58  avg
 * dOBAWLENA OBRABOTKA CTRL/C PRI WWODE POLEJ ZAGOLOWKA.
 * 
 * Revision 1.4  90/04/20  19:17:26  avg
 * pRIKRU^ENO POD System V
 * 
 * Revision 1.3  88/07/23  20:38:56  ache
 * rUSSKIE DIAGNOSTIKI
 * 
 * Revision 1.2  88/01/11  12:35:07  avg
 * dLQ demos 2 SDELAN WWOD BEZ ISPOLXZOWANIQ TIOCSTI (T.K. ESTX RAZNYE
 * N@ANSY W RABOTE EQ S RUSSKIMI BUKWAMI).
 * dOBAWLEN NOXSTR U rcsid.
 * 
 * Revision 1.1  87/12/25  16:01:01  avg
 * Initial revision
 *
 */

#include "rcv.h"

/*NOXSTR
static char rcsid[] = "$Header: /usr/src/Relcom/relcom/mailx/RCS/tty.c,v 1.14 1991/07/22 16:36:47 ache Exp $";
YESXSTR*/

#ifdef TIOCSETA /* Demos2 Only */
#undef TIOCSTI
#endif

#ifdef  MSDOS
#include        <conio.h>
#endif

#ifdef M_SYSV
#define stty(f, s)      ioctl(f, TCSETA, s)
#define gtty(f, s)      ioctl(f, TCGETA, s)
#define sg_erase        c_cc[VERASE]
#define sg_kill         c_cc[VKILL]
#define sgttyb termio

static struct termio svm;       /* Saved old modes */

#undef TIOCSTI
#undef TIOCSETA
#endif

#define _CTRL(c) ((c)&037)

static  int     crterase = 1;    /* TTY allows CRT BS-ing */
static  int     c_erase = _CTRL('H');            /* Current erase char */
static  int     c_kill =  _CTRL('U');            /* Current kill char */
static  int     c_intr = _CTRL('C');
static  int     c_quit = _CTRL('\\');
static  int     hadcont;                /* Saw continue signal */
#ifdef  VMUNIX
static  jmp_buf rewrite;                /* Place to go when continued */
#endif
static char *readtty();

#ifndef TIOCSTI
#ifndef MSDOS
#if defined(TIOCSETA)
#define sgttyb  sgttya
#define stty(f, s)      ioctl(f, TIOCSETA, s)
#define gtty(f, s)      ioctl(f, TIOCGETA, s)
static  int             sflgs;
#endif
static  int     ttyset;                 /* We must now do erase/kill */
static  struct sgttyb ttybuf, xttybuf;
#endif
#endif

/*
 * Read all relevant header fields.
 */
grabh(hp, gflags)
	struct header *hp;
{
#ifdef  VMUNIX
	sigtype signull();
	sigtype (*savecont)();
#endif
#ifndef TIOCSTI
	sigtype (*savesigs[2])();
#endif
	register int s;
	int errs;

# ifdef VMUNIX
	savecont = sigset(SIGCONT, signull);
# endif
	errs = 0;
#ifndef MSDOS
#ifndef TIOCSTI
	ttyset = 0;
#endif
	if (gtty(fileno(stdin), &ttybuf) < 0) {
		perror("gtty");
		return(-1);
	}
	c_erase = ttybuf.sg_erase;
	c_kill = ttybuf.sg_kill;
#ifndef TIOCSTI
	ttybuf.sg_erase = 0;
	ttybuf.sg_kill = 0;
#ifdef TIOCSETA
	sflgs = ttybuf.sg_flags;
	ttybuf.sg_flags |= CBREAK;
	ttybuf.sg_flags &= ~ECHO;
	crterase = ((ttybuf.sg_local & (LCRTERA|LCRTBS|LPRTERA)) ==
					   (LCRTERA|LCRTBS));
	c_intr = ttybuf.sg_intrc;
	c_quit = ttybuf.sg_quitc;
	ttybuf.sg_intrc = 0;
	ttybuf.sg_quitc = 0;
#endif
#ifdef M_SYSV
	svm = ttybuf;
	c_intr = ttybuf.c_cc[VINTR];
	c_quit = ttybuf.c_cc[VQUIT];
	ttybuf.c_cc[VINTR] = 0;
	ttybuf.c_cc[VQUIT] = 0;
	ttybuf.c_lflag &= ~(ICANON|ECHO);
	ttybuf.c_cc[VMIN]  = 1;
	ttybuf.c_cc[VTIME] = 0;
	crterase = (ttybuf.c_lflag & ECHOE) || (c_erase == '\10');
#endif
	if ((savesigs[0] = signal(SIGINT, SIG_IGN)) == SIG_DFL)
		signal(SIGINT, SIG_DFL);
	if ((savesigs[1] = signal(SIGQUIT, SIG_IGN)) == SIG_DFL)
		signal(SIGQUIT, SIG_DFL);
#endif
#endif  /* not MSDOS */
	if (gflags & GTO) {
#ifndef MSDOS
#ifndef TIOCSTI
		if (!ttyset && hp->h_to != NOSTR)
			ttyset++, stty(fileno(stdin), &ttybuf);
#endif
#endif
		hp->h_to = readtty(ediag("To: ", "kOMU: "), hp->h_to);
		if (hp->h_to != NOSTR)
			hp->h_seq++;
	}
	if (gflags & GNGR) {
#ifndef MSDOS
#ifndef TIOCSTI
		if (!ttyset && hp->h_newsgroups != NOSTR)
			ttyset++, stty(fileno(stdin), &ttybuf);
#endif
#endif
		hp->h_newsgroups = readtty(ediag("Newsgroups: ", "tELEKONFERENCII: "), hp->h_newsgroups);
		if (hp->h_newsgroups != NOSTR)
			hp->h_seq++;
	}
	if (gflags & GSUBJECT) {
#ifndef MSDOS
#ifndef TIOCSTI
		if (!ttyset && hp->h_subject != NOSTR)
			ttyset++, stty(fileno(stdin), &ttybuf);
#endif
#endif
		hp->h_subject = readtty(ediag("Subject: ", "tEMA: "), hp->h_subject);
		if (hp->h_subject != NOSTR)
			hp->h_seq++;
	}
	if (gflags & GNEWS) {
#ifndef MSDOS
#ifndef TIOCSTI
		if (!ttyset && hp->h_keywords != NOSTR)
			ttyset++, stty(fileno(stdin), &ttybuf);
#endif
#endif
		hp->h_keywords = readtty(ediag("Keywords: ", "kL@^EWYE SLOWA: "), hp->h_keywords);
		if (hp->h_keywords != NOSTR)
			hp->h_seq++;
#ifndef MSDOS
#ifndef TIOCSTI
		if (!ttyset && hp->h_summary != NOSTR)
			ttyset++, stty(fileno(stdin), &ttybuf);
#endif
#endif
		hp->h_summary = readtty(ediag("Summary: ", "sODERVANIE: "), hp->h_summary);
		if (hp->h_summary != NOSTR)
			hp->h_seq++;
#ifndef MSDOS
#ifndef TIOCSTI
		if (!ttyset && hp->h_distribution != NOSTR)
			ttyset++, stty(fileno(stdin), &ttybuf);
#endif
#endif
		hp->h_distribution = readtty(ediag("Distribution: ", "rASPROSTRANENIE: "), hp->h_distribution);
		if (hp->h_distribution != NOSTR)
			hp->h_seq++;
	}
	if (gflags & GCC) {
#ifndef MSDOS
#ifndef TIOCSTI
		if (!ttyset && hp->h_cc != NOSTR)
			ttyset++, stty(fileno(stdin), &ttybuf);
#endif
#endif
		hp->h_cc = readtty(ediag("Cc: ", "kOPI@: "), hp->h_cc);
		if (hp->h_cc != NOSTR)
			hp->h_seq++;
	}
	if (gflags & GBCC) {
#ifndef MSDOS
#ifndef TIOCSTI
		if (!ttyset && hp->h_bcc != NOSTR)
			ttyset++, stty(fileno(stdin), &ttybuf);
#endif
#endif
		hp->h_bcc = readtty(ediag("Bcc: ", "wTORU@ KOPI@: "), hp->h_bcc);
		if (hp->h_bcc != NOSTR)
			hp->h_seq++;
	}
# ifdef VMUNIX
	sigset(SIGCONT, savecont);
# endif
#ifndef MSDOS
#ifndef TIOCSTI
#ifdef TIOCSETA
	ttybuf.sg_flags = sflgs;
	ttybuf.sg_intrc = c_intr;
	ttybuf.sg_quitc = c_quit;
#endif
#ifdef M_SYSV
	ttybuf = svm;
#endif
	ttybuf.sg_erase = c_erase;
	ttybuf.sg_kill  = c_kill;
	if (ttyset)
		stty(fileno(stdin), &ttybuf);
	signal(SIGINT, savesigs[0]);
	signal(SIGQUIT, savesigs[1]);
#endif
#endif  /* not MSDOS */
	return(errs);
}

/*
 * Read up a header from standard input.
 * The source string has the preliminary contents to
 * be read.
 *
 */

static
char *
readtty(pr, src)
	char pr[], src[];
{
	char ch, canonb[BUFSIZ];
	int c;
	register char *cp, *cp2;
	int sigctr = 0;
#ifdef  VMUNIX
	sigtype ttycont (), signull ();
#endif

EditAgain:
	fputs(pr, stdout);
	flush();
	if (src != NOSTR && strlen(src) > BUFSIZ - 2) {
		printf(ediag("too long to edit\n",
"SLI[KOM DLINNAQ DLQ REDAKTIROWANIQ\n"));
		return(src);
	}
#ifndef TIOCSTI
	if (src != NOSTR)
		cp = copy(src, canonb);
	else
		cp = copy("", canonb);
	fputs(canonb, stdout);
	flush();
#else
	cp = src == NOSTR ? "" : src;
	while (c = *cp++) {
		if (c == c_erase || c == c_kill) {
			ch = '\\';
			ioctl(0, TIOCSTI, &ch);
		}
		ch = c;
		ioctl(0, TIOCSTI, &ch);
	}
	cp = canonb;
	*cp = 0;
#endif
	cp2 = cp;
	while (cp2 < canonb + BUFSIZ)
		*cp2++ = 0;
	cp2 = cp;
# ifdef VMUNIX
	if (setjmp(rewrite))
		goto redo;
	sigset(SIGCONT, ttycont);
# endif
#if defined(TIOCSTI) || (!defined(TIOCSETA) && !defined(M_SYSV) && !defined(MSDOS))
	while (cp2 < canonb + BUFSIZ) {
		c = getc(stdin);
		if (c == EOF || c == '\n')
			break;
		*cp2++ = c;
	}
	*cp2 = 0;
#else
	/* SIMULATE SCREEN EDITING FUNCTIONS */
#ifndef MSDOS
	if (!ttyset)
		ttyset++, stty(fileno(stdin), &ttybuf);
#endif
	for( cp2 = canonb; *cp2 ; cp2++ );
	for(;;) {
		static crterf = 0;

#ifndef MSDOS
		c = getchar();
#else
		c = getch();
#endif
		if( c == c_intr || c == c_quit ) {
#ifndef	MSDOS
#ifdef TIOCSETA
			xttybuf = ttybuf;
			xttybuf.sg_flags = sflgs;
			xttybuf.sg_intrc = c_intr;
			xttybuf.sg_quitc = c_quit;
#endif
#ifdef M_SYSV
			xttybuf = svm;
#endif
			xttybuf.sg_erase = c_erase;
			xttybuf.sg_kill  = c_kill;
			stty(fileno(stdin), &xttybuf);
			ttyset = 0;
#endif	/* not MSDOS */
			putchar('\n');

			if( sigctr ) {
				extern hadintr;
				hadintr++;
				collrub(SIGINT);
			}

			printf(ediag("(Interrupt -- one more to kill letter)\n",
					 "(pRERYWANIE -- ^TOBY UNI^TOVITX PISXMO NUVNO E]E ODNO)\n"));
			sigctr++;
			goto EditAgain;
		}
		if( c == '\n' || c == '\r' || c == EOF ) {
			putchar('\n');
			break;
		}
		if( c == c_erase ) {
			if( cp2 > canonb ) {
				if( crterase )
					printf( "\b \b" );
				else {
					if( !crterf ) {
						crterf++;
						putchar( '[' );
					}
					putc(cp2[-1], stdout);
				}
				*--cp2 = 0;
			}
		} else if( c == c_kill ) {
			if( crterase ) {
				while( cp2-- > canonb )
					printf( "\b \b" );
			} else
				printf( "^U\n" );
			cp2 = canonb;
			*cp2 = 0;
			crterf = 0;
		} else {
			if( !isprint(c) )
				c = '$';
			if( crterf ) {
				crterf = 0;
				putchar( ']' );
			}
			if( cp2 < canonb + BUFSIZ + 1 ) {
				putc(c, stdout);
				*cp2++ = c;
				*cp2 = '\0';
			}
		}
		flush();
	}
#endif
# ifdef VMUNIX
	sigset(SIGCONT, signull);
# endif
	if (c == EOF && ferror(stdin) && hadcont) {
redo:
		hadcont = 0;
		cp = strlen(canonb) > 0 ? canonb : NOSTR;
		clearerr(stdin);
		return(readtty(pr, cp));
	}
#if !defined(TIOCSTI) && !defined(TIOCSETA) && !defined(M_SYSV) && !defined(MSDOS)
	if (cp == NOSTR || *cp == '\0')
		return(src);
	cp2 = cp;
	if (!ttyset)
		return(strlen(canonb) > 0 ? savestr(canonb) : NOSTR);
	while (*cp != '\0') {
		c = *cp++;
		if (c == c_erase) {
			if (cp2 == canonb)
				continue;
			if (cp2[-1] == '\\') {
				cp2[-1] = c;
				continue;
			}
			cp2--;
			continue;
		}
		if (c == c_kill) {
			if (cp2 == canonb)
				continue;
			if (cp2[-1] == '\\') {
				cp2[-1] = c;
				continue;
			}
			cp2 = canonb;
			continue;
		}
		*cp2++ = c;
	}
	*cp2 = '\0';
#endif
	if (equal("", canonb))
		return(NOSTR);
	return(savestr(canonb));
}

# ifdef VMUNIX
/*
 * Receipt continuation.
 */
sigtype ttycont(s)
{

	hadcont++;
	signal(SIGCONT, ttycont);
	longjmp(rewrite, 1);
}

/*
 * Null routine to satisfy
 * silly system bug that denies us holding SIGCONT
 */
sigtype signull(s)
{}
# endif
