/*
 * Рисование окружности - Э-85
 * by @VG
 */
int _Ox, _Oy;

circle(x, y, r)
{
	int xx=_Ox, yy=_Oy;

	_DotC(036, x, y);
	_DotC(02, x+r, y);
	_DotC(036, xx, yy);
}
