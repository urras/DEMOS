/* ğòùöëé ğï ğïìñí (ÍÅÎÀ) */

#include "tty_codes.h"

int dpt( elements, cur, lines, width, x0, y0 )
    int  elements,*cur, lines, width, x0, y0;
{
    register int c, current;

    current = *cur;
    if( current<0   ||  current>=elements  ||  lines>elements ) {
	dpend();  exit( 100 );
    }

    for(;;) {
	dpp( x0 + ((current/lines)*width), y0 + (current%lines) );
	switch( c=dpi() ) {
	    case cu:
                if( lines<=1 )  goto retdpt;
		if(--current < 0)  current += elements;
		break;
	    case cd:
                if( lines<=1 )  goto retdpt;
                if(++current >= elements) current -= elements;
                break;
            case cr:
                if( lines==elements )  goto retdpt;
		if((current += lines)>= elements) {
		    current = (current+1) % lines;
		}
		break;
	    case cl:
                if( lines==elements )  goto retdpt;
		if((current -= lines) < 0) {
		    current += ((elements+lines-1)/lines)*lines-1;
		    if(current>=elements)  current -= lines;
		}
		break;
	    default:
		goto retdpt;
	}
    }
retdpt:
    *cur = current;
    return( c );
}
