/*
 * ��������� �����.
 */

# include	"curses.ext"

/*
 * ��������� ����� � ����������� ���� (stdscr).
 */
printw (fmt, args)
char   *fmt;
int     args; {

    return _sprintw (stdscr, fmt, &args);
}

/*
 * ��������� ����� � ���� "win".
 */
wprintw (win, fmt, args)
WINDOW * win;
char   *fmt;
int     args; {

    return _sprintw (win, fmt, &args);
}

_sprintw (win, fmt, args)
WINDOW * win;
char   *fmt;
int    *args; {

    FILE junk;
    char    buf[512];

    junk._flag = _IOWRT + _IOSTRG;
    junk._ptr = buf;
    junk._cnt = 32767;
    _doprnt (fmt, args, &junk);
    putc ('\0', &junk);
    return waddstr (win, buf);
}
