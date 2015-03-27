/*
 *  MTO - send a binary file(s) to the user over E-mail
 *        using uuencode format
 *
 *  Copyright 1990, DEMOS cooperative, Moscow, USSR
 *  All rights reserved.
 *
 *  The copyright above and this notice must be preserved in all
 *  copies of this source code. The copyright above does not
 *  evidence any actual or intended publication of this source
 *  code.
 *
 *  This is unpublished proprietary trade secret source code of
 *  DEMOS cooperative. This source code may not be copied,
 *  disclosed, distributed, demonstrated or licensed except as
 *  expressly authorized by DEMOS.
 *
 *
 * Usage:  mto [-u] [-z] [-bN] file... address
 *
 *         -u   - send mail to the user named instead of uu-decode
 *                (for the sake of compatibility with old E-mail systems)
 *         -z   - compress the file before sending
 *         -bN  - number of bits for compress (default - 12)
 *
 *       File name "-" stands for stdio.
 *
 * Calls:  compress, mail
 *
 * $Log:	mto.c,v $
 * Revision 1.5  90/08/15  20:57:21  avg
 * name changed from uuto to mto
 * 
 * Revision 1.4  90/08/10  13:49:42  avg
 * Mailer now will be affected with -x key.
 * 
 * Revision 1.3  90/08/02  17:47:20  avg
 * *** empty log message ***
 * 
 * Revision 1.2  90/08/02  15:07:06  avg
 * Пралена работа с ключем -u.
 * 
 * Revision 1.1  90/07/30  22:27:14  avg
 * Initial revision
 * 
 */

#ifndef lint
static char rcsid[] = "$Header: mto.c,v 1.5 90/08/15 20:57:21 avg Exp $";
#endif

#define COMPRESS        "compress"
#define MAIL            "mail -x"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

/* ENC is the basic 1 character encoding function to make a char printing */
#define ENC(c) ((c) ? ((c) & 077) + ' ': '`')

int debug = 0;

main(argc, argv)
	char **argv;
{
	register i;
	register char *p;
	FILE *in, *out;
	struct stat sbuf;
	int   mode, savein, alrcomp;
	int   compress = 0, compat = 0, bits = 12;
	char *username, *sendto, *remote_user;
	char *exuser(), *changeuser();
	char buf[300], filename[300];

	/*
	 * Parse options
	 */
	for( i = 1 ; i < argc ; i++ ) {
		if( argv[i][0] != '-' )
			break;

		p = &argv[i][1];
		if( *p == '\0' )
			break;
		while( *p ) {
			switch( *p++ ) {
			    case 'z':
				compress++;
				break;
			    case 'u':
				compat++;
				break;
			    case 'b':
				bits = 0;
				while( '0' <= *p && *p <= '9' )
					bits = bits*10 + (*p++ - '0');
				if( bits < 10 || bits > 19 ) {
					fprintf(stderr, "%s: illegal number of bits\n", argv[0]);
					exit(1);
				}
				break;
			    case 'D':
				debug++;
				break;
			    case '-':
				i++;
				goto Files;
			    default:
				goto Usage;
			}
		}
	}

	/*
	 * Handle file names
	 */
Files:  if( i >= argc-1 ) {
Usage:          fprintf(stderr, "Usage: %s [-z] [-h] [-bN] file... address\n", argv[0]);
		exit(1);
	}
	username = argv[argc-1];
	remote_user = exuser(username);
	sendto = compat ? username : changeuser(username);
	if( remote_user[0] == '\0' )
		compat++;
	savein = dup(0);        /* save stdin */
	for( ; i < argc-1 ; i++ ) {
		close(0);       /* close stdin */
		if( !strcmp(argv[i], "-") )
			dup(savein);
		else if( open(argv[i], 0) != 0 ) {
			fprintf(stderr, "%s: cannot open %s\n", argv[0], argv[i]);
			exit(2);
		}

		/*
		 * Construct file name for compressed info
		 */
rebuild_name:   strcpy(filename, argv[i]);
		alrcomp = 0;
		if( compress ) {
			p = filename;
			while( *p ) p++;
			if( p > &filename[2] && p[-2] ==  '.' &&
					     p[-1] == 'Z' )
				alrcomp++;
			else {
				*p++ = '.';
				*p++ = 'Z';
				*p = '\0';
			}
		}

		/* Remove "/", ".", ".." at the beginning */
		p = filename;
		for(;;) {
			if( *p == '/' )
				p++;
			else if( *p == '.' && p[1] == '/' )
				p += 2;
			else if( *p == '.' && p[1] == '.' && p[2] == '/' )
				p += 3;
			else
				break;
		}

		/*
		 * If the compression required - start compress
		 */
		if( compress && !alrcomp ) {
			sprintf(buf, "%s -b%d", COMPRESS, bits);
			in = popen(buf, "r");
			if( in == NULL ) {
				fprintf(stderr, "%s: cannot execute compress (warning only)\n", argv[0]);
				compress = 0;
				goto rebuild_name;
			}
		} else
			in = stdin;

		/*
		 * Start MAIL
		 */
		if( compat )
			sprintf(buf, "%s -s 'File %s' '%s'", MAIL, p, sendto);
		else
			sprintf(buf, "%s '%s'", MAIL, sendto);
		out = popen(buf, "w");
		if( out == NULL ) {
			fprintf(stderr, "%s: can not execute mailer\n", argv[0]);
			exit(3);
		}

		/*
		 * Read status of the file
		 */
		fstat(0, &sbuf);
		mode = sbuf.st_mode & 0777;
		if( !compat ) {
			if( debug )
				fprintf(stderr, "SEND FILE ~%s/%s mode %o\n", remote_user, p, mode);
			fprintf(out, "begin %o ~%s/%s\n", mode, remote_user, p);
		} else {
			if( debug )
				fprintf(stderr, "SEND FILE %s mode %o\n", p, mode);
			fprintf(out, "begin %o %s\n", mode, p);
		}

		encode(in, out);

		fprintf(out, "end\n");

		if( in != stdin ) {
			if( (mode = pclose(in)) != 0 ) {
				fprintf("%s: compress has returned code %d\n", mode);
				exit(4);
			}
		}
		if( (mode = pclose(out)) != 0 ) {
			fprintf("%s: mailer has returned code %d\n", mode);
			exit(mode);
		}
	}
	exit(0);
}

