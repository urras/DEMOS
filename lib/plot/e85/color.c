/*
 * Установка цвета для вывода на Электронике-85
 * by @VG
 */
color(col)
{
	putch(033);
	putch('x');
	if( col == 0 ) {
		putch('7');
		putch(033);
		putch('d');             /* clear by white */
	} else {
		col = (col-1)%7;
		putch('1'+col);
		putch(033);
		putch('c');             /* draw by col */
	}
}
