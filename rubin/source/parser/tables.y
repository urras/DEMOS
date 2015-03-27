/*
**	SCANNER KEYWORD TABLES
**
**	Keywords, tokens, and opcode tuples are included in this file
**	The keyword table MUST be in sorted order.
**	The operator table does not need to be sorted
**
**	Version:
**		@(#)tables.y	7.1	2/5/81
*/
struct optab	Keyword[] =
{
	"abs",			FOP,			opABS,
	"all",			ALL,			0,
	"and",			LBOP,			AND,
	"any",			AGOP, 			opANY,
	"append",		APPEND,			GOVAL,
	"ascii",		FOP,			opASCII,
	"at",			AT,			0,
	"atan",			FOP,			opATAN,
	"avg",			AGOP,			opAVG,
	"avgu",			AGOP,			opAVGU,
	"by",			BY,			opEQ,
	"concat", 		FBOP,			opCONCAT,
	"copy",			COPY,			GOVAL,
	"cos",			FOP,			opCOS,
	"count",		AGOP,			opCOUNT,
	"countu",		AGOP,			opCOUNTU,
	"create",		CREATE,			GOVAL,
	"date4",                FOP,                    opDATE,
	"define",		DEFINE,			GOVAL,
	"delete",		DELETE,			GOVAL,
	"destroy",		DESTROY,		GOVAL,
#	ifdef	DISTRIB
	"distribute",		DISTRIBUTE,		0,
	"distributed",		DISTRD,			0,
#	endif
	"exp",			FOP,			opEXP,
	"float4",		FOP,			opFLOAT4,
	"float8",		FOP,			opFLOAT8,
	"from",			FROM,			0,
	"from_date",            FBOP,                   opDACHAR,
	"from_time",            FBOP,                   opTICHAR,
# 	ifdef xV6_UNIX
	"gamma",		FOP,			opGAMMA,
# 	endif
	"help",			HELP,			GOVAL,
	"in",			IN,			0,
	"index",		INDEX,			GOVAL,
	"int1",			FOP,			opINT1,
	"int2",			FOP,			opINT2,
	"int4",			FOP,			opINT4,
	"integrity",		INTEGRITY,		0,
	"into",			INTO,			0,
	"is",			IS,			opEQ,
	"log",			FOP,			opLOG,
	"max",			AGOP,			opMAX,
	"min",			AGOP,			opMIN,
	"mod",			FBOP,			opMOD,
	"modify",		MODIFY,			GOVAL,
	"not",			LUOP,			opNOT,
	"of",			OF,			0,
	"on",			ON,			0,
	"onto",			ONTO,			0,
	"or",			LBOP,			OR,
	"permit",		PERMIT,			0,
	"print",		PRINT,			GOVAL,
	"range",		RANGE,			GOVAL,
	"replace",		REPLACE,		GOVAL,
	"retrieve",		RETRIEVE,		GOVAL,
	"save",			SAVE,			GOVAL,
	"ser_last",             FOP,                    opLAST,
	"ser_next",             FOP,                    opNEXT,
	"ser_set",              FBOP,                   opINIT,
	"sin",			FOP,			opSIN,
	"sqrt",			FOP,			opSQRT,
	"sum",			AGOP,			opSUM,
	"sumu",			AGOP,			opSUMU,
	"time4",                FOP,                    opTIME,
	"to",			TO,			0,
	"to_date",              FBOP,                   opTODA,
	"to_time",              FBOP,                   opTOTI,
	"unique",		UNIQUE,			0,
	"until",		UNTIL,			0,
	"view",			VIEW,			0,
	"where",		WHERE,			0,
};
int	Keyent =		sizeof Keyword / sizeof Keyword[0];

struct optab	Optab[] =
{
	/* PUNCTUATION */
	",",			COMMA,			0,
	"(",			LPAREN,			0,
	".",			PERIOD,			0,
	")",			RPAREN,			0,
	"\"",			SCONST,			0,
	"'",                    DATECONST,              0,
	"`",                    TIMECONST,              0,
	"/*",			BGNCMNT,		0,
	"*/",			ENDCMNT,		0,
	":",			COLON,			0,

	/* UNARY ARITHMETIC OPERATORS */
	"+",			UAOP,			opADD,
	"-",			UAOP,			opSUB,

	/* BINARY ARITHMETIC OPERATORS */
	"*",			BAOP,			opMUL,
	"/",			BAOP,			opDIV,
	"**",			BAOPH,			opPOW,

	/* BOUNDS OPERATORS */
	">",			BDOP,			opGT,
	">=",			BDOP,			opGE,
	"<",			BDOP,			opLT,
	"<=",			BDOP,			opLE,

	/* EQUALITY OPERATORS */
	"!=",			EOP,			opNE,
	"=",			IS,			opEQ,
	0
};

/* constant operators table for lookup */
struct constop	Coptab[] =
{
	"dba",		opDBA,		CHAR,		2,
	"usercode",	opUSERCODE,	CHAR,		2,
	"sysdate",      opSYSDATE,      DATE,           4,
	"systime",      opSYSTIME,      TIME,           4,
	0,		0,		0,		0
};


/*
** Tokens
** a structure initialized here to contain the
** yacc generated tokens for the indicated
** terminal symbols.
*/
struct special	Tokens =
{
	SCONST,
	BGNCMNT,
	ENDCMNT,
	I2CONST,
	I4CONST,
	F4CONST,
	F8CONST,
	NAME,
	DATECONST,
	TIMECONST
};
