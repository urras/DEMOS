/*
 * Форматный ввод
 */

# include	"curses.ext"

/*
 * Форматный ввод в стандартном окне (stdscr).
 */
scanw (fmt, args)
char   *fmt;
int     args; {

    return _sscans (stdscr, fmt, &args);
}
/*
 * Форматный ввод в окне "win".
 */
wscanw (win, fmt, args)
WINDOW * win;
char   *fmt;
int     args; {

    return _sscans (win, fmt, &args);
}

_sscans (win, fmt, args)
WINDOW * win;
char   *fmt;
int    *args; {

    char    buf[100];
    FILE junk;

    junk._flag = _IOREAD | _IOSTRG;
    junk._base = junk._ptr = buf;
    if (wgetstr (win, buf) == ERR)
	return ERR;
    junk._cnt = strlen (buf);
    return _doscan (&junk, fmt, args);
}
