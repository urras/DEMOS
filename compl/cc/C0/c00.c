#ifndef lint
/*NOXSTR*/
static char rcsid[] = "$Header: c00.c,v 2.5 88/09/28 21:28:38 dvolodin Exp $" ;
/*YESXSTR*/
#endif  lint

/* C compiler
 *
 *
 *
 * Called from cc:
 *   c0 source temp1 temp2 [ profileflag ] [ ovlyflag ]
 * temp1 gets most of the intermediate code;
 * strings are put on temp2, which c1 reads after temp1.
 *
 *      $Log:	c00.c,v $
 *	Revision 2.5  88/09/28  21:28:38  dvolodin
 *	Добавлена конструкция #line
 *	
 *	Revision 2.4  87/03/04  12:44:08  alex
 *	Сделаны одинаковыми смешение автоматических переменных
 *	с оверлеем и без. См. новый csv.s без оверлея.
 *	
 *	Revision 2.3  86/12/26  21:21:56  alex
 *	Убрали "void".
 *	
 *	Revision 2.2  86/11/26  17:25:56  dmitry
 *	Исправлена ошибка -
 *	Headerы не должны обрабатываться xstrом.
 *	
 *      Revision 2.1  86/11/26  16:34:09  dmitry
 *      Оформлено под Manifest avg.
 *
 *      Последнее изменение: 06.05.86 by MV
 *          Декабрь 1985 / FLEXNAME (Паремский М.В.)
 *
 *          1 проход C-комилятора 3-АПР-84
 *          ИПК Минавтопрома, Антонов В.Г.,Леонтьев С.Е.
 */

#include "c0.h"

int     lineexpr = -1;
int     isn     =  1;
int     peeksym = -1;
int     line    = 1;
struct  tnode    funcblk = { NAME, 0, NULL, NULL, NULL, NULL };

struct kwtab {
	char    *kwname;
	int     kwval;
} kwtab[] = {
	"int",          INT,
	"char",         CHAR,
	"float",        FLOAT,
	"double",       DOUBLE,
	"struct",       STRUCT,
	"long",         LONG,
	"unsigned",     UNSIGN,
	"union",        UNION,
	"short",        INT,
	"auto",         AUTO,
	"extern",       EXTERN,
	"static",       STATIC,
	"register",     REG,
	"goto",         GOTO,
	"return",       RETURN,
	"if",           IF,
	"while",        WHILE,
	"else",         ELSE,
	"switch",       SWITCH,
	"case",         CASE,
	"break",        BREAK,
	"continue",     CONTIN,
	"do",           DO,
	"default",      DEFAULT,
	"for",          FOR,
	"sizeof",       SIZEOF,
	"typedef",      TYPEDEF,
	"enum",         ENUM,
/*      "void",         INT,            */
	0,              0,
};

main(argc, argv)
char *argv[];
{
	register char *sp;
	register i;
	register struct kwtab *ip;

	if(argc<4) {
		error(ediag("c0: Arg count",
			    "c0: плохое число аргументов"));
		exit(1);
	}
	if (freopen(argv[1], "r", stdin)==NULL) {
		error(ediag("Can't find %s",
			    "%s не найден"), argv[1]);
		exit(1);
	}
	close(creat(argv[2],0660));
	if (freopen(argv[2], "w", stdout)==NULL ||
	   (sbufp=fopen(argv[3],"w"))==NULL) {
		error(ediag("Can't create temp",
			    "нельзя создать временные файлы"));
		exit(1);
	}
	setbuf(sbufp, sbuf);
	STAUTO = -8;    /* Для совместимости - см. новый csv.s */
	while (argc>4) {
		switch (argv[4][1]) {
		case 'A':
			ciflg++;
			break;
		case 'P':
			proflg++;
			break;
		case 'Y':       strucflg = 1;   break;
		case 'y':       strucflg = -1;  break;
		case 'V':
			/*
			 * Overlays: allow an extra word on the stack for
			 * each stack frame to store the overlay number.
			 * Внимание - теперь всегда равно -8
			 */
			STAUTO = -8;
			break;
		}
		argc--; argv++;
	}
	/*
	 * The hash table locations of the keywords
	 * are marked; if an identifier hashes to one of
	 * these locations, it is looked up in in the keyword
	 * table first.
	 */
	for (ip=kwtab; (sp = ip->kwname); ip++) {
		i = 0;
		while (*sp)
			i += *sp++;
		hshtab[i%HSHSIZ].hflag = FKEYW;
	}

	symbuf = malloc(NCPS);
#ifdef sparc
	funcbase = curbase = malloc( 0x10000 );
	coremax = curbase + 0x10000;
#else
	funcbase = curbase = coremax = sbrk(0);
#endif

	while(!eof)
		extdef();
	outcode("B", EOFC);
	strflg++;
	outcode("B", EOFC);
	exit(nerror!=0);
}

