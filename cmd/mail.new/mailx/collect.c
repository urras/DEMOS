/*
 * Mail -- a mail program
 *
 * Collect input from standard input, handling
 * ~ escapes.
 *
 * $Log: collect.c,v $
 * Revision 1.30  1991/01/25  18:50:28  ache
 * Запрос Cc: по askcc переставлен из send.c в collect.c
 *
 * Revision 1.29  1991/01/25  18:04:45  ache
 * Убраны старые (4.1) сигналы
 *
 * Revision 1.28  1991/01/19  15:38:23  ache
 * убраны буфера 16к, как не оправдавшие доверия народа
 *
 * Revision 1.27  90/12/23  21:52:20  ache
 * Буферизация IO по 16 K
 * 
 * Revision 1.26  90/12/08  20:53:20  ache
 * Добавлено Abort, ... or Send?
 * 
 * Revision 1.25  90/12/07  07:14:08  ache
 * Переделана обработка временных файлов и мелочи
 * 
 * Revision 1.24  90/10/25  01:14:09  ache
 * setjmp переставлен выше.
 * 
 * Revision 1.23  90/10/16  09:12:56  ache
 * Чуть ускорена обработка >From
 * 
 * Revision 1.22  90/10/13  22:53:36  ache
 * ~> escape corrected.
 * 
 * Revision 1.21  90/10/13  20:23:44  ache
 * Deleting 'From '.
 * 
 * Revision 1.20  90/10/09  01:13:03  ache
 * Добавлена подсказка.
 * 
 * Revision 1.19  90/09/26  22:16:23  ache
 * Signature added like in INEWS
 * 
 * Revision 1.18  90/09/21  21:58:16  ache
 * MS-DOS extends + some new stuff
 * 
 * Revision 1.17  90/09/13  13:18:41  ache
 * MS-DOS & Unix together...
 * 
 * Revision 1.16  90/09/08  13:39:13  avg
 * Добавлена команда ~>.
 * 
 * Revision 1.15  90/08/17  18:22:36  avg
 * Добавлено удаление .b - файлов после редактирования.
 * 
 * Revision 1.14  90/07/15  20:14:23  avg
 * При обработке remote from теперь @ в локальной части имени
 * будет преобразовываться в %.
 * 
 * Revision 1.13  90/06/30  23:07:45  avg
 * Помимо всего прочего, правлена ф-я transmit на предмет
 * добавления > перед From (никогда не помешает).
 * 
 * Revision 1.12  90/06/30  22:45:43  avg
 * По настоятельным просьбам Коли Сауха сделано ~M как синоним ~f.
 * 
 * Revision 1.11  90/05/31  22:17:45  avg
 * Добавлена обработка CTRL/C при вводе полей заголовка.
 * 
 * Revision 1.10  90/05/31  19:46:39  avg
 * Правлен набор выдаваемых полей заголовков.
 * 
 * Revision 1.9  90/04/30  19:44:12  avg
 * Починен режим rmail
 * 
 * Revision 1.8  90/04/20  19:16:39  avg
 * Прикручено под System V
 * 
 * Revision 1.7  88/08/23  16:37:18  avg
 * Сделан двуязычный help и правлены русские диагностики.
 * 
 * Revision 1.6  88/07/23  22:11:23  ache
 * Ошибка ф формате
 *
 * Revision 1.5  88/07/23  20:31:45  ache
 * Русские диагностики
 *
 * Revision 1.4  88/07/04  21:23:33  ache
 * Перед ~q (collrub) поставлен sighold
 *
 * Revision 1.3  88/07/04  21:02:12  ache
 * Обработка SIGQUIT и снятие/восстановление hold перед
 * grabh если не TIOCSTI
 *
 * Revision 1.2  88/01/11  12:31:34  avg
 * Теперь tilde escapes не работают только тогда, когда ввод происходит не
 * с терминала (а не при наличии ключа -r, как было).
 *
 * Revision 1.1  87/12/25  15:58:47  avg
 * Initial revision
 *
 */

/*NOXSTR*/
static char rcsid[] = "$Header: /usr/src/relcom/mailx/RCS/collect.c,v 1.30 1991/01/25 18:50:28 ache Exp $";
/*YESXSTR*/

#include "rcv.h"
#include <sys/stat.h>
#ifdef  MSDOS
#include        <process.h>
#endif

