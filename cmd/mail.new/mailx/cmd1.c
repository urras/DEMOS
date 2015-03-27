#include "rcv.h"
#include <sys/stat.h>
#ifdef  MSDOS
#include        <process.h>
#endif

/*
 * Mail -- a mail program
 *
 * User commands.
 *
 * $Log: cmd1.c,v $
 * Revision 1.22  1991/01/25  18:04:45  ache
 * Убраны старые (4.1) сигналы
 *
 * Revision 1.21  90/12/22  22:53:49  ache
 * Сортировка + выдача ФИО
 * 
 * Revision 1.20  90/12/16  21:30:12  ache
 * set nopipe -- копировать файл целиком, потом пускать more.
 * 
 * Revision 1.19  90/12/07  07:40:48  ache
 * Правлена обработка временных файлов и мелочи
 * 
 * Revision 1.18  90/12/03  03:46:12  ache
 * Правлен разнос стека
 * 
 * Revision 1.17  90/11/18  02:34:04  ache
 * Правлена печать заголовков по поводу адреса, даты, длины
 * 
 * Revision 1.16  90/11/18  01:08:32  ache
 * Теперь uudecode передается ДО КУДА декодировать.
 * 
 * Revision 1.15  90/10/13  20:20:40  ache
 * Удаление 'From '
 * 
 * Revision 1.14  90/10/04  04:14:33  ache
 * Добавлено ВКЛЮЧИТЕ ПРИНТЕР
 * 
 * Revision 1.13  90/10/02  00:03:01  ache
 * Uses LPR variable now.
 * 
 * Revision 1.12  90/09/28  17:53:59  ache
 * Lpr,lpr added
 * 
 * Revision 1.11  90/09/21  21:56:45  ache
 * MS-DOS extends + some new stuff
 * 
 * Revision 1.10  90/09/13  13:17:13  ache
 * MS-DOS & Unix together...
 * 
 * Revision 1.9  90/08/15  19:44:04  avg
 * Вделан встроенный pager.
 * 
 * Revision 1.8  90/06/10  00:38:21  avg
 * Добавлена команда @from.
 * Немного правлена выдача даты по header.
 * 
 * Revision 1.7  90/06/05  20:09:56  avg
 * Правлена выдача команды headers.
 * 
 * Revision 1.6  90/04/20  19:16:27  avg
 * Прикручено под System V
 * 
 * Revision 1.5  88/07/23  20:29:51  ache
 * Русские диагностики
 * 
 * Revision 1.4  88/02/19  15:58:05  avg
 * Для экономии памяти в поле m_size заменен long на unsigned
 * для машин типа pdp11.
 * 
 * Revision 1.2  88/01/11  12:21:48  avg
 * Добавлены куски для работы с PMCS.
 * У rcsid поставлены комментарии NOXSTR.
 * 
 * Revision 1.1  87/12/25  15:58:21  avg
 * Initial revision
 *
 */

/*NOXSTR*/
static char rcsid[] = "$Header: /usr/src/relcom/mailx/RCS/cmd1.c,v 1.22 1991/01/25 18:04:45 ache Exp $";
/*YESXSTR*/

/*
 * Print the current active headings.
 * Don't change dot if invoker didn't give an argument.
 */

static int screen;
extern char tempBack[];
extern int uglyfromflag;
extern long getdate();
extern char *ctime();

Xheaders(msgvec)
	int *msgvec;
{
	uglyfromflag = 1;
	headers(msgvec);
	uglyfromflag = 0;
}

headers(msgvec)
	int *msgvec;
{
	register int n, mesg, flag;
	register struct message *mp;
	int size;

	size = screensize();
	n = msgvec[0];
	if (n != 0)
		screen = (n-1)/size;
	if (screen < 0)
		screen = 0;
	mp = &message[screen * size];
	if (mp >= &message[msgCount])
		mp = &message[msgCount - size];
	if (mp < &message[0])
		mp = &message[0];
	flag = 0;
	mesg = mp - &message[0];
	if (dot != &message[n-1])
		dot = mp;
	for (; mp < &message[msgCount]; mp++) {
		mesg++;
		if (mp->m_flag & MDELETED)
			continue;
		if (flag++ >= size)
			break;
		printhead(mesg);
		sreset();
	}
	if (flag == 0) {
		printf(ediag("No more mail.\n", "Больше почты нет.\n"));
		return(1);
	}
	return(0);
}

