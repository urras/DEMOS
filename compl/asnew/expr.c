#
/*
 * Ассемблер  вер. 1.0
 *    ЭВМ СМ-4   МНОС РЛ 1.2
 *    Автор: Антонов В.Г. ( ВМК МГУ, ИПК Минавтопрома )
 *    Последнее изменение: 15.06.1985
 */

#include <stdio.h>
#include <ediag.h>
#include "as.h"

int      secnd;            /* Флаг - установлен для 2-го прохода */
int      address[3];       /* Счетчики смещений сегментов text, data, bss */
int      Segflg = 0;       /* Текущий сегмент text/data/bss */
int      elevel = 0;       /* Уровень вложенности [ ] */
int      blex[2] ,bval[2], bdval[2]; /* Пред. лексема - для возврата */
int      blflg = 0;        /* Флаг - был возврат лексемы */
extern int linecnt;
int      dval;             /* Доп. значение лексемы - pass2, LR_COM */
int      nmval;            /* Номер в табл. символов глоб. неопр. имени */
int      oldaddress;       /* Адрес старой . */

#define OP      01
#define UNOP    03
#define EOE     04
char    XLclass[] = {
    0,                             /* ERROR   */
    0, 0, 0, 0, 4, 4, 0, 0, 0, 0,  /*  0 -  9 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /* 10 - 19 */
    1, 3, 3, 0, 4, 0, 0, 4, 3, 1,  /* 20 - 29 */
    4, 0, 1, 1, 1, 1, 1, 1, 0, 0,  /* 30 - 39 */
    4, 0, 0, 0                     /* 40 - 44 */
};
char    *Lclass = &XLclass[1];
FILE    *Wfile;                 /* Вх. файл для чтения выражений */

/*
 * Вычислить значение выражения
 */
