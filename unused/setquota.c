# include	<sys/param.h>
# include	<sys/inode.h>
# include	<sys/stat.h>
# include	<sys/qstat.h>

struct stat	sbuf;
struct qstat	qsbuf;
int	start;
int	fdir;
int	ldir;
daddr_t	nused;
daddr_t	nquot;
char	dotq[]	".q";
char	dot[] ".";
char	dotdot[] "..";
char	dummy1[] ".1.1.1";
char	dummy2[] ".2.2.2";
char	dummy3[] ".3.3.3";
extern daddr_t	atol();

main(argc, argv)
char *argv[];
{
	register i, j;
	int flag;

	fdir = 1;
	start = 0;
	for(i=1; i<argc; i++) {
		start = 1;
		if(isdir(argv[i]))
			continue;
		ldir = i;
		if(fdir >= ldir) {
			printf("no directories specified\n");
			exit(1);
		}
		if(!isnumb(argv[i])) {
			printf("%s not a directory\n", argv[i]);
			exit(1);
		}
		nused = 0;
		flag = 1;
		if(i+1 < argc && isnumb(argv[i+1])) {
			nused = atol(argv[i]);
			flag = 0;
			i++;
		}
		nquot = atol(argv[i]);
		for(j=fdir; j<ldir; j++)
			setq(argv[j], flag, nused, nquot);
		fdir = i;
		start = 0;
	}
	if(start) {
		printf("no quota specified\n");
		exit(1);
	}
	exit(0);
}

isdir(s)
char *s;
{

	if(stat(s, &sbuf) < 0)
		return(0);
	if((sbuf.st_mode&S_IFMT)==S_IFDIR)
		return(1);
	return(0);
}

isnumb(s)
char *s;
{
	register c;

	while(c = *s++)
		if(c < '0' || c > '9')
			return(0);
	return(1);
}

setq(dir, flg, n, m)
char *dir;
int flg;
daddr_t n, m;
{
	register i;
	int count, f, junk[25];

	if(i=fork()) {
		while(wait(junk) != -1)
			;
		return;
	}
	if(chdir(dir) < 0) {
		printf("chdir:");
		perror(dir);
		exit(1);
	}
	count = 0;

loop:
	if(count > 2) {
		printf("attempts fail\n");
		exit(1);
	}
	count++;
	if(qstat(dotq, &qsbuf) >= 0) {
		if((qsbuf.qs_mode&S_IFMT) != S_IFQUOT) {
			unlink(dotq);
			goto loop;
		}
		if(flg==1)
			n=qsbuf.qs_un.qs_qused;
		if(quota(dotq, fsbtodb(n), fsbtodb(m)) < 0) {
			printf("quota:");
			perror(dotq);
			exit(1);
		}
		exit(0);
	}
	unlink(dummy1);
	f = open(dot, 0);
	if(f < 0) {
		printf("open:");
		perror(dir);
		exit(1);
	}
	for(i=0; i<sizeof junk / sizeof (int); i++)
		junk[i] = 0;
	read(f, junk, 48);
	close(f);
	for(i=0; i<24; i=+8) {
		if(junk[i] == 0) {
			if (mknod(dotq, S_IFQUOT, 0) < 0) {
				perror("mknod:1");
				exit(1);
			}
			goto slot;
		}
		if(junk[i+1] == '.')
			continue;
		if(junk[i+1] == '..' && junk[i+2] == 0)
			continue;
		junk[i+8] = 0;
		link(junk+i+1, dummy1);
		unlink(junk+i+1);
		if (mknod(dotq, S_IFQUOT, 0) < 0) {
			perror("mknod:2");
			exit(1);
		}
		link(dummy1, junk+i+1);
		goto slot;
	}
	printf("first 3 slots contain . and ..\n");
	exit(1);

slot:
	i = open(dotdot, 0);	/* keep .. referenced during procedure */
	unlink(dummy1);
	unlink(dummy2);
	unlink(dummy3);
	link(dotq, dummy1);
	link(dot, dummy2);
	link(dotdot, dummy3);
	unlink(dotq);
	unlink(dot);
	unlink(dotdot);
	link(dummy1, dotq);
	link(dummy2, dot);
	link(dummy3, dotdot);
	unlink(dummy1);
	unlink(dummy2);
	unlink(dummy3);
	close(i);
	goto loop;
}
