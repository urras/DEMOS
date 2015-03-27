/* Copyright (c) 1979 Regents of the University of California */
/*
 * clock - continually print the time of day
 */
#include        <sys/types.h>
#include        <stdio.h>
#include	<setjmp.h>
#include        <signal.h>
#include        <sys/ioctl.h>
jmp_buf	env;

main()
{
	register char *tp;
	time_t tim;
	int stop(), pgrp;

	if (fork())
		exit(0);
	ioctl (0, TIOCGPGRP, &pgrp);
	setpgrp (0, pgrp);
	signal(SIGINT, SIG_IGN);
	setjmp(env);
	signal(SIGQUIT, stop);
	time(&tim);
	tp = ctime(&tim)+11;
	printf("\n\t%8.8s\t", tp);
	while (1) {
		time(&tim);
		tp = ctime(&tim)+11;
		printf("%8.8s\b\b\b\b\b\b\b\b",tp);
		fflush(stdout);
		sleep(1);
	}
}


stop()
{
	int start();

	signal(SIGQUIT, start);
	putchar('\n');
	while (1)
		pause();
}

start()
{
	longjmp(env, 1);
}
