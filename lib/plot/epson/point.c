/*
 * Рисование точки - Epson-80
 * by @VG
 */

#include "eps.h"

int _OX, _OY;
int _lptr;

point(x, y)
{
	_OX = x; _OY = y;
	_convert(&x, &y);
	SETPIX(x, y);
	_lptr = 0;
}
