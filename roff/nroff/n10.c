#include "tdef.h"
#include <sgtty.h>

extern
#include "d.h"

extern
#include "v.h"

extern
#include "tw.h"

/*
 * nroff10.c
 *
 * Device interfaces
 */

extern char
                obuf[],
               *obufp,
                termtab[];

extern struct sgttyb    ttys;

extern int
               *olinep,
                ascii,
                eqflg,
                esc,
                esct,
                hflg,
                ics,
                lead,
                lss,
                oline[],
                pipeflg,
                ptid,
                sps,
                tabtab[],
                ttysave,
                ulfont,
                waitf,
                xfont;

int     dtab,
        bdmode,
        plotmode;
int     itmode;

#include <a.out.h>

ptinit () {
	register        i,
	                j;
	register char **p;
	char   *q;
	struct exec     x;
	extern char    *setbrk ();

	if ((i = open (termtab, 0)) < 0) {
		prstr ("Cannot open ");
		prstr (termtab);
		prstr ("\n");
		exit (-1);
	}
	read (i, (char *) & x, sizeof (struct exec));
	read (i, (char *) & t.bset, j = sizeof (int) * ((int *) & t.zzz - &t.bset));
	x.a_data -= j;
	q = setbrk (x.a_data);
	lseek (i, (N_TXTOFF (x) + (long) t.twinit), 0);
	i = read (i, q, x.a_data);
	j = q - t.twinit;
	for (p = &t.twinit; p < &t.zzz; p++) {
		if (*p)
			*p += j;
		else
			*p = "";
	}
	sps = EM;
	ics = EM * 2;
	dtab = 8 * t.Em;
	for (i = 0; i < 16; i++)
		tabtab[i] = dtab * (i + 1);
	if (eqflg)
		t.Adj = t.Hor;
}

twdone () {
	if (bdmode)
		oputs (t.bdoff);
	if (itmode)
		oputs (t.itoff);
	obufp = obuf;
	oputs (t.twrest);
	flusho ();
	if (pipeflg) {
		close (ptid);
		wait (&waitf);
	}
	if (ttysave != -1) {
		ttys.sg_flags = ttysave;
		stty (1, &ttys);
	}
}

ptout (i)
int     i;
{
	*olinep++ = i;
	if (olinep >= &oline[LNSIZE])
		olinep--;
	if ((i & CMASK) != '\n')
		return;
	olinep--;
	lead += dip -> blss + lss - t.Newline;
	dip -> blss = 0;
	esct = esc = 0;
	if (olinep > oline) {
		move ();
		ptout1 ();
		if (bdmode && t.Bnomove) {
			oputs (t.bdoff);
			bdmode = 0;
		}
		else
			if (itmode && t.Inomove) {
				oputs (t.itoff);
				itmode = 0;
			}
		oputs (t.twnl);
	}
	else {
		lead += t.Newline;
		move ();
	}
	lead += dip -> alss;
	dip -> alss = 0;
	olinep = oline;
}

