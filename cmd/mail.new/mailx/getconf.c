# include	"rcv.h"
# include <assert.h>
/*
 * $Log:	getconf.c,v $
 * Revision 1.6  90/12/07  07:14:45  ache
 * Переделана обработка временных файлов и мелочи
 * 
 * Revision 1.5  90/12/03  03:02:11  ache
 * В досе: переходит в домашний каталог, прежде чем позвать sendmail
 * 
 * Revision 1.4  90/11/11  20:06:16  ache
 * Исправлено распознавание имен файлов
 * 
 * Revision 1.3  90/10/04  04:47:22  ache
 * SPOOLDIR removed, UUPC... -> UU...
 * 
 * Revision 1.2  90/09/21  21:58:45  ache
 * MS-DOS extends + some new stuff
 * 
 * Revision 1.1  90/09/13  07:48:06  ache
 * Initial revision
 * 
 */

/*NOXSTR*/
static char rcsid[] = "$Header: getconf.c,v 1.6 90/12/07 07:14:45 ache Exp $";
/*YESXSTR*/
#ifdef	MSDOS
/* From <stdlib.h> */
/* sizes for buffers used by the _makepath() and _splitpath() functions.
 * note that the sizes include space for 0-terminator
 */

#define _MAX_PATH	260	/* max. length of full pathname */
#define MAXPATH _MAX_PATH
#define _MAX_DRIVE	3	/* max. length of drive component */
#define _MAX_DIR	256	/* max. length of path component */
#define _MAX_FNAME	256	/* max. length of file name component */
#define _MAX_EXT	256	/* max. length of extension component */

typedef char boolean;
#define TRUE 1
#define FALSE 0
extern char *strdup();
#ifdef	__TURBOC__
extern char *getcwd();
#else
extern char *_getdcwd();
#endif

static	void  getconfig(FILE*, int);
static void getrcnames();
void mkfilename();

/* the following table contols the configurations files processing */

static struct	Table {
	char*	sym;
	char	must;
	char    sys;
	char	dir;
	char	std;
	char	*suff;
} table[] = {
	"COMSPEC",		FALSE,	FALSE, TRUE, FALSE, NULL,
	"DOMAIN",		TRUE,	TRUE,  FALSE, FALSE, NULL,
	"HOME", 		TRUE,	FALSE, TRUE, FALSE, NULL,
	"MAILDIR",		TRUE,	TRUE,  TRUE, TRUE, "mail\\",
	"MSG",			FALSE,	FALSE,	FALSE, FALSE, NULL,
	"SHELL",		FALSE,	FALSE, TRUE, FALSE, NULL,
	"TMP",			TRUE,	FALSE, TRUE, TRUE, "tmp\\",
	"TZ",			TRUE,	TRUE,  FALSE, FALSE, NULL,
	"USER", 		TRUE,	FALSE, FALSE, FALSE, NULL,
	NOSTR
}; /* table */

char	calldir[_MAX_PATH];

static void AssignDefaultDirs(void)
{
	struct Table*	tptr;
	char buf[_MAX_PATH];

	for (tptr = table; tptr->sym != NULL; tptr++) {
		if (tptr->std && value(tptr->sym) == NULL) {
				mkfilename(buf, calldir, tptr->suff);
				assign(tptr->sym, buf);
		}
	}
}

/*
   getconfig - process a configuration file
*/

static void	getconfig(FILE* fp, int sysmode) {
	struct Table*	tptr;

	for (;;) {
		char	buff[80];
		char*	cp;

		if (fgets(buff, sizeof buff, fp) == NOSTR)
			break;				/* end of file */
		if ((*buff == '\n') || (*buff == '#'))
			continue;			/* comment line */
		if (*(cp = buff + strlen(buff) - 1) == '\n')
			*cp = '\0';
		if ((cp = strchr(buff, '=')) == NOSTR)
			continue;
		*cp++ = '\0';
		for (tptr = table; tptr->sym != NOSTR; tptr++) {
			if (icequal(buff, tptr->sym)) {
				if (tptr->sys && !sysmode)
					(void) fprintf(stderr, ediag(
"User specified system parameter `%s' ignored.\n",
"Указание пользователем системного параметра `%s' игнорируется.\n"),
tptr->sym);
				else {
					if (tptr->dir) {
						char *s;

						for (s = cp; *s; s++)
							if (*s == '/')
								*s = SEPCHAR;
					}
					assign(tptr->sym, cp);
				}
				break;
			}
		}
	} /*for*/
} /*getconfig*/

/*
   configure - define the global parameters of UUPC
*/

