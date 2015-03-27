#ifndef lint
static char rcsid[] = "$Header: mailx.c 1.11 1990/11/04 01:38:33 Exp $";
#endif

/*
 * mail - PROSTOJ WARIANT OBRABOTKI PO^TY
 *
 * $Log: mail.c,v $
 * Revision 1.11  1990/11/04  01:38:33  ache
 * #endif corrected
 *
 * Revision 1.10  90/09/22  20:25:51  avg
 * dOBAWLENY POWTORY PRI BLOKIROWKE.
 * 
 * Revision 1.9  90/09/21  21:59:14  ache
 * MS-DOS extends + some new stuff
 * 
 * Revision 1.8  90/08/06  16:02:01  avg
 * wYREZANA PE^ATX SODERVIMOGO P/Q]IKA.
 * 
 * Revision 1.7  90/06/23  21:28:57  avg
 * dOBAWLEN SPECIALXNYJ ADRESAT DLQ RASPROSTRANENIQ MAR[RUTOW.
 * 
 * Revision 1.6  90/06/12  17:43:56  avg
 * zAPRE]ENA ZAPISX dead.letter W SLU^AE KOGDA UKAZAN KL@^ -r.
 * 
 * Revision 1.5  90/06/07  14:07:17  avg
 * pRAWLENY KODY WOZWRATA.
 * 
 * Revision 1.4  90/04/20  19:17:05  avg
 * pRIKRU^ENO POD System V
 * 
 * Revision 1.3  88/06/23  16:08:12  avg
 * tEPERX DLQ POISKA IMENI POLXZOWATELQ ISPOLXZUETSQ
 * (W PERWU@ O^EREDX) getlogin.
 * kL@^ -f TEPERX RABOTAET TAK, KAK EMU PREDPISANO bOGOM.
 * 
 * Revision 1.2  88/01/11  12:28:45  avg
 * tEPERX PRAWILXNO OBRABATYWAETSQ KL@^ -r,
 * DATA W SOOB]ENII wsegda ZAPISYWAETSQ W ANGL. FORMATE.
 *
 * Revision 1.1  88/01/04  19:57:16  avg
 * Initial revision
 *
 * Revision 1.1  86/11/23  23:01:54  alex
 * ---
 *
 */

#include <stdio.h>
#include <pwd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <setjmp.h>
#include <utmp.h>
#include <sysexits.h>
/* #include <whoami.h> */

/* #define PRINTMAIL    /* Allow printing mail */

#define ediag(e,r)      ((e))
#define EDIAG_R 0
#define EDIAG_E 1
int     _ediag = EDIAG_E;

#define MSGS            /* pipe mail to "msgs" to /usr/ucb/msgs */
/*copylet flags */
/*remote mail, add rmtmsg */
#define REMOTE  1
	/* zap header and trailing empty line */
#define ZAP     3
#define ORDINARY 2
#define FORWARD 4
#define LSIZE   256
#define MAXLET  300     /* maximum number of letters */
#define MAILMODE (~0600)                /* mode of created mail */

char    line[LSIZE];
char    resp[LSIZE];
struct let {
	long    adr;
	char    change;
} let[MAXLET];
int     nlet    = 0;
char    lfil[50];
long    iop, time();
char    lettmp[] = "/tmp/maXXXXX";
char    maildir[] = "/usr/spool/mail/";
char    mailfile[] = "/usr/spool/mail/xxxxxxxxxxxxxxxxxxxxxxx";
char    dead[] = "dead.letter";
char    forwmsg[] = " forwarded\n";
char    *curlock;
int     lockerror;
FILE    *tmpf;
FILE    *malf;
char    *my_name;
struct  passwd  *getpwuid();
char            *getlogin();
int     error;
int     locked;
int     changed;
int     forward;
char    from[] = "From ";
long    ftell();
int     delete();
char    *ctime();
int     flgf;
int     flgp;
int     delflg = 1;
jmp_buf sjbuf;
char    hostname[32];
char   *rflag;

#ifdef M_SYSV
#define index strchr
#include <sys/utsname.h>
struct utsname utsname;
#endif

