# include	"curses.ext"

/*
 * Чтение символа с экрана терминала.
 */

wgetch (win)
reg WINDOW * win; {

    reg inp;

    if (!win -> _scroll && (win -> _flags & _FULLWIN)
	    && win -> _curx == win -> _maxx - 1
	    && win -> _cury == win -> _maxy - 1)
	return ERR;
# ifdef DEBUG
    fprintf (outf,
	    "WGETCH: _echoit = %c, _rawmode = %c\n",
	    _echoit ? 'T' : 'F', _rawmode ? 'T' : 'F');
# endif
    inp = getchar ();
# ifdef DEBUG
    fprintf (outf, "WGETCH got '%s'\n", unctrl (inp));
# endif
    if (_echoit) {
	mvwaddch (curscr, win -> _cury, win -> _curx, inp);
	waddch (win, inp);
    }
    return inp;
}
