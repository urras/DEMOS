/* === DEVICE TEST === */

#include "screen.h"
#include "dtest.h"

main()
{
    register char c;
    register int fd, lgt;
    char buf[512+1];

    ini_mem();
    if( ini_screen() < 0 )  exit( 0 );
    dpo( увт );

    if( (fd=open("/usr/minihelp/dtest",0)) > 0 ) {
        while( read(fd,buf,512) > 0 ) {buf[lgt]=0;  dpc( buf );}
        close( fd );
    }

    state = 0;  radr=0177776;  wadr=0177776;

    dpp( xr, yr-1 ); dpc( " READ" );
    dpp( xr, yr );   dpo( '(' );  dpc( conv( radr, 6, 8, '0' ) );
    dpc( ")=" );     dpc( conv( get(radr), 6, 8, '0' ) );

    dpp( xw, yw-1 ); dpc( " WRITE" );
    dpp( xw, yw );   dpo( '(' );  dpc( conv( wadr, 6, 8, '0' ) );
    dpc( ")=" );     dpc( conv( wvalue, 6, 8, '0' ) );

    dpp( x=xr+1, y=yr );

    for(;;) {
        switch( state ) {
            case 0:  c = getnumb( &radr, x-xr-1, 0177777, 0160000 ); break;
            case 1:  c = getnumb( &wadr, x-xw-1, 0177777, 0160000 ); break;
            case 2:  c = getnumb(&wvalue,x-xw-9, 0177777, 0000000 ); break;
        }
rep:    switch( c ) {
            case CTRL(D):               end_screen();      exit(0);
            case 'r':  case 'R':        state=0;  x=xr+1;  dpp(x,y); break;
            case 'w':  case 'W':        state=1;  x=xw+1;  dpp(x,y); break;
            case 'v':  case 'V':        state=2;  x=xw+9;  dpp(x,y); break;
            case 'c':  case 'C':
                if( state ) {
                    wcycle = !wcycle;
                    dpp(xw+8,yw-1);  dpc( wcycle ? "(CYCLE)" : "       " );
                } else {
                    rcycle = !rcycle;
                    dpp(xr+8,yr-1);  dpc( rcycle ? "(CYCLE)" : "       " );
                }
                dpp( x, y );
                break;
            case CR:
                switch( state ) {
                    case 0:
                        rvalue = get( radr );
                        dpp( xr+9, yr );  dpc( conv(rvalue,6,8,'0') );
                        c='r';  goto rep;
                    case 1:
                        c='v';  goto rep;
                    case 2:
                        set( wadr, wvalue );
                        c='v';  goto rep;
                }
        }
    }
}