main(argc, argv)
char **argv;
{
	register i;
	char sobuf[BUFSIZ];
	char **vv;

	setbuf(stdout, sobuf);
	mktemp(lettmp);
	unlink(lettmp);
	if (my_name == NULL) {
		struct passwd *pwent;

		if( (my_name = getlogin()) == NULL ) {
			pwent = getpwuid(getuid());
			if (pwent==NULL)
			       my_name = "???";
			else
			       my_name = pwent->pw_name;
		}
	}
	if(setjmp(sjbuf)) done();
	for (i = SIGHUP; i <= SIGTERM; i++)
		setsig(i, delete);
	tmpf = fopen(lettmp, "w");
	if (tmpf == NULL) {
		fprintf(stderr, ediag("mail: cannot open %s for writing\n",
				      "mail: NE MOGU OTKRYTX %s DLQ ZAPISI\n"), lettmp);
		done();
	}
	if (argv[0][0] == 'r')
		sendmail(argc, argv);
	else {
		for( vv = &argv[1] ; *vv ; vv++ ) {
			if( **vv != '-' ) {
				sendmail(argc, argv);
				done();
			} else if( (*vv)[1] == 'f' )
				vv++;
		}
#ifdef PRINTMAIL
		printmail(argc, argv);
#else
		error = EX_USAGE;
		fprintf(stderr, "localmail: send mode only\n");
#endif
	}
	done();
}

setsig(i, f)
int i;
int (*f)();
{
	if(signal(i, SIG_IGN) != SIG_IGN)
		signal(i, f);
}

#ifdef PRINTMAIL
printmail(argc, argv)
char **argv;
{
	int flg, i, j, print;
	char *p, *getarg();
	char *zz;

	setuid(getuid());
	cat(mailfile, maildir, my_name);
	for (; argc>1; argv++, argc--) {
		if (argv[1][0]=='-') {
			zz = &argv[1][1];
		   Next:
			switch( *zz++ ) {
			case 'q':
				delflg = 0;
				goto Next;
			case 'p':
				flgp++;
				delflg = 0;
				goto Next;
			case 'f':
				if (argc>=3) {
					strcpy(mailfile, argv[2]);
					argv++;
					argc--;
				}
				goto Next;
			case 'r':
				forward = 1;
				goto Next;
			default:
				fprintf(stderr, ediag("mail: unknown option %c\n","mail: NEIZWESTNYJ KL@^ %c\n"), argv[1][1]);
				done();
			case '\0':
				break;
			}
		} else
			break;
	}
	malf = fopen(mailfile, "r");
	if (malf == NULL) {
		fprintf(stdout, ediag("No mail.\n","nET PO^TY.\n"));
		return;
	}
	lock(mailfile);
	copymt(malf, tmpf);
	fclose(malf);
	fclose(tmpf);
	unlock();
	tmpf = fopen(lettmp, "r");

	changed = 0;
	print = 1;
	for (i = 0; i < nlet; ) {
		j = forward ? i : nlet - i - 1;
		if(setjmp(sjbuf)) {
			print=0;
		} else {
			if (print)
				copylet(j, stdout, ORDINARY);
			print = 1;
		}
		if (flgp) {
			i++;
			continue;
		}
		setjmp(sjbuf);
		fprintf(stdout, "? ");
		fflush(stdout);
		if (fgets(resp, LSIZE, stdin) == NULL)
			break;
		switch (resp[0]) {

		default:
			fprintf(stderr, ediag("usage\n","iSPOLXZOWANIE:\n"));
		case '?':
			print = 0;
			fprintf(stderr, ediag("q\tquit\n","q\tWYHOD\n"));
			fprintf(stderr, ediag("x\texit without changing mail\n","x\tWYHOD BEZ IZMENENIQ PO^TY\n"));
			fprintf(stderr, ediag("p\tprint\n","p\tPE^ATX\n"));
			fprintf(stderr, ediag("s[file]\tsave (default mbox)\n","s[file]\tZAPISATX (PO UMOL^ANI@ mbox)\n"));
			fprintf(stderr, ediag("w[file]\tsame without header\n","w[file]\tTO VE BEZ ZAGOLOWKA\n"));
			fprintf(stderr, ediag("-\tprint previous\n","-\tPE^ATATX PREDYDU]EE\n"));
			fprintf(stderr, ediag("d\tdelete\n","d\tISKL@^ITX\n"));
			fprintf(stderr, ediag("+\tnext (no delete)\n","+\tSLEDU@]EE (NE ISKL@^ATX)\n"));
			fprintf(stderr, ediag("m user\tmail to user\n","m user\tPOSLATX PO^TU DLQ user\n"));
			fprintf(stderr, ediag("! cmd\texecute cmd\n","! cmd\tWYPOLNITX KOMANDU cmd\n"));
			break;

		case '+':
		case 'n':
		case '\n':
			i++;
			break;
		case 'x':
			changed = 0;
		case 'q':
			goto donep;
		case 'p':
			break;
		case '^':
		case '-':
			if (--i < 0)
				i = 0;
			break;
		case 'y':
		case 'w':
		case 's':
			flg = 0;
			if (resp[1] != '\n' && resp[1] != ' ') {
				printf(ediag("illegal\n","NEWERNO\n"));
				flg++;
				print = 0;
				continue;
			}
			if (resp[1] == '\n' || resp[1] == '\0')
				cat(resp+1, "mbox", "");
			for (p = resp+1; (p = getarg(lfil, p)) != NULL; ) {
				malf = fopen(lfil, "a");
				if (malf == NULL) {
					fprintf(stdout, ediag("mail: cannot append to %s\n","mail: NE MOGU DOPISATX W %s\n"), lfil);
					flg++;
					continue;
				}
				copylet(j, malf, resp[0]=='w'? ZAP: ORDINARY);
				fclose(malf);
			}
			if (flg)
				print = 0;
			else {
				let[j].change = 'd';
				changed++;
				i++;
			}
			break;
		case 'm':
			flg = 0;
			if (resp[1] == '\n' || resp[1] == '\0') {
				i++;
				continue;
			}
			if (resp[1] != ' ') {
				printf(ediag("invalid command\n","O[IBO^NAQ KOMANDA\n"));
				flg++;
				print = 0;
				continue;
			}
			for (p = resp+1; (p = getarg(lfil, p)) != NULL; )
				if (!sendrmt(j, lfil))  /* couldn't send it */
					flg++;
			if (flg)
				print = 0;
			else {
				let[j].change = 'd';
				changed++;
				i++;
			}
			break;
		case '!':
			system(resp+1);
			printf("!\n");
			print = 0;
			break;
		case 'd':
			let[j].change = 'd';
			changed++;
			i++;
			if (resp[1] == 'q')
				goto donep;
			break;
		}
	}
   donep:
	if (changed)
		copyback();
}

