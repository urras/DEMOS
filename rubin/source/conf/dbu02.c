# include	<func.h>
# include	<sccs.h>
# define NULL (struct fn_def *)0

SCCSID(@(#)alldbu.c	7.1	2/5/81)



char    Qbuf[3000];     /*      1500 было */
int	QbufSize = sizeof Qbuf;

int	Noupdt;
short	tTdbu[100];

extern struct fn_def	CopyFn;
extern struct fn_def	CreateFn;
extern struct fn_def	DstroyFn;
extern struct fn_def	HelpFn;
extern struct fn_def	DsplayFn;
/* extern struct fn_def	KsortFn; */
extern struct fn_def	ModifyFn;
extern struct fn_def	PrintFn;
extern struct fn_def	ResetrFn;
extern struct fn_def	RmqmFn;
extern struct fn_def	RupdatFn;
extern struct fn_def	SaveFn;
extern struct fn_def	IndexFn;

struct fn_def	*FuncVect[] =
{
	/* 0 -- copy */         &CopyFn,
	/* 1 -- save */         &SaveFn,
	/* 2  -- display */     &DsplayFn
};

int	NumFunc = sizeof FuncVect / sizeof FuncVect[0];
