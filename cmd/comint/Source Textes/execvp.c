/*
 *      execlp(name, arg,...,0) (like execl, but does path search)
 *      execvp(name, argv)      (like execv, but does path search)
 */

/*
 * $Log:        execvp.c,v $
 * Revision 1.2  89/12/02  16:42:37  rygoff
 * Путь поиска берется из среды (как было у Флерова),
 * потому что среду среду можно переустаавливать.
 * 
 * Revision 1.1  89/08/19  18:18:09  rygoff
 * Initial revision
 * 
 *
 */


#include <errno.h>
#define NULL    ((char*)0)
extern  int     errno;
extern  char   *getenv(), *index();
#ifdef USG
#  define index strchr
#endif

static char  shell[]  = "/bin/sh";
static char  cshell[] = "/bin/csh";
static char *execat();

execlp(   name,  argv )
    char *name,**argv;
{
    return(execvp(name, &argv));
}

execvp(   name,  argv )
    char *name,**argv;
{
        register char *cp;
        char           fname[128];
        char          *newargs[256];
        register int   i;
        char           c;

        if( (cp=getenv("PATH")) == NULL )
                cp = ":/bin:/usr/bin:/usr/ucb:/etc";
        if( index(name,'/') )  cp = "";

        do {
                cp = execat(cp, name, fname);
                execv(fname, argv);
                if( errno == ENOEXEC ) {
                        if( (i=open(fname,0)) == -1 )  return(-1);
                        c = 0;  read(i,&c,1);  close(i);
                        newargs[0] = (c=='#') ? "csh" : "sh";
                        newargs[1] = fname;
                        for (i=1; newargs[i+1]=argv[i]; i++) {
                                if ( i>=254 )  return(-1);
                        }
                        execv( c=='#' ? cshell : shell, newargs );
                        return(-1);
                }
        } while( cp );
        return(-1);
}

static char *execat(s1, s2, si)
    register char  *s1,*s2;
    char                   *si;
{
        register char *s;

        s = si;
        while( *s1 && *s1 != ':' && *s1 != '-' )  *s++ = *s1++;
        if( si != s )  *s++ = '/';
        strcpy( s, s2 );
        return( *s1 ? ++s1 : 0 );
}
