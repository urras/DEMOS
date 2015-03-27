
# line 34 "grammar.y"
	/* STANDARD SCANNER & PARSER GLOBALS */

# include	"constants.h"
# include	<stdio.h>
# include	"globals.h"
# include	<sccs.h>

SCCSID(@(#)grammar.y	7.1	2/5/81)

static int Inqual; /* 1 - мы в условии !! */

# line 45 "grammar.y"
typedef union 
{
	struct disp_node	*u_dn;
} YYSTYPE;
# define APPEND 257
# define COPY 258
# define CREATE 259
# define DEFINE 260
# define DELETE 261
# define DESTROY 262
# define HELP 263
# define INDEX 264
# define MODIFY 265
# define PRINT 266
# define INTEGRITY 267
# define RANGE 268
# define REPLACE 269
# define RETRIEVE 270
# define SAVE 271
# define UNIQUE 272
# define PERMIT 273
# define VIEW 274
# define RUBIN 275
# define EXIT 276
# define PARAM 277
# define TYPE 278
# define ALLOC 279
# define STRUCT 280
# define STRUCT_VAR 281
# define ALL 282
# define BY 283
# define FROM 284
# define IN 285
# define INTO 286
# define IS 287
# define OF 288
# define ON 289
# define ONTO 290
# define TO 291
# define WHERE 292
# define UNTIL 293
# define AT 294
# define NAME 295
# define SCONST 296
# define I2CONST 297
# define I4CONST 298
# define F8CONST 299
# define C_CODE 300
# define COMMA 301
# define LPAREN 302
# define RPAREN 303
# define PERIOD 304
# define QUOTE 305
# define BGNCMNT 306
# define ENDCMNT 307
# define LBRACE 308
# define RBRACE 309
# define LBRKT 310
# define RBRKT 311
# define NONREF 312
# define SEMICOL 313
# define POINTER 314
# define COLON 315
# define UOP 316
# define BOP 317
# define BDOP 318
# define EOP 319
# define LBOP 320
# define LUOP 321
# define FOP 322
# define FBOP 323
# define AOP 324
# define unaryop 325
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
typedef int yytabelem;

# line 113 "grammar.y"
	struct cvar	*cvarp;
# define YYERRCODE 256

# line 1264 "grammar.y"


# include	"tokens.y"
yytabelem yyexca[] ={
-1, 1,
	0, -1,
	257, 9,
	258, 9,
	259, 9,
	260, 9,
	261, 9,
	262, 9,
	263, 9,
	264, 9,
	265, 9,
	266, 9,
	268, 9,
	269, 9,
	270, 9,
	271, 9,
	275, 9,
	276, 9,
	277, 9,
	-2, 0,
-1, 63,
	302, 131,
	-2, 313,
-1, 187,
	272, 277,
	302, 277,
	-2, 274,
-1, 284,
	287, 181,
	302, 187,
	304, 187,
	316, 181,
	317, 181,
	318, 181,
	319, 181,
	-2, 198,
-1, 363,
	302, 187,
	304, 187,
	-2, 181,
	};
# define YYNPROD 316
# define YYLAST 732
yytabelem yyact[]={

   296,   447,   422,   236,   312,   468,   240,   384,   156,   220,
   282,   393,   348,   227,   303,   302,   126,   271,   335,   254,
   373,   278,   359,   328,   371,    95,   199,   448,   278,   371,
   370,   436,   278,   113,   201,   199,   371,   370,   359,   219,
   443,   371,   370,    92,   397,   371,   370,    97,   396,   221,
   371,   370,   112,   120,   369,   128,   140,   144,   148,   149,
    95,   128,   158,   180,   133,   170,   289,   172,   173,   369,
   278,     7,   283,   371,   370,   190,   119,   157,   177,    11,
    12,   131,    97,   371,   370,   368,   367,   132,   142,   119,
    95,   301,   298,   299,   300,   119,   136,   206,   371,   370,
   368,   367,   199,    95,   103,   410,   119,   115,   207,    95,
   143,   304,    97,   206,   399,   352,   288,   305,   306,   307,
    95,   105,   469,   222,   278,    97,   115,   205,   224,   119,
   136,    97,   255,   208,   103,   119,   128,   115,   338,   206,
   337,   218,    97,    95,   280,   313,   314,   216,   234,    95,
   198,   199,   226,   201,   242,   453,   199,   217,   278,   221,
   115,   247,   235,   131,   199,    97,   115,   251,   199,   132,
   142,    97,   245,   253,   221,   210,   197,   119,   223,   237,
   205,   243,   131,   167,   221,   205,   250,   119,   132,   209,
   190,    95,   256,   257,   258,   259,   119,   168,   119,   136,
   168,    95,   169,   313,   314,   169,   273,   119,   115,   277,
    95,   119,    95,    97,   264,   473,   243,   192,   115,   277,
   274,    95,   143,    97,   308,    95,   140,   115,   317,   115,
   318,   193,    97,   311,    97,   119,   119,   321,   115,   322,
   320,   285,   115,    97,   270,   228,   324,    97,   381,    95,
    95,   191,    16,   332,   284,   205,     6,   278,   315,    17,
   351,   162,   161,   343,   350,   163,   115,   115,   142,   315,
   196,    97,    97,   351,   269,   167,    91,   350,    16,    17,
    20,   352,   338,   266,   337,    16,   325,    20,   326,   327,
   336,   356,   151,   394,   333,   355,   361,   364,   339,   340,
     7,   213,   137,   360,   344,   204,   285,   285,    11,    12,
   178,   275,   181,   225,   276,   374,   375,   376,   377,   363,
   363,   244,   243,   106,   237,   125,   123,   363,   383,   239,
   124,   385,   165,    57,   199,   395,   108,   392,   379,   111,
   109,   110,    93,   387,   130,   349,   390,     4,   273,   141,
   402,   188,   362,    13,   164,   405,   395,   407,   403,   382,
   372,   159,   274,   398,   391,   400,   315,   406,    99,   408,
   232,   127,   404,   233,   230,   231,   463,   346,   442,   285,
   128,   353,   363,   418,   423,   297,   425,   357,   128,   171,
   427,   420,   363,   116,   295,   294,   419,   431,   139,   363,
   363,   285,   146,   430,   428,   395,   182,   363,   363,   363,
   363,   363,   315,   429,   363,   401,   433,   129,   293,   434,
   366,   292,   194,    14,   175,   101,   315,   138,   454,   445,
   174,   385,   411,   412,   388,   458,   455,    98,   432,   450,
   413,   414,   415,   416,   417,   104,   291,   423,   466,   465,
   467,   347,   114,   474,   354,   290,   147,   441,   272,   435,
   354,   475,   117,    94,   363,   246,   365,   409,   287,   363,
   248,   358,   363,    96,   363,   145,   363,   483,   482,   200,
   286,   480,   215,   488,   470,   489,   486,   267,   260,   378,
   203,   492,   490,   261,   491,   493,   495,   457,   494,   102,
   496,    19,   459,   478,    18,   460,   315,   461,   389,   464,
    10,   212,    89,   363,   470,   330,   279,   329,    15,   141,
   437,   487,   439,   315,   196,   444,   249,   315,   316,   315,
   281,   153,   323,   100,   456,   154,   179,   119,    71,   265,
   319,   263,   176,   155,   152,   345,   485,   462,    70,   202,
    69,    95,   301,   298,   299,   300,   136,   268,   206,    96,
    68,   189,   183,   184,   185,    82,   476,   477,   115,    74,
   202,   119,   304,    97,   186,   187,    67,   288,   305,   306,
   307,   262,   334,    66,   484,    95,   301,   298,   299,   300,
    65,   331,   206,   252,   438,   440,   211,    64,   214,   166,
   446,    63,   115,   449,   160,    62,   304,    97,   341,   342,
    61,    60,   305,   306,   307,    75,    76,    77,    82,    78,
    79,    80,    81,    83,    84,   472,    85,    86,    87,    88,
   452,   241,   481,    90,    73,    72,   471,   451,   426,   386,
   238,   150,   479,    59,   424,   421,    58,   380,   229,    94,
    56,   135,   134,    55,   310,   309,    54,   122,   121,    96,
    53,    52,    51,   107,    50,    49,    48,    47,    46,    45,
    44,    43,    42,    41,    40,    39,    38,    37,    36,    35,
    34,    33,    32,    31,    30,    29,    28,    27,    26,    25,
    24,     8,   211,     5,   214,    23,    22,    21,     3,     2,
     1,   195,   118,     9,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    96 };
yytabelem yypact[]={

 -1000,     0, -1000,   358, -1000, -1000, -1000, -1000, -1000, -1000,
  -270, -1000, -1000,   -20,     7, -1000, -1000, -1000,  -204,  -174,
 -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
 -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
 -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
    50,   -70,   -70,    41,   -85,  -186,   -70,  -104,   -70,   274,
   -70,   -70,   -24,   -89,   -70,    50,   -70,   -70,   -24,   -86,
  -230,  -245,   305, -1000,   -45, -1000, -1000, -1000, -1000, -1000,
 -1000,   -72, -1000, -1000, -1000,   -57, -1000, -1000, -1000,     3,
 -1000,  -137, -1000, -1000,  -276, -1000,  -235, -1000, -1000, -1000,
 -1000, -1000, -1000, -1000, -1000, -1000,  -163,   -70, -1000, -1000,
 -1000, -1000,  -163, -1000,  -276,  -106,  -276,  -192,  -157, -1000,
  -163,  -118,   -70, -1000, -1000, -1000,  -150,   -70, -1000, -1000,
 -1000, -1000, -1000, -1000,  -150,   -70, -1000, -1000, -1000, -1000,
 -1000, -1000, -1000, -1000,   -42,    85, -1000,   -70,   -42,  -112,
    33, -1000, -1000, -1000, -1000, -1000, -1000,  -150,   -42,  -163,
   -70, -1000, -1000, -1000,  -163,  -163,   -70, -1000, -1000, -1000,
  -120,  -170,  -170,  -170,  -170,  -170, -1000, -1000, -1000, -1000,
 -1000, -1000,    -6, -1000, -1000, -1000, -1000, -1000, -1000,   -45,
 -1000, -1000, -1000, -1000,  -170, -1000,  -235, -1000, -1000, -1000,
 -1000, -1000, -1000,   -26,  -118,   -70, -1000, -1000,   -46, -1000,
 -1000, -1000, -1000, -1000,  -157,  -151, -1000, -1000,   -70, -1000,
  -205, -1000, -1000,   -70,  -152,   -74,  -150,   -70, -1000,   -70,
 -1000, -1000, -1000, -1000,  -163,   -94,   -70, -1000,   -70,   274,
 -1000, -1000, -1000, -1000, -1000,   -70,  -118, -1000,  -118,  -118,
 -1000, -1000,   -70, -1000,  -118, -1000,  -146, -1000,  -118,  -118,
  -229,  -229,   -70, -1000,  -118, -1000, -1000, -1000,  -235, -1000,
 -1000,  -179,  -150,   -23, -1000,  -145,    -2,   -42, -1000, -1000,
 -1000,  -145,  -298,  -170, -1000,   256,   256, -1000, -1000,  -218,
 -1000, -1000, -1000,   290,  -163,  -163,  -189,  -163, -1000, -1000,
 -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
  -150, -1000, -1000, -1000, -1000, -1000, -1000,  -163,   -39,  -118,
 -1000,  -108,  -163, -1000, -1000, -1000, -1000, -1000,  -179,  -150,
   -70, -1000,   -94, -1000,   -74, -1000, -1000, -1000, -1000, -1000,
 -1000,  -261,  -265,  -170, -1000,  -199, -1000,   -70,   290,   -83,
 -1000, -1000, -1000,    -2,   -70,   -74,   -70, -1000,   256, -1000,
 -1000,  -282,  -233, -1000, -1000,   290,   290, -1000, -1000, -1000,
 -1000, -1000, -1000,   290,   290,   290,   290,   290,   -94,   -70,
   256, -1000, -1000,   -70, -1000,   -70,  -112,   -70, -1000, -1000,
   -10, -1000,   -94, -1000, -1000, -1000, -1000, -1000,  -118, -1000,
 -1000,  -280, -1000, -1000,   -74,   -42, -1000, -1000, -1000, -1000,
 -1000,  -287,  -292,  -271,  -271,  -266,  -275,  -243, -1000,  -145,
  -298,  -133, -1000,  -288,  -150,   -42,  -139,   -83,  -145, -1000,
   290, -1000, -1000, -1000,   -70,   290, -1000, -1000,   290, -1000,
   290,  -179,   290, -1000, -1000, -1000,   -70,   -70, -1000,   -70,
  -175,   -69,   -83, -1000, -1000, -1000, -1000,  -280, -1000,  -280,
  -271,  -271, -1000,  -133,  -280, -1000, -1000,   -42, -1000, -1000,
 -1000,   -73,   -94, -1000, -1000, -1000, -1000, -1000,  -179,   290,
  -175,  -118,   -70,  -288, -1000,  -280, -1000, -1000,  -112,   -94,
   -70,  -112, -1000,   -94,  -288,   -94, -1000 };
yytabelem yypgo[]={

     0,   703,    33,   452,   393,   462,   301,   702,     0,   701,
   700,   699,   698,   697,   696,   695,   347,   693,   691,   690,
   689,   688,   687,   686,   685,   684,   683,   682,   681,   680,
   679,   678,   677,   676,   675,   674,   673,   672,   671,   670,
   669,   668,   667,   666,   665,   664,   323,   305,    39,   663,
   662,    20,   311,   105,    18,    11,   293,   661,   660,   658,
   657,   656,    16,   371,   655,   417,   344,   654,     8,   653,
   652,   651,   302,   178,   398,   650,    13,   333,   648,   647,
    10,   646,     3,     6,   645,     7,     2,     1,     9,   644,
     5,   643,   641,   640,   639,   638,   637,   636,   632,   292,
   631,   630,   625,     4,   611,   610,   605,   361,   604,   601,
   354,   599,   597,   593,   591,   590,    19,   583,   582,   576,
   569,   351,   561,   560,   550,   548,   542,   538,   536,   332,
   526,    23,   517,   515,    15,    12,    66,   310,   312,   512,
   510,   276,   353,   423,   518,   504,   425,   501,   499,   490,
   487,   342,   479,   482,   480,   471,   468,   466,   459,    17,
   458,    14,   455,   446,   421,   420,   418,   395,   394,   385,
   378,   376,   345 };
yytabelem yyr1[]={

     0,    10,    10,    11,    11,    11,    11,    11,    11,    12,
    16,    16,    16,    18,     1,    13,    13,    13,    13,    13,
    13,    13,    13,    13,    13,    13,    13,    13,    13,    13,
    13,    13,    14,    14,    14,    14,    14,    14,    14,    15,
    15,    19,    46,    20,    20,    55,    55,    21,    22,    59,
    23,    23,    63,    63,    64,    64,    24,    24,    24,    24,
    70,    70,    72,    72,    71,    71,    25,    26,    27,    27,
    84,    84,    86,    86,    85,    85,    89,    89,    90,    90,
    28,    91,    92,    92,    99,    99,    99,    99,    99,    93,
    93,    93,    94,    94,    95,    95,    95,    96,    96,    96,
    97,    97,    98,    98,    29,    30,    31,   107,    32,   110,
    33,   114,    34,    35,    36,    37,   118,   118,    38,    43,
    44,   121,   121,   122,   122,    39,    40,    41,    41,   125,
   129,   129,   130,   131,   131,   132,   133,   126,   126,   137,
   128,   128,   138,   127,    42,    17,    17,     9,     9,   140,
   140,   140,   140,   140,   140,   143,   142,   144,   144,   144,
   145,   146,   149,   149,   150,   150,   141,   141,   151,     2,
     2,     2,     2,     4,     4,     4,     7,     6,     6,   153,
   153,   134,     3,     3,     3,     5,   152,     8,    62,    62,
   103,   103,   103,    67,    67,   116,    48,    48,    48,    48,
    80,    80,    80,    80,    80,   156,   156,    52,    52,    56,
    47,   159,   159,   160,   160,   136,   136,   136,   136,   136,
   136,   136,   136,   136,   136,   163,   162,   164,   164,   164,
   164,   164,   164,   171,   171,   161,   161,   155,   154,   158,
   157,   157,   157,   166,   167,   168,   165,   165,   170,   169,
   115,    45,    50,   117,    54,    54,    57,   119,    77,    58,
    61,    69,    75,   120,    65,   135,   135,    81,    66,   104,
   105,   106,   123,   109,   124,   112,   147,   139,    74,    68,
    49,    49,    49,    49,    49,   101,    87,    73,    60,    60,
    60,    60,   102,    78,    78,    78,    78,    76,    79,    79,
   148,    51,   100,    83,   172,   108,   108,   108,   108,    53,
    82,   111,   111,   111,   113,    88 };
yytabelem yyr2[]={

     0,     5,     0,     5,     5,     5,     2,     2,     2,     1,
     2,     3,     3,     3,     3,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     8,     4,    14,    12,     2,     2,    10,     6,     4,
     4,     8,     2,     2,     2,     2,     2,     4,     4,     6,
     2,     6,     2,     2,     2,     2,    14,    12,    14,    10,
     2,     6,     2,     6,     4,     0,     6,    10,     3,     3,
    20,     4,     2,     6,     3,     3,     3,     3,     2,     2,
     2,     2,     6,     0,     4,     4,     0,     4,     4,     0,
    16,     0,     8,     0,     4,     8,     8,     4,     8,     4,
     8,     6,    10,     8,     8,    10,     2,     3,     6,     3,
     5,     3,     4,     3,     3,     8,     8,     5,     5,     9,
     3,     1,     6,     2,     6,     8,     1,     9,     3,     1,
     9,     3,     1,     9,    12,     7,     5,     2,     2,     2,
     2,     4,     4,     3,     5,     3,     3,     4,     2,     4,
     5,     7,     4,     0,     7,     2,     2,     6,     3,     3,
     5,     5,     3,     5,     5,     4,     3,     5,     5,     3,
     3,     3,     3,     5,     5,     3,     3,     3,     2,     6,
     3,     3,     3,     2,     6,     3,     4,     6,     5,     0,
     6,     4,     6,     2,     0,     6,    10,     6,    10,     3,
     6,     2,     6,     6,     2,     2,     2,     2,     6,     6,
     4,     8,    12,     8,    12,    10,    14,     3,     3,     3,
     3,     2,     2,     6,     2,     6,     7,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     5,     3,     3,     5,     3,     3,     3,     5,     3,     3,
     3,     3,     5,     3,     3,     3,     3,     3,     3,     3,
     5,     3,     5,     3,     5,     3,     3,     5,     3,     3,
     2,     2,     2,     2,     0,     3,     3,     3,     2,     2,
     2,     0,     3,     2,     2,     2,     2,     3,     2,     0,
     3,     3,     3,     3,     3,     2,     2,     2,     0,     3,
     3,     2,     2,     0,     3,     3 };
yytabelem yychk[]={

 -1000,   -10,   -11,   -12,   -16,   -17,   256,   300,   -18,    -1,
  -140,   308,   309,  -142,  -143,  -144,   278,   279,  -145,  -147,
   280,   -13,   -14,   -15,   -19,   -20,   -21,   -22,   -23,   -24,
   -25,   -26,   -27,   -28,   -29,   -30,   -31,   -32,   -33,   -34,
   -35,   -36,   -37,   -38,   -39,   -40,   -41,   -42,   -43,   -44,
   -45,   -50,   -57,   -58,   -61,   -69,   -75,   -77,   -81,   -91,
  -104,  -105,  -106,  -109,  -112,  -115,  -117,  -119,  -123,  -124,
  -125,  -127,   277,   276,  -120,   257,   258,   259,   261,   262,
   263,   264,   260,   265,   266,   268,   269,   270,   271,  -139,
   275,  -141,   313,  -151,    -3,   295,    -5,   317,  -143,  -142,
  -144,  -146,  -148,   308,  -146,   295,   -46,   -49,   286,   290,
   291,   289,    -8,    -2,    -3,   312,    -4,    -5,    -7,   281,
    -8,   -59,   -60,   285,   289,   284,   -62,   -63,    -8,   -65,
   -66,   267,   273,   -68,   -70,   -71,   282,   -72,   -63,   -74,
    -8,   -56,   274,   296,    -8,   -65,   -66,   -74,    -8,    -8,
   -92,   -99,   270,   257,   261,   269,   -68,   -62,    -8,  -107,
  -108,   286,   285,   289,  -110,  -129,  -111,   272,   286,   291,
    -8,   -46,    -8,    -8,  -107,  -110,  -126,   308,  -137,  -128,
   308,  -138,   -77,   257,   258,   259,   269,   270,  -121,  -122,
    -8,   296,   289,   288,  -129,    -9,   -73,   313,   287,   301,
  -152,   310,    -3,  -149,   -47,   -51,   302,    -8,   -51,   295,
   281,  -152,    -4,    -6,  -152,  -153,   304,   314,   -51,   -48,
   -88,   292,    -8,   -73,    -8,   -73,   -62,   -76,   287,   -78,
   289,   290,   285,   288,    -8,   -76,   -82,   291,   -93,   -73,
   -83,  -100,   -82,   289,   288,   -76,   -47,    -8,   -47,  -130,
   -51,    -8,  -113,   293,  -116,   302,  -116,  -116,  -116,  -116,
  -137,  -138,   -74,  -121,  -116,  -151,   309,  -150,  -142,   300,
   -48,  -159,  -160,    -8,  -161,   -52,   -53,    -8,   303,    -6,
   295,   -52,   -80,   277,    -2,   -51,  -154,  -156,   321,  -136,
  -162,  -163,  -164,  -166,  -167,  -168,    -8,  -169,   297,   298,
   299,   296,  -134,  -161,   316,   322,   323,   324,    -8,   -64,
   -67,   -68,  -103,   297,   298,    -2,   -72,    -8,    -8,   -47,
  -103,    -8,    -8,   -99,    -8,   -48,   -48,   -48,  -131,  -132,
  -133,  -114,    -8,   -48,  -118,   -54,    -2,   286,   284,   -48,
   -48,   -16,   -16,    -8,   -48,  -141,   -53,   -73,  -135,  -172,
   287,   283,   304,   -53,   -73,   -54,   -76,   -53,  -155,   320,
  -116,   -80,  -136,    -2,   -80,  -157,  -165,   319,   318,   287,
   317,   316,  -136,   -51,   -51,   -51,   -51,   -51,   -73,   -51,
   -79,   287,   -48,   -83,   -85,   -88,   -94,   -51,   -53,   -73,
  -134,    -2,  -103,   -55,   -56,    -8,   309,   309,  -116,   313,
  -159,  -136,    -8,   -68,   -54,    -8,   -55,    -8,   -80,   -53,
   -53,  -136,  -136,  -136,  -136,  -136,  -136,  -136,  -103,   -62,
   -80,   -84,   -86,    -8,   -89,    -8,   -95,   -82,   -62,  -131,
  -135,  -103,   -48,   -55,   -76,  -158,   318,   -53,   -73,   -53,
   -73,   -48,  -170,   283,   -53,   -85,   -73,   -87,   315,   -73,
   -76,   -96,  -101,   294,    -8,   -68,   -53,  -136,    -8,  -136,
  -136,  -136,   -53,  -171,  -136,   -86,    -8,    -8,   -90,   297,
    -2,   -97,  -102,   284,    -8,   -68,   -53,   -53,   -48,   -73,
   -76,   -98,   -83,  -103,   -53,  -136,   -90,   -48,    -8,   -87,
   -82,  -103,    -8,   -82,  -103,   -87,  -103 };
yytabelem yydef[]={

     2,    -2,     1,     0,     6,     7,     8,    10,    11,    12,
     0,    13,    14,   149,   150,   153,   156,   155,   158,     0,
   276,     3,     4,     5,    15,    16,    17,    18,    19,    20,
    21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
    31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
   284,     0,     0,   291,     0,    56,     0,     0,     0,     0,
     0,     0,   308,    -2,     0,   284,     0,     0,   308,   313,
   139,   142,     0,   119,     0,   251,   252,   256,   259,   260,
   261,     0,   258,   267,   269,     0,   271,   273,   275,   131,
   263,     0,   146,   166,   168,   182,     0,   185,   151,   152,
   154,   157,   163,   300,   159,   160,     0,     0,   280,   281,
   282,   283,     0,   187,   169,     0,   172,     0,     0,   176,
     0,   199,     0,   288,   289,   290,    50,     0,   188,    52,
    53,   264,   268,    57,    58,     0,   279,    60,    64,    65,
    62,    63,   278,   209,     0,     0,    81,     0,     0,     0,
     0,    82,    84,    85,    86,    87,    88,   104,     0,     0,
     0,   305,   306,   307,     0,     0,     0,   130,   311,   312,
     0,     0,     0,     0,     0,     0,   127,   139,   138,   128,
   142,   141,     0,   250,   253,   257,   272,    -2,   120,   121,
   123,   124,   262,   270,     0,   145,     0,   147,   148,   287,
   184,   186,   183,     0,   199,     0,   301,    42,     0,   170,
   171,   174,   173,   175,     0,     0,   179,   180,     0,    48,
   204,   315,    49,     0,     0,     0,    59,     0,   297,     0,
   293,   294,   295,   296,     0,     0,     0,   310,     0,     0,
    89,    90,    91,   303,   302,     0,   199,   107,   199,   199,
   136,   109,     0,   314,   199,   195,     0,   118,   199,   199,
     0,     0,     0,   122,   199,   167,   161,   162,     0,   165,
    41,     0,   211,     0,   214,     0,     0,     0,   309,   177,
   178,     0,   196,     0,    -2,   204,   204,   203,   238,     0,
   215,   216,   217,     0,     0,     0,     0,     0,   227,   228,
   229,   230,   231,   232,   243,   244,   245,   249,   189,    51,
    54,    55,   193,   190,   191,   192,    61,     0,   299,   199,
   113,    75,    93,    83,   105,   106,   108,   129,     0,   133,
     0,   110,     0,   114,     0,   116,   117,   254,   255,   125,
   126,     0,     0,     0,   143,     0,   210,     0,     0,     0,
   265,   266,   304,     0,     0,     0,     0,    47,   204,   237,
   197,     0,     0,    -2,   201,     0,     0,   240,   241,   242,
   246,   247,   220,     0,     0,     0,     0,     0,     0,     0,
   204,   298,   112,     0,    69,     0,    96,     0,   132,   136,
     0,   181,     0,   115,    45,    46,   137,   140,   199,   164,
   212,   213,   235,   236,     0,     0,    44,   207,   202,   200,
   219,   205,   218,     0,     0,     0,     0,   199,   194,     0,
    67,    75,    70,    72,    74,     0,    99,     0,     0,   134,
     0,   111,   144,    43,     0,     0,   239,   221,     0,   223,
     0,     0,     0,   248,    66,    68,     0,     0,   286,     0,
     0,   101,     0,   285,    94,    95,    92,   135,   208,   206,
     0,     0,   225,   199,   234,    71,    73,     0,    76,    78,
    79,   103,     0,   292,    97,    98,   222,   224,     0,     0,
     0,   199,     0,     0,   226,   233,    77,    80,     0,     0,
     0,     0,   102,     0,     0,     0,   100 };
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
	"DEFINE",	260,
	"DELETE",	261,
	"DESTROY",	262,
	"HELP",	263,
	"INDEX",	264,
	"MODIFY",	265,
	"PRINT",	266,
	"INTEGRITY",	267,
	"RANGE",	268,
	"REPLACE",	269,
	"RETRIEVE",	270,
	"SAVE",	271,
	"UNIQUE",	272,
	"PERMIT",	273,
	"VIEW",	274,
	"RUBIN",	275,
	"EXIT",	276,
	"PARAM",	277,
	"TYPE",	278,
	"ALLOC",	279,
	"STRUCT",	280,
	"STRUCT_VAR",	281,
	"ALL",	282,
	"BY",	283,
	"FROM",	284,
	"IN",	285,
	"INTO",	286,
	"IS",	287,
	"OF",	288,
	"ON",	289,
	"ONTO",	290,
	"TO",	291,
	"WHERE",	292,
	"UNTIL",	293,
	"AT",	294,
	"NAME",	295,
	"SCONST",	296,
	"I2CONST",	297,
	"I4CONST",	298,
	"F8CONST",	299,
	"C_CODE",	300,
	"COMMA",	301,
	"LPAREN",	302,
	"RPAREN",	303,
	"PERIOD",	304,
	"QUOTE",	305,
	"BGNCMNT",	306,
	"ENDCMNT",	307,
	"LBRACE",	308,
	"RBRACE",	309,
	"LBRKT",	310,
	"RBRKT",	311,
	"NONREF",	312,
	"SEMICOL",	313,
	"POINTER",	314,
	"COLON",	315,
	"UOP",	316,
	"BOP",	317,
	"BDOP",	318,
	"EOP",	319,
	"LBOP",	320,
	"LUOP",	321,
	"FOP",	322,
	"FBOP",	323,
	"AOP",	324,
	"unaryop",	325,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"program : program statement",
	"program : /* empty */",
	"statement : startquel quel_statement",
	"statement : startquel equel_statement",
	"statement : startquel rubin_statement",
	"statement : c_code",
	"statement : declaration",
	"statement : error",
	"startquel : /* empty */",
	"c_code : C_CODE",
	"c_code : beginblock",
	"c_code : endblock",
	"beginblock : LBRACE",
	"endblock : RBRACE",
	"quel_statement : append",
	"quel_statement : copy",
	"quel_statement : create",
	"quel_statement : delete",
	"quel_statement : destroy",
	"quel_statement : help",
	"quel_statement : index",
	"quel_statement : integrity",
	"quel_statement : modify",
	"quel_statement : permit",
	"quel_statement : print",
	"quel_statement : range",
	"quel_statement : replace",
	"quel_statement : retrieve",
	"quel_statement : save",
	"quel_statement : view",
	"quel_statement : lets",
	"equel_statement : append_p",
	"equel_statement : copy_p",
	"equel_statement : create_p",
	"equel_statement : replace_p",
	"equel_statement : retrieve_p",
	"equel_statement : tupret",
	"equel_statement : view_p",
	"rubin_statement : exit",
	"rubin_statement : rubin",
	"append : append_key apclause tlclause qualclause",
	"apclause : apkword id",
	"copy : copy_key id lparen ctl rparen cp_kword filename",
	"copy : copy_key id lparen rparen cp_kword filename",
	"filename : sconst",
	"filename : id",
	"create : create_key id lparen ctl rparen",
	"delete : delete_key delclause qualclause",
	"delclause : delnoise id",
	"destroy : destroy_key idlist",
	"destroy : destroy_key integ_permit id int_list_all",
	"integ_permit : integ_key",
	"integ_permit : permit_key",
	"int_list_all : int_list",
	"int_list_all : all",
	"help : help_key",
	"help : help_key all",
	"help : help_key hlist",
	"help : help_key int_perm_view idlist",
	"hlist : hparam",
	"hlist : hlist comma hparam",
	"hparam : id",
	"hparam : sconst",
	"int_perm_view : integ_permit",
	"int_perm_view : view_key",
	"index : index_key id is id lparen idlist rparen",
	"integrity : define_key integ_key integnoise id isnoise qual",
	"modify : modify_key id to id on modkeylist density",
	"modify : modify_key id to id density",
	"modkeylist : modkey",
	"modkeylist : modkeylist comma modkey",
	"modkey : id",
	"modkey : id colon id",
	"density : where modfill",
	"density : /* empty */",
	"modfill : id is mod_var",
	"modfill : modfill comma id is mod_var",
	"mod_var : I2CONST",
	"mod_var : c_variable",
	"permit : def_perm permit_list on_of_to id perm_tl perm_who perm_term perm_time perm_day qualclause",
	"def_perm : define_key permit_key",
	"permit_list : permlistelm",
	"permit_list : permit_list comma permlistelm",
	"permlistelm : RETRIEVE",
	"permlistelm : APPEND",
	"permlistelm : DELETE",
	"permlistelm : REPLACE",
	"permlistelm : all",
	"on_of_to : on",
	"on_of_to : of",
	"on_of_to : to",
	"perm_tl : lparen idlist rparen",
	"perm_tl : /* empty */",
	"perm_who : to id",
	"perm_who : to all",
	"perm_who : /* empty */",
	"perm_term : at id",
	"perm_term : at all",
	"perm_term : /* empty */",
	"perm_time : from integer colon integer to integer colon integer",
	"perm_time : /* empty */",
	"perm_day : on id to id",
	"perm_day : /* empty */",
	"print : print_key idlist",
	"range : range_of id is id",
	"replace : replace_key repclause tlclause qualclause",
	"repclause : repkword id",
	"retrieve : retrieve_key resclause tlclause qualclause",
	"resclause : retkword id",
	"save : save_key id until date",
	"date : id integer integer",
	"view : define_key view_key id tlclause qualclause",
	"lets : define_key id is integer",
	"append_p : append_p_key apclause param_tl qualclause",
	"copy_p : copy_p_key id param_tl fr_in_id filename",
	"fr_in_id : cp_kword",
	"fr_in_id : c_variable",
	"create_p : create_p_key id param_tl",
	"exit : EXIT",
	"rubin : rubin_key param_list",
	"param_list : param",
	"param_list : param param_list",
	"param : id",
	"param : SCONST",
	"replace_p : replace_p_key repclause param_tl qualclause",
	"retrieve_p : retrieve_p_key resclause param_tl qualclause",
	"tupret : tupret_keyw xc_code",
	"tupret : tupret_p o_xc_code",
	"tupret_keyw : retrieve_key unique c_tlclause qualclause",
	"unique : UNIQUE",
	"unique : /* empty */",
	"c_tlclause : lparen c_tlist rparen",
	"c_tlist : c_tlelm",
	"c_tlist : c_tlelm comma c_tlist",
	"c_tlelm : reduc cvar is_key afcn",
	"reduc : /* empty */",
	"xc_code : LBRACE gen_while c_code RBRACE",
	"xc_code : gen_while",
	"gen_while : /* empty */",
	"o_xc_code : LBRACE o_gen_while c_code RBRACE",
	"o_xc_code : o_gen_while",
	"o_gen_while : /* empty */",
	"tupret_p : tupret_p_key unique param_tl qualclause",
	"view_p : PARAM define_key view_key id param_tl qualclause",
	"declaration : decl_specifer declarator_list end_decl",
	"declaration : decl_specifer SEMICOL",
	"end_decl : SEMICOL",
	"end_decl : IS",
	"decl_specifer : type_specifier",
	"decl_specifer : sc_specifier",
	"decl_specifer : type_specifier sc_specifier",
	"decl_specifer : sc_specifier type_specifier",
	"decl_specifer : struct_dec",
	"decl_specifer : sc_specifier struct_dec",
	"sc_specifier : ALLOC",
	"type_specifier : TYPE",
	"struct_dec : struct_name field_declaration",
	"struct_dec : struct_name",
	"struct_dec : struct_key field_declaration",
	"struct_name : struct_key NAME",
	"field_declaration : lbrace field_seq RBRACE",
	"field_seq : field_seq field",
	"field_seq : /* empty */",
	"field : type_specifier declarator_list SEMICOL",
	"field : C_CODE",
	"declarator_list : cvar_dec",
	"declarator_list : declarator_list comma cvar_dec",
	"cvar_dec : cvarx",
	"c_variable : cvarx",
	"c_variable : NONREF NAME",
	"c_variable : NONREF STRUCT_VAR",
	"c_variable : struct_var",
	"struct_var : ptr struct_var",
	"struct_var : struct_var arraysub",
	"struct_var : str_var_key selector_part",
	"str_var_key : STRUCT_VAR",
	"selector_part : arraysub selector_part",
	"selector_part : select_op NAME",
	"select_op : PERIOD",
	"select_op : POINTER",
	"cvar : c_variable",
	"cvarx : NAME",
	"cvarx : ptr cvarx",
	"cvarx : cvarx arraysub",
	"ptr : BOP",
	"arraysub : LBRKT",
	"id : c_variable",
	"idlist : id",
	"idlist : idlist comma id",
	"integer : I2CONST",
	"integer : I4CONST",
	"integer : c_variable",
	"int_list : integer",
	"int_list : int_list comma integer",
	"param_tl : LPAREN",
	"qualclause : where qual",
	"qualclause : where PARAM param_tl",
	"qualclause : where c_variable",
	"qualclause : /* empty */",
	"qual : lparen qual rparen",
	"qual : luop qual",
	"qual : qual lbop qual",
	"qual : clause",
	"qual : /* empty */",
	"clause : afcn rop afcn",
	"clause : afcn rop afcn bdop afcn",
	"ctl : id is id",
	"ctl : ctl comma id is id",
	"sconst : SCONST",
	"tlclause : lparen tlist rparen",
	"tlist : tlelm",
	"tlist : tlelm comma tlist",
	"tlelm : id is_key afcn",
	"tlelm : attrib",
	"afcn : aggrfcn",
	"afcn : aggr",
	"afcn : attribfcn",
	"afcn : afcn bop afcn",
	"afcn : lparen afcn rparen",
	"afcn : uop afcn",
	"afcn : fop lparen afcn rparen",
	"afcn : fbop lparen afcn comma afcn rparen",
	"afcn : id lparen afcn rparen",
	"afcn : id lparen afcn comma afcn rparen",
	"aggr : aop lparen afcn qualclause rparen",
	"aggrfcn : aop lparen afcn by aseq qualclause rparen",
	"attribfcn : I2CONST",
	"attribfcn : I4CONST",
	"attribfcn : F8CONST",
	"attribfcn : SCONST",
	"attribfcn : cvar",
	"attribfcn : attrib",
	"aseq : aseq comma afcn",
	"aseq : afcn",
	"attrib : id period id",
	"attrib : id period all",
	"lbop : LBOP",
	"luop : LUOP",
	"bdop : BDOP",
	"rop : EOP",
	"rop : BDOP",
	"rop : IS",
	"uop : UOP",
	"fop : FOP",
	"fbop : FBOP",
	"bop : BOP",
	"bop : UOP",
	"by : BY",
	"aop : AOP",
	"append_p_key : PARAM APPEND",
	"append_key : APPEND",
	"copy_key : COPY",
	"copy_p_key : PARAM COPY",
	"cp_kword : INTO",
	"cp_kword : FROM",
	"create_key : CREATE",
	"create_p_key : PARAM CREATE",
	"define_key : DEFINE",
	"delete_key : DELETE",
	"destroy_key : DESTROY",
	"help_key : HELP",
	"index_key : INDEX ON",
	"rubin_key : RUBIN",
	"integ_key : INTEGRITY",
	"is_key : IS",
	"is_key : BY",
	"modify_key : MODIFY",
	"permit_key : PERMIT",
	"print_key : PRINT",
	"range_of : RANGE OF",
	"replace_key : REPLACE",
	"replace_p_key : PARAM REPLACE",
	"retrieve_key : RETRIEVE",
	"retrieve_p_key : PARAM RETRIEVE",
	"save_key : SAVE",
	"struct_key : STRUCT",
	"tupret_p_key : PARAM RETRIEVE",
	"view_key : VIEW",
	"all : ALL",
	"apkword : INTO",
	"apkword : ONTO",
	"apkword : TO",
	"apkword : ON",
	"apkword : /* empty */",
	"at : AT",
	"colon : COLON",
	"comma : COMMA",
	"delnoise : IN",
	"delnoise : ON",
	"delnoise : FROM",
	"delnoise : /* empty */",
	"from : FROM",
	"integnoise : ON",
	"integnoise : ONTO",
	"integnoise : IN",
	"integnoise : OF",
	"is : IS",
	"isnoise : IS",
	"isnoise : /* empty */",
	"lbrace : LBRACE",
	"lparen : LPAREN",
	"of : OF",
	"on : ON",
	"period : PERIOD",
	"repkword : INTO",
	"repkword : IN",
	"repkword : ON",
	"repkword : /* empty */",
	"rparen : RPAREN",
	"to : TO",
	"retkword : INTO",
	"retkword : TO",
	"retkword : /* empty */",
	"until : UNTIL",
	"where : WHERE",
};
#endif /* YYDEBUG */
/*#ident  "@(#)yaccpar    2.3 - 88/05/27"       */

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
# line 120 "grammar.y"
 
		{
			/* for each "statement", free the symbol space
			 * used by that query (lookahed space must not
			 * be freed), and catch up on output lines
			 */
			symspfree();
			equate_lines();
		} break;
case 3:
# line 131 "grammar.y"
 
		{
			w_sync();
			/* the purpose of the actions for startquel
			 * and this action is to make each query
			 * a single compound C statement (if (x) "{query}")
			 */
			w_op("}");
			Inqual = 0;
		} break;
case 4:
# line 141 "grammar.y"

		{
			end_quote();
			w_sync();
			w_op("}");
			Inqual = 0;
		} break;
case 5:
# line 148 "grammar.y"

		{
			end_quote();
			w_op("}");
			Inqual = 0;
		} break;
case 9:
# line 158 "grammar.y"

			w_op("{"); break;
case 11:
# line 168 "grammar.y"

			Block_level += 1; break;
case 12:
# line 170 "grammar.y"

		{
			if (Block_level == 0)
				yyserror("extra '}'", yypvt[-0].u_dn);
			else if ((Block_level -= 1) == 0)
			{
				freecvar(&C_locals);
				freecvar(&F_locals);
			}
		} break;
case 13:
# line 185 "grammar.y"

			w_op(yypvt[-0].u_dn->d_elm); break;
case 14:
# line 188 "grammar.y"

			w_op(yypvt[-0].u_dn->d_elm); break;
case 78:
# line 297 "grammar.y"

			w_con(I2CONST, yypvt[-0].u_dn->d_elm); break;
case 79:
# line 299 "grammar.y"

		{
			if (yypvt[-0].u_dn)
			{
				if (!Cvarp)
					w_key(yypvt[-0].u_dn->d_elm);
				else if (Fieldp && Fieldp->c_type == opINT
					|| Cvarp->c_type == opINT)
						w_var(Cv_display, opINT);
				else if (Fieldp && Fieldp->c_type == opSTRING 
					|| Cvarp->c_type == opSTRING)
						w_var(Cv_display, opIDSTRING);
				else
					yyserror("in MODIFY, qual var must be in or string",
					yypvt[-0].u_dn);
			}
			else
				yyserror("bad modify qualification", 0);
			free_display(Cv_display);
			Cvarp = Fieldp = 0;
		} break;
case 84:
# line 330 "grammar.y"

			w_key(yypvt[-0].u_dn->d_elm); break;
case 85:
# line 332 "grammar.y"

			w_key(yypvt[-0].u_dn->d_elm); break;
case 86:
# line 334 "grammar.y"

			w_key(yypvt[-0].u_dn->d_elm); break;
case 87:
# line 336 "grammar.y"
 
			w_key(yypvt[-0].u_dn->d_elm); break;
case 117:
# line 400 "grammar.y"

		{
			if (yypvt[-0].u_dn && Cvarp)
			{
				if (Fieldp && Fieldp->c_type != opSTRING
				   || !Fieldp && Cvarp->c_type != opSTRING)
					yyserror("string var expected for from/into in COPY",
					yypvt[-0].u_dn);
				else
					w_var(Cv_display, opIDSTRING);
			}
			else
				yyserror("into/from expected in COPY", yypvt[-0].u_dn);
			free_display(Cv_display);
			Fieldp = Cvarp = 0;
		} break;
case 119:
# line 421 "grammar.y"
 
		{ 
			Opflag = mdEXIT;
			w_new("IIexit();"); 
		} break;
case 120:
# line 428 "grammar.y"
 
			w_op(");"); break;
case 121:
# line 431 "grammar.y"

			w_op("0"); break;
case 123:
# line 435 "grammar.y"

			w_op(","); break;
case 124:
# line 437 "grammar.y"

		{
			w_string(yypvt[-0].u_dn->d_elm, 0);
			w_op(",");
		} break;
case 127:
# line 450 "grammar.y"

			w_flush(); break;
case 128:
# line 452 "grammar.y"

			w_flush(); break;
case 129:
# line 455 "grammar.y"

		{
			w_new("IIsetup();");
		} break;
case 130:
# line 460 "grammar.y"

		{
			Opflag = mdTUPRET;
			Inqual = 0;
			w_key(yypvt[-0].u_dn->d_elm);
		} break;
case 131:
# line 466 "grammar.y"

			{
			Inqual = 0;
			Opflag = mdTUPRET;
			} break;
case 136:
# line 479 "grammar.y"

			Opflag = mdCTLELM; break;
case 137:
# line 482 "grammar.y"

			w_op("}"); break;
case 138:
# line 484 "grammar.y"

			w_op("}"); break;
case 139:
# line 487 "grammar.y"

		{
			w_new("while(IIn_get(");
			w_file();
			w_op(")){");
			w_ret();
			free_ret();
			w_op("if(IIerrtest())continue;");
			equate_lines();
		} break;
case 140:
# line 498 "grammar.y"

			w_op("}"); break;
case 141:
# line 500 "grammar.y"

			w_op("}"); break;
case 142:
# line 503 "grammar.y"

		{
			w_new("while(IIgettup(");
			w_file();
			w_op(")){");
			equate_lines();
		} break;
case 143:
# line 511 "grammar.y"

		{
			w_new("IIsetup();");
		} break;
case 145:
# line 524 "grammar.y"

		{
			w_op(yypvt[-0].u_dn->d_elm);
			Type_spec = 0;
		} break;
case 146:
# line 529 "grammar.y"
 
		{ 
			w_op(yypvt[-0].u_dn->d_elm);
			Type_spec = 0;
		} break;
case 153:
# line 542 "grammar.y"

		{
			Struct_flag = 0;
			Type_spec = opSTRUCT;
		} break;
case 154:
# line 547 "grammar.y"

		{
			Struct_flag = 0;
			Type_spec = opSTRUCT;
		} break;
case 155:
# line 553 "grammar.y"

		{
			Opflag = mdDECL;
			w_key(yypvt[-0].u_dn->d_elm);
			/* in case the default "int" should be assumed,
			 * the Type_spec is set up for it, if a previous
			 * type hasn't been given
			 */
			if (!Type_spec)
				Type_spec = opINT;
		} break;
case 156:
# line 565 "grammar.y"

		{
			Opflag = mdDECL;
			w_key(yypvt[-0].u_dn->d_elm);
			Type_spec = Opcode;
		} break;
case 160:
# line 576 "grammar.y"

			w_key(yypvt[-0].u_dn->d_elm); break;
case 161:
# line 579 "grammar.y"

		{
			w_op(yypvt[-0].u_dn->d_elm);
			Type_spec = 0;
		} break;
case 164:
# line 588 "grammar.y"

		{
			w_op(yypvt[-0].u_dn->d_elm);
			Type_spec = 0;
		} break;
case 168:
# line 598 "grammar.y"

		{
			if (Type_spec == opSTRING)
				Indir_level -= 1;
			if (Struct_flag)
				decl_field(yypvt[-0].u_dn->d_elm, Type_spec,
					Indir_level, Block_level);
			else
				decl_cvar(yypvt[-0].u_dn->d_elm, Type_spec,
					Indir_level, Block_level);
			free_display(Cv_display);
			Indir_level = Field_indir = 0;
			Fieldp = Cvarp = 0;
		} break;
case 169:
# line 613 "grammar.y"

		{
			yyval.u_dn = yypvt[-0].u_dn;
			if (Cvarp && Cvarp->c_indir != Indir_level)
			{
				yyserror("bad indirection on a C variable", yypvt[-0].u_dn);
				yyval.u_dn = 0;
			}
			Indir_level = Field_indir = 0;
		} break;
case 170:
# line 623 "grammar.y"

		{
			enter_display(Cv_display, salloc(yypvt[-1].u_dn->d_elm));
			Cvarp = Fieldp = 0;
			yyval.u_dn = yypvt[-0].u_dn;
		} break;
case 171:
# line 629 "grammar.y"

		{
			enter_display(Cv_display, salloc(yypvt[-1].u_dn->d_elm));
			Cvarp = Fieldp = 0;
			yyval.u_dn = yypvt[-0].u_dn;
		} break;
case 172:
# line 635 "grammar.y"

		{
			if (!Fieldp)
			{
				yyserror("undeclared field", yypvt[-0].u_dn);
				yyval.u_dn = yypvt[-1].u_dn;
			}
			else if (Fieldp->c_indir != Field_indir)
			{
				yyserror("bad indirection on a structure's field",
				yypvt[-0].u_dn);
				yyval.u_dn = 0;
			}
			if (Cvarp->c_indir != Indir_level)
			{
				yysemerr("bad indirection a structure variable",
				Cvarp->c_indir);
				yyval.u_dn = 0;
			}
			Indir_level = Field_indir = 0;
		} break;
case 173:
# line 657 "grammar.y"

		{
			if (yypvt[-1].u_dn->d_elm[1] == '*')
				Field_indir += 1;
			Field_indir += 1;
			yyval.u_dn = yypvt[-0].u_dn;
		} break;
case 174:
# line 664 "grammar.y"

			Field_indir += 1; break;
case 176:
# line 668 "grammar.y"

		{
			Cvarp = getcvar(yypvt[-0].u_dn->d_elm);
			enter_display(Cv_display, yypvt[-0].u_dn->d_elm);
		} break;
case 177:
# line 674 "grammar.y"

		{
			Indir_level += 1;
			yyval.u_dn = yypvt[-0].u_dn;
		} break;
case 178:
# line 679 "grammar.y"

		{
			enter_display(Cv_display, yypvt[-0].u_dn->d_elm);
			Fieldp = getfield(yypvt[-0].u_dn->d_elm);
			yyval.u_dn = yypvt[-0].u_dn;
		} break;
case 179:
# line 686 "grammar.y"

			enter_display(Cv_display, yypvt[-0].u_dn->d_elm); break;
case 180:
# line 688 "grammar.y"

		{
			enter_display(Cv_display, yypvt[-0].u_dn->d_elm);
			Indir_level += 1;
		} break;
case 181:
# line 696 "grammar.y"
	
		{
			if (yypvt[-0].u_dn)
			{
				if (!Fieldp && ! Cvarp)
				{
					if (!Field_indir && !Indir_level
					  && ( sequal(yypvt[-0].u_dn->d_elm, "dba")
					    || sequal(yypvt[-0].u_dn->d_elm, "sysdate")
					    || sequal(yypvt[-0].u_dn->d_elm, "systime")
					    || sequal(yypvt[-0].u_dn->d_elm, "usercode")))
						/* constant operator COP */
						w_key(yypvt[-0].u_dn->d_elm);
					else
						yyserror("C var expected", yypvt[-0].u_dn);
				}
				else if (Opflag == mdCTLELM)
				{
					w_con(NAME,
					  Fieldp ? Fieldp->c_id: Cvarp->c_id);
					enter_ret(Cv_display,
					  Fieldp ? Fieldp->c_type: Cvarp->c_type);
				}
				else
					w_var(Cv_display,
					  Fieldp ? Fieldp->c_type: Cvarp->c_type);
			}
			free_display(Cv_display);
			Fieldp = Cvarp = 0;
			Indir_level = Field_indir = 0;
		} break;
case 182:
# line 727 "grammar.y"

		{
			if (Opflag == mdDECL)
				w_con(NAME, yypvt[-0].u_dn->d_elm);
			else
			{
				Cvarp = getcvar(yypvt[-0].u_dn->d_elm);
				enter_display(Cv_display, salloc(yypvt[-0].u_dn->d_elm));
			}
		} break;
case 183:
# line 737 "grammar.y"

		{
			if (yypvt[-1].u_dn->d_elm [1] == '*')
				Indir_level += 1;
			Indir_level += 1;
			yyval.u_dn = yypvt[-0].u_dn;
		} break;
case 184:
# line 744 "grammar.y"

		{
			Indir_level += 1;
		} break;
case 185:
# line 749 "grammar.y"

		{
			if (!sequal(yypvt[-0].u_dn->d_elm, "*") && !sequal((struct disp_node *)(yypvt[-0].u_dn)->d_elm, "**"))
				yyserror(Opflag == mdDECL ?
				"invalid operator in declaration":
				"invalid operator in C variable",
				yypvt[-0].u_dn);
			if (Opflag == mdDECL)
				w_op(yypvt[-0].u_dn->d_elm);
			else
				enter_display(Cv_display, salloc(yypvt[-0].u_dn->d_elm));
		} break;
case 186:
# line 762 "grammar.y"

		{
			if (Opflag == mdDECL)
				eat_display(0, '[', ']');
			else
				eat_display(Cv_display, '[', ']');
		} break;
case 187:
# line 775 "grammar.y"

		{
			if (yypvt[-0].u_dn)
			{
				if (Cvarp)
				{
					if (Fieldp && Fieldp->c_type != opSTRING
					   || !Fieldp && Cvarp->c_type != opSTRING)
						yyserror("string var expected", yypvt[-0].u_dn);
					else if (Opflag == mdFILENAME)
						w_var(Cv_display, opSTRING);
					else if (Opflag == mdRUBIN)
						w_display(Cv_display);
					else
						w_var(Cv_display, opIDSTRING);
				}
				else if (Opflag == mdRUBIN)
					w_string(yypvt[-0].u_dn->d_elm, 0);
				else if (Opflag == mdFILENAME)
					yyserror("file for a COPY must be a string or string variable",
					yypvt[-0].u_dn);
				else
					w_key(yypvt[-0].u_dn->d_elm);
			}
			free_display(Cv_display);
			Fieldp = Cvarp = 0;
		} break;
case 190:
# line 808 "grammar.y"

			w_con(I2CONST, yypvt[-0].u_dn->d_elm); break;
case 191:
# line 810 "grammar.y"

			w_con(I4CONST, yypvt[-0].u_dn->d_elm); break;
case 192:
# line 812 "grammar.y"

		{
			if (yypvt[-0].u_dn)
			{
				if (Cvarp)
					if (Fieldp && Fieldp->c_type == opINT
					   || Cvarp->c_type == opINT)
						w_var(Cv_display, opINT);
					else
						yyserror("integer variable required",
						yypvt[-0].u_dn);
				else
					yyserror("integer variable required", yypvt[-0].u_dn);
			}
			free_display(Cv_display);
		} break;
case 195:
# line 834 "grammar.y"

		{
			w_op("(");
			end_quote();
			if (Opflag == mdTUPRET && Inqual == 0)
				w_key("IIw_left");
			else
				w_key("IIw_right");
			eat_display(0, '(', ')');
			w_op(";");
			begin_quote();
			w_op(")");

		} break;
case 198:
# line 852 "grammar.y"

		{
			if (!yypvt[-0].u_dn || !Cvarp)
				yyserror("C var (string) expected", yypvt[-0].u_dn);
			else if (Fieldp && Fieldp->c_type == opSTRING
				|| Cvarp->c_type == opSTRING)
			{
				w_op("(");
				end_quote();
				w_op("IIwrite(");
				w_display(Cv_display);
				w_op(");");
				begin_quote();
				w_op(")");
				end_quote();
			}
			else
				yyserror("var must be string valued for qualification",
				yypvt[-0].u_dn);
			free_display(Cv_display);
			Cvarp = Fieldp = 0;
		} break;
case 209:
# line 889 "grammar.y"
 
			w_con(SCONST, yypvt[-0].u_dn->d_elm); break;
case 227:
# line 921 "grammar.y"
 
			w_con(I2CONST, yypvt[-0].u_dn->d_elm); break;
case 228:
# line 923 "grammar.y"
 
			w_con(I4CONST, yypvt[-0].u_dn->d_elm); break;
case 229:
# line 925 "grammar.y"
 
			w_con(F8CONST, yypvt[-0].u_dn->d_elm); break;
case 230:
# line 927 "grammar.y"
 
			w_con(SCONST, yypvt[-0].u_dn->d_elm); break;
case 236:
# line 936 "grammar.y"

		{
			if (Opflag != mdVIEW && Opflag != mdRETRIEVE
			   && Opflag != mdAPPEND)
				yyserror(
				"'all' applied to this range variable illegal in this kind of statement",
				yypvt[-2].u_dn);
		} break;
case 237:
# line 945 "grammar.y"
 
			w_key(yypvt[-0].u_dn->d_elm); break;
case 238:
# line 948 "grammar.y"
 
			w_key(yypvt[-0].u_dn->d_elm); break;
case 239:
# line 951 "grammar.y"
 
			w_op(yypvt[-0].u_dn->d_elm); break;
case 240:
# line 954 "grammar.y"
 
			w_op(yypvt[-0].u_dn->d_elm); break;
case 241:
# line 956 "grammar.y"
 
			w_op(yypvt[-0].u_dn->d_elm); break;
case 242:
# line 958 "grammar.y"
 
			w_op("="); break;
case 243:
# line 961 "grammar.y"
 
			w_op(yypvt[-0].u_dn->d_elm); break;
case 244:
# line 964 "grammar.y"
 
			w_key(yypvt[-0].u_dn->d_elm); break;
case 245:
# line 967 "grammar.y"
 
			w_key(yypvt[-0].u_dn->d_elm); break;
case 246:
# line 970 "grammar.y"
 
			w_op(yypvt[-0].u_dn->d_elm); break;
case 247:
# line 972 "grammar.y"
 
			w_op(yypvt[-0].u_dn->d_elm); break;
case 248:
# line 975 "grammar.y"

			w_key(yypvt[-0].u_dn->d_elm); break;
case 249:
# line 978 "grammar.y"
 
			w_key(yypvt[-0].u_dn->d_elm); break;
case 250:
# line 986 "grammar.y"

		{
			begin_quote();
			w_key(yypvt[-0].u_dn->d_elm);
			Opflag = mdAPPEND;
		} break;
case 251:
# line 993 "grammar.y"

		{
			Opflag = mdAPPEND;
			begin_quote();
			w_key(yypvt[-0].u_dn->d_elm);
		} break;
case 252:
# line 1000 "grammar.y"

		{
			Opflag = mdCOPY;
			begin_quote();
			w_key(yypvt[-0].u_dn->d_elm);
		} break;
case 253:
# line 1007 "grammar.y"

		{
			Opflag = mdCOPY;
			begin_quote();
			w_key(yypvt[-0].u_dn->d_elm);
		} break;
case 254:
# line 1014 "grammar.y"
 
		{
			w_key(yypvt[-0].u_dn->d_elm);
			Opflag = mdFILENAME;
		} break;
case 255:
# line 1019 "grammar.y"
 
		{
			w_key(yypvt[-0].u_dn->d_elm);
			Opflag = mdFILENAME;
		} break;
case 256:
# line 1025 "grammar.y"

		{
			Opflag = mdCREATE;
			begin_quote();
			w_key(yypvt[-0].u_dn->d_elm);
		} break;
case 257:
# line 1032 "grammar.y"

		{
			Opflag = mdCREATE;
			begin_quote();
			w_key(yypvt[-0].u_dn->d_elm);
		} break;
case 258:
# line 1039 "grammar.y"

		{
			Opflag = mdDEFINE;
			begin_quote();
			w_key(yypvt[-0].u_dn->d_elm);
		} break;
case 259:
# line 1046 "grammar.y"

		{
			Opflag = mdDELETE;
			begin_quote();
			w_key(yypvt[-0].u_dn->d_elm);
		} break;
case 260:
# line 1053 "grammar.y"

		{
			Opflag = mdDESTROY;
			begin_quote();
			w_key(yypvt[-0].u_dn->d_elm);
		} break;
case 261:
# line 1060 "grammar.y"

		{
			Opflag = mdHELP;
			begin_quote();
			w_key(yypvt[-0].u_dn->d_elm);
		} break;
case 262:
# line 1067 "grammar.y"

		{
			Opflag = mdINDEX;
			begin_quote();
			w_key(yypvt[-1].u_dn->d_elm);
			w_key(yypvt[-0].u_dn->d_elm);
		} break;
case 263:
# line 1075 "grammar.y"

		{
			Opflag = mdRUBIN;
			w_new("IIrubin(");
		} break;
case 264:
# line 1081 "grammar.y"

		{
			if (Opflag == mdDEFINE)
				Opflag = mdINTEGRITY;
			w_key(yypvt[-0].u_dn->d_elm);
		} break;
case 265:
# line 1088 "grammar.y"

		{ 
			if (Opflag == mdCTLELM)
				Opflag = mdTUPRET;
			w_op("=");
		} break;
case 266:
# line 1094 "grammar.y"
 
			w_key(yypvt[-0].u_dn->d_elm); break;
case 267:
# line 1097 "grammar.y"

		{
			Opflag = mdMODIFY;
			begin_quote();
			w_key(yypvt[-0].u_dn->d_elm);
		} break;
case 268:
# line 1104 "grammar.y"

		{
			if (Opflag == mdDEFINE)
	/*                      Opflag = mdPROT;        */
				Opflag = mdINTEGRITY;
			w_key(yypvt[-0].u_dn->d_elm);
		} break;
case 269:
# line 1112 "grammar.y"
  
		{ 
			Opflag = mdPRINT;
			begin_quote();
			w_key(yypvt[-0].u_dn->d_elm);
		} break;
case 270:
# line 1119 "grammar.y"
 
		{
			Opflag = mdRANGE;
			begin_quote();
			w_key(yypvt[-1].u_dn->d_elm);
			w_key(yypvt[-0].u_dn->d_elm);
		} break;
case 271:
# line 1127 "grammar.y"

		{
			Opflag = mdREPLACE;
			begin_quote();
			w_key(yypvt[-0].u_dn->d_elm);
		} break;
case 272:
# line 1134 "grammar.y"

		{
			begin_quote();
			Opflag = mdREPLACE;
			w_key(yypvt[-0].u_dn->d_elm);
		} break;
case 273:
# line 1141 "grammar.y"

		{
			Opflag = mdRETRIEVE;
			begin_quote();
			w_key(yypvt[-0].u_dn->d_elm);
		} break;
case 274:
# line 1148 "grammar.y"

		{
			Opflag = mdRETRIEVE;
			begin_quote();
			w_key(yypvt[-0].u_dn->d_elm);
		} break;
case 275:
# line 1155 "grammar.y"

		{
			Opflag = mdSAVE;
			begin_quote();
			w_key(yypvt[-0].u_dn->d_elm);
		} break;
case 276:
# line 1162 "grammar.y"

		{
			Opflag = mdDECL;
			Struct_flag = 1;
			w_key(yypvt[-0].u_dn->d_elm);
		} break;
case 277:
# line 1169 "grammar.y"

		{
			begin_quote();
			w_key(yypvt[-0].u_dn->d_elm);
			Opflag = mdTUPRET;
		} break;
case 278:
# line 1176 "grammar.y"

		{
			if (Opflag == mdDEFINE)
				Opflag = mdVIEW;
			w_key(yypvt[-0].u_dn->d_elm);
		} break;
case 279:
# line 1188 "grammar.y"

			w_key(yypvt[-0].u_dn->d_elm); break;
case 285:
# line 1197 "grammar.y"

			w_key(yypvt[-0].u_dn->d_elm); break;
case 286:
# line 1200 "grammar.y"

			w_op(yypvt[-0].u_dn->d_elm); break;
case 287:
# line 1203 "grammar.y"
 
			w_op(yypvt[-0].u_dn->d_elm); break;
case 292:
# line 1211 "grammar.y"

			w_key(yypvt[-0].u_dn->d_elm); break;
case 297:
# line 1219 "grammar.y"
 
			w_op("="); break;
case 300:
# line 1225 "grammar.y"

			w_op(yypvt[-0].u_dn->d_elm); break;
case 301:
# line 1228 "grammar.y"
 
			w_op(yypvt[-0].u_dn->d_elm); break;
case 302:
# line 1231 "grammar.y"

			w_key(yypvt[-0].u_dn->d_elm); break;
case 303:
# line 1234 "grammar.y"

			w_key(yypvt[-0].u_dn->d_elm); break;
case 304:
# line 1237 "grammar.y"

			w_op(yypvt[-0].u_dn->d_elm); break;
case 309:
# line 1245 "grammar.y"

			w_op(yypvt[-0].u_dn->d_elm); break;
case 310:
# line 1248 "grammar.y"
 
			w_key(yypvt[-0].u_dn->d_elm); break;
case 314:
# line 1255 "grammar.y"
 
			w_key(yypvt[-0].u_dn->d_elm); break;
case 315:
# line 1258 "grammar.y"
 
			{
			w_key(yypvt[-0].u_dn->d_elm);
			Inqual = 1;
			} break;
	}
	goto yystack;		/* reset registers in driver code */
}
