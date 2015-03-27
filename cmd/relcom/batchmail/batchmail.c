/*
 *
 * Usage:
 *      batchmail [options] UUCP-destination-host address(es) < message
 *
 * Options:
 *      -s_SIZE_        - size of a batch file in KB
 *      -u_uux-option_  - option for UUX command (may be plural)
 *      -f              - force flush batch file
 *
 * Batch file format:
 *
 *      msg_length checksum address address...\n
 *      ...message...
 *      msg_length checksum address\n
 *      ...message...
 *  etc
 *      msg_length is a 7-digit decimal number of bytes in message
 *      checksum is a 4-digit hexadecimal checksum of a body of the message
 *
 * EX:
 *      0007453 fa90 chernenko@kgbvax piet@mcsun.eu.net
 *      From luser Wed Nov 07 23:15:11 MSK 1990 remote from npole
 *      Subject: April Fools Day joke
 *      To: piet@mcsun.eu.net (Piet Beertema)
 *      .....7327 bytes of bullshit.....
 *
 * Config file format:
 *
 *      uucp-hostname   [batch-size     [uux-options]]
 * EX:
 *      kgbvax          100             -z
 *      npole           50
 *      DEFAULT         100             -z -a
 *
 * $Log:	batchmail.c,v $
 * Revision 1.2  90/12/17  20:12:48  avg
 * Добавлена запись log-файла.
 * 
 * Revision 1.1  90/12/10  16:22:40  avg
 * Initial revision
 * 
 */

#ifndef lint
static char rcsid[] = "$Header: batchmail.c,v 1.2 90/12/17 20:12:48 avg Exp $";
#endif

/* #define DEBUG */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <sys/signal.h>
#include <stdio.h>

/* TUNABLE PARAMETERS */

#define UUX1            "/usr/bin/uux"          /* uux command */
#define UUX2            "/bin/uux"

#define BATCHSPOOL      "/usr/spool/batchmail"  /* spool directory */

#define BATCHLOG        "/usr/adm/batchlog"     /* logfile */

#define BATCHCONFIG     "/usr/lib/batchmail.cf" /* config file */

#define DEFAULT_BATCHSIZE        50             /* KBs */

#define LOCKEXPIRE              600             /* lock expiration time (secs) */


#ifdef DEBUG
#undef BATCHSPOOL
#define BATCHSPOOL "."
#undef BATCHCONFIG
#define BATCHCONFIG "batchmail.cf"
#endif DEBUG

#ifndef BATCHLOG
#define log(host, size)
#else
#include <time.h>
#endif

struct WORD {
	struct WORD *next;
	char        str[1];
};

struct WORD *addword(s, list)
	char        *s;
	struct WORD *list;
{
	struct WORD *wp;

	wp = (struct WORD *)malloc( strlen(s) + sizeof(struct WORD) );
	wp->next = list;
	strcpy(wp->str, s);
	return wp;
}

/*
 * Parse line of config file
 */
static int          p_bsiz;     /* batch size found */
static struct WORD *p_uxfl;     /* uux flags found */
static char         p_host[100]; /* hostname */

int parseconf(line, host)
	char *line;
	char *host;
{
	register char *p;
	char *q, c;

#ifdef DEBUG
	printf("parseconf(\"%s\", \"%s\")\n", line, host);
#endif

	p_uxfl = NULL;
	p_bsiz = 0;
	p = line;
	while( *p == ' ' || *p == '\t' ) p++;
	if( *p == '#' || !*p )
		return 0;
	q = p;
	while( *p != ' ' && *p != '\t' && *p ) p++;
	c = *p;
	*p = '\0';
	if( (host==NULL && !strcmp(q, "DEFAULT")) ||
	    (host!=NULL &&  strcmp(q, host)) ) {
		*p = c;
		return 0;
	}
	strcpy(p_host, q);
	*p = c;

	if( *p != ' ' && *p != '\t' )
		return 1;
	while( *p == ' ' || *p == '\t' ) p++;
	q = p;
	while( '0' <= *p && *p <= '9' ) p++;
	c = *p;
	*p = '\0';
	p_bsiz = atoi(q);
	*p = c;
	if( !c )
		return 1;
	if( c != ' ' && c != '\t' ) {
		fprintf(stderr, "batchmail: bad config entry for %s\n", p_host);
		exit(70);       /* EX_SOFTWARE */
	}
	for(;;) {
		while( *p == ' ' || *p == '\t' ) p++;
		if( *p == '\0' )
			break;
		q = p;
		while( *p != ' ' && *p != '\t' && *p ) p++;
		c = *p;
		*p = '\0';
		p_uxfl = addword(q, p_uxfl);
		*p = c;
	}
	return 1;
}