#ifdef TIOCSETA /* Demos2 Only */
#undef TIOCSTI
#endif

/*
 * Read a message from standard output and return a read file to it
 * or NULL on error.
 */

/*
 * The following hokiness with global variables is so that on
 * receipt of an interrupt signal, the partial message can be salted
 * away on dead.letter.  The output file must be available to flush,
 * and the input to read.  Several open files could be saved all through
 * Mail if stdio allowed simultaneous read/write access.
 */

static  int     (*savesig)();           /* Previous SIGINT value */
#ifndef MSDOS
static  int     (*savehup)();           /* Previous SIGHUP value */
#endif
static  int     (*savequit)();          /* Previous SIGQUIT value */
# ifdef VMUNIX
static  int     (*savecont)();          /* Previous SIGCONT value */
# endif
static  int     hf;                     /* Ignore interrups */
	int     hadintr;                /* Have seen one SIGINT so far */

static  jmp_buf coljmp;                 /* To get back to work */
extern  char tmp[], helpt[], rhelpt[];
static  FILE *buf;

FILE *
collect(hp)
	struct header *hp;
{
	FILE *fbuf;
	long lc, cc;
	int escape, collrub(), intack(), eof;
	register int c, t;
	char linebuf[LINESIZE], *cp, *p1;
	extern char tempMail[];
	int notify();
#ifndef MSDOS
	extern collintsig(), collhupsig(), collquitsig();
#else
	extern collintsig(), collquitsig();
#endif
# ifdef VMUNIX
	int collcont ();
# endif

	if( debug && (rmail || rflag != NOSTR) )
		printf("collect: rmail=%d rflag=%s\n", rmail, (rflag==NOSTR)?"(null)":rflag);
	noreset++;
	buf = NULL;
	if (value("ignore") != NOSTR)
		hf = 1;
	else
		hf = 0;
	hadintr = 0;
	savesig = signal(SIGINT, SIG_IGN);
#ifndef MSDOS
	savehup = signal(SIGHUP, SIG_IGN);
#endif
	savequit = signal(SIGQUIT, SIG_IGN);
# ifdef VMUNIX
	savecont = sigset(SIGCONT, collcont);
# endif
#ifdef  MSDOS
	maketemp(tempMail);
#endif
	if ((t = creat(tempMail, 0600)) < 0) {
		perror(tempMail);
		goto err;
	}
	close(t);
	if ((buf = TmpOpen(tempMail, "a+")) == NULL) {
		perror(tempMail);
		remove(tempMail);
		goto err;
	}

	/*
	 * If we are going to prompt for a subject,
	 * refrain from printing a newline after
	 * the headers (since some people mind).
	 */

