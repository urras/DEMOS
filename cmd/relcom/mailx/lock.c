/*
 * A mailing program.
 *
 * Stuff to do version 7 style locking.
 *
 * $Log: lock.c,v $
 * Revision 1.10  1991/07/22  16:36:47  ache
 * Port to Borland C
 *
 * Revision 1.9  1991/03/10  18:26:37  ache
 * uBRAN ungetch (DOS)
 *
 * Revision 1.8  90/10/04  04:25:57  ache
 * sleep TEPERX NE BLOKIRUET SIGINT.
 * 
 * Revision 1.7  90/09/25  18:54:45  ache
 * MS-DOS locking use HIDDEN now.
 * 
 * Revision 1.6  90/09/22  20:25:19  avg
 * ++ POWTORY.
 * 
 * Revision 1.5  90/09/22  20:22:38  avg
 * w Unix SDELANA SOWMESTIMAQ S localmail BLOKIROWKA.
 *
 * Revision 1.4  90/09/13  13:19:48  ache
 * MS-DOS & Unix together...
 * 
 * Revision 1.3  88/07/23  20:35:32  ache
 * rUSSKIE DIAGNOSTIKI
 * 
 * Revision 1.2  88/01/11  12:43:48  avg
 * dOBAWLEN NOXSTR U rcsid.
 * 
 * Revision 1.1  87/12/25  16:00:00  avg
 * Initial revision
 *
 */

#include "rcv.h"
#include <sys/stat.h>
#ifdef  MSDOS
#include	<dos.h>
#include	<time.h>
#ifndef __TURBOC__
void sleep();
#endif
#endif

/*NOXSTR
static char rcsid[] = "$Header: /usr/src/Relcom/relcom/mailx/RCS/lock.c,v 1.10 1991/07/22 16:36:47 ache Exp $";
YESXSTR*/

static char             curlock[PATHSIZE];      /* Last used name of lock */
static	unsigned		locked = 0; 			/* To note that we locked it */


/*
 * Unix v7-style locking using user x-bit
 */
void
lock(file)
char *file;
{
	struct stat stbuf;
	int trycnt;
#ifdef	MSDOS
	unsigned fattr;
#endif

	if( locked )
		return;
	trycnt = 0;
checkagain:
	if( stat(file, &stbuf) < 0 )
		return;
#ifdef	MSDOS
#if __TURBOC__
	fattr = _chmod(file, 0);
#else
	_dos_getfileattr( file, &fattr );
#endif
#endif
#ifndef MSDOS
	if( stbuf.st_mode&01 ) {		 /* user x bit is the lock */
#else
#ifdef __TURBOC__
	if (fattr & FA_HIDDEN) {		 /* hidden bit is the lock */
#else
	if (fattr & _A_HIDDEN) {		 /* hidden bit is the lock */
#endif
#endif
		if( stbuf.st_ctime+60 >= time((time_t *)NULL) ) {
			if( trycnt++ < 12 ) {
				sleep(5);
				goto checkagain;
			}
			fprintf(stderr, ediag("%s busy; try again in a minute\n","%s ZANQT; POWTORITE POPYTKU ^UTX POZVE\n"), file);
			exit(1);
		}
	}
#ifndef MSDOS
	chmod(file, stbuf.st_mode|01);
	locked = stbuf.st_mode & ~01;
#else
#ifdef	__TURBOC__
	_chmod(file, 1, locked = (fattr | FA_HIDDEN) );
#else
	_dos_setfileattr( file, locked = (fattr | _A_HIDDEN) );
#endif
#endif
	strcpy(curlock, file);
}

void
unlock()
{
	if( locked ) {
#ifndef MSDOS
		chmod(curlock, locked);
#else
#ifdef	__TURBOC__
		_chmod(curlock, 1, locked &~ FA_HIDDEN);
#else
		_dos_setfileattr(curlock, locked &~_A_HIDDEN);
#endif
#endif
	}
	locked = 0;
}

#ifdef	MSDOS
#ifndef	__TURBOC__
#include <conio.h>
#include <signal.h>

/* Pauses for a specified number of seconds. */
void sleep(unsigned int interval) {
	time_t	start;

	(void) time(&start);
	while (time((time_t *)NULL) - start < interval)
		if (kbhit()) {
			char c;

			if ((c = getch()) == '\3') {
				raise(SIGINT);
				return;
			}
		}
} /*sleep*/
#endif
#endif /* MSDOS */
