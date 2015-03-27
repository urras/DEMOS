#include "tdef.h"

/*
 * troff8.c
 *
 * hyphenation
 */

#ifdef SOAN
#ifndef ASA
#define russ(c) (((c)&0377) >= 0270 && ((c)&0377) <= 0367)
#else
#define russ(c) (((c)&0777) >= 0300 && ((c)&0377) <= 0376)
#endif
#endif

#define THRESH  160		/* digram goodness threshold */


char    hbuf[NHEX],
       *nexth = hbuf;

extern int
               *wdstart,
               *wdend,
               *hyptr[],
              **hyp,
                hyoff,
                noscale;

int     thresh = THRESH,
       *hyend;

hyphen (wp)
int    *wp;
{
	register       *i,
	                j;

	i = wp;
	while (punct (*i++));
	if (!alph (*--i))
		return;
	wdstart = i++;
	while (alph (*i++));
	hyend = wdend = --i - 1;
	while (punct (*i++));
	if (*--i)
		return;
	if ((wdend - wdstart - 4) < 0)
		return;
	hyp = hyptr;
	*hyp = 0;
	hyoff = 2;
#ifndef SOAN
	if (!exword () && !suffix ())
		digram ();
#else
	if (!russhyp ())
		if (!exword () && !suffix ())
			digram ();
#endif  SOAN
	*hyp++ = 0;
	if (*hyptr)
		for (j = 1; j;) {
			j = 0;
			for (hyp = hyptr + 1; *hyp != 0; hyp++) {
				if (*(hyp - 1) > *hyp) {
					j++;
					i = *hyp;
					*hyp = *(hyp - 1);
					*(hyp - 1) = i;
				}
			}
		}
}

punct (i)
int     i;
{
	return (i != 0 && !alph (i));
}

alph (i)
int     i;
{
	register        j;

	j = i & CMASK;
#ifndef SOAN
	if (((j >= 'A') && (j <= 'Z')) || ((j >= 'a') && (j <= 'z')))
#else
	if ((j >= 'A' && j <= 'Z') || (j >= 'a' && j <= 'z') || russ (j))
#endif SOAN
			return (1);
	else
		return (0);
}

caseht () {
	thresh = THRESH;
	if (skip ())
		return;
	noscale++;
	thresh = atoi ();
	noscale = 0;
}

casehw () {
	register        i,
	                k;
	register char  *j;

	k = 0;
	while (!skip ()) {
		if ((j = nexth) >= (hbuf + NHEX - 2))
			goto full;
		for (;;) {
			if ((i = getch ()) & MOT)
				continue;
			if (((i &= CMASK) == ' ') || (i == '\n')) {
				*j++ = 0;
				nexth = j;
				*j = 0;
				if (i == ' ')
					break;
				else
					return;
			}
			if (i == '-') {
				k = 0200;
				continue;
			}
			*j++ = maplow (i) | k;
			k = 0;
			if (j >= (hbuf + NHEX - 2))
				goto full;
		}
	}
	return;
full: 
	prstr ("Exception word list full.\n");
	*nexth = 0;
}

exword () {
	register int   *w;
	register char  *e;
	char   *save;

	e = hbuf;
	while (1) {
		save = e;
		if (*e == 0)
			return (0);
		w = wdstart;
		while ((*e && (w <= hyend)) &&
				((*e & 0177) == maplow (*w & CMASK))) {
			e++;
			w++;
		};
		if (!*e) {
			if (((w - 1) == hyend) ||
					((w == wdend) && (maplow (*w & CMASK) == 's'))) {
				w = wdstart;
				for (e = save; *e; e++) {
					if (*e & 0200)
						*hyp++ = w;
					if (hyp > (hyptr + NHYP - 1))
						hyp = hyptr + NHYP - 1;
					w++;
				}
				return (1);
			}
			else {
				e++;
				continue;
			}
		}
		else
			while (*e++);
	}
}

suffix () {
	register int   *w;
	register char  *s,
	               *s0;
	int     i;
	extern char    *suftab[];
	extern int     *chkvow ();

again: 
#ifndef SOAN
	if (!alph (i = *hyend & CMASK))
#else
	if (!alph (i = *hyend & CMASK) || russ (i))
#endif SOAN
			return (0);
	if (i < 'a')
		i -= 'A' - 'a';
	if ((s0 = suftab[i - 'a']) == 0)
		return (0);
	for (;;) {
		if ((i = *s0 & 017) == 0)
			return (0);
		s = s0 + i - 1;
		w = hyend - 1;
		while (((s > s0) && (w >= wdstart)) &&
				((*s & 0177) == maplow (*w))) {
			s--;
			w--;
		}
		if (s == s0)
			break;
		s0 += i;
	}
	s = s0 + i - 1;
	w = hyend;
	if (*s0 & 0200)
		goto mark;
	while (s > s0) {
		w--;
		if (*s-- & 0200) {
	mark: 
			hyend = w - 1;
			if (*s0 & 0100)
				continue;
			if (!chkvow (w))
				return (0);
			*hyp++ = w;
		}
	}
	if (*s0 & 040)
		return (0);
	if (exword ())
		return (1);
	goto again;
}