	t = GTO|GSUBJECT|GCC|GNL;
	c = 0;
	if (intty && !rmail &&
	    hp->h_subject == NOSTR && value("ask"))
		t &= ~GNL, c++;
	if (hp->h_seq != 0) {
		puthead(hp, stdout, t);
		flush();
	}
	escape = ESCAPE;
	if ((cp = value("escape")) != NOSTR)
		escape = *cp;
	t = 1;
	if (setjmp(coljmp))     /* Должен быть здесь для выхода из grabh */
		c = t = 0;
	if (c) {
#ifndef TIOCSTI
		if (savesig != SIG_IGN)
			signal(SIGINT, hf ? intack : collintsig);
#ifndef MSDOS
		if (savehup != SIG_IGN)
			signal(SIGHUP, collhupsig);
#endif
		if (savequit != SIG_IGN)
			signal(SIGQUIT, collquitsig);
#endif
		grabh(hp, GSUBJECT);
#ifndef TIOCSTI
		signal(SIGINT, SIG_IGN);
#ifndef MSDOS
		signal(SIGHUP, SIG_IGN);
#endif
		signal(SIGQUIT, SIG_IGN);
# ifdef VMUNIX
		sigset(SIGCONT, collcont);
# endif
#endif
	}
	if (t && intty && outtty && value("quiet") == NOSTR) {
		printf(ediag("'%c?' -- help, ",
			     "'%c?' -- подсказка, "), escape);
		printf(ediag("%s at the begining of a line -- complete message\n",
			     "%s в начале строки -- завершение сообщения\n"),
		       value("dot") != NOSTR ? "'.'" :
#ifdef  MSDOS
				"Ctrl+Z<Enter>"
#else
				"CTRL/D"
#endif
		);
		flush();
	}
	eof = 0;
AgainLabel:
	for (;;) {
		if (savesig != SIG_IGN)
			signal(SIGINT, hf ? intack : collintsig);
#ifndef MSDOS
		if (savehup != SIG_IGN)
			signal(SIGHUP, collhupsig);
#endif
		if (savequit != SIG_IGN)
			signal(SIGQUIT, collquitsig);
		flush();
		if (readline(stdin, linebuf) <= 0) {
			if (intty && value("ignoreeof") != NOSTR) {
				if (++eof > 35)
					break;
				printf(ediag(
"Use \".\" to terminate letter\n",
"Используйте \".\" чтобы окончить письмо\n"));
				continue;
			}
			break;
		}
		eof = 0;
		hadintr = 0;
		if (intty && equal(".", linebuf) &&
		    (value("dot") != NOSTR || value("ignoreeof") != NOSTR))
			break;

		/*
		 * Process old ugly From ... remote from ...
		 * uucp rmail header line.
		 */
		if( rmail && rflag == NOSTR && *linebuf == 'F' &&
		    strncmp(linebuf, "From ", 5) == 0) {
			register char *p, *q;
			static char remotename[100];

			p = &linebuf[5];
			q = &remotename[80];
			while( *p != '\n' && *p != ' ' && *p != '\0' )
				*q++ = *p++;
			*q = '\0';
			q = &remotename[80];
			while( *p != '\0' && *p != '\n' ) {
				if( strncmp(p++, "remote from ", 12) )
					continue;
				while( *p ) p++;
				while( *--p == ' ' || *p == '\n' ) ;
				/* add an uucp part separated by ! */
				*--q = '!';
				while( *p != ' ' )
					*--q = *p--;
				p = &remotename[80];
				/* if there was an Internet address,
				   change all '@'s to '%'s (sigh) */
				while( *p ) {
					if( *p == '@' )
						*p = '%';
					p++;
				}
				break;
			}
			rflag = q;
		}

		if (linebuf[0] != escape || !intty || rflag != NOSTR) {
			if (   fseek(buf, 0L, 2) != 0
				|| (t = putline(buf, linebuf)) < 0) {
				ioerror(tmp, 1);
				goto err;
			}
			continue;
		}
		c = linebuf[1];
		switch (c) {
		default:
			/*
			 * On double escape, just send the single one.
			 * Otherwise, it's an error.
			 */

			if (c == escape) {
				if (   fseek(buf, 0L, 2) != 0
					|| putline(buf, &linebuf[1]) < 0) {
					ioerror(tmp, 1);
					goto err;
				}
				else
					break;
			}
			printf(ediag(
"Unknown tilde escape.\n",
"Неизвестная управляющая последовательность.\n"));
			break;

		case 'C':
			/*
			 * Dump core.
			 */

			core();
			break;

		case '!':
			/*
			 * Shell escape, send the balance of the
			 * line to sh -c.
			 */

			shell(&linebuf[2]);
			break;

		case ':':
		case '_':
			/*
			 * Escape to command mode, but be nice!
			 */
			execute(&linebuf[2], 1);
			printf(ediag("(continue)\n","(продолжайте)\n"));
			break;

		case '.':
			/*
			 * Simulate end of file on input.
			 */
			goto eofl;

		case 'q':
		case 'Q':
AbortLabel:
			/*
			 * Force a quit of sending mail.
			 * Act like an interrupt happened.
			 */

			hadintr++;
			signal(SIGINT, SIG_IGN);
			collrub(SIGINT);
			exit(1);

		case 'h':
			/*
			 * Grab a bunch of headers.
			 */
			if (!outtty) {
				printf(ediag(
"~h: no can do!?\n",
"~h: нельзя сделать!\n"));
				break;
			}
			grabh(hp, GTO|GSUBJECT|GCC|GBCC);
			printf(ediag("(continue)\n","(продолжайте)\n"));
			break;

		case 't':
			/*
			 * Add to the To list.
			 */

			hp->h_to = addto(hp->h_to, &linebuf[2]);
			hp->h_seq++;
			break;

		case 's':
			/*
			 * Set the Subject list.
			 */

			cp = &linebuf[2];
			while (any(*cp, " \t"))
				cp++;
			hp->h_subject = savestr(cp);
			hp->h_seq++;
			break;

		case 'c':
			/*
			 * Add to the CC list.
			 */

			hp->h_cc = addto(hp->h_cc, &linebuf[2]);
			hp->h_seq++;
			break;

		case 'b':
			/*
			 * Add stuff to blind carbon copies list.
			 */
			hp->h_bcc = addto(hp->h_bcc, &linebuf[2]);
			hp->h_seq++;
			break;

		case 'd':
			copy(deadletter, &linebuf[2]);
			/* fall into . . . */

		case 'r':
			/*
			 * Invoke a file:
			 * Search for the file name,
			 * then open it and copy the contents to obuf.
			 */

			cp = &linebuf[2];
			while (any(*cp, " \t"))
				cp++;
			if (*cp == '\0') {
				printf(ediag(
"Interpolate what file?\n",
"Какой файл вставлять?\n"));
				break;
			}
			cp = expand(cp);
			if (cp == NOSTR)
				break;
			if (isdir(cp)) {
				printf(ediag(
"%s: directory\n",
"%s: каталог\n"), cp);
				break;
			}
			if ((fbuf = fopen(cp, "r")) == NULL) {
				perror(cp);
				break;
			}
			printf("\"%s\" ", cp);
			flush();
			lc = 0;
			cc = 0;
			if (fseek(buf, 0L, 2) != 0) {
				ioerror(tmp, 2);
				fclose(fbuf);
				goto err;
			}
			while (readline(fbuf, linebuf) > 0) {
				lc++;
				if ((t = putline(buf, linebuf)) < 0) {
					ioerror(tmp, 1);
					fclose(fbuf);
					goto err;
				}
				cc += t;
			}
			fclose(fbuf);
			printf("%ld/%ld\n", lc, cc);
			break;

		case 'u':
			/*
			 * UUENCODE	a file:
			 * Search for the file name,
			 * then open it and copy the contents to obuf.
			 */

			cp = &linebuf[2];
			while (any(*cp, " \t"))
				cp++;
			if (*cp == '\0') {
				printf(ediag(
"Uuencode what file?\n",
"Какой файл для uuencode?\n"));
				break;
			}
			for(p1 = cp; *p1 && !any(*p1, " \t"); p1++)
				;
			while (*p1 && any(*p1, " \t"))
				*p1++ = '\0';
			if (*p1 == '\0')
				p1 = cp;
			cp = expand(cp);
			if (cp == NOSTR)
				break;
			if (isdir(cp)) {
				printf(ediag(
"%s: directory\n",
"%s: каталог\n"), cp);
				break;
			}
#ifndef MSDOS
			if ((fbuf = fopen(cp, "r")) == NULL) {
#else
			if ((fbuf = fopen(cp, "rb")) == NULL) {
#endif
				perror(cp);
				break;
			}
			printf("\"%s\" ", cp);
			flush();
			lc = 0;
			cc = 0;
			if (fseek(buf, 0L, 2) != 0) {
				ioerror(tmp, 2);
				fclose(fbuf);
				goto err;
			}
			if (!uuencode(p1, fbuf, buf, &lc, &cc)) {
				ioerror(tmp, 1);
				fclose(fbuf);
				goto err;
			}
			fclose(fbuf);
			printf("%ld/%ld\n", lc, cc);
			break;

		case 'w':
			/*
			 * Write the message on a file.
			 */

			cp = &linebuf[2];
			while (any(*cp, " \t"))
				cp++;
			if (*cp == '\0') {
				fprintf(stderr, ediag(
"Write what file!?\n",
"В какой файл писать?\n"));
				break;
			}
			if ((cp = expand(cp)) == NOSTR)
				break;
			rewind(buf);
			exwrite(cp, buf, 1);
			break;

		case 'm':
		case 'M':
		case 'f':
		case '>':
			/*
			 * Interpolate the named messages, if we
			 * are in receiving mail mode.  Does the
			 * standard list processing garbage.
			 * If ~f is given, we don't shift over.
			 */

			if (!rcvmode) {
				printf(ediag(
"No messages to send from!?!\n",
"Откуда взять сообщение?!\n"));
				break;
			}
			cp = &linebuf[2];
			while (any(*cp, " \t"))
				cp++;
			if (forward(cp, buf, c) < 0)
				goto err;
			printf(ediag("(continue)\n","(продолжайте)\n"));
			break;

		case '?':
			if ((fbuf = fopen(ediag(helpt,rhelpt), "r")) == NULL) {
				perror(ediag(helpt,rhelpt));
				break;
			}
			while ((t = getc(fbuf)) != EOF)
				putchar(t);
			fclose(fbuf);
			break;

		case 'p':
ListLabel:
			/*
			 * Print out the current state of the
			 * message without altering anything.
			 */
			rewind(buf);
			printf(ediag(
"-------\nMessage contains:\n",
"-------\nСообщение содержит:\n"));
			puthead(hp, stdout, GTO|GSUBJECT|GCC|GBCC|GNL|GREFS|GADD);
			while ((t = getc(buf)) != EOF)
				putchar(t);
			/*
			 *  Copy out signature file
			 */

			if (   value("autosign") != NOSTR
				&& (fbuf = fopen(signature, "r")) != NULL
			   ) {
				puts("-- ");
				for (t = 0; t < 4; t++)
					if (fgets(linebuf, sizeof(linebuf), fbuf) == NOSTR)
						break;
					else {
						if (*linebuf == 'F' && strncmp("From ", linebuf, 5) == 0)
							putchar('>');
						fputs(linebuf, stdout);
					}
				fclose(fbuf);
			}
			printf(ediag("\n(continue)\n","\n(продолжайте)\n"));
			break;

		case '^':
		case '|':
			/*
			 * Pipe message through command.
			 * Collect output as new message.
			 */

			buf = mespipe(buf, &linebuf[2]);
			printf(ediag("(continue)\n","(продолжайте)\n"));
			break;
EditLabel:
			c = 'v';
		case 'v':
		case 'e':
			/*
			 * Edit the current message.
			 * 'e' means to use EDITOR
			 * 'v' means to use VISUAL
			 */

			if ((buf = mesedit(buf, c)) == NULL)
				goto err;
			printf(ediag("(continue)\n","(продолжайте)\n"));
			break;
		}
	}
eofl:
	if (intty && outtty && !rmail) {
		if (hp->h_cc == NOSTR && value("askcc") != NOSTR)
			grabh(hp, GCC);
		if (value("quiet") == NOSTR) {
			char buf[80];
Ask:
			printf(ediag(
"Abort, Continue, Edit, List or Send? ",
"Отменить(a), продолжить(c), редактировать(e), просмотреть(l) или послать(s)? "));
			flush();
			gets(buf);
			*buf = tolower(*buf);
			switch (*buf) {
			case 'a':
				goto AbortLabel;
			case 'c':
				printf(ediag("(continue)\n","(продолжайте)\n"));
				goto AgainLabel;
			case 'e':
				goto EditLabel;
			case 'l':
				goto ListLabel;
			case 's':
			case '\0':
				break;
			default:
				goto Ask;
			}
		}
	}
	rewind(buf);
	signal(SIGINT, savesig);
#ifndef MSDOS
	signal(SIGHUP, savehup);
#endif
	signal(SIGQUIT, savequit);
# ifdef VMUNIX
	sigset(SIGCONT, savecont);
# endif
	noreset = 0;
	return(buf);

err:
	TmpDel(buf);
	signal(SIGINT, savesig);
#ifndef MSDOS
	signal(SIGHUP, savehup);
#endif
	signal(SIGQUIT, savequit);
# ifdef VMUNIX
	sigset(SIGCONT, savecont);
# endif
	noreset = 0;
	return(NULL);
}

/*
 * Non destructively interrogate the value of the given signal.
 */

psig(n)
{
	register (*wassig)();

	wassig = signal(n, SIG_IGN);
	signal(n, wassig);
	return((int) wassig);
}

/*
 * Write a file, ex-like if f set.
 */

exwrite(name, ibuf, f)
	char name[];
	FILE *ibuf;
{
	register FILE *of;
	long cc;
	long lc;
	char line[LINESIZE];
	struct stat junk;

	if (f) {
		printf("\"%s\" ", name);
		flush();
	}
	if (stat(name, &junk) >= 0 && (junk.st_mode & S_IFMT) == S_IFREG) {
		if (!f)
			fprintf(stderr, "%s: ", name);
		fprintf(stderr, ediag("file exists\n", "файл уже существует\n"));
		return(-1);
	}
	if ((of = fopen(name, "w")) == NULL) {
		perror(f ? NOSTR : name);
		return(-1);
	}
	lc = 0;
	cc = 0;
	while (fgets(line, sizeof(line), ibuf) != NOSTR) {
		lc++;
		cc += strlen(line);
		if (*line == 'F' && strncmp("From ", line, 5) == 0) {
			putc('>', of);
			cc++;
		}
		fputs(line, of);
		if (ferror(of)) {
	err:
			ioerror(f ? NOSTR : name, 1);
			fclose(of);
			return(-1);
		}
	}
	(void) fflush(of);
	if (ferror(of))
		goto err;
	fclose(of);
	printf("%ld/%ld\n", lc, cc);
	flush();
	return(0);
}

extern char tempEdit[];

/*
 * Edit the message being collected on ibuf and obuf.
 * Write the message out onto some poorly-named temp file
 * and point an editor at it.
 *
 * On return, make the edit file the new temp file.
 */

FILE *
mesedit(buf, c)
	FILE *buf;
{
	int pid, s;
	FILE *fbuf;
	register int t;
	int (*sig)(), (*scont)(), signull(), (*qsig)();
	struct stat sbuf;
	register char *edit;
	char tmpback[100];      /* Editor's .bak file name */

	sig = signal(SIGINT, SIG_IGN);
	qsig = signal(SIGQUIT, SIG_IGN);
# ifdef VMUNIX
	scont = sigset(SIGCONT, signull);
# endif
#ifdef  MSDOS
	maketemp(tempEdit);
#endif
	if (stat(tempEdit, &sbuf) >= 0) {
		printf(ediag("%s: file exists\n",
"%s: файл уже существует\n"),
tempEdit);
		goto out;
	}
	close(creat(tempEdit, 0600));
	if ((fbuf = fopen(tempEdit, "a")) == NULL) {
		perror(tempEdit);
		remove(tempEdit);
		goto out;
	}
	rewind(buf);
	while ((t = getc(buf)) != EOF)
		putc(t, fbuf);
	fflush(fbuf);
	if (ferror(fbuf)) {
		perror(tempEdit);
		remove(tempEdit);
		goto out;
	}
	fclose(fbuf);
	if ((edit = value(c == 'e' ? "EDITOR" : "VISUAL")) == NOSTR)
		edit = c == 'e' ? EDITOR : VISUAL;
#ifndef MSDOS
	pid = vfork();
	if (pid == 0) {
		if (sig != SIG_IGN)
			sigsys(SIGINT, SIG_DFL);
		if (qsig != SIG_IGN)
			sigsys(SIGQUIT, SIG_DFL);
		execlp(edit, edit, tempEdit, NULL);
		perror(edit);
		_exit(1);
	}
	if (pid == -1) {
		perror("fork");
#else
	pid = spawnlp (P_WAIT, edit, "mail-editor", tempEdit, NULL);
	if (pid < 0) {
		perror(edit);
#endif
		remove(tempEdit);
		goto out;
	}
#ifndef MSDOS
	while (wait(&s) != pid)
		;
	if ((s & 0377) != 0) {
#else
	if (pid != 0) {
#endif
		printf(ediag(
"Fatal error in \"%s\"\n",
"Фатальная ошибка в \"%s\"\n"),
edit);
		remove(tempEdit);
		goto out;
	}

	/*
	 * Now switch to new file.
	 */

	if ((fbuf = TmpOpen(tempEdit, "a+")) == NULL) {
		perror(tempEdit);
		remove(tempEdit);
		goto out;
	}
	TmpDel(buf);
	buf = fbuf;
	if (fseek(buf, 0L, 2) != 0)
		ioerror(tmp, 2);
out:
	/* remove backfile */
	strcpy(tmpback, tempEdit);
#ifndef MSDOS
	strcat(tmpback, ".b");
#else
	strcat(tmpback, ".~");
#endif
	remove(tmpback);

	strcpy(tmpback, tempEdit);
	strcat(tmpback, ".bak");
	remove(tmpback);

# ifdef VMUNIX
	sigset(SIGCONT, scont);
# endif
	signal(SIGINT, sig);
	signal(SIGQUIT, qsig);
	return(buf);
}

/*
 * Pipe the message through the command.
 * Old message is on stdin of command;
 * New message collected from stdout.
 * Sh -c must return 0 to accept the new message.
 */

FILE *
mespipe(buf, cmd)
	FILE *buf;
	char cmd[];
{
	register FILE *n;
	int pid, s;
	int (*savesig)(), (*saveqsig)();
	char *Shell;
#ifdef  MSDOS
	int oo, ii;
#endif
#ifdef  MSDOS
	maketemp(tempEdit);
#endif
	close(creat(tempEdit, 0600));
	if ((n = TmpOpen(tempEdit, "a+")) == NULL) {
		perror(tempEdit);
		remove(tempEdit);
		return(buf);
	}
	rewind(buf);
	savesig = signal(SIGINT, SIG_IGN);
	saveqsig = signal(SIGQUIT, SIG_IGN);
	if ((Shell = value("SHELL")) == NULL)
#ifdef  MSDOS
	    if ((Shell = value("COMSPEC")) == NULL)
#endif
		Shell = SHELL;
#ifndef MSDOS
	if ((pid = vfork()) == -1) {
		perror("fork");
		goto err;
	}
	if (pid == 0) {
		/*
		 * stdin = current message.
		 * stdout = new message.
		 */

		close(0);
		dup(fileno(buf));
		close(1);
		dup(fileno(n));
		for (s = 4; s < 15; s++)
			close(s);
		execlp(Shell, Shell, "-c", cmd, NULL);
		perror(Shell);
		_exit(1);
	}
	while (wait(&s) != pid)
		;
	if (s != 0 || pid == -1) {
#else   /* MSDOS */
	/*
	 * stdin = current message.
	 * stdout = new message.
	 */

	ii = dup (fileno(stdin));
	close(fileno(stdin));
	dup(fileno(buf));
	oo = dup (fileno(stdout));
	close(fileno(stdout));
	dup(fileno(n));
	s = spawnlp (P_WAIT, Shell, "mail-pipe", "/C", cmd, NULL);
	if (s < 0) {
		dup2 (ii, fileno(stdin));
		dup2 (oo, fileno(stdout));
		perror (Shell);
		goto err;
	}
	dup2 (ii, fileno(stdin));
	dup2 (oo, fileno(stdout));
	if (s > 0) {
#endif  /* MSDOS */
		fprintf(stderr, ediag(
"\"%s\" failed!?\n",
"\"%s\" завершился неудачей?\n"), cmd);
		goto err;
	}
	if (fsize(n) == 0) {
		fprintf(stderr, ediag(
"No bytes from \"%s\" !?\n",
"Ничего нет из \"%s\" !?\n"), cmd);
		goto err;
	}

	/*
	 * Take new files.
	 */
	TmpDel(buf);
	if (fseek(n, 0L, 2) != 0) {
		ioerror(tmp, 2);
		goto err;
	}

	signal(SIGINT, savesig);
	signal(SIGQUIT, saveqsig);

	return(n);

err:
	signal(SIGINT, savesig);
	signal(SIGQUIT, saveqsig);
	TmpDel(n);
	return(buf);
}

/*
 * Interpolate the named messages into the current
 * message, preceding each line with a tab.
 * Return a count of the number of characters now in
 * the message, or -1 if an error is encountered writing
 * the message temporary.  The flag argument is 'm' if we
 * should shift over and 'f' if not.
 */
forward(ms, obuf, f)
	char ms[];
	FILE *obuf;
{
	register int *msgvec, *ip;

	if ((msgvec = (int *) salloc((msgCount+1) * sizeof *msgvec)) == NULL) {
		panic("No memory\n");
		return(-1);
	}
	if (getmsglist(ms, msgvec, 0) < 0)
		return(0);
	if (*msgvec == (int)NULL) {
		*msgvec = first(0, MMNORM);
		if (*msgvec == (int)NULL) {
			printf(ediag(
"No appropriate messages\n",
"Нет подходящих сообщений\n"));
			return(0);
		}
		msgvec[1] = (int)NULL;
	}
	printf(ediag("Interpolating:","Вставка сообщени%s:"),
					  (msgvec[1]==(int)NULL)?"я":"й");
	if (fseek(obuf, 0L, 2) != 0) {
		ioerror(tmp, 2);
		return(-1);
	}
	for (ip = msgvec; *ip != (int)NULL; ip++) {
		touch(*ip);
		printf(" %d", *ip);
		if (transmit(&message[*ip-1], obuf, f) < 0) {
			ioerror(tmp, 1);
			return(-1);
		}
	}
	putchar('\n');
	return(0);
}

/*
 * Send message described by the passed pointer to the
 * passed output buffer.  Insert a tab in front of each
 * line.  Return a count of the characters sent, or -1
 * on error.
 */

long
transmit(mailp, obuf, f)
	struct message *mailp;
	FILE *obuf;
{
	register struct message *mp;
	register int ch;
	long c, n;
	int fcnt = 0;
	FILE *ibuf;
	char line[BUFSIZ];

	mp = mailp;
	ibuf = setinput(mp);
	c = mp->m_size;
	n = c;
	while( fgets(line, sizeof line, ibuf) != NOSTR ) {
		if (*line == 'F' && strncmp(line, "From ", 5) == 0) {
			if( fcnt++ )
				break;  /* Next message is started there */
			if( f != 'm' && f != '>' ) {
				putc('>', obuf);
				n++;
			}
		}
		if( f == 'm' ) {
			putc('\t', obuf);
			n++;
		} else if( f == '>' ) {
			putc('>', obuf);
			n++;
		}
		fputs(line, obuf);
		if (ferror(obuf)) {
			ioerror(tmp, 1);
			return(-1);
		}
	}
	(void) fflush(obuf);
	if (ferror(obuf)) {
		ioerror(tmp, 1);
		return(-1);
	}
	return(n);
}

#ifdef VMUNIX
/*
 * Print (continue) when continued after ^Z.
 */
collcont(s)
{

	printf(ediag("(continue)\n","(продолжайте)\n"));
	flush();
}
#endif

/*
 * On interrupt, go here to save the partial
 * message on ~/dead.letter.
 * Then restore signals and execute the normal
 * signal routine.  We only come here if signals
 * were previously set anyway.
 */
collintsig()
{
	signal(SIGINT, SIG_IGN);
	collrub(SIGINT);
}
#ifndef MSDOS
collhupsig()
{
	signal(SIGHUP, SIG_IGN);
	collrub(SIGHUP);
}
#endif
collquitsig()
{
	signal(SIGQUIT, SIG_IGN);
	collrub(SIGQUIT);
}

collrub(s)
{
	register FILE *dbuf;
	register int c;

	if (s == SIGINT && hadintr == 0) {
		hadintr++;
		clrbuf(stdout);
#ifndef MSDOS
		putchar('\n');
#endif
		printf(ediag(
"(Interrupt -- one more to kill letter)\n",
"(Прерывание -- чтобы уничтожить письмо нужно еще одно)\n"));
		flush();
		longjmp(coljmp, 1);
	}
	if (s == SIGINT && value("nosave") != NOSTR || fsize(buf) == 0)
		goto done;
	if ((c = creat(deadletter, 0600)) < 0)
		goto done;
	close(c);
	if ((dbuf = fopen(deadletter, "a")) == NULL) {
		remove(deadletter);
		goto done;
	}
	rewind(buf);
	while ((c = getc(buf)) != EOF)
		putc(c, dbuf);
	fclose(dbuf);

done:
	TmpDel(buf);
# ifdef VMUNIX
	sigset(SIGCONT, savecont);
# endif
	if (rcvmode) {
#ifndef MSDOS
		if (s == SIGHUP)
			hangup();
		else
#endif
			stop(s);
	}
	else
		exit(1);
}

/*
 * Acknowledge an interrupt signal from the tty by typing an @
 */

intack(s)
{

	puts("@");
	flush();
	clearerr(stdin);
}

/*
 * Add a string to the end of a header entry field.
 */

char *
addto(hf, news)
	char hf[], news[];
{
	register char *cp, *cp2, *linebuf;

	if (hf == NOSTR)
		hf = "";
	if (*news == '\0')
		return(hf);
	linebuf = salloc(strlen(hf) + strlen(news) + 2);
	for (cp = hf; any(*cp, " \t"); cp++)
		;
	for (cp2 = linebuf; *cp;)
		*cp2++ = *cp++;
	*cp2++ = ' ';
	for (cp = news; any(*cp, " \t"); cp++)
		;
	while (*cp != '\0')
		*cp2++ = *cp++;
	*cp2 = '\0';
	return(linebuf);
}
