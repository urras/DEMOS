# include	"curses.ext"

/*
 * Уничтожение окна "win".
 */

delwin (win)
reg WINDOW * win; {

    reg int i;
    reg WINDOW * wp, *np;

    if (win -> _orig == NULL) {
    /* 
     * Если окно является основным, уничтожаем все подокна
     * и освобождаем память.
     */
	for (i = 0; i < win -> _maxy && win -> _y[i]; i++)
	    cfree (win -> _y[i]);
	wp = win -> _nextp;
	while (wp != win) {
	    np = wp -> _nextp;
	    delwin (wp);
	    wp = np;
	}
    }
    else {
    /* 
     * Если подокно - вычеркиваем из списка подокон основного
     * окна.
     */
	for (wp = win -> _nextp; wp -> _nextp != win;
		wp = wp -> _nextp)
	    continue;
	wp -> _nextp = win -> _nextp;
    }
    cfree (win -> _y);
    cfree (win -> _firstch);
    cfree (win -> _lastch);
    cfree (win);
}
