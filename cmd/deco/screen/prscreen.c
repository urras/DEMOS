# include <stdio.h>

# define EPSON

# ifdef VT100
#       define NORM             "\33[m"
#       define BOLD             "\33[1m"
#       define GRAPH            "\33(0"
#       define UNGRAPH          "\33(B"
#       define REV              "\33[7m"
#       define GTAB             "qxmvjtnulwk"
#       define SYMBOL(c)        (c)
# endif

# ifdef EPSON
#       define GTAB             "\315\272\310\312\274\314\316\271\311\313\273"
#       define SYMBOL(c)        koitoecod (c)
# endif

# ifndef SYMBOL
#       define SYMBOL(c)        (c)
# endif
# ifndef GTAB
#       define GTAB             "-|+++++++++"
# endif
# ifndef REV
#       define REV              ""
# endif
# ifndef UNGRAPH
#       define UNGRAPH          ""
# endif
# ifndef GRAPH
#       define GRAPH            ""
# endif
# ifndef NORM
#       define NORM             ""
# endif
# ifndef BOLD
#       define BOLD             NORM
# endif
# ifndef DIM
#       define DIM              NORM
# endif
# ifndef INIT
#       define INIT             ""
# endif
# ifndef RESTORE
#       define RESTORE          ""
# endif

# define cntrl(c) (c & 037)

int graphmode;

static unsigned char local1 [48] = {
        0341, 0342, 0367, 0347, 0344, 0345, 0366, 0372,	/* ¡¬◊«ƒ≈÷⁄ */
	0351, 0352, 0353, 0354, 0355, 0356, 0357, 0360, /* … ÀÃÕŒœ– */
	0362, 0363, 0364, 0365, 0346, 0350, 0343, 0376, /* “”‘’∆»√ﬁ */
	0373, 0375, 0277, 0371, 0370, 0374, 0340, 0361, /* €›ﬂŸÿ‹¿— */
        0301, 0302, 0327, 0307, 0304, 0305, 0326, 0332,	/* ¡¬◊«ƒ≈÷⁄ */
	0311, 0312, 0313, 0314, 0315, 0316, 0317, 0320, /* … ÀÃÕŒœ– */
};

static unsigned char local2 [16] = {
	0322, 0323, 0324, 0325, 0306, 0310, 0303, 0336, /* “”‘’∆»√ﬁ */
	0333, 0335, 0337, 0331, 0330, 0334, 0300, 0321, /* €›ﬂŸÿ‹¿— */
};

static unsigned char local [64] = {
	0356,	0240,	0241,	0346,	0244,	0245,	0344,	0243,
	0345,	0250,	0251,	0252,	0253,	0254,	0255,	0256,
	0257,	0357,	0340,	0341,	0342,	0343,	0246,	0242,
	0354,	0353,	0247,	0350,	0355,	0351,	0347,	0352,
	0236,	0200,	0201,	0226,	0204,	0205,	0224,	0203,
	0225,	0210,	0211,	0212,	0213,	0214,	0215,	0216,
	0217,	0237,	0220,	0221,	0222,	0223,	0206,	0202,
	0234,	0233,	0207,	0230,	0235,	0231,	0227,
};

ecodtokoi (c)
register c;
{
	c &= 0377;
	if (c < 0200 | c == 0377)
		;
	else if (c >= 0360)
		c -= 0100;
	else if (c >= 0340)
		c = local2 [c - 0340];
	else if (c >= 0260)
		c -= 060;
	else if (c >= 0200)
		c = local1 [c - 0200];
	return (c);
}

koitoecod (c)
register c;
{
	c &= 0377;
	if (c < 0200 || c == 0377)
		;
	else if (c >= 0300)
		c = local [c - 0300];
	else if (c >= 0260)
		c += 0100;
	else if (c >= 0200)
		c += 060;
	return (c);
}

main (argc, argv)
char **argv;
{
	register c;

	if (argc != 2) {
		fprintf (stderr, "Usage: %s filename\n", argv [0]);
		exit (1);
	}
	if (!freopen (argv [1], "r", stdin)) {
		fprintf (stderr, "Cannot open '%s'\n", argv [1]);
		exit (2);
	}
	printf (INIT);
	while ((c = getchar ()) != EOF)
		prchar (c);
	printf (UNGRAPH);
	printf (NORM);
	printf (RESTORE);
	return (0);
}

prchar (c)
{
	static r = 0, l = 2;

	switch (c) {
	case 1:
		printf (DIM);
		if (r)
			printf (REV);
		l = 1;
		break;
	case 2:
		printf (NORM);
		if (r)
			printf (REV);
		l = 2;
		break;
	case 3:
		printf (BOLD);
		if (r)
			printf (REV);
		l = 3;
		break;
	case cntrl ('['):
		printf (GRAPH);
		graphmode = 1;
		break;
	case cntrl ('\\'):
		printf (UNGRAPH);
		graphmode = 0;
		if (l == 1)
			printf (DIM);
		else if (l == 3)
			printf (BOLD);
		if (r)
			printf (REV);
		break;
	case cntrl ('n'):
		printf (REV);
		r = 1;
		break;
	case cntrl ('o'):
		printf (NORM);
		if (l == 1)
			printf (DIM);
		else if (l == 3)
			printf (BOLD);
		r = 0;
		break;
	case '\n':
	case ' ':
		putchar (c);
		break;
	default:
		putchar (graphmode ? graphsym (c) : SYMBOL (c));
		break;
	}
}

graphsym (c)
{
	switch (c) {
	case '-':
		return (GTAB [0]);
	case '|':
		return (GTAB [1]);
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return (GTAB [c-'1'+2]);
	}
	return ('?');
}
