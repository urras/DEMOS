# include "dextern"

# define IDENTIFIER 257
# define MARK 258
# define TERM 259
# define LEFT 260
# define RIGHT 261
# define BINARY 262
# define PREC 263
# define LCURLY 264
# define C_IDENTIFIER 265  /* name followed by colon */
# define NUMBER 266
# define START 267
# define TYPEDEF 268
# define TYPENAME 269
# define UNION 270
# define ENDFILE 0

	/* communication variables between various I/O routines */

char *infile;   /* input file name */
int numbval;    /* value of an input number */
char tokname[NAMESIZE]; /* input token name */

	/* storage of names */

char cnames[CNAMSZ];    /* place where token and nonterminal names are stored */
int cnamsz = CNAMSZ;    /* size of cnames */
char * cnamp = cnames;  /* place where next name is to be put in */
int ndefout = 3;  /* number of defined symbols output */

	/* storage of types */
int ntypes;     /* number of types defined */
char * typeset[NTYPES]; /* pointers to type tags */

	/* symbol tables for tokens and nonterminals */

int ntokens = 0;
struct toksymb tokset[NTERMS];
int toklev[NTERMS];
int nnonter = -1;
struct ntsymb nontrst[NNONTERM];
int start;      /* start symbol */

	/* assigned token type values */
int extval = 0;

	/* input and output file descriptors */

FILE * finput;          /* yacc input file */
FILE * faction;         /* file for saving actions */
FILE * fdefine;         /* file for # defines */
FILE * ftable;          /* y.tab.c file */
FILE * ftemp;           /* tempfile to pass 2 */
FILE * foutput;         /* y.output file */

	/* storage for grammar rules */

int mem0[MEMSIZE] ; /* production storage */
int *mem = mem0;
int nprod= 1;   /* number of productions */
int *prdptr[NPROD];     /* pointers to descriptions of productions */
int levprd[NPROD] ;     /* precedence levels for the productions */

char *parname = PARSER;
int      oflag = 0;

