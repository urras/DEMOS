/* ������������� ��������� ������ */

#include <stty.h>
#include "screen.h"

#define lbuf  500
static char buf[lbuf];
static int xbuf;
static int oldtty[3], newtty[3];

#define escape '\033'

static struct {                         /* �������� ��������� */
    char *name;                                 /* ��� ��������� */
    char *beg, *end;                            /* ����. � ���������� */
    char *cu,   *cd,   *cl,   *cr,  *er,  *lf;  /* ���� �������� */
    char *dpp, xdpp, ydpp;                      /* ������ ������������. */
    char escflag;                               /* ���� �� escape �� ����*/
} *pt, tdescr[] = {

    {   "15��-00-013",
        "\033\105", "\r\n\010\034\013\027",
        "\034","\035","\032","\031","\014","\r\n",
        "\027\033\131\041\041\033\105", 4, 3,
        0,
    },

    {   "���2000-2",
        "", "\r\n\033s\033z",
        "\033l","\033i","\033h","\033c","\033_","\r\n",
        "\033r\001\001", 3, 2,
        1,
    },
};



ini_screen()
{
    register char *penv;

    penv = getenv( "TERM" );
    for( pt=tdescr; pt<(char*)tdescr+(sizeof tdescr); ++pt) {
        if( !strcmp( pt->name, penv ) )  goto ok;
    }
    return( -1 );

ok: gtty(0,oldtty);
    newtty[0] = oldtty[0];   newtty[1] = oldtty[1];
    newtty[2] = (oldtty[2] & ~(CRMOD|ECHO)) | RAWI | RAWO;
    stty(0,newtty);
    xbuf=0;
    sdpc( pt->beg );
    return( 0 );
}


/* ������������ � ������� ����� */

end_screen()
{
    sdpc( pt->end );
    scrbufout();
    stty(0,oldtty);
}


/* ����� ������ */

static scrbufout()
{
    if( xbuf )  write(1,buf,xbuf);
    xbuf = 0;
}


/* ���� ������� */

dpi()
{
    char c;

    scrbufout();
    read(0,&c,1);
    if( c == 3 ) {end_screen();  exit(1);}
    if( c == escape   &&  pt->escflag ) {
        read(0,&c,1);
        if(      c == *(pt->cu+1) )  c=CU;
        else if( c == *(pt->cd+1) )  c=CD;
        else if( c == *(pt->cr+1) )  c=CR;
        else if( c == *(pt->cl+1) )  c=CL;
        else if( c == *(pt->lf+1) )  c='\n';
        else if( c == *(pt->er+1) )  c=���;
    }
    return( c );
}


/* ��������, ��� �� ����� ������ */

dpm()
{
    char c;
    scrbufout();
    if( empty(0) ) {
        return( -1 );
    } else {
        return( dpi() );
    }
}


/* ������� ������� � ������� (x,y) */

dpp(x,y)
    register int x, y;
{
    static char posit[20];
    strcpy( posit, pt->dpp );
    posit[pt->xdpp] += x-1;
    posit[pt->ydpp] += y-1;
    sdpc( posit );
}


/* ����� ������ �������� �� \0 */

dpc( str )
    register char *str;
{
    while( *str )  dpo( *str++ );
}


/* ����� ������ ������� */

dpo( c )
    char c;
{
    switch( c ) {
        case CU:        sdpc( pt->cu );  break;
        case CD:        sdpc( pt->cd );  break;
        case CR:        sdpc( pt->cr );  break;
        case CL:        sdpc( pt->cl );  break;
        case '\n':      sdpc( pt->lf );  break;
        case ���:       sdpc( pt->er );  break;
        default:        sdpo( c );
    }
}


/* ����� ������� 'c' 'n' ��� */

dpn(n,c)
    char c;
    register int n;
{
    for(; n--;)  dpo( c );
}



/* ����� ������ �������� �� \0 */

static sdpc( str )
    register char *str;
{
    while( *str )  sdpo( *str++ );
}


/* ����� ������ ������� */

static sdpo( c )
    char c;
{
    buf[xbuf] = c;
    if( ++xbuf >= lbuf )  scrbufout();
}
