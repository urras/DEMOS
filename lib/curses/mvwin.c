# include	"curses.ext"

/*
 * Изменение начальной позиции окна "win".
 */

mvwin (win, by, bx)
reg WINDOW * win;
reg int by,
        bx; {

    if (by + win -> _maxy > LINES
	    || bx + win -> _maxx > COLS)
	return ERR;
    win -> _begy = by;
    win -> _begx = bx;
    touchwin (win);
    return OK;
}
