/*
 * Ассемблер  вер. 1.0
 *    ЭВМ СМ-4   МНОС РЛ 1.2
 *    Автор: Антонов В.Г. ( ВМК МГУ, ИПК Минавтопрома )
 *    Последнее изменение: 21.12.1985
 */

#include <stdio.h>
/** #include <misc.h> **/
#include <ediag.h>
#include "as.h"

/*
 * Первый проход
 */

char    Ebfs[256];      /* Буфер для аргуменов команды */
int     Ebfp = 0;

int     Lfvals[DLABS];  /* Значение цифр. f-меток */
char    Lftypes[DLABS]; /* Тип цифр. f-меток */
int     Xfps[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
            /* Номер цифр. f-метки */
int     lavail = 0;     /* Указатель на своб. цифровую метку */
int     Lbvals[10];     /* Значение цифр. b-меток */
char    Lbtypes[10];    /* Тип цифр. b-меток */

/*
 * Главный цикл первого прохода
 */
pass1()
{
    int     clex, cval, i, j, k;
    extern int Segflg, errcnt, linecnt;
    extern int address[];
    extern struct STAB tabsym[], Ctable[];
    extern char istring[], *Lclass;
    int     eval, etype;
    int     words;
    char    *p;
    extern char UBRtype[];
    extern int  UBRaddr[], UBReval[], uavail;

again:
    while( (clex = getlex( &cval )) != LEX_EOF ) {
        switch( clex ) {
        case LEX_LID :
            if( tabsym[cval].type&07 ) {
                eval = tabsym[cval].type;
                tabsym[cval].type = 0;
                msg( ediag( "'%' - multiply defined",
                    "'%' - уже определено" ), tabsym[cval].name);
                tabsym[cval].type = eval;
            } else {
                tabsym[cval].type |= Segflg+2;
                tabsym[cval].value = address[Segflg];
            }
            goto again;
        case LEX_LDIG :
            Lbvals[cval] = address[Segflg];
            Lbtypes[cval] = Segflg+2;
            if( (eval = Xfps[cval]) != -1 ) {
                Lfvals[eval] = address[Segflg];
                Lftypes[eval] = Segflg+2;
                Xfps[cval] = -1;
            }
            goto again;
        case LEX_TEXT :
            Segflg = 0;
            break;
        case LEX_DATA :
            Segflg = 1;
            break;
        case LEX_BSS :
            Segflg = 2;
            break;
        case LEX_EVEN :
            if( address[Segflg] & 01 ) {
                if( Segflg != 2 )
                    wrilex( LR_SKIP, 1 );
                address[Segflg]++;
            }
            break;
        case LEX_GLOBL :
            do {
                if( (clex = getlex( &cval )) == LEX_NAME )
                    tabsym[cval].type |= EXTERN;
                else if( clex == LEX_COP ) {
                    i = LEX_ERR;
                    cval = intsym( Ctable[cval].name, &i );
                    tabsym[cval].value = 0;
                    tabsym[cval].type = UNDEF | EXTERN;
                } else {
                    if( clex == LEX_EOL )
                        break;
                    goto Globerr;
                }
                if( (clex = getlex( &cval ) ) != LS_COMMA &&
                     clex != LEX_EOL )
                    goto Globerr;
            } while( clex != LEX_EOL );
            backlex;
            break;
        case LEX_COMM :
            if( (getlex( &i )    != LEX_NAME ) ||
                (getlex( &cval ) != LS_COMMA) ) {
                p = ediag( "bad .comm args",
                      "плохие аргументы .comm" );
                goto ERROK;
            }
            tabsym[i].value = expr( &cval );
            if( (cval & 07) != ABS )
                goto Btype;
            tabsym[i].type = UNDEF | EXTERN;
            break;
        case LEX_BYTE :
            if( Segflg == 2 )     /* BSS */
                goto BSSconst;
            do {
                Ebfp = 0;
                wrilex( LR_BYTE, 0 );
                address[Segflg]++;
                if( cpyarg() == 0 ||
                  ( (clex = getlex( &cval )) != LEX_EOL &&
                     clex != LS_COMMA ) ) {
                    p = ediag( "bad .byte args",
                          "плохие аргументы .byte" );
                    goto ERROK;
                }
                wricons( Ebfs, Ebfp );
            } while( clex != LEX_EOL );
            backlex;
            break;
        case LEX_DOT2 :
            address[Segflg] += 2;
            if( Segflg != 2 )
                wrilex( LEX_NUM, 0 );
            break;
        case LEX_STR :
            if( Segflg == 2 )     /* BSS */
                goto BSSconst;
            wrilex( LEX_STR, cval );
            wricons( istring, cval );
            address[Segflg] += cval;
            break;
        case LEX_EIF :
            break;
        case LEX_IF :
            cval = expr( &i );
            if( (i&07) != ABS )
                goto Btype;
            if( cval == 0 ) {
                i = 1;
                do {
                    clex = getlex( &cval );
                    if( clex == LEX_IF )
                        i++;
                    else if( clex == LEX_EIF )
                        i--;
                    else if( clex == LEX_EOF )
                        break;
                } while( i );
                if( i != 0 ) {
                    p = ediag( "missing .endif",
                          "нет .endif" );
                    goto ERROK;
                }
            }
            break;
        case LEX_DOT :
        case LEX_NAME :
            if( getlex( &i ) != LS_EQ ) {
                backlex;
                if( clex == LEX_NAME )
                    goto Name;
                goto Expr;
            }
            j = expr( &i );
            if( clex == LEX_NAME ) {
                tabsym[cval].type |= i;
                tabsym[cval].value = j;
            } else {
                if( ((i&ISX) != XPC  &&
                     (i&ISX) != XABS     ) ||
                     (i&07)  != (Segflg+2) )
                    goto Btype;
                if(((unsigned)j)<((unsigned)address[Segflg])){
                    p =  ediag(
                   "negative ddress counter increment",
                   "отрицательное приращение счетчика адреса" );
                    goto ERROK;
                }
                if( Segflg != 2 )       /* Not BSS segment */
                    wrilex( LR_SKIP, j-address[Segflg] );
                address[Segflg] = j;
            }
            break;
Name:
            if( (i = (tabsym[cval].type & ISX)) && i != XPC )
                goto Command;
            /*** FALL TROUGH ***/
        case LS_PLUS :
        case LS_MINUS:
        case LS_NOT  :
        case LS_OBRA :
        case LEX_NUM :
        case LEX_UDL :
        case LEX_LDD :
        case LEX_LDT :
        case LEX_LDB :
Expr:
            if( Segflg == 2 )     /* BSS */
                goto BSSconst;
            wrilex( clex, cval );
            while( ( j = getlex( &i )) != LEX_EOL &&
                 j != LEX_EOF )
                wrilex( j, i );
            address[Segflg] += 2;
            backlex;
            break;

        case LEX_COP :
            if( getlex( &eval ) == LS_EQ ) {
                i = LEX_ERR;
                eval = intsym( Ctable[cval].name, &i );
                tabsym[eval].value = expr( &etype );
                tabsym[eval].type = etype;
                break;
            }
            backlex;
            if( Ctable[cval].type == XABS )
                goto Expr;
Command:
            if( Segflg == 2 )     /* BSS */
                goto BSSconst;
            i = (clex == LEX_NAME) ? tabsym[cval].type :
                         Ctable[cval].type ;
            j = (clex == LEX_NAME) ? tabsym[cval].value:
                         Ctable[cval].value;
            words = 0;
            Ebfp = 0;
            i &= ISX;
            switch( i ) {
            case XJSR :
            case XFRD :
            case XSOB :
            case XXOR :
                if( (etype = getlex( &eval )) == LEX_COP &&
                     Ctable[eval].type == XREG )
                    etype = Ctable[eval].value;
                else if( etype == LEX_NAME &&
                     tabsym[eval].type == (XREG | ABS) )
                    etype = tabsym[eval].value;
                else
                    goto BadReg;
                if( etype > ((i==XFRD)?3:7) )
                    goto BadReg;
                if( getlex( &eval ) != LS_COMMA ||
                   ((words = cpyarg())==0 && i == XSOB) )
                    goto BadArg;
                if( i == XSOB )
                    words = 0;
                j |= etype<<6;
                break;
            case XBR  :
            case XSYS :
                if( cpyarg() == 0 )
                    goto BadArg;
                break;
            case XSR  :
                if( (etype = getlex( &eval )) == LEX_COP &&
                     Ctable[eval].type == XREG )
                    etype = Ctable[eval].value;
                else if( etype == LEX_NAME &&
                     tabsym[eval].type == (XREG | ABS) )
                    etype = tabsym[eval].value;
                else
                    goto BadReg;
                j |= etype;
                break;
            case XFSR :
            case XEIS :
                words = cpyarg();
                if( getlex( &eval ) != LS_COMMA )
                    goto BadArg;
                if( (etype = getlex( &eval )) == LEX_COP &&
                     Ctable[eval].type == XREG )
                    etype = Ctable[eval].value;
                else if( etype == LEX_NAME &&
                     tabsym[eval].type == (XREG | ABS) )
                    etype = tabsym[eval].value;
                else
                    goto BadReg;
                if( etype > ((i==XEIS)?7:3) )
                    goto BadReg;
                j |= etype<<6;
                break;
            case XSOP :
            case XSOS :
                words = cpyarg();
                break;
            case XNOP :
                break;
            case XDDD :
            case XDOP :
            case XMVF :
                words = cpyarg();
                if( getlex( &eval ) != LS_COMMA )
                    goto BadArg;
                Ebfs[Ebfp++] = LS_COMMA;
                words += cpyarg();
                break;
            case XJBC :
            case XJBR :
                if( Segflg ) {
                    p = ediag( "bad command in data",
                        "плохая команда в сегменте данных" );
                    goto ERROK;
                }

/* Проверить, является ли аргумент именем */
                if( (etype = getlex( &eval )) != LEX_NAME &&
                     etype != LEX_LDT && etype != LEX_UDL )
                    goto Lbr;
                if( getlex( &k ) != LEX_EOL ) {
                    backlex;
                    goto Lbr;
                }
                backlex;      /* Back EOL */

/* Если имя уже определено, вычислить смещение */
                switch( etype ) {
                case LEX_NAME:
                    if( tabsym[eval].type == TEXT )
                        k = tabsym[eval].value -
                            (address[0]+2);
                    else if( tabsym[eval].type == UNDEF )
                        goto Unbr;
                    else
                        goto BadAdr;
                    if( k > 254 || k < -256 )
                        goto Lbr;
                    goto Sbr;
                case LEX_LDT:
                    k = eval - (address[0]+2);
                    if( k > 254 || k < -256 )
                        goto Lbr;
Sbr:
/* Короткий переход, сгенерировать код */
                    if( k & 01 )
                        goto ODDa;
                    Ebfs[0] = etype;
                    Ebfs[1] = eval;
                    Ebfs[2] = (eval>>8);
                    Ebfp = 3;
                    words = 0;
                    i = XBR;
                    break;

                case LEX_UDL:
Unbr:
/* Нельзя определить группу перехода, отложить до 1.5-го и 2-го прохода */
                    if( uavail >= MAXUBR )
                        goto Lbr;
                    words = 1;
                    UBRaddr[uavail] = address[0]+2;
                    UBRtype[uavail] = etype;
                    if( i == XJBC ) {
                        UBRtype[uavail] |= 0100;
                        words++;
                    }
                    UBReval[uavail++] = eval;
                }
                break;
Lbr:
/* Длинный переход, сгенерировать коды */
                backlex;               /* Back arg */
                words = cpyarg();
                if( i != XJBR ) {
                    address[0] += 2;
                    wrilex( LR_COM, XNOP );
                    eval = (j^NOBR) | (words+1);
                     /*cond.branch COP*/
                    putcons( eval );    putcons( eval>>8 );
                    wrilex( LEX_EOL, linecnt );
                }
                wrilex( LR_COM, XNOP );
                eval = JMPCODE | 037;
                 /*long jump COP*/
                putcons( eval );    putcons( eval>>8 );
                wrilex( LEX_EOL, linecnt );
                wricons( Ebfs, Ebfp );
                Ebfp = 0;
                address[0] += 4;
                goto ENDS;

            case XREG :
                backlex;
                goto Expr;
            default :
                p = ediag( "bad command",
                      "плохая команда" );
                goto ERROK;
            }
            if( address[Segflg] & 01 )
                goto ODDa;
            address[Segflg] += (words+1)*2;
            wrilex( LR_COM, i );
             /*Запись COP'а*/
            putcons( j );    putcons( j>>8 );
            wricons( Ebfs, Ebfp );
            Ebfp = 0;
            break;
        case LEX_EOL:
            backlex;
            break;
        default:
            p = ediag( "bad command head",
                  "плохое начало команды" );
            goto ERROK;
        }
ENDS:
        if( getlex( &eval ) != LEX_EOL ) {
            p = ediag( "bad command tail",
                  "плохой конец команды" );
            goto ERROK;
        }
        if( Segflg != 2 )       /* BSS */
            wrilex( LEX_EOL, eval );
        goto SKP1;
ODDa:
        p = ediag( "odd address",
              "нечетный адрес" );
        goto ERROK;
BadAdr:
        p = ediag( "bad address",
              "плохой адрес" );
        goto ERROK;
BadArg:
        p = ediag( "bad argument",
              "плохой аргумент команды" );
        goto ERROK;
BadReg:
        p = ediag( "bad register",
              "плохой регистр" );
        goto ERROK;
BSSconst:
        p = ediag( "contant or command in bss",
              "константа или команда в сегменте bss" );
        goto ERROK;
Globerr:
        p = ediag( "bad name list",
              "плохой список идентификаторов" );
        goto ERROK;
Btype:
        p = ediag( "bad expr type",
              "плохой тип выражения" );
ERROK:
        flushlex();
        msg( p, 0 );
SKP1:           ;
    }
/* END OF MAIN LOOP */

    /*Маркировка EOF для работы через sqopen*/
    Segflg = 0;
    wrilex( LEX_EOF, 0 );
    Segflg = 1;
    wrilex( LEX_EOF, 0 );
}

/*
 * Скопировать в буфер аргументы команды
 *      Выдает 1, если аргумент содержит смещение
 */
int
cpyarg()
{
    int     cl, cv;
    int     res;
    extern struct STAB Ctable[], tabsym[];
    extern char LexL[];
    int     sf, mf, bcl;

    res = 0;
    sf = 0;
    mf = 0;
AGAIN1:
    cl = getlex( &cv );
    switch( cl ) {
    case LS_STAR :
        Ebfs[Ebfp++] = LS_STAR;
        sf++;
        goto AGAIN1;
    case LS_OPAR :
        goto FLU;
    case LS_MINUS :
        Ebfs[Ebfp++] = LS_MINUS;
        mf++;
        goto AGAIN1;
    case LEX_COP :
        mf = 0;
        if( Ctable[cv].type == XREG )
            goto FLU;
        break;
    case LEX_NAME :
        mf = 0;
        if( tabsym[cv].type == (XREG|ABS) )
            goto FLU;
        break;
    case LEX_NUM :
        mf = 0;
    }
    mf = 0;
    res = 1;
FLU:
    bcl = 0;
    do {
        Ebfs[Ebfp++] = cl;
        if( LexL[cl] == 3 ) {
            Ebfs[Ebfp++] = cv;
            Ebfs[Ebfp++] = (cv>>8);
        }
        bcl = cl;
        cl = getlex( &cv );
    } while( cl != LEX_EOL && cl != LEX_EOF && cl != LS_COMMA );
    backlex;
    if( sf && !mf && bcl == LS_CPAR )
        res = 1;
    return( res );
}
