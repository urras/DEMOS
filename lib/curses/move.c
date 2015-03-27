# include	"curses.ext"

/*
 * Изменение значений текущих координат в окне "win".
 */

wmove (win, y, x)
reg WINDOW * win;
reg int y,
        x; {

# ifdef DEBUG
    fprintf (outf, "MOVE to (%d, %d)\n", y, x);
# endif
    if (x < 0 || x >= win -> _maxx
	|| y < 0 || y >= win -> _maxy)
	return ERR;
    win -> _curx = x;
    win -> _cury = y;
    return OK;
}
