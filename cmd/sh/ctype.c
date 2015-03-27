#
/*
 *      ���� �� 1.2 shell && sysV shell Bell Telephone Laboratories
 *
 *
 */

#include        "defs.h"

char    _ctype1[] = {
/*      000     001     002     003     004     005     006     007     */
	_EOF,   0,      0,      0,      0,      0,      0,      0,

/*      bs      ht      nl      vt      np      cr      so      si      */
	0,      _TAB,   _EOR,   0,      0,      0,      0,      0,

	0,      0,      0,      0,      0,      0,      0,      0,

	0,      0,      0,      0,      0,      0,      0,      0,

/*      sp      !       "       #       $       %       &       '       */
	_SPC,   0,      _DQU,   0,      _DOL1,  0,      _AMP,   0,

/*      (       )       *       +       ,       -       .       /       */
	_BRA,   _KET,   0,      0,      0,      0,      0,      0,

/*      0       1       2       3       4       5       6       7       */
	0,      0,      0,      0,      0,      0,      0,      0,

/*      8       9       :       ;       <       =       >       ?       */
	0,      0,      0,      _SEM,   _LT,    0,      _GT,    0,

/*      @       A       B       C       D       E       F       G       */
	0,      0,      0,      0,      0,      0,      0,      0,

/*      H       I       J       K       L       M       N       O       */
	0,      0,      0,      0,      0,      0,      0,      0,

/*      P       Q       R       S       T       U       V       W       */
	0,      0,      0,      0,      0,      0,      0,      0,

/*      X       Y       Z       [       \       ]       ^       _       */
	0,      0,      0,      0,      _BSL,   0,      _HAT,   0,

/*      `       a       b       c       d       e       f       g       */
	_LQU,   0,      0,      0,      0,      0,      0,      0,

/*      h       i       j       k       l       m       n       o       */
	0,      0,      0,      0,      0,      0,      0,      0,

/*      p       q       r       s       t       u       v       w       */
	0,      0,      0,      0,      0,      0,      0,      0,

/*      x       y       z       {       |       }       ~       del     */
	0,      0,      0,      0,      _BAR,   0,      0,      0
};


char    _ctype2[] = {
/*      000     001     002     003     004     005     006     007     */
	0,      0,      0,      0,      0,      0,      0,      0,

/*      bs      ht      nl      vt      np      cr      so      si      */
	0,      0,      0,      0,      0,      0,      0,      0,

	0,      0,      0,      0,      0,      0,      0,      0,

	0,      0,      0,      0,      0,      0,      0,      0,

/*      sp      !       "       #       $       %       &       '       */
	0,      _PCS,   0,      _NUM,   _DOL2,  0,      0,      0,

/*      (       )       *       +       ,       -       .       /       */
	0,      0,      _AST,   _PLS,   0,      _MIN,   0,      0,

/*      0       1       2       3       4       5       6       7       */
	_DIG,   _DIG,   _DIG,   _DIG,   _DIG,   _DIG,   _DIG,   _DIG,

/*      8       9       :       ;       <       =       >       ?       */
	_DIG,   _DIG,   0,      0,      0,      _EQ,    0,      _QU,

/*      @       A       B       C       D       E       F       G       */
	_AT,    _UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,

/*      H       I       J       K       L       M       N       O       */
	_UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,

/*      P       Q       R       S       T       U       V       W       */
	_UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,

/*      X       Y       Z       [       \       ]       ^       _       */
	_UPC,   _UPC,   _UPC,   _SQB,   0,      0,      0,      _UPC,

/*      `       a       b       c       d       e       f       g       */
	0,      _LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,

/*      h       i       j       k       l       m       n       o       */
	_LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,

/*      p       q       r       s       t       u       v       w       */
	_LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,

/*      x       y       z       {       |       }       ~       del     */
	_LPC,   _LPC,   _LPC,   _CBR,   0,      _CKT,   0,      0,


/*      000     001     002     003     004     005     006     007     */
	0,      0,      0,      0,      0,      0,      0,      0,

/*      bs      ht      nl      vt      np      cr      so      si      */
	0,      0,      0,      0,      0,      0,      0,      0,

	0,      0,      0,      0,      0,      0,      0,      0,

	0,      0,      0,      0,      0,      0,      0,      0,

/*      sp      !       "       #       $       %       &       '       */
	0,      0,      0,      0,      0,      0,      0,      0,

/*      (       )       *       +       ,       -       .       /       */
	0,      0,      0,      0,      0,      0,      0,      0,

/*      0       1       2       3       4       5       6       7       */
	0,      0,      0,      0,      0,      0,      0,      0,

/*      8       9       :       ;       <       =       >       ?       */
	0,      0,      0,      0,      0,      0,      0,      0,

# ifndef Ucode
/*      �       �       �       �       �       �       �       �       */
	_LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,

/*      �       �       �       �       �       �       �       �       */
	_LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,

/*      �       �       �       �       �       �       �       �       */
	_LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,

/*      �       �       �       �       �       �       �       �       */
	_LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,

/*      �       �       �       �       �       �       �       �       */
	_UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,

/*      �       �       �       �       �       �       �       �       */
	_UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,

/*      �       �       �       �       �       �       �       �       */
	_UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,

/*      �       �       �       �       �       �       �       0       */
	_UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,   0
# else
	_UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,
	_UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,
	_UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,
	_UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,   _UPC,
	_LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,
	_LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,
	_LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,
	_LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC,   _LPC
# endif Ucode
};