setup(argc,argv) int argc; char *argv[];
{       int i,j,lev,t, ty;
	int c;
	int *p;
	char actname[8];

	foutput = NULL;
	fdefine = NULL;
	i = 1;
	while( argc >= 2  && argv[1][0] == '-' ) {
		while( *++(argv[1]) ){
			switch( *argv[1] ){
			case 'f': parname = *(++argv+1);
				  argc--; goto nextcc;
			case 'v':
			case 'V':
				foutput = fopen(FILEU, "w" );
				if( foutput == NULL ) error( "Нельзя создать y.output" );
				continue;
			case 'D':
			case 'd':
				fdefine = fopen( FILED, "w" );
				continue;
			case 'o':
			case 'O':
				oflag = 1;
				continue;
		     /*
			case 'r':
			case 'R':
				error( "Ratfor Yacc умер: sorry...\n" );
		      */
			default:
				error( "Неверный ключ: %c", *argv[1]);
				}
			}
nextcc:         argv++;
		argc--;
		}

	ftable = fopen( OFILE, "w" );

	if( ftable == NULL ) error( "Нельзя создатьть y.tab.c" );

	ftemp = fopen( TEMPNAME, "w" );
	faction = fopen( ACTNAME, "w" );
	if( ftemp==NULL || faction==NULL ) error( "Нельзя создать временные файлы" );

	if( argc < 2 || ((finput=fopen( infile=argv[1], "r" )) == NULL ) ){
		error( "Нельзя открыть входной файл" );
		}

	cnamp = cnames;
	defin(0,"$end");
	extval = 0400;
	defin(0,"error");
	defin(1,"$accept");
	mem=mem0;
	lev = 0;
	ty = 0;
	i=0;

	/* sorry -- no yacc parser here.....
		we must bootstrap somehow... */

	for( t=gettok();  t!=MARK && t!= ENDFILE; ){
		switch( t ){

		case ';':
			t = gettok();
			break;

		case START:
			if( (t=gettok()) != IDENTIFIER ){
				error( "Неправильный %%start" );
				}
			start = chfind(1,tokname);
			t = gettok();
			continue;

		case TYPEDEF:
			if( (t=gettok()) != TYPENAME ) error( "Неправильный %%type" );
			ty = numbval;
			for(;;){
				t = gettok();
				switch( t ){

				case IDENTIFIER:
					if( (t=chfind( 1, tokname ) ) < NTBASE ) {
						j = TYPE( toklev[t] );
						if( j!= 0 && j != ty ){
							error( "Переопределение типа терминала %s",
								tokset[t].name );
							}
						else SETTYPE( toklev[t],ty);
						}
					else {
						j = nontrst[t-NTBASE].tvalue;
						if( j != 0 && j != ty ){
							error( "Переопределение типа нетерминала %s",
								nontrst[t-NTBASE].name );
							}
						else nontrst[t-NTBASE].tvalue = ty;
						}
				case ',':
					continue;

				case ';':
					t = gettok();
					break;
				default:
					break;
					}
				break;
				}
			continue;

		case UNION:
			/* copy the union declaration to the output */
			cpyunion();
			t = gettok();
			continue;

		case LEFT:
		case BINARY:
		case RIGHT:
			++i;
		case TERM:
			lev = t-TERM;  /* nonzero means new prec. and assoc. */
			ty = 0;

			/* get identifiers so defined */

			t = gettok();
			if( t == TYPENAME ){ /* there is a type defined */
				ty = numbval;
				t = gettok();
				}

			for(;;) {
				switch( t ){

				case ',':
					t = gettok();
					continue;

				case ';':
					break;

				case IDENTIFIER:
					j = chfind(0,tokname);
					if( lev ){
						if( ASSOC(toklev[j]) ) error( "Переопределение предшествования %s", tokname );
						SETASC(toklev[j],lev);
						SETPLEV(toklev[j],i);
						}
					if( ty ){
						if( TYPE(toklev[j]) ) error( "Переопределение типа %s", tokname );
						SETTYPE(toklev[j],ty);
						}
					if( (t=gettok()) == NUMBER ){
						tokset[j].value = numbval;
						if( j < ndefout && j>2 ){
							error( "Тип %s не определен", tokset[j].name );
							}
						t=gettok();
						}
					continue;

					}

				break;
				}

			continue;

		case LCURLY:
			defout();
			cpycode();
			t = gettok();
			continue;

		default:
			error( "Синтаксическая ошибка" );

			}

		}

	if( t == ENDFILE ){
		error( "Конец файла перед %%" );
		}

	/* t is MARK */

	defout();

		fprintf( ftable,  "#define yyclearin yychar = -1\n" );
		fprintf( ftable,  "#define yyerrok yyerrflag = 0\n" );
		fprintf( ftable,  "extern int yychar;\nextern short yyerrflag;\n" );
		fprintf( ftable,  "#ifndef YYMAXDEPTH\n#define YYMAXDEPTH 150\n#endif\n" );
		if( !ntypes ) fprintf( ftable,  "#ifndef YYSTYPE\n#define YYSTYPE int\n#endif\n" );
		fprintf( ftable,  "YYSTYPE yylval, yyval;\n" );

	prdptr[0]=mem;
	/* added production */
	*mem++ = NTBASE;
	*mem++ = start;  /* if start is 0, we will overwrite with the lhs of the first rule */
	*mem++ = 1;
	*mem++ = 0;
	prdptr[1]=mem;

	while( (t=gettok()) == LCURLY ) cpycode();

	if( t != C_IDENTIFIER ) error( "Плохой синтаксис первого правила" );

	if( !start ) prdptr[0][1] = chfind(1,tokname);

	/* read rules */

	while( t!=MARK && t!=ENDFILE ){

		/* process a rule */

		if( t == '|' ){
			*mem++ = *prdptr[nprod-1];
			}
		else if( t == C_IDENTIFIER ){
			*mem = chfind(1,tokname);
			if( *mem < NTBASE ) error( "Терминал недопустим" );
			++mem;
			}
		else error( "пропущено ':' или '|'" );

		/* read rule body */


		t = gettok();
	more_rule:
		while( t == IDENTIFIER ) {
			*mem = chfind(1,tokname);
			if( *mem<NTBASE ) levprd[nprod] = toklev[*mem];
			++mem;
			t = gettok();
			}


		if( t == PREC ){
			if( gettok()!=IDENTIFIER) error( "Неправильный %%prec" );
			j = chfind(2,tokname);
			if( j>=NTBASE)error("Нетерминал %s недопустим после %%prec", nontrst[j-NTBASE].name);
			levprd[nprod]=toklev[j];
			t = gettok();
			}

		if( t == '=' ){
			levprd[nprod] |= ACTFLAG;
			if( oflag ){
				fprintf( faction, "\nb%d ", nprod );
			}else{
				fprintf( faction, "\ncase %d:", nprod );
			}
			cpyact( mem-prdptr[nprod]-1 );
			if( oflag ){
				fprintf( faction, "\ne%d", nprod );
			}else{
				fprintf( faction, " break;" );
			}
			if( (t=gettok()) == IDENTIFIER ){
				/* action within rule... */

				sprintf( actname, "$$%d", nprod );
				j = chfind(1,actname);  /* make it a nonterminal */

				/* the current rule will become rule number nprod+1 */
				/* move the contents down, and make room for the null */

				for( p=mem; p>=prdptr[nprod]; --p ) p[2] = *p;
				mem += 2;

				/* enter null production for action */

				p = prdptr[nprod];

				*p++ = j;
				*p++ = -nprod;

				/* update the production information */

				levprd[nprod+1] = levprd[nprod] & ~ACTFLAG;
				levprd[nprod] = ACTFLAG;

				if( ++nprod >= NPROD ) error( "Больше, чем %d правил", NPROD );
				prdptr[nprod] = p;

				/* make the action appear in the original rule */
				*mem++ = j;

				/* get some more of the rule */

				goto more_rule;
				}

			}

		while( t == ';' ) t = gettok();

		*mem++ = -nprod;

		/* check that default action is reasonable */

		if( ntypes && !(levprd[nprod]&ACTFLAG) && nontrst[*prdptr[nprod]-NTBASE].tvalue ){
			/* no explicit action, LHS has value */
			register tempty;
			tempty = prdptr[nprod][1];
			if( tempty < 0 ) error( "Должно возвращать значение" );
			else if( tempty >= NTBASE ) tempty = nontrst[tempty-NTBASE].tvalue;
			else tempty = TYPE( toklev[tempty] );
			if( tempty != nontrst[*prdptr[nprod]-NTBASE].tvalue ){
				error( "Действие по умолчанию может вызвать конфликт типов" );
				}
			}

		if( ++nprod >= NPROD ) error( "Больше, чем %d правил", NPROD );
		prdptr[nprod] = mem;
		levprd[nprod]=0;

		}

	/* end of all rules */

	finact();
	if( t == MARK ){
		fprintf( ftable, "\n# line %d \"%s\"\n", lineno, infile );
		while( (c=getc(finput)) != EOF ) putc( c, ftable );
		}
	fclose( finput );
	}

