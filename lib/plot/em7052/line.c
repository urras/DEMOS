/*
 * Рисование линии - ЭМ-7052
 * by @VG
 */
#include <stdio.h>

int _draws;

line(x0,y0,x1,y1)
{
	move(x0, y0);
	putchar('D');
	cont(x1, y1);
	_draws = 1;
}
