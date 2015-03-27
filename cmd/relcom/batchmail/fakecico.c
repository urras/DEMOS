/*
 * Fake uucico - used for calling batchmail -f *before* dialing
 */

#define BATCHMAIL  "/usr/lib/relcom/batchmail"
#define UUCICO     "/usr/lib/uucp/UUCICO"
#define HDBUUCP                 /* Honey DanBer UUCP */


#ifdef HDBUUCP
#define STATUS          "/usr/spool/uucp/.Status/"
#define LCKFILE         "/usr/spool/uucp/LCK.."
#define PERMS           "/usr/lib/uucp/Permissions"

#include <stdio.h>
#include <errno.h>

char *hdbsysname();
#endif


#ifndef NULL
#define NULL    ((char *)0)
#endif

main(ac, av)
	char **av;
{
	char **ap, *systemname;
	int pid, f, n, slave = 0;
	char *getlogin();
	register char *p;
#ifdef HDBUUCP
	static char statfile[256] = STATUS;
	static char lckfile[256]  = LCKFILE;
	char statline[256];
	int oldlck;
#endif

	systemname = NULL;
	if( ac == 1 ) {         /* dial-in */
		p = getlogin();
	SlaveMode:
		slave++;
#ifdef HDBUUCP
		systemname = hdbsysname(p);
printf("SLAVE, user=%s systemname=%s\n", p, systemname);

#else /* HDBUUCP */

		/* SHOULD BE CUSTOMIZED */

		systemname = p;
		if( systemname != NULL ) {
			if( systemname[0] == 'u' &&
			    systemname[1] == 'u' )
				systemname += 2;        /* skip uu.. */
		}

#endif /* HDBUUCP */

	} else {
		/* scan for -s or -S */
		for( ap = &av[1] ; ap < &av[ac] ; ap++ ) {
			if( ap[0][0] != '-' )
				continue;
#ifdef HDBUUCP
			if( ap[0][1] == 'u' ) {
				if( ap[0][2] == '\0' )
					p = *++ap;
				else
					p = &ap[0][2];
				goto SlaveMode;
			}
#endif /* HDBUUCP */
			if( ap[0][1] != 's' && ap[0][1] != 'S' )
				continue;
			if( ap[0][2] == '\0' )
				systemname = *++ap;
			else
				systemname = &ap[0][2];
			break;
		}
	}

	/* Call batchmail */
	if( systemname != NULL ) {

		if( !slave ) {  /* Master Mode */
#ifdef HDBUUCP
			if( fork() == 0 ) {     /* New process */

				/* Wait for TALKING status */
				strcat(statfile, systemname);
				strcat(lckfile,  systemname);
				oldlck = 0;
				for( pid = 0 ; pid < 900 ; sleep(1), pid++ ) {
					/* Check for lock file */
					f = open(lckfile, 0);
					if( f < 0 ) {
						if( oldlck )
							break;
						if( pid >= 15 )  /* No lockfile appeared for 15 sec */
							break;
						continue;
					} else {
						n = read(f, statline, 20);
						close(f);
						oldlck = 1;
					}

					/* Is uucico alive? */
					if( n <= 0 ) {
						if( pid >= 15 )  /* No lockfile appeared for 15 sec */
							break;
						continue;
					}
					statline[20] = 0;
					p = statline;
					while( *p == ' ' || *p == '\t' ) p++;
					n = 0;
					while( '0' <= *p && *p <= '9' )
						n = (n*10) + (*p++ - '0');
					errno = 0;
					kill(n, 0);
					if( errno == ESRCH ) {
						if( pid >= 15 )  /* No lockfile appeared for 15 sec */
							break;
						continue;
					}

					/* Read the status line */
					f = open(statfile, 0);
					if( f < 0 )
						continue;
					n = read(f, statline, (sizeof statline)-1);
					close(f);
					if( n <= 0 )
						continue;
					statline[n] = '\0';
					p = statline;
					while( *p ) {
						if( *p == '\n' )
							*p = '\0';
						else
							p++;
					}

					/* Is there a word TALKING? */
					p = statline;
					for( n = 0 ; *p && n < 4 ; n++ ) {
						while( *p && *p != ' ' ) p++;
						while( *p && *p == ' ' ) p++;
					}
					if( !strncmp(p, "TALKING", 7) )
						goto batch;
				}
				exit(0);        /* OBLOM */

			batch:
			/* Flush batchmail buffer */
				execl(BATCHMAIL, "batchmail", "-f", systemname, NULL);
				exit(1);
			}
		} else {        /* Slave mode */
#endif /* HDBUUCP */
			/* Flush batchmail buffer */
			if( (pid = fork()) == 0 ) {
				/* new process */
				execl(BATCHMAIL, "batchmail", "-f", systemname, NULL);
				exit(1);
			}
			if( pid != -1 )
				wait(&pid);
#ifdef HDBUUCP
		}
#endif
	}

	/* FINALLY, call the "true" uucico */
	execv(UUCICO, av);
	exit(1);
}

#ifdef HDBUUCP
char *hdbsysname(log)
char *log;
{
	FILE *f;
	register char *p, *q;
	char entry[1024];
	char line[512];
	static char name[80];

	if( log == NULL )
		return NULL;
	f = fopen(PERMS, "r");
	if( f == NULL )
		return NULL;

	q = entry;
	while( fgets(line, 511, f) != NULL ) {

		/* Collect an entry */
		line[511] = '\0';
		p = line;
		while( *p && *p != '\n' ) p++;
		*p = '\0';

		p = line;
		while( *p == ' ' || *p == '\t' ) p++;
		if( *p == '#' || *p == '\0' )
			continue;       /* Empty line */
		while( *p ) *q++ = *p++;
		if( q[-1] == '\\' ) {
			q[-1] = ' ';
			continue;
		}
		*q = '\0';

		/* Parse the entry */
		p = entry;
		while( *p && strncmp(p, "LOGNAME=", 8) ) {
			/* skip word */
			while( *p && *p != ' ' && *p != '\t' ) p++;
			while( *p == ' ' || *p == '\t' ) p++;
		}
		if( *p == '\0' ) {      /* No LOGNAME= */
			q = entry;
			continue;
		}
		p += 8;
		q = name;
		while( *p && *p != ' ' && *p != '\t' ) *q++ = *p++;
		*q = '\0';
		if( strcmp(log, name) ) {
			q = entry;
			continue;
		}

		/* Matched, retrieve UUCP machine name */

		p = entry;
		while( *p && strncmp(p, "MACHINE=", 8) ) {
			/* skip word */
			while( *p && *p != ' ' && *p != '\t' ) p++;
			while( *p == ' ' || *p == '\t' ) p++;
		}
		if( *p == '\0' ) {      /* No MACHINE= */
			q = entry;
			continue;
		}
		p += 8;
		q = name;
		while( *p && *p != ' ' && *p != '\t' ) *q++ = *p++;
		*q = '\0';
		return name;
	}
	return NULL;
}
#endif /* HDBUUCP */
