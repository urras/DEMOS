/*
 * Рисование текста - Э-85
 * by @VG
 */

int _COx, _COy;

label(s)
register char *s;
{
	register c;

	putch(033); putch('S'); putch(033) ; putch('e');
	putch(03);
	_DotCC(_COx-5, _COy+4);  /* Center the char */
	while( c = *s++ ) {
		_COx += 12;
		putch(c);
	}
	putch(05);      /* End of graph text mode */
	putch(036);
	_DotCC(_COx+5, _COy-4);  /* Set next point */
	putch(033); putch('R');
}
