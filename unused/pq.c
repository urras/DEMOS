/*
 * PQ - print the quota file affecting you most immediately
 */

#ifndef	lint
static char	*sccsid	= "@(#)pq.c	2.8	6/2/81";
#endif

#include	<whoami.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/inode.h>
#include	<sys/stat.h>
#include	<sys/qstat.h>

#define	reg	register

#define	isquot(i)	((i.qs_mode & S_IFMT) == S_IFQUOT)

struct qstat	Sbuf, Zbuf;

main(argc, argv)
reg int		argc;
char		**argv;
{
	reg int 	i;
	reg char	*sp;
	int		flag = 0;
	int		lastnode;
	char		buf[80];

	if (argc > 1 && argv[1][0] == '-') {
		flag++;
		argv++;
		argc--;
		}

	if (argc==1) {
		lastnode=0;
		if (qstat(".",&Sbuf) == -1) {
			perror("qstat");
			exit(1);
		}
		while (lastnode != Sbuf.qs_number) {
			lastnode = Sbuf.qs_number;
			(void) qstat(".q",&Sbuf);
			if (isquot(Sbuf)) {
				PrintQuota(&Sbuf);
				exit(0);
			}
			chdir("..");
			if (qstat(".",&Sbuf) == -1) {
				perror("qstat");
				exit(1);
			}
		}
		exit(9);
	}
	for (i = 1; i < argc; ++i) {
		strcpy(buf, argv[i]);
		if ((sp = rindex(buf, '.')) == NULL || strcmp(sp, ".q") != 0)
			strcat(buf, "/.q");
		Sbuf = Zbuf;			/* clean out this turkey */
		if (flag || argc > 2)  printf("%-18s    ",argv[i]); 
		qstat(buf,&Sbuf);
		if (isquot(Sbuf))
			PrintQuota(&Sbuf);
		else
			printf("no quota\n");
	}
}

PrintQuota()
{
	float	cut, a, b;

	a = Sbuf.qs_un.qs_qused;
	b = Sbuf.qs_un.qs_qmax;
	printf("%ld/%ld", Sbuf.qs_un.qs_qused, Sbuf.qs_un.qs_qmax);
	if (b > 0.) {
		cut = 100.0 * a / b;
		printf(" (%1.0f%%)", cut);
		}
	putchar('\n');
}
