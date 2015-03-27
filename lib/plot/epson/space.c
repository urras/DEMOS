/*
 * Установка размещения области рисования на Epson-80
 * by @VG
 */

#include "eps.h"

extern float botx;
extern float boty;
extern float obotx;
extern float oboty;
extern float scalex;
extern float scaley;

space(x0,y0,x1,y1)
{
	boty = 0.;
	obotx = x0;
	oboty = y0;
	scalex = ((float)NXS)/(x1-x0);
	scaley = ((float)NYS)/(y1-y0);
	botx = 0.;     /* Moves graph area to left ... */
}
