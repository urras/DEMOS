#ifndef lint
/*NOXSTR*/
static char rcsid[] = "$Header: c20.c,v 2.11 88/06/10 20:47:59 dvolodin Exp $" ;
/*YESXSTR*/
#endif  lint
/*
 *       C object code improver
 *       Оптимизатор C-комилятора 3-АПР-84
 *       ИПК Минавтопрома, Антонов В.Г.,Леонтьев С.Е.
 *
 *      $Log:	c20.c,v $
 * Revision 2.11  88/06/10  20:47:59  dvolodin
 * 1) Добавлено еще места (на этот раз из стека).
 * 2) Исправлено зацикливание.
 * 3) Добавлены красивости для отладки.
 * 
 * Revision 2.6  87/04/05  17:01:49  dmitry
 * Пустые строчки теперь выкидываются.
 *
 * Revision 2.5  87/04/01  18:12:50  dmitry
 * Подправлена еще пара плюх.
 *
 * Revision 2.4  87/04/01  17:41:11  dmitry
 * Заделана обработка в ситуации типа
 *      jne     L0
 * L0:  ...
 *
 * Revision 2.3  87/02/28  16:01:21  avg
 * Правлена апшипка относительно 2.2.
 *
 * Revision 2.2  87/02/27  18:46:22  avg
 * Правлена ошибка (возникает при генерации для assign ... goto):
 *
 *      mov     $L47, r0
 *      jmp     (r0)
 *
 * L20: mov     r1, r0
 * L47: .....
 *
 * терялась метка L47.
 *
 * Revision 2.1  86/12/12  20:36:25  dmitry
 * Первая рабочая версия для Демос2
 *
 *      Последнее изменение: 28.11.85 @VG
 */

#include "c2.h"
#include <ediag.h>

char    revbr[] = { JNE, JEQ, JGT, JLT, JGE, JLE, JHIS, JLOS, JHI, JLO };
int     isn     = 20000;
int     lastseg = -1;

#define NSTK    5000

#ifdef SUPER
/***** ЗАТЫЧКИ by @VG *****/
char *ibuf;
char *obuf;
int   fd0 = 0;
int   fd1 = 1;
char *ip, *op;
int   _ilen;
#define EOF (-1)

getchar()
{
	register c;

    AGAIN:
	if( ip == ibuf )
		_ilen = read( fd0, ibuf, 512 );
	if( (ip-ibuf) < _ilen )
		return( *ip++ & 0377 );
	ip = ibuf;
	if( _ilen > 0 )
		goto AGAIN;
	return( EOF );
}

putchar( c )
{
	*op++ = c;
	if( (op-obuf) == 512 ) {
		write( fd1, obuf, 512 );
		op = obuf;
	}
}

puterr( c )
{
	int cc = c;

	write( 2, &cc, 1 );
}

exit( n )
{
	if( op > obuf )
		write( fd1, obuf, op-obuf );
	_exit(n);
}

fprintf( f, fmt, a )
{
	_doprnt( &puterr, fmt, &a );
}

printf( fmt, a )
{
	_doprnt( &putchar, fmt, &a );
}

_doprnt( fp, fmt, a )
int     (*fp)();
{
	register char *p, *p1;
	register *parg;
	int n, n1, base;
	char ia[9];

	parg = a;
	p = fmt;
	while( *p ) {
		if( *p == '%' ) {
			if( *++p == 's' ) {
				p1 = *parg++;
			    OUS:
				while( *p1 )
					(*fp)(*p1++);
			} else {
				base = (*p == 'o')?8 : 10;
				n1 = n = *parg++;
				if( n < 0 )
					n = -n;
				ia[9] = 0;
				p1 = &ia[9];
				while( n > 0 ) {
					*--p1 = (n%base) + '0';
					n = n/base;
				}
				if( n1 < 0 )
					*--p1 = '-';
				goto OUS;
			}
		} else
			(*fp)(*p);
		p++;
	}
}
#endif SUPER

