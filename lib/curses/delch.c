# include	"curses.ext"

/*
 * Стирание символа в текущий позиции окна "win".
 * Текущие координаты не изменяются. Все символы,
 * следующие за уничтоженным, сдвигаются на один
 * влево.
 */

wdelch (win)
reg WINDOW * win; {

    reg char   *temp1,
               *temp2;
    reg char   *end;

    end = &win -> _y[win -> _cury][win -> _maxx - 1];
    temp1 = &win -> _y[win -> _cury][win -> _curx];
    temp2 = temp1 + 1;
    while (temp1 < end)
	*temp1++ = *temp2++;
    *temp1 = ' ';
    win -> _lastch[win -> _cury] = win -> _maxx - 1;
    if (win -> _firstch[win -> _cury] == _NOCHANGE ||
	    win -> _firstch[win -> _cury] > win -> _curx)
	win -> _firstch[win -> _cury] = win -> _curx;
    return OK;
}
