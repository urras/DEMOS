# include	"curses.ext"

/*
 * Занесение строки "str" в окно "win.  Начало
 * строки в текущих координатах.
 */

waddstr (win, str)
reg WINDOW * win;
reg char   *str;
{
# ifdef DEBUG
    fprintf (outf, "WADDSTR(\"%s\")\n", str);
# endif
    while (*str)
	if (waddch (win, *str++) == ERR)
	    return ERR;
    return OK;
}
