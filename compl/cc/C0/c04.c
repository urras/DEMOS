#ifndef lint
/*NOXSTR*/
static char rcsid[] = "$Header: c04.c,v 2.2 86/11/26 17:28:35 dmitry Exp $" ;
/*YESXSTR*/
#endif  lint

/*
 * C compiler
 *
 *      $Log:	c04.c,v $
 *	Revision 2.2  86/11/26  17:28:35  dmitry
 *	Исправлена ошибка -
 *	Headerы не должны обрабатываться xstrом.
 *	
 *      Revision 2.1  86/11/26  16:38:17  dmitry
 *      Оформлено под Manifest avg.
 *
 *      Последнее изменение:31.03.85 @VG
 */

#include "c0.h"
#include <varargs.h>

/*
 * Reduce the degree-of-reference by one.
 * e.g. turn "ptr-to-int" into "int".
 */
decref(at)
{
	register t;

	t = at;
	if ((t & ~TYPE) == 0) {
		error(ediag("Illegal indirection",
			    "неправильное использование указателя"));
		return(t);
	}
	return((t>>TYLEN) & ~TYPE | t&TYPE);
}

/*
 * Increase the degree of reference by
 * one; e.g. turn "int" to "ptr-to-int".
 */
incref(t)
{
	return(((t&~TYPE)<<TYLEN) | (t&TYPE) | PTR);
}

/*
 * Make a tree that causes a branch to lbl
 * if the tree's value is non-zero together with the cond.
 */
cbranch(t, lbl, cond)
struct tnode *t;
{
	treeout(t, 0);
	if(lineexpr < 0 ) lineexpr = line;
	outcode("BNNN", CBRANCH, lbl, cond, lineexpr);
	lineexpr = -1;
}

/*
 * Write out a tree.
 */
rcexpr(atp)
struct tnode *atp;
{
	register struct tnode *tp;

	/*
	 * Special optimization
	 */
	if ((tp=atp)->op==INIT && tp->tr1->op==CON) {
		if( tp->type == CHAR ){
			outcode("B1N0", BDATA, tp->tr1->value);
			return;
		}else if( tp->type == INT || tp->type == UNSIGN ){
			outcode("BN", SINIT, tp->tr1->value);
			return;
		}
	}
	treeout(tp, 0);
	if(lineexpr < 0 ) lineexpr = line;
	outcode("BN", EXPR, lineexpr);
	lineexpr = -1;
}

treeout(atp, isstruct)
struct tnode *atp;
{
	register struct tnode *tp;
	register struct hshtab *hp;
	register nextisstruct;

	if ((tp = atp) == 0) {
		outcode("B", NULLOP);
		return;
	}
	nextisstruct = tp->type==STRUCT;
	switch(tp->op) {

	case NAME:
		hp = tp->tr1;
		if (hp->hclass==TYPEDEF)
			error(ediag("Illegal use of type name",
				    "неправильное использование имени типа"));
		outcode("BNN", NAME, hp->hclass==0?STATIC:hp->hclass, tp->type);
		if (hp->hclass==EXTERN)
			outcode("S", hp->name);
		else
			outcode("N", hp->hoffset);
		break;

	case LCON:
		outcode("BNNN", tp->op, tp->type, (short)(tp->lvalue>>16), (short)tp->lvalue);
		break;

	case CON:
		outcode("BNN", tp->op, tp->type, tp->value);
		break;

	case FCON:
		outcode("BNF", tp->op, tp->type, tp->cstr);
		break;

	case STRING:
		outcode("BNNN", NAME, STATIC, tp->type, tp->tr1);
		break;

	case FSEL:
		treeout(tp->tr1, nextisstruct);
		outcode("BNNN",tp->op,tp->type,tp->tr2->bitoffs,tp->tr2->flen);
		break;

	case ETYPE:
		error(ediag("Illegal use of type",
			    "неправильное использование типа"));
		break;

	case AMPER:
		treeout(tp->tr1, 1);
		outcode("BN", tp->op, tp->type);
		break;


	case CALL:
		treeout(tp->tr1, 1);
		treeout(tp->tr2, 0);
		outcode("BN", CALL, tp->type);
		break;

	default:
		treeout(tp->tr1, nextisstruct);
		if (opdope[tp->op]&BINARY)
			treeout(tp->tr2, nextisstruct);
		outcode("BN", tp->op, tp->type);
		break;
	}
	if (nextisstruct && isstruct==0)
		outcode("BNN", STRASG, STRUCT, tp->strp->ssize);
}

/*
 * Generate a branch
 */
branch(lab)
{
	outcode("BN", BRANCH, lab);
}

/*
 * Generate a label
 */
label(l)
{
	outcode("BN", LABEL, l);
}

/*
 * ap is a tree node whose type
 * is some kind of pointer; return the size of the object
 * to which the pointer points.
 */
plength(ap)
struct tname *ap;
{
	register t, l;
	register struct tnode *p;

	p = ap;
	if (p==0 || ((t=p->type)&~TYPE) == 0)           /* not a reference */
		return(1);
	p->type = decref(t);
	l = length(p);
	p->type = t;
	return(l);
}

/*
 * return the number of bytes in the object
 * whose tree node is acs.
 */
length(acs)
struct tnode *acs;
{
	register t, elsz;
	long n;
	register struct tnode *cs;
	int nd;

