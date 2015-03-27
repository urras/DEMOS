#include "comint.h"
#include <sys/stat.h>

extern char  ftype;
extern char *name_f, *name_g, *inp_name();
static char *scopy(), *afnm;

char *copy( name, mv_fl )
    char   *name;
    int           mv_fl;
{
    register int   i;
    register char *ret;

    afnm = name;
    for( i=0; i<nalt;    ++i )  ret=scopy(&altnam[i],          1,mv_fl);
    for( i=0; i<nmarked; ++i )  ret=scopy(&cat[marked[i]&0377],0,mv_fl);
    return( ret );
}



static char *scopy( pd, altflag, mv_fl )
    struct dir2    *pd;
    int                 altflag, mv_fl;
{
    char *name, *new;
    char *p;

    if( pd->d_mark=='>' or pd->d_mark=='<' )  return(0);
    new = name = pd->d_name;

    if( exist(new) )  new = inp_name( new, "", 'c' );
    if( new == NULL )  return( afnm );

    name_f=name; name_g=new;
    if( p=lookcom(('e'+mv_fl)|0x0200,afnm) ) exefound(p);
    else {
        register int   lgt, finp, fout;
        register char *fullname, *s;
        char           buf[1024];

        finp = fout = -1;  s = 0;
	if( !altflag ) {
	    fullname = name;
        } else {
	    s = altcat + strlen(altcat);
            strcat(altcat,"/");
            fullname = strcat(altcat,name);
        }

        errno = 0;

        if( !mv_fl ) {
            struct stat st;
            if( stat(fullname,&st) == -1)  goto ei;
            if( (finp=open(fullname,0))<= 0 ) goto ei;
            if( (fout=creat(new,0600)) <= 0 ) {name=new;goto ei;}
            while( (lgt=read(finp,buf,sizeof buf)) > 0 ) {
                if( write(fout,buf,lgt) != lgt )  {name=new;goto ei;}
            }
            chmod(new,st.st_mode);
        } else {
            link( fullname, new );
            if( ind_err(new,1)  )  goto er;
            unlink( fullname );
            if( ind_err(fullname,1) )  goto er;
#ifndef BSD
            {
#               include <sys/types.h>
                long   time();
                time_t timep[2];
                timep[0] = timep[1] = time(0);
                utime( new, timep );
            }
#endif
        }

rt:     close(finp);  close(fout);
        if( s )  *s = 0;
        return( new );

ei:     ind_err(name,1);
        unlink(new);
er:     new[0] = 0;
        goto rt;
    }
    return(new);
}
