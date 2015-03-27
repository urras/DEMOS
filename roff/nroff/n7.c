#include "tdef.h"

extern
# include "d.h"

extern
# include "v.h"

extern
# include "tw.h"

#include "s.h"

#define GETCH gettch


/*
 * troff7.c
 *
 * text
 */

extern struct s
                       *frame,
                       *stk,
                       *ejl;

extern int
              **hyp,
               *hyptr[NHYP],
               *linep,
               *pendw,
               *pnp,
               *vlist,
               *wdend,
               *wdstart,
               *wordp,
                ad,
                admod,
                adrem,
                adsp,
                ascii,
                ce,
                ch,
                chbits,
                cu,
                cwidth,
                donef,
                dpn,
                ejf,
                fi,
                flss,
                font,
                hyf,
                hyoff,
                ic,
                icf,
                ics,
                in,
                it,
                itmac,
                lastl,
                line[],
                ll,
                lnmod,
                lnsize,
                ls,
                lss,
                mlist[NTRAP],
                nb,
                nc,
                ndf,
                ndone,
                ne,
                nel,
                nflush,
                nform,
                nhyp,
                ni,
                nlflg,
                nlist[NTRAP],
                nmbits,
                nms,
                nn,
                npn,
                npnflg,
                nrbits,
                nwd,
                ohc,
                over,
                pendnf,
                pendt,
                pfrom,
                pl,
                po,
                print,
                pto,
                sfont,
                spflg,
                spread,
                sps,
                stop,
                totout,
                trap,
                ul,
                ulbit,
                un1,
                un,
                vflag,
                wch,
                widthp,
                wne,
                word[],
                xbitf,
                xxx;

int     brflg;

tbreak () {
	register       *i,
	                j,
	                pad;
	int     res;

	trap = 0;
	if (nb)
		return;
	if ((dip == d) && (v.nl == -1)) {
		newline (1);
		return;
	}
	if (!nc) {
		setnel ();
		if (!wch)
			return;
		if (pendw)
			getword (1);
		movword ();
	}
	else
		if (pendw && !brflg) {
			getword (1);
			movword ();
		}
	*linep = dip -> nls = 0;
	if (dip == d)
		horiz (po);
	if (lnmod)
		donum ();
	lastl = ne;
	if (brflg != 1) {
		totout = 0;
	}
	else
		if (ad) {
			if ((lastl = (ll - un)) < ne)
				lastl = ne;
		}
	if (admod && ad && (brflg != 2)) {
		lastl = ne;
		adsp = adrem = 0;
		if (admod == 1)
			un += quant (nel / 2, t.Adj);
		else
			if (admod == 2)
				un += nel;
	}
	totout++;
	brflg = 0;
	if ((lastl + un) > dip -> maxl)
		dip -> maxl = (lastl + un);
	horiz (un);
	if (adrem % t.Adj)
		res = t.Hor;
	else
		res = t.Adj;
	for (i = line; nc > 0;) {
		if (((j = *i++) & CMASK) == ' ') {
			pad = 0;
			do {
				pad += width (j);
				nc--;
			} while (((j = *i++) & CMASK) == ' ');
			i--;
			pad += adsp;
			--nwd;
			if (adrem) {
				if (adrem < 0) {
					pad -= res;
					adrem += res;
				}
				else
					if ((totout & 01) ||
							((adrem / res) >= (nwd))) {
						pad += res;
						adrem -= res;
					}
			}
			horiz (pad);
		}
		else {
			pchar (j);
			nc--;
		}
	}
	if (ic) {
		if ((j = ll - un - lastl + ics) > 0)
			horiz (j);
		pchar (ic);
	}
	if (icf)
		icf++;
	else
		ic = 0;
	ne = nwd = 0;
	un = in;
	setnel ();
	newline (0);
	if (dip != d) {
		if (dip -> dnl > dip -> hnl)
			dip -> hnl = dip -> dnl;
	}
	else {
		if (v.nl > dip -> hnl)
			dip -> hnl = v.nl;
	}
	for (j = ls - 1; (j > 0) && !trap; j--)
		newline (0);
	spread = 0;
}

donum () {
	register        i,
	                nw;
	extern  pchar ();

	nrbits = nmbits;
	nw = width ('1' | nrbits);
	if (nn) {
		nn--;
		goto d1;
	}
	if (v.ln % ndf) {
		v.ln++;
d1: 
		un += nw * (3 + nms + ni);
		return;
	}
	i = 0;
	if (v.ln < 100)
		i++;
	if (v.ln < 10)
		i++;
	horiz (nw * (ni + i));
	nform = 0;
	fnumb (v.ln, pchar);
	un += nw * nms;
	v.ln++;
}