	cs = acs;
	t = cs->type;
	n = 1;
	nd = 0;
	while ((t&XTYPE) == ARRAY) {
		t = decref(t);
		n *= cs->subsp[nd++];
	}
	if ((t&~TYPE)==FUNC)
		return(0);
	if (t>=PTR)
		elsz = SZPTR;
	else switch(t&TYPE) {

	case INT:
	case UNSIGN:
		elsz = SZINT;
		break;

	case CHAR:
		elsz = 1;
		break;

	case FLOAT:
		elsz = SZFLOAT;
		break;

	case LONG:
		elsz = SZLONG;
		break;

	case DOUBLE:
		elsz = SZDOUB;
		break;

	case STRUCT:
		if ((elsz = cs->strp->ssize) == 0)
			error(ediag("Undefined structure",
				    "неопределенная структура"));
		break;
	default:
		error(ediag( "Compiler error (length)",
			     "внутренняя ошибка компилятора (length)"));
		return(0);
	}
	n *= elsz;
	if (n >= (unsigned)50000) {
		error(ediag( "Warning: very large data structure",
			     "предупреждение: слишком большой массив данных"));
		nerror--;
	}
	return(n);
}

/*
 * The number of bytes in an object, rounded up to a word.
 */
rlength(cs)
struct tnode *cs;
{
	return((length(cs)+ALIGN) & ~ALIGN);
}

/*
 * After an "if (...) goto", look to see if the transfer
 * is to a simple label.
 */
simplegoto()
{
	register struct hshtab *csp;

	if ((peeksym=symbol())==NAME && nextchar()==';') {
		csp = csym;
		if (csp->hblklev == 0)
			pushdecl(csp);
		if (csp->hclass==0 && csp->htype==0) {
			csp->htype = ARRAY;
			csp->hflag |= FLABL;
			if (csp->hoffset==0)
				csp->hoffset = isn++;
		}
		if ((csp->hclass==0||csp->hclass==STATIC)
		 &&  csp->htype==ARRAY) {
			peeksym = -1;
			return(csp->hoffset);
		}
	}
	return(0);
}

/*
 * Return the next non-white-space character
 */
nextchar()
{
	while (spnextchar()==' ')
		peekc = 0;
	return(peekc);
}

/*
 * Return the next character, translating all white space
 * to blank and handling line-ends.
 */
spnextchar()
{
	register c;

	if ((c = peekc)==0)
		c = getchar();
	if (c=='\t' || c=='\014')       /* FF */
		c = ' ';
	else if (c=='\n') {
		c = ' ';
		if (inhdr==0)
			line++;
		inhdr = 0;
	} else if (c=='\001') { /* SOH, insert marker */
		inhdr++;
		c = ' ';
	}
	peekc = c;
	return(c);
}

/*
 * is a break or continue legal?
 */
chconbrk(l)
{
	if (l==0)
		error(ediag( "Break/continue error",
			     "ошибка в break/continue"));
}

/*
 * The goto statement.
 */
dogoto()
{
	register struct tnode *np;

	*cp++ = tree();
	build(STAR);
	chkw(np = *--cp, -1);
	rcexpr(block(JUMP,0,NULL,NULL,np));
}

/*
 * The return statement, which has to convert
 * the returned object to the function's type.
 */
doret()
{
	register struct tnode *t;

	if (nextchar() != ';') {
		t = tree();
		*cp++ = &funcblk;
		*cp++ = t;
		build(ASSIGN);
		cp[-1] = cp[-1]->tr2;
		if( funcblk.type == CHAR )
			cp[-1] = block(ITOC, INT, NULL, NULL, cp[-1]);
		build(RFORCE);
		rcexpr(*--cp);
	}
	branch(retlab);
}

/*
 * Write a character on the error output.
 */
/*
 * Coded output:
 *   B: beginning of line; an operator
 *   N: a number
 *   S: a symbol (external)
 *   1: number 1
 *   0: number 0
 */
outcode(s, va_alist)
register char *s;
va_dcl
{
	va_list ap;        /** register *ap; **/
	register FILE *bufp;
	register n;
	register char *np;

	bufp = stdout;
	if (strflg)
		bufp = sbufp;
	va_start(ap);        /** ap = &a; **/
	for (;;) switch(*s++) {
	case 'B':
                putc(va_arg(ap, int), bufp);
		putc(0376, bufp);
		continue;

	case 'N':
		n = va_arg(ap, int);
                putc(n, bufp);
                putc(n>>8, bufp);
		continue;

	case 'F':
		n = 1000;
                np = va_arg(ap, char *);
		goto str;

	case 'S':
		n = NCPS;
                np = va_arg(ap, char *);
		if (*np)
			putc('_', bufp);
	str:
		while (n-- && *np) {
			putc(*np++&CMASK, bufp);
		}
		putc(0, bufp);
		continue;

	case '1':
		putc(1, bufp);
		putc(0, bufp);
		continue;

	case '0':
		putc(0, bufp);
		putc(0, bufp);
		continue;

	case '\0':
		if (ferror(bufp)) {
			error(ediag( "Write error on temp",
				     "ошибка записи во временный файл"));
			exit(1);
		}
		return;

	default:
		error(ediag( "Botch in outcode",
			     "внутренняя ошибка компилятора в outcode"));
	}
	va_end(ap);
}
