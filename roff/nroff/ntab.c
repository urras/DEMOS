#define BYTE 8
#define PAIR(A,B) (A|(B<<BYTE))

/*
 * character name tables
 */

int     chtab[] = {
	PAIR ('h', 'y'), 0200,	/* hyphen */
	PAIR ('b', 'u'), 0201,	/* bullet */
	PAIR ('s', 'q'), 0202,	/* square */
	PAIR ('e', 'm'), 0203,	/* 3/4em */
	PAIR ('r', 'u'), 0204,	/* rule */
	PAIR ('1', '4'), 0205,	/* 1/4 */
	PAIR ('1', '2'), 0206,	/* 1/2 */
	PAIR ('3', '4'), 0207,	/* 3/4 */
	PAIR ('m', 'i'), 0210,  /* equation minus */
	PAIR ('f', 'i'), 0211,	/* fi */
	PAIR ('f', 'l'), 0212,	/* fl */
	PAIR ('f', 'f'), 0213,	/* ff */
	PAIR ('F', 'i'), 0214,	/* ffi */
	PAIR ('F', 'l'), 0215,	/* ffl */
	PAIR ('d', 'e'), 0216,	/* degree */
	PAIR ('d', 'g'), 0217,	/* dagger */
	PAIR ('s', 'c'), 0220,	/* section */
	PAIR ('f', 'm'), 0221,	/* foot mark */
	PAIR ('a', 'a'), 0222,	/* acute accent */
	PAIR ('g', 'a'), 0223,	/* grave accent */
	PAIR ('u', 'l'), 0224,	/* underrule */
	PAIR ('s', 'l'), 0225,	/* slash (longer) */
				/* 0226 half narrow space*/
				/* 0227 unpaddable space*/
	PAIR ('*', 'a'), 0230,	/* alpha */
	PAIR ('*', 'b'), 0231,	/* beta */
	PAIR ('*', 'g'), 0232,	/* gamma */
	PAIR ('*', 'd'), 0233,	/* delta */
	PAIR ('*', 'e'), 0234,	/* epsilon */
	PAIR ('*', 'z'), 0235,	/* zeta */
	PAIR ('*', 'y'), 0236,	/* eta */
	PAIR ('*', 'h'), 0237,	/* theta */
	PAIR ('*', 'i'), 0240,	/* iota */
	PAIR ('*', 'k'), 0241,	/* kappa */
	PAIR ('*', 'l'), 0242,	/* lambda */
	PAIR ('*', 'm'), 0243,	/* mu */
	PAIR ('*', 'n'), 0244,	/* nu */
	PAIR ('*', 'c'), 0245,	/* xi */
	PAIR ('*', 'o'), 0246,	/* omicron */
	PAIR ('*', 'p'), 0247,	/* pi */
	PAIR ('*', 'r'), 0250,	/* rho */
	PAIR ('*', 's'), 0251,	/* sigma */
	PAIR ('*', 't'), 0252,	/* tau */
	PAIR ('*', 'u'), 0253,	/* upsilon */
	PAIR ('*', 'f'), 0254,	/* phi */
	PAIR ('*', 'x'), 0255,	/* chi */
	PAIR ('*', 'q'), 0256,	/* psi */
	PAIR ('*', 'w'), 0257,	/* omega */
	PAIR ('*', 'A'), 0101,	/* Alpha */
	PAIR ('*', 'B'), 0102,	/* Beta */
	PAIR ('*', 'G'), 0260,	/* Gamma */
	PAIR ('*', 'D'), 0261,	/* Delta */
	PAIR ('*', 'E'), 0105,	/* Epsilon */
	PAIR ('*', 'Z'), 0132,	/* Zeta */
	PAIR ('*', 'Y'), 0110,	/* Eta */
	PAIR ('*', 'H'), 0262,	/* Theta */
	PAIR ('*', 'I'), 0111,	/* Iota */
	PAIR ('*', 'K'), 0113,	/* Kappa */
	PAIR ('*', 'L'), 0263,	/* Lambda */
	PAIR ('*', 'M'), 0115,	/* Mu */
	PAIR ('*', 'N'), 0116,	/* Nu */
	PAIR ('*', 'C'), 0264,	/* Xi */
	PAIR ('*', 'O'), 0117,  /* Omicron */
	PAIR ('*', 'P'), 0265,	/* Pi */
	PAIR ('*', 'R'), 0120,	/* Rho */
	PAIR ('*', 'S'), 0266,	/* Sigma */
	PAIR ('*', 'T'), 0124,	/* Tau */
	PAIR ('*', 'U'), 0270,	/* Upsilon */
	PAIR ('*', 'F'), 0271,	/* Phi */
	PAIR ('*', 'X'), 0130,	/* Chi */
	PAIR ('*', 'Q'), 0272,	/* Psi */
	PAIR ('*', 'W'), 0273,	/* Omega */
	PAIR ('s', 'r'), 0274,	/* square root */
	PAIR ('t', 's'), 0275,	/* terminal sigma */
	PAIR ('r', 'n'), 0276,	/* root en */
	PAIR ('>', '='), 0277,	/* >= */
	PAIR ('<', '='), 0400,  /* <= */
	PAIR ('=', '='), 0401,  /* identically equal */
	PAIR ('~', '='), 0403,  /* approx = */
	PAIR ('a', 'p'), 0404,  /* approximates */
	PAIR ('!', '='), 0405,  /* not equal */
	PAIR ('-', '>'), 0406,  /* right arrow */
	PAIR ('<', '-'), 0407,  /* left arrow */
	PAIR ('u', 'a'), 0410,  /* up arrow */
	PAIR ('d', 'a'), 0411,  /* down arrow */
	PAIR ('e', 'q'), 0412,  /* equation equal */
	PAIR ('m', 'u'), 0413,  /* multiply */
	PAIR ('d', 'i'), 0414,  /* divide */
	PAIR ('+', '-'), 0415,  /* plus-minus */
	PAIR ('c', 'u'), 0416,  /* cup (union) */
	PAIR ('c', 'a'), 0417,  /* cap (intersection) */
	PAIR ('s', 'b'), 0420,  /* subset of */
	PAIR ('s', 'p'), 0421,  /* superset of */
	PAIR ('i', 'b'), 0422,  /* improper subset */
	PAIR ('i', 'p'), 0423,  /*  " superset */
	PAIR ('i', 'f'), 0424,  /* infinity */
	PAIR ('p', 'd'), 0425,  /* partial derivative */
	PAIR ('g', 'r'), 0426,  /* gradient */
	PAIR ('n', 'o'), 0427,  /* not */
	PAIR ('i', 's'), 0430,  /* integral sign */
	PAIR ('p', 't'), 0431,  /* proportional to */
	PAIR ('e', 's'), 0432,  /* empty set */
	PAIR ('m', 'o'), 0433,  /* member of */
	PAIR ('p', 'l'), 0434,  /* equation plus */
	PAIR ('r', 'g'), 0435,  /* registered */
	PAIR ('c', 'o'), 0436,  /* copyright */
	PAIR ('b', 'r'), 0437,  /* box vert rule */
	PAIR ('c', 't'), 0440,  /* cent sign */
	PAIR ('d', 'd'), 0441,  /* dbl dagger */
	PAIR ('r', 'h'), 0442,  /* right hand */
	PAIR ('l', 'h'), 0443,  /* left hand */
	PAIR ('*', '*'), 0444,  /* math * */
	PAIR ('b', 's'), 0445,  /* bell system sign */
	PAIR ('o', 'r'), 0446,  /* or */
	PAIR ('c', 'i'), 0447,  /* circle */
	PAIR ('l', 't'), 0450,  /* left top (of big curly) */
	PAIR ('l', 'b'), 0451,  /* left bottom */
	PAIR ('r', 't'), 0452,  /* right top */
	PAIR ('r', 'b'), 0453,  /* right bot */
	PAIR ('l', 'k'), 0454,  /* left center of big curly
				   bracket */
	PAIR ('r', 'k'), 0455,  /* right center of big curly
				   bracket */
	PAIR ('b', 'v'), 0456,  /* bold vertical */
	PAIR ('l', 'f'), 0457,  /* left floor (left bot of big
				   sq bract) */
	PAIR ('r', 'f'), 0460,  /* right floor (rb of ") */
	PAIR ('l', 'c'), 0461,  /* left ceiling (lt of ") */
	PAIR ('r', 'c'), 0462,  /* right ceiling (rt of ") */
	0, 0
};