copyback()      /* copy temp or whatever back to /usr/spool/mail */
{
	register i, n, c;
	int new = 0;
	struct stat stbuf;

	signal(SIGINT, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	lock(mailfile);
	stat(mailfile, &stbuf);
	if (stbuf.st_size != let[nlet].adr) {   /* new mail has arrived */
		malf = fopen(mailfile, "r");
		if (malf == NULL) {
			fprintf(stdout, ediag("mail: can't re-read %s\n","mail: NE MOGU PRO^ITATX IZ %s\n"), mailfile);
			done();
		}
		fseek(malf, let[nlet].adr, 0);
		fclose(tmpf);
		tmpf = fopen(lettmp, "a");
		fseek(tmpf, let[nlet].adr, 0);
		while ((c = fgetc(malf)) != EOF)
			fputc(c, tmpf);
		fclose(malf);
		fclose(tmpf);
		tmpf = fopen(lettmp, "r");
		let[++nlet].adr = stbuf.st_size;
		new = 1;
	}
	malf = fopen(mailfile, "w");
	if (malf == NULL) {
		fprintf(stderr, ediag("mail:  can't rewrite %s\n","mail:  NE MOGU ZAPISATX W %s\n"), lfil);
		done();
	}
	n = 0;
	for (i = 0; i < nlet; i++)
		if (let[i].change != 'd') {
			copylet(i, malf, ORDINARY);
			n++;
		}
	fclose(malf);
	if (new)
		fprintf(stdout, ediag("new mail arrived\n","POQWILASX NOWAQ PO^TA\n"));
	unlock();
}

copymt(f1, f2)  /* copy mail (f1) to temp (f2) */
FILE *f1, *f2;
{
	long nextadr;

	nlet = nextadr = 0;
	let[0].adr = 0;
	while (fgets(line, LSIZE, f1) != NULL) {
		if (isfrom(line))
			let[nlet++].adr = nextadr;
		nextadr += strlen(line);
		fputs(line, f2);
	}
	let[nlet].adr = nextadr;        /* last plus 1 */
}
#endif /* PRINTMAIL */

copylet(n, f, type) FILE *f;
{       int ch;
	long k;
	fseek(tmpf, let[n].adr, 0);
	k = let[n+1].adr - let[n].adr;
	while(k-- > 1L && (ch=fgetc(tmpf))!='\n')
		if(type!=ZAP) fputc(ch,f);
	if(type==REMOTE) {
#ifdef M_SYSV
		uname(&utsname);
		strcpy(hostname, utsname.nodename);
#else
		gethostname(hostname, sizeof (hostname));
#endif
		fprintf(f, " remote from %s\n", hostname);
	}
	else if (type==FORWARD)
		fprintf(f, forwmsg);
	else if(type==ORDINARY)
		fputc(ch,f);
	while(k-->1L)
		fputc(ch=fgetc(tmpf), f);
	if(type!=ZAP || ch!= '\n')
		fputc(fgetc(tmpf), f);
}

isfrom(lp)
register char *lp;
{
	register char *p;

	for (p = from; *p; )
		if (*lp++ != *p++)
			return(0);
	return(1);
}

sendmail(argc, argv)
char **argv;
{
	int sed;

	if( !strcmp(*++argv, "-r") ) {
		rflag = *++argv;
		argc -= 2;
	} else
		--argv;
	time(&iop);
	sed = _ediag;
	_ediag = 1;     /* Allow English Date */
	fprintf(tmpf, "%s%s %s", from, rflag? rflag : my_name, ctime(&iop));
	_ediag = sed;
	iop = ftell(tmpf);
	flgf = 1;
	while (fgets(line, LSIZE, stdin) != NULL) {
		if (line[0] == '.' && line[1] == '\n')
			break;
		if (isfrom(line))
			fputs(">", tmpf);
		fputs(line, tmpf);
		flgf = 0;
	}
	fputs("\n", tmpf);
	nlet = 1;
	let[0].adr = 0;
	let[1].adr = ftell(tmpf);
	fclose(tmpf);
	if (flgf)
		return;
	tmpf = fopen(lettmp, "r");
	if (tmpf == NULL) {
		fprintf(stderr, ediag("mail: cannot reopen %s for reading\n",
				      "mail: NE MOGU POWTORNO OTKRYTX %s DLQ ^TENIQ\n"), lettmp);
		return;
	}
	while (--argc > 0)
		if (!send(0, *++argv))  /* couldn't send to him */
			error = EX_NOUSER;
	if( error && !rflag ) {
		if( safefile(dead) ) {
			setuid(getuid());
			malf = fopen(dead, "w");
			if (malf == NULL) {
				fprintf(stdout, ediag("Mail lost...\n","pISXMO POTERQNO...\n"));
			} else {
				copylet(0, malf, ZAP);
				fclose(malf);
				fprintf(stdout, ediag("Mail saved in %s\n","pISXMO SPASENO W %s\n"), dead);
			}
		} else
			error = 0;      /* Message saved successfully */
	}
	fclose(tmpf);
}

sendrmt(n, name)
char *name;
{
	FILE *rmf, *popen();
	register char *p;
	char rsys[64], cmd[64];
	register local, pid;
	int ret, sts;

	local = 0;
	if (*name=='!')
		name++;
	for(p=rsys; *name!='!'; *p++ = *name++)
		if (*name=='\0') {
			local++;
			break;
		}
	if ((pid = fork()) == -1) {
		fprintf(stderr, ediag("mail: can't create proc for remote\n","mail:  NE MOGU SOZDATX PRO^ESS DLQ UDALENNOGO DOSTUPA\n"));
		return(0);
	}
	if (pid) {
		while ((ret = wait(&sts)) != pid) {
			if (ret == -1)
				return(0);
		}
		return(!sts);
	}
	setuid(getuid());
	if (local)
		sprintf(cmd, "mail %s", rsys);
	else {
		if (index(name+1, '!'))
			sprintf(cmd, "uux - %s!rmail \\(%s\\)", rsys, name+1);
		else
			sprintf(cmd, "uux - %s!rmail %s", rsys, name+1);
	}
	if ((rmf=popen(cmd, "w")) == NULL)
		exit(EX_SOFTWARE);
	copylet(n, rmf, local?  FORWARD : REMOTE);
	exit(pclose(rmf)? EX_UNAVAILABLE : 0);
}

send(n, name)   /* send letter n to name */
int n;
char *name;
{
	char file[50];
	register char *p;
	register mask;
	struct passwd *pw, *getpwnam();
	struct stat statb;
	char username[50];
#if defined(MSGS)
	FILE *pip;
	char line[256];
	int count;
#endif

	for (p = name; *p != '!' && *p != '^' && *p != '\0'; p++)
		;
	if (*p == '!' || *p == '^')
		return(sendrmt(n, name));
#ifdef MSGS
/*
 * modification to call 'msgs' program when the 'recipient'
 * is "msgs".  Assumption is that 'n' is 0, no REMOTE, FORWARD, etc.
 * pipe the letter to "msgs -s"
 */
	if(!strcmp(name,"msgs"))        /* this letter is really a mesg */
	{
	    pip = popen("/usr/bin/msgs -s", "w");
	    if (pip == NULL)
		return(0);
	    fseek(tmpf,0L,0);
	    count = 0;
	    while(fgets(line,256,tmpf) != NULL)
	    {
		count++;
		if(count == 2)
		{
		    fprintf(pip,"To: msgs\n");
 /*
  * check 2nd line for a  'Mail'-added "To:" line....
  * we don't want to add 2 "To:"'s
  */
		    if(!strncmp(line,"To: msgs",8))
			continue;
		}
		fputs(line,pip);
	    }
	    pclose(pip);
	    return(1);
	}
#endif
	strcpy(username, name);
	for(p = username ; *p ; p++)
		if( 'A' <= *p && *p <= 'Z' )
			*p |= 040;              /* lower name*/
	if ((pw = getpwnam(username)) == NULL) {
		fprintf(stdout, ediag("mail: can't send to %s\n","mail: NE MOGU POSLATX PO^TU DLQ %s\n"), name);
		return 0;
	}
	cat(file, maildir, username);
	if (stat(file, &statb) >= 0 && (statb.st_mode & S_IFMT) == S_IFDIR) {
		strcat(file, "/");
		strcat(file, username);
	}
	mask = umask(MAILMODE);
	if (!safefile(file))
		return 0 ;
	lock(file);
	malf = fopen(file, "a");
	umask(mask);
	if (malf == NULL) {
		unlock();
		fprintf(stdout, ediag("mail: cannot append to %s\n","mail:  NE MOGU DOPISATX W %s\n"), file);
		return 0;
	}
	chown(file, pw->pw_uid, pw->pw_gid);
	copylet(n, malf, ORDINARY);
	fclose(malf);
	unlock();
	return 1;
}

delete(i)
{
	setsig(i, delete);
	fprintf(stderr, "\n");
	if(delflg)
		longjmp(sjbuf, 1);
	done();
}

done()
{
	if(!lockerror)
		unlock();
	unlink(lettmp);
	exit(lockerror? EX_TEMPFAIL : error);
}

lock(file)
char *file;
{
	struct stat stbuf;
	int trycnt;

	if (locked || flgf)
		return;
	trycnt = 0;
checkagain:
	if (stat(file, &stbuf)<0)
		return;
	if (stbuf.st_mode&01) {         /* user x bit is the lock */
		if (stbuf.st_ctime+60 >= time((long *)0)) {
			if( trycnt++ < 12 ) {
				sleep(5);
				goto checkagain;
			}
			fprintf(stderr, ediag("%s busy; try again in a minute\n","%s ZANQTO; POWTORITE POPYTKU ^UTX POZVE\n"), file);
			lockerror++;
			done();
		}
	}
	locked = stbuf.st_mode & ~01;
	curlock = file;
	chmod(file, stbuf.st_mode|01);
}

unlock()
{
	if (locked)
		chmod(curlock, locked);
	locked = 0;
}

cat(to, from1, from2)
char *to, *from1, *from2;
{
	int i, j;

	j = 0;
	for (i=0; from1[i]; i++)
		to[j++] = from1[i];
	for (i=0; from2[i]; i++)
		to[j++] = from2[i];
	to[j] = 0;
}

char *getarg(s, p)      /* copy p... into s, update p */
register char *s, *p;
{
	while (*p == ' ' || *p == '\t')
		p++;
	if (*p == '\n' || *p == '\0')
		return(NULL);
	while (*p != ' ' && *p != '\t' && *p != '\n' && *p != '\0')
		*s++ = *p++;
	*s = '\0';
	return(p);
}

safefile(f)
	char *f;
{
	struct stat statb;

#ifdef  UCB_SYMLINKS
	if (lstat(f, &statb) < 0)
#else
	if (stat(f, &statb) < 0)
#endif
		return(1);
	if (statb.st_nlink != 1
#ifdef  UCB_SYMLINKS
	|| (statb.st_mode & S_IFMT) == S_IFLNK
#endif
						) {
		fprintf(stderr, ediag("mail:  %s has more than one link or is a symbolic link\n","mail:  %s IMEET BOLEE ODNOJ SWQZI ILI symlink\n"), f);
		return(0);
	}
	return(1);
}