/*
 * Copy from in to out, encoding as you go along.
 */
encode(in, out)
FILE *in;
FILE *out;
{
	char buf[80];
	int i, n;
	int c1, c2, c3, c4;
	register char *p;

	do {
		/* 1 (up to) 45 character line */
		n = fread(buf, 1, 45, in);
		putc(ENC(n), out);

		for( i = 0 ; i < n ; i += 3 ) {
			p = &buf[i];
			c1 = *p >> 2;
			c2 = (*p << 4) & 060 | (p[1] >> 4) & 017;
			c3 = (p[1] << 2) & 074 | (p[2] >> 6) & 03;
			c4 = p[2] & 077;
			putc(ENC(c1), out);
			putc(ENC(c2), out);
			putc(ENC(c3), out);
			putc(ENC(c4), out);
		}
		putc('\n', out);
		if( n <= 0 )
			break;
	} while( n > 0 );
}

#ifdef M_SYSV
#define rindex  strrchr
#endif
extern char *rindex();

/*
 * Change the name of user to uu-decode on the remote machine
 */
char *changeuser(name)
	char *name;
{
	register char *p, *q;
	static char buf[300];
	char c;

	p = name;
	/* Leftside delimitors */
	if( (q = rindex(p, '!')) != NULL ) p = q+1;
	if( (q = rindex(p, ':')) != NULL ) p = q+1;
	c = *p;
	*p = '\0';
	strcpy(buf, name);      /* Left part of the host name */
	*p = c;

	strcat(buf, "uu-decode");       /* User name */

	/* Rightside delimitors */
	while( *p && *p != '%' && *p != '@' )
		p++;
	strcat(buf, p);         /* Right part of the host name */
	if( debug )
		fprintf(stderr, "SEND MAIL TO: '%s'\n", buf);
	return buf;
}

/*
 * Extract the name of user from the network address
 */
char *exuser(name)
	char *name;
{
	register char *p, *q;
	static char buf[80];
	char c;

	p = name;
	/* Leftside delimitors */
	if( (q = rindex(p, '!')) != NULL ) p = q+1;
	if( (q = rindex(p, ':')) != NULL ) p = q+1;

	/* Rightside delimitors */
	q = p;
	while( *q && *q != '%' && *q != '@' )
		q++;

	c = *q;
	*q = '\0';
	strcpy(buf, p);
	*q = c;

	if( debug )
		fprintf(stderr, "REMOTE USER: '%s'\n", buf);
	return buf;
}
