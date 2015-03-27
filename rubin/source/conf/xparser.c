# include	<defines.h>
# include	<aux.h>
# include	<access.h>
# include	<func.h>
# include	<sccs.h>

SCCSID(@(#)parser.c	7.1	2/5/81)



char    Qbuf[3000];     /* 2000 было */
int	QbufSize = sizeof Qbuf;
short	tTdbu[100];

extern struct fn_def	ParserFn;
extern struct fn_def	HelpFn;
extern struct fn_def	PrintFn;

DESC	Attdes;
DESC	Reldes;
DESC	Inddes;


struct desxx	Desxx[] =
{
	"relation",	&Reldes,	&Admin.adreld,
	"attribute",	&Attdes,	&Admin.adattd,
	"indexes",	&Inddes,	NULL,
	NULL
};

struct fn_def	*FuncVect[] =
{
	&ParserFn,
	&HelpFn,
	&PrintFn
};

int	NumFunc = sizeof FuncVect / sizeof FuncVect[0];