/*
 * Return is a ptr to the symbol table entry.
 */

struct hshtab *
hash(sp, length)
register length;
register char *sp;
{
	register ihash;

	ihash = 0;
	while (length--) {
		ihash += *sp++&0377;
	}
	return(&hshtab[ihash%HSHSIZ]);
}

/*
 * Look up the identifier in symbuf in the symbol table.
 * If it hashes to the same spot as a keyword, try the keyword table
 * first.  An initial "." is ignored in the hash.
 * Return is a ptr to the symbol table entry.
 */

lookup(length, rp)
register struct hshtab *rp;

{
	register i;
	register char *sp, *np;

	if (rp->hflag&FKEYW)
		if (findkw())
			return(KEYW);
	while ((np = rp->name)) {
		if(length != rp->hnlen)
			goto no;
		sp=symbuf;
		for (i = length; i-- ;) {
			if (*np++ != *sp++)
				goto no;
		}
		if (mossym != (rp->hflag&FMOS))
			goto no;
		csym = rp;
		return(NAME);
	no:
		if (++rp >= &hshtab[HSHSIZ])
			rp = hshtab;
	}
	if(++hshused >= HSHSIZ) {
		error(ediag("Symbol table overflow",
			    "переполнение таблицы символов"));
		exit(1);
	}
	rp->hclass = 0;
	rp->htype = 0;
	rp->hoffset = 0;
	rp->hsubsp = NULL;
	rp->hstrp = NULL;
	rp->hpdown = NULL;
	rp->hblklev = blklev;
	rp->hnlen = length;
	rp->hflag |= mossym;
	rp->name = realloc(symbuf, length+1);
	symbuf = malloc(NCPS);
	csym = rp;
	return(NAME);
}

/*
 * Search the keyword table.
 * Ignore initial "." to avoid member-of-structure
 * problems.
 */
findkw()
{
	register struct kwtab *kp;
	register char *p1, *p2;
	char *wp;
	int firstc;

	wp = symbuf;
	firstc = *wp;
	for (kp=kwtab; (p2 = kp->kwname); kp++) {
		p1 = wp;
		while (*p1 == *p2++)
			if (*p1++ == '\0') {
				cval = kp->kwval;
				return(1);
			}
	}
	*wp = firstc;
	return(0);
}


/*
 * Return the next symbol from the input.
 * peeksym is a pushed-back symbol, peekc is a pushed-back
 * character (after peeksym).
 * mosflg means that the next symbol, if an identifier,
 * is a member of structure or a structure tag, and it
 * gets a "." prepended to it to distinguish
 * it from other identifiers.
 */

static int sharpseen = 0 ;      /* #line hack */

