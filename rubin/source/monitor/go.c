# include	"monitor.h"
# include	<defines.h>
# include	<aux.h>
# include	<resp.h>
# include	<symbol.h>
# include	<pv.h>
# include	<pipes.h>
# include	<setjmp.h>
# include	<sccs.h>

SCCSID(@(#)go.c	7.1	2/5/81)



/*
**  PROCESS QUERY
**
**	The appropriate messages are printed, and the query is scanned.
**	Tokens are passed to the parser.  A parser response is then
**	expected.
**
**	Trace Flags:
**		5
*/

# define	QRYTRAP		"{querytrap}"

jmp_buf		GoJmpBuf;

go(msg)
int msg;
{
	FILE		*iop;
	int ci; auto char c;
	register char	*p;
	extern int	fgetc();
	pb_t		pb;
	extern char	*macro();
	if (msg)
	{
		clrline(1);
		fflush(Qryiop);
	}
	if ((iop = fopen(Qbname, "r")) == NULL)
		syserr("go: open 1");
	if (Nodayfile >= 0)
		printf("Executing . . .\n\n");

#	ifdef xMTM
	if (tTf(76, 1))
		timtrace(3, 0);
#	endif

	if (!Nautoclear)
		Autoclear = 1;

	/* arrange to call the parser */
	initp();
	call_setup(&pb, mdPARSER, NULL);
	pb_prime(&pb, PB_REG);
	pb.pb_proc = 1;		/**** PARSER MUST BE IN PROC ONE ****/
	send_off(&pb, 0, NULL);
	pb_tput(PV_EOF, "", 0, &pb);
	macinit(fgetc, iop, 1);
	while ((ci = macgetch()) > 0)
		{
		if ((ci&0177) < 040 && ci != '\n' && ci != '\t')
		{
			printf("Funny character 0%o converted to blank\n", ci);
			ci = ' ';
		}
		  c = ci;               /* !!! - 1 байт */
		  pb_put(&c, 1, &pb);
		}
	pb_flush(&pb);
	fclose(iop);

	/* wait for the response */
	setjmp(GoJmpBuf);
	readinput(&pb);

	if (Resp.resp_tups >= 0)
		macdefine("{tuplecount}", locv(Resp.resp_tups), TRUE);
	
	if (Error_id == 0 && (p = macro(QRYTRAP)) != NULL)
		trapquery(&Resp, p);
	
	resetp();

	mcall("{continuetrap}");

#	ifdef xMTM
	if (tTf(76, 1))
		timtrace(4, 0);
#	endif
	if (msg)
		prompt("\ncontinue");
}