/*
 * Set the list of alternate names for out host.
 */
local(namelist)
	char **namelist;
{
	register int c;
	register char **ap, **ap2, *cp;

	c = argcount(namelist) + 1;
	if (c == 1) {
		if (localnames == 0)
			return(0);
		for (ap = localnames; *ap; ap++)
			printf("%s ", *ap);
		printf("\n");
		return(0);
	}
	if (localnames != 0)
		cfree((char *) localnames);
	localnames = (char **) calloc(c, sizeof (char *));
	for (ap = namelist, ap2 = localnames; *ap; ap++, ap2++) {
		cp = (char *) calloc(strlen(*ap) + 1, sizeof (char));
		strcpy(cp, *ap);
		*ap2 = cp;
	}
	*ap2 = 0;
	return(0);
}

/*
 * Scroll to the next/previous screen
 */

scroll(arg)
	char arg[];
{
	register int s, size;
	int cur[1];

	cur[0] = 0;
	size = screensize();
	s = screen;
	switch (*arg) {
	case '.':
	case '$':       /* Last screen */
		screen = msgCount/size;
		break;

	case '0':
	case '1':       /* First screen */
		screen = 0;
		break;

	case 0:
	case '+':
		s++;
		if (s * size > msgCount) {
			printf(ediag(
"On last screenful of messages\n",
"На последнем экране сообщений\n"));
			return(0);
		}
		screen = s;
		break;

	case '-':
		if (--s < 0) {
			printf(ediag(
"On first screenful of messages\n",
"На первом экране сообщений\n"));
			return(0);
		}
		screen = s;
		break;

	default:
		printf(ediag(
"Unrecognized scrolling command \"%s\"\n",
"Неизвестная команда прокрутки \"%s\"\n"),
arg);
		return(1);
	}
	return(headers(cur));
}

/*
 * Compute what the screen size should be.
 * We use the following algorithm:
 *      If user specifies with screen option, use that.
 *      If baud rate < 1200, use  5
 *      If baud rate = 1200, use 10
 *      If baud rate > 1200, use 20
 */
screensize()
{
	register char *cp;
	register int s;

	if ((cp = value("screen")) != NOSTR) {
		s = atoi(cp);
		if (s > 0)
			return(s);
	}
#ifndef MSDOS
	if (baud < B1200)
		s = 5;
	else if (baud == B1200)
		s = 10;
	else
#endif
		s = 20;
	return(s);
}

/*
 * Print out the headlines for each message
 * in the passed message list.
 */

Xfrom(msgvec)
	int *msgvec;
{
	uglyfromflag = 1;
	from(msgvec);
	uglyfromflag = 0;
}

from(msgvec)
	int *msgvec;
{
	register int *ip;

	for (ip = msgvec; *ip != (int)NULL; ip++) {
		printhead(*ip);
		sreset();
	}
	if (--ip >= msgvec)
		dot = &message[*ip - 1];
	return(0);
}

decode(msgvec)
	int *msgvec;
{
	register int *ip;
	FILE *ibuf;
	struct message *mp;
	char headline[LINESIZE];
	int hit;

	for (ip = msgvec; *ip != (int)NULL; ip++) {
		mp = &message[*ip-1];
		ibuf = setinput(mp);
		hit = 0;
		for (;;) {
			if (fgets(headline, LINESIZE, ibuf) == NULL) {
				if (!hit)
				printf(ediag("No begin line in %d message\n",
							 "Нет строки begin в %d сообщении\n"),
						*ip);
				break;
			}
			if (ftell(ibuf) >= mp->m_offset + mp->m_size)
				break;
			if (strncmp(headline, "begin ", 6) == 0) {
				hit = 1;
				uudecode(headline, ibuf, mp->m_offset + mp->m_size);
			}
		}
	}
	if (--ip >= msgvec)
		dot = &message[*ip - 1];
	return(0);
}


/*
 * Print out the header of a specific message.
 * This is a slight improvement to the standard one.
 */

