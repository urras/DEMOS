#include <sgtty.h>
#include <signal.h>
#include <stdio.h>
#include "lrnref"
wrapup(n)
register int n;
{
	/* в программе не используется
		 * вызов system(s), т.к. НЕОБХОДИМО
		 * чтобы прерывания были
		 * ЗАБЛОКИРОВАНЫ.
		 */
	register int pid;
	extern term, ctl;
	ioctl(fdi, TIOCSETP, &term);
	ioctl(fdi, TIOCSETC, &ctl);
	signal(SIGINT, SIG_IGN);
	chdir("..");
	if ( (pid=fork()) ==0) {
		signal(SIGHUP, SIG_IGN);
		execl("/bin/rm", "rm", "-r", dir, 0);
		execl("/usr/bin/rm", "rm", "-r", dir, 0);
		if (flag) fprintf (stderr,"Не можем найти команду 'rm'.\n");
		else     fprintf(stderr, "Can't find 'rm' command.\n");
		exit(0);
	}
	if (flag) printf("Пока.\n");
	else   printf("Bye.\n");
	fflush(stdout);
	exit(n);
}