main(argc, argv)
char **argv;
{
	register int niter, maxiter, isend;
	extern end;
	int nflag;
	char    stspace[NSTK];
#ifdef SUPER
	char Ibuf[512], Obuf[512];
#endif

	if (argc>1 && argv[1][0]=='+') {
		argc--;
		argv++;
		debug++;
	}
	nflag = 0;
	if (argc>1 && argv[1][0]=='-') {
		argc--;
		argv++;
		nflag++;
	}
	if (argc>1) {
#ifdef SUPER
		if ((fd0 = open(argv[1], 0)) < 0 ) {
#else
		if (freopen(argv[1], "r", stdin) == NULL) {
#endif
			fprintf(stderr, ediag("C2: can't find %s\n",
					      "c2: не найден файл %s\n"), argv[1]);
			fprintf(stderr, "C2: can't find %s\n", argv[1]);
			exit(1);
		}
	}
	if (argc>2) {
#ifdef SUPER
		if ((fd1 = creat(argv[2], 0666)) < 0 ) {
#else
		if (freopen(argv[2], "w", stdout) == NULL) {
#endif
			fprintf(stderr,ediag("C2: can't create %s\n",
					     "c2: нельзя создать файл %s\n"), argv[2]);
			exit(1);
		}
	}
#ifdef SUPER
	ip = ibuf = Ibuf;
	op = obuf = Obuf;
#endif
	lasta = firstr = lastr = sbrk(2);
	maxiter = 0;
	opsetup();
	do {
	alasta = stspace;
	alastr = &stspace[NSTK];
		isend = input();
		movedat();
		niter = 0;
		do {
			refcount();
			do {
				iterate();
				clearreg();
				niter++;
			} while (nchange);
			comjump();
			rmove();
		} while (nchange || jumpsw());
		addsob();
		output();
		if (niter > maxiter)
			maxiter = niter;
		lasta = firstr;
	} while (isend);
/* @VG : Все это хорошо, но требует слишком много памяти
	if (nflag) {
		if( _ediag ) {
			fprintf(stderr, "%d iterations\n", maxiter);
			fprintf(stderr, "%d jumps to jumps\n", nbrbr);
			fprintf(stderr, "%d inst. after jumps\n", iaftbr);
			fprintf(stderr, "%d jumps to .+2\n", njp1);
			fprintf(stderr, "%d redundant labels\n", nrlab);
			fprintf(stderr, "%d cross-jumps\n", nxjump);
			fprintf(stderr, "%d code motions\n", ncmot);
			fprintf(stderr, "%d branches reversed\n", nrevbr);
			fprintf(stderr, "%d redundant moves\n", redunm);
			fprintf(stderr, "%d simplified addresses\n", nsaddr);
			fprintf(stderr, "%d loops inverted\n", loopiv);
			fprintf(stderr, "%d redundant jumps\n", nredunj);
			fprintf(stderr, "%d common seqs before jmp's\n", ncomj);
			fprintf(stderr, "%d skips over jumps\n", nskip);
			fprintf(stderr, "%d sob's added\n", nsob);
			fprintf(stderr, "%d redundant tst's\n", nrtst);
			fprintf(stderr, "%d literals eliminated\n", nlit);
			fprintf(stderr, "%dK core\n", (((int)lastr+01777)>>10)&077);
		} else {
			fprintf(stderr, "%d итераций\n", maxiter);
			fprintf(stderr, "%d переходов на переходы\n", nbrbr);
			fprintf(stderr, "%d команд после переходов\n", iaftbr);
			fprintf(stderr, "%d переходов на .+2\n", njp1);
			fprintf(stderr, "%d лишних меток\n", nrlab);
			fprintf(stderr, "%d кросс-переходов\n", nxjump);
			fprintf(stderr, "%d кодов перемещено\n", ncmot);
			fprintf(stderr, "%d переходов обращено\n", nrevbr);
			fprintf(stderr, "%d лишних пересылок\n", redunm);
			fprintf(stderr, "%d упрощений адресации\n", nsaddr);
			fprintf(stderr, "%d циклов инвертировано\n", loopiv);
			fprintf(stderr, "%d лишних переходов\n", nredunj);
			fprintf(stderr, "%d общих послед. команд после jmp\n", ncomj);
			fprintf(stderr, "%d пропусков после переходов\n", nskip);
			fprintf(stderr, "%d команд sob добавлено\n", nsob);
			fprintf(stderr, "%d лишних команд tst\n", nrtst);
			fprintf(stderr, "%d литералов преобразовано\n", nlit);
			fprintf(stderr, "%dK памяти\n", (((int)lastr+01777)>>10)&077);
		}
	}
*/
	exit(0);
}

input()
{
	register struct node *p, *lastp;
	register int oper;

	lastp = &first;
	for (;;) {
		oper = getline();
		switch (oper&0377) {

		case LABEL:
			p = (struct node *)alloc(sizeof first);
			if (line[0] == 'L') {
				p->op = LABEL;
				p->subop = 0;
				p->labno = getnum(line+1);
				p->code = 0;
			} else {
				p->op = DLABEL;
				p->subop = 0;
				p->labno = 0;
				p->code = copy(1, line);
			}
			break;

		case JBR:
		case CBR:
		case JMP:
		case JSW:
			p = (struct node *)alloc(sizeof first);
			p->op = oper&0377;
			p->subop = oper>>8;
			if (*curlp=='L' && (p->labno = getnum(curlp+1)))
				p->code = 0;
			else {
				p->labno = 0;
				p->code = copy(1, curlp);
			}
			break;

		default:
			p = (struct node *)alloc(sizeof first);
			p->op = oper&0377;
			p->subop = oper>>8;
			p->labno = 0;
			p->code = copy(1, curlp);
			break;

		}
		p->forw = 0;
		p->back = lastp;
		lastp->forw = p;
		lastp = p;
		p->ref = 0;
		if (oper==EROU)
			return(1);
		if (oper==END)
			return(0);
	}
}

getline()
{
	register char *lp;
	register c;

	lp = line;
OneMoreTime:
	while ((c = getchar())==' ' || c=='\t')
		;
	do {
		if (c==':') {
			*lp++ = 0;
			return(LABEL);
		}
		if (c=='\n') {
			if( lp == line ) goto OneMoreTime ;
			*lp++ = 0;
			return(oplook());
		}
		if (lp >= &line[LSIZE-2]) {
			fprintf(stderr, ediag( "C2: Sorry, input line too long\n",
					       "c2: входная строка слишком длинная\n"));
			exit(1);
		}
/*
 * Средство для борьбы с лишними '+', которые генерит второй проход
 * Ричевского компилятора после того, как в нем покопался Д. Володин.
 */
		if( c != '+' || lp[-1] != '$'  && lp[-1] != ','
			     && lp[-1] != '\t' && lp[-1] != ' ' )
/*******************************************************************/
			*lp++ = c;
	} while ((c = getchar()) != EOF);
	*lp++ = 0;
	return(END);
}

getnum(ap)
char *ap;
{
	register char *p;
	register n, c;

	p = ap;
	n = 0;
	while ((c = *p++) >= '0' && c <= '9')
		n = n*10 + c - '0';
	if (*--p != 0)
		return(0);
	return(n);
}

output()
{
	register struct node *t;
	register struct optab *oper;
	register int byte;

	t = &first;
	while (t = t->forw) switch (t->op) {

	case END:
		return;

	case LABEL:
		printf("L%d:", t->labno);
		continue;

	case DLABEL:
		printf("%s:", t->code);
		continue;

	case TEXT:
	case DATA:
	case BSS:
		lastseg = t->op;

	default:
		if ((byte = t->subop) == BYTE)
			t->subop = 0;
		for (oper = optab; oper->opstring!=0; oper++)
			if ((oper->opcode&0377) == t->op
			 && (oper->opcode>>8) == t->subop) {
				printf("%s", oper->opstring);
				if (byte==BYTE)
					printf("b");
				break;
			}
		if (t->code) {
			reducelit(t);
			printf("\t%s\n", t->code);
		} else if (t->op==JMP || t->op==JBR || t->op==CBR)
			printf("\tL%d\n", t->labno);
		else
			printf("\n");
		continue;

	case JSW:
		printf("L%d\n", t->labno);
		continue;

	case SOB:
		printf("sob     %s", t->code);
		if (t->labno)
			printf(",L%d", t->labno);
		printf("\n");
		continue;

	case 0:
		if (t->code)
			printf("%s", t->code);
		printf("\n");
		continue;
	}
}

/*
 * Notice addresses of the form
 * $xx,xx(r)
 * and replace them with (pc),xx(r)
 *     -- Thanx and a tip of the Hatlo hat to Bliss-11.
 */
reducelit(at)
struct node *at;
{
	register char *c1, *c2;
	char *c2s;
	register struct node *t;

	t = at;
	if (*t->code != '$')
		return;
	c1 = t->code;
	while (*c1 != ',')
		if (*c1++ == '\0')
			return;
	c2s = c1;
	c1++;
	if (*c1=='*')
		c1++;
	c2 = t->code+1;
	while (*c1++ == *c2++);
	if (*--c1!='(' || *--c2!=',')
		return;
	t->code = copy(2, "(pc)", c2s);
	nlit++;
}

char *
copy(na, ap)
char *ap;
{
	register char *p, *np;
	char *onp;
	register n;

	p = ap;
	n = 0;
	if (*p==0)
		return(0);
	do
		n++;
	while (*p++);
	if (na>1) {
		p = (&ap)[1];
		while (*p++)
			n++;
	}
	onp = np = alloc(n);
	p = ap;
	while (*np++ = *p++)
		;
	if (na>1) {
		p = (&ap)[1];
		np--;
		while (*np++ = *p++);
	}
	return(onp);
}

opsetup()
{
	register struct optab *optp, **ophp;
	register char *p;

	for (optp = optab; p = optp->opstring; optp++) {
		ophp = &ophash[(((p[0]<<3)+(p[1]<<1)+p[2])&077777) % OPHS];
		while (*ophp++)
			if (ophp > &ophash[OPHS])
				ophp = ophash;
		*--ophp = optp;
	}
}

oplook()
{
	register struct optab *optp;
	register char *lp, *np;
	static char tmpop[32];
	struct optab **ophp;

	if (line[0]=='\0') {
		curlp = line;
		return(0);
	}
	np = tmpop;
	for (lp = line; *lp && *lp!=' ' && *lp!='\t';)
		*np++ = *lp++;
	*np++ = 0;
	while (*lp=='\t' || *lp==' ')
		lp++;
	curlp = lp;
	ophp = &ophash[(((tmpop[0]<<3)+(tmpop[1]<<1)+tmpop[2])&077777) % OPHS];
	while (optp = *ophp) {
		np = optp->opstring;
		lp = tmpop;
		while (*lp == *np++)
			if (*lp++ == 0)
				return(optp->opcode);
		if (*lp++=='b' && *lp++==0 && *--np==0)
			return(optp->opcode + (BYTE<<8));
		ophp++;
		if (ophp >= &ophash[OPHS])
			ophp = ophash;
	}
	if (line[0]=='L') {
		lp = &line[1];
		while (*lp)
			if (*lp<'0' || *lp++>'9')
				return(0);
		curlp = line;
		return(JSW);
	}
	curlp = line;
	return(0);
}

refcount()
{
	register struct node *p, *lp;
	static struct node *labhash[LABHS];
	register struct node **hp, *tp;
	int     Labno;

	for (hp = labhash; hp < &labhash[LABHS];)
		*hp++ = 0;
	for (p = first.forw; p!=0; p = p->forw)
		if (p->op==LABEL) {
			labhash[p->labno % LABHS] = p;
			p->refc = 0;
		}
	for (p = first.forw; p!=0; p = p->forw) {
		if (p->op==JMP || p->op==JBR || p->op==CBR || p->op==JSW) {
			p->ref = 0;
			Labno = p->labno;
			lp = labhash[Labno % LABHS];
			if (lp==0 || Labno!=lp->labno)
			for (lp = first.forw; lp!=0; lp = lp->forw) {
				if (lp->op==LABEL && Labno==lp->labno)
					break;
			}
			if (lp) {
				tp = nonlab(lp)->back;
				if (tp!=lp) {
					p->labno = tp->labno;
					lp = tp;
				}
				p->ref = lp;
				lp->refc++;
			}
		} else if( p->code != 0 && (Labno = srchlab(p->code)) != 0 ) {
			lp = labhash[Labno % LABHS];
			if (lp==0 || Labno!=lp->labno)
			for (lp = first.forw; lp!=0; lp = lp->forw) {
				if (lp->op==LABEL && Labno==lp->labno)
					break;
			}
			if (lp)
				lp->refc++;
		}
	}
	for (p = first.forw; p!=0; p = p->forw)
		if (p->op==LABEL && p->refc==0
		 && (lp = nonlab(p))->op && lp->op!=JSW)
			decref(p);
}

srchlab(s)
register char *s;
{
	register unsigned n;

	while( *s != '$' && *s != '\0' ) s++;
	if( *s != '$' )
		return(0);
	if( *++s != 'L' )
		return(0);
	n = 0;
	while( '0' <= *++s && *s <= '9' )
		n = (n*10) + (*s-'0');
	return(n);
}

iterate()
{
	register struct node *p, *rp, *p1;

	nchange = 0;
	for (p = first.forw; p!=0; p = p->forw) {
		CHECK(0);
		if ((p->op==JBR||p->op==CBR||p->op==JSW||p->op==JMP) && p->ref) {
			rp = nonlab(p->ref);
			if (rp->op==JBR && rp->labno && p->labno!=rp->labno) {
				nbrbr++;
				p->labno = rp->labno;
				decref(p->ref);
				rp->ref->refc++;
				p->ref = rp->ref;
				CHECK(1);
				nchange++;
			}
		}
		if (p->op==CBR && (p1 = p->forw)->op==JBR) {
			rp = p->ref;
			do
				rp = rp->back;
			while (rp->op==LABEL);
			if (rp==p1) {
				decref(p->ref);
				p->ref = p1->ref;
				p->labno = p1->labno;
				p1->forw->back = p;
				p->forw = p1->forw;
				p->subop = revbr[p->subop];
				nchange++;
				CHECK(2);
				nskip++;
			}
		}
		if (p->op==JBR || p->op==JMP) {
			while (p->forw && p->forw->op!=LABEL
				&& p->forw->op!=DLABEL
				&& p->forw->op!=EROU && p->forw->op!=END
				&& p->forw->op!=0 && p->forw->op!=DATA) {
				nchange++;
				iaftbr++;
				if (p->forw->ref)
					decref(p->forw->ref);
				p->forw = p->forw->forw;
				p->forw->back = p;
				CHECK(3);
			}
		}
		if( p->op == JBR || p->op == CBR || p->op == JMP ) {
			rp = p->forw;
			while (rp && rp->op==LABEL) {
				if (p->ref == rp) {
					p->back->forw = p->forw;
					p->forw->back = p->back;
					p = p->back;
					decref(rp);
					nchange++;
					CHECK(4);
					njp1++;
					break;
				}
				rp = rp->forw;
			}
		}
		if (p->op==JBR || p->op==JMP) {
			xjump(p);
			p = codemove(p);
		}
	}
}

xjump(p1)
register struct node *p1;
{
	register struct node *p2, *p3;

	if ((p2 = p1->ref)==0)
		return;
	for (;;) {
		while ((p1 = p1->back) && p1->op==LABEL);
		while ((p2 = p2->back) && p2->op==LABEL);
		if (!equop(p1, p2) || p1==p2)
			return;
		p3 = insertl(p2);
		p1->op = JBR;
		p1->subop = 0;
		p1->ref = p3;
		p1->labno = p3->labno;
		p1->code = 0;
		nxjump++;
		CHECK(5);
		nchange++;
	}
}

struct node *
insertl(oldp)
register struct node *oldp;
{
	register struct node *lp;

	if (oldp->op == LABEL) {
		oldp->refc++;
		return(oldp);
	}
	if (oldp->back->op == LABEL) {
		oldp = oldp->back;
		oldp->refc++;
		return(oldp);
	}
	lp = (struct node *)alloc(sizeof first);
	lp->op = LABEL;
	lp->subop = 0;
	lp->labno = isn++;
	lp->ref = 0;
	lp->code = 0;
	lp->refc = 1;
	lp->back = oldp->back;
	lp->forw = oldp;
	oldp->back->forw = lp;
	oldp->back = lp;
	CHECK(6);
	return(lp);
}

struct node *
codemove(p)
struct node *p;
{
	register struct node *p1, *p2, *p3;
	struct node *t, *tl;
	int n;

	p1 = p;
	if (p1->op!=JBR || (p2 = p1->ref)==0)
		return(p1);
	while (p2->op == LABEL)
		if ((p2 = p2->back) == 0)
			return(p1);
	if (p2->op!=JBR && p2->op!=JMP)
		goto ivloop;
	if (p1==p2)
		return(p1);
	p2 = p2->forw;
	p3 = p1->ref;
	while (p3) {
		if (p3->op==JBR || p3->op==JMP) {
			if (p1==p3 || p1->forw==p3 || p1->back==p3)
				return(p1);
			ncmot++;
			nchange++;
			CHECK(70);
			p1->back->forw = p2;
			p1->forw->back = p3;
			p2->back->forw = p3->forw;
			p3->forw->back = p2->back;
			p2->back = p1->back;
			p3->forw = p1->forw;
			decref(p1->ref);
			CHECK(7);
			return(p2);
		} else
			p3 = p3->forw;
	}
	return(p1);
ivloop:
	if (p1->forw->op!=LABEL)
		return(p1);
	p3 = p2 = p2->forw;
	n = 16;
	do {
		if ((p3 = p3->forw) == 0 || p3==p1 || --n==0)
			return(p1);
	} while (p3->op!=CBR || p3->labno!=p1->forw->labno);
	do
		if ((p1 = p1->back) == 0)
			return(p);
	while (p1!=p3);
	p1 = p;
	tl = insertl(p1);
	p3->subop = revbr[p3->subop];
	decref(p3->ref);
	p2->back->forw = p1;
	p3->forw->back = p1;
	p1->back->forw = p2;
	p1->forw->back = p3;
	t = p1->back;
	p1->back = p2->back;
	p2->back = t;
	t = p1->forw;
	p1->forw = p3->forw;
	p3->forw = t;
	p2 = insertl(p1->forw);
	p3->labno = p2->labno;
	p3->ref = p2;
	decref(tl);
	if (tl->refc<=0)
		nrlab--;
	loopiv++;
	nchange++;
	CHECK(8);
	return(p3);
}

comjump()
{
	register struct node *p1, *p2, *p3;

	for (p1 = first.forw; p1!=0; p1 = p1->forw)
		if (p1->op==JBR && (p2 = p1->ref) && p2->refc > 1)
			for (p3 = p1->forw; p3!=0; p3 = p3->forw)
				if (p3->op==JBR && p3->ref == p2)
					backjmp(p1, p3);
}

backjmp(ap1, ap2)
struct node *ap1, *ap2;
{
	register struct node *p1, *p2, *p3;

	p1 = ap1;
	p2 = ap2;
	for(;;) {
		while ((p1 = p1->back) && p1->op==LABEL);
		p2 = p2->back;
		if (equop(p1, p2)) {
			p3 = insertl(p1);
			p2->back->forw = p2->forw;
			p2->forw->back = p2->back;
			p2 = p2->forw;
			decref(p2->ref);
			p2->labno = p3->labno;
			p2->ref = p3;
			nchange++;
			ncomj++;
			CHECK(9);
		} else
			return;
	}
}
