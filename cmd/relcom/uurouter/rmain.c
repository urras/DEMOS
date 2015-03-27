/*
 * Routes collection and updating
 *
 * Usage:
 *      rsort -r from_address < message         Receive routing message
 *      rsort -u                                Update remote routes and
 *                                              local routes table
 *      rsort -q                                Query remote systems for
 *                                              sending their routing tables
 *
 * Options:
 *      -D      turn on debugging messages
 *      -d dir  use dir as a working directory (default is /usr/spool/uurouter)
 *      -n      rebuild route file, but do not send messages to neigbor systems
 *      -w      do not collect routes and write routing file
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include "def.h"

char    *wdir = "/usr/spool/uurouter";
char    filename[200];
char    my_name[70] = "uucp@";

int dontsend  = 0;
int dontwrite = 0;
int debug     = 0;
int update    = 0;
int query     = 0;
char *uassumed = NULL;  /* For QUERY ctl message processing */

FILE *pexec();

main(ac, av)
	char **av;
{
	char *a0;
	char *remotefrom = NULL;
	int   i;
	struct stat st;
	FILE *f, *f2;
	struct SYSTEM *sy;
	register c, c2;

	if( (a0 = rindex(av[0], '/')) != NULL )
		a0++;
	else
		a0 = av[0];
	av++;
	while( av[0][0] == '-' ) {
		switch( av[0][1] ) {
		    case 'r':
			if( av[0][2] != '\0' )
				remotefrom = &av[0][2];
			else if( av[2] == NULL )
				remotefrom = *(++av);
			break;

		    case 'u':
			update++;
			break;

		    case 'q':
			query++;
			break;

		    case 'D':
			debug++;
			break;

		    case 'd':
			if( av[0][2] != '\0' )
				wdir = &av[0][2];
			else if( av[2] == NULL )
				wdir = *(++av);
			break;

		    case 'n':
			dontsend++;
			break;

		    case 'w':
			dontwrite++;
			break;

		    default:
		    usage:
			fprintf(stderr, "Usage: %s [options] -r remote_from < message  (receive control message)\n", a0);
			fprintf(stderr, "       %s [options] -u                        (force update of remotes)\n", a0);
			fprintf(stderr, "       %s [options] -q                        (poll remotes to get info)\n", a0);
			fprintf(stderr, "Options:  -n      - prohibit sending messages\n");
			fprintf(stderr, "          -w      - prohibit updating local route table\n");
			fprintf(stderr, "          -d dir  - assume working directory at dir\n");
			fprintf(stderr, "          -D      - print debugging information\n");
			exit(1);
		}
		av++;
	}
	if( *av != NULL )
		goto usage;
	if( (query!=0) + (update!=0) + (remotefrom!=NULL) != 1 )
		goto usage;
	if( chdir(wdir) ) {
		fprintf(stderr, "%s: '%s' is not a directory\n", a0, wdir);
		exit(2);
	}

	gethost(my_name+5, 65);

	/*
	 * Read systems file
	 */
	strcpy(errors_to, "root");
	if( (f = fopen("Systems", "r")) == NULL ) {
		fprintf(stderr, "%s: cannot open Systems file\n", a0);
		exit(3);
	}
	if( debug )
		printf("============= READING Systems\n");
	joinroutes(f, 1, 0, 0, "Systems");
	fclose(f);
	if( query )
		goto Distribute;

	/*
	 * If in receiving mode - copy message into
	 * the file
	 */
	if( remotefrom != NULL && receive(remotefrom) )
		exit(0);        /* No changes -- do nothing */

	if( dontwrite ) {
		if( (f = fopen("Routes", "r")) == NULL ) {
			errmsg("There is no Routes table - you should update it\n");
			exit(1);
		}
		goto Distribute;
	}

	/*
	 * Read in collected remote routes.
	 */
	for( sy = syslist ; sy != NULL ; sy = sy->s_next ) {
		sprintf(filename, RFNAME, sy->s_nor);
		if( debug )
			printf("---- READING %s\n", filename);
		if( (f = fopen(filename, "r")) == NULL ) {
			if( debug )
				printf("OOPS\n");
			continue;
		}
		sprintf(filename, "route %d to <%s>", sy->s_nor, sy->s_path);
		joinroutes(f, 0, sy->s_nor, sy->s_metric, filename);
		fclose(f);
	}

	/*
	 * Optimize routes by joining co-directed paths
	 */
	/* NOT IMPLEMENTED NOW */

	/*
	 * Write the new route table into Routes file
	 */
	if( debug )
		printf("========== Updating Routes\n");
	if( (f = fopen("Routes.tmp", "w")) == NULL ) {
		errmsg("Can't create the temp. file\n");
		exit(4);
	}
	outtab(f);
	fclose(f);

	/*
	 * Compare the files
	 */
	if( (f = fopen("Routes", "r")) != NULL ) {
		if( (f2 = fopen("Routes.tmp", "r")) == NULL ) {
			errmsg("Can't read temp file\n");
			exit(5);
		}
		while( (c = getc(f)) != EOF &&
		       (c2 = getc(f2)) != EOF ) {
			if( c != c2 )
				goto FilesDiffer;
		}
		/* files are the same */
		if( update ) {
			fclose(f2);
			goto FilesSame;
		}
		/* exit if no force update assumed */
		exit(0);
	}
FilesDiffer:
	if( debug )
		printf("*** Files differ ***\n");
	fclose(f);
	f = f2;
	unlink("Routes.old");
	link("Routes", "Routes.old");
	unlink("Routes");
	if( link("Routes.tmp", "Routes") ) {
		errmsg("Can not relink Routes file\n");
		exit(5);
	}
FilesSame:
	unlink("Routes.tmp");

	/*
	 * Distribute new route table to
	 * remote systems
	 */
Distribute:
	if( dontsend )
		exit(0);
	for( sy = syslist ; sy != NULL ; sy = sy->s_next ) {
		if( uassumed == NULL )
			makeNname(filename, sy->s_path, UU_USER);
		else
			strcpy(filename, uassumed);
		if( debug )
			printf("===== Send Mail to '%s' =================>\n", filename);
		f2 = pexec(MAILER, "send-mail", "-r", my_name, filename, NULL);
		if( sy->s_passwd != NULL )
			fprintf(f2, "Password: %s\n", sy->s_passwd);
		fprintf(f2, "To: %s\n", filename);
		fprintf(f2, "From: %s (UUCP routing service)\n", my_name);
		if( query ) {
			fprintf(f2, "Control: Q\n\n");
		} else {
			fprintf(f2, "Control: U\n");
			if( errors_to[0] != '\0' )
				fprintf(f2, "Reply-To: %s\n", errors_to);
			if( sy->s_nor == -1 )
				fprintf(f2, "\n# No backward route found\n");
			else if( sy->s_nor == -2 )
				fprintf(f2, "\n# Several backard routes match the address.\n# Try to specify your address more precisely.\n");
			else
				fprintf(f2, "\n%%b %d\n", sy->s_nor);
			rewind(f);
			while( (c = getc(f)) != EOF )
				putc(c, f2);
		}
		if( !debug ) {
			fclose(f2);
			i = 0;
			wait(&i);
			if( i != 0 )
				errmsg("Mailer has returned code %d sending to '%s'\n", i, filename);
		}
	}
	exit(0);
}

