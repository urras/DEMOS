#include "tdef.h"

char    obuf[OBUFSZ];
char   *obufp = obuf;
int     r[NN] = {
	PAIR ('%', 0),
	PAIR ('n', 'l'),
	PAIR ('y', 'r'),
	PAIR ('h', 'p'),
	PAIR ('c', 't'),
	PAIR ('d', 'n'),
	PAIR ('m', 'o'),
	PAIR ('d', 'y'),
	PAIR ('d', 'w'),
	PAIR ('l', 'n'),
	PAIR ('d', 'l'),
	PAIR ('s', 't'),
	PAIR ('s', 'b'),
	PAIR ('c', '.')
};

int     pto = 10000;
int     pfrom = 1;
int     print = 1;
char    nextf[NS] = "/usr/lib/tmac/tmac.xxxxx";
int     nfi = 19;

char    termtab[NS] = "/usr/lib/term/tabstd";
int     tti = 17;


#ifdef SOAN
int     rdflg;
#endif

int     init = 1;
int     fc = IMP;
int     eschar = '\\';
int     pl = 11 * INCH;
int     po = PO;
int     dfact = 1;
int     dfactd = 1;
int     res = 1;
int     smnt = 4;
int     ascii = ASCII;
int     ptid = PTID;
char    ptname[] = "/dev/cat";
int     lg = LG;
int     pnlist[NPN] = {
	-1
};
int    *pnp = pnlist;
int     npn = 1;
int     npnflg = 1;
int     xflg = 1;
int     dpn = -1;
int     totout = 1;
int     ulfont = 1;
#ifndef ASA
int     ulbit = 1 << 9;
#else
int     ulbit = 1 <<10;
#endif
int     tabch = TAB;
int     ldrch = LEADER;
int     xxx;

extern  caseds (), caseas (), casesp (), caseft (), caseps (), casevs (),
        casenr (), caseif (), casepo (), casetl (), casetm (), casebp (), casech (),
        casepn (), tbreak (), caseti (), casene (), casenf (), casece (), casefi (),
        casein (), caseli (), casell (), casens (), casemk (), casert (), caseam (),
        casede (), casedi (), caseda (), casewh (), casedt (), caseit (), caserm (),
        casern (), casead (), casers (), casena (), casepl (), caseta (), casetr (),
        caseul (), caselt (), casenx (), caseso (), caseig (), casetc (), casefc (),
        caseec (), caseeo (), caselc (), caseev (), caserd (), caseab (), casefl (),
        done (), casess (), casefp (), casecs (), casebd (), caselg (), casehc (),
        casehy (), casenh (), casenm (), casenn (), casesv (), caseos (), casels (),
        casecc (), casec2 (), caseem (), caseaf (), casehw (), casemc (), casepm (),
        casecu (), casepi (), caserr (), caseuf (), caseie (), caseel (), casepc (),
        caseht (), casecf ();

#ifdef RUSMAC
#  define RMM 0,
#else
#  define RMM
#endif

