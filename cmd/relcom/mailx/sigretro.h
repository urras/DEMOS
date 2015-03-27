#

/*
 * Define extra stuff not found in signal.h
 *
 * $Log: sigretro.h,v $
 * Revision 1.2  1990/09/13  13:21:35  ache
 * MS-DOS & Unix together...
 *
 * Revision 1.1  87/12/25  16:00:52  avg
 * Initial revision
 * 
 */

/* $Header: /usr/src/Relcom/relcom/mailx/RCS/sigretro.h,v 1.2 1990/09/13 13:21:35 ache Exp $ */

#ifndef SIGRETRO

#define SIGRETRO                                /* Can use this for cond code */

#ifndef SIG_HOLD

#define SIG_HOLD        (int (*)()) 3           /* Phony action to hold sig */
#define BADSIG          (int (*)()) -1          /* Return value on error */

#endif

typedef int     (*sigtype)();

sigtype sigset(), sigsys(), sighold(), sigignore();

#endif

