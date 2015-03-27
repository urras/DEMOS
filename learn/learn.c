#include <stdio.h>
#include "lrndef"
#include "lrnref"
#include <signal.h>
main(argc,argv)
register int argc;
register char *argv[];
{
	extern hangup(), intrpt();
	extern char * getlogin();
	char *malloc();
	speed = 0;
	more = 1;
	pwline = getlogin();
	fdi=fileno(stdin);
	ioctl(fdi, TIOCGETP, &term);
	ioctl(fdi, TIOCGETC, &ctl);
	system("stty dec");
	setbuf(stdout, malloc(BUFSIZ));
	selsub(argc, argv);
	signal(SIGHUP, hangup);
	signal(SIGINT, intrpt);
	while (more) {
		selunit();
		dounit();
		whatnow();
	}
	wrapup(0);
}
hangup()
{
	wrapup(1);
}
intrpt()
{
	extern term, ctl;
	char response[20];
	signal(SIGINT, hangup);
	if (flag) fprintf(stderr,"\nПрерывание.\nХотите продолжить?\nОтветьте да\\нет.\n$");
	else fprintf(stderr, "\nInterrupt.\nWant to go on? \n Answer yes\\no.\n$");
	fflush(stderr);
	gets(response);
	if((response[0] == 0)|| (response[0] =='n')||(response[0] == 'н'))
		wrapup(1);
	else signal(SIGINT, intrpt);
	return;
}