struct contab {
#ifdef RUSMAC
	int     mask;
#endif
	int     rq;
	int     (*f) ();
}               contab[NM] = {
	                RMM PAIR ('d', 's'), caseds,
	                RMM PAIR ('a', 's'), caseas,
	                RMM PAIR ('s', 'p'), casesp,
	                RMM PAIR ('f', 't'), caseft,
	                RMM PAIR ('p', 's'), caseps,
	                RMM PAIR ('v', 's'), casevs,
	                RMM PAIR ('n', 'r'), casenr,
	                RMM PAIR ('i', 'f'), caseif,
	                RMM PAIR ('i', 'e'), caseie,
	                RMM PAIR ('e', 'l'), caseel,
	                RMM PAIR ('p', 'o'), casepo,
	                RMM PAIR ('t', 'l'), casetl,
	                RMM PAIR ('t', 'm'), casetm,
	                RMM PAIR ('b', 'p'), casebp,
	                RMM PAIR ('c', 'h'), casech,
	                RMM PAIR ('p', 'n'), casepn,
	                RMM PAIR ('b', 'r'), tbreak,
	                RMM PAIR ('t', 'i'), caseti,
	                RMM PAIR ('n', 'e'), casene,
	                RMM PAIR ('n', 'f'), casenf,
	                RMM PAIR ('c', 'e'), casece,
	                RMM PAIR ('f', 'i'), casefi,
	                RMM PAIR ('i', 'n'), casein,
	                RMM PAIR ('l', 'i'), caseli,
	                RMM PAIR ('l', 'l'), casell,
	                RMM PAIR ('n', 's'), casens,
	                RMM PAIR ('m', 'k'), casemk,
	                RMM PAIR ('r', 't'), casert,
	                RMM PAIR ('a', 'm'), caseam,
	                RMM PAIR ('d', 'e'), casede,
	                RMM PAIR ('d', 'i'), casedi,
	                RMM PAIR ('d', 'a'), caseda,
	                RMM PAIR ('w', 'h'), casewh,
	                RMM PAIR ('d', 't'), casedt,
	                RMM PAIR ('i', 't'), caseit,
	                RMM PAIR ('r', 'm'), caserm,
	                RMM PAIR ('r', 'r'), caserr,
	                RMM PAIR ('r', 'n'), casern,
	                RMM PAIR ('a', 'd'), casead,
	                RMM PAIR ('r', 's'), casers,
	                RMM PAIR ('n', 'a'), casena,
	                RMM PAIR ('p', 'l'), casepl,
	                RMM PAIR ('t', 'a'), caseta,
	                RMM PAIR ('t', 'r'), casetr,
	                RMM PAIR ('u', 'l'), caseul,
	                RMM PAIR ('c', 'u'), casecu,
	                RMM PAIR ('l', 't'), caselt,
	                RMM PAIR ('n', 'x'), casenx,
	                RMM PAIR ('s', 'o'), caseso,
	                RMM PAIR ('i', 'g'), caseig,
	                RMM PAIR ('t', 'c'), casetc,
	                RMM PAIR ('f', 'c'), casefc,
	                RMM PAIR ('e', 'c'), caseec,
	                RMM PAIR ('e', 'o'), caseeo,
	                RMM PAIR ('l', 'c'), caselc,
	                RMM PAIR ('e', 'v'), caseev,
	                RMM PAIR ('r', 'd'), caserd,
	                RMM PAIR ('a', 'b'), caseab,
	                RMM PAIR ('f', 'l'), casefl,
	                RMM PAIR ('e', 'x'), done,
	                RMM PAIR ('s', 's'), casess,
	                RMM PAIR ('f', 'p'), casefp,
	                RMM PAIR ('c', 's'), casecs,
	                RMM PAIR ('b', 'd'), casebd,
	                RMM PAIR ('l', 'g'), caselg,
	                RMM PAIR ('h', 'c'), casehc,
	                RMM PAIR ('h', 'y'), casehy,
	                RMM PAIR ('n', 'h'), casenh,
	                RMM PAIR ('n', 'm'), casenm,
	                RMM PAIR ('n', 'n'), casenn,
	                RMM PAIR ('s', 'v'), casesv,
	                RMM PAIR ('o', 's'), caseos,
	                RMM PAIR ('l', 's'), casels,
	                RMM PAIR ('c', 'c'), casecc,
	                RMM PAIR ('c', '2'), casec2,
	                RMM PAIR ('e', 'm'), caseem,
	                RMM PAIR ('a', 'f'), caseaf,
	                RMM PAIR ('h', 'w'), casehw,
	                RMM PAIR ('m', 'c'), casemc,
	                RMM PAIR ('p', 'm'), casepm,
	                RMM PAIR ('p', 'i'), casepi,
	                RMM PAIR ('u', 'f'), caseuf,
	                RMM PAIR ('p', 'c'), casepc,
	                RMM PAIR ('h', 't'), caseht,
	                RMM PAIR ('c', 'f'), casecf
};
#undef RMM

/*
 *  troff environment block
 */

int     block = 0;
int     ics = ICS;
int     ic = 0;
int     icf = 0;
int     chbits = 0;
int     spbits = 0;
int     nmbits = 0;
int     apts = PS;
int     apts1 = PS;
int     pts = PS;
int     pts1 = PS;
int     font = FT;
int     font1 = FT;
int     sps = SPS;
int     spacesz = SS;
int     lss = VS;
int     lss1 = VS;
int     ls = 1;
int     ls1 = 1;
int     ll = LL;
int     ll1 = LL;
int     lt = LL;
int     lt1 = LL;
int     ad = 1;
int     nms = 1;
int     ndf = 1;
int     fi = 1;
int     cc = '.';
int     c2 = '\'';
int     ohc = OHC;
int     tdelim = IMP;
int     hyf = 1;
int     hyoff = 0;
int     un1 = -1;
int     tabc = 0;
int     dotc = '.';
int     adsp = 0;
int     adrem = 0;
int     lastl = 0;
int     nel = 0;
int     admod = 0;
int    *wordp = 0;
int     spflg = 0;
int    *linep = 0;
int    *wdend = 0;
int    *wdstart = 0;
int     wne = 0;
int     ne = 0;
int     nc = 0;
int     nb = 0;
int     lnmod = 0;
int     nwd = 0;
int     nn = 0;
int     ni = 0;
int     ul = 0;
int     cu = 0;
int     ce = 0;
int     in = 0;
int     in1 = 0;
int     un = 0;
int     wch = 0;
int     pendt = 0;
int    *pendw = 0;
int     pendnf = 0;
int     spread = 0;
int     it = 0;
int     itmac = 0;
int     lnsize = LNSIZE;
int    *hyptr[NHYP] = {
	0
};
int     tabtab[NTAB] = {
	DTAB, DTAB * 2, DTAB * 3, DTAB * 4, DTAB * 5, DTAB * 6, DTAB * 7, DTAB * 8,
	DTAB * 9, DTAB * 10, DTAB * 11, DTAB * 12, DTAB * 13, DTAB * 14, DTAB * 15, 0
};
int     line[LNSIZE] = {
	0
};
int     word[WDSIZE] = {
	0
};
int     blockxxx[EVS - 68 - NHYP - NTAB - WDSIZE - LNSIZE] = {
	0
};
/*spare 5 words*/
int     oline[LNSIZE + 1];
