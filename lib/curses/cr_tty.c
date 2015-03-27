/*
 * Настройка на тип терминала.
 * Этот кусок выдран из libcurses только потому,
 * что getcap выдавала только ОПРЕДЕЛЕННЫЕ описатели, а не все,
 * поэтому ku,kr, ..., f0, ... f9 были недоступны
 */

# include       "curses.ext"

static bool     *sflags[]       = {
			&AM, &BS, &EO, &HZ, &IN, &MI, &MS, &NC, &OS, &UL, &XN
		};

static char     *xPC,
		**sstrs[]       = {
			&AL, &BC, &BT, &CD,  &CE, &CL, &CM, &CR, &DC,
			&DL, &DM, &DO, &ED,  &EI, &HO, &IC, &IM, &IP,
			&LL, &MA, &ND, &NL, &xPC, &SE, &SF, &SO, &SR,
			&TA, &TE, &TI, &UC,  &UE, &UP, &US, &VB, &VS,
			&VE
		},
		*tgoto();

static char     tspace[612],            /* Буфер для описаний из termcap */
		genbuf[512],            /* Буфер для tgetent */
		*aoftspace;             /* Адрес в буфере */

static int      destcol, destline;

short   ospeed = -1;

gettmode() {

	if (gtty(_tty_ch, &_tty) < 0)
		return;
	savetty();
	if (stty(_tty_ch, &_tty) < 0)
		_tty.sg_flags = _res_flg;
	ospeed = _tty.sg_ospeed;
	_res_flg = _tty.sg_flags;
	UPPERCASE = (_tty.sg_flags & LCASE) != 0;
	GT = ((_tty.sg_flags & XTABS) == 0);
	NONL = ((_tty.sg_flags & CRMOD) == 0);
	_tty.sg_flags &= ~XTABS;
	if (UPPERCASE) _tty.sg_flags &= ~UCASE;
	stty(_tty_ch, &_tty);
}


setterm(type)
reg char        *type; {

	reg int         unknown;

extern char *ErAsEd;    /* из нового longname.c */
extern char WhAtErAsEd;

	if (type[0] == '\0')
		type = "xx";
	unknown = FALSE;
	if (tgetent(genbuf, type) != 1) {
		unknown++;
		strcpy(genbuf, "xx|dumb:");
	}
	if (LINES == 0)
		LINES = tgetnum("li");
	if (LINES <= 5)
		LINES = 24;
	else if (LINES > 48)
		LINES = 48;

	if (COLS == 0)
		COLS = tgetnum("co");
	if (COLS <= 4)
		COLS = 80;
	else if (COLS > 1000)
		COLS = 1000;
	aoftspace = tspace;
	zap();                  /* получение описания терминала */
	if (tgoto(CM, destcol, destline)[0] == 'O')
		CA = FALSE, CM = 0;
	else
		CA = TRUE;
	PC = xPC ? xPC[0] : FALSE;
	/* aoftspace = tspace; явно вредительская строка */
	strcpy(ttytype, longname(genbuf, type));
/* Эта сволочь longname пишет в genbuf \0 */
/* Ща мы его оттуда вытрем */
	if( ErAsEd != -1 ) *ErAsEd = WhAtErAsEd;
	if (unknown)
		return ERR;
	return OK;
}
/*
 *      This routine gets all the terminal flags from the termcap database
 */
zap() {

	reg bool        **fp;
	reg char        *namp, ***sp;
/*-------------------------------------------------------*\
 *      reg int         SG, UG;                          *
\*-------------------------------------------------------*/
	extern char     *tgetstr();

	/*
	 * get boolean flags
	 */
	namp = "ambseohzinmimsncosulxn\0\0";
	fp = sflags;
	do {
		*(*fp++) = tgetflag(namp);
		namp += 2;
	} while (*namp);

	/*
	 * get string values
	 */
	namp = "albcbtcdceclcmcrdcdldmdoedeihoicimipllmandnlpcsesfsosrtatetiucueupusvbvsve";
	sp = sstrs;
	do {
		*(*sp++) = tgetstr(namp, &aoftspace);
		namp += 2;
	} while (*namp);
/*-------------------------------------------------------*\
 *      SG = tgetnum("sg");                              *
 *      UG = tgetnum("ug");                              *
 *      if ((SG > 0 || !SO) && (UG <= 0 && US)) {        *
 *              SO = US;                                 *
 *              SE = UE;                                 *
 *      }                                                *
\*-------------------------------------------------------*/
}

/*
 * Получение описания из termcap
 */
char *
getcap(name)
char *name;
{
	char *tgetstr();
#ifdef DEBUG
	fprintf( stderr, "%s ", name );
#endif
	return tgetstr(name, &aoftspace );
}