/*
 * The main routine
 */
main(ac, av)
	int     ac;
	char  **av;
{
	int batchsize = 0, dflt_bsiz = 0, orig_bsiz = 0;
	struct WORD *uxflist = NULL, *dflt_uxfl = NULL, *orig_uxfl = NULL;
	int force = 0, status;
	char *desthost = NULL;
	struct stat st;
	char batchfile[300];
	char configline[200];
	FILE *cf;
	register i;
	register unsigned sum;
	register char *p;
	long cnt;
	long ftell(), curpos;

	/*
	 * Parse options
	 */
	for( i = 1 ; i < ac ; i++ ) {
		if( av[i][0] != '-' )
			break;
		switch( av[i][1] ) {
		    case '-':
			break;
		    case 'f':
			force++;
			break;
		    case 'u':
			uxflist = addword(&av[i][1], uxflist);
			uxflist->str[0] = '-';
			break;
		    case 's':
			if( av[i][2] == '\0' )
				goto usage;
			batchsize = atoi(&av[i][2]);
			break;
		    default:
			goto usage;
		}
	}
	if( i >= ac ) {
		if( force ) {
			/*
			 * Scan config file
			 */
			if( (cf = fopen(BATCHCONFIG, "r")) == NULL ) {
				fprintf(stderr, "batchmail: cannot open %s\n", BATCHCONFIG);
				exit(72);       /* EX_OSFILE */
			}
			while( fgets(configline, (sizeof configline)-1, cf) != NULL ) {
				configline[(sizeof configline) - 1] = '\0';
				p = configline;
				while( *p != '\n' && *p ) p++;
				*p = '\0';
				if( parseconf(configline, "DEFAULT") ) {
					dflt_uxfl = p_uxfl;
					break;
				}
			}
			fseek(cf, 0l, 0);
			while( fgets(configline, (sizeof configline)-1, cf) != NULL ) {
				configline[(sizeof configline) - 1] = '\0';
				p = configline;
				while( *p != '\n' && *p ) p++;
				*p = '\0';
				if( !parseconf(configline, NULL) )
					continue;
#ifdef DEBUG
				printf("FOUND: host='%s', bsiz=%d\n", p_host, p_bsiz);
#endif
				if( uxflist != NULL )
					p_uxfl = uxflist;
				if( p_uxfl == NULL )
					p_uxfl = dflt_uxfl;

				/*
				 * Check for empty file(s)
				 */
				sprintf(batchfile, "%s/B.%s", BATCHSPOOL, p_host);
				lockfile(p_host);
				if( stat(batchfile, &st) == -1 || st.st_size == 0 ) {
					removelock(p_host);
					log(p_host, 0l, 1);
					continue;
				}

				/*
				 * Queue non-empty jobs
				 */
				status = queuejob(p_host, batchfile, p_uxfl);
				removelock(p_host);
				log(p_host, status? -1l : st.st_size, 1);
				if( status != 0 )
					exit(status);
			}
			fclose(cf);
			exit(0);        /* SUCCESS */
		}
    usage:      fprintf(stderr, "Usage: batchmail [-f] [-uuux-flag] [-ssize] [uucp-dest] [address...]\n");
		exit(64);       /* EX_USAGE */
	}
	desthost = av[i++];
	sprintf(batchfile, "%s/B.%s", BATCHSPOOL, desthost);

	/*
	 * Look for a record in config file
	 */
	if( (cf = fopen(BATCHCONFIG, "r")) != NULL ) {
		while( fgets(configline, (sizeof configline)-1, cf) != NULL ) {
			configline[(sizeof configline) - 1] = '\0';
			p = configline;
			while( *p != '\n' && *p ) p++;
			*p = '\0';
			if( parseconf(configline, desthost) ) {
				orig_uxfl = p_uxfl;
				orig_bsiz = p_bsiz;
				break;
			}
			if( parseconf(configline, "DEFAULT") ) {
				dflt_uxfl = p_uxfl;
				dflt_bsiz = p_bsiz;
			}
		}
		if( batchsize == 0 )
			batchsize = orig_bsiz;
		if( uxflist == NULL )
			uxflist   = orig_uxfl;
		if( batchsize == 0 )
			batchsize = dflt_bsiz;
		if( uxflist == NULL )
			uxflist   = dflt_uxfl;
		fclose(cf);
	}
	if( batchsize == 0 )
		batchsize = DEFAULT_BATCHSIZE;
#ifdef DEBUG
	printf("BATCHSIZE=%d\n", batchsize);
	printf("UUX FLAGS=");
	for( dflt_uxfl = uxflist ; dflt_uxfl != NULL ; dflt_uxfl = dflt_uxfl->next )
		printf(" %s", dflt_uxfl->str);
	printf("\n");
#endif

	/*
	 * Set signals
	 */
#ifndef DEBUG
	signal(SIGHUP,  SIG_IGN);
	signal(SIGINT,  SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
#endif DEBUG

	/*
	 * Check for dest addresses
	 */
	if( i >= ac ) {
		if( force ) {
#ifdef DEBUG
			printf("NO DEST ADDR, FLUSH BATCH BUFFER\n");
#endif
			/*
			 * If batchfile is not empty, queue the job
			 */
			lockfile(desthost);
			if( stat(batchfile, &st) == -1 || st.st_size == 0 ) {
#ifdef DEBUG
				printf("EMPTY BATCHFILE %s\n", batchfile);
#endif
				removelock(desthost);
				log(desthost, 0l, 1);
				exit(0);
			}

			status = queuejob(desthost, batchfile, uxflist);
			removelock(desthost);
			log(desthost, status? -1l : st.st_size, 1);
			exit(status);
		}
		fprintf(stderr, "batchmail: at least one dest address should be specified\n");
		exit(64);       /* EX_USAGE */
	}

	/*
	 * Lock batchfile
	 */
#ifdef DEBUG
	printf("ADD MESSAGE TO BATCHFILE\n");
#endif
	lockfile(desthost);
	status = 0;

	/*
	 * Add information to the batch file
	 */
	if( (cf = fopen(batchfile, "r+")) == NULL ) {
		if( (cf = fopen(batchfile, "w")) == NULL ) {
			fprintf(stderr, "batchmail: cannot write temp file\n");
			removelock(desthost);
			exit(75);       /* EX_TEMPFAIL */
		}
	}
	fseek(cf, 0l, 2);
	curpos = ftell(cf);
#ifdef DEBUG
	printf("FILE CURPOS=%ld\n", curpos);
#endif
	fputs("0000000 0000", cf);
	while( i < ac ) {
		putc(' ', cf);
		fputs(av[i++], cf);
	}
	putc('\n', cf);
	cnt = 0;
	sum = 0;
	while( (i = getc(stdin)) != EOF ) {
		cnt++;
		if( sum & 01 )
			sum = (sum>>1) + 0x8000;
		else
			sum >>= 1;
		sum += i;
		sum &= 0xFFFF;
		putc(i, cf);
	}
	if( ferror(cf) )
		status = 74;    /* EX_IOERR */
	fseek(cf, curpos, 0);   /* back to the beginning of the message */
	sprintf(configline, "%07lu %04x", cnt, sum);
	fwrite(configline, 12, 1, cf);
	if( ferror(cf) )
		status = 74;    /* EX_IOERR */
	fclose(cf);
#ifdef DEBUG
	printf("... %lu BYTES ADDED, SUM=%0x\n", cnt, sum);
#endif

	/*
	 * Check for batch file size
	 */
	i = 0;
	if( force || curpos+cnt >= batchsize*1024l ) {
		status = queuejob(desthost, batchfile, uxflist);
		i++;
	}
	removelock(desthost);
	if( i )
		log(desthost, status? -1l : (curpos+cnt), 0);
	exit(status);
}

/*
 * Queue job with UUX
 */
int queuejob(host, file, opts)
	char *host;         /* Remote host name */
	char *file;         /* Batch file name */
	struct WORD *opts;  /* UUX options */
{
	int status;
	register pid;
	char *argv[50], **ap;
	char rcmd[100];

#ifdef DEBUG
	printf("QUEUE JOB host='%s' file='%s'\n", host, file);
#endif
	ap = argv;
	*ap++ = "uux";
	*ap++ = "-";
	*ap++ = "-r";
	while( opts != NULL ) {
		if( opts->str[0] != '\0' )
			*ap++ = opts->str;
		opts = opts->next;
	}
	strcpy(rcmd, host);
	strcat(rcmd, "!rbmail");        /* receive batch mail */
	*ap++ = rcmd;
	*ap = NULL;

	if( (pid = fork()) == 0 ) {
		/* New process, reopen stdin for batchfile */
		close(0);
		if( open(file, 0) != 0 ) {      /* ??!! */
			fprintf(stderr, "batchmail: cannot open %s for reading!\n", file);
			exit(71); /* EX_OSERR */
		}
#ifdef DEBUG
		execv("/bin/echo", argv);
#else
		execv(UUX1, argv);
		execv(UUX2, argv);
#endif DEBUG
		fprintf(stderr, "batchmail: cannot exec uux!\n");
		exit(71); /* EX_OSERR */
	}
	if( pid == -1 ) {
		fprintf(stderr, "batchmail: cannot queue uucp job, try -f\n");
		return 75; /* EX_TEMPFAIL */
	}
	wait(&status);
	if( status == 0 ) {     /* Job was queued sucessfully */
		if( unlink(file) == -1 ) {
			fprintf(stderr, "batchmail: cannot unlink batch file %s\n", file);
			return 72;       /* EX_OSFILE */
		}
	}
	return status;
}

/*
 * Lock the batch file
 */
lockfile(host)
	char *host;
{
	struct stat st;
	time_t tt;
	char templockname[300];
	char lockfilename[300];
	int fd;
	int ntry = 20;

	sprintf(templockname, "%s/T.%d", BATCHSPOOL, getpid());
	sprintf(lockfilename, "%s/L.%s", BATCHSPOOL, host);
#ifdef DEBUG
	printf("LOCKING: %s -> %s\n", templockname, lockfilename);
#endif

	/*
	 * Create temporary lockfile
	 */
	if( (fd = creat(templockname, 0666)) == -1 ) {
		fprintf(stderr, "batchmail: cannot create lock file in %s\n", BATCHSPOOL);
		exit(72);       /* EX_OSFILE */
	}
	close(fd);

	/*
	 * Link temp lockfile to the effective lock
	 */
	time(&tt);
	srand((int)tt);

tryagain:
#ifdef DEBUG
	printf("TRY TO LINK...\n");
#endif
	if( ntry-- <= 0 ) {
		fprintf(stderr, "batchmail: cannot lock batchfile, looks like a problem\n");
		exit(71);       /* EX_OSERR */
	}
	if( link(templockname, lockfilename) == -1 ) {
		if( stat(lockfilename, &st) == -1 )
			goto tryagain;
		if( st.st_mtime + LOCKEXPIRE < tt ) {
			unlink(lockfilename);
			goto tryagain;
		}
		sleep( 20 + (rand() & 07) );
		goto tryagain;
	}

	/*
	 * Remove temporary lockfile
	 */
	unlink(templockname);
#ifdef DEBUG
	printf("LOCKING OK\n");
#endif
}


/*
 * Lock the batch file
 */
removelock(host)
	char *host;
{
	char lockfilename[300];

	sprintf(lockfilename, "%s/L.%s", BATCHSPOOL, host);
#ifdef DEBUG
	printf("REMOVE LOCK %s\n", lockfilename);
#endif
	unlink(lockfilename);
}

#ifdef BATCHLOG
/*
 * Write a log record
 */
log(host, size, force)
	char *host;
	long size;
{
	int fd;
	char buf[200];
	long now;
	struct tm *t;

	if( (fd = open(BATCHLOG, 1)) == -1 )
		return;
	time(&now);
	t = localtime(&now);
	sprintf(buf, "%s\t%ld\t%02d:%02d:%02d %d.%d.%02d%s\n", host, size,
		     t->tm_hour, t->tm_min, t->tm_sec,
		     t->tm_mday, t->tm_mon + 1, t->tm_year % 100,
		     force ? "\tF" : "");
	lseek(fd, 0l, 2);
	write(fd, buf, strlen(buf));
	close(fd);
}
#endif /* BATCHLOG */
