/*
 * A bunch of global variable declarations lie herein.
 * def.h must be included first.
 *
 * $Log: glob.h,v $
 * Revision 1.5  1991/07/22  16:36:47  ache
 * Port to Borland C
 *
 * Revision 1.4  1990/12/25  03:53:36  ache
 * pRAWLENA DREWNQQ O[IBKA S ZATIRANIEM PAMQTI
 *
 * Revision 1.3  90/12/03  01:00:24  ache
 * iSPRAWLENO NSPACE DLQ 16-BITN. TA^EK
 * 
 * Revision 1.2  90/09/13  13:21:20  ache
 * MS-DOS & Unix together...
 * 
 * Revision 1.1  87/12/25  15:59:32  avg
 * Initial revision
 *
 */

/*
 * $Header: /usr/src/Relcom/relcom/mailx/RCS/glob.h,v 1.5 1991/07/22 16:36:47 ache Exp $
 */

EXTERN int     msgCount;                       /* Count of messages read in */
EXTERN int     mypid;                          /* Current process id */
EXTERN int     rcvmode;                        /* True if receiving mail */
EXTERN int     sawcom;                         /* Set after first command */
EXTERN int     hflag;                          /* Sequence number for network -h */
EXTERN char    *rflag;                         /* -r address for network */
EXTERN char    *Tflag;                         /* -T temp file for netnews */
EXTERN char    nosrc;                          /* Don't source /usr/lib/Mail.rc */
EXTERN char    noheader;                       /* Suprress initial header listing */
EXTERN int     selfsent;                       /* User sent self something */
EXTERN int     senderr;                        /* An error while checking */
EXTERN int     edit;                           /* Indicates editing a file */
EXTERN int     readonly;                       /* Will be unable to rewrite file */
EXTERN int     noreset;                        /* String resets suspended */
EXTERN int     sourcing;                       /* Currently reading variant file */
EXTERN int     loading;                        /* Loading user definitions */
EXTERN int     shudann;                        /* Print headers when possible */
EXTERN int     cond;                           /* Current state of conditional exc. */
EXTERN FILE    *tf;                            /* I/O temp file buffer */
#ifndef MSDOS
EXTERN FILE    *pipef;                         /* Pipe file we have opened */
#endif
EXTERN FILE    *image;                         /* File for image of msg */
EXTERN FILE    *input;                         /* Current command input file */
EXTERN char    *editfile;                      /* Name of file being edited */
EXTERN char    *sflag;                         /* Subject given from non tty */
EXTERN int     outtty;                         /* True if standard output a tty */
EXTERN int     intty;                          /* True if standard input a tty */
EXTERN int     baud;                           /* Output baud rate */
EXTERN char    mbox[PATHSIZE];                 /* Name of mailbox file */
EXTERN char    mailname[PATHSIZE];             /* Name of system mailbox */
EXTERN int     uid;                            /* The invoker's user id */
EXTERN char    mailrc[PATHSIZE];               /* Name of startup file */
EXTERN char    deadletter[PATHSIZE];           /* Name of ~/dead.letter */
EXTERN char    signature[PATHSIZE];            /* Name of ~/.signature */
EXTERN char    homedir[PATHSIZE];              /* Path name of home directory */
EXTERN char    myname[PATHSIZE];               /* My login id */
EXTERN off_t   mailsize;                       /* Size of system mailbox */
EXTERN int     lexnumber;                      /* Number of TNUMBER from scan() */
EXTERN char    lexstring[STRINGLEN];           /* String from TSTRING, scan() */
EXTERN int     regretp;                        /* Pointer to TOS of regret tokens */
EXTERN int     regretstack[REGDEP];            /* Stack of regretted tokens */
EXTERN char    *stringstack[REGDEP];           /* Stack of regretted strings */
EXTERN int     numberstack[REGDEP];            /* Stack of regretted numbers */
EXTERN struct  message *dot;                   /* Pointer to current message */
EXTERN struct  message *message;               /* The actual message structure */
EXTERN struct  var     *variables[HSHSIZE];    /* Pointer to active var list */
EXTERN struct  grouphead       *groups[HSHSIZE];/* Pointer to active groups */
EXTERN struct  ignore          *ignore[HSHSIZE];/* Pointer to ignored fields */
EXTERN char    **altnames;                     /* List of alternate names for user */
EXTERN char    **localnames;                   /* List of aliases for our local host */
EXTERN int     debug;                          /* Debug flag set */
EXTERN int     rmail;                          /* Being called as rmail */

#include <setjmp.h>

EXTERN jmp_buf srbuf;
