#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <signal.h>

/*
 * xstr - extract and hash strings in a C program
 */
#define NOXX "NOXXSTR"          /* Псевдокомментарий - запрет xstr -x */
#define LNOXX 7
#define NOX "NOXSTR"            /* Псевдокомментарий - запрет xstr  */
#define LNOX 6
#define YESX "YESXSTR"          /* Псевдокомментарий - разрешение xstr  */
#define LYESX 7
#define ignore(a)       Ignore((char *) a)
#ifdef DEBUG
unsigned XMEM = 0;
#define ALLOC( x, y ) ( XMEM+=(x)*(y), calloc(x,y) )
#else
#define ALLOC( x, y ) calloc( x, y )
#endif

char    *calloc();
off_t   tellpt;
off_t   hashit();
char    *mktemp();
int     onintr();
char    *savestr();
char    *strcat();
char    *strcpy();
off_t   yankstr();

off_t   mesgpt;
char    *strings;
int noxxfl,noxfl;       /* Флаги запрета */
int     cflg;
int     vflg;
int     xflg;
int     readstd;
int     Stlen;

main(argc, argv)
	int argc;
	char *argv[];
{

	argc--, argv++;
	strings = "strings";
	while (argc > 0 && argv[0][0] == '-') {
		register char *cp = &(*argv++)[1];

		argc--;
		if (*cp == 0) {
			readstd++;
			continue;
		}
		do switch (*cp++) {

		case 'c':
			cflg++;
			continue;

		case 'v':
			vflg++;
			continue;

		case 'x':
			xflg++;
			strings = "Messages";
			continue;

		default:
			fprintf(stderr, "Вызов: xstr [-v] [-c] [-x] [-] [имя...]\n");
		} while (*cp);
	}
	if (signal(SIGINT, SIG_IGN) == SIG_DFL)
		signal(SIGINT, onintr);
	if (cflg || xflg || argc == 0 && !readstd )
		inithash();
	else
		strings = mktemp(savestr("/tmp/xstrXXXXXX"));
	while (readstd || argc > 0) {
		if (freopen("x.c", "w", stdout) == NULL)
			perror("x.c"), exit(1);
		if (!readstd && freopen(argv[0], "r", stdin) == NULL)
			perror(argv[0]), exit(2);
		process("x.c");
		if (readstd == 0)
			argc--, argv++;
		else
			readstd = 0;
	};
	flushsh();
	if (cflg == 0 || xflg)
		xsdotc();
	if (strings[0] == '/' && !xflg)
		ignore(unlink(strings));
#ifdef DEBUG
	fprintf( stderr, "Использовано %u байтов\n", XMEM );
#endif
	exit(0);
}

process(name)
	char *name;
{
	char   *cp;
	char    linebuf[2*BUFSIZ];
	register int c;
	register int incomm = 0;
	char   *lb;
	int     xwork;

	if( !xflg )
		printf("char\txstr[];\n");
	else
		printf("extern char *Xsline();\n" );
	for (;;) {
		lb = linebuf;
Again:
		if (fgets(lb, sizeof linebuf, stdin) == NULL) {
			if (ferror(stdin)) {
				perror(name);
				exit(3);
			}
			break;
		}
		lb += strlen( lb );
		if( *(lb-1) == '\n' && *(lb-2) == '\\' ) {
			lb -= 2;
			goto Again;
		}
		if (linebuf[0] == '#') {
			/* if (linebuf[1] == ' ' && isdigit(linebuf[2]))
				printf("#line%s", &linebuf[1]);
Д. Володин      16.12.84          else   */
				printf("%s", linebuf);
			continue;
		}
		for (cp = linebuf; c = *cp++;) switch (c) {

		case '"':
			if (incomm)
				goto def;
			if (noxfl) {flushstring(&cp); break;};
			if( !xflg )
				printf("(&xstr[%d])", (int) yankstr(&cp));
			else {
				if((xwork = (int) yankstr(&cp)) != -1)
					printf( "(Xsline(%d))", xwork );
				else
					flushstring( &cp );
			}
			break;

		case '\'':
			if (incomm)
				goto def;
			putchar(c);
			if (*cp)
				putchar(*cp++);
			break;

		case '/':
			if (incomm || *cp != '*')
				goto def;
			incomm = 1;
			cp++;
			printf("/*");
			if ((xflg && strncmp(cp,NOXX,LNOXX) == 0)
			   || (!xflg &&strncmp(cp,NOX,LNOX) == 0)) noxfl = 1;
			if (strncmp(cp,YESX,LYESX) == 0) noxfl = 0;
			continue;
		case '*':
			if (incomm && *cp == '/') {
				incomm = 0;
				cp++;
				printf("*/");
				continue;
			}
			goto def;

def:
		default:
			putchar(c);
			break;
		}
	}
	if (ferror(stdout))
		perror("x.c"), onintr();
}