text () {
	register        i;
	static int      spcnt;

	nflush++;
	if ((dip == d) && (v.nl == -1)) {
		newline (1);
		return;
	}
	setnel ();
	if (ce || !fi) {
		nofill ();
		return;
	}
	if (pendw)
		goto t4;
	if (pendt)
		if (spcnt)
			goto t2;
		else
			goto t3;
	pendt++;
	if (spcnt)
		goto t2;
	while (((i = GETCH ()) & CMASK) == ' ')
		spcnt++;
	if (nlflg) {
t1: 
		nflush = pendt = ch = spcnt = 0;
		callsp ();
		return;
	}
	ch = i;
	if (spcnt) {
t2: 
		tbreak ();
		if (nc || wch)
			goto rtn;
		un += spcnt * sps;
		spcnt = 0;
		setnel ();
		if (trap)
			goto rtn;
		if (nlflg)
			goto t1;
	}
t3: 
	if (spread)
		goto t5;
	if (pendw || !wch)
t4: 
		if (getword (0))
			goto t6;
	if (!movword ())
		goto t3;
t5: 
	if (nlflg)
		pendt = 0;
	adsp = adrem = 0;
	if (ad) {
		if (nwd == 1)
			adsp = nel;
		else
			adsp = nel / (nwd - 1);
		adsp = (adsp / t.Adj) * t.Adj;
		adrem = nel - adsp * (nwd - 1);
	}
	brflg = 1;
	tbreak ();
	spread = 0;
	if (!trap)
		goto t3;
	if (!nlflg)
		goto rtn;
t6: 
	pendt = 0;
	ckul ();
rtn: 
	nflush = 0;
}

nofill () {
	register        i,
	                j;

	if (!pendnf) {
		over = 0;
		tbreak ();
		if (trap)
			goto rtn;
		if (nlflg) {
			ch = nflush = 0;
			callsp ();
			return;
		}
		adsp = adrem = 0;
		nwd = 10000;
	}
	while ((j = ((i = GETCH ()) & CMASK)) != '\n') {
		if (j == ohc)
			continue;
		if (j == CONT) {
			pendnf++;
			nflush = 0;
			flushi ();
			ckul ();
			return;
		}
		storeline (i, -1);
	}
	if (ce) {
		ce--;
		if ((i = quant (nel / 2, HOR)) > 0)
			un += i;
	}
	if (!nc)
		storeline (FILLER, 0);
	brflg = 2;
	tbreak ();
	ckul ();
rtn: 
	pendnf = nflush = 0;
}

callsp () {
	register        i;

	if (flss)
		i = flss;
	else
		i = lss;
	flss = 0;
	casesp (i);
}

ckul () {
	if (ul && (--ul == 0)) {
		cu = 0;
		font = sfont;
		mchbits ();
	}
	if (it && (--it == 0) && itmac)
		control (itmac, 0);
}

storeline (c, w) {
	register        i;

	if ((c & CMASK) == JREG) {
#ifndef ASA
		if ((i = findr (c >> BYTE)) != -1)
			vlist[i] = ne;
#else
		if ((i = findr (c >> 9)) != -1)
			vlist[i] = ne;
#endif
		return;
	}
	if (linep >= (line + lnsize - 1)) {
		if (!over) {
			prstrfl ("Line overflow.\n");
			over++;
#ifndef ASA
			c = 0343;
#else
			c = 0443;
#endif  ASA
			w = -1;
			goto s1;
		}
		return;
	}
s1: 
	if (w == -1)
		w = width (c);
	ne += w;
	nel -= w;
/*
 *      if( cu && !(c & MOT) && (trtab[(c & CMASK)] == ' '))
 *              c = ((c & ~ulbit) & ~CMASK) | '_';
 */
	*linep++ = c;
	nc++;
}