ptout1 () {
	register        i,
	                k;
	register char  *codep;
	extern char    *plot ();
	int    *q,
	        w,
	        j,
	        phyw;

	for (q = oline; q < olinep; q++) {
		if ((i = *q) & MOT) {
			j = i & ~MOTV;
			if (i & NMOT)
				j = -j;
			if (i & VMOT)
				lead += j;
			else
				esc += j;
			continue;
		}
		if ((k = (i & CMASK)) <= 040) {
			switch (k) {
				case ' ': /* space */
					esc += t.Char;
					break;
			}
			continue;
		}
		codep = t.codetab[k - 32];
		w = t.Char * (*codep++ & 0177);
		phyw = w;
		if (i & ZBIT)
			w = 0;
		if (*codep && (esc || lead))
			move ();
		esct += w;
#ifndef ASA
		if (i & 074000)
			xfont = (i >> 9) & 03;
#else
/*              if (i & 076000)         */
			xfont = (i >> 10) & 03;
#endif
		if (*t.bdon & 0377) {
			if (!bdmode && (xfont == 2)) {
				if (itmode) {
					oputs (t.itoff);
					itmode = 0;
				}
				oputs (t.bdon);
				bdmode++;
			}
			if (bdmode && (xfont != 2)) {
				oputs (t.bdoff);
				bdmode = 0;
			}
		}
		if (*t.iton & 0377) {
			if (!itmode && (xfont == 1)) {
				oputs (t.iton);
				itmode++;
			}
			if (itmode && (xfont != 1)) {
				oputs (t.itoff);
				itmode = 0;
			}
		}
		else
			if (xfont == ulfont) {
				for (k = w / t.Char; k > 0; k--)
					oput ('_');
				for (k = w / t.Char; k > 0; k--)
					oput ('\b');
			}
		while (*codep != 0) {
#ifndef SOAN
			if (*codep & 0200) {
				codep = plot (codep);
				oputs (t.plotoff);
				oput (' ');
			}
			else {
				if (plotmode)
					oputs (t.plotoff);
#endif SOAN
				*obufp++ = *codep++;
				if (obufp == (obuf + OBUFSZ + ascii - 1))
					flusho ();
#ifdef  ASA
				if (xfont == 2 && xfont != ulfont
					&& !(*t.bdon & 0377)) {
					oput ('\b');
					oput (*(codep-1));
				}
#endif  ASA
/*                      oput(*codep++);*/
#ifndef SOAN
			}
#endif
		}
		if (!w)
			for (k = phyw / t.Char; k > 0; k--)
				oput ('\b');
	}
}

char   *plot (x)
char   *x;
{
	register int    i;
	register char  *j,
	               *k;

	if (!plotmode)
		oputs (t.ploton);
	k = x;
	if ((*k & 0377) == 0200)
		k++;
	for (; *k; k++) {
		if (*k & 0200) {
			if (*k & 0100) {
				if (*k & 040)
					j = t.up;
				else
					j = t.down;
			}
			else {
				if (*k & 040)
					j = t.left;
				else
					j = t.right;
			}
			if (!(i = *k & 037))
				return (++k);
			while (i--)
				oputs (j);
		}
		else
			oput (*k);
	}
	return (k);
}

move () {
	register        k;
	register char  *i,
	               *j;
	char   *p,
	       *q;
	int     iesct,
	        dt;

	iesct = esct;
	if (esct += esc)
		i = "\0";
	else
		i = "\n\0";
	j = t.hlf;
	p = t.right;
	q = t.down;
	if (lead || esc) {
		if (bdmode && t.Bnomove) {
			oputs (t.bdoff);
			bdmode = 0;
		}
		else
			if (itmode && t.Inomove) {
				oputs (t.itoff);
				itmode = 0;
			}
	}
	if (lead) {
		if (lead < 0) {
			lead = -lead;
			i = t.flr;
		/*      if(!esct)i = t.flr; else i = "\0"; */
			j = t.hlr;
			q = t.up;
		}
		if (*i & 0377) {
			k = lead / t.Newline;
			lead = lead % t.Newline;
			while (k--)
				oputs (i);
		}
		if (*j & 0377) {
			k = lead / t.Halfline;
			lead = lead % t.Halfline;
			while (k--)
				oputs (j);
		}
		else {		/* no half-line forward, not at
				   line begining */
			k = lead / t.Newline;
			lead = lead % t.Newline;
			if (k > 0)
				esc = esct;
			i = "\n";
			while (k--)
				oputs (i);
		}
	}
	if (esc) {
		if (esc < 0) {
			esc = -esc;
			j = "\b";
			p = t.left;
		}
		else {
			j = " ";
			if (hflg)
				while ((dt = dtab - (iesct % dtab)) <= esc) {
					if (dt % t.Em)
						break;
					oput (TAB);
					esc -= dt;
					iesct += dt;
				}
		}
		k = esc / t.Em;
		esc = esc % t.Em;
		while (k--)
			oputs (j);
	}
	if ((*t.ploton & 0377) && (esc || lead)) {
		if (!plotmode)
			oputs (t.ploton);
		esc /= t.Hor;
		lead /= t.Vert;
		while (esc--)
			oputs (p);
		while (lead--)
			oputs (q);
		oputs (t.plotoff);
	}
	esc = lead = 0;
}

ptlead () {
	move ();
}

dostop () {
	char    junk;

	flusho ();
	read (2, &junk, 1);
}
