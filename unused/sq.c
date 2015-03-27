#define		BMAX	100
#include	<stdio.h>
#include	<ctype.h>

FILE	*ifp, *popen();
char	*quota, diskuse[BMAX];
char	*usage = "Usage:  %s d1 ... dn quota\n";

main(argc,argv)
int	argc;
char	**argv;
{
	char cmdbuf[BUFSIZ];
	register char	*s;

	if (!atoi(quota = argv[argc-1]) || (argc < 3)) {
		fprintf(stderr, usage, *argv);
		exit(1);
	}
	while (argc-- > 2) {
		sprintf(cmdbuf, "du -s %s", *++argv);
		if ((ifp = popen(cmdbuf, "r")) == NULL) {
			perror(popen);
			exit(1);
		}
		else
			while (fgets(diskuse, BMAX, ifp) != NULL)
				;
		pclose(ifp);
		for (s = diskuse; isdigit(*s); s++)
			;
		*s = '\0';
		sprintf(cmdbuf, "setquota %s %s %s", *argv, diskuse, quota);
		system(cmdbuf);
		sprintf(cmdbuf, "pq - %s", *argv);
		system(cmdbuf);
	}
	exit(0);
}
