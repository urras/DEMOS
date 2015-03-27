# include	"curses.ext"

/*
 * Форматный вывод в заданных координатах окна.
 */

mvprintw (y, x, fmt, args)
reg int y,
        x;
char   *fmt;
int     args; {

    return move (y, x) == OK
	? _sprintw (stdscr, fmt, &args)
	: ERR;
}

mvwprintw (win, y, x, fmt, args)
reg WINDOW * win;
reg int y,
        x;
char   *fmt;
int     args; {

    return wmove (win, y, x) == OK
	? _sprintw (win, fmt, &args)
	: ERR;
}
