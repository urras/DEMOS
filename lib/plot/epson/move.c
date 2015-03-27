/*
 * Установка текущей позиции - Epson-80
 * by @VG
 */

int _OX, _OY;
int _lptr;

move(x, y)
{
	_OX = x; _OY = y;
	_lptr = 0;
}
