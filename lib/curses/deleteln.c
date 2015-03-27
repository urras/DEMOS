# include       "curses.ext"
#include <curses.h>
/*
 * Уничтожение текущей строки. Все строки,
 * следующие за уничтоженной, сдвигаются на
 * одну вверх. Текущие координаты не меняются.
 */

wdeleteln (win)
reg WINDOW * win; {

    reg char   *temp;
    reg int y;
    reg char   *end;

    temp = win -> _y[win -> _cury];
    for (y = win -> _cury; y < win -> _maxy - 1; y++) {
	win -> _y[y] = win -> _y[y + 1];
	win -> _firstch[y] = 0;
	win -> _lastch[y] = win -> _maxx - 1;
    }
    for (end = &temp[win -> _maxx]; temp < end;)
	*temp++ = ' ';
    /* и обозначить , что изменилась последняя строка */
    win-> _firstch[ win->_maxy -1 ] = 0;
    win-> _lastch [ win->_maxy -1 ] = win->_maxx -1;
    win -> _y[win -> _maxy - 1] = temp - win -> _maxx;
}