maplow (i)
int     i;
{
	if ((i &= CMASK) < 'a')
		i += 'a' - 'A';
	return (i);
}

vowel (i)
int     i;
{
	switch (maplow (i)) {
		case 'a': 
		case 'e': 
		case 'i': 
		case 'o': 
		case 'u': 
		case 'y': 
			return (1);
		default: 
			return (0);
	}
}

int    *chkvow (w)
int    *w;
{
	while (--w >= wdstart)
		if (vowel (*w & CMASK))
			return (w);
	return (0);
}

digram () {
	register       *w,
	                val;
	int    *nhyend,
	       *maxw,
	        maxval;
	extern char     bxh[26][13],
	                bxxh[26][13],
	                xxh[26][13],
	                xhx[26][13],
	                hxx[26][13];

again: 
	if (!(w = chkvow (hyend + 1)))
		return;
	hyend = w;
	if (!(w = chkvow (hyend)))
		return;
	nhyend = w;
	maxval = 0;
	w--;
	while ((++w < hyend) && (w < (wdend - 1))) {
		val = 1;
		if (w == wdstart)
			val *= dilook ('a', *w, bxh);
		else
			if (w == wdstart + 1)
				val *= dilook (*(w - 1), *w, bxxh);
			else
				val *= dilook (*(w - 1), *w, xxh);
		val *= dilook (*w, *(w + 1), xhx);
		val *= dilook (*(w + 1), *(w + 2), hxx);
		if (val > maxval) {
			maxval = val;
			maxw = w + 1;
		}
	}
	hyend = nhyend;
	if (maxval > thresh)
		*hyp++ = maxw;
	goto again;
}

dilook (a, b, t)
int     a,
        b;
char    t[26][13];
{
	register        i,
	                j;

	i = t[maplow (a) - 'a'][(j = maplow (b) - 'a') / 2];
	if (!(j & 01))
		i >>= 4;
	return (i & 017);
}

#ifdef SOAN
russhyp () {
	register int   *w,
	               *h;
	register int    washyp = 0;
	int    *wfirstg = (int *) 0;
	for (w = wdstart; w <= wdend; w++)
		if (!russ (*w))
			return (0);
		else
			if (!wfirstg && russvow (*w))
				wfirstg = w;
	w = wdend;

	while (w > wdstart) {
		if (w - wdstart < 2)
			return (washyp);
		if (!russvow (*w--))
			continue;
		if (russvow (*w)) {
			h = w + 1;/*   Г|Г  */
			hyend = w;
		/* МНОС */
		}
		else
			if (w > wfirstg && !ismznak (*w)) {
			/* Чтобы не было ...|ь.. */
				h = w;/*   ...|СГ  */
				hyend = w - 1;
			}
	/* МНОС */
			else
				continue;/* Ь */
		if (wdend - h < 2)
			continue;
		*hyp++ = h;	/*  Место  разрезания  слова  
				*/
		washyp = 1;
	}
	return (washyp);
}

#ifdef MNOS
ismznak (c) int c;
{
	c &= 0377;
	return (c == 0334 || c == 0274 || c == 0332 || c == 0272 ? 1 : 0);
}

russvow (c)
int     c;
{
	switch (c & 0377) {
		case 0300: 
		case 0340: 	/*  А   */
		case 0305: 
		case 0345: 	/*  Е   */
		case 0310: 
		case 0350: 	/*  И   */
		case 0316: 
		case 0356: 	/*  О   */
		case 0323: 
		case 0363: 	/*  У   */
		case 0333: 
		case 0273: 	/*  Ы   */
		case 0335: 
		case 0275: 	/*  Э   */
		case 0336: 
		case 0276: 	/*  Ю   */
		case 0337: 
		case 0277: 
			return (1);/*  Я   */

		default: 
			return (0);
	}
}
#endif  MNOS

#ifdef DEMOS
ismznak (c) int c;
{
#ifndef ASA
	c &= 0377;
	return (c == 0330 || c == 0270 || c == 0337 ? 1 : 0);
#else
	c &= 0777;
	return (c == 0330 || c == 0370 || c == 0337 ? 1 : 0);
#endif  ASA
}

russvow (c)
int     c;
{
#ifndef ASA
	c &= 0377;
#else
	c &= 0777;
#endif  ASA
	switch (c) {
		case 0301: 
		case 0341: 	/*  А   */
		case 0305: 
		case 0345: 	/*  Е   */
		case 0311: 
		case 0351: 	/*  И   */
		case 0317: 
		case 0357: 	/*  О   */
		case 0325: 
		case 0365: 	/*  У   */
#ifndef ASA
		case 0271: 
#else
		case 0371:
#endif
		case 0331: 	/*  Ы   */
#ifndef ASA
		case 0274: 
#else
		case 0374:
#endif
		case 0334: 	/*  Э   */
		case 0300: 
		case 0340: 	/*  Ю   */
		case 0321: 
		case 0361: 
			return (1);/*  Я   */

		default: 
			return (0);
	}
}
#endif
#endif SOAN
