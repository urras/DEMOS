#include <curses.h>
#include <signal.h>

#define W_LINES 5
#define W_COLS  8

main()
{
	reg WINDOW *win, *blank;
	reg x, y;
	extern die();

	if (initscr () == ERR) {
		fprintf (stderr, "Sorry, unknown terminal.\n");
		exit (1);
	}

	signal (SIGINT, die);
	signal (SIGQUIT, die);
	signal (SIGHUP, die);

	noecho ();

	delwin (stdscr);

	win = newwin (W_LINES, W_COLS, 0, 0);
	blank = newwin (W_LINES, W_COLS, 0, 0);

	box (win, '*', '*');
	mvwaddstr (win, W_LINES/2, (W_COLS - strlen ("demos"))/2, "demos");

	srand (time(0));

	for (;;) {
		x = rand () / COLS;
		y = rand () / LINES;
		wrefresh (blank);
		if (mvwin (win, y, x) == OK) {
			wrefresh (win);
			mvwin (blank, y, x);
		}
	}
}

die (sig)
{
	signal (sig, SIG_IGN);
	werase (curscr);
	wmove (curscr, 0, 0);
	wrefresh (curscr);
	endwin ();
	exit (0);
}