finact(){
	/* finish action routine */

	fclose(faction);

	fprintf( ftable, "# define YYERRCODE %d\n", tokset[2].value );

	}

defin( t, s ) register char  *s; {
/*      define s to be a terminal if t=0
	or a nonterminal if t=1         */

	register val;

	if (t) {
		if( ++nnonter >= NNONTERM )
error("Слишком много нетерминалов, предел - %d",NNONTERM);
		nontrst[nnonter].name = cstash(s);
		return( NTBASE + nnonter );
		}
	/* must be a token */
	if( ++ntokens >= NTERMS ) error("Слишком много терминалов, предел %d",NTERMS );
	tokset[ntokens].name = cstash(s);

	/* establish value for token */

	if( s[0]==' ' && s[2]=='\0' ) /* single character literal */
		val = s[1];
	else if ( s[0]==' ' && s[1]=='\\' ) { /* escape sequence */
		if( s[3] == '\0' ){ /* single character escape sequence */
			switch ( s[2] ){
					 /* character which is escaped */
			case 'n': val = '\n'; break;
			case 'r': val = '\r'; break;
			case 'b': val = '\b'; break;
			case 't': val = '\t'; break;
			case 'f': val = '\f'; break;
			case '\'': val = '\''; break;
			case '"': val = '"'; break;
			case '\\': val = '\\'; break;
			default: error( "Плохая \\-последовательность" );
				}
			}
		else if( s[2] <= '7' && s[2]>='0' ){ /* \nnn sequence */
			if( s[3]<'0' || s[3] > '7' || s[4]<'0' ||
				s[4]>'7' || s[5] != '\0' )
error("Плохая конструкция \\nnn" );
			val = 64*s[2] + 8*s[3] + s[4] - 73*'0';
			if( val == 0 ) error( "'\\000' недопустим" );
			}
		}
	else {
		val = extval++;
		}
	tokset[ntokens].value = val;
	toklev[ntokens] = 0;
	return( ntokens );
	}

