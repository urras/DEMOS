#include <stdio.h>
#include "eps.h"

closevt()
{
	closepl();
}

closepl()
{
	register int i, maxi;
	int l;

	/*
	 * Вывод битовой карты в режиме Plotter Graphic (1/72")
	 */

	for( l = 0 ; l < NFL ; l++ ) {
		for( maxi = NXS-1; maxi >= 0; maxi-- )
			if( _Field[l][maxi] ) break;
		if( maxi >= 0 ) {
			maxi++;
#if PINS != 8
			if( maxi & 0200 )
				maxi = (maxi + 0400) & ~0377;
#endif
			putchar( 033 );
			putchar( '*' );
			putchar( 5 );   /* 1/72" */
			putchar( maxi & 0377 );
			putchar( maxi >> 8   );
			for( i = 0; i < maxi ; i++ ) {
				putchar( (i<NXS)? _Field[l][i] : 0 );
			}
		}
		putchar( 012 );
		putchar( 015 );
	}
}
