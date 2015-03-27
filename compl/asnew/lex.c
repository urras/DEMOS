/*
 * Ассемблер  вер. 1.0
 *    ЭВМ СМ-4   МНОС РЛ 1.2
 *    Автор: Антонов В.Г. ( ВМК МГУ, ИПК Минавтопрома )
 *    Последнее изменение: 28.12.1985
 */

#include <stdio.h>
#include <ediag.h>
#include "as.h"

/*
 * Лексический анализатор
 */

int     linecnt = 1;    /* Счетчик строк */
int     eofflag = 0;    /* Флаг - конец файла */
int     backflg = 0;    /* Флаг - признак того,что был произведен возврат на
               один символ назад */
int     badchflg = 0;   /* Флаг для подавления диагностик о плохих символах */
char    str[80];
int     nextsym;
char    istring[MAXSTRING]; /* Входная строка символов */
int     istrp;              /* Длина входной строки */

#define BACK    backflg++
#define READC   (backflg?(backflg = 0, nextsym): readc1())

/*  Source EOF  */
#define Seof    0177  /*UNIX*/
/*#define Seof    0     MIUS*/

#define DIG     01
#define DDIG    02
#define LET     04
#define SPEC    010
#define EOW     020
#define Z       EOW

static char    sclass[] = {
/*  0               10              20              30                      */
    0,0,0,0,0,0,0,0,0,Z,Z,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 000 */
    Z,8,0,0,8,8,8,0,8,8,8,8,8,8,4,0,1,1,1,1,1,1,1,1,3,3,0,Z,0,8,0,0, /* 040 */
    0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,8,Z,8,8,4, /* 100 */
    0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,8,0,4,0, /* 140 */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 200 */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 240 */
    4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4, /* 300 */
    4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4  /* 340 */
};

#define W_TEXT  "text"
#define W_BSS   "bss"
#define W_DATA  "data"
#define W_GLOBL "globl"
#define W_BYTE  "byte"
#define W_EVEN  "even"
#define W_COMM  "comm"
#define W_DOT2  "."
#define W_IF    "if"
#define W_ENDIF "endif"

char    *tabswords[] = {
    W_TEXT, W_BSS, W_DATA, W_GLOBL, W_BYTE, W_EVEN,
    W_COMM, W_DOT2, W_IF, W_ENDIF, 0     /* To avoid xstr bug - @VG */
};

int     tabswval[] = {
    LEX_TEXT,LEX_BSS,LEX_DATA,LEX_GLOBL,LEX_BYTE,LEX_EVEN,LEX_COMM,LEX_DOT2,
    LEX_IF,LEX_EIF
};

#define Xctable "t\011b\010n\012r\015\\\\>>s p\033e\0040"
char    ctable[] = Xctable;

#define CVT( x )        for( i = 0 ; i < sizeof ctable ; i += 2 )\
                  if( ctable[i] == x )\
                     x = ctable[i+1]

#define XSps    "*+-=()[]!&,$^%|"
char    Sps[] = XSps;

int     Splex[] = {
    LS_STAR, LS_PLUS, LS_MINUS, LS_EQ, LS_OPAR, LS_CPAR, LS_OBRA,
    LS_CBRA, LS_NOT, LS_AND, LS_COMMA, LS_DOLLAR, LS_STYPE,
    LS_MOD, LS_OR
};

/*
 * Собственно анализатор
 */