symbol() {
	register c;
	register char *sp;
	register ihash;
	register length;
	int tline;

	if (peeksym>=0) {
		c = peeksym;
		peeksym = -1;
		if (c==NAME)
			mosflg = 0;
		return(c);
	}
	if (peekc) {
		c = peekc;
		peekc = 0;
	} else
		if (eof)
			return(EOFC);
		else
			c = getchar();
loop:
	if (c==EOF) {
		eof++;
		return(EOFC);
	}
	c &= CMASK;
	switch(ctab[c]) {
	case NEWLN:
		if (!inhdr)
			line++;
		inhdr = 0;

	case SPACE:
		c = getchar();
		goto loop;

	case LETTER:
		sp = symbuf;
		length = ihash = 0;
		while(ctab[c]==LETTER || ctab[c]==DIGIT) {
			if (sp<symbuf+NCPS-1) {
				*sp++ = c;
				length++;
				ihash += c;
			}
			c = getchar();
		}
		if(sp<symbuf+NCPS-1)
			*sp++ = '\0';
		if( sharpseen ) return NAME ;
		mossym = 0;
		if (mosflg) {
			mossym = FMOS;
			mosflg = 0;
		}
		peekc = c;
		if ((c=lookup(length, &hshtab[ihash%HSHSIZ]))==KEYW
		     && cval==SIZEOF)
			c = SIZEOF;
		return(c);

	case PERIOD:
	case DIGIT:
		peekc = c;
		return(getnum());

	case SHARP:
		sharpseen++ ;
		if ( (c=symbol())!=NAME||strcmp("line",symbuf)!=0 ) {
conseen:
		    if( c!=CON ) {
			error(ediag("Illegal #",
				    "неизвестный # оператор"));
			sharpseen = 0 ;
			return(c);
		    }
		} else {
		    c=symbol();
		    goto conseen ;
		}
		tline = cval;
		while (ctab[peekc&CMASK]==SPACE)
			peekc = getchar();
		if (peekc=='"') {
			sp = filename;
			while ((c = mapch('"')) >= 0)
				*sp++ = c;
			*sp++ = 0;
			peekc = getchar();
		}
		if (peekc != '\n') {
			error(ediag("Illegal #",
				    "неизвестный # оператор"));
			while (getchar()!='\n' && eof==0)
				;
		}
		peekc = 0;
		line = tline;
		sharpseen = 0 ;
		return(symbol());

	case INSERT:            /* ignore newlines */
		inhdr = 1;
		c = getchar();
		goto loop;

	case PLUS:
		return(subseq(c,PLUS,INCBEF));

	case MINUS:
		return(subseq(c,subseq('>',MINUS,ARROW),DECBEF));

	case ASSIGN:
		c = spnextchar();
		peekc = 0;
		if (c=='=')
			return(EQUAL);
		if (c==' ')
			return(ASSIGN);
		if (c=='<' || c=='>') {
			if (spnextchar() != c) {
				peeksym = ctab[c&CMASK];
				return(ASSIGN);
			}
			peekc = 0;
			return(c=='<'? ASLSH: ASRSH);
		}
		if (ctab[c&CMASK]>=PLUS && ctab[c&CMASK]<=EXOR) {
			if (spnextchar() != ' '
			 && (c=='-' || c=='&' || c=='*')) {
				error(ediag("Warning: %c= operator assumed",
					    "предупреждение: подразумевается %c="), c);
				nerror--;
			}
			c = ctab[c&CMASK];
			return(c+ASPLUS-PLUS);
		}
		peekc = c;
		return(ASSIGN);

	case LESS:
		if (subseq(c,0,1)) return(LSHIFT);
		return(subseq('=',LESS,LESSEQ));

	case GREAT:
		if (subseq(c,0,1)) return(RSHIFT);
		return(subseq('=',GREAT,GREATEQ));

	case EXCLA:
		return(subseq('=',EXCLA,NEQUAL));

	case BSLASH:
		if (subseq('/', 0, 1))
			return(MAX);
		goto unkn;

	case DIVIDE:
		if (subseq('\\', 0, 1))
			return(MIN);
		if (subseq('*',1,0))
			return(DIVIDE);
		while ((c = spnextchar()) != EOFC) {
			peekc = 0;
			if (c=='*') {
				if (spnextchar() == '/') {
					peekc = 0;
					c = getchar();
					goto loop;
				}
			}
		}
		eof++;
		error(ediag("Nonterminated comment",
			    "неоконченный комментарий"));
		return(0);

	case DQUOTE:
		cval = isn++;
		return(STRING);

	case SQUOTE:
		return(getcc());

	case AND:
		return(subseq('&', AND, LOGAND));

	case OR:
		return(subseq('|', OR, LOGOR));

	case UNKN:
	unkn:
		error(ediag("Unknown charachter",
			    "неизвестный символ"));
		c = getchar();
		goto loop;

	}
	return(ctab[c&CMASK]);
}

