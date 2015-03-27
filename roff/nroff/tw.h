/* typewriter driving table structure*/
struct {
	int     bset;
	int     breset;
	int     Hor;
	int     Vert;
	int     Newline;
	int     Char;
	int     Em;
	int     Halfline;
	int     Adj;
	int     Bnomove;	/* don't move with bold */
	int     Inomove;	/* don't move with italic */
	char   *twinit;
	char   *twrest;
	char   *twnl;
	char   *hlr;
	char   *hlf;
	char   *flr;
	char   *bdon;
	char   *bdoff;
	char   *iton;
	char   *itoff;
	char   *ploton;
	char   *plotoff;
	char   *up;
	char   *down;
	char   *right;
	char   *left;
#ifndef ASA
	char   *codetab[256 - 32];
#else
	char   *codetab[512 - 32];
#endif
	char   *zzz;
}       t;
