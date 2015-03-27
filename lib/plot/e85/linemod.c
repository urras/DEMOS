/*
 * Установка типа линии - Э-85
 * by @VG
 */

linemod(s)
char *s;
{
	char c;

	putch(033);
	if( !strcmp(s, "solid") )
		putch('0');
	else
		putch('1');
}