/*
 * Send an error message
 */
errmsg(f, a, b, c, d, e)
	char *f;
{
	static FILE *erf = NULL;

	if( erf == NULL ) {
		if( isatty(2) )
			erf = stderr;
		else {
			erf = pexec(MAILER, "send-mail", "-r", my_name, errors_to, NULL);
			if( erf == NULL )
				erf = stdout;
			else {
				fprintf(erf, "From: %s (UUCP routing service)\n", my_name);
				fprintf(erf, "To: %s\n", errors_to);
				fprintf(erf, "Subject: Troubles collecting routes\n\n");
			}
		}
	}
	fprintf(erf, f, a, b, c, d, e);
	/* Anyway, print errors to stderr */
	if( erf != stdout && erf != stderr )
		fprintf(stderr, f, a, b, c, d, e);
}

/*
 * Exec with piping
 */
FILE *pexec(a, b, c, d, e, f, g, h)
	char *a, *b, *c, *d, *e, *f, *g, *h;
{
	int     rcnt, pid;
	int     pd[2];
	FILE *of;

	if( debug )
		return stdout;
	rcnt = 0;
retry:
	if( (pid = fork()) == 0 ) {
		/* New process */
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		close(0);
		close(pd[1]);
		dup(pd[0]);
		for( rcnt = 3 ; rcnt < 20 ; rcnt++ )
			close(rcnt);
		execl(a, b, c, d, e, f, g, h);
		exit(0177);
	}
	if( pid == -1 ) {
		if( ++rcnt < 20 ) {
			sleep(5);
			goto retry;
		}
		return (FILE *)NULL;
	}
	close(pd[0]);
	if( (of = fdopen(pd[1], "w")) == NULL ) {
		close(pd[1]);
		fprintf(stderr, "Can't fdopen\n");
	}
	return of;
}

