#include "tdef.h"

extern
# include "d.h"

extern
# include "v.h"

extern
# include "tw.h"

/*
 * troff6.c
 *
 * width functions, sizes and fonts
 */

extern int
                ch0,
                ch,
                chbits,
                dfact,
                dfactd,
                eschar,
                font1,
                font,
                level,
                lss1,
                lss,
                nform,
                nlflg,
                nonumb,
                noscale,
                ohc,
                pts,
                res,
                setwdf,
                smnt,
                sps,
                vflag,
                widthp,
                xfont;

int     fontlab[] = {
	'R', 'I', 'B', 'S', 0
};

width (c)
int     c;
{
	register        i,
	                j,
	                k;

	j = c;
	k = 0;
	if (j & MOT) {
		if (j & VMOT)
			goto rtn;
		k = j & ~MOTV;
		if (j & NMOT)
			k = -k;
		goto rtn;
	}
	if ((i = (j & CMASK)) == 010) {
		k = -widthp;
		goto rtn;
	}
	if (i == PRESC)
		i = eschar;
	if ((i == ohc) ||
#ifndef ASA
			(i >= 0370))
#else
			(i >= 0500))
#endif
		goto rtn;
	if (j & ZBIT)
		goto rtn;
#ifndef ASA
	i = trtab[i] & BMASK;
#else
	i = trtab[i] & 0777;
#endif
	if (i < 040)
		goto rtn;
#ifdef PROPORTIONAL
	k = (*(t.codetab[i - 32]) & 0177);
#else
	k = (*(t.codetab[i - 32]) & 0177) * t.Char;
#endif PROPORTIONAL
	widthp = k;
rtn: 
	return (k);
}

setch () {
	register        i,
	               *j,
	                k;
	extern int      chtab[];

	if ((i = getrq ()) == 0)
		return (0);
	for (j = chtab; *j != i; j++)
		if (*(j++) == 0)
			return (0);
	k = *(++j) | chbits;
	return (k);
}

find (i, j)
int     i,
        j[];
{
	register        k;

	if (((k = i - '0') >= 1) && (k <= 4) && (k != smnt))
		return (--k);
	for (k = 0; j[k] != i; k++)
		if (j[k] == 0)
			return (-1);
	return (k);
}

mchbits () {
#ifndef ASA
	chbits = (((pts) << 2) | font) << (BYTE + 1);
#else
	chbits = font << (BYTE + 2);
#endif
	sps = width (' ' | chbits);
}

setps () {
	register        i,
	                j;

	if ((((i = getch () & CMASK) == '+') || (i == '-')) &&
			(((j = (ch = getch () & CMASK) - '0') >= 0) && (j <= 9))) {
		ch = 0;
		return;
	}
	if ((i -= '0') == 0) {
		return;
	}
	if ((i > 0) && (i <= 9)) {
		if ((i <= 3) &&
				((j = (ch = getch () & CMASK) - '0') >= 0) && (j <= 9)) {
			i = 10 * i + j;
			ch = 0;
		}
	}
}

caseft () {
	skip ();
	setfont (1);
}

setfont (a)
int     a;
{
	register        i,
	                j;

	if (a)
		i = getrq ();
	else
		i = getsn ();
	if (!i || (i == 'P')) {
		j = font1;
		goto s0;
	}
	if (i == 'S')
		return;
	if ((j = find (i, fontlab)) == -1)
		return;
s0: 
	font1 = font;
	font = j;
	mchbits ();
}

setwd () {
	register        i,
	                base,
	                wid;
	int     delim,
	        em,
	        k;
	int     savlevel,
	        savhp,
	        savfont,
	        savfont1;

	base = v.st = v.sb = wid = v.ct = 0;
	if ((delim = getch () & CMASK) & MOT)
		return;
	savhp = v.hp;
	savlevel = level;
	v.hp = level = 0;
	savfont = font;
	savfont1 = font1;
	setwdf++;
	while ((((i = getch ()) & CMASK) != delim) && !nlflg) {
		wid += width (i);
		if (!(i & MOT)) {
			em = 2 * t.Halfline;
		}
		else
			if (i & VMOT) {
				k = i & ~MOTV;
				if (i & NMOT)
					k = -k;
				base -= k;
				em = 0;
			}
			else
				continue;
		if (base < v.sb)
			v.sb = base;
		if ((k = base + em) > v.st)
			v.st = k;
	}
	nform = 0;
	setn1 (wid);
	v.hp = savhp;
	level = savlevel;
	font = savfont;
	font1 = savfont1;
	mchbits ();
	setwdf = 0;
}

vmot () {
	dfact = lss;
	vflag++;
	return (mot ());
}

hmot () {
	dfact = EM;
	return (mot ());
}

mot () {
	register        i,
	                j;

	j = HOR;
	getch ();		/* eat delim */
	if (i = atoi ()) {
		if (vflag)
			j = VERT;
		i = makem (quant (i, j));
	}
	getch ();
	vflag = 0;
	dfact = 1;
	return (i);
}

sethl (k)
int     k;
{
	register        i;

	i = t.Halfline;
	if (k == 'u')
		i = -i;
	else
		if (k == 'r')
			i = -2 * i;
	vflag++;
	i = makem (i);
	vflag = 0;
	return (i);
}

makem (i)
int     i;
{
	register        j;

	if ((j = i) < 0)
		j = -j;
	j = (j & ~MOTV) | MOT;
	if (i < 0)
		j |= NMOT;
	if (vflag)
		j |= VMOT;
	return (j);
}

casefp () {
	register        i,
	                j;

	skip ();
	if (((i = (getch () & CMASK) - '0' - 1) < 0) || (i > 3))
		return;
	if (skip () || !(j = getrq ()))
		return;
	fontlab[i] = j;
}

casevs () {
	register        i;

	skip ();
	vflag++;
	dfact = INCH;		/* default scaling is points! 
				*/
	dfactd = 72;
	res = VERT;
	i = inumb (&lss);
	if (nonumb)
		i = lss1;
	if (i < VERT)
		i = VERT;
	lss1 = lss;
	lss = i;
}

xlss () {
	register        i,
	                j;

	getch ();
	dfact = lss;
	i = quant (atoi (), VERT);
	dfact = 1;
	getch ();
	if ((j = i) < 0)
		j = -j;
	ch0 = ((j & 03700) << 3) | HX;
	if (i < 0)
		ch0 |= 040000;
	return (((j & 077) << 9) | LX);
}

casefz () {
}
caseps () {
}
caselg () {
}
casecs () {
}
casebd () {
}
casess () {
}

getlg (i)
int     i;
{
	return (i);
}
