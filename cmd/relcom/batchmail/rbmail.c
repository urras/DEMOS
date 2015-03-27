/*
 * Receive batched mail
 *
 * Usage:       rbmail <  batchfile
 *
 * Batch file format:
 *
 *      msg_length checksum address address...\n
 *      ...message...
 *      msg_length checksum address...\n
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
 * $Log: rbmail.c,v $
 * Revision 1.2  1991/02/07  21:11:38  avg
 * A small buf fixed in From_ line of error report.
 *
 * Revision 1.1  90/12/10  16:23:29  avg
 * Initial revision
 * 
 */

#ifndef lint
static char rcsid[] = "$Header: /usr/src/relcom/batchmail/RCS/rbmail.c,v 1.2 1991/02/07 21:11:38 avg Exp $";
#endif

#include <stdio.h>

#define RMAIL1  "/usr/bin/rmail"
#define RMAIL2  "/bin/rmail"

#define BATCHSPOOL      "/usr/spool/batchmail"  /* spool directory */

#define ERRORS_TO       "uucp"          /* A user to send errors to */

main(ac, av)
	char **av;
{
	char errfile[300];
	char tmpfile[50];
	char headerline[1000];
	char diagheader[1000];
	char *rargs[100];
	unsigned rcsum, csum;
	int retry, status, i, pid, pfd[2];
	register c;
	long     len;
	char    *p, **ap;
	long     tt;
	char *err = NULL, *err1 = NULL;
	FILE *temp;
	FILE *diag;
	extern char *ctime();

	chdir(BATCHSPOOL);   /* to make it to leave cores in the proper place */

	/* Close extra file descriptors */
	for( i = 3 ; i < 25 ; i++ )
		close(i);

	if( ac != 1 ) {
		fprintf(stderr, "Usage: rbmail < batchfile\n");
		exit(64);       /* EX_USAGE */
	}
	sprintf(tmpfile, "/tmp/BMTMP.%d", getpid());
	umask(077);

	/*
	 * Loop!!!
	 */
	while( fgets(headerline, (sizeof headerline)-1, stdin) != NULL ) {

		/*
		 * Parse header line
		 */
		rargs[0] = NULL;
		headerline[(sizeof headerline)-1] = '\0';
		strcpy(diagheader, headerline);
		p = headerline;
		while( *p != '\n' && *p ) p++;
		temp = NULL;
		if( ! *p ) {
			err = "Header line corrupted (missing newline)";
copy_on_error:
			time(&tt);
			sprintf(errfile, "%s/ERR%lu", BATCHSPOOL, tt);
			diag = fopen(errfile, "w");
			if( diag == NULL )
				err1 = "CANNOT CREATE FILE ";
			else {
				err1 = "The rest of batchfile is in ";
				fputs(diagheader, diag);
				if( temp != NULL ) {
					while( (c = getc(temp)) != EOF )
						putc(c, diag);
					unlink(tmpfile);
				}
				while( (c = getc(stdin)) != EOF )
					putc(c, diag);
				fclose(diag);
			}
			if( pipe(pfd) == -1 ) {
				fprintf(stderr, "RBMAIL: CANNOT MAKE PIPE!\n");
				diag = stderr;
			} else {
		do_retry:       if( !(pid = fork()) ) {
					close(0);
					dup(pfd[0]);
					close(pfd[0]);
					close(pfd[1]);
					execl(RMAIL1, "rmail", ERRORS_TO, 0);
					execl(RMAIL2, "rmail", ERRORS_TO, 0);
					exit(1);
				}
				if( pid == -1 ) {
					sleep(20);
					goto do_retry;
				}
				close(pfd[0]);
				diag = fdopen(pfd[1], "w");
				if( diag == NULL )
					diag = stderr;
			}
			fprintf(diag, "From uucp %s", ctime(&tt));
			fprintf(diag, "From: uucp (UUCP Batch Mail Service)\n");
			fprintf(diag, "To: %s\n", ERRORS_TO);
			fprintf(diag, "Subject: batch receiption failed\n\n");
			fprintf(diag, "ERROR: %s.\n       %s %s\n\n", err, err1, errfile);
			fclose(diag);
			exit(0);
		}
		*p = '\0';

		/* Length */
		p = headerline;
		len = 0;
		for( i = 0 ; i < 7 ; i++ ) {
			c = *p++;
			if( '0' > c || c > '9' ) {
				err = "Header line corrupted (bad length)";
				goto copy_on_error;
			}
			len = (len*10) + (c-'0');
		}
		if( *p++ != ' ' ) {
			err = "Header line corrupted (no space after length)";
			goto copy_on_error;
		}

		/* Checksum */
		rcsum = 0;
		for( i = 0 ; i < 4 ; i++ ) {
			c = *p++;
			rcsum <<= 4;
			if( '0' <= c && c <= '9' )
				rcsum |= c-'0';
			else if( 'A' <= c && c <= 'F' )
				rcsum |= c-'A'+0xa;
			else if( 'a' <= c && c <= 'f' )
				rcsum |= c-'a'+0xa;
			else {
				err = "Header line corrupted (bad checksum field)";
				goto copy_on_error;
			}
		}
		if( *p != ' ' ) {
			err = "Header line corrupted (no space after checksum)";
			goto copy_on_error;
		}

		/* Destination addresses */
		if( ! *p ) {
			err = "Header line corrupted (no dest address)";
			goto copy_on_error;
		}
		ap = &rargs[1];
		for(;;) {
			while( *p == ' ' || *p == '\t' ) p++;
			if( *p == '\0' )
				break;
			if( ap >= &rargs[99] ) {
				err = "Header line corrupted (too many addresses)";
				goto copy_on_error;
			}
			*ap++ = p;
			for(;;) {
				if( *p == '"' ) {
					do {
						p++;
					} while( *p && *p != '"' );
					if( *p ) p++;
					continue;
				}
				if( *p == ' ' ) {
					*p++ = '\0';
					break;
				}
				if( *p == '\0' )
					break;
				p++;
			}
		}
		*ap = NULL;
		if( ap == &rargs[1] ) {
			err = "Header line corrupted (no dest address(es))";
			goto copy_on_error;
		}

		/*
		 * Create temporary file
		 */
		temp = fopen(tmpfile, "w");
		if( temp == NULL ) {
			err = "Cannot create temporary file";
			goto copy_on_error;
		}

		/*
		 * Copy into temp. file calculating checksum on the fly
		 */
		csum = 0;
		while( len-- > 0 ) {
			c = getchar();
			if( c == EOF ) {
				fclose(temp);
				temp = fopen(tmpfile, "r");
				err = "Premature end of the batch file";
				goto copy_on_error;
			}
			putc(c, temp);
			if( ferror(temp) ) {
				fclose(temp);
				temp = fopen(tmpfile, "r");
				err = "Error writing temp file";
				goto copy_on_error;
			}
			if( csum & 01 )
				csum = (csum>>1) + 0x8000;
			else
				csum >>= 1;
			csum += c;
			csum &= 0xFFFF;
		}
		fclose(temp);

		if( csum != rcsum ) {
			temp = fopen(tmpfile, "r");
			err = "Checksum error";
			goto copy_on_error;
		}

		/*
		 * Read the temp file
		 */
		if( (temp = fopen(tmpfile, "r")) == NULL ) {
			err = "Cannot reopen temp file (?!!)";
			goto copy_on_error;
		}

		/*
		 * Call rmail
		 */
		retry = 0;
tryagain:       if( (pid = fork()) == 0 ) {
			   rargs[0] = "rmail";
			   c = dup(0);          /* Save stdin */
			   close(0);
			   dup(fileno(temp));   /* Assign tempfile to stdin */

			   execv(RMAIL1, rargs);
			   execv(RMAIL2, rargs);

			   close(0);
			   dup(c);
			   close(c);
			   err = "Cannot exec rmail";
			   goto copy_on_error;
		}
		if( pid == -1 ) {
			if( retry++ < 10 ) {
				sleep(10);
				goto tryagain;
			}
			err = "Cannot fork";
			goto copy_on_error;
		}
		wait(&status);
	}
	unlink(tmpfile);
	exit(0);
}