/*
 * Get a current host name for replies (XENIX version)
 */
gethost(buf, len)
	char buf[];
{
	int     fd[2];
	int     l, rlen;
	static char hn[80] = "";
	int     istream;
	char *p;
	FILE *inf;

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
			if( (inf = popen(UUNAME, "r")) != NULL ) {
				fgets(hn, sizeof hn - 6, inf);
				pclose(inf);
				if( (p = index(hn, '\n')) != NULL )
					*p = '\0';
			} else
				strcpy(hn, "foobar");
			strcat(hn, ".UUCP");
		} else {
			(void) read(istream, hn, 79);
			close( istream );
		}
	}
	if( (p = index(hn, '\n')) != NULL )
		*p = '\0';
	/* Ugh! */
	strncpy(buf, hn, len);
}

/*
 * Construct the full network name from the net path
 */
makeNname(res, path, uname)
	char *res;
	char *path;
	char *uname;
{
	register char *p, *q, *r;

	r = res;
	q = path;

	/* Try to parse an Internet address (with %) */
	if( (p = rindex(path, '%')) == NULL &&
	    (p = rindex(path, '!')) == NULL &&
	    (p = rindex(path, '@')) == NULL ) {
		strcpy(res, uname);
		return;
	}
	if( *p == '!' )
		p++;
	while( q < p )
		*r++ = *q++;
	q = uname;
	while( *r++ = *q++ );
	r--;
	while( *r++ = *p++ );
}

/*
 * Compare user name with pre-defined names
 */
int cuname(name)
	char *name;
{
	register char *p, *q;
	char c;
	int res;

	q = rindex(name, '@');
	if( q == NULL )
		q = rindex(name, '%');
	else if( (p = rindex(name, '%')) != NULL && p < q )
		q = p;
	if( q != NULL ) {
		c = *q;
		*q = '\0';
	}

	if( (p = rindex(name, '!')) == NULL &&
	    (p = rindex(name, ':')) == NULL )
		p = name;
	else
		p++;

	res = !strcmp(p, "uucp")     ||
	      !strcmp(p, "uurouter") ||
	      !strcmp(p, "UUrouter");

	if( q != NULL )
		*q = c;
	return res;
}


/*
 * Receive the control message.
 * Return 0 if no new information were found.
 */
