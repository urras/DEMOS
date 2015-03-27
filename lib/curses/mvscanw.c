# include	"curses.ext"

/*
 * Форматный ввод из заданной позиции окна.
 */

mvscanw (y, x, fmt, args)
reg int y,
        x;
char   *fmt;
int     args; {

    return move (y, x) == OK
	? _sscans (stdscr, fmt, &args)
	: ERR;
}

mvwscanw (win, y, x, fmt, args)
reg WINDOW * win;
reg int y,
        x;
char   *fmt;
int     args; {

    return wmove (win, y, x) == OK
	? _sscans (win, fmt, &args)
	: ERR;
}
