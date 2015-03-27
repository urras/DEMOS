#include <curses.h>
#include <signal.h>

#define W_LINES 10
#define W_COLS  30

#define center(WIND,LN,TEXT)    \
	mvwaddstr ((WIND), (LN), (W_COLS - strlen (TEXT))/2, (TEXT))

main()
{
	reg WINDOW *win1, *win2;
	extern die();

	if (initscr () == ERR) {
		fprintf (stderr, "Sorry, unknown terminal.\n");
		exit (1);
	}

	signal (SIGINT, die);
	signal (SIGQUIT, die);
	signal (SIGHUP, die);

	noecho ();

	win1 = newwin (W_LINES, W_COLS, ((LINES - W_LINES)/2), ((COLS - W_COLS)/2));
	win2 = newwin (W_LINES, W_COLS, ((LINES - W_LINES)/2 + 4), ((COLS - W_COLS)/2 + 10));

	scrollok (win1, FALSE);
	scrollok (win2, FALSE);

	box (win1, '*', '*');
	box (win2, '*', '*');

	center (win1, 3, "This is window 1");
	center (win2, 3, "This is window 2");

	for (;;) {
		wrefresh (win1);
		touchwin (win1);
		wrefresh (win2);
		touchwin (win2);
	}
}

die (sig)
{
	signal (sig, SIG_IGN);
	clear ();
	refresh ();
	endwin ();
	exit (0);
}
