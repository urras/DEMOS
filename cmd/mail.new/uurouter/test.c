#include <stdio.h>

int debug = 1;

main()
{
	char nm[200], res[200];

	for(;;) {
		printf( "ADR? " ); fflush(stdout);
		gets(nm);
		switch(findroute(nm, res)) {
		    case 0:     printf( "---- NOT FOUND ---\n");
				break;
		    case 1:     printf( "---- PATH: %s ---\n", res);
				break;
		    case 2:     printf( "---- AMB: %s ---\n", res);
				break;
		}
	}
}

errmsg(f, a, b, c, d, e)
{
	fprintf(stderr, f, a, b, c, d, e);
}

/*
 * Get a current host name for replies
 */
gethost(buf, len)
	char buf[];
{
	int     fd[2];
	int     l, rlen;
	static char hn[80] = "";
	int     istream;
	char *p, *strchr();

	if( hn[0] != '\0' ) {
		strncpy(buf, hn, len);
		return;
	}

	pipe(fd);
	if( fork() == 0 ) {
		/* New proc */
		close(fd[0]);
		close(0);
		close(1);
		close(2);
		open("/dev/null", 0);
		dup(fd[1]);
		dup(fd[1]);
		close(fd[1]);

		execl("/usr/bin/hostname", "hostname", 0);
		execl("/bin/hostname", "hostname", 0);
		execl("/etc/hostname", "hostname", 0);
		exit(1);
	}
	close(fd[1]);
	rlen = read(fd[0], hn, 79);
	close(fd[0]);
	wait(&l);
	if( l != 0 || rlen <= 0 ) {     /* Error */
		istream = open("/etc/myhostname", 0);
		if( istream < 0 ) {    /* OK, use UUCP hostname */
			strcpy(hn, "uuname");
			strcat(hn, ".UUCP");
		} else {
			(void) read(istream, hn, 79);
			close( istream );
		}
	}
	if( (p = strchr(hn, '\n')) != NULL )
		*p = '\0';
	/* Ugh! */
	strncpy(buf, hn, len);
}