printhead(mesg)
{
	struct message *mp;
	char wcount[30], *subjline, dispc, curind;
	char pbuf[BUFSIZ];
	int s;
	long date;
	register char *cp;
	char *mdate;
	char addr[22];

	mp = &message[mesg-1];
	subjline = hfield("subject", mp);
	if (subjline == NOSTR)
		subjline = hfield("subj", mp);

	curind = dot == mp ? '>' : ' ';
	dispc = ' ';
	if (mp->m_flag & MSAVED)
		dispc = '*';
	if (mp->m_flag & MPRESERVE)
		dispc = 'P';
	if ((mp->m_flag & (MREAD|MNEW)) == MNEW)
		dispc = 'N';
	if ((mp->m_flag & (MREAD|MNEW)) == 0)
		dispc = 'U';
	if (mp->m_flag & MBOX)
		dispc = 'M';

	date = getdate(mp);
	mdate = ctime(&date) + 4; /* Skip week day */
	patchmonth(mdate);

	sprintf(wcount, "%ld/%ld", (long)mp->m_lines, (long)mp->m_size);
	cp = nameof(mp, 0);
	sprintf(addr, "%-20.20s%c", cp, strlen(cp) > 20 ? '>' : ' ');
	if (subjline != NOSTR)
		sprintf(pbuf, "%c%c%4d %s%.12s %-8s %.40s", curind, dispc, mesg,
		    addr, mdate, wcount, subjline);
	else
		sprintf(pbuf, "%c%c%4d %s%.12s %-8s", curind, dispc, mesg,
		    addr, mdate, wcount);
	printf("%.79s\n", pbuf);
}

/*
 * Print out the value of dot.
 */

pdot()
{
	printf("%d\n", dot - &message[0] + 1);
	return(0);
}

/*
 * Print out all the possible commands.
 */

pcmdlist()
{
	register struct cmd *cp;
	register int cc;
	extern struct cmd cmdtab[];

	printf(ediag("Commands are:\n","Список команд:\n"));
	for (cc = 0, cp = cmdtab; cp->c_name != NULL; cp++) {
		cc += strlen(cp->c_name) + 2;
		if (cc > 72) {
			printf("\n");
			cc = strlen(cp->c_name) + 2;
		}
		if ((cp+1)->c_name != NOSTR)
			printf("%s, ", cp->c_name);
		else
			printf("%s\n", cp->c_name);
	}
	return(0);
}

/*
 * Type out messages, honor ignored fields.
 */
type(msgvec)
	int *msgvec;
{

	return(type1(msgvec, SF_DOIGN));
}

/*
 * Type out messages, even printing ignored fields.
 */
Type(msgvec)
	int *msgvec;
{

	return(type1(msgvec, SF_NONE));
}

/*
 * Print out messages, honor ignored fields.
 */
lpr(msgvec)
	int *msgvec;
{

	return(type1(msgvec, SF_DOIGN|SF_LPR));
}

/*
 * Print out messages, even printing ignored fields.
 */
Lpr(msgvec)
	int *msgvec;
{

	return(type1(msgvec, SF_LPR));
}

/*
 * Type out the messages requested.
 */
#ifndef MSDOS
jmp_buf pipestop;
#endif
int     crt_lines;

