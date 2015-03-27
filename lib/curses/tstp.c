# include	<signal.h>

# include	"curses.ext"

/*
 * Обработка стоп/старт сигналов.
 */
tstp() {

# ifdef SIGTSTP

	SGTTY	tty;
# ifdef DEBUG
	if (outf)
		fflush(outf);
# endif
	tty = _tty;
	mvcur(0, COLS - 1, LINES - 1, 0);
	endwin();
	fflush(stdout);
	kill(0, SIGTSTP);
	signal(SIGTSTP, tstp);
	_tty = tty;
	stty(_tty_ch, &_tty);
	wrefresh(curscr);
# endif	SIGTSTP
}

