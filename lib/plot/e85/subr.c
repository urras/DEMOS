/*
 * Процедура вывода координат для Э-85
 * by @VG
 */

#include <stdio.h>

float obotx = 0.;
float oboty = 0.;
float botx = 0.;
float boty = 0.;
float scalex = 1.;
float scaley = 1.;

cont(x,y)
{
	_DotC(035, x, y);
}

int _Ox, _Oy;
int _COx, _COy;

_DotC(c, x, y)
{
	_Ox = x; _Oy = y;
	putch(c);
	x = (x-obotx)*scalex + botx;
	y = (y-oboty)*scaley + boty;
	if( x < 0 )    x = 0;
	if( x > 1023 ) x = 1023;
	if( y < 0 )    y = 0;
	if( y > 239  ) y = 239;
	_DotCC(x, y);
	_COx = x; _COy = y;
}

_DotCC(x, y)
{
	long Lc;

	Lc = ((long)(239-y)<<10) + x;
	putch(((short)(Lc>>12) & 077) + '@');
	putch(((short)(Lc>> 6) & 077) + '@');
	putch(((short) Lc      & 077) + '@');
}

putch(c)
{
	putc(c,stdout);
}