type1(msgvec, sflags)
	int *msgvec;
{
	register *ip;
	register struct message *mp;
	register int mesg;
	register char *cp;
	char *pager, *p;
	long    nlines;
	int xdisc, lpr;
#ifndef MSDOS
	int brokpipe();
#endif
	FILE *obuf;

	obuf = stdout;
	lpr = (sflags & SF_LPR);
#ifndef MSDOS
	if (setjmp(pipestop)) {
		if (obuf != stdout) {
			pipef = NULL;
			pclose(obuf);
		}
		signal(SIGPIPE, SIG_DFL);
		return(0);
	}
#endif
	if (lpr || intty && outtty && (cp = value("crt")) != NOSTR) {
		xdisc = 0;
		if( sflags & SF_DOIGN ) {
			/* Unnatural intelligence */
			xdisc += isign("received")*2;
			xdisc += isign("-from");
			xdisc += isign("message-id");
			xdisc += isign("from");
			xdisc += isign("to");
			xdisc += isign("subject");
			xdisc += isign("date");
			xdisc += isign("status");
		}
		for (ip = msgvec, nlines = 0; *ip && ip-msgvec < msgCount; ip++)
			nlines += message[*ip - 1].m_lines - xdisc;
		if( !lpr && (pager = value("PAGER")) == NOSTR ) {
			obuf = stdout;
			sflags |= SF_PAGER;
		} else {
			if ( lpr || nlines > atoi(cp) ) {
				if (lpr)
					pager = value("LPR");
				if( pager == NOSTR || *pager == '\0' )
					pager = lpr ? "lpr" : MORE;
				if (   !lpr
#ifndef MSDOS
				    && value("nopipe") != NOSTR
#endif
				   ) {
#ifdef  MSDOS
					maketemp(tempBack);
#endif
					close(creat(tempBack, 0600));
					obuf = fopen(tempBack, "a");
				}
				else {
#ifndef MSDOS
					obuf = popen(pager, "w");
#else
					pager = "PRN";
					if (intty && outtty) {
						char buf[80];

						printf(ediag("Turn on printer and press Enter: ",
							     "Включите печать и нажмите Enter: "));
						flush();
						(void) gets(buf);
					}
					obuf = fopen(pager, "w");
#endif
				}
				if (obuf == NULL) {
					if (   !lpr
#ifndef MSDOS
					    && value("nopipe") != NOSTR
#endif
					   ) {
						perror(tempBack);
						remove(tempBack);
					}
					else
						perror(pager);
					if (!lpr) {
						sflags |= SF_PAGER;
						obuf = stdout;
					}
					else {
						printf(ediag("Can't print message\n",
							     "Нельзя напечатать сообщение\n"));
						return(0);
					}
				}
#ifndef MSDOS
				else if (lpr || value("nopipe") == NOSTR) {
					pipef = obuf;
					signal(SIGPIPE, brokpipe);
				}
#endif
			}
		}
		if( !lpr && (sflags & SF_PAGER) ) {
			crt_lines = atoi(cp);
			if( crt_lines <= 1 )
#ifdef  MSDOS
				crt_lines = 25;
#else
				crt_lines = 24;
#endif
		}
	}
Do_again:
	for (ip = msgvec; *ip && ip-msgvec < msgCount; ip++) {
		mesg = *ip;
		touch(mesg);
		mp = &message[mesg-1];
		dot = mp;
		print(mp, obuf, sflags);
	}
	if (obuf != stdout) {
		int s, pid;

#ifndef MSDOS
		if (lpr || value("nopipe") == NOSTR) {
			pipef = NULL;
			pclose(obuf);
			signal(SIGPIPE, SIG_DFL);
		}
		else
#endif
		{
			int (*sigint)(), (*sigquit)();

			fclose(obuf);
			sigint = signal(SIGINT, SIG_IGN);
			sigquit = signal(SIGQUIT, SIG_IGN);
#ifndef MSDOS
			pid = vfork();
			if (pid == -1) {
				perror("fork");
#else
			pid = spawnlp (P_WAIT, pager, "mail-pager", tempBack, NULL);
			if (pid < 0) {
				perror(pager);
#endif
				signal(SIGINT, sigint);
				signal(SIGQUIT, sigquit);
		Perr:
				remove(tempBack);
				sflags |= SF_PAGER;
				obuf = stdout;
				goto Do_again;
			}
#ifndef MSDOS
			if (pid == 0) {
				if (sigint != SIG_IGN)
					sigsys(SIGINT, SIG_DFL);
				if (sigquit != SIG_IGN)
					sigsys(SIGQUIT, SIG_DFL);
				execlp(pager, "mail-pager", tempBack, NULL);
				perror(pager);
				_exit(1);
			}
			while (wait(&s) != pid)
				;
#endif
			signal(SIGINT, sigint);
			signal(SIGQUIT, sigquit);
#ifndef MSDOS
			if ((s & 0377) != 0)
#else
			if (pid != 0)
#endif
				goto Perr;
			remove(tempBack);
		}
	}
	return(0);
}

#ifndef MSDOS
/*
 * Respond to a broken pipe signal --
 * probably caused by using quitting more.
 */

brokpipe()
{
	signal(SIGPIPE, brokpipe);
	longjmp(pipestop, 1);
}
#endif

/*
 * Print the indicated message on standard output.
 */

print(mp, obuf, sflags)
	register struct message *mp;
	FILE *obuf;
{

	if ((sflags & SF_LPR) || value("quiet") == NOSTR)
		fprintf(obuf, ediag(
"Message %2d:\n",
"Сообщение %2d:\n"),
 mp - &message[0] + 1);
	touch(mp - &message[0] + 1);
	(void) send(mp, obuf, sflags);
}

/*
 * Print the top so many lines of each desired message.
 * The number of lines is taken from the variable "toplines"
 * and defaults to 5.
 */

top(msgvec)
	int *msgvec;
{
	register int *ip;
	register struct message *mp;
	register int mesg;
	int topl, lineb;
	long c, lines;
	char *valtop, linebuf[LINESIZE];
	FILE *ibuf;

	topl = 5;
	valtop = value("toplines");
	if (valtop != NOSTR) {
		topl = atoi(valtop);
		if (topl < 0 || topl > 10000)
			topl = 5;
	}
	lineb = 1;
	for (ip = msgvec; *ip && ip-msgvec < msgCount; ip++) {
		mesg = *ip;
		touch(mesg);
		mp = &message[mesg-1];
		dot = mp;
		if (value("quiet") == NOSTR)
			printf(ediag("Message %2d:\n", "Сообщение %2d:\n"), mesg);
		ibuf = setinput(mp);
		c = mp->m_lines;
		if (!lineb)
			putchar('\n');
		for (lines = 0; lines < c && lines <= topl; lines++) {
			if (readline(ibuf, linebuf) <= 0)
				break;
			puts(linebuf);
			lineb = blankline(linebuf);
		}
	}
	return(0);
}

/*
 * Touch all the given messages so that they will
 * get mboxed.
 */

stouch(msgvec)
	int msgvec[];
{
	register int *ip;

	for (ip = msgvec; *ip != 0; ip++) {
		dot = &message[*ip-1];
		dot->m_flag |= MTOUCH;
		dot->m_flag &= ~MPRESERVE;
	}
	return(0);
}

/*
 * Make sure all passed messages get mboxed.
 */

mboxit(msgvec)
	int msgvec[];
{
	register int *ip;

	for (ip = msgvec; *ip != 0; ip++) {
		dot = &message[*ip-1];
		dot->m_flag |= MTOUCH|MBOX;
		dot->m_flag &= ~MPRESERVE;
	}
	return(0);
}

/*
 * List the folders the user currently has.
 */
folders()
{
	char dirname[BUFSIZ], cmd[BUFSIZ];
	int pid, s, e;
#ifdef  MSDOS
	int (*sig[2])();
	char *Shell;
#endif

	if (getfold(dirname) < 0) {
		printf(ediag(
"No value set for \"folder\"\n",
"Не задано значение \"folder\"\n"));
		return(-1);
	}
#ifndef MSDOS
	switch ((pid = fork())) {
	case 0:
		execlp("ls", "ls", "-sx", dirname, NULL);
		clrbuf(stdout);
		exit(1);

	case -1:
		perror("fork");
		return(-1);

	default:
		while ((e = wait(&s)) != -1 && e != pid)
			;
	}
#else   /* MSDOS */
	strcpy(cmd, "DIR/W ");
	strcat(cmd, dirname);
	if ((Shell = value("SHELL")) == NOSTR)
		if ((Shell = value("COMSPEC")) == NOSTR)
			Shell = SHELL;
	sig[0] = signal(SIGINT, SIG_IGN);
	sig[1] = signal(SIGQUIT, SIG_IGN);
	s = spawnlp(P_WAIT, Shell, "mail-shell", "/C", cmd, NULL);
	if (s < 0)
		perror(Shell);
	signal(SIGINT, sig[0]);
	signal(SIGQUIT, sig[1]);
#endif  /* MSDOS */
	return(0);
}

#ifdef PMCS
/*
 * List all of projects in projspool
 */
projects()
{
	int pid, s, e;
	char  *pspool;

	if( (pspool = value("projspool")) == NOSTR )
		pspool = PROJSPOOL;
	switch ((pid = fork())) {
	case 0:
		execlp("pmcs", "pmcs", "-ls", pspool, 0);
		clrbuf(stdout);
		exit(1);

	case -1:
		perror("fork");
		return(-1);

	default:
		while ((e = wait(&s)) != -1 && e != pid)
			;
	}
	return(0);
}
#endif
