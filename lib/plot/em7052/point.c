/*
 * Рисование точки - ЭМ-7052
 * by @VG
 */
#include <stdio.h>

int _draws;

point(x, y)
{
	move(x, y);
	putchar('D');
	_draws = 1;
}