/*
 * Read a number.  Return kind.
 */
getnum()
{
	register char *np;
	register c, base;
	int expseen, sym, ndigit;
	char *nsyn;
	int maxdigit;

	nsyn = ediag("Number syntax",
		     "синтаксическая ошибка в числе");
	lcval = 0;
	base = 10;
	maxdigit = 0;
	np = numbuf;
	ndigit = 0;
	sym = CON;
	expseen = 0;
	if ((c=spnextchar()) == '0')
		base = 8;
	for (;; c = getchar()) {
		*np++ = c;
		if (ctab[c&CMASK]==DIGIT || (base==16) &&
			  ('a'<=c&&c<='f'||'A'<=c&&c<='F')) {
			if (base==8)
				lcval <<= 3;
			else if (base==10)
				lcval = ((lcval<<2) + lcval)<<1;
			else
				lcval <<= 4;
			if (ctab[c&CMASK]==DIGIT)
				c -= '0';
			else if (c>='a')
				c -= 'a'-10;
			else
				c -= 'A'-10;
			lcval += c;
			ndigit++;
			if (c>maxdigit)
				maxdigit = c;
			continue;
		}
		if (c=='.') {
			if (base==16 || sym==FCON)
				error(nsyn);
			sym = FCON;
			base = 10;
			continue;
		}
		if (ndigit==0) {
			sym = DOT;
			break;
		}
		if ((c=='e'||c=='E'||c=='е'||c=='Е') && expseen==0) {
			expseen++;
			sym = FCON;
			if (base==16 || maxdigit>=10)
				error(nsyn);
			base = 10;
			*np++ = c = getchar();
			if (c!='+' && c!='-' && ctab[c&CMASK]!=DIGIT)
				break;
		} else if(c=='x' || c=='X' || c=='х' || c=='Х' ) {
			if (base!=8 || lcval!=0 || sym!=CON)
				error(nsyn);
			base = 16;
		} else if ((c=='l' || c=='L') && sym==CON) {
			c = getchar();
			sym = LCON;
			break;
		} else
			break;
	}
	peekc = c;
	if (maxdigit >= base)
		error(nsyn);
	if (sym==FCON) {
		np[-1] = 0;
		cval = np-numbuf;
		return(FCON);
	}
	if (sym==CON && (lcval<0 || lcval>MAXINT&&base==10 || (lcval>>1)>MAXINT)) {
		sym = LCON;
	}
	cval = lcval;
	return(sym);
}

/*
 * If the next input character is c, return b and advance.
 * Otherwise push back the character and return a.
 */
subseq(c,a,b)
{
	if (spnextchar() != c)
		return(a);
	peekc = 0;
	return(b);
}

/*
 * Write out a string, either in-line
 * or in the string temp file labelled by
 * lab.
 */
putstr(lab, amax)
{
	register int c, max;

	nchstr = 0;
	max = amax;
	if (lab) {
		strflg++;
		outcode("BNB", LABEL, lab, BDATA);
		max = 10000;
	} else
		outcode("B", BDATA);
	while ((c = mapch('"')) >= 0) {
		if (nchstr < max) {
			nchstr++;
			if (nchstr%15 == 0)
				outcode("0B", BDATA);
			outcode("1N", c & 0377);
		}
	}
	if (nchstr < max) {
		nchstr++;
		outcode("10");
	}
	outcode("0");
	strflg = 0;
}

/*
 * read a single-quoted character constant.
#ifndef sparc
 * The routine is sensitive to the layout of
 * characters in a word.
#endif
 */
getcc()
{
	register int c, cc;
#ifdef sparc
	register int accum;

	accum = 0;
	cc = 0;
	while((c=mapch('\'')) >= 0) {
		if(cc++ >= LNCPW) break;
		accum = (accum << 8) + (c & 0377);
	}
	cval = accum;
#else
	register char *ccp;

	cval = 0;
	ccp = &cval;
	cc = 0;
	while((c=mapch('\'')) >= 0)
		if(cc++ < LNCPW)
			*ccp++ = c;
#endif
	if (cc>LNCPW)
		error(ediag("Long character constant",
			    "слишком длинная символьная константа"));
	if (cc==1) {
		char realc = cval;
		cval = realc /* & 0377 */ ; /* @VG: 31.03.85 */
	}
	return(CON);
}

