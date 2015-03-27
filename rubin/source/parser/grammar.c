
# line 15 "grammar.z"
/* SCANNER/PARSER GLOBALS & TABLES */
# include	<defines.h>
# include	<aux.h>
# include	<tree.h>
# include	<symbol.h>
# include	<pv.h>
# include	"parser.h"
# include	<sccs.h>

SCCSID(@(#)grammar.y	7.2	4/7/82)

# ifdef		xPTR1
# define        YYDEBUG         1
# endif

int				i;
struct atstash			*aptr;
char				permbuf[3];
/* space for two names, their null bytes and the seperator */
char				modbuf[(2 * (MAXNAME + 1)) + 1];
static char			hqmbuf[2];

extern DESC			Reldesc;
extern int			Opflag;
extern QTREE			*Lastree;
extern QTREE			*Tidnode;
extern int			Rsdmno;
extern int			Resrng;
extern int			Qrymod;
extern int			Permcomd;
extern char			*Trname;
extern int			Qlflag;
extern struct atstash 		Faketid;

# ifdef	DISTRIB
extern struct atstash		Fakesid;
# endif

extern int			Patflag;
extern char			*Indexname;

extern QTREE			*tree();
extern QTREE			*tlprepend();
extern QTREE			*addresdom();
extern QTREE			*xdot();
extern QTREE			*norml();
extern struct atstash		*attlookup();
extern int			rngent();
extern int			rnglook();
extern PARRNG			Parrng[];

# line 71 "grammar.z"
typedef union 
{
	int				type_type;	/* OPERATOR TYPES ETC. */
	QTREE				*tree_type;
	int				rng_type;
	char				char_type;
	int				int_type;
	short				*I2_type;
	long				*I4_type;
	float				*F4_type;
	double				*F8_type;
	char				*string_type;
} YYSTYPE;
# define APPEND 257
# define COPY 258
# define CREATE 259
# define DELETE 260
# define DESTROY 261
# define HELP 262
# define INDEX 263
# define MODIFY 264
# define PRINT 265
# define RANGE 266
# define REPLACE 267
# define RETRIEVE 268
# define SAVE 269
# define DEFINE 270
# define PERMIT 271
# define VIEW 272
# define INTEGRITY 273
# define ALL 274
# define BY 275
# define FROM 276
# define IN 277
# define INTO 278
# define UNIQUE 279
# define AT 280
# define IS 281
# define OF 282
# define ON 283
# define ONTO 284
# define TO 285
# define UNTIL 286
# define WHERE 287
# define NAME 288
# define SCONST 289
# define I2CONST 290
# define I4CONST 291
# define F4CONST 292
# define F8CONST 293
# define DATECONST 294
# define TIMECONST 295
# define COMMA 296
# define LPAREN 297
# define PERIOD 298
# define RPAREN 299
# define COLON 300
# define BGNCMNT 301
# define ENDCMNT 302
# define UAOP 303
# define BAOP 304
# define BAOPH 305
# define BDOP 306
# define EOP 307
# define LBOP 308
# define LUOP 309
# define FOP 310
# define FBOP 311
# define AGOP 312
# define unaryop 313
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
typedef int yytabelem;
# define YYERRCODE 256

# line 1142 "grammar.z"

# include	"scanner.h"
# include	"tables.y"
# include	"yyerror.y"
yytabelem yyexca[] ={
-1, 0,
	0, 3,
	-2, 0,
-1, 1,
	0, -1,
	-2, 0,
-1, 38,
	297, 48,
	-2, 52,
-1, 208,
	298, 135,
	-2, 143,
	};
# define YYNPROD 215
# define YYLAST 520
yytabelem yyact[]={

   194,   327,   152,   208,   207,   201,   202,   203,   204,   205,
   206,   297,   191,   238,   243,   335,   244,   243,   210,   245,
   244,   243,   247,   320,   192,   198,   199,   200,   208,   207,
   201,   202,   203,   204,   205,   206,   247,   250,   319,   345,
   284,   310,   334,   210,   245,   244,   243,   248,   246,    61,
   198,   199,   200,   154,   300,   283,   273,   340,   245,   244,
   243,   248,   246,   329,   238,   245,   244,   243,   220,   245,
   244,   243,   309,   280,    68,    69,   279,   254,   217,   245,
   244,   243,   308,   154,    96,   253,   245,   244,   243,   114,
   284,   252,   337,    86,   245,   244,   243,   223,   213,   127,
   224,   212,   173,   145,   144,   251,   146,    85,    87,   116,
   179,   130,   129,    90,   314,   136,   299,   142,   221,   137,
   134,   127,   264,   331,   332,    81,    82,    62,    86,   189,
   118,   117,   119,   346,   169,    79,   344,   339,   324,    62,
   266,   306,    85,    87,   278,   189,   170,   120,    76,   293,
    75,    78,   184,    62,    60,   305,   257,   342,   277,   333,
   298,   317,   274,    62,   234,    62,   219,   258,   259,   157,
   258,   235,   126,   177,   172,   161,   147,   162,   133,   166,
   168,   132,    89,    64,   154,   271,   236,   151,    67,    65,
    66,   343,   240,   112,   111,   338,   231,    94,   249,   140,
   113,   187,    95,    92,    93,   107,   106,    73,    71,   321,
   229,   108,   124,   315,    72,   256,    21,    42,    43,    44,
    45,    46,    48,    56,    50,    41,    51,    52,    53,    54,
    47,   216,   104,   218,   182,   175,   160,   215,   233,   276,
   262,   304,   261,   285,   286,   287,   288,   102,   190,   214,
   101,   289,   290,   291,   292,   188,   211,   103,   100,   122,
   121,   123,   209,   156,    99,   115,   125,    84,    98,    59,
   186,   263,   265,   267,     3,   268,    58,     2,   185,    57,
    39,    22,   296,   270,   272,   228,   269,   227,   176,    34,
    49,    32,    88,    31,    83,   311,    30,    74,    77,    28,
    27,    25,   167,   165,   318,   260,   164,    24,   153,    80,
   325,   328,    29,   174,    91,    33,   303,   302,   143,   301,
   275,   230,   141,    97,    35,    55,   330,   323,    40,   336,
   128,   110,   109,    38,   105,    37,    70,   131,   328,   341,
    26,    63,   322,    23,   265,   313,    36,    20,    19,    18,
    17,   135,    16,    15,    14,    13,   316,    12,   139,    11,
   138,    10,     9,     8,     7,     6,     5,     4,   197,   196,
   326,   242,   148,   159,   195,   149,   193,   150,   155,   158,
   232,   178,     1,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   163,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   171,     0,     0,     0,     0,     0,
   181,   180,     0,     0,   183,     0,   222,   312,   225,     0,
     0,     0,     0,   226,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   239,   241,   237,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   159,     0,     0,     0,     0,     0,   158,   255,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   282,     0,     0,
     0,     0,     0,   281,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   294,     0,     0,     0,   295,     0,   307 };
yytabelem yypact[]={

   -40,   -40, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
 -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
 -1000, -1000,  -135,   -95,  -125,  -125,   -69,  -123,  -139,  -165,
  -146,  -106,  -184,   -80,  -125,   -10,   -50,   -72,   -85,  -125,
  -188, -1000, -1000, -1000, -1000, -1000, -1000,  -141,   -12,   -71,
 -1000, -1000, -1000, -1000, -1000,  -116, -1000, -1000, -1000,  -175,
 -1000, -1000, -1000,  -116, -1000, -1000, -1000, -1000,  -185,  -186,
  -116, -1000, -1000, -1000,  -175,  -107,  -110,  -176, -1000, -1000,
 -1000, -1000, -1000,  -181, -1000, -1000, -1000, -1000,  -177, -1000,
  -125,  -116, -1000, -1000, -1000, -1000,   -86,  -179, -1000, -1000,
 -1000, -1000, -1000, -1000,  -112,  -116, -1000, -1000, -1000,  -188,
  -116, -1000, -1000, -1000,   -99,  -103,  -119, -1000, -1000, -1000,
   -45, -1000, -1000, -1000,  -113, -1000, -1000,  -125,  -188,  -125,
  -125,  -103, -1000, -1000,  -144, -1000,  -146,  -114,  -197,   -46,
  -115,  -187,   -10,  -116, -1000, -1000, -1000,   -47,  -188,  -103,
 -1000,  -161, -1000,  -285, -1000,  -198, -1000,   -44, -1000,  -220,
 -1000,   -48, -1000,  -103,  -231,  -178,   -44,  -199,   -44, -1000,
 -1000, -1000, -1000, -1000,  -285, -1000,   -73, -1000,   -89,  -124,
 -1000, -1000,  -117,  -103, -1000, -1000,  -145, -1000, -1000, -1000,
  -295,  -285,  -285, -1000,  -245, -1000, -1000,  -260,  -192,  -206,
  -212, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
 -1000,  -221, -1000,  -119,  -260, -1000, -1000,  -118,  -120, -1000,
   -36,  -125,  -149,  -125, -1000,  -125,  -295,  -102,  -126, -1000,
   -41,  -130,  -223, -1000, -1000, -1000, -1000,  -145,  -285,  -244,
  -259, -1000,  -260,  -260,  -260,  -260, -1000, -1000, -1000, -1000,
  -260,  -260,  -260,  -260,  -121, -1000,  -238, -1000, -1000, -1000,
  -140, -1000, -1000,   -44, -1000, -1000, -1000,   -44, -1000, -1000,
  -128, -1000,  -180, -1000,  -246,   -35,  -133, -1000, -1000, -1000,
  -124, -1000, -1000, -1000, -1000,  -238, -1000,  -291,  -288,  -209,
  -217,  -224,  -234, -1000,  -149,  -125,  -182, -1000,   -68,  -126,
  -127,  -103,   -74,   -74,  -152, -1000, -1000, -1000, -1000,  -260,
  -260,  -236, -1000, -1000,  -128,  -167, -1000, -1000, -1000, -1000,
 -1000,  -129, -1000, -1000,  -258,  -284,  -204, -1000,  -238, -1000,
 -1000, -1000, -1000,   -90,  -153, -1000,  -242,  -260,  -131,   -94,
 -1000, -1000, -1000,  -154,  -261,  -157, -1000 };
yytabelem yypgo[]={

     0,   382,   381,   380,   238,   265,   378,   263,     2,   248,
   376,     0,   374,   371,   370,     1,   262,   256,   369,   368,
    49,   277,   274,   367,   366,   365,   364,   363,   362,   361,
   359,   357,   355,   354,   353,   352,   350,   349,   348,   347,
   346,   343,   341,   266,   340,   336,   335,   334,   333,   332,
   331,   328,   325,   324,   323,   322,   321,   320,   319,   268,
   318,   317,    38,   316,    23,   315,   314,   313,   312,   309,
   249,   308,   307,   306,   305,   303,   122,   302,   301,   300,
   269,   299,   298,   296,   294,   293,   292,   267,   291,   290,
   289,   288,   287,   286,   285,   284,    56,   283,   282,    11,
   281,   280,   278,   270,   255 };
yytabelem yyr1[]={

     0,     1,     1,     1,     1,     1,    21,    21,    21,    21,
    21,    21,    21,    21,    21,    21,    21,    21,    21,    21,
    21,    21,    21,    21,    35,    40,    23,    41,    42,    42,
    42,    42,    42,    26,    44,    45,    45,    45,    45,    36,
    46,    47,    47,    47,    47,    37,    48,    49,    49,    49,
    50,    50,    50,    39,    51,    52,    33,    53,    54,    54,
    59,    59,    59,    59,    59,    55,    60,    60,    60,     2,
     2,     3,     3,     4,    56,    56,    57,    57,    57,    58,
    58,    58,    58,    63,    64,    61,    62,    31,    65,    66,
    66,    66,    66,    66,    67,    67,    28,    68,    69,    69,
    43,     5,     6,     6,     7,     7,     7,    70,    70,     8,
     8,    71,     9,     9,     9,     9,    10,    13,    13,    13,
    11,    11,    11,    11,    11,    11,    11,    11,    11,    12,
    12,    14,    14,    15,    16,    17,    18,    18,    18,    18,
    18,    18,    18,    18,    18,    19,    24,    72,    73,    73,
    75,    75,    76,    76,    20,    77,    77,    74,    74,    25,
    78,    27,    27,    79,    81,    81,    82,    82,    82,    29,
    29,    29,    83,    85,    85,    85,    84,    84,    84,    87,
    87,    87,    86,    86,    30,    88,    89,    32,    90,    91,
    92,    92,    94,    95,    95,    96,    96,    93,    93,    97,
    98,    98,    99,    99,    80,    80,    34,   100,    22,    38,
   101,   102,   103,   103,   104 };
yytabelem yyr2[]={

     0,     5,     3,     1,     4,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     3,    11,     3,    11,     3,     2,     2,
     2,     2,     0,     9,     3,     2,     2,     2,     0,    11,
     3,     2,     2,     2,     0,     9,     3,     5,     1,     3,
     2,     2,     0,     7,     5,     5,    17,     5,     2,     6,
     3,     3,     3,     3,     3,     5,     2,     2,     2,     7,
     1,     2,     7,     3,     5,     5,     5,     5,     1,     4,
     4,     4,     4,     1,     1,    17,     9,    11,     5,     2,
     2,     2,     2,     0,     2,     0,     4,     7,     3,     3,
     3,     7,     2,     7,     7,     3,     7,     2,     2,     5,
     1,     3,     7,     5,     7,     2,     7,     2,     2,     2,
     2,     2,     7,     7,     7,     7,     5,     9,    13,    15,
    11,     2,     7,     3,     7,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     2,     3,    15,     3,     2,     0,
     6,    10,     2,     3,     3,     6,    10,     3,     3,    10,
     3,     4,     4,     3,     7,     7,     3,     7,     2,     4,
     3,     4,     3,     5,     5,     5,     2,     4,     6,     3,
     3,     3,     3,     7,     9,    11,     3,    12,     3,     3,
     4,     0,     3,     2,     6,     3,     7,     4,     0,     3,
     2,     6,     7,     7,     3,     7,     4,     3,     5,     8,
     3,     6,     2,     2,     3 };
yytabelem yychk[]={

 -1000,    -1,   -21,   -22,   -23,   -24,   -25,   -26,   -27,   -28,
   -29,   -30,   -31,   -32,   -33,   -34,   -35,   -36,   -37,   -38,
   -39,   256,  -100,   -41,   -72,   -78,   -44,   -79,   -81,   -68,
   -83,   -85,   -88,   -65,   -90,   -53,   -40,   -46,   -48,  -101,
   -51,   265,   257,   258,   259,   260,   261,   270,   262,   -89,
   264,   266,   267,   268,   269,   -52,   263,   -21,   -22,   -80,
   289,   -20,   288,   -42,   278,   284,   285,   283,   -20,   -20,
   -45,   277,   283,   276,   -80,   273,   271,   -82,   290,   274,
   -69,   290,   291,   -84,   -87,   288,   274,   289,   -86,   288,
   297,   -66,   283,   284,   277,   282,   -20,   -54,   -59,   274,
   268,   260,   257,   267,   282,   -47,   278,   277,   283,   -49,
   -50,   279,   278,   285,   -20,    -5,   297,   272,   271,   273,
   288,   272,   271,   273,   283,   -43,   288,   296,   -43,   297,
   297,   -43,   288,   288,   296,   -87,   296,   296,   -80,   -43,
   285,   -55,   296,   -60,   283,   282,   285,   288,   -43,    -5,
   -43,   286,    -8,   -71,   287,    -6,    -7,   288,   -16,   -17,
   281,   288,   -20,    -5,   -73,   -75,   -20,   -77,   -20,    -8,
   290,   -87,   288,   299,   -67,   281,   -91,   288,    -2,   297,
   -59,   -43,   281,    -5,    -8,  -102,  -103,   -20,  -104,   290,
    -9,   297,   309,   -10,   -11,   -12,   -18,   -19,   310,   311,
   312,   290,   291,   292,   293,   294,   295,   289,   288,   -16,
   303,   -17,   299,   296,   -70,   281,   275,   298,   281,    -8,
   299,   296,   -70,   296,   299,   -70,    -9,   -92,   -94,   283,
   -56,   285,    -3,    -4,   288,   288,    -8,  -104,   308,    -9,
   -11,    -9,   -13,   305,   304,   303,   307,   281,   306,   -11,
   297,   297,   297,   297,   298,    -7,   -11,   274,   288,   288,
   -74,   278,   276,   -20,   -76,   -20,   289,   -20,   -20,   -93,
   -97,   287,   -95,   -96,   288,   -57,   280,   288,   274,   299,
   296,  -104,    -9,   299,   299,   -11,   -11,   -11,   -11,   -11,
   -11,   -11,   -11,   289,   -70,   -70,   -98,   -99,   288,   296,
   300,   -58,   -61,   -63,   276,   288,   274,    -4,   299,   296,
   275,    -8,   -76,   -20,   296,   281,   -96,   288,    -8,   -62,
   -64,   283,   -62,   -64,   290,   -11,   -14,   -15,   -11,   299,
   -99,   290,   291,   288,   300,   299,    -8,   296,   285,   290,
   299,   -15,   288,   285,   290,   300,   290 };
yytabelem yydef[]={

    -2,    -2,     2,     5,     6,     7,     8,     9,    10,    11,
    12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
    22,    23,     0,    32,     0,     0,    38,     0,     0,     0,
   170,     0,     0,    93,     0,     0,     0,    44,    -2,     0,
     0,   207,    27,   147,   160,    34,   163,     0,   172,     0,
   188,    25,    40,    46,   210,     0,   186,     1,     4,   206,
   208,   204,   154,     0,    28,    29,    30,    31,     0,     0,
     0,    35,    36,    37,   161,     0,     0,   162,   166,   168,
    96,    98,    99,   169,   176,   179,   180,   181,   171,   182,
     0,     0,    89,    90,    91,    92,     0,     0,    58,    60,
    61,    62,    63,    64,     0,     0,    41,    42,    43,     0,
     0,    49,    50,    51,     0,   110,     0,    55,    57,    88,
     0,   173,   174,   175,     0,    54,   100,     0,     0,   149,
     0,   110,   164,   165,     0,   177,     0,     0,     0,    95,
     0,    70,     0,     0,    66,    67,    68,     0,     0,   110,
    47,     0,    53,     0,   111,     0,   102,   135,   105,     0,
    97,     0,   205,   110,     0,   148,     0,     0,     0,    33,
   167,   178,   183,   184,     0,    94,   191,   189,     0,     0,
    59,    65,     0,   110,    45,   209,     0,   212,   213,   214,
   109,     0,     0,   115,     0,   120,   121,     0,     0,     0,
     0,   136,   137,   138,   139,   140,   141,   142,    -2,   144,
   145,     0,   101,     0,     0,   107,   108,     0,     0,    26,
     0,     0,     0,     0,   159,     0,    87,   198,     0,   192,
    78,     0,     0,    71,    73,    24,    39,     0,     0,     0,
     0,   113,     0,     0,     0,     0,   117,   118,   119,   126,
     0,     0,     0,     0,     0,   103,   104,   106,   134,   185,
     0,   157,   158,     0,   150,   152,   153,     0,   155,   187,
     0,   199,   190,   193,   195,    83,     0,    74,    75,    69,
     0,   211,   114,   112,   125,   116,   122,   123,   124,     0,
     0,     0,   110,   146,     0,     0,   197,   200,     0,     0,
     0,   110,    84,    84,     0,    76,    77,    72,   127,     0,
     0,     0,   151,   156,     0,     0,   194,   196,    56,    79,
    81,     0,    80,    82,     0,     0,   110,   131,   133,   130,
   201,   202,   203,     0,     0,   128,     0,     0,     0,     0,
   129,   132,    86,     0,     0,     0,    85 };
typedef struct { char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

yytoktype yytoks[] =
{
	"APPEND",	257,
	"COPY",	258,
	"CREATE",	259,
	"DELETE",	260,
	"DESTROY",	261,
	"HELP",	262,
	"INDEX",	263,
	"MODIFY",	264,
	"PRINT",	265,
	"RANGE",	266,
	"REPLACE",	267,
	"RETRIEVE",	268,
	"SAVE",	269,
	"DEFINE",	270,
	"PERMIT",	271,
	"VIEW",	272,
	"INTEGRITY",	273,
	"ALL",	274,
	"BY",	275,
	"FROM",	276,
	"IN",	277,
	"INTO",	278,
	"UNIQUE",	279,
	"AT",	280,
	"IS",	281,
	"OF",	282,
	"ON",	283,
	"ONTO",	284,
	"TO",	285,
	"UNTIL",	286,
	"WHERE",	287,
	"NAME",	288,
	"SCONST",	289,
	"I2CONST",	290,
	"I4CONST",	291,
	"F4CONST",	292,
	"F8CONST",	293,
	"DATECONST",	294,
	"TIMECONST",	295,
	"COMMA",	296,
	"LPAREN",	297,
	"PERIOD",	298,
	"RPAREN",	299,
	"COLON",	300,
	"BGNCMNT",	301,
	"ENDCMNT",	302,
	"UAOP",	303,
	"BAOP",	304,
	"BAOPH",	305,
	"BDOP",	306,
	"EOP",	307,
	"LBOP",	308,
	"LUOP",	309,
	"FOP",	310,
	"FBOP",	311,
	"AGOP",	312,
	"unaryop",	313,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"program : program stmnt",
	"program : stmnt",
	"program : /* empty */",
	"program : program echo",
	"program : echo",
	"stmnt : append",
	"stmnt : copy",
	"stmnt : create",
	"stmnt : delete",
	"stmnt : destroy",
	"stmnt : defvar",
	"stmnt : help",
	"stmnt : index",
	"stmnt : integrity",
	"stmnt : modify",
	"stmnt : permit",
	"stmnt : print",
	"stmnt : range",
	"stmnt : replace",
	"stmnt : retrieve",
	"stmnt : save",
	"stmnt : view",
	"stmnt : error",
	"range : rngstmnt OF NAME IS NAME",
	"rngstmnt : RANGE",
	"append : apstmnt apto relation tlclause qualclause",
	"apstmnt : APPEND",
	"apto : INTO",
	"apto : ONTO",
	"apto : TO",
	"apto : ON",
	"apto : /* empty */",
	"delete : delstmnt delwd relation qualclause",
	"delstmnt : DELETE",
	"delwd : IN",
	"delwd : ON",
	"delwd : FROM",
	"delwd : /* empty */",
	"replace : repstmnt repkwd relation tlclause qualclause",
	"repstmnt : REPLACE",
	"repkwd : INTO",
	"repkwd : IN",
	"repkwd : ON",
	"repkwd : /* empty */",
	"retrieve : retstmnt retclause tlclause qualclause",
	"retstmnt : RETRIEVE",
	"retclause : retkwd relation",
	"retclause : /* empty */",
	"retclause : UNIQUE",
	"retkwd : INTO",
	"retkwd : TO",
	"retkwd : /* empty */",
	"view : viewclause tlclause qualclause",
	"viewclause : viewstmnt relation",
	"viewstmnt : DEFINE VIEW",
	"permit : permstmnt permlist permrel permtarg permwho permplace permtd qualclause",
	"permstmnt : DEFINE PERMIT",
	"permlist : permxlist",
	"permlist : permlist COMMA permxlist",
	"permxlist : ALL",
	"permxlist : RETRIEVE",
	"permxlist : DELETE",
	"permxlist : APPEND",
	"permxlist : REPLACE",
	"permrel : permword relation",
	"permword : ON",
	"permword : OF",
	"permword : TO",
	"permtarg : LPAREN permtlist RPAREN",
	"permtarg : /* empty */",
	"permtlist : permtlelm",
	"permtlist : permtlist COMMA permtlelm",
	"permtlelm : NAME",
	"permwho : TO NAME",
	"permwho : TO ALL",
	"permplace : AT NAME",
	"permplace : AT ALL",
	"permplace : /* empty */",
	"permtd : permtime permday",
	"permtd : permdeftime permday",
	"permtd : permtime permdefday",
	"permtd : permdeftime permdefday",
	"permdeftime : /* empty */",
	"permdefday : /* empty */",
	"permtime : FROM I2CONST COLON I2CONST TO I2CONST COLON I2CONST",
	"permday : ON NAME TO NAME",
	"integrity : integstmnt integnoise relation integis qual",
	"integstmnt : DEFINE INTEGRITY",
	"integnoise : ON",
	"integnoise : ONTO",
	"integnoise : IN",
	"integnoise : OF",
	"integnoise : /* empty */",
	"integis : IS",
	"integis : /* empty */",
	"defvar : deflet ipconst",
	"deflet : DEFINE NAME IS",
	"ipconst : I2CONST",
	"ipconst : I4CONST",
	"relation : NAME",
	"tlclause : LPAREN tlist RPAREN",
	"tlist : tlelm",
	"tlist : tlist COMMA tlelm",
	"tlelm : NAME is afcn",
	"tlelm : attrib",
	"tlelm : var PERIOD ALL",
	"is : IS",
	"is : BY",
	"qualclause : where qual",
	"qualclause : /* empty */",
	"where : WHERE",
	"qual : LPAREN qual RPAREN",
	"qual : LUOP qual",
	"qual : qual LBOP qual",
	"qual : clause",
	"clause : afcn relop afcn",
	"relop : EOP",
	"relop : IS",
	"relop : BDOP",
	"afcn : aggrfcn",
	"afcn : attribfcn",
	"afcn : afcn BAOPH afcn",
	"afcn : afcn BAOP afcn",
	"afcn : afcn UAOP afcn",
	"afcn : LPAREN afcn RPAREN",
	"afcn : uop afcn",
	"afcn : FOP LPAREN afcn RPAREN",
	"afcn : FBOP LPAREN afcn COMMA afcn RPAREN",
	"aggrfcn : AGOP LPAREN afcn BY domseq qualclause RPAREN",
	"aggrfcn : AGOP LPAREN afcn qualclause RPAREN",
	"domseq : targdom",
	"domseq : domseq COMMA targdom",
	"targdom : afcn",
	"attrib : var PERIOD NAME",
	"var : NAME",
	"attribfcn : I2CONST",
	"attribfcn : I4CONST",
	"attribfcn : F4CONST",
	"attribfcn : F8CONST",
	"attribfcn : DATECONST",
	"attribfcn : TIMECONST",
	"attribfcn : SCONST",
	"attribfcn : NAME",
	"attribfcn : attrib",
	"uop : UAOP",
	"copy : copstmnt alias LPAREN coparam RPAREN keywd SCONST",
	"copstmnt : COPY",
	"coparam : cospecs",
	"coparam : /* empty */",
	"cospecs : alias is coent",
	"cospecs : cospecs COMMA alias is coent",
	"coent : alias",
	"coent : SCONST",
	"alias : NAME",
	"specs : alias is alias",
	"specs : specs COMMA alias is alias",
	"keywd : INTO",
	"keywd : FROM",
	"create : crestmnt alias LPAREN specs RPAREN",
	"crestmnt : CREATE",
	"destroy : destmnt keys",
	"destroy : destqm destlist",
	"destmnt : DESTROY",
	"destqm : destmnt INTEGRITY NAME",
	"destqm : destmnt PERMIT NAME",
	"destlist : I2CONST",
	"destlist : destlist COMMA I2CONST",
	"destlist : ALL",
	"help : helstmnt hlist",
	"help : helstmnt",
	"help : helqmstmnt hqmlist",
	"helstmnt : HELP",
	"helqmstmnt : HELP VIEW",
	"helqmstmnt : HELP PERMIT",
	"helqmstmnt : HELP INTEGRITY",
	"hlist : hparam",
	"hlist : hlist hparam",
	"hlist : hlist COMMA hparam",
	"hparam : NAME",
	"hparam : ALL",
	"hparam : SCONST",
	"hqmlist : NAME",
	"hqmlist : hqmlist COMMA NAME",
	"index : instmnt LPAREN keys RPAREN",
	"instmnt : indexq ON NAME IS NAME",
	"indexq : INDEX",
	"modify : modstmnt alias TO modstorage modkeys modqual",
	"modstmnt : MODIFY",
	"modstorage : NAME",
	"modkeys : modstkey modrptkey",
	"modkeys : /* empty */",
	"modstkey : ON",
	"modrptkey : modbasekey",
	"modrptkey : modrptkey COMMA modbasekey",
	"modbasekey : NAME",
	"modbasekey : NAME COLON NAME",
	"modqual : modcond modfill",
	"modqual : /* empty */",
	"modcond : WHERE",
	"modfill : modfillnum",
	"modfill : modfill COMMA modfillnum",
	"modfillnum : NAME IS I2CONST",
	"modfillnum : NAME IS I4CONST",
	"keys : alias",
	"keys : keys COMMA alias",
	"print : prinstmnt keys",
	"prinstmnt : PRINT",
	"echo : prinstmnt SCONST",
	"save : savstmnt alias UNTIL date",
	"savstmnt : SAVE",
	"date : month day_year day_year",
	"month : alias",
	"month : day_year",
	"day_year : I2CONST",
};
#endif /* YYDEBUG */
/*	@(#) yaccpar.src 1.1 87/05/20 
 *
 *	Copyright (C) The Santa Cruz Operation, 1985.
 *	This Module contains Proprietary Information of
 *	The Santa Cruz Operation, Microsoft Corporation
 *	and AT&T, and should be treated as Confidential.
 */

/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#define YYERROR		goto yyerrlab
#define YYACCEPT	return(0)
#define YYABORT		return(1)
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
	{\
		yyerror( "syntax error - cannot backup" );\
		goto yyerrlab;\
	}\
	yychar = newtoken;\
	yystate = *yyps;\
	yylval = newvalue;\
	goto yynewstate;\
}
#define YYRECOVERING()	(!!yyerrflag)
#ifndef YYDEBUG
#	define YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int yydebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-1000)

/*
** global variables used by the parser
*/
YYSTYPE yyv[ YYMAXDEPTH ];	/* value stack */
int yys[ YYMAXDEPTH ];		/* state stack */

YYSTYPE *yypv;			/* top of value stack */
int *yyps;			/* top of state stack */

int yystate;			/* current state */
int yytmp;			/* extra var (lasts between blocks) */

int yynerrs;			/* number of errors */
int yyerrflag;			/* error recovery flag */
int yychar;			/* current input token number */



/*
** yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
int
yyparse()
{
	register YYSTYPE *yypvt;	/* top of value stack for $vars */

	/*
	** Initialize externals - yyparse may be called more than once
	*/
	yypv = &yyv[-1];
	yyps = &yys[-1];
	yystate = 0;
	yytmp = 0;
	yynerrs = 0;
	yyerrflag = 0;
	yychar = -1;

	goto yystack;
	{
		register YYSTYPE *yy_pv;	/* top of value stack */
		register int *yy_ps;		/* top of state stack */
		register int yy_state;		/* current state */
		register int  yy_n;		/* internal state number info */

		/*
		** get globals into registers.
		** branch to here only if YYBACKUP was called.
		*/
	yynewstate:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;
		goto yy_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	yystack:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	yy_stack:
		/*
		** put a state and value onto the stacks
		*/
#if YYDEBUG
		/*
		** if debugging, look up token value in list of value vs.
		** name pairs.  0 and negative (-1) are special values.
		** Note: linear search is used since time is not a real
		** consideration while debugging.
		*/
		if ( yydebug )
		{
			register int yy_i;

			printf( "State %d, token ", yy_state );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ++yy_ps >= &yys[ YYMAXDEPTH ] )	/* room on stack? */
		{
			yyerror( "yacc stack overflow" );
			YYABORT;
		}
		*yy_ps = yy_state;
		*++yy_pv = yyval;

		/*
		** we have a new state - find out what to do
		*/
	yy_newstate:
		if ( ( yy_n = yypact[ yy_state ] ) <= YYFLAG )
			goto yydefault;		/* simple state */
#if YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		yytmp = yychar < 0;
#endif
		if ( ( yychar < 0 ) && ( ( yychar = yylex() ) < 0 ) )
			yychar = 0;		/* reached EOF */
#if YYDEBUG
		if ( yydebug && yytmp )
		{
			register int yy_i;

			printf( "Received token " );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ( ( yy_n += yychar ) < 0 ) || ( yy_n >= YYLAST ) )
			goto yydefault;
		if ( yychk[ yy_n = yyact[ yy_n ] ] == yychar )	/*valid shift*/
		{
			yychar = -1;
			yyval = yylval;
			yy_state = yy_n;
			if ( yyerrflag > 0 )
				yyerrflag--;
			goto yy_stack;
		}

	yydefault:
		if ( ( yy_n = yydef[ yy_state ] ) == -2 )
		{
#if YYDEBUG
			yytmp = yychar < 0;
#endif
			if ( ( yychar < 0 ) && ( ( yychar = yylex() ) < 0 ) )
				yychar = 0;		/* reached EOF */
#if YYDEBUG
			if ( yydebug && yytmp )
			{
				register int yy_i;

				printf( "Received token " );
				if ( yychar == 0 )
					printf( "end-of-file\n" );
				else if ( yychar < 0 )
					printf( "-none-\n" );
				else
				{
					for ( yy_i = 0;
						yytoks[yy_i].t_val >= 0;
						yy_i++ )
					{
						if ( yytoks[yy_i].t_val
							== yychar )
						{
							break;
						}
					}
					printf( "%s\n", yytoks[yy_i].t_name );
				}
			}
#endif /* YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register int *yyxi = yyexca;

				while ( ( *yyxi != -1 ) ||
					( yyxi[1] != yy_state ) )
				{
					yyxi += 2;
				}
				while ( ( *(yyxi += 2) >= 0 ) &&
					( *yyxi != yychar ) )
					;
				if ( ( yy_n = yyxi[1] ) < 0 )
					YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( yy_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( yyerrflag )
			{
			case 0:		/* new error */
				yyerror( "syntax error" );
				goto skip_init;
			yyerrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				yy_pv = yypv;
				yy_ps = yyps;
				yy_state = yystate;
				yynerrs++;
			skip_init:
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				yyerrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( yy_ps >= yys )
				{
					yy_n = yypact[ *yy_ps ] + YYERRCODE;
					if ( yy_n >= 0 && yy_n < YYLAST &&
						yychk[yyact[yy_n]] == YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						yy_state = yyact[ yy_n ];
						goto yy_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( yydebug )
						printf( _POP_, *yy_ps,
							yy_ps[-1] );
#	undef _POP_
#endif
					yy_ps--;
					yy_pv--;
				}
				/*
				** there is no state on stack with "error" as
				** a valid shift.  give up.
				*/
				YYABORT;
			case 3:		/* no shift yet; eat a token */
#if YYDEBUG
				/*
				** if debugging, look up token in list of
				** pairs.  0 and negative shouldn't occur,
				** but since timing doesn't matter when
				** debugging, it doesn't hurt to leave the
				** tests here.
				*/
				if ( yydebug )
				{
					register int yy_i;

					printf( "Error recovery discards " );
					if ( yychar == 0 )
						printf( "token end-of-file\n" );
					else if ( yychar < 0 )
						printf( "token -none-\n" );
					else
					{
						for ( yy_i = 0;
							yytoks[yy_i].t_val >= 0;
							yy_i++ )
						{
							if ( yytoks[yy_i].t_val
								== yychar )
							{
								break;
							}
						}
						printf( "token %s\n",
							yytoks[yy_i].t_name );
					}
				}
#endif /* YYDEBUG */
				if ( yychar == 0 )	/* reached EOF. quit */
					YYABORT;
				yychar = -1;
				goto yy_newstate;
			}
		}/* end if ( yy_n == 0 ) */
		/*
		** reduction by production yy_n
		** put stack tops, etc. so things right after switch
		*/
#if YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( yydebug )
			printf( "Reduce by (%d) \"%s\"\n",
				yy_n, yyreds[ yy_n ] );
#endif
		yytmp = yy_n;			/* value to switch over */
		yypvt = yy_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using yy_state here as temporary
		** register variable, but why not, if it works...
		** If yyr2[ yy_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto yy_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int yy_len = yyr2[ yy_n ];

			if ( !( yy_len & 01 ) )
			{
				yy_len >>= 1;
				yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
				yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
					*( yy_ps -= yy_len ) + 1;
				if ( yy_state >= YYLAST ||
					yychk[ yy_state =
					yyact[ yy_state ] ] != -yy_n )
				{
					yy_state = yyact[ yypgo[ yy_n ] ];
				}
				goto yy_stack;
			}
			yy_len >>= 1;
			yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
			yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
				*( yy_ps -= yy_len ) + 1;
			if ( yy_state >= YYLAST ||
				yychk[ yy_state = yyact[ yy_state ] ] != -yy_n )
			{
				yy_state = yyact[ yypgo[ yy_n ] ];
			}
		}
					/* save until reenter driver code */
		yystate = yy_state;
		yyps = yy_ps;
		yypv = yy_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( yytmp )
	{
		
case 1:
# line 158 "grammar.z"

		{
#			ifdef	xPTR1
			tTfp(38, 0, "*** [program stmnt] parsed.\n");
#			endif

			if (endquelst(Opflag) < 0)
				return (-1);
		} break;
case 2:
# line 167 "grammar.z"

		{
#			ifdef	xPTR1
			tTfp(38, 1, "*** [stmnt] parsed.\n");
#			endif

			if (endquelst(Opflag) < 0)
				return (-1);
		} break;
case 3:
# line 177 "grammar.z"
{
#			ifdef	xPTR1
			tTfp(38, 2, "*** [(NULL)] parsed.\n");
#			endif
		} break;
case 23:
# line 203 "grammar.z"
{
#			ifdef	xPTR1
			tTfp(38, 0, "*** [error] parsed.\n");
#			endif
		} break;
case 24:
# line 210 "grammar.z"

		{
			if ((i = openr(&Reldesc, -1, yypvt[-0].string_type)) < 0)
				syserr("relname: error in openr '%d'", i);
			if (i > 0)
			{
				/* invalid relation name */
				par_error(RNGEXIST, WARN, yypvt[-0].string_type, 0);
				YYERROR;
			}
			else
				rngent(R_EXTERNAL, yypvt[-2].string_type, &Reldesc);
		} break;
case 25:
# line 224 "grammar.z"

		{
			Opflag = mdRANGE;
		} break;
case 26:
# line 229 "grammar.z"

		{
			/* make root node */
			Lastree = tree(yypvt[-1].tree_type, yypvt[-0].tree_type, ROOT, sizeof(struct rootnode), 1);
		} break;
case 27:
# line 235 "grammar.z"

		{
			Opflag = mdAPP;
		} break;
case 33:
# line 246 "grammar.z"

		{
			/* make root node for delete, with a TIDNODE at leftmost */
			Lastree = tree(tree(NULL, Tidnode, RESDOM, sizeof(struct resdomnode), NULL), yypvt[-0].tree_type, ROOT, sizeof(struct rootnode), 1);
		} break;
case 34:
# line 252 "grammar.z"

		{
			Opflag = mdDEL;
		} break;
case 39:
# line 262 "grammar.z"

		{
			/* make root node for replace */
			Lastree = tree(yypvt[-1].tree_type, yypvt[-0].tree_type, ROOT, sizeof(struct rootnode), 1);
		} break;
case 40:
# line 268 "grammar.z"

		{
			Opflag = mdREPL;
		} break;
case 45:
# line 278 "grammar.z"

		{
			/* make root node for retrieve */
			Lastree = tree(yypvt[-1].tree_type, yypvt[-0].tree_type, ROOT, sizeof(struct rootnode), 1);
		} break;
case 46:
# line 284 "grammar.z"

		{
			Opflag = mdRETR;
		} break;
case 47:
# line 289 "grammar.z"

		{
			/* set up pipe block and save relname for create */
#			ifdef	xPTR2
			tTfp(38, 4, "retclause: Rsdmno %d", Rsdmno);
#			endif
			Rsdmno = 0;
			setp(PV_STR, "0");	/* relstat = nil */
			setp(PV_STR, trim_relname(Parrng[Resrng].vardesc.reldum.relid));
		} break;
case 48:
# line 299 "grammar.z"

		{
			/* no result relation, output to terminal */
			Rsdmno = 0;
			Resrng = -1;
		} break;
case 49:
# line 305 "grammar.z"

		{
			Opflag = mdRET_UNI;
			Rsdmno = 0;
			Resrng = -1;
		} break;
case 53:
# line 316 "grammar.z"

		{
			Lastree = tree(yypvt[-1].tree_type, yypvt[-0].tree_type, ROOT, sizeof(struct rootnode), 1);
		} break;
case 54:
# line 321 "grammar.z"

		{
			Rsdmno = 0;
			setp(PV_STR, "0040");	/* relstat = S_VIEW */
			setp(PV_STR, trim_relname(Parrng[Resrng].vardesc.reldum.relid));
		} break;
case 55:
# line 328 "grammar.z"

		{
			Opflag = mdVIEW;
			if (!Qrymod)
			{
				/* no qrymod in database */
				par_error(NOQRYMOD, WARN, 0);
			}
		} break;
case 56:
# line 338 "grammar.z"

		{
			Lastree = tree(yypvt[-4].tree_type, yypvt[-0].tree_type, ROOT, sizeof(struct rootnode), 1);
		} break;
case 57:
# line 343 "grammar.z"

		{
			Opflag = mdPROT;
			if (!Qrymod)
			{
				/* no qrymod in database */
				par_error(NOQRYMOD, WARN, 0);
			}
		} break;
case 60:
# line 356 "grammar.z"

		{
			permcom(-1);	/* means 'all' commands */
		} break;
case 61:
# line 360 "grammar.z"

		{
			permcom(mdRETR);
		} break;
case 62:
# line 364 "grammar.z"

		{
			permcom(mdDEL);
		} break;
case 63:
# line 368 "grammar.z"

		{
			permcom(mdAPP);
		} break;
case 64:
# line 372 "grammar.z"

		{
			permcom(mdREPL);
		} break;
case 65:
# line 377 "grammar.z"

		{
			/* put command vector into list now since this always happens */
			setp(PV_INT, Permcomd);
			Permcomd = 0;		/* reset command map */
			setp(PV_STR, trim_relname(Parrng[Resrng].vardesc.reldum.relid));
			bmove(Parrng[Resrng].vardesc.reldum.relowner, permbuf, 2);
			permbuf[2] = 0;
			setp(PV_STR, permbuf);
		} break;
case 69:
# line 392 "grammar.z"

		{
			yyval.tree_type = yypvt[-1].tree_type;
		} break;
case 70:
# line 396 "grammar.z"

		{
			yyval.tree_type = NULL;
		} break;
case 72:
# line 402 "grammar.z"

		{
			/*
			** attach bulk of permit tl to leftmost node of new elem
			*/
			if (!Err_current)
				yyval.tree_type = tlprepend(yypvt[-2].tree_type, yypvt[-0].tree_type);
		} break;
case 73:
# line 411 "grammar.z"

		{
			/* Resrng is set by the "relation" production */
			if (!Err_current)
			{
				Trname = yypvt[-0].string_type;
				aptr = attlookup(Resrng, Trname);
				yyval.tree_type = tree(NULL, NULL, VAR, sizeof(struct varnode), Resrng, aptr);
				yyval.tree_type = addresdom(NULL, yyval.tree_type);
			}
		} break;
case 74:
# line 423 "grammar.z"

		{
			setp(PV_STR, yypvt[-0].string_type);
		} break;
case 75:
# line 427 "grammar.z"

		{
			setp(PV_STR, "all");
		} break;
case 76:
# line 432 "grammar.z"

		{
			setp(PV_STR, yypvt[-0].string_type);
		} break;
case 77:
# line 436 "grammar.z"

		{
			setp(PV_STR, "all");
		} break;
case 78:
# line 440 "grammar.z"

		{
			setp(PV_STR, "all");		/* default is all */
		} break;
case 83:
# line 450 "grammar.z"

		{
			setp(PV_INT, 0);
			setp(PV_INT, 1440);
		} break;
case 84:
# line 456 "grammar.z"

		{
			setp(PV_STR, "sun");
			setp(PV_STR, "sat");
		} break;
case 85:
# line 462 "grammar.z"

		{
			setp(PV_INT, timeofday(yypvt[-6].I2_type, yypvt[-4].I2_type));
			setp(PV_INT, timeofday(yypvt[-2].I2_type, yypvt[-0].I2_type));
		} break;
case 86:
# line 468 "grammar.z"

		{
			setp(PV_STR, yypvt[-2].string_type);
			setp(PV_STR, yypvt[-0].string_type);
		} break;
case 87:
# line 474 "grammar.z"

		{
			Lastree = tree(NULL, norml(yypvt[-0].tree_type), ROOT, sizeof(struct rootnode), 1);
			Qlflag--;	/* turn off here */
		} break;
case 88:
# line 480 "grammar.z"

		{
			Opflag = mdINTEG;
			Qlflag++;	/* OK to turn on here because integrity doesn't have a targ list */
			if (!Qrymod)
			{
				/* no qrymod in database */
				par_error(NOQRYMOD, WARN, 0);
			}
		} break;
case 97:
# line 502 "grammar.z"

		{
			Opflag = mdCREATE;
			setp(PV_STR,"0100000"); /* S_SERIAL */
			setp(PV_STR,yypvt[-1].string_type);
		} break;
case 98:
# line 509 "grammar.z"

		{
			if (*((i_2 *)yypvt[-0].I2_type) < 0)
				par_error(SERNONEG,WARN,0);
			setp(PV_INT,INT);
			i = iocv(*((i_2 *)yypvt[-0].I2_type));
			setp(PV_STR, i);
		} break;
case 99:
# line 517 "grammar.z"

		{
			if (*((i_4 *)yypvt[-0].I4_type) < 0)
				par_error(SERNONEG,WARN,0);
			setp(PV_INT,INT);
			i = locv(*((i_4 *)yypvt[-0].I4_type));
			setp(PV_STR, i);
		} break;
case 100:
# line 526 "grammar.z"

		{
#			ifdef	xPTR2
			tTfp(38, 3, "res rel name/var: '%s'\n", yypvt[-0].string_type);
#			endif
			switch (Opflag)
			{
			  case mdRETR:
			  case mdVIEW:
				/* result better not be a rel name */
				if ((i = openr(&Reldesc, -1, yypvt[-0].string_type)) < 0)
					syserr("relation: err openr '%d'", i);
				if (i == 0)
				{
					/* reln exists */
					if (bequal(Reldesc.reldum.relowner, Usercode, 2))
					{
						/* same owner, can't duplicate name */
						par_error(RESEXIST, WARN, yypvt[-0].string_type, 0);
						YYERROR;
					}
					else if (!Err_current)
					{
						/* owned by dba -- purge range table */
						rngdel(yypvt[-0].string_type);
					}
				}
				if (!Err_current)
				{
					bmove(Usercode, Reldesc.reldum.relowner, 2);
					pmove(yypvt[-0].string_type, Reldesc.reldum.relid, MAXNAME, ' ');
					Resrng = rngent(R_INTERNAL, "", &Reldesc);
				}
				break;

			  case mdAPP:
				/* result is a rel name */
				if (!Err_current)
				{
					Resrng = rnglook(yypvt[-0].string_type, LOOKREL);
					if (Resrng < 0)
					{
						if ((i = openr(&Reldesc, -1, yypvt[-0].string_type)) < 0)
							syserr("relation: err openr '%d'", i);
						if (i)
						{
							/* invalid relation name */
							par_error(RESAPPEX, WARN, yypvt[-0].string_type, 0);
							YYERROR;
						}
						Resrng = rngent(R_INTERNAL, "", &Reldesc);
					}
					else
						ctlmod_decl(Resrng);
					checkupd(Resrng);
				}
				break;

			  case mdPROT:
			  case mdINTEG:
#			  ifdef	DISTRIB
			  case mdDISTRIB:
#			  endif
				/* the result is a tuple variable */
				Resrng = rnglook(yypvt[-0].string_type, LOOKVAR);
				if (Resrng < 0)
				{
					/* variable not declared */
					par_error(NOVBLE, WARN, yypvt[-0].string_type, 0);
					YYERROR;
				}
				else
					ctlmod_decl(Resrng);
				break;

			  case mdREPL:
			  case mdDEL:
				/* the result is a tuple variable */
				Resrng = rnglook(yypvt[-0].string_type, LOOKVAR);
				if (Resrng < 0)
					/* variable not declared */
				{
					par_error(NOVBLE, WARN, yypvt[-0].string_type, 0);
					YYERROR;
				}
				else
					ctlmod_decl(Resrng);

				checkupd(Resrng);
				Tidnode = tree(NULL, NULL, VAR, sizeof(struct varnode), Resrng, &Faketid);
				break;
			}
		} break;
case 101:
# line 620 "grammar.z"

		{
			if (Patflag)
			{
				/* no patt match in targ list */
				par_error(NOPATMAT, WARN, 0);
			}
			yyval.tree_type = yypvt[-1].tree_type;

			/*
			** replace must have tid node as left branch
			**	(so does delete but it doesn't have a targ list)
			*/
			if (Opflag == mdREPL && !Err_current)
			{
				yyval.tree_type = tlprepend(tree(NULL, Tidnode, RESDOM, sizeof(struct resdomnode), 0), yyval.tree_type);
			}
		} break;
case 103:
# line 640 "grammar.z"

		{
			/*
			** attach bulk of targ list to leftmost node
			** of new element
			*/
			if (!Err_current)
				yyval.tree_type = tlprepend(yypvt[-2].tree_type, yypvt[-0].tree_type);
		} break;
case 104:
# line 650 "grammar.z"

		{
			Trname = yypvt[-2].string_type;
			/* make a new resdom entry for targ list */
			if (!Err_current)
				yyval.tree_type = addresdom(NULL, yypvt[-0].tree_type);
		} break;
case 105:
# line 657 "grammar.z"

		{
			/* makes a new resdom entry for targ list */
			if (!Err_current)
				yyval.tree_type = addresdom(NULL, yypvt[-0].tree_type);
		} break;
case 106:
# line 663 "grammar.z"

		{
			if (Opflag == mdREPL)
			{
				/* ALL not defined for REPLACE */
				par_error(REPALL, WARN,
				    trim_relname(Qt.qt_rangev[yypvt[-2].rng_type].rngvdesc->relvname), 0);
				YYERROR;
			}
			/* makes set of new resdom entries for targ list */
			else if (!Err_current)
				yyval.tree_type = xdot(yypvt[-2].rng_type);
		} break;
case 109:
# line 680 "grammar.z"

		{
			yyval.tree_type = norml(yypvt[-0].tree_type);
			Qlflag--;
		} break;
case 110:
# line 685 "grammar.z"

		{
			/* null qualification */
			yyval.tree_type = norml(NULL);
		} break;
case 111:
# line 691 "grammar.z"

		{
			Qlflag++;
		} break;
case 112:
# line 696 "grammar.z"

		{
			yyval.tree_type = yypvt[-1].tree_type;
		} break;
case 113:
# line 700 "grammar.z"

		{
			yyval.tree_type = tree(NULL, yypvt[-0].tree_type, UOP, 2, yypvt[-1].type_type);
		} break;
case 114:
# line 704 "grammar.z"

		{
			yyval.tree_type = tree(yypvt[-2].tree_type, yypvt[-0].tree_type, yypvt[-1].type_type, sizeof (struct rootnode) -2, 0);
		} break;
case 116:
# line 710 "grammar.z"

		{
			yyval.tree_type = tree(yypvt[-2].tree_type, yypvt[-0].tree_type, BOP, 2, yypvt[-1].type_type);
		} break;
case 122:
# line 721 "grammar.z"

		{
			yyval.tree_type = tree(yypvt[-2].tree_type, yypvt[-0].tree_type, BOP, 2, yypvt[-1].type_type);
		} break;
case 123:
# line 725 "grammar.z"

		{
			yyval.tree_type = tree(yypvt[-2].tree_type, yypvt[-0].tree_type, BOP, 2, yypvt[-1].type_type);
		} break;
case 124:
# line 729 "grammar.z"

		{
			yyval.tree_type = tree(yypvt[-2].tree_type, yypvt[-0].tree_type, BOP, 2, yypvt[-1].type_type);
		} break;
case 125:
# line 733 "grammar.z"

		{
			yyval.tree_type = yypvt[-1].tree_type;
		} break;
case 126:
# line 737 "grammar.z"

		{
			yyval.tree_type = tree(NULL, yypvt[-0].tree_type, UOP, 2, yypvt[-1].type_type);
		} break;
case 127:
# line 741 "grammar.z"

		{
			yyval.tree_type = tree(yypvt[-1].tree_type, NULL, UOP, 2, yypvt[-3].type_type);
		} break;
case 128:
# line 745 "grammar.z"

		{
			yyval.tree_type = tree(yypvt[-3].tree_type, yypvt[-1].tree_type, BOP, 2, yypvt[-5].type_type);
		} break;
case 129:
# line 750 "grammar.z"

		{
#			ifdef	xPTR2
			tTfp(39, 0, "agg func\n");
#			endif
			windup(yypvt[-2].tree_type);
			yyval.tree_type = tree(tree(yypvt[-2].tree_type, tree(NULL, yypvt[-4].tree_type, AOP, 6, yypvt[-6].type_type), BYHEAD, sizeof(struct resdomnode), 0), yypvt[-1].tree_type, AGHEAD, sizeof(struct rootnode), 0);
			tlprepend(tree(NULL, NULL, TREE, 0), yyval.tree_type);
		} break;
case 130:
# line 759 "grammar.z"

		{
			yyval.tree_type = tree(tree(NULL, yypvt[-2].tree_type, AOP, 6, yypvt[-4].type_type), yypvt[-1].tree_type,  AGHEAD, sizeof(struct rootnode), 0);
		} break;
case 132:
# line 765 "grammar.z"

		{
			yyval.tree_type = tlprepend(yypvt[-2].tree_type, yypvt[-0].tree_type);
		} break;
case 133:
# line 770 "grammar.z"

		{
			yyval.tree_type = tree(NULL, yypvt[-0].tree_type, RESDOM, sizeof(struct resdomnode), Rsdmno);
		} break;
case 134:
# line 775 "grammar.z"

		{
#			ifdef	xPTR2
			tTfp(39, 1, "attrib %12s.%12s found\n",
			Qt.qt_rangev[yypvt[-2].rng_type].rngvdesc->relvname, yypvt[-0].string_type);
#			endif

			/* remember attribute name */
			Trname = yypvt[-0].string_type;

			/* look up attribute */
			aptr = attlookup(yypvt[-2].rng_type, Trname);
			yyval.tree_type = tree(NULL, NULL, VAR, sizeof(struct varnode), yypvt[-2].rng_type, aptr);
		} break;
case 135:
# line 790 "grammar.z"

		{
			yyval.rng_type = rnglook(yypvt[-0].string_type, LOOKVAR);
			if (yyval.rng_type < 0)
			{
				/* variable not declared */
				par_error(NOVBLE, WARN, yypvt[-0].string_type, 0);
				YYERROR;
			}
			else
				ctlmod_decl(yyval.rng_type);
		} break;
case 136:
# line 803 "grammar.z"

		{
			yyval.tree_type = tree(NULL, NULL, INT, 2, yypvt[-0].I2_type);
		} break;
case 137:
# line 807 "grammar.z"

		{
			yyval.tree_type = tree(NULL, NULL, INT, 4, yypvt[-0].I4_type);
		} break;
case 138:
# line 811 "grammar.z"

		{
			yyval.tree_type = tree(NULL, NULL, FLOAT, 4, yypvt[-0].F4_type);
		} break;
case 139:
# line 815 "grammar.z"

		{
			yyval.tree_type = tree(NULL, NULL, FLOAT, 8, yypvt[-0].F8_type);
		} break;
case 140:
# line 819 "grammar.z"

		{
			yyval.tree_type = tree(NULL, NULL, DATE, 4, yypvt[-0].I4_type);
		} break;
case 141:
# line 823 "grammar.z"

		{
			yyval.tree_type = tree(NULL, NULL, TIME, 4, yypvt[-0].I4_type);
		} break;
case 142:
# line 827 "grammar.z"

		{
			if (patmat(yypvt[-0].string_type) && !Qlflag)
				Patflag = 1;
			yyval.tree_type = tree(NULL, NULL, CHAR, length(yypvt[-0].string_type), yypvt[-0].string_type);
		} break;
case 143:
# line 833 "grammar.z"

		{
			yyval.tree_type = tree(NULL, NULL, COP, 2, yypvt[-0].string_type);
		} break;
case 145:
# line 839 "grammar.z"

		{
			if (yypvt[-0].type_type == opADD)
				yyval.type_type = opPLUS;
			else
				if (yypvt[-0].type_type == opSUB)
					yyval.type_type = opMINUS;
		} break;
case 146:
# line 848 "grammar.z"

		{
#			ifdef	xPTR2
			tTfp(39, 3, "copy %12s,%12s\n", yypvt[-5].string_type, yypvt[-0].string_type);
#			endif

			setp(PV_STR, yypvt[-0].string_type);
		} break;
case 147:
# line 857 "grammar.z"

		{
			Opflag = mdCOPY;
		} break;
case 153:
# line 869 "grammar.z"

		{
			setp(PV_STR, yypvt[-0].string_type);
		} break;
case 154:
# line 874 "grammar.z"

		{
			if (!Err_current)
			{
				setp(PV_STR, yypvt[-0].string_type);
				if (Opflag == mdDESTROY || Opflag == mdCREATE
#					ifdef	DISTRIB
					|| Opflag == mdDCREATE
#					endif
								)
					rngdel(yypvt[-0].string_type);
			}
		} break;
case 157:
# line 891 "grammar.z"

		{
			setp(PV_STR, "\0");
			setp(PV_STR, "i");
		} break;
case 158:
# line 896 "grammar.z"

		{
			setp(PV_STR, "\0");
			setp(PV_STR, "f");
		} break;
case 160:
# line 904 "grammar.z"

		{
			Opflag = mdCREATE;

			/* set up parameters for regular create */
			setp(PV_STR, "0");		/* relstat = nil */
		} break;
case 163:
# line 915 "grammar.z"

		{
			Opflag = mdDESTROY;
		} break;
case 164:
# line 920 "grammar.z"

		{
			Opflag = mdREMQM;
			if (!Qrymod)
				/* no qrymod in database */
				par_error(NOQRYMOD, WARN, 0);
			setp(PV_STR, "6");
			setp(PV_STR, yypvt[-0].string_type);
		} break;
case 165:
# line 929 "grammar.z"

		{
			Opflag = mdREMQM;
			if (!Qrymod)
				/* no qrymod in database */
				par_error(NOQRYMOD, WARN, 0);
			setp(PV_STR, "5");
			setp(PV_STR, yypvt[-0].string_type);
		} break;
case 166:
# line 939 "grammar.z"

		{
			i = iocv(*((i_2 *)yypvt[-0].I2_type));
			setp(PV_STR, i);
		} break;
case 167:
# line 944 "grammar.z"

		{
			i = iocv(*((i_2 *)yypvt[-0].I2_type));
			setp(PV_STR, i);
		} break;
case 170:
# line 952 "grammar.z"

		{
			setp(PV_STR, "2");	/* all relns */
		} break;
case 172:
# line 958 "grammar.z"

		{
			Opflag = mdHELP;
		} break;
case 173:
# line 963 "grammar.z"

		{
			Opflag = mdDISPLAY;
			if (!Qrymod)
				/* no qrymod in database */
				par_error(NOQRYMOD, WARN, 0);
			smove("4", hqmbuf);
		} break;
case 174:
# line 971 "grammar.z"

		{
			Opflag = mdDISPLAY;
			if (!Qrymod)
				/* no qrymod in database */
				par_error(NOQRYMOD, WARN, 0);
			smove("5", hqmbuf);
		} break;
case 175:
# line 979 "grammar.z"

		{
			Opflag = mdDISPLAY;
			if (!Qrymod)
				/* no qrymod in database */
				par_error(NOQRYMOD, WARN, 0);
			smove("6", hqmbuf);
		} break;
case 179:
# line 992 "grammar.z"

		{
			/* relation */
			setp(PV_STR, "0");
			setp(PV_STR, yypvt[-0].string_type);
		} break;
case 180:
# line 998 "grammar.z"

		{
			setp(PV_STR, "3");
		} break;
case 181:
# line 1002 "grammar.z"

		{
			/* manual page */
			setp(PV_STR, "1");
			setp(PV_STR, yypvt[-0].string_type);
		} break;
case 182:
# line 1009 "grammar.z"

		{
			setp(PV_STR, hqmbuf);
			setp(PV_STR, yypvt[-0].string_type);
		} break;
case 183:
# line 1014 "grammar.z"

		{
			setp(PV_STR, hqmbuf);
			setp(PV_STR, yypvt[-0].string_type);
		} break;
case 184:
# line 1020 "grammar.z"

		{
			if (Rsdmno > MAXKEYS)
				/* too many attributes in key */
				par_error(INDEXTRA, WARN, 0);
		} break;
case 185:
# line 1027 "grammar.z"

		{
			/* init INDEX command */
			Rsdmno = 0;
			setp(PV_STR, yypvt[-2].string_type);
			setp(PV_STR, yypvt[-0].string_type);
			Indexname = yypvt[-0].string_type;
		} break;
case 186:
# line 1036 "grammar.z"

		{
			Opflag = mdINDEX;
		} break;
case 188:
# line 1043 "grammar.z"

		{
			Opflag = mdMODIFY;
			Rsdmno = 0;
		} break;
case 189:
# line 1049 "grammar.z"

		{
			setp(PV_STR, yypvt[-0].string_type);
		} break;
case 192:
# line 1056 "grammar.z"

		{
			setp(PV_STR, "name");
		} break;
case 195:
# line 1064 "grammar.z"

		{
			setp(PV_STR, yypvt[-0].string_type);
		} break;
case 196:
# line 1068 "grammar.z"

		{
			concat(yypvt[-2].string_type, ztack(":", yypvt[-0].string_type), modbuf);
			setp(PV_STR, modbuf);
		} break;
case 199:
# line 1077 "grammar.z"

		{
			setp(PV_STR, "\0");
		} break;
case 202:
# line 1085 "grammar.z"

		{
			setp(PV_STR, yypvt[-2].string_type);
			i = iocv(*((i_2 *)yypvt[-0].I2_type));
			setp(PV_STR, i);
		} break;
case 203:
# line 1091 "grammar.z"

		{
			setp(PV_STR, yypvt[-2].string_type);
			i = locv(*((i_4 *)yypvt[-0].I4_type));
			setp(PV_STR, i);
		} break;
case 204:
# line 1098 "grammar.z"

		{
			Rsdmno++;
		} break;
case 205:
# line 1102 "grammar.z"

		{
			Rsdmno++;
		} break;
case 207:
# line 1109 "grammar.z"

		{
			Opflag = mdPRINT;
		} break;
case 208:
# line 1114 "grammar.z"

		{
			printf("%s\n",yypvt[-0].string_type);
		} break;
case 210:
# line 1121 "grammar.z"

		{
			Opflag = mdSAVE;
		} break;
case 214:
# line 1131 "grammar.z"

		{
			i = iocv(*((i_2 *)yypvt[-0].I2_type));

#			ifdef	xPTR3
			tTfp(39, 4, "day_year: %s\n", i);
#			endif

			setp(PV_STR, i);
		} break;
	}
	goto yystack;		/* reset registers in driver code */
}