defout(){ /* write out the defines (at the end of the declaration section) */

	register int i, c;
	register char *cp;

	for( i=ndefout; i<=ntokens; ++i ){

		cp = tokset[i].name;
		if( *cp == ' ' ) ++cp;  /* literals */

		for( ; (c= *cp)!='\0'; ++cp ){

			if( islower(c) || isupper(c) || isdigit(c) || c=='_' );  /* VOID */
			else goto nodef;
			}

		fprintf( ftable, "# define %s %d\n", tokset[i].name, tokset[i].value );
		if( fdefine != NULL ) fprintf( fdefine, "# define %s %d\n", tokset[i].name, tokset[i].value );

	nodef:  ;
		}

	ndefout = ntokens+1;

	}

char *
cstash( s ) register char *s; {
	char *temp;

	temp = cnamp;
	do {
		if( cnamp >= &cnames[cnamsz] )
error("Слишком много символов в именах и литералах" );
		else *cnamp++ = *s;
		}  while ( *s++ );
	return( temp );
	}

gettok() {
	register i, base;
	static int peekline; /* number of '\n' seen in lookahead */
	register c, match, reserve;

begin:
	reserve = 0;
	lineno += peekline;
	peekline = 0;
	c = getc(finput);
	while( c==' ' || c=='\n' || c=='\t' || c=='\f' ){
		if( c == '\n' ) ++lineno;
		c=getc(finput);
		}
	if( c == '/' ){ /* skip comment */
		lineno += skipcom();
		goto begin;
		}

	switch(c){

	case EOF:
		return(ENDFILE);
	case '{':
		ungetc( c, finput );
		return( '=' );  /* action ... */
	case '<':  /* get, and look up, a type name (union member name) */
		i = 0;
		while( (c=getc(finput)) != '>' && c>=0 && c!= '\n' ){
			tokname[i] = c;
			if( ++i >= NAMESIZE ) --i;
			}
		if( c != '>' ) error( "Незаконченое <...> предложение" );
		tokname[i] = '\0';
		for( i=1; i<=ntypes; ++i ){
			if( !strcmp( typeset[i], tokname ) ){
				numbval = i;
				return( TYPENAME );
				}
			}
		typeset[numbval = ++ntypes] = cstash( tokname );
		return( TYPENAME );

	case '"':
	case '\'':
		match = c;
		tokname[0] = ' ';
		i = 1;
		for(;;){
			c = getc(finput);
			if( c == '\n' || c == EOF )
				error("Недопустимая ' или \"" );
			if( c == '\\' ){
				c = getc(finput);
				tokname[i] = '\\';
				if( ++i >= NAMESIZE ) --i;
				}
			else if( c == match ) break;
			tokname[i] = c;
			if( ++i >= NAMESIZE ) --i;
			}
		break;

	case '%':
	case '\\':

		switch(c=getc(finput)) {

		case '0':       return(TERM);
		case '<':       return(LEFT);
		case '2':       return(BINARY);
		case '>':       return(RIGHT);
		case '%':
		case '\\':      return(MARK);
		case '=':       return(PREC);
		case '{':       return(LCURLY);
		default:        reserve = 1;
			}

	default:

		if( isdigit(c) ){ /* number */
			numbval = c-'0' ;
			base = (c=='0') ? 8 : 10 ;
			for( c=getc(finput); isdigit(c) ; c=getc(finput) ){
				numbval = numbval*base + c - '0';
				}
			ungetc( c, finput );
			return(NUMBER);
			}
		else if( islower(c) || isupper(c) || c=='_' || c=='.' || c=='$' ){
			i = 0;
			while( islower(c) || isupper(c) || isdigit(c) || c=='_' || c=='.' || c=='$' ){
				tokname[i] = c;
				if( reserve && isupper(c) ) tokname[i] += 'a'-'A';
				if( ++i >= NAMESIZE ) --i;
				c = getc(finput);
				}
			}
		else return(c);

		ungetc( c, finput );
		}

	tokname[i] = '\0';

	if( reserve ){ /* find a reserved word */
		if( !strcmp(tokname,"term")) return( TERM );
		if( !strcmp(tokname,"token")) return( TERM );
		if( !strcmp(tokname,"left")) return( LEFT );
		if( !strcmp(tokname,"nonassoc")) return( BINARY );
		if( !strcmp(tokname,"binary")) return( BINARY );
		if( !strcmp(tokname,"right")) return( RIGHT );
		if( !strcmp(tokname,"prec")) return( PREC );
		if( !strcmp(tokname,"start")) return( START );
		if( !strcmp(tokname,"type")) return( TYPEDEF );
		if( !strcmp(tokname,"union")) return( UNION );
		error("Неправильное ключевое слово: %s", tokname );
		}

	/* look ahead to distinguish IDENTIFIER from C_IDENTIFIER */

	c = getc(finput);
	while( c==' ' || c=='\t'|| c=='\n' || c=='\f' || c== '/' ) {
		if( c == '\n' ) ++peekline;
		else if( c == '/' ){ /* look for comments */
			peekline += skipcom();
			}
		c = getc(finput);
		}
	if( c == ':' ) return( C_IDENTIFIER );
	ungetc( c, finput );
	return( IDENTIFIER );
}