/*
 * Read a character in a string or character constant,
 * detecting the end of the string.
 * It implements the escape sequences.
 */
mapch(ac)
{
	register int a, c, n;
	static mpeek;

	c = ac;
	if (a = mpeek)
		mpeek = 0;
	else
		a = getchar();
loop:
	if (a==c)
		return(-1);
	switch(a) {

	case '\n':
	case '\0':
		error(ediag("Nonterminated string",
			    "неоконченная строка"));
		peekc = a;
		return(-1);

	case '\\':
		switch (a=getchar()) {

		case 't':
			return('\t');

		case 'n':
			return('\n');

		case 'b':
			return('\b');

		case 'f':
			return('\014');

		case 'v':
			return('\013');

		case '0': case '1': case '2': case '3':
		case '4': case '5': case '6': case '7':
			n = 0;
			c = 0;
			while (++c<=3 && '0'<=a && a<='7') {
				n <<= 3;
				n += a-'0';
				a = getchar();
			}
			mpeek = a;
			return(n);

		case 'r':
			return('\r');

		case '\n':
			if (!inhdr)
				line++;
			inhdr = 0;
			a = getchar();
			goto loop;
		}
	}
	return(a);
}

/*
 * Read an expression and return a pointer to its tree.
 * It's the classical bottom-up, priority-driven scheme.
 * The initflg prevents the parse from going past
 * "," or ":" because those delimiters are special
 * in initializer (and some other) expressions.
 */
