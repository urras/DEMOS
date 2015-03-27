/*
 * UU-DECODE - acceptor of mto's mail messages
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
 * Flags:       -r reply_to     - reply an error
 *
 * Calls:       localmail       - local mailer for delivering
 *                                arrival reports
 *              mail            - mailer for delivering error messages
 *
 * $Log:	uu-decode.c,v $
 * Revision 1.5  90/08/26  14:59:51  avg
 * uuto -> mto
 * 
 * Revision 1.4  90/08/10  20:10:26  avg
 * Changed location of localmail.
 * 
 * Revision 1.3  90/08/08  15:32:25  avg
 * More Security.
 * 
 * Revision 1.2  90/08/02  17:47:03  avg
 * *** empty log message ***
 * 
 * Revision 1.1  90/08/01  21:46:20  avg
 * Initial revision
 * 
 */

#ifndef lint
static char rcsid[] = "$Header: uu-decode.c,v 1.5 90/08/26 14:59:51 avg Exp $";
#endif

# include "../Dispose.h"
#define LOCALMAIL       U_LOCAL
#define MAIL            U_MAIL
#ifdef DEMOS
#define RCVDIR          "/usr/local/spool/uucppublic/rcv"
#else
#define RCVDIR          "/usr/spool/uucppublic/rcv"
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <sysexits.h>

/* single character decode */
#define DEC(c)	(((c) - ' ') & 077)

#ifdef M_SYSV
#define index strchr
#endif
char *index(), *mktemp();

FILE *Fcreate();

main(argc, argv)
char **argv;
{
	FILE *out, *mtemp;
	int  mode;
	char dest[300];
	char fname[250];
	char buf[260];
	char *reply_to;
	char *tempname, *ctime();
	char *file;
	struct passwd *getpwnam();
	struct passwd *user;
	int  inheader, subj;
	long t;
	char *ers;
	int   erc;

	switch( argc ) {
	    case 1:
		reply_to = NULL;
		break;
	    case 2:
		if( argv[1][0] != '-' || argv[1][1] != 'r' ||
		    argv[1][2] == '\0' )
			goto Usage;
		reply_to = &argv[1][2];
		break;
	    case 3:
		if( strcmp(argv[1], "-r") )
			goto Usage;
		reply_to = argv[2];
		break;
	    default:
	    Usage:
		fprintf(stderr, "Usage: %s [-r reply_to] < msg\n", argv[0]);
		exit(EX_USAGE);
	}

	/* Some protection */
	setuid(0);
	umask(077);

	/*
	 * Read message header lines into temp. file
	 */
	tempname = mktemp("/tmp/uudecXXXXXX");
	mtemp = fopen(tempname, "w");
	if( mtemp == NULL )
		exit(EX_TEMPFAIL);

	inheader = 2;
	subj = 0;
	fname[0] = '\0';
	while( fgets(buf, sizeof buf, stdin) != NULL ) {
		if( inheader == 2 ) {
			inheader = 1;
			if( !strncmp(buf, "From ", 5) )
				continue;
		}
		if( inheader ) {
			if( buf[0] == '\n' || buf[0] == '\r' ) {
				inheader = 0;
			} else {
				fputs(buf, mtemp);
				if( !strncmp(buf, "Subject:", 8) )
					subj++;
			}
		} else if( !strncmp(buf, "begin ", 6) )
			goto HdrFound;
	}
	ers = "No uuencode header line found in your message,\ncheck your software.\n";
	erc = EX_DATAERR;

	/*
	 * Report an error back to the user
	 */
ErrMail:
	fclose(mtemp);
	unlink(tempname);
	if( reply_to == NULL )
		exit(erc);
	sprintf(buf, "%s -s 'receiption failed' '%s'", MAIL, reply_to);
	setuid(getuid());
	if( (mtemp = popen(buf, "w")) == NULL )
		exit(erc);
	if( fname[0] != '\0' )
		fprintf(mtemp, "Troubles receiving file %s:\n", fname);
	fputs(ers, mtemp);
	if( pclose(mtemp) )
		exit(erc);
	exit(0);

	/*
	 * Parse header line
	 */
HdrFound:
	sscanf(buf, "begin %o %s", &mode, dest);
	if (dest[0] == '~') {
		file = index(dest, '/');
		if( file == NULL ) {
			erc = EX_DATAERR;
			ers = "Illegal format of ~user/file.\n";
			goto ErrMail;
		}
		*file++ = 0;
		user = getpwnam(dest+1);
		if( user == NULL ) {
			static char erb[80];

			erc = EX_NOUSER;
			sprintf(erb, "No shuch user as %s\n", dest+1);
			ers = erb;
			strcpy(buf, file);      /* Save filename */
			strcpy(dest, buf);
			goto ErrMail;
		}
	} else {
		static struct passwd anybody;

		anybody.pw_uid = 9999;
		anybody.pw_gid = 9999;
		anybody.pw_name = "ANYBODY";
		user = &anybody;
		file = dest;
		mode |= 0444;
	}

	/* Remove "/", ".", ".." at the beginning */
	for(;;) {
		if( *file == '/' )
			file++;
		else if( *file == '.' && file[1] == '/' )
			file += 2;
		else if( *file == '.' && file[1] == '.' && file[2] == '/' )
			file += 3;
		else
			break;
	}

	/*
	 * Create output file
	 */
	strcpy(fname, file);
	sprintf(dest, "%s/%s/%s", RCVDIR, user->pw_name, fname);
	if( (out = Fcreate(dest, user->pw_uid, user->pw_gid, mode)) == NULL ) {
		erc = EX_CANTCREAT;
		ers = "Cannot create spool file (may be this file already exist), sorry.\n";
		goto ErrMail;
	}

	/*
	 * Gogo machine
	 */
	if( decode(stdin, out) ) {
		fclose(out);
		unlink(dest);
		erc = EX_DATAERR;
		ers = "Data format error (file too short).\n";
		goto ErrMail;
	}

	fclose(out);

	/*
	 * Check that file is complete
	 */
	if( fgets(buf, sizeof buf, stdin) == NULL || strcmp(buf, "end\n") ) {
		unlink(dest);
		erc = EX_DATAERR;
		ers = "Received message is incomplete -- file discarded.\n";
		goto ErrMail;
	}

	/*
	 * Notice user about arrival of file
	 */
	if( !strcmp(user->pw_name, "ANYBODY") ) {
		unlink(tempname);
		exit(0);
	}
	if( !subj )
		fprintf(mtemp, "Subject: file arrival\n");
	fprintf(mtemp, "\nFile arrived: %s\n", fname);
	fprintf(mtemp, "Use mpick to retrieve it from the spool area.\n");
	fclose(mtemp);
	sprintf(buf, "%s %s < %s", LOCALMAIL, user->pw_name, tempname);
	setuid(getuid());
	system(buf);
	unlink(tempname);
	exit(0);
}