fdtype( t ){ /* determine the type of a symbol */
	register v;
	if( t >= NTBASE ) v = nontrst[t-NTBASE].tvalue;
	else v = TYPE( toklev[t] );
	if( v <= 0 ) error( "Неопределеный тип %s", (t>=NTBASE)?nontrst[t-NTBASE].name:
			tokset[t].name );
	return( v );
	}

chfind( t, s ) register char *s; {
	int i;

	if (s[0]==' ')t=0;
	TLOOP(i){
		if(!strcmp(s,tokset[i].name)){
			return( i );
			}
		}
	NTLOOP(i){
		if(!strcmp(s,nontrst[i].name)) {
			return( i+NTBASE );
			}
		}
	/* cannot find name */
	if( t>1 )
		error( "%s неопределен", s );
	return( defin( t, s ) );
	}

cpyunion(){
	/* copy the union declaration to the output, and the define file if present */

	int level, c;
	fprintf( ftable, "\n# line %d \"%s\"\n", lineno, infile );
	fprintf( ftable, "typedef union " );
	if( fdefine ) fprintf( fdefine, "\ntypedef union " );

	level = 0;
	for(;;){
		if( (c=getc(finput)) < 0 )
error( "EOF в %%union" );
		putc( c, ftable );
		if( fdefine ) putc( c, fdefine );

		switch( c ){

		case '\n':
			++lineno;
			break;

		case '{':
			++level;
			break;

		case '}':
			--level;
			if( level == 0 ) { /* we are finished copying */
				fprintf( ftable, " YYSTYPE;\n" );
				if( fdefine ) fprintf( fdefine, " YYSTYPE;\nextern YYSTYPE yylval;\n" );
				return;
				}
			}
		}
	}

cpycode(){ /* copies code between \{ and \} */

	int c;
	c = getc(finput);
	if( c == '\n' ) {
		c = getc(finput);
		lineno++;
		}
	fprintf( ftable, "\n# line %d \"%s\"\n", lineno, infile );
	while( c>=0 ){
		if( c=='\\' )
			if( (c=getc(finput)) == '}' ) return;
			else putc('\\', ftable );
		if( c=='%' )
			if( (c=getc(finput)) == '}' ) return;
			else putc('%', ftable );
		putc( c , ftable );
		if( c == '\n' ) ++lineno;
		c = getc(finput);
		}
	error("EOF перед %%}" );
	}