struct tnode *
tree()
{
	int *op, opst[SSIZE], *pp, prst[SSIZE];
	register int andflg, o;
	register struct hshtab *cs;
	int p, ps, os;
	struct tnode *cmst[CMSIZ];
	struct lnode *lcp;
	lineexpr = line;
	curbase = funcbase;
	op = opst;
	pp = prst;
	cp = cmst;
	*op = SEOF;
	*pp = 06;
	andflg = 0;

advanc:
	switch (o=symbol()) {

	case NAME:
		cs = csym;
		if (cs->hclass==TYPEDEF)
			goto atype;
		if (cs->hclass==ENUMCON) {
			*cp++ = cblock(cs->hoffset);
			goto tand;
		}
		if (cs->hclass==0 && cs->htype==0)
			if(nextchar()=='(') {
				/* set function */
				cs->hclass = EXTERN;
				cs->htype = FUNC;
			} else {
				cs->hclass = STATIC;
				error(ediag("%s undefined; func. %s%s",
					    "имя %s не определено; функ. %s%s"),
					       cs->name, funcsym->name,
					       initflg?ediag(" (Warning only)",
							     " (только предупреждение)")
						      : "" );
				if (initflg) {
					cs->hclass = EXTERN;
					nerror--;
				}
			}
		*cp++ = nblock(cs);
		goto tand;

	case FCON:
		*cp++ = fblock(DOUBLE, copnum(cval));
		goto tand;

	case LCON:
		cs = gblock(sizeof(*lcp));
		cs->op = LCON;
		cs->type = LONG;
		cs->lvalue = lcval;
		*cp++ = cs;
		goto tand;

	case CON:
		*cp++ = cblock(cval);
		goto tand;

	/* fake a static char array */
	case STRING:
		putstr(cval, 0);
		cs = gblock(sizeof(*cs));
		cs->hclass = STATIC;
		cs->hoffset = cval;
		*cp++ = block(NAME, ARRAY+CHAR, &nchstr, NULL, cs);

	tand:
		if(cp>=cmst+CMSIZ) {
			error(ediag("Expression overflow",
				    "слишком сложное выражение"));
			exit(1);
		}
		if (andflg)
			goto syntax;
		andflg = 1;
		goto advanc;

	case KEYW:
	atype:
		if (*op != LPARN || andflg)
			goto syntax;
		peeksym = o;
		*cp++ = xprtype(gblock(sizeof(*xprtype())));
		if ((o=symbol()) != RPARN)
			goto syntax;
		o = CAST;
		--op;
		--pp;
		if (*op == SIZEOF) {
			andflg = 1;
			*pp = 100;
			goto advanc;
		}
		goto oponst;

	case INCBEF:
	case DECBEF:
		if (andflg)
			o += 2;
		goto oponst;

	case COMPL:
	case EXCLA:
	case SIZEOF:
		if (andflg)
			goto syntax;
		goto oponst;

	case MINUS:
		if (!andflg)
			o = NEG;
		andflg = 0;
		goto oponst;

	case AND:
	case TIMES:
		if (andflg)
			andflg = 0;
		else if (o==AND)
			o = AMPER;
		else
			o = STAR;
		goto oponst;

	case LPARN:
		if (andflg) {
			o = symbol();
			if (o==RPARN)
				o = MCALL;
			else {
				peeksym = o;
				o = CALL;
				andflg = 0;
			}
		}
		goto oponst;

	case RBRACK:
	case RPARN:
		if (!andflg)
			goto syntax;
		goto oponst;

	case DOT:
	case ARROW:
		mosflg++;
		break;

	case ASSIGN:
		if (andflg==0 && PLUS<=*op && *op<=EXOR) {
			o = *op-- + ASPLUS - PLUS;
			pp--;
			goto oponst;
		}
		break;

	}
	/* binaries */
	if (andflg==0)
		goto syntax;
	andflg = 0;

oponst:
	p = (opdope[o]>>9) & 077;
opon1:
	ps = *pp;
	if (p>ps || p==ps && (opdope[o]&RASSOC)!=0) {
		switch (o) {

		case INCAFT:
		case DECAFT:
			p = 37;
			break;
		case LPARN:
		case LBRACK:
		case CALL:
			p = 04;
		}
		if (initflg) {
			if ((o==COMMA && *op!=LPARN && *op!=CALL)
			 || (o==COLON && *op!=QUEST)) {
				p = 00;
				goto opon1;
			}
		}
		if (op >= &opst[SSIZE-1]) {
			error(ediag("Expression overflow",
				    "слишком сложное выражение"));
			exit(1);
		}
		*++op = o;
		*++pp = p;
		goto advanc;
	}
	--pp;
	switch (os = *op--) {

	case SEOF:
		peeksym = o;
		build(0);               /* flush conversions */
		return(*--cp);

	case COMMA:
		if (*op != CALL)
			os = SEQNC;
		break;

	case CALL:
		if (o!=RPARN)
			goto syntax;
		build(os);
		goto advanc;

	case MCALL:
		*cp++ = NULL;   /* empty arglist */
		os = CALL;
		break;

	case INCBEF:
	case INCAFT:
	case DECBEF:
	case DECAFT:
		*cp++ = cblock(1);
		break;

	case LPARN:
		if (o!=RPARN)
			goto syntax;
		goto advanc;

	case LBRACK:
		if (o!=RBRACK)
			goto syntax;
		build(LBRACK);
		goto advanc;
	}
	build(os);
	goto opon1;

syntax:
	error(ediag("Expression syntax",
		    "синтаксическая ошибка в выражении"));
	errflush(o);
	return(0);
}

struct hshtab *
xprtype(atyb)
struct hshtab *atyb;
{
	register struct hshtab *tyb;
	struct hshtab typer;
	int sc;
	register char *md, *fb;
	struct tnode *scp;

	tyb = atyb;
	fb = funcbase;
	md = maxdecl;
	scp = cp;
	funcbase = curbase;
	sc = DEFXTRN;           /* will cause error if class mentioned */
	getkeywords(&sc, &typer);
	tyb->hclass = 0;
	tyb->hblklev = 0;
	decl1(&sc, &typer, 0, tyb);
	funcbase = fb;
	maxdecl = md;
	cp = scp;
	tyb->op = ETYPE;
	return(tyb);
}

char *
copnum(len)
{
	register char *s1, *s2, *s3;

	s1 = s2 = gblock((len+LNCPW-1) & ~(LNCPW-1));
	s3 = numbuf;
	while (*s2++ = *s3++);
	return(s1);
}
