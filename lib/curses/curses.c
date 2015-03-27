/*
 * Декларация глобальных переменных библиотеки curses.
 */
# include	"curses.h"

bool    _echoit         = TRUE, /* терминал работает в режиме ECHO      */
	_rawmode        = FALSE,/* терминал работает в режиме RAW       */
	My_term         = FALSE,/* тип терминала задан пользователем    */
	_endwin         = FALSE;/* работа в экранном режиме окончена    */

char    ttytype[10],            /* полное имя терминала                 */
	*Def_term       = "unknown";    /* тип терминала по умолчанию   */

int     _tty_ch         = 1,    /* дескриптор для доступа к экрану      */
	LINES,                  /* число строк на экране терминала      */
	COLS,                   /* число символов в строке              */
	_res_flg;               /* прежние режиме работы терминала      */

WINDOW	*stdscr		= NULL,
	*curscr		= NULL;

# ifdef DEBUG
FILE    *outf;                  /* файл для вывода отладочных сообщений */
# endif

SGTTY   _tty;                   /* режимы работы терминала              */

bool	AM, BS, CA, DA, DB, EO, GT, HZ, IN, MI, MS, NC, OS, UL, XN,
	NONL, UPPERCASE, normtty, _pfast;

char	*AL, *BC, *BT, *CD, *CE, *CL, *CM, *CR, *DC, *DL, *DM, *DO,
	*ED, *EI, *HO, *IC, *IM, *IP, *LL, *MA, *ND, *NL, *SE, *SF,
	*SO, *SR, *TA, *TE, *TI, *UC, *UE, *UP, *US, *VB, *VE, *VS, PC;
