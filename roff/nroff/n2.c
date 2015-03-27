#include "tdef.h"
#include <sgtty.h>

extern
#include "d.h"

extern
#include "v.h"

extern
#include "tw.h"

#include "s.h"

#include <setjmp.h>
        jmp_buf sjbuf;

/*
 * troff2.c
 *
 * output, cleanup
 */

extern struct s
                       *frame,
                       *stk,
                       *nxf;

extern  filep
        ip,
        woff,
        offset;


extern char
               *enda,
                obuf[OBUFSZ],
               *obufp,
                ptname[],
               *unlkp,
		nextf[];

extern struct sgttyb    ttys;

extern int
               *pendw,
                app,
                ascii,
                dilev,
                donef,
                ds,
                ejf,
                em,
                eschar,
                gflag,
                lead,
                level,
                lgf,
                mflg,
                ndone,
                nflush,
                nfo,
                no_out,
                nofeed,
                paper,
                pendnf,
                pipeflg,
                print,
                ptid,
                quiet,
                ralss,
                stopmesg,
                tflg,
                tlss,
                trap,
                ttysave,
                waitf,
                xxx;

int     toolate,
        error;

pchar (c)
int     c;
{
	register        i,
	                j;

	if ((i = c) & MOT) {
		pchar1 (i);
		return;
	}
	switch (j = i & CMASK) {
		case 0: 
		case IMP: 
		case RIGHT: 
		case LEFT: 
			return;
		case HX: 
			j = (tlss >> 9) | ((i & ~0777) >> 3);
			if (i & 040000) {
				j &= ~(040000 >> 3);
				if (j > dip -> blss)
					dip -> blss = j;
			}
			else {
				if (j > dip -> alss)
					dip -> alss = j;
				ralss = dip -> alss;
			}
			tlss = 0;
			return;
		case LX: 
			tlss = i;
			return;
		case PRESC: 
			if (dip == &d[0])
				j = eschar;
		default: 
#ifndef ASA
			i = (trtab[j] & BMASK) | (i & ~CMASK);
#else
			i = (trtab[j] & 0777) | (i & ~CMASK);
#endif  ASA
	}
	pchar1 (i);
}

pchar1 (c)
int     c;
{
	register        i,
	                j,
	               *k;
	extern int      chtab[];

	j = (i = c) & CMASK;
	if (dip != &d[0]) {
		wbf (i);
		dip -> op = offset;
		return;
	}
	if (!tflg && !print) {
		if (j == '\n')
			dip -> alss = dip -> blss = 0;
		return;
	}
	if (no_out || (j == FILLER))
		return;
	ptout (i);
}

oput (i)
char    i;
{
	*obufp++ = i;
	if (obufp == (obuf + OBUFSZ + ascii - 1))
		flusho ();
}

oputs (i)
char   *i;
{
	while (*i != 0)
		oput (*i++);
}

flusho () {
	if (!ascii)
		*obufp++ = 0;
	if (!ptid) {
		while ((ptid = open (ptname, 1)) < 0) {
			if (++waitf <= 2)
				prstr ("Waiting for Typesetter.\n");
			sleep (15);
		}
	}
	if (no_out == 0) {
		if (!toolate) {
			toolate++;
			if (t.bset || t.breset) {
				if (ttysave == -1) {
					gtty (1, &ttys);
					ttysave = ttys.sg_flags;
				}
				ttys.sg_flags &= ~t.breset;
				ttys.sg_flags |= t.bset;
				stty (1, &ttys);
			}
			{
				char   *p = t.twinit;
				while (*p++);
				write (ptid, t.twinit, p - t.twinit - 1);
			}
		}
		toolate += write (ptid, obuf, obufp - obuf);
	}
	obufp = obuf;
}

done (x)
int     x;
{
	register        i;

	error |= x;
	level = 0;
	app = ds = lgf = 0;
	if (i = em) {
		donef = -1;
		em = 0;
		if (control (i, 0))
			longjmp (sjbuf, 1);
	}
	if (!nfo)
		done3 (0);
	mflg = 0;
	dip = &d[0];
	if (woff)
		wbt (0);
	if (pendw)
		getword (1);
	pendnf = 0;
	if (donef == 1)
		done1 (0);
	donef = 1;
	ip = 0;
	frame = stk;
	nxf = frame + 1;
	if (!ejf)
		tbreak ();
	nflush++;
	eject ((struct s       *) 0);
	longjmp (sjbuf, 1);
}

done1 (x)
int     x;
{
	error |= x;
	if (v.nl) {
		trap = 0;
		eject ((struct s       *) 0);
		longjmp (sjbuf, 1);
	}
	if (nofeed) {
		ptlead ();
		flusho ();
		done3 (0);
	}
	else {
		if (!gflag)
			lead += TRAILER;
		done2 (0);
	}
}

done2 (x)
int     x;
{
	register        i;

	ptlead ();
	flusho ();
	done3 (x);
}

done3 (x)
int     x;
{
	error |= x;
	signal (SIGINT, SIG_IGN);
	signal (SIGTERM, SIG_IGN);
	unlink (unlkp);
	twdone ();
	if (quiet) {
		ttys.sg_flags |= ECHO;
		stty (0, &ttys);
	}
	if (ascii)
		mesg (1);
	exit (error);
}

edone (x)
int     x;
{
	frame = stk;
	nxf = frame + 1;
	ip = 0;
	done (x);
}


casepi () {
	register        i;
	int     id[2];

	if (toolate || skip () || !getname () || (pipe (id) == -1) ||
			((i = fork ()) == -1)) {
		prstr ("Pipe not created.\n");
		return;
	}
	ptid = id[1];
	if (i > 0) {
		close (id[0]);
		toolate++;
		pipeflg++;
		return;
	}
	close (0);
	dup (id[0]);
	close (id[1]);
	execl (nextf, nextf, 0);
	prstr ("Cannot exec: ");
	prstr (nextf);
	prstr ("\n");
	exit (-4);
}
