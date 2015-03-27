# include	<func.h>
# include	<sccs.h>
/*
SCCSID(@(#)decompx.c     7.1     2/5/81)
 */



char Qbuf[1400];        /* 1000 было */
int QbufSize = sizeof Qbuf;

extern struct fn_def	DeOvqpFn;

struct fn_def	*FuncVect[] =
{
	&DeOvqpFn,
};

int	NumFunc = sizeof FuncVect / sizeof FuncVect[0];