receive(from)
	char *from;
{
	char line[300];
#define filename  line
#define tfilename (&line[150])
	register char *p;
	register c;
	int pass, ctl;
	struct SYSTEM *sy, *syf;
	FILE *newf, *oldf;
	static struct SYSTEM S = { NULL, -1, 0, NULL, NULL };

	pass = (mypassword[0] == '\0');
	ctl = 0;

	/*
	 * Parse the header of message
	 */
	while( gets(line) != NULL ) {
		if( line[0] == '\0' )
			break;                  /* End of header */
		if( !strncmp(line, "Password:", 9) ) {
			p = &line[9];
			while( *p == ' ' || *p == '\t' ) p++;
			if( !strcmp(mypassword, p) )
				pass++;
			continue;
		}
		if( !strncmp(line, "Control:", 8) ) {
			p = &line[8];
			while( *p == ' ' || *p == '\t' ) p++;
			if( *p == 'Q' || *p == 'U' )
				ctl = *p;
			continue;
		}
	}
	if( debug )
		printf("Header field: ctl=%c pass=%d\n", ctl, pass);
	if( !ctl ) {
		errmsg("Garbage from %s has been received and thrown out.\n", from);
		return 1;       /* give it up */
	}
	if( ctl != 'Q' && !pass ) {
		errmsg("Password mismatch on CTL(%c) message from %s.\n", ctl, from);
		return 1;
	}

	/*
	 * Search the system through Systems list
	 * to determine number of system in Systems file
	 */
	if( !cuname(from) ) {
		syf = &S;
		goto unrecognized;
	}
	if( parsefrom(from) ) {
		if( ctl == 'Q' ) {
			syf = &S;
			goto unrecognized;
		}
		errmsg("Source of CTL message has the local address: '%s'\n", from);
		return 1;
	}
	syf = NULL;
	c = 0;
	for( sy = syslist ; sy != NULL ; sy = sy->s_next ) {
		if( isfrom(sy->s_path) ) {
			if( debug )
				printf("MATCHED\n");
			if( syf != NULL ) {
				if( ctl == 'Q' ) {
					syf = &S;
					S.s_nor = -2;
					goto unrecognized;
				}
				if( !c++ ) {
					errmsg("WARNING: ambigious ctl message source identification.\n");
					errmsg("   Source address '%s' is matched with:\n", from);
					errmsg("   NOR=%d PATH='%s'\n", syf->s_nor, syf->s_path);
				}
				errmsg("   NOR=%d PATH='%s'\n", sy->s_nor, sy->s_path);

			}
			syf = sy;
		}
	}
	if( c )
		return 1;
	if( syf == NULL ) {
		if( ctl == 'Q' ) {
			syf = &S;
			goto unrecognized;
		}
		errmsg("CTL(%c) message from unknown system: '%s'\n", ctl, from);
		return 1;       /* OOPS, system not found */
	}

	/*
	 * Process QUERY message
	 */
	if( ctl == 'Q' ) {
		syf->s_next = NULL;
unrecognized:
		syslist = syf;
		uassumed = from;
		dontwrite = 1;
		return 0;
	}

	/*
	 * Process UPDATE message - receive it and compare with existing
	 * route table.
	 */
       sprintf(filename, RFNAME, syf->s_nor);
       if( (oldf = fopen(filename, "r")) == NULL ) {

		/* There are no old route file - just copy all stdin */
		if( (newf = fopen(filename, "w")) == NULL ) {
			 errmsg("Can not create temp. file\n");
			 return 1;
		}
		while( (c = getchar()) != EOF )
			putc(c, newf);
		fclose(newf);
		return 0;
	}
	sprintf(tfilename, TRFNAME, syf->s_nor);
	if( (newf = fopen(filename, "w")) == NULL ) {
		 errmsg("Can not create temp. file\n");
		 return 1;
	}
	while( (c = getchar()) != EOF ) {
		putc(c, newf);
		if( c != getc(oldf) )
			goto NotSame;
	}
	if( getc(oldf) == EOF ) {
		fclose(newf);
		fclose(oldf);
		unlink(tfilename);
		if( debug )
			printf("Received and new files are the same\n");
		return 1;               /* there is nothing to do */
	}
NotSame:
	fclose(oldf);
	while( (c = getchar()) != EOF )
		putc(c, newf);
	fclose(newf);
	unlink(filename);
	if( link(tfilename, filename) == -1 ) {
		errmsg("Can not rename temp file\n");
		unlink(tfilename);
		return 1;
	}
	unlink(tfilename);
	if( debug )
		printf("Received and Old files differ.\n");
	return 0;
}

int     WORKAREA[2048/sizeof(int)];    /* Working area for address parsing */
int     wordsbusy, savebusy;

struct AP {
	struct AP *ap_next;     /* ptr to next structure */
	char       ap_in;       /* 1 for Internet, 0 for UUCP */
	char       ap_name[1];  /* Word of address */
}      *fromlist;

/*
 * Allocate a new FROM structure
 */
struct AP *allocap(type, name)
	char type;
	char *name;
{
	register struct AP *ap;

	ap = (struct AP *) &WORKAREA[wordsbusy];
	wordsbusy += (sizeof(struct AP) +
		      strlen(name) + sizeof(int)-1) / sizeof(int);
	if( wordsbusy > (sizeof WORKAREA)/sizeof(int) ) {
		errmsg("No more free space in workarea");
		exit(100);
	}
	ap->ap_next = NULL;
	ap->ap_in = type;
	strcpy(ap->ap_name, name);
	return ap;
}

/*
 * Parse the network address
 */
