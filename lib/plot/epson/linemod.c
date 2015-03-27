/*
 * Установка типа линии - Epson-80
 * by @VG
 */

int _lptr;
int _dotpat;

linemod(s)
char *s;
{
	_lptr = 0;
	if(      !strcmp(s, "dotted") )
		_dotpat = 052525;
	else if( !strcmp(s, "dotdashed") )
		_dotpat = 0377 | (3<<11);
	else if( !strcmp(s, "shortdashed") )
		_dotpat = 017 | (017<<8);
	else if( !strcmp(s, "longdashed") )
		_dotpat = 0377;
	else /* solid */
		_dotpat = 0177777;
}
