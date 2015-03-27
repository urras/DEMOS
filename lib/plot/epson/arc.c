/*
 * Рисование дуги - Epson-80
 * by @VG
 */
#include "eps.h"

arc(cx, cy, x0, y0, x1, y1)
{
	register x, y;
	int D, d1, d2, d3, dd1, dd2, dd3;
	extern int _lptr, _dotpat;
	int snt, dx, dy;

	_convert(&cx, &cy);
	_convert(&x0, &y0);
	_convert(&x1, &y1);

	x = x0; y = y0;

	for( D = 0, snt = 0;; ) {
		if( (_dotpat & (1<<_lptr)) && x >= 0 && x < NXS &&
					      y >= 0 && y < NYS )
			SETPIX(x, y);
		_lptr++; _lptr &= 017;

		d1 = x-x1; if( d1 < 0 ) d1 = -d1;
		d2 = y-y1; if( d2 < 0 ) d2 = -d2;
		if( d1+d2 <= 1 && snt > 2 ) break;
		if( snt++ > 2000 ) break;

		dx = (y < cy)?  1 : -1;
		dy = (x < cx)? -1 :  1;
		d1 = dd1 = D + 1 + 2*dx*(x-cx);
		d2 = dd2 = D + 1 + 2*dy*(y-cy);
		d3 = dd3 = d1+d2-D;
		if( dd1 < 0 ) dd1 = -dd1;
		if( dd2 < 0 ) dd2 = -dd2;
		if( dd3 < 0 ) dd3 = -dd3;
		if( dd1 < dd2 ) {
			if( dd3 < dd1 ) { x += dx; y += dy; D = d3; }
			else            { x += dx;          D = d1; }
		} else {
			if( dd3 < dd2 ) { x += dx; y += dy; D = d3; }
			else            {          y += dy; D = d2; }
		}
	}
	SETPIX(x1, y1);
}
