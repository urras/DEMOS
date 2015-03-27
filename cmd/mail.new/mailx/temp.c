#include "rcv.h"

/*
 * Mail -- a mail program
 *
 * Give names to all the temporary files that we will need.
 *
 * $Log:	temp.c,v $
 * Revision 1.8  90/12/07  14:01:09  ache
 * Правлена обработка временных файлов и мелочи
 * 
 * Revision 1.7  90/09/29  18:22:33  ache
 * <ctype.h> kicked out...
 * 
 * Revision 1.6  90/09/25  18:57:08  ache
 * atexit(unlock) added
 * 
 * Revision 1.5  90/09/21  22:00:44  ache
 * MS-DOS extends + some new stuff
 * 
 * Revision 1.4  90/09/13  13:20:51  ache
 * MS-DOS & Unix together...
 * 
 * Revision 1.3  88/07/23  20:38:45  ache
 * Русские диагностики
 * 
 * Revision 1.2  88/01/11  12:46:03  avg
 * Добавлен NOXSTR у rcsid.
 * 
 * Revision 1.1  87/12/25  16:00:59  avg
 * Initial revision
 * 
 */

/*NOXSTR*/
static char rcsid[] = "$Header: temp.c,v 1.8 90/12/07 14:01:09 ache Exp $";
/*YESXSTR*/

char    tempMail[PATHSIZE];
char    tempQuit[PATHSIZE];
char    tempEdit[PATHSIZE];
char    tempSet[PATHSIZE];
char    tempResid[PATHSIZE];
char    tempMesg[PATHSIZE];
char    tempBack[PATHSIZE];
/*NOXSTR*/
char    tmp[20] = "/tmp";
char    master[PATHSIZE] = MASTER;
char    aliases[PATHSIZE] = ALIASES;
char    hlp[PATHSIZE] = HELPFILE;
char    rhelp[PATHSIZE] = RHELPFILE;
char    helpt[PATHSIZE] = THELPFILE;
char    rhelpt[PATHSIZE] = RTHELPFILE;
#ifdef  NETMAIL
char    sendprog[PATHSIZE] = "";
#endif
#ifdef  MSDOS
extern int deltftemp(), uuiocall(), PopDir();
extern void unlock();
extern char calldir[];
#endif
/*YESXSTR*/

tinit()
{
	register char *cp, *cp2;
	char uname[PATHSIZE];
	register int err = 0;
	register int pid;

	pid = getpid();
#ifndef MSDOS
	sprintf(tempMail, "/tmp/Rs%05d", pid);
	sprintf(tempResid, "/tmp/Rq%05d", pid);
	sprintf(tempQuit, "/tmp/Rm%05d", pid);
	sprintf(tempEdit, "/tmp/Re%05d", pid);
	sprintf(tempSet, "/tmp/Rx%05d", pid);
	sprintf(tempMesg, "/tmp/Ry%05d", pid);
	sprintf(tempBack, "/tmp/Rb%05d", pid);
#else   /* MSDOS */
	PushDir(".");
	if ((cp = value("MAILDIR")) == NOSTR)
		cp = ".";
	cp = copy(cp, master);
	if (cp[-1] != SEPCHAR)
		*cp++ = SEPCHAR;
	*cp = '\0';
	strcpy(aliases, master);
	strcpy(hlp, master);
	strcpy(rhelp, master);
	strcpy(helpt, master);
	strcpy(rhelpt, master);
#ifdef	NETMAIL
	strcpy(sendprog, calldir);
#endif

	strcat(master, MASTER);
	strcat(aliases, ALIASES);
	strcat(hlp, HELPFILE);
	strcat(rhelp, RHELPFILE);
	strcat(helpt, THELPFILE);
	strcat(rhelpt, RTHELPFILE);
#endif  /* MSDOS */
#ifdef  NETMAIL
	strcat(sendprog, SENDMAIL);
#endif
#ifdef	MSDOS
	(void) atexit(uuiocall);
	(void) atexit(PopDir);
	(void) atexit(TmpDelAll);
	(void) atexit(deltftemp);
	(void) atexit(unlock);
#endif  /* MSDOS */

	if (strlen(myname) != 0) {
#ifndef MSDOS
		uid = getuserid(myname);
		if (uid == -1) {
			printf(ediag(
"\"%s\" is not a user of this system\n",
"\"%s\" не пользователь этой системы\n"),
			    myname);
			exit(1);
		}
#endif
	}
	else {
#ifndef MSDOS
		uid = getuid() & UIDMASK;
#else
		uid = -1;
#endif
		if (username(uid, uname) < 0) {
#ifndef MSDOS
			copy("ubluit", myname);
			err++;
			if (rcvmode) {
				printf(ediag("Who are you!?\n","Кто вы!?\n"));
				exit(1);
			}
#else
			printf(ediag("Who are you? ","Кто вы? "));
			if (!intty)
			    printf(ediag("(user name required)\n",
					 "(должно быть имя пользователя)\n"));
			if (!intty || (flush (), gets(myname)) == NOSTR)
				exit(1);
#endif
		}
		else
			copy(uname, myname);

	}
	if ((cp = value("HOME")) == NOSTR)
		cp = ".";
	cp = copy(cp, homedir);
	if (cp[-1] != SEPCHAR)
		*cp++ = SEPCHAR;
	*cp = '\0';
	findmail();
	cp = copy(homedir, mbox);
	copy("mbox", cp);
	cp = copy(homedir, mailrc);
#ifndef MSDOS
	copy(".mailrc", cp);
#else
	copy("mailrc", cp);
#endif
	cp = copy(homedir, deadletter);
#ifndef MSDOS
	copy("dead.letter", cp);
#else
	copy("dead.let", cp);
#endif
	cp = copy(homedir, signature);
#ifndef MSDOS
	copy(".signature", cp);
#else
	copy("personal.sig", cp);
#endif
	if (debug) {
		printf("uid = %d, user = %s, mailname = %s\n",
		    uid, myname, mailname);
		printf("deadletter = %s, mailrc = %s, mbox = %s\n",
			deadletter, mailrc, mbox);
		flush();
	}
}

#ifdef  MSDOS
maketemp(temp)
char *temp;
{
	register char *cp;

	if ((cp = value("TMP")) == NOSTR)
		if ((cp = value("TEMP")) == NOSTR)
			if ((cp = value("TEMPDIR")) == NOSTR)
				cp = value("HOME");
	strcpy(tmp, cp);
	cp = copy(cp, temp);
	if (cp[-1] != SEPCHAR)
		*cp++ = SEPCHAR;
	copy("RtXXXXXX", cp);
	mktemp(temp);
}

/*
   changedir - like chdir() but also changes the current drive
*/

int changedir(path)
char* path;
{
	if ((*path != '\0') && (path[1] == ':')) {
		unsigned char   drive;

		if (((drive = toupper(*path)) >= 'A') && (drive <= 'Z'))
# if defined(__MSDOS__)
			setdisk(drive - (unsigned char)'A');
# else
#ifdef MSDOS
		{
			unsigned    tdrives = 26;

			_dos_setdrive(drive - 'A' + 1, &tdrives);
		}
# else
   error Unsupported OS/compiler
# endif
# endif
		else
			return -1;
		if (path[2] == '\0')
			return 0;
	}
	return chdir(path);
} /*changedir*/

deltftemp()
{
	TmpDel(tf);
}
#endif  /* MSDOS */