skipcom(){ /* skip over comments */
	register c, i=0;  /* i is the number of lines skipped */

	/* skipcom is called after reading a / */

	if( getc(finput) != '*' ) error( "Плохой коментарий" );
	c = getc(finput);
	while( c != EOF ){
		while( c == '*' ){
			if( (c=getc(finput)) == '/' ) return( i );
			}
		if( c == '\n' ) ++i;
		c = getc(finput);
		}
	error( "EOF в коментарии ?" );
	/* NOTREACHED */
	}

cpyact(offset){ /* copy C action to the next ; or closing } */
	int brac, c, match, j, s, tok;

	if( oflag ){
		fprintf( faction, "%d '%s'\n", lineno, infile );
	}else{
		fprintf( faction, "\n# line %d \"%s\"\n", lineno, infile );
	}

	brac = 0;

loop:
	c = getc(finput);
swt:
	switch( c ){

case ';':
		if( brac == 0 ){
			putc( c , faction );
			return;
			}
		goto lcopy;

case '{':
		brac++;
		goto lcopy;

case '$':
		s = 1;
		tok = -1;
		c = getc(finput);
		if( c == '<' ){ /* type description */
			ungetc( c, finput );
			if( gettok() != TYPENAME )
				error( "Плохой синтаксис в $<ident>-предложении" );
			tok = numbval;
			c = getc(finput);
			}
		if( c == '$' ){
			fprintf( faction, "yyval");
			if( ntypes ){ /* put out the proper tag... */
				if( tok < 0 ) tok = fdtype( *prdptr[nprod] );
				fprintf( faction, ".%s", typeset[tok] );
				}
			goto loop;
			}
		if( c == '-' ){
			s = -s;
			c = getc(finput);
			}
		if( isdigit(c) ){
			j=0;
			while( isdigit(c) ){
				j= j*10+c-'0';
				c = getc(finput);
				}

			j = j*s - offset;
			if( j > 0 ){
				error( "Неправильное $%d", j+offset );
				}

			fprintf( faction, "yypvt[-%d]", -j );
			if( ntypes ){ /* put out the proper tag */
				if( j+offset <= 0 && tok < 0 )
					error( "Неопределен тип $%d", j+offset );
				if( tok < 0 ) tok = fdtype( prdptr[nprod][j+offset] );
				fprintf( faction, ".%s", typeset[tok] );
				}
			goto swt;
			}
		putc( '$' , faction );
		if( s<0 ) putc('-', faction );
		goto swt;

case '}':
		if( --brac ) goto lcopy;
		putc( c, faction );
		return;


case '/':       /* look for comments */
		putc( c , faction );
		c = getc(finput);
		if( c != '*' ) goto swt;

		/* it really is a comment */

		if( !oflag ) putc( c , faction );
		c = getc(finput);
		while( c != EOF ){
			while( c=='*' ){
				if( !oflag ) putc( c , faction );
				if( (c=getc(finput)) == '/' ) goto lcopy;
				}
			if( !oflag ) putc( c , faction );
			if( c == '\n' )++lineno;
			c = getc(finput);
			}
		error( "EOF в коментарии" );

case '\'':      /* character constant */
		match = '\'';
		goto string;

case '"':       /* character string */
		match = '"';

	string:

		putc( c , faction );
		while( c=getc(finput) ){

			if( c=='\\' ){
				putc( c , faction );
				c=getc(finput);
				if( c == '\n' ) ++lineno;
			}else if( c==match ) goto lcopy;
			 else if( c=='\n' )
				error( "EOL в символьной константе или цепочке" );
			putc( c , faction );
		}
		error( "EOF в символьной константе или цепочке" );

case EOF:
		error("" );

case '\n':      ++lineno;
		goto lcopy;

		}

lcopy:
	putc( c , faction );
	goto loop;
	}
