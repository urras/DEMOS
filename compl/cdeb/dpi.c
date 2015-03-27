/* ιξιγιαμιϊαγιρ όλςαξξοηο ςεφινα */

#include <stty.h>
#include "screen.h"

#define lbuf  500
static char buf[lbuf];
static int xbuf;
static int oldtty[3], newtty[3];

#define escape '\033'

static struct {                         /* ΟΠΙΣΑΞΙΕ ΤΕÒΝΙΞΑΜΑ */
    char *name;                                 /* ΙΝΡ ΤΕÒΝΙΞΑΜΑ */
    char *beg, *end;                            /* ΙΞΙΓ. Ι ΪΑΧΕÒΫΕΞΙΕ */
    char *cu,   *cd,   *cl,   *cr,  *er,  *lf;  /* ΛΟΔΩ ΣΙΝΧΟΜΟΧ */
    char *dpp, xdpp, ydpp;                      /* ΣΤÒΟΛΑ ΠΟΪΙΓΙΟΞΙÒΟΧ. */
    char escflag;                               /* ΕΣΤΨ ΜΙ escape ΞΑ ΧΧΟΔ*/
} *pt, tdescr[] = {

    {   "15ιό-00-013",
        "\033\105", "\r\n\010\034\013\027",
        "\034","\035","\032","\031","\014","\r\n",
        "\027\033\131\041\041\033\105", 4, 3,
        0,
    },

    {   "χτα2000-2",
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


/* πεςελμΰώεξιε χ οβωώξωκ ςεφιν */

end_screen()
{
    sdpc( pt->end );
    scrbufout();
    stty(0,oldtty);
}


/* σβςοσ βυζεςα */

static scrbufout()
{
    if( xbuf )  write(1,buf,xbuf);
    xbuf = 0;
}


/* χχοδ σινχομα */

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
        else if( c == *(pt->er+1) )  c=σβς;
    }
    return( c );
}


/* πςοχεςλα, βωμ μι ξαφατ σινχομ */

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


/* υσταξοχ λυςσοςα χ ποϊιγιΰ (x,y) */

dpp(x,y)
    register int x, y;
{
    static char posit[20];
    strcpy( posit, pt->dpp );
    posit[pt->xdpp] += x-1;
    posit[pt->ydpp] += y-1;
    sdpc( posit );
}


/* χωχοδ στςολι σινχομοχ δο \0 */

dpc( str )
    register char *str;
{
    while( *str )  dpo( *str++ );
}


/* χωχοδ οδξοηο σινχομα */

dpo( c )
    char c;
{
    switch( c ) {
        case CU:        sdpc( pt->cu );  break;
        case CD:        sdpc( pt->cd );  break;
        case CR:        sdpc( pt->cr );  break;
        case CL:        sdpc( pt->cl );  break;
        case '\n':      sdpc( pt->lf );  break;
        case σβς:       sdpc( pt->er );  break;
        default:        sdpo( c );
    }
}


/* χωχοδ σινχομα 'c' 'n' ςαϊ */

dpn(n,c)
    char c;
    register int n;
{
    for(; n--;)  dpo( c );
}



/* χωχοδ στςολι σινχομοχ δο \0 */

static sdpc( str )
    register char *str;
{
    while( *str )  sdpo( *str++ );
}


/* χωχοδ οδξοηο σινχομα */

static sdpo( c )
    char c;
{
    buf[xbuf] = c;
    if( ++xbuf >= lbuf )  scrbufout();
}
