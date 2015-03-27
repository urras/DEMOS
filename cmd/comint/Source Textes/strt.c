/* úáğõóë ëïíáîäù ó ğáòáíåôòáíé 'p', 'p2', 'p3' */

/*
 * $Log:        strt.c,v $
 * Revision 1.1  89/08/19  18:12:58  rygoff
 * Initial revision
 * 
 *
 */

#include <signal.h>
#include "comint.h"
#include "run.h"

int strt( p, p2, p3, p4 )  { return( _strt( 0, p, p2, p3, p4 ) ); }
int strtv( av )            { return( _strt( av ) ); }

static int _strt( av,   p, p2, p3, p4 )
    char        *av[],*p,*p2,*p3,*p4;
{
    register int i;

    for( i=3; i<=16; ++i )  close(i);     /* ÚÁËÒÙ×ÁÅÍ ×ÓÅ ÄÅÓËÒÉĞÔÏÒÙ */
    vdpend();
    if( (i=fork()) == 0 ) {
        signal(SIGINT,SIG_DFL);
        signal(SIGQUIT,SIG_DFL);
        nice(addnice);

        if( av )   execvp( av[0], av );
        else       execlp( p, p, p2, p3, p4, 0 );

        vdpbeg();
        /* dpp(0,ysize-2);      */
        dps( av ? *av : p );
        dps( " ÎÅ ÎÁÊÄÅÎ");
        vdpend();

        exit(1);
    }
    i = wait_pid(i);
    vdpbeg();
    if( i && i != -SIGINT )  {
        dps("---");  dps( conv( i<0 ? -i : i, 3, 10, ' ' ) );
        dpo( i<0 ? '-' : '?' );  dps(" --- ");  dpi();
    }
    return( i );
}
