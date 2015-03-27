#include <stdio.h>
#include "lrnref"
#include <signal.h>

int istop;

list(r)
register char *r;
{
	int stop(), intrpt();
register FILE *ft;
register char s[100];

	if (r==0)
		return;
	istop = 1;
	signal(SIGINT, stop);
	ft = fopen(r, "r");
	if (ft != NULL) {
		while (fgets(s, 100, ft) && istop)
			fputs(s, stdout);
		fclose(ft);
	}
	signal(SIGINT, intrpt);
}

stop()
{
	istop=0;
}
