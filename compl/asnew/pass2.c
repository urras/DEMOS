#
/*
 * Ассемблер  вер. 1.0
 *    ЭВМ СМ-4   МНОС РЛ 1.2
 *    Автор: Антонов В.Г. ( ВМК МГУ, ИПК Минавтопрома )
 *    Последнее изменение: 19.04.1985
 */

#include <stdio.h>
#include <ediag.h>
#include "as.h"

/*
 * Проход 2
 */
#define MAGIC1   007    /* Magic value for a.out */
#define MAGIC2   001

extern FILE *outfile;

#define putw(x,f) {  putc((x),f); putc((x)>>8,f); }

/*
 * Вывести заголовок файла
 */
wrhead()
{
    extern int address[];
    extern int avail;
    register i;

    putc( MAGIC1, outfile );
    putc( MAGIC2, outfile );
    putw( (address[0]+1) & ~01, outfile );  /* Сделать длины сегментов */
    putw( (address[1]+1) & ~01, outfile );  /* четными                 */
    putw( (address[2]+1) & ~01, outfile );
    putw( avail*12, outfile );              /* Каждый элемент - 6 слов */
    for ( i = 6 ; i ; i-- )
        putc( 0, outfile );
}

/*
 * Запись сегмента текста/данных
 */
pass2( fil )
FILE    *fil;
{
    register     cl;
    int          eval, cv, etype, caddr, ubrcnt, el, ev;
    extern FILE *datafile, *Wfile, *outfile;
    extern char  UBRtype[];
    extern int   linecnt, errcnt, blflg;
    extern int   Segflg, UBReval[];
    extern int   address[], dval, Lfvals[];
    extern char  Lftypes[];
    extern struct STAB Ctable[], tabsym[];
    int          word[2], wt[2], words;
    extern FILE *relocf;
    char        *p;
    extern unsigned Dshift;
    extern int   oldaddress;
    extern int   nmval;     /* Значение - номер глобального неопрделенного
                   имени в таблице символов, уст. expr */

    blflg = 0;      /* For any case ... */
    Segflg = ( fil == datafile );
    Wfile = fil;
    caddr = address[Segflg];
    address[Segflg] = 0;
    ubrcnt = 0;
    linecnt = 1;
MLOOP:
    while( (cl = getlex( &cv )) != LEX_EOF ) {
        oldaddress = address[Segflg];
        switch( cl ) {
        case LEX_EOL :
            linecnt = cv;
            goto MLOOP;
        case LR_BYTE :
Getbyte:
            eval = expr( &etype );
            if( (etype&07) != ABS )
                msg( ediag( "bad type for .byte argument",
                       "плохой тип для аргумена .byte" ), 0 );
            putc( eval, outfile );
            if( (++address[Segflg] & 01) == 0 ) {
                putc( RABS, relocf );
                putc( 0, relocf );
            }
            if( getlex( &ev ) == LR_BYTE )
                goto Getbyte;
            backlex;
            break;
        case LEX_STR :
            while( cv-- ) {
                putc( getc( fil ), outfile );
                if( (++address[Segflg] & 01) == 0 ) {
                    putc( RABS, relocf );
                    putc( 0, relocf );
                }
            }
            break;
        case LR_SKIP :
            while( cv-- ) {
                putc( 0, outfile );
                if( (++address[Segflg] & 01) == 0 ) {
                    putc( RABS, relocf );
                    putc( 0, relocf );
                }
            }
            break;
        case LEX_NAME :
        case LEX_NUM :
        case LS_MINUS :
        case LS_PLUS :
        case LS_OBRA :
        case LS_NOT :
        case LEX_DOT :
        case LEX_DOT2 :
        case LEX_LDD :
        case LEX_LDT :
        case LEX_LDB :
        case LEX_UDL :
        case LEX_COP :
            backlex;
            eval = expr( &etype );
            address[Segflg] += 2;
            if( address[Segflg] & 01 )
                goto ODD;
            putw( eval, outfile );
            if( etype == (EXTERN|UNDEF) ) {
                putw( REXTRN|RNSYM( nmval ), relocf );
            } else
                putw( ((etype-1)<<1)&06 , relocf );
            break;
ODD:
            msg( ediag( "odd address", "нечетный адрес" ), 0 );
            break;
        default :
            msg( "IE10", 0 );
            exit( 1 );
        case LR_COM :
/* Command: */
            address[Segflg] += 2;
            if( address[Segflg] & 01 )
                goto ODD;
            cv &= ISX;
            words = 0;
            switch( cv ) {
            case XNOP :
            case XSR :
                putw( dval, outfile );
                putc( RABS, relocf );
                putc( 0, relocf );
                break;
            case XSYS :
                eval = expr( &etype );
                if( (etype&07) != ABS )
                    goto BadType;
                putw( dval | (eval&0377), outfile );
                putc( RABS, relocf );
                putc( 0, relocf );
                break;
            case XBR :
            case XSOB :
                eval = expr( &etype );
                if( (etype&07) != (Segflg+2) )
                    goto BadType;
                if( eval & 01 )
                    goto ODD;
                eval -= address[Segflg];
                if( Segflg )
                    eval -= Dshift;
                if( (cv == XBR)? (eval < -256 || eval > 254)
                           : (eval < -254 || eval > 0 ) )
                    msg( ediag( "offset too big",
                         "слишком большое смещение" ), 0 );
                if( cv == XSOB )
                    eval = -eval;
                eval = (eval>>1)&0377;
                putw( dval|eval, outfile );
                putc( RABS, relocf );
                putc( 0, relocf );
                break;
            case XJBR :
            case XJBC :
                eval = ( (UBRtype[ubrcnt]&077) == LEX_NAME ) ?
                        tabsym[UBReval[ubrcnt]].value :
                        Lfvals[UBReval[ubrcnt]];
                if( UBRtype[ubrcnt] & 0200 ) { /* Это br */
                    dval |= ((eval - address[Segflg]
                               - (Segflg? Dshift: 0)
                                  )>>1)&0377;
                    putw( dval, outfile );
                    putc( RABS, relocf );
                    putc( 0, relocf );
                } else {
                    if( cv == XJBC ) {
                        putw( (dval^NOBR)+2, outfile );
                        putc( RABS, relocf );
                        putc( 0, relocf );
                        address[Segflg] += 2;
                    }
                    putw( JMPCODE | 037, outfile );
                    putc( RABS, relocf );
                    putc( 0, relocf );
                    address[Segflg] += 2;
                    putw( eval, outfile );
                    putc( Segflg? RDATA: RTEXT, relocf );
                    putc( 0, relocf );
                }
                ubrcnt++;
                break;
            case XSOP :
            case XSOS :
            case XFRD :
            case XMVF :
            case XFSR :
            case XEIS :
            case XDOP :
            case XDDD :
            case XJSR :
            case XXOR :
#define IADDR   010
#define IMMED   027
#define RADDR   060
#define AINCR   020
#define ADECR   040
#define PC      07

Getoprnd:
                el = 0;         /* Тип адресации */
CAGA:
                switch( getlex( &ev ) ) {
                case LS_STAR :
                    if( el )
                        goto BadArg;
                    el |= IADDR;
                    goto CAGA;
                case LS_DOLLAR :
                    if( el & ~IADDR )
                        goto BadArg;
                    el |= IMMED;
                    goto GEXP;
                case LS_MINUS :
                    if( getlex( &ev ) != LS_OPAR ) {
                        backlex;
                        goto Getexpr;
                    }
                    el |= ADECR;
                case LS_OPAR :
                    switch( getlex( &ev ) ) {
                    case LEX_COP :
                        if( Ctable[ev].type != XREG )
                            goto BadReg;
                        el |= Ctable[ev].value;
                        break;
                    case LEX_NAME :
                        if((tabsym[ev].type&ISX)!=XREG)
                            goto BadReg;
                        ev = tabsym[ev].value;
                        if( ev < 0 || ev > 7 )
                            goto BadReg;
                        el |= ev;
                        break;
                    default :
                        goto BadReg;
                    }
                    if( getlex( &ev ) != LS_CPAR )
                        goto BadReg;
                    switch( getlex( &ev ) ) {
                    case LS_PLUS :
                        if( el & ADECR )
                            goto BadArg;
                        el |= AINCR;
                        break;
                    case LEX_EOL :
                    case LS_COMMA :
                        backlex;
                        break;
                    default :
                        goto BadArg;
                    }
                    if((el&(IADDR|AINCR|ADECR)) == IADDR) {
                        word[words] = 0;
                        wt[words] = RABS;
                        words++;
                        address[Segflg] += 2;
                        el |= RADDR;
                    } else if( (el&(RADDR|IADDR)) == 0 )
                        el |= IADDR;
                    goto EOC;

                case LEX_NAME :
                    if( (tabsym[ev].type&ISX) == XREG ) {
                        if( (etype = getlex( &eval ))
                               == LEX_EOL ||
                             etype == LS_COMMA ) {
                            backlex;
                            ev = tabsym[ev].value;
                            goto REG;
                        }
                        backlex;
                    }
                    goto Getexpr;

                case LEX_COP :
                    if( Ctable[ev].type != XREG )
                        goto Getexpr;
                    if( (etype = getlex( &eval ))
                           != LEX_EOL &&
                         etype != LS_COMMA ) {
                        backlex;
                        goto Getexpr;
                    }
                    backlex;
                    ev = Ctable[ev].value;
REG:
                    if( ev < 0 || ev > 7 )
                        goto BadReg;
                    el |= ev;
                    goto EOC;

                case LS_NOT :
                case LS_OBRA :
                case LS_PLUS :
                case LEX_NUM :
                case LEX_UDL :
                case LEX_LDT :
                case LEX_LDD :
                case LEX_LDB :
                case LEX_DOT :
                case LEX_DOT2 :
Getexpr:
                    backlex;
GEXP:
                    address[Segflg] += 2;
                    eval = expr( &etype );
                    if( getlex( &ev ) == LS_OPAR ) {
                        switch( getlex( &ev ) ) {
                        case LEX_COP :
                            if(Ctable[ev].type != XREG)
                                goto BadReg;
                            el |= Ctable[ev].value;
                            break;
                        case LEX_NAME :
                            if( (tabsym[ev].type&ISX)
                                      != XREG)
                                goto BadReg;
                            ev = tabsym[ev].value;
                            if( ev < 0 || ev > 7 )
                                goto BadReg;
                            break;
                        default :
                            goto BadReg;
                        }
                        if( getlex( &ev ) != LS_CPAR )
                            goto BadReg;
                        el |= RADDR;
                    } else {
                        backlex;
                        if( (el & IMMED) == 0 )
                            el |= RADDR|PC;
                    }
                    word[words] = eval;
                    if( etype&07 )
                          wt[words] = ((etype&07)-1)<<1;
                    else
                          wt[words] = REXTRN|RNSYM(nmval);
                    if( (el&(RADDR|PC)) == (RADDR|PC) ) {
                        word[words] -= address[Segflg];
                        if( Segflg )
                            word[words] -= Dshift;
                        wt[words] |= REL_PC;
                    }
                    words++;
EOC:
                    if( cv == XDOP || cv == XDDD ||
                        cv == XMVF ) {
                        if( getlex( &ev ) != LS_COMMA )
                            goto BadArg;
                        cv |= 01;
                        dval |= (el<<6) & 07700;
                        goto Getoprnd;
                    }
                    if( el == 027 && cv != XEIS &&
                        cv != (XDDD|01) && cv != XJSR &&
                        cv != XFSR && cv != XSOS &&
                        !Segflg ) {
                        p = ediag(
                 "dst with an immediate mode",
                 "непосредственный аргумент - приемник" );
                        goto ER;
                    }
                    el &= 077;
                    dval |= el;
                    if( cv == (XMVF|01) ) {
                        if( (dval&07000) == 0 &&
                            (ev = dval & 0700) < 0400) {
                            dval = STFCODE|ev|el;
                        } else if( (dval&070) == 0 ) {
                            ev = (dval&07)<<6;
                            el = (dval>>6) & 077;
                            dval = LDFCODE|ev|el;
                        } else {
                            p = ediag(
                  "bad movf operands modes",
                  "плохое сочетание типов аргументов movf" );
                            goto ER;
                        }
                        if( (ev>>6) > 3 )
                            goto BadReg;
                    }
                    putw( dval, outfile );
                    putc( RABS, relocf );
                    putc( 0, relocf );
                    for( cv = 0 ; cv < words ; cv++ ) {
                        putw( word[cv], outfile );
                        putw( wt[cv], relocf );
                    }
                    break;
                default :
                    goto BadArg;
                }
                break;
            default:
                msg( "IE09", 0 );
                exit( 1 );
            }
        }
        if( (el = getlex( &ev )) == LEX_EOL ) {
            linecnt = ev;
            goto NOER;
        }
        if( el == LEX_EOF )
            break;
        p = ediag( "bad command tail",
              "плохой конец команды" );
        goto ER;
BadType:
        p = ediag( "bad arg type",
              "плохой тип аргумента" );
        goto ER;
BadReg:
        p = ediag( "bad register",
              "плохой регистр" );
        goto ER;
BadArg:
        p = ediag( "bad argument",
              "плохой аргумент команды" );
ER:
        msg( p, 0 );
        flushlex();
NOER:           ;
    }

    if( caddr & 01 ) {
        putc( 0,    outfile );   /* Выровнять границу сегмента */
        putc( RABS, relocf );
        putc( 0,    relocf );
    }
}

/*
 * Вывести таблицу символов в вых. файл
 */
wrsymtab()
{
    extern struct STAB tabsym[];
    extern int  avail;
    extern char *refname;
    extern FILE *outfile, *relocf;
    register struct STAB *p, *q;
    register i;
    extern int address[];

/* Копировать информацию о перемещаемости в вых. файл */
    fseek( relocf, 0l, 0 );
    for( i = (address[0]+1 & ~01) + (address[1]+1 & ~01); i != 0; ){
        --i;
        putc( getc( relocf ), outfile );
    }
    fclose( relocf );

/* Копировать таблицу символов в вых. файл */
    q = tabsym+avail;
    for( p = tabsym; p < q; p++ ){
        for( i = 0;  i < 8; i++ ) putc( p->name[i], outfile );
        i = p->type;
        i = ( ((i & ISX) == XREG)? 020: 0 ) | (i & 047);
        putw( i, outfile );
        putw( p->value, outfile );
    }
}
