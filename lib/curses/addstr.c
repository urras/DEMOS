# include	"curses.ext"

/*
 * ��������� ������ "str" � ���� "win.  ������
 * ������ � ������� �����������.
 */

waddstr (win, str)
reg WINDOW * win;
reg char   *str;
{
# ifdef DEBUG
    fprintf (outf, "WADDSTR(\"%s\")\n", str);
# endif
    while (*str)
	if (waddch (win, *str++) == ERR)
	    return ERR;
    return OK;
}