struct AP *parsename(name)
	char *name;
{
	register char *p, *q;
	char c;
	struct AP *ap, *apl;

	if( debug )
		printf("Parse '%s' -->", name);

	/* Strip out explicit Internet path */
	while( name[0] == '@' || name[0] == '%' ) {
		if( (p = index(name, ':')) == NULL )
			break;  /* ???? */
		name = p+1;
	}

	/* Fetch the remote Internet name */
	apl = NULL;
	if( (p = rindex(name, '%')) != NULL ) {
		q = ++p;
		while( *q && *q != '@' ) q++;
		c = *q;
		*q = '\0';
		ap = allocap(1, p);
		ap->ap_next = NULL;
		apl = ap;
		*q = c;
	}

	/* Fetch the UUCP part of name */
	if( (p = rindex(name, '!')) != NULL ) {
		q = p;
		while( --p >= &name[-1] ) {
			if( p < name || *p == '!' ) {
				c = *q; *q = '\0';
				ap = allocap(0, p+1);
				ap->ap_next = apl;
				apl = ap;
				*q = c;
				q = p;
			}
		}
	}

	/* Fetch the local Internet address part */
	if( (p = rindex(name, '@')) != NULL ) {
		ap = allocap(1, p+1);
		ap->ap_next = apl;
		apl = ap;
	}

	if( debug ) {
		for( ap = apl ; ap != NULL; ap = ap->ap_next )
			printf(" %c%s", ap->ap_in?'@':'!', ap->ap_name);
		printf("\n");
	}
	return apl;
}

/*
 * Parse source address
 */
parsefrom(from)
	char *from;
{
	wordsbusy = 0;
	fromlist = parsename(from);
	savebusy = wordsbusy;
	if( fromlist == NULL )
		return 1;
	return 0;
}

#define tolower(c)      ((c) | (('A'<=(c) && (c)<='Z') ? 040 : 0))

/*
 * Compare host names
 */
int hncmp(patt, name)
	register char *patt;
	register char *name;
{
	while( *name && tolower(*patt) == tolower(*name) ) patt++, name++;
	if( *name != '\0' && *name != '.' )
		return 0;
	return (*patt == '\0' || *patt == '.');
}

/*
 * If given address is the name of host from which message arrived?
 */
int isfrom(name)
	char *name;
{
	struct AP *path, *from;

	path = parsename(name);
	from = fromlist;
	wordsbusy = savebusy;   /* trick */

	/*
	 * There are only Internet address - compare with remote part
	 */
	if( from->ap_in && from->ap_next == NULL ) {
		while( path->ap_next != NULL )
			path = path->ap_next;
		if( path->ap_in && hncmp(from->ap_name, path->ap_name) )
			return 1;
		return 0;
	}
	if( path->ap_in && path->ap_next == NULL ) {
		while( from->ap_next != NULL )
			from = from->ap_next;
		if( from->ap_in && hncmp(from->ap_name, path->ap_name) )
			return 1;
		return 0;
	}

	/*
	 * There are both local Internet addresses given
	 */
	if( from->ap_in && path->ap_in &&
	    !hncmp(from->ap_name, path->ap_name) )
		return 0;

	/*
	 * Skip local Internet part
	 */
	if( from->ap_in )
		from = from->ap_next;
	if( path->ap_in )
		path = path->ap_next;

	/*
	 * There are UUCP path
	 */
	if( !from->ap_in && !path->ap_in ) {
		while( from != NULL && path != NULL ) {
			if( from->ap_in || path->ap_in )
				break;
			if( strcmp(from->ap_name, path->ap_name) )
				return 0;       /* OOPS, names differ */
			from = from->ap_next;
			path = path->ap_next;
		}
		/* exact matching */
		if( from == NULL && path == NULL )
			return 1;
		/* ignore remote Internet part - it's quite possible */
		if( from == NULL && path->ap_in )
			return 1;
		if( path == NULL && from->ap_in )
			return 1;

		/* avoid matching exeeding UUCP hops */
		if( from == NULL || path == NULL )
			return 0;
	}

	if( !from->ap_in || !path->ap_in )
		return 0;       /* !@ or %!@  vs  %@ */
	/* compare remote Internet parts */
	return hncmp(from->ap_name, path->ap_name);
}