int
expr( type )
int     *type;
{
    int     clex;
    int     cval;
    int     Res, Nflg;
    int     Fop, Fopt;
    int     Op, Uop;
    register int i;
    int     Loprnd;
    extern struct STAB tabsym[], Ctable[];
    extern int  errcnt, Lfvals[], uflag;
    extern char Lftypes[];
    extern unsigned Dshift, Bshift;

    *type = UNDEF;
    Nflg = 0;
    Res = 0;
    Op = 0;
    Loprnd = 0;
    Uop = 0;
Loop:
    clex = getlex( &cval );
    if( (i = Lclass[clex])&OP ) {
        if( Op || !Nflg ) {
            if( (i&UNOP) && !Uop ) {
                   Uop = clex;
                   goto Loop;
            }
            goto ERROR;
        }
        Op = clex;
        Fop = Res;
        Fopt = *type;
        Loprnd = 0;
        goto Loop;
    }
    switch( clex ) {
    case LEX_NAME :
        Res = tabsym[cval].value;
        if( ((*type = tabsym[cval].type)&(07|EXTERN)) == UNDEF ) {
            tabsym[cval].name[8] = 0;
            msg( ediag( "'%' undefined",
                   "'%' неопределено" ), tabsym[cval].name );
            tabsym[cval].type = *type;
            *type = UNDEF;
            elevel = 0;
            return( 0 );
        } else if( (*type&07) == UNDEF ) {
            nmval = cval;
            Res = 0;
        }
        goto COMP;
    case LEX_UDL :
        Res = Lfvals[cval];
        *type = secnd ? Lftypes[cval] : UNDEF;
        goto XCOMP;
    case LEX_DOT :
        *type = (Segflg+2) | XPC;
        if( !secnd ) {
            Res = address[ Segflg ];
            goto COMP;
        }
        Res = oldaddress;
XCOMP:
        switch( *type & ~ISX ) {
        case UNDEF :
            msg( ediag( "undefined local label",
                   "неопределенная локальная метка" ), 0 );
        case DATA :
            Res += Dshift;
            break;
        case BSS :
            Res += Bshift;
        }
        goto COMP;
    case LEX_ERR :
        *type = UNDEF;
        return( 0 );
    case LEX_LDT :
        *type = TEXT;
        Res = cval;
        goto COMP;
    case LEX_LDD :
        *type = DATA;
        Res = cval+Dshift;
        goto COMP;
    case LEX_LDB :
        *type = BSS;
        Res = cval+Bshift;
        goto COMP;
    case LEX_NUM :
        Res = cval;
        *type = ABS;
        goto COMP;
    case LEX_COP :
        Res = Ctable[cval].value;
        *type = Ctable[cval].type | ABS;
        goto COMP;
    case LEX_DOT2 :
        Res = 0;
        *type = ABS;
        goto COMP;
    case LS_OBRA :
        elevel++;
        Res = expr( type );
        if( *type == UNDEF ) {
            elevel = 0;
            return( 0 );
        }
        if( getlex( &cval ) != LS_CBRA )
            goto ERROR;
        goto COMP;
    }
    if( i & EOE ) {
        backlex;
        if( (*type & 07) == ABS )
            *type &= ~INVERTED;
        if( !elevel && (*type & INVERTED) ) {
            msg( ediag( "result of an inverted type",
                   "результат - инвертированный тип" ), 0 );
            goto ERRX;
        }
        if( (Op || Uop) && (uflag == 0) )
            goto ERROR;
        if( clex != LS_CBRA ) {
            if( elevel != 0 )
                goto ERROR;
        } else if( elevel <= 0 )
            goto ERROR;
        else
            elevel--;
        if( !Nflg ) {
            msg( ediag( "null expression",
                   "пустое выражение" ), 0 );
            goto ERRX;
        }
        return( Res );
    }
ERROR:
    msg( ediag( "syntax error in expression",
           "синтаксическая ошибка в выражении" ), 0 );
ERRX:
    *type = UNDEF;
    backlex;
    elevel = 0;
    return( 0 );
COMP:
    Nflg++;
    if( Loprnd++ )
        goto ERROR;
    if( Uop ) {
        switch( Uop ) {
        case LS_MINUS :
            *type ^= INVERTED;
            Res = -Res;
            break;
        case LS_PLUS :
            break;
        case LS_NOT :
            if( (*type&07) != ABS )
                goto BTYPE;
            Res = ~Res;
        }
        Uop = 0;
    }
    if( Op ) {
        switch( Op ) {
        case LS_MINUS :
            if( (Fopt&07) == (*type&07) && (Fopt&07) )
                *type = ABS;
            else if( (Fopt&07) == ABS )
                *type ^= INVERTED;
            else if( (*type&07) == ABS )
                *type = Fopt;
            else
                goto BTYPE;
            Res = Fop-Res;
            goto sktc;
        case LS_PLUS :
            if( (Fopt ^ *type ) == INVERTED && (Fopt & 07) )
                *type = ABS;
            else if( (*type&07) == ABS )
                *type = Fopt;
            else if( (Fopt&07) != ABS )
                goto BTYPE;
            Res = Fop+Res;
            goto sktc;
        case LS_STYPE :
            Res = Fop;
            goto sktc;
        }
        if( (*type&07) != ABS || (Fopt&07) != ABS )
            goto BTYPE;
        *type |= Fopt;
        switch( Op ) {
        case LS_STAR :
            Res = Fop * Res;
            break;
        case LS_DIV :
            Res = Fop / Res;
            break;
        case LS_OR :
            Res = Fop | Res;
            break;
        case LS_AND :
            Res = Fop & Res;
            break;
        case LS_NOT :
            Res = Fop | ~Res;
            break;
        case LS_MOD :
            Res = Fop % Res;
            break;
        case LS_RSHIFT :
            Res = Fop >> Res;
            break;
        case LS_LSHIFT :
            Res = Fop << Res;
        }
sktc:
        Op = 0;
    }
    goto Loop;
BTYPE:
    msg( ediag( "operands type clash",
           "несовместимы типы операндов" ), 0 );
    goto ERRX;
}

/*
 * Выдать очередную лексему
 */
int
getlex( vl )
int     *vl;
{
    extern char LexL[];
    register  s,w;

    if( blflg ) {
        blflg--;
        *vl = bval[blflg];
        dval = bdval[blflg];
    } else if( secnd ) {
        blex[1] = blex[0];
        bval[1] = bval[0];
        if( (s = getc( Wfile )) == EOF ) {
            *vl = bval[0] = 0;
            return( blex[0] = LEX_EOF );
        }
        w = 0;
        switch( LexL[s] ) {
        case 3:
            w  = (getc( Wfile )&0377);
            w |= (getc( Wfile ))<<8;
        case 1:
            break;
        case 5:
            w     = (getc( Wfile )&0377);
            w    |= (getc( Wfile ))<<8;
            dval  = (getc( Wfile )&0377);
            dval |= (getc( Wfile ))<<8;
            break;
        default:
            msg( "IE03", 0 );
            exit( 1 );
        }
        bdval[1] = bdval[0];
        bval[0] = w;
        bdval[0] = dval;
        *vl = w;
        return( blex[0] = s );
    } else {
        blex[1] = blex[0];
        bval[1] = bval[0];
        blex[0] = lex( bval );
        *vl = bval[0];
    }
    return( blex[blflg] );
}

/*
 * Пропустить лексемы до LEX_EOL
 */
flushlex()
{
    int     cv, cl;

    blflg++;
    while( (cl = getlex( &cv )) != LEX_EOL && cl != LEX_EOF );
    blflg++;
    if( cl == LEX_EOF ) {
        blex[0] = LEX_EOL;
        bval[0] = linecnt;
    }
}
