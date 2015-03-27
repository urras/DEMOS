/*
 * Процедура вывода линии на ЭМ-7052
 * by @VG
 */

#include <stdio.h>

float obotx = 0.;
float oboty = 0.;
float botx = 0.;
float boty = 0.;
float scalex = 1.;
float scaley = 1.;

int _draws;

cont(x1 ,y1)
{
	_convert(&x1, &y1);
	printf("DA%u,%u", x1, y1);
	_draws = 1;
}

_convert(x, y)
int *x, *y;
{
	register ix, iy;

	ix = (*x-obotx)*scalex + botx;
	iy = (*y-oboty)*scaley + boty;
	*x = ix;
	*y = iy;
}
