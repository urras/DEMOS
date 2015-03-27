/*
 * Установка размещения области рисования на экране Э-85
 * by @VG
 */

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
	scalex = (2.5*239.)/(x1-x0);
	scaley = 239./(y1-y0);
	botx = 400.;     /* Moves graph area to left ... */
}