void configure(void) {
	FILE*		fp;
	char*		sysrc;
	char*		usrrc;
	int		ok;
	int		success;
	struct Table*	tptr;
	char            *s;
	static char buf[_MAX_FNAME + 20];

	getrcnames(&sysrc, &usrrc);
	if ((fp = fopen(sysrc, "r")) == NULL) {
		(void) fprintf(stderr, ediag (
"Cannot open system configuration file `%s'\n",
"Нельзя открыть системный файл конфигурации `%s'\n"),
			       sysrc);
		exit(1);
	}
	getconfig(fp, TRUE);
	fclose(fp);
	if ((fp = fopen(usrrc, "r")) != NULL) {
		getconfig(fp, FALSE);
		fclose(fp);
	}

	if ((s = value("MSG")) != NULL && *s != '\0' && strchr("RrрР", *s) != NULL)
		_ediag = 0;

	success = TRUE;
	for (tptr = table; tptr->sym != NOSTR; tptr++) {
		if (tptr->must && value(tptr->sym) == NOSTR) {
			(void) fprintf(stderr, ediag(
"Configuration parameter `%s' must be set.\n",
"Должен быть задан параметр конфигурации `%s'.\n"),
tptr->sym);
			success = FALSE;
		}
	}
	if (success == FALSE)
		exit(1);

	if (getenv("TZ") == NOSTR && (s = value("TZ")) != NOSTR) {
		strcpy(buf, "TZ=");
		strcat(buf, s);
		putenv(buf);
	}
	tzset();

	strcpy(buf, "HOME=");
	strcat(buf, value("HOME"));
	putenv(buf);
} /*configure*/

/*
   mkfilename - build a path name out of a directory name and a file name
*/

void mkfilename(char* pathname, char* path, char* name) {
	char *s;

	for (s = path; *s; s++)
		if (*s == '/')
			*s = SEPCHAR;
	if (s[-1] != SEPCHAR)
		sprintf(pathname, "%s%c%s", path, SEPCHAR, name);
	else
		sprintf(pathname, "%s%s", path, name);
} /*mkfilename*/

/*
   getrcnames - return the name of the configuration files
*/

# define        SYSRCSYM        "UUSYSRC"
# define        USRRCSYM        "UUUSRRC"
extern char *argv0;

static void getrcnames(char** sysp, char** usrp) {
	char*	debugp;		/* Pointer to debug environment variable  */
	int	lvl;
	char	drv[_MAX_DRIVE];
	char	dir[_MAX_DIR];
	char	fname[_MAX_FNAME];
	char	ext[_MAX_EXT];
	char	buf[_MAX_PATH];
	static char pers[] = "PERSONAL.RC";
	static char sub_conf[] = "CONF\\";
	char homedir[_MAX_DRIVE + _MAX_DIR];
	char confdir[_MAX_DRIVE + _MAX_DIR];

	_splitpath(argv0, drv, dir, fname, ext);
	strcpy(calldir, drv);
	strcat(calldir, dir);
	if ((*sysp = getenv(SYSRCSYM)) != NULL) {
		*sysp = strdup(*sysp);
		_splitpath(*sysp, drv, dir, fname, ext);
		strcpy(confdir, drv);
		strcat(confdir, dir);
	}
	else {
		strcpy(confdir, calldir);
		strcat(confdir, sub_conf);
		mkfilename(buf, confdir, "UUPC.RC");
		*sysp = strdup(buf);
	}
	if ((*usrp = getenv(USRRCSYM)) != NULL) {
		*usrp = strdup(*usrp);
		_splitpath(*usrp, drv, dir, fname, ext);
		strcpy(homedir, drv);
		strcat(homedir, dir);
	}
	else {
		*usrp = strdup(pers);
		(void) getcwd(homedir, sizeof(homedir));
	}
	assign("HOME", homedir);
	AssignDefaultDirs();
} /*getrcnames*/

#define MAXDEPTH 10

static char *dirstack[MAXDEPTH];
static depth = 0;

/*--------------------------------------------------------------------*/
/*            Change to a directory and push on our stack             */
/*--------------------------------------------------------------------*/

PushDir( char *directory )
{
   assert ( depth < MAXDEPTH );
#ifdef __TURBOC__
   dirstack[depth] = getcwd( NULL , MAXPATH );
#else
   dirstack[depth] = _getdcwd( 0, NULL , MAXPATH );
#endif
   if (dirstack[depth] == NULL )
   {
      perror("PushDir");
      abort();
   }
   depth++;
   if (strcmp(directory, ".") == 0)
	return 0;
   return changedir( directory );
} /* PushDir */

/*--------------------------------------------------------------------*/
/*               Return to a directory saved by PushDir               */
/*--------------------------------------------------------------------*/

PopDir( void )
{
   assert ( depth > 0 );
   changedir( dirstack[--depth] );
   free( dirstack[depth] );
} /* PopDir */

#endif  /* MSDOS */
