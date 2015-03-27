/*
 * Declarations and constants specific to an installation.
 *
 * DEMOS 2.
 *
 * $Log: local.h,v $
 * Revision 1.9  1991/04/19  22:49:02  asa
 * iZMENENIQ DLQ dEMOS 32
 *
 * Revision 1.8  1990/09/21  21:59:09  ache
 * MS-DOS extends + some new stuff
 * 
 * Revision 1.7  90/09/13  13:21:26  ache
 * MS-DOS & Unix together...
 * 
 * Revision 1.6  90/08/08  17:14:46  avg
 * SENDMAIL->NETMAIL
 * 
 * Revision 1.5  90/08/07  22:12:10  avg
 * oPMSANIE IMENI SENDMAILA WYNESENO W OTDELXNYJ FAJL.
 * 
 * Revision 1.4  90/04/20  19:17:29  avg
 * pRIKRU^ENO POD System V
 * 
 * Revision 1.3  88/08/23  17:43:15  avg
 * sDELANY RUSSKIE help-FAJLY.
 *
 * Revision 1.2  88/01/11  12:27:17  avg
 * dOBAWLENY KUSKI DLQ RABOTY S PMCS.
 * u rcsid POSTAWLENY KOMMENTARII NOXSTR.
 *
 * Revision 1.1  87/12/25  16:01:08  avg
 * Initial revision
 *
 */

/*
 * $Header: /usr/src/Relcom/relcom/mailx/RCS/local.h,v 1.9 1991/04/19 22:49:02 asa Exp $
 */
#ifndef MSDOS
#define NETMAIL                 /* network mailing allowed */

#define GETHOST                         /* System has gethostname syscall */
#define MYGETHOST                       /* XENIX hack by avg */

#ifdef  GETHOST
#define LOCAL           EMPTYID         /* Dynamically determined local host */
#else
#define LOCAL           'V'             /* Local host id */
#endif

#define MAIL            "/usr/lib/relcom/localmail"  /* Name of mail sender */
#define MAILDIR         "/usr/spool/mail"
#define EDITOR          "/bin/ed"       /* Name of text editor */
#define VISUAL          "/usr/local/bin/re"       /* Name of display editor */
#define SHELL           "/bin/csh"                /* Standard shell */
#define MORE            "/usr/local/bin/more"     /* Standard output pager */
#define HELPFILE        "/usr/lib/relcom/Mail.help"    /* Name of casual help file */
#define THELPFILE       "/usr/lib/relcom/Mail.help.~"  /* Name of casual tilde help */
#define RHELPFILE       "/usr/lib/relcom/Mail.rhelp"   /* Name of casual help file */
#define RTHELPFILE      "/usr/lib/relcom/Mail.rhelp.~" /* Name of casual tilde help */
#define POSTAGE         "/usr/adm/maillog"      /* Where to audit mail sending */
#define UIDMASK         0177777         /* Significant uid bits */
#define MASTER          "/usr/lib/relcom/Mail.rc"
#define ALIASES         "/usr/lib/aliases"      /* Sendmail's aliases */
#define APPEND                          /* New mail goes to end of mailbox */
#define CANLOCK                         /* Locking protocol actually works */
#define UTIME                           /* System implements utime(2) */

/* #define PMCS            "/usr/ucb/bin/pmcs"     /* project mailing control system */
/* #define PROJSPOOL       "/usr/spool/projects"   /* KATALOG FAJLOW PROEKTOW */
/* #define PROJMAILER      "/etc/projmail"         /* sender DLQ PROEKTOW */
#ifdef  VMUNIX
#define sigsys  signal
#define sigset  signal
#endif
#else   /*------------------------------MSDOS----------------------*/
/*
 * Declarations and constants specific to an installation.
 *
 * MSDOS.
 *
 */
#define NETMAIL                 /* network mailing allowed */

#define GETHOST                         /* System has gethostname syscall */

#ifdef  GETHOST
#define LOCAL           EMPTYID         /* Dynamically determined local host */
#else
#define LOCAL           'V'             /* Local host id */
#endif

#define MAIL            "mailx.exe"     /* Name of mail sender */
#define MAILDIR         "."
#define EDITOR		"edlin.com"   /* Name of text editor */
#define VISUAL		"me.exe"      /* Name of display editor */
#define SHELL           "command.com"   /* Standard shell */
#define MORE            "wpview.exe"    /* Standard file pager */
#define HELPFILE        "help.txt"    /* Name of casual help file */
#define THELPFILE       "helpt.txt"  /* Name of casual tilde help */
#define RHELPFILE       "rhelp.txt"   /* Name of casual help file */
#define RTHELPFILE      "rhelpt.txt" /* Name of casual tilde help */
#define POSTAGE         "maillog"      /* Where to audit mail sending */
#define MASTER          "mail.rc"
#define ALIASES         "aliases.txt"   /* Sendmail's aliases */
#define APPEND                          /* New mail goes to end of mailbox */
#define CANLOCK                         /* Locking protocol actually works */
#define UTIME                           /* System implements utime(2) */
#endif  /*------------------------------MSDOS----------------------*/
