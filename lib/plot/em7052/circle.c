/*
 * Рисование окружности - ЭМ-7052
 * by @VG
 */

int _draws;

circle(x, y, r)
{
	int  tmp = 0;

	_draws = 1;
	_convert(&x, &y);
	_convert(&tmp, &r);
	printf("MA%d,%dDC%d,0,360", x+r, y, r);
}
