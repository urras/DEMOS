# include	"curses.ext"

/*
 * Чтение строки с экрана терминала.
 */

wgetstr (win, str)
reg WINDOW * win;
reg char   *str; {

    while ((*str = wgetch (win)) != ERR && *str != '\n')
	str++;
    if (*str == ERR) {
	*str = '\0';
	return ERR;
    }
    *str = '\0';
    return OK;
}
