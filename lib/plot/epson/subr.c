/*
 * Процедуры вывода линии и точки на Epson-80
 * by @VG
 */

#include <stdio.h>
#include "eps.h"

float obotx = 0.;
float oboty = 0.;
float botx = 0.;
float boty = 0.;
float scalex = 1.;
float scaley = 1.;

int _OX, _OY;
int _lptr = 0;
int _dotpat = 0177777;

cont(x1 ,y1)
{
	register x, y;
	int a, b, D, dx, dy, dd1, dd2, dd3;

	_convert(&_OX, &_OY);
	x = _OX; y = _OY;
	_OX = x1; _OY = y1;
	_convert(&x1, &y1);

	b = x1 - x;
	a = y1 - y;
	if( a < 0 ) a = -a;
	if( b > 0 ) b = -b;
	dx = (x > x1)? -1 : 1;
	dy = (y > y1)? -1 : 1;
	D = 0;
	while( x != x1 || y != y1 ) {
		if( _dotpat & (1<<_lptr) )
			SETPIX(x, y);
		_lptr++; _lptr &= 017;
		dd1 = D + a;
		dd2 = D + b;
		dd3 = dd1 + b;
		if( dd1 < 0 ) dd1 = -dd1;
		if( dd2 < 0 ) dd2 = -dd2;
		if( dd3 < 0 ) dd3 = -dd3;
		if( dd1 < dd2 ) {
			if( dd3 < dd1 )
				x += dx, y += dy, D += a+b;
			else
				x += dx, D += a;
		} else {
			if( dd3 < dd2 )
				x += dx, y += dy, D += a+b;
			else
				y += dy, D += b;
		}
	}
	SETPIX(x, y);
}

_convert(x, y)
int *x, *y;
{
	register ix, iy;

	ix = (*x-obotx)*scalex + botx;
	iy = (*y-oboty)*scaley + boty;
	if( ix < 0 )     ix = 0;
	if( ix > NXS-1 ) ix = NXS-1;
	if( iy < 0 )     iy = 0;
	if( iy > NYS-1 ) iy = NYS-1;
	*x = ix;
	*y = iy;
}
