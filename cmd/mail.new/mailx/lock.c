#

/*
 * A mailing program.
 *
 * Stuff to do version 7 style locking.
 *
 * $Log:	lock.c,v $
 * Revision 1.8  90/10/04  04:25:57  ache
 * sleep теперь не блокирует SIGINT.
 * 
 * Revision 1.7  90/09/25  18:54:45  ache
 * MS-DOS locking use HIDDEN now.
 * 
 * Revision 1.6  90/09/22  20:25:19  avg
 * ++ повторы.
 * 
 * Revision 1.5  90/09/22  20:22:38  avg
 * В Unix сделана совместимая с localmail блокировка.
 * 
 * Revision 1.4  90/09/13  13:19:48  ache
 * MS-DOS & Unix together...
 * 
 * Revision 1.3  88/07/23  20:35:32  ache
 * Русские диагностики
 * 
 * Revision 1.2  88/01/11  12:43:48  avg
 * Добавлен NOXSTR у rcsid.
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
void sleep();
#endif

/*NOXSTR*/
static char rcsid[] = "$Header: lock.c,v 1.8 90/10/04 04:25:57 ache Exp $";
/*YESXSTR*/

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
	_dos_getfileattr( file, &fattr );
#endif
#ifndef MSDOS
	if( stbuf.st_mode&01 ) {		 /* user x bit is the lock */
#else
	if (fattr & _A_HIDDEN) {		 /* hidden bit is the lock */
#endif
		if( stbuf.st_ctime+60 >= time((time_t *)NULL) ) {
			if( trycnt++ < 12 ) {
				sleep(5);
				goto checkagain;
			}
			fprintf(stderr, ediag("%s busy; try again in a minute\n","%s занят; повторите попытку чуть позже\n"), file);
			exit(1);
		}
	}
#ifndef MSDOS
	chmod(file, stbuf.st_mode|01);
	locked = stbuf.st_mode & ~01;
#else
	_dos_setfileattr( file, locked = (fattr | _A_HIDDEN) );
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
		_dos_setfileattr(curlock, locked & ~_A_HIDDEN);
#endif
	}
	locked = 0;
}

#ifdef	MSDOS
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
			else
				(void) ungetch(c);
		}
} /*sleep*/
#endif /* MSDOS */
