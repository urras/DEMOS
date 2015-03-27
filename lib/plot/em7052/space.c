/*
 * Установка размещения области рисования на ЭМ-7052
 * by @VG
 */

extern float botx;
extern float boty;
extern float obotx;
extern float oboty;
extern float scalex;
extern float scaley;
extern unsigned _sizex, _sizey;

space(x0,y0,x1,y1)
{
	unsigned siz;

	siz = _sizex;   /* search for max square plot area */
	if( siz < _sizey ) siz = _sizey;

	boty = 0.;
	botx = 0.;
	obotx = x0;
	oboty = y0;
	scalex = ((float)siz)/(x1-x0);
	scaley = ((float)siz)/(y1-y0);
}
