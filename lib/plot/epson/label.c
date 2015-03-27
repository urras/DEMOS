/*
 * Рисование текста - Epson-80
 * by @VG
 */

#include "eps.h"

label(s)
register char *s;
{
	int c, ny;
	extern int _OX, _OY;
	int x = _OX, y = _OY;
	register char *p;
	extern char Font[];
	register ix, iy;
	static COX = -1, COY = -1;
	static six, siy;

	if( COX != _OX || COY != _OY ) {
		_convert( &x, &y );
		COX = _OX; COY = _OY;
		x -= 3; y += 4;         /* centering letter's boxes */
		six = x; siy = y;
		if( siy < 9 ) siy = 9;
		if( siy >= NYS ) siy = NYS-1;
		if( six < 0 ) six = 0;
	}
	while( c = *s++ ) {
		c &= 0377;
		if( c >= 040 && c < 0200 ) p = &Font[(c-040)*9];
		else if( c > 0277 )        p = &Font[(c-0300+0200-040)*9];
		else continue;
		if( six >= NXS-7 ) break;
		for(ny = 0 ; ny < 9 ; ny++) {
			iy = siy - ny;
			c = *p++;
			for( ix = 0 ; ix < 7 ; ix++ ) {
				if(c & 01)
					SETPIX(ix+six, iy);
				c >>= 1;
			}
		}
		six += 8;
	}
}
