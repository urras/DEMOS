#include <curses.h>
#include <signal.h>
#include <ctype.h>

#define CNTRL(c)        (('c')&037)

main () {
    reg WINDOW * win, *boxing;
    reg int c;
    int     x,
            y;
    extern int  die ();

    initscr ();

    signal (SIGINT, die);
    noecho ();
    crmode ();

    win = subwin (stdscr,
	    LINES / 2,
	    COLS / 2,
	    LINES / 4,
	    COLS / 4);
    scrollok (win, TRUE);

    boxing = subwin (stdscr,
	    (LINES / 2 + 2),
	    (COLS / 2 + 2),
	    (LINES / 4 - 1),
	    (COLS / 4 - 1));

    box (boxing, '!', '-');
    refresh ();

    wmove (win, 0, 0);
    wrefresh (win);

    while ((c = wgetch (win)) != '#') {
	if (iscntrl (c))
	    switch (c) {
		case CNTRL (e): 
		    werase (win);
		    wrefresh (win);
		    continue;
		case CNTRL (r): 
		    wrefresh (curscr);
		    continue;
		case CNTRL ([): 
		    getyx (win, y, x);
		    c = wgetch (win);
		    switch (c) {
			case 'A': 
			    y--;
			    goto change;
			case 'B': 
			    y++;
			    goto change;
			case 'C': 
			    x++;
			    goto change;
			case 'D': 
			    x--;
		    change: 
			    if (x >= win -> _maxx) {
				x = 0;
				y++;
			    }
			    if (y >= win -> _maxy)
				y = 0;
			    wmove (win, y, x);
			    wrefresh (win);
			    continue;
			default: 
			    break;
		    }
		    break;
		default: 
		    continue;
	    }
	waddch (win, c);
	wrefresh (win);
    }
    die (SIGINT);
}

die (sig) {
    signal (sig, SIG_IGN);
    move ((LINES - 1), 0);
    refresh ();
    endwin ();
    exit OK;
}
