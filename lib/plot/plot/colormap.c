/*
 * Библиотека libplot, установка палитры
 *
 * Цвета R/G/B в диапазоне от 0 до 1023
 */

#include <stdio.h>

colormap(color, R, G, B)
{
	register cm;

	putc('M', stdout);
	putsi(color);
	putsi(R);
	putsi(G);
	putsi(B);
}
