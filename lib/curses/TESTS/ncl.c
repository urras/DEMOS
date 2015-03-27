#include <curses.h>
#include <signal.h>

#define DEFSLEEP        1
#define TIMEMASK        077

#define centred(y,str)  mvaddstr(y,(COLS - strlen(str))/2,str)

main (ac, av)
int     ac;
char   *av[];
{
    register    move_col,
                sleep_time;
    int     time_line,
            move_line;
    long    num_start_time,
            num_now_time;
    char    str_start_time[60],
            str_now_time[60];
    extern long time ();
    extern char *ctime (), *index ();
    extern int  die (), strlen ();

    sleep_time = 0;
    if (ac > 1)
	if ((sleep_time = atoi (av[1])) < DEFSLEEP)
	    sleep_time = DEFSLEEP;
	else
	    sleep_time &= TIMEMASK;
    if (initscr () == ERR)
	exit (1);

    signal (SIGINT, die);
    signal (SIGQUIT, die);
    signal (SIGHUP, die);
    signal (SIGTERM, die);

    box (stdscr, '*', '*');

    time (&num_start_time);
    strcpy (str_start_time, ctime (&num_start_time));
    *(index (str_start_time, '\n')) = '\0';

    centred (3, "*** Test started at: ***");
    centred (4, str_start_time);

    move_col = 1;
    move_line = (LINES - 2) / 2;
    time_line = move_line + (LINES - move_line - 1) / 2;

    while (TRUE) {
	time (&num_now_time);
	strcpy (str_now_time, ctime (&num_now_time));
	*(index (str_now_time, '\n')) = '\0';

	centred (time_line, str_now_time);

	mvaddstr (move_line, move_col, "<-*->");

	refresh ();

	mvaddstr (move_line, move_col, "     ");
	if (++move_col >= (COLS - 6))
	    move_col = 1;

	if (sleep_time)
	    sleep (sleep_time);
    }
}

die () {
    alarm (0);
    signal (SIGINT, SIG_IGN);
    signal (SIGQUIT, SIG_IGN);
    signal (SIGHUP, SIG_IGN);
    signal (SIGTERM, SIG_IGN);
    clear ();
    refresh ();
    endwin ();
    exit (0);
}
