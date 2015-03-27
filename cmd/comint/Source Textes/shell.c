/* ВЫЗОВ SHELL'а */

/*
 * $Log:        shell.c,v $
 * Revision 1.2  89/11/11  23:00:08  rygoff
 * Введен режим MS_mode при редактировании строки.
 * 
 * Revision 1.1  89/08/19  18:12:17  rygoff
 * Initial revision
 * 
 *
 */

#include <tty_codes.h>
#include "comint.h"

static char  hist[history];
static char *hend = hist;
static char  cmd[maxcmd+1];

shell() {
    register char *p;
    register int   i, c, l;
    int pos;

    cmd[0] = 0;

cm: while( hend >= &hist[history-maxcmd-1] ) {     /* чистим history */
        i = strlen(hist) + 1;
        hend -= i;  move( hist+i, hist, hend-hist );
    }

    dpp(0,ysize-1);
    dpo('>');  dps(cmd);  dpn(xsize-2-strlen(cmd),' ');
    dpp(1,ysize-1);
    pos = 0;

in: for( i=strlen(cmd); i<maxcmd; ++i )  cmd[i]=' ';
    if( (l = xsize - 2) > maxcmd )  l = maxcmd;
    c = dpr( cmd, l, &pos, on_control|MS_mode );
    for( i=maxcmd; --i>=0 && cmd[i]==' ';  cmd[i]=0 )  ;

    if(      c == spec('&') )  c = '&';
    else if( c == spec('6') )  c = '&';
    else if( c == s_refuse  ) {savecmd(); return;} /* rygoff 24.07.89 */
    else if( c == s_finish  ) {savecmd(); return;} /* rygoff 25.07.89 */
    else if( c == s_help    ) {savecmd(); help("comint",'!');  return;}
    else if( c == cu        ) {
        if( hend == hist )  goto in;
        if( cmd[0] != 0 ) {
            l = strlen(cmd) + 1;
            move( hist, hist+l, hend-hist );  hend += l;
            strcpy( hist, cmd );
        }
        for( hend-=2; *hend; --hend ) {};
        ++hend;
        strcpy( cmd, hend );
        goto cm;
    }
    else if( c == cd        ) {
        if( hend == hist )  goto in;
        savecmd();
        l = strlen(hist) + 1;
        strcpy( cmd, hist );
        hend -= l;  move( hist+l, hist, hend-hist);
        goto cm;
    }
    else if( c != rn        )  goto in;

    savecmd();

    if( *cmd == 0 ) {
        p = "/bin/csh";  dps(p);
        strt(p,0,0,0);
    } else {
        start( cmd, c, YES );
    }
}


/*static*/ savecmd() {        /* ДОБАВЛЯЕТ КОМАНДУ 'cmd' К 'history' */
    if( cmd[0] != 0 ) {
        strcpy( hend, cmd );  hend += strlen(cmd) + 1;
    }
}