/*
 * Copy from in to out, decoding as you go along.
 */
decode(in, out)
FILE *in;
FILE *out;
{
	char buf[80];
	register char *bp;
	int n;
	int c1, c2, c3;

	for(;;) {
		if( fgets(buf, sizeof buf, in) == NULL )
			return 1;
		n = DEC(buf[0]);
		if( n <= 0 )
			break;

		bp = &buf[1];
		while( n > 0 ) {
			c1 = DEC(*bp) << 2 | DEC(bp[1]) >> 4;
			c2 = DEC(bp[1]) << 4 | DEC(bp[2]) >> 2;
			c3 = DEC(bp[2]) << 6 | DEC(bp[3]);
			if( n >= 1 ) putc(c1, out);
			if( n >= 2 ) putc(c2, out);
			if( n >= 3 ) putc(c3, out);
			bp += 4;
			n -= 3;
		}
	}
	return 0;
}

/*
 * Safe file creation
 */
FILE *Fcreate(name, uid, gid, mode)
	char *name;
	int uid, gid, mode;
{
	struct stat st;
	register char *p;
	int status;
	FILE *of;
	char dn[300];

	if( stat(name, &st) != -1 )
		return NULL;    /* file already exist */

	/*
	 * Assure that all directories exist
	 */
	p = name;     /* Skip leading / */
	for(;;) {
		p++;
		while( *p != '/' && *p != '\0' ) p++;
		if( *p == '\0' )
			break;
		strncpy(dn, name, p-name);
		dn[p-name] = '\0';

		if( stat(dn, &st) == -1 ) {

			/* make new directory */
			if( fork() == 0 ) {
				execl("/bin/mkdir", "mkdir", dn, 0);
				exit(1);
			}
			status = -1;
			wait(&status);
			if( status != 0 )
				return NULL;
			if( !strcmp(dn, RCVDIR) ) {
				if( chown(dn, 0, 0) == -1 ||
				    chmod(dn, 0755) == -1 )
					return NULL;
			} else {
				if( chown(dn, uid, gid) == -1 ||
				    chmod(dn, (uid==9999 && gid==9999)? 0777 : 0700) == -1 )
					return NULL;
			}

		} else if( (st.st_mode & S_IFMT) != S_IFDIR )
			return NULL;
	}

	/*
	 * Directories are OK -- create a file
	 */
	if( (of = fopen(name, "w")) == NULL )
		return NULL;
	mode &= 0777;
	if( chown(name, uid, gid) == -1 || chmod(name, mode) == -1 ) {
		fclose(of);
		return NULL;
	}
	return of;
}
