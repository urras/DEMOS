/*
 * Рисование дуги - ЭМ-7052
 * by @VG
 */

#include <stdio.h>
#include <math.h>

int _draws;

#define CVT (360/3.1415926)

arc(cx, cy, x0, y0, x1, y1)
{
	double r, fi1, fi2;
	double dcx, dcy, dx0, dy0, dx1, dy1;

	_convert(&cx, &cy); dcx = cx; dcy = cy;
	_convert(&x0, &y0); dx0 = x0; dy0 = y0;
	_convert(&x1, &y1); dx1 = x1; dy1 = y1;

	r =  sqrt((dx0-dcx)*(dx0-dcx) + (dy0-dcy)*(dy0-dcy));

	fi1 = CVT * atan2(dx0-dcx, dy0-dcy);
	if( fi1 < 0 ) fi1 += 360;
	fi2 = CVT * atan2(dx1-dcx, dy1-dcy);
	if( fi2 < 0 ) fi2 += 360;

	x1 = (int)fi1;
	y1 = (int)fi2;
	cx = (int)r;
	printf("MA%d,%dDC%d,%d,%d", x0, y0, cx, x1, y1);
}