newline (a)
int     a;
{
	register        i,
	                j,
	                nlss;
	int     opn;

	if (a)
		goto nl1;
	if (dip != d) {
		j = lss;
		pchar1 (FLSS);
		if (flss)
			lss = flss;
		i = lss + dip -> blss;
		dip -> dnl += i;
		pchar1 (i);
		pchar1 ('\n');
		lss = j;
		dip -> blss = flss = 0;
		if (dip -> alss) {
			pchar1 (FLSS);
			pchar1 (dip -> alss);
			pchar1 ('\n');
			dip -> dnl += dip -> alss;
			dip -> alss = 0;
		}
		if (dip -> ditrap && !dip -> ditf &&
				(dip -> dnl >= dip -> ditrap) && dip -> dimac)
			if (control (dip -> dimac, 0)) {
				trap++;
				dip -> ditf++;
			}
		return;
	}
	j = lss;
	if (flss)
		lss = flss;
	nlss = dip -> alss + dip -> blss + lss;
	v.nl += nlss;
	pchar1 ('\n');
	flss = 0;
	lss = j;
	if (v.nl < pl)
		goto nl2;
nl1: 
	ejf = dip -> hnl = v.nl = 0;
	ejl = frame;
#ifndef SOAN
	if (donef) {
#else
	if (donef == 1) {
#endif SOAN
		if ((!nc && !wch) || ndone)
			done1 (0);
		ndone++;
		donef = 0;
		if (frame == stk)
			nflush++;
	}
	opn = v.pn;
	v.pn++;
	if (npnflg) {
		v.pn = npn;
		npn = npnflg = 0;
	}
nlpn: 
	if (v.pn == pfrom) {
		print++;
		pfrom = -1;
	}
	else
		if (opn == pto) {
			print = 0;
			opn = -1;
			chkpn ();
			goto nlpn;
		}
	if (stop && print) {
		dpn++;
		if (dpn >= stop) {
			dpn = 0;
			dostop ();
		}
	}
nl2: 
	trap = 0;
	if (v.nl == 0) {
		if ((j = findn (0)) != NTRAP)
			trap = control (mlist[j], 0);
	}
	else
		if ((i = findt (v.nl - nlss)) <= nlss) {
			if ((j = findn1 (v.nl - nlss + i)) == NTRAP) {
				prstrfl ("Trap botch.\n");
				done2 (-5);
			}
			trap = control (mlist[j], 0);
		}
}

findn1 (a)
int     a;
{
	register        i,
	                j;

	for (i = 0; i < NTRAP; i++) {
		if (mlist[i]) {
			if ((j = nlist[i]) < 0)
				j += pl;
			if (j == a)
				break;
		}
	}
	return (i);
}

chkpn () {
	pto = *(pnp++);
	pfrom = pto & ~MOT;
	if (pto == -1) {
		flusho ();
		done1 (0);
	}
	if (pto & MOT) {
		pto &= ~MOT;
		print++;
		pfrom = 0;
	}
}

findt (a)
int     a;
{
	register        i,
	                j,
	                k;

	k = 32767;
	if (dip != d) {
		if (dip -> dimac && ((i = dip -> ditrap - a) > 0))
			k = i;
		return (k);
	}
	for (i = 0; i < NTRAP; i++) {
		if (mlist[i]) {
			if ((j = nlist[i]) < 0)
				j += pl;
			if ((j -= a) <= 0)
				continue;
			if (j < k)
				k = j;
		}
	}
	i = pl - a;
	if (k > i)
		k = i;
	return (k);
}

findt1 () {
	register        i;

	if (dip != d)
		i = dip -> dnl;
	else
		i = v.nl;
	return (findt (i));
}

eject (a)
struct s       *a;
{
	register        savlss;

	if (dip != d)
		return;
	ejf++;
	if (a)
		ejl = a;
	else
		ejl = frame;
	if (trap)
		return;
e1: 
	savlss = lss;
	lss = findt (v.nl);
	newline (0);
	lss = savlss;
	if (v.nl && !trap)
		goto e1;
}

movword () {
	register        i,
	                w,
	               *wp;
	int     savwch,
	        hys;

	over = 0;
	wp = wordp;
	if (!nwd) {
		while (((i = *wp++) & CMASK) == ' ') {
			wch--;
			wne -= width (i);
		}
		wp--;
	}
	if ((wne > nel) &&
			!hyoff && hyf &&
			(!nwd || (nel > 3 * sps)) &&
			(!(hyf & 02) || (findt1 () > lss))
		)
		hyphen (wp);
	savwch = wch;
	hyp = hyptr;
	nhyp = 0;
	while (*hyp && (*hyp <= wp))
		hyp++;
	while (wch) {
		if ((hyoff != 1) && (*hyp == wp)) {
			hyp++;
			if (!wdstart ||
					((wp > (wdstart + 1)) &&
						(wp < wdend) &&
						(!(hyf & 04) || (wp < (wdend - 1))) &&
						(!(hyf & 010) || (wp > (wdstart + 2)))
					)
				) {
				nhyp++;
				storeline (IMP, 0);
			}
		}
		i = *wp++;
		w = width (i);
		wne -= w;
		wch--;
		storeline (i, w);
	}
	if (nel >= 0) {
		nwd++;
		return (0);
	}
	xbitf = 1;
	hys = width (0200);	/* hyphen */
m1: 
	if (!nhyp) {
		if (!nwd)
			goto m3;
		if (wch == savwch)
			goto m4;
	}
	if (*--linep != IMP)
		goto m5;
	if (!(--nhyp))
		if (!nwd)
			goto m2;
	if (nel < hys) {
		nc--;
		goto m1;
	}
m2: 
	if (((i = *(linep - 1) & CMASK) != '-') &&
			(i != 0203)
		) {
		*linep = (*(linep - 1) & ~CMASK) | 0200;
		w = width (*linep);
		nel -= w;
		ne += w;
		linep++;
/*
	hsend();
*/
	}
m3: 
	nwd++;
m4: 
	wordp = wp;
	return (1);
m5: 
	nc--;
	w = width (*linep);
	ne -= w;
	nel += w;
	wne += w;
	wch++;
	wp--;
	goto m1;
}

horiz (i)
int     i;
{
	vflag = 0;
	if (i)
		pchar (makem (i));
}

setnel () {
	if (!nc) {
		linep = line;
		if (un1 >= 0) {
			un = un1;
			un1 = -1;
		}
		nel = ll - un;
		ne = adsp = adrem = 0;
	}
}

getword (x)
int     x;
{
	register        i,
	                j,
	                swp;
	int     noword;

	noword = 0;
	if (x)
		if (pendw) {
			*pendw = 0;
			goto rtn;
		}
	if (wordp = pendw)
		goto g1;
	hyp = hyptr;
	wordp = word;
	over = wne = wch = 0;
	hyoff = 0;
	while (1) {
		j = (i = GETCH ()) & CMASK;
		if (j == '\n') {
			wne = wch = 0;
			noword = 1;
			goto rtn;
		}
		if (j == ohc) {
			hyoff = 1;
			continue;
		}
		if (j == ' ') {
			storeword (i, cwidth);
			continue;
		}
		break;
	}
	swp = widthp;
	storeword (' ' | chbits, -1);
	if (spflg) {
		storeword (' ' | chbits, -1);
		spflg = 0;
	}
	widthp = swp;
g0: 
	if (j == CONT) {
		pendw = wordp;
		nflush = 0;
		flushi ();
		return (1);
	}
	if (hyoff != 1) {
		if (j == ohc) {
			hyoff = 2;
			*hyp++ = wordp;
			if (hyp > (hyptr + NHYP - 1))
				hyp = hyptr + NHYP - 1;
			goto g1;
		}
		if ((j == '-') ||
				(j == 0203)/* 3/4 Em dash */
			)
			if (wordp > word + 1) {
				hyoff = 2;
				*hyp++ = wordp + 1;
				if (hyp > (hyptr + NHYP - 1))
					hyp = hyptr + NHYP - 1;
			}
	}
	storeword (i, cwidth);
g1: 
	j = (i = GETCH ()) & CMASK;
	if (j != ' ') {
		if (j != '\n')
			goto g0;
		j = *(wordp - 1) & CMASK;
		if ((j == '.') ||
				(j == '!') ||
				(j == '?'))
			spflg++;
	}
	*wordp = 0;
rtn: 
	wdstart = 0;
	wordp = word;
	pendw = 0;
	*hyp++ = 0;
	setnel ();
	return (noword);
}

storeword (c, w)
int     c,
        w;
{

	if (wordp >= &word[WDSIZE - 1]) {
		if (!over) {
			prstrfl ("Word overflow.\n");
			over++;
#ifndef ASA
			c = 0343;
#else
			c = 0443;
#endif
			w = -1;
			goto s1;
		}
		return;
	}
s1: 
	if (w == -1)
		w = width (c);
	wne += w;
	*wordp++ = c;
	wch++;
}

gettch () {
	register int    i,
	                j;

	if (!((i = getch ()) & MOT) && (i & ulbit)) {
		j = i & CMASK;
		if (cu && (trtab[j] == ' '))
			i = ((i & ~ulbit) & ~CMASK) | '_';
#ifndef ASA
		if (!cu && (j > 32) && (j < 0370) && !(*t.codetab[j - 32] & 0200))
#else
		if (!cu && (j > 32) && (j < 0477) && !(*t.codetab[j - 32] & 0200))
#endif  ASA
			i &= ~ulbit;
	}
	return (i);
}
