/*
 * ���������� ���������� ���������� ���������� curses.
 */
# include	"curses.h"

bool    _echoit         = TRUE, /* �������� �������� � ������ ECHO      */
	_rawmode        = FALSE,/* �������� �������� � ������ RAW       */
	My_term         = FALSE,/* ��� ��������� ����� �������������    */
	_endwin         = FALSE;/* ������ � �������� ������ ��������    */

char    ttytype[10],            /* ������ ��� ���������                 */
	*Def_term       = "unknown";    /* ��� ��������� �� ���������   */

int     _tty_ch         = 1,    /* ���������� ��� ������� � ������      */
	LINES,                  /* ����� ����� �� ������ ���������      */
	COLS,                   /* ����� �������� � ������              */
	_res_flg;               /* ������� ������ ������ ���������      */

WINDOW	*stdscr		= NULL,
	*curscr		= NULL;

# ifdef DEBUG
FILE    *outf;                  /* ���� ��� ������ ���������� ��������� */
# endif

SGTTY   _tty;                   /* ������ ������ ���������              */

bool	AM, BS, CA, DA, DB, EO, GT, HZ, IN, MI, MS, NC, OS, UL, XN,
	NONL, UPPERCASE, normtty, _pfast;

char	*AL, *BC, *BT, *CD, *CE, *CL, *CM, *CR, *DC, *DL, *DM, *DO,
	*ED, *EI, *HO, *IC, *IM, *IP, *LL, *MA, *ND, *NL, *SE, *SF,
	*SO, *SR, *TA, *TE, *TI, *UC, *UE, *UP, *US, *VB, *VE, *VS, PC;