int
lex( value )
unsigned *value;
{
    extern FILE *infile;
    extern int  errcnt, Lbvals[], Xfps[], lavail;
    extern char Lbtypes[], Lftypes[];
    register char *p;
    register int  s;
    unsigned sc,dec,iddiag;
    int      i,ps;
    char    *q;
    int      lexq;

    p = str;
    *value = 0;
L0:
    while( (s = READC) == ' ' || s == '\t' );
    if( s == '\\' ) {
        switch( READC ) {
        case '/' :
            return( LS_DIV );
        case '<' :
            return( LS_LSHIFT );
        case '>' :
            return( LS_RSHIFT );
        case '!' :
            return( LS_OR );
        case '\n' :
            goto L0;
        }
        goto BADchar;
    }
    if( s == '/' )
        while( (s = READC) != '\n' && s != Seof );
    if( s == Seof )
        return( LEX_EOF );
    if( s == '\n' || s == ';' ) {
        *value = linecnt;
        if( s == '\n' )
            (*value)++;
        return( LEX_EOL );
    }
    if( s == ':' ) {
        msg( ediag( "bad label","плохая метка" ), 0 );
        goto Flush;
    }
    if( s == '<' ) {
        istrp = 0;
        goto L6;
    }
    if( (sc = sclass[s]) & SPEC ) {
        i = -1;
        while( s != Sps[++i] );
        return( Splex[i] );
    }
    if( sc & LET )
        goto L1;
    if( sc & DIG )
        goto L3;
    if( s == '"' || s == '\'' ) {
        ps = s;
        s = READC;
        if( s == Seof || s == '\n' ) {
            s = ' ';
            BACK;
        }
        if( s == '\\' ) {
            s = READC;
            if( s == Seof || s == '\n' ) {
                *value = (ps == '"')? (' ' | (' '<<8)) : ' ';
                BACK;
                return( LEX_NUM );
            }
            CVT( s );
        }
        sc = s & 0377;
        if( ps == '"' ) {
            s = READC;
            if( s == Seof || s == '\n' ) {
                s = ' ';
                BACK;
            }
            if( s == '\\' ) {
                s = READC;
                if( s == Seof || s == '\n' ) {
                    s = ' ';
                    BACK;
                }
                CVT( s );
            }
            sc |= (s&0377)<<8;
        }
        s = READC;
        BACK;
        if( (sclass[s] & (SPEC|EOW)) == 0 ) {
            msg( ediag( "syntax error in ' or \"",
                   "ошибка в синтаксисе ' или \"" ), 0 );
            goto Flush;
        }
        *value = sc;
        return( LEX_NUM );
    }
BADchar :
    if( badchflg == 0 ) {
        msg( ediag( "bad charachter in string",
               "плохой символ в строке" ), 0 );
        badchflg++;
    }
    goto Flush;

L1:
    *p++ = s;
    s = READC;
    if( sclass[s] & (LET | DIG) )
        goto L1;
    if( s == ' ' || s == '\t' )
        goto L2;
    if( s == ':' )
        goto Llid;
Lname:
    *p = 0;
    BACK;
    if( str[0] == '.' ) {
        if( str[1] == 0 )
            return( LEX_DOT );
        p = str+1;
        i = 0;
        while( q = tabswords[i++] )
            if( sequal( p, q ) )
                return( tabswval[i-1] );
    }

    lexq = LEX_NAME;
    *value = intsym( str, &lexq );
    return( lexq );

L2:
    while( (s = READC) == ' ' || s == '\t' );
Llid:
    if( s == ':' ) {
        *p = 0;

        lexq = LEX_LID;
        *value = intsym( str, &lexq );
        return( lexq );
    }
    goto Lname;

L3:
    *p++ = s;
    s = READC;
    if( s == 'b' || s == 'f' )
        goto L3_1;
    if( s == ' ' || s == '\t' )
        goto L4;
    if( sclass[s] & DIG )
        goto L5;
    if( s == ':' )
        goto Ldig;
    if( s == '.' ) {
        *p++ = '.';
        goto Lnumb1;
    }
    if( sclass[s] & LET )
        goto Numerr;
Lnumber:
    BACK;
Lnumb1:
    *p = 0;
    dec = 0;
    iddiag = 0;
    if(*(p-1) == '.' ) {
        dec++;
        *(p-1) = 0;
    }
    p = str;
    sc = 0;
    while( s = *p++ ) {
        if( dec )
            sc = sc*10 + (s-'0');
        else {
            if( (sclass[s] & DDIG) && iddiag == 0 ) {
                msg(ediag( "decimal digit in octal number",
                    "десятичная цифра в восьмеричном числе" ),0);
                iddiag++;
            }
            sc = (sc<<3) + (s-'0');
        }
    }
    *value = sc;
    return( LEX_NUM );

Numerr:
    msg( ediag( "bad number","плохое число" ), 0 );
    errcnt++;
    goto Flush;

L3_1:
    *p = s;
    s = READC;
    BACK;
    if( (sclass[s] & (SPEC|EOW)) == 0 ) {
        msg( ediag( "bad local label",
               "плохая локальная метка" ), 0 );
        errcnt++;
        goto Flush;
    }
    *value = sc = str[0]-'0';
    if( str[1] == 'b' ) {
        *value = Lbvals[sc];
        switch( Lbtypes[sc] ) {
        case TEXT :
            return( LEX_LDT );
        case DATA :
            return( LEX_LDD );
        case BSS :
            return( LEX_LDB );
        case UNDEF :
            msg( ediag( "undefined local label",
                   "неопределенная локальная метка" ), 0 );
            return( LEX_ERR );
        }
        msg( "IE01", 0 );
        exit( 1 );
    }
    if( Xfps[sc] == -1 ) {
        Xfps[sc] = lavail;
        if( ++lavail >= DLABS ) {
            msg( ediag( "local labels table overflow",
                   "переполнение таблицы локальных меток" ), 0 );
            exit( 1 );
        }
    }
    *value = Xfps[sc];
    Lftypes[*value] = UNDEF;
    return( LEX_UDL );

L4:
    while( (s = READC) == ' ' || s == '\t' );
    if( s != ':' )
        goto Lnumber;
Ldig:
    *value = str[0]-'0';
    return( LEX_LDIG );

L5:
    *p++ = s;
    s = READC;
    if( sclass[s] & DIG )
        goto L5;
    if( s == '.' ) {
        *p++ = '.';
        goto Lnumb1;
    }
    if( sclass[s] & LET )
        goto Numerr;
    goto Lnumber;
L6:
    s = READC;
    if( s == '\\' ) {
        s = READC;
        if( s == '\n' )
            goto L6;
        CVT( s );
        goto Skip;
    }
    if( s == '>' ) {
        istring[istrp] = 0;
        *value = istrp;
        return( LEX_STR );
    }
    if( s == '\n' || s == Seof ) {
        msg( ediag( "nonterminated string",
               "незаконченная строка" ), 0 );
        goto Flush;
    }
Skip:
    istring[istrp++] = s;
    if( istrp >= MAXSTRING ) {
        msg( ediag( "string too long",
               "слишком длинная строка" ), 0 );
        goto Flush;
    }
    goto L6;

Flush:
    while( ( s = READC ) != '\n' && s != ';' );
    BACK;
    p = str;
    goto L0;
}

/*
 * Чтение одного символа
 * С проверками на конец текстового файла и установкой nextsym
 */
int
readc1()
{
    int     s;
    static  nlf = 0;
    extern FILE *infile;
    FILE    *openinf();
cy:
    if( eofflag )
        s = Seof;
    else if( infile == NULL || (s = getc( infile )) == EOF ) {
        if( (infile = openinf()) == NULL ) {
            s = Seof;
            eofflag++;
        } else
            goto cy;
    }
    if( nlf ) {
        nlf = 0;
        linecnt++;
    }
    s &= 0377;
    if( s == '\n' ) {
        badchflg = 0;
        nlf++;
    }
    nextsym = s;
    return( s );
}

/*
 * Быстрое сравнение строк
 */
int
sequal( p, q )
register char *p,*q;
{
    while( *p++ == *q )
        if( *q++ == 0 )
            return( 1 );
    return( 0 );
}