char    _ctype3[] = {
/*      nul     soh     stx     etc     eot     enq     ack     bel     */
	0200,   0201,   0202,   0203,   0204,   0205,   0206,   0207,

/*      bs      ht      nl      vt      np      cr      so      si      */
	0210,   0211,   0212,   0213,   0214,   0215,   0,      0,

/*      dle     dc1     dc2     dc3     dc4     nak     syn     etb     */
	0220,   0221,   0222,   0223,   0224,   0225,   0226,   0227,

/*      can     em      sub     esc     fs      gs      rs      us      */
	0230,   0231,   0232,   0233,   0234,   0235,   0236,   0237,

/*      sp      !       "       #       $       %       &       '       */
	0240,   0241,   0242,   0243,   0244,   0245,   0246,   0247,

/*      (       )       *       +       ,       -       .       /       */
	0250,   0251,   0252,   0253,   0254,   0255,   0256,   0257,

/*      0       1       2       3       4       5       6       7       */
	0,      0,      0,      0,      0,      0,      0,      0,

/*      8       9       :       ;       <       =       >       ?       */
	0,      0,      0272,   0273,   0274,   0275,   0276,   0277,

/*      @       A       B       C       D       E       F       G       */
	0216,   0,      0,      0,      0,      0,      0,      0,

/*      H       I       J       K       L       M       N       O       */
	0,      0,      0,      0,      0,      0,      0,      0,

/*      P       Q       R       S       T       U       V       W       */
	0,      0,      0,      0,      0,      0,      0,      0,

/*      X       Y       Z       [       \       ]       ^       _       */
	0,      0,      0,      0260,   0261,   0262,   0263,   0264,

/*      `       a       b       c       d       e       f       g       */
	0217,   0,      0,      0,      0,      0,      0,      0,

/*      h       i       j       k       l       m       n       o       */
	0,      0,      0,      0,      0,      0,      0,      0,

/*      p       q       r       s       t       u       v       w       */
	0,      0,      0,      0,      0,      0,      0,      0,

/*      x       y       z       {       |       }       ~       del     */
	0,      0,      0,      0265,   0266,   0267,   0270,   0271,


/*      nul     soh     stx     etc     eot     enq     ack     bel     */
	00,     01,     02,     03,     04,     05,     06,     07,

/*      bs      ht      nl      vt      np      cr      R(@)    R(')    */
	010,    011,    012,    013,    014,    015,    0100,   0140,

/*      dle     dc1     dc2     dc3     dc4     nak     syn     etb     */
	020,    021,    022,    023,    024,    025,    026,    027,

/*      can     em      sub     esc     fs      gs      rs      us      */
	030,    031,    032,    033,    034,    035,    036,    037,

/*      sp      !       "       #       $       %       &       '       */
	040,    041,    042,    043,    044,    045,    046,    047,

/*      (       )       *       +       ,       -       .       /       */
	050,    051,    052,    053,    054,    055,    056,    057,

/*      0       1       2       3       4       5       6       7       */
	0133,   0134,   0135,   0136,   0137,   0173,   0174,   0175,

/*      8       9       :       ;       <       =       >       ?       */
	0176,   0177,   072,    073,    074,    075,    076,    077,

/*      �       �       �       �       �       �       �       �       */
	0,      0,      0,      0,      0,      0,      0,      0,

/*      �       �       �       �       �       �       �       �       */
	0,      0,      0,      0,      0,      0,      0,      0,

/*      �       �       �       �       �       �       �       �       */
	0,      0,      0,      0,      0,      0,      0,      0,

/*      �       �       �       �       �       �       �       �       */
	0,      0,      0,      0,      0,      0,      0,      0,

/*      �       �       �       �       �       �       �       �       */
	0,      0,      0,      0,      0,      0,      0,      0,

/*      �       �       �       �       �       �       �       �       */
	0,      0,      0,      0,      0,      0,      0,      0,

/*      �       �       �       �       �       �       �       �       */
	0,      0,      0,      0,      0,      0,      0,      0,

/*      �       �       �       �       �       �       �       0       */
	0,      0,      0,      0,      0,      0,      0,      0,
};

/* ��� ��. �. �. ���������, ������, 1984 (���-8) */