off_t
yankstr(cpp)
	register char **cpp;
{
	register char *cp = *cpp;
	register int c, ch;
	char dbuf[BUFSIZ];
	register char *dp = dbuf;
	register char *tp;

	while (c = *cp++) {
		switch (c) {

		case '"':
			cp++;
			goto out;

		case '\\':
			c = *cp++;
			if (c == 0)
				break;
			if (c == '\n')
				continue;
			for (tp = "b\bt\tr\rn\nf\f\\\\\"\""; ch = *tp++; tp++)
				if (c == ch) {
					c = *tp;
					goto gotc;
				}
			if (!octdigit(c)) {
				*dp++ = '\\';
				break;
			}
			c -= '0';
			if (!octdigit(*cp))
				break;
			c <<= 3, c += *cp++ - '0';
			if (!octdigit(*cp))
				break;
			c <<= 3, c += *cp++ - '0';
			break;
		}
gotc:
		*dp++ = c;
	}
out:
	*dp = 0;
	if( xflg ) {
		Stlen = strlen( dbuf )+1;
		if( Stlen < 12 )
			return( -1 );
	}
	*cpp = --cp;
	return (hashit(dbuf, 1));
}

flushstring( cpp )
char  **cpp;
{
	register char c;

	putchar( '"' );
	while( c = *(*cpp)++ )
		switch( c ) {
		    case '"':
			putchar( c );
			return;
		    case '\\':
			if( (c = *(*cpp)++) == '"' ) {
				printf( "\\\"" );
				break;
			}
			if( c == '\n' )
				continue;
			putchar( '\\' );
		    default:
			putchar( c );
		}
}

octdigit(c)
	char c;
{

	return (isdigit(c) && c != '8' && c != '9');
}

inithash()
{
	char buf[BUFSIZ];
	register FILE *mesgread = fopen(strings, "r");

	if (mesgread == NULL)
		return;
	for (;;) {
		mesgpt = tellpt;
		if (fgetNUL(buf, sizeof buf, mesgread) == NULL)
			break;
		ignore(hashit(buf, 0));
	}
	ignore(fclose(mesgread));
}

fgetNUL(obuf, rmdr, file)
	char *obuf;
	register int rmdr;
	FILE *file;
{
	register c;
	register char *buf = obuf;

	while (--rmdr > 0 && (c = xgetc(file)) != 0 && c != EOF)
		*buf++ = c;
	*buf++ = 0;
	return ((feof(file) || ferror(file)) ? NULL : 1);
}

xgetc(file)
	FILE *file;
{

	tellpt++;
	return (getc(file));
}

#define BUCKETS 128

struct  hash {
	off_t   hpt;
	char    *hstr;
	struct  hash *hnext;
	short   hnew;
} bucket[BUCKETS];

off_t
hashit(str, new)
	char *str;
	int new;
{
	int i;
	register struct hash *hp, *hp0;

	hp = hp0 = &bucket[lastchr(str) & 0177];
	while (hp->hnext) {
		hp = hp->hnext;
		i = istail(str, hp->hstr);
		if (i >= 0)
			return (hp->hpt + i);
	}
	hp = (struct hash *) ALLOC(1, sizeof (*hp));
	if( hp == NULL ) {
		fprintf( stderr, "Нет памяти\n" );
#ifdef DEBUG
		fprintf( stderr, "Использовано %u байтов\n", XMEM );
#endif
		exit( 1 );
	}
	hp->hpt = mesgpt;
	hp->hstr = savestr(str);
	mesgpt += strlen(hp->hstr) + 1;
	hp->hnext = hp0->hnext;
	hp->hnew = new;
	hp0->hnext = hp;
	return (hp->hpt);
}

