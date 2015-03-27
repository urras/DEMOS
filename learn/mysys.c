#include <stdio.h>
#include <signal.h>
#include "lrnref"
#define	EASY	1
#define	MEDIUM	2
#define	HARD	3

mysys(s)
register char *s;
{
	/* аналог system(s) - стандартной,
	 * но некоторые команды
	 * выполняет сразу через
	 * exec, a не через shell
	 */
	char p[300];
	char *np[40];
	register char *t;
	int nv, type, stat;

	type = EASY;
	for (t = s; *t && type != HARD; t++) {
		switch (*t) {
		case '*': 
		case '[': 
		case '?': 
		case '>': 
		case '<': 
		case '$':
		case '\'':
		case '"':
			type = MEDIUM;
			break;
		case '|': 
		case ';': 
		case '&':
			type = HARD;
			break;
		}
	}
	switch (type) {
	case HARD:
		return(system(s));
	case MEDIUM:
		strcpy(p, "exec ");
		strcat(p, s);
		return(system(p));
	case EASY:
		strcpy(p,s);
		nv = getargs(p, np);
		t=np[0];
		if ((strcmp(t, "mv") == 0)||
		    (strcmp(t, "cp") == 0)||
		    (strcmp(t, "rm") == 0)||
		    (strcmp(t, "ls") == 0) ) {
			if (fork() == 0) {
				char b[100];
				signal(SIGINT, SIG_DFL);
				strcpy(b, "/bin/");
				strcat(b, t);
				np[nv] = 0;
				execv(b, np); 
				if (flag) fprintf(stderr,"ошибка при выполнении execv\n");
				else    fprintf(stderr, "Execv failed\n");
				exit(1);
			}
			wait(&stat);
			return(stat);
		}
		return(system(s));
	}
}

/*
 * system():
 *      аналог библиотечной версии,
 *      но устанавливает в порожденном процессе
 *      обработку сигналов по умолчанию.
 */

system(s)
register char *s;
{
	int status, pid, w;
	register int (*istat)(), (*qstat)();

	istat = signal(SIGINT, SIG_IGN);
	qstat = signal(SIGQUIT, SIG_IGN);
	if ((pid = fork()) == 0) {
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execl("/bin/sh", "sh", "-c", s, 0);
		_exit(127);
	}
	while ((w = wait(&status)) != pid && w != -1)
		;
	if (w == -1)
		status = -1;
	signal(SIGINT, istat);
	signal(SIGQUIT, qstat);
	return(status);
}

getargs(s, v)
register char *s, **v;
{
register int i;

	i = 0;
	for (;;) {
		v[i++]=s;
		while (*s != 0 && *s!=' '&& *s != '\t')
			s++;
		if (*s == 0)
			break;
		*s++ =0;
		while (*s == ' ' || *s == '\t')
			s++;
		if (*s == 0)
			break;
	}
	return(i);
}