flushsh()
{
	register int i;
	register struct hash *hp;
	register FILE *mesgwrit;
	register int old = 0, new = 0;

	for (i = 0; i < BUCKETS; i++)
		for (hp = bucket[i].hnext; hp != NULL; hp = hp->hnext)
			if (hp->hnew)
				new++;
			else
				old++;
	if (new == 0 && old != 0)
		return;
	mesgwrit = fopen(strings, old ? "r+" : "w");
	for (i = 0; i < BUCKETS; i++)
		for (hp = bucket[i].hnext; hp != NULL; hp = hp->hnext) {
			found(hp->hnew, hp->hpt, hp->hstr);
			if (hp->hnew) {
				fseek(mesgwrit, hp->hpt, 0);
				ignore(fwrite(hp->hstr, strlen(hp->hstr) + 1, 1, mesgwrit));
				if (ferror(mesgwrit))
					perror(strings), exit(4);
			}
		}
	ignore(fclose(mesgwrit));
}

found(new, off, str)
	int new;
	off_t off;
	char *str;
{
	register char *cp;

	if (vflg == 0)
		return;
	if (!new)
		fprintf(stderr, "найдено с %d:", (int) off);
	else
		fprintf(stderr, "новая   с %d:", (int) off);
	prstr(str);
	fprintf(stderr, "\n");
}

prstr(cp)
	register char *cp;
{
	register int c;

	while( c = ( (*cp++)&0377 ) ) {
		if (c < ' ')
			fprintf(stderr, "^%c", c + '`');
		else if (c == 0177)
			fprintf(stderr, "^?");
		else if ( c < 0300 && c > 0200 )
			fprintf(stderr, "\\%03o", c);
		else
			fprintf(stderr, "%c", c);
	}
}

xsdotc()
{
	register FILE *strf = fopen(strings, "r");
	register FILE *xdotcf;

	if (strf == NULL)
		perror(strings), exit(5);
	xdotcf = fopen("xs.c", "w");
	if (xdotcf == NULL)
		perror("xs.c"), exit(6);
	if( !xflg ) {
		fprintf(xdotcf, "char\txstr[] = {\n");
		for (;;) {
			register int i, c;

			for (i = 0; i < 8; i++) {
				c = getc(strf);
				if (ferror(strf)) {
					perror(strings);
					onintr();
				}
				if (feof(strf)) {
					fprintf(xdotcf, "\n");
					goto out;
				}
				fprintf(xdotcf, "%04o,", c);
			}
			fprintf(xdotcf, "\n");
		}
out:
		fprintf(xdotcf, "};\n");
	} else {
		fprintf(xdotcf, "#ifndef XSFILE\n" );
		fprintf(xdotcf, "# define XSFILE \"Messages\"\n" );
		fprintf(xdotcf, "#endif\n\n" );
		fprintf(xdotcf, "char *Xsline( sh )\n" );
		fprintf(xdotcf, "unsigned sh;\n{\n" );
		fprintf(xdotcf, "\tstatic char Xslin[128];\n\tregister leng;\n" );
		fprintf(xdotcf, "\tstatic Xfd = -1;\n\n" );
		fprintf(xdotcf, "\tif( Xfd < 0 && (Xfd = open(XSFILE, 0))<0)\n" );
		fprintf(xdotcf, "\t\t{ *Xslin = 0; return( Xslin ); }\n" );
		fprintf(xdotcf, "\tlseek( Xfd, (long)sh, 0 );\n" );
		fprintf(xdotcf, "\tXslin[read( Xfd, Xslin, 128 )] = 0;\n" );
		fprintf(xdotcf, "\treturn( Xslin );\n}\n" );
	}
	ignore(fclose(xdotcf));
	ignore(fclose(strf));
}

char *
savestr(cp)
	register char *cp;
{
	register char *dp = (char *) ALLOC(1, strlen(cp) + 1);

	if( dp == NULL ) {
		fprintf( stderr, "Нет памяти\n" );
#ifdef DEBUG
		fprintf( stderr, "Использовано %u байтов\n", XMEM );
#endif
		exit( 1 );
	}
	return (strcpy(dp, cp));
}

Ignore(a)
	char *a;
{

	a = a;
}

ignorf(a)
	int (*a)();
{

	a = a;
}

lastchr(cp)
	register char *cp;
{

	while (cp[0] && cp[1])
		cp++;
	return (*cp);
}

istail(str, of)
	register char *str, *of;
{
	register int d = strlen(of) - strlen(str);

	if (d < 0 || strcmp(&of[d], str) != 0)
		return (-1);
	return (d);
}

onintr()
{

	ignorf(signal(SIGINT, SIG_IGN));
	if (strings[0] == '/')
		ignore(unlink(strings));
	ignore(unlink("x.c"));
	ignore(unlink("xs.c"));
	exit(7);
}
