#
/*
 * Ассемблер  вер. 1.0
 *    ЭВМ СМ-4   МНОС РЛ 1.2
 *    Автор: Антонов В.Г. ( ВМК МГУ, ИПК Минавтопрома )
 *    Последнее изменение: 09.02.1985
 */

#include <ediag.h>
#include "as.h"

int     hash[MAXTABSYM];        /* Хэш-таблица */

struct STAB tabsym[MAXTABSYM];  /* Таблица символов */

int     avail = 0;              /* Указ. на свободный элемент tabsym */
char    FNM[9];
int     point;
int     xtflag;                 /* 1 если первый символ = '~' */

/*
 * Поиск в таблице символов
 *  Возвращает номер в табл. символов
 */
int
search( )
{
    extern   char str[];

    register char *p;
    register char *s;
    register int   l;

    auto     int   hval;
    auto     int   hptr;

     /* Вычислить хэш-функцию */
    hval = ( FNM[0] + FNM[3] )<<3 | ( FNM[1] ^ FNM[7] ) |
               ( FNM[2] ^ FNM[4] ^ FNM[5] )<<1 | ( FNM[6] )<<2 ;
    hval = ((21*hval+8521)&077777) % MAXTABSYM;

     /* Поиск в таблице */
    for(;;){
        hptr = hash[hval];
        if( hptr == 0 ) {     /* Больше нет возможностей */
            point = hval;
            return( Not_found );
        }
        p = FNM;
        s = tabsym[hptr-1].name;
        for( l = 0; l < 8 && p[l] == s[l] ; l++ ) ;
        if( l == 8 ) {        /* Совпали все буквы */
            point = hval;
            return( hptr-1 );
        }
        hval = (hval+17) % MAXTABSYM;
    }
}

/*
 * Поиск по таблице команд и, если не найдено, занесение в табл. символов
 */
int
intsym( pnm, lexx )
char    *pnm;
int     *lexx;
{
    char    *nm;
    int     k,l;
    register int i;
    register char *q,*r;
    extern int Rctbl;
    extern struct STAB Ctable[];
    extern char str[];

    nm = pnm;
    if( xtflag = ( (*nm) == '~' ) )
        nm++;

/* Дополнить имя символами '\0' */
    r = nm;
    for( i = 0; i<8 ; i++ ) {
        if( *r )
            FNM[i] = *r++;
        else
            FNM[i] = 0;
    }

    if( !xtflag ) {
        if( (k = search( )) != Not_found )
            return( k );
        i = 0;
        q = nm;
        r = FNM;
        while( i++ < 8 && (*r++ = *q++) );
        while( i++ < 9 )
            *r++ = 0;

        if( *lexx != LEX_ERR ) {
/* Двоичный поиск */
            k = 0;
            l = Rctbl;
            do {
                i = (k+l)/2;
                q = FNM;
                r = Ctable[i].name;
                if( *q == *r ) {
                    while( *++r == *++q )
                        if( *r == 0 )
                            goto Found;
                }
                if( i == k ) {
                    k = l;
                    continue;
                } else if( (*q) > (*r) )
                    k = i;
                else
                    l = i;
            } while( k<l );
        }
    }

/* Не найдено, занести в хэш - таблицу */
    if( avail == MAXTABSYM-1 ) {
        msg( ediag( "symbol table overflow",
               "переполнилась таблица символов" ), 0 );
        END();
    }
    r = tabsym[avail++].name;
    if( !xtflag )
           hash[point] = avail;
    q = FNM;
    *r++ = *q++; *r++ = *q++; *r++ = *q++; *r++ = *q++;
    *r++ = *q++; *r++ = *q++; *r++ = *q++; *r = *q;
    return( avail-1 );
Found:
    if( (Ctable[i].type != XABS) && (*lexx == LEX_LID) ) {
        msg( ediag( "command mnemonics used as label",
               "использование мнемокода команды как метки" ), 0 );
        *lexx = LEX_ERR;
        return( -1 );
    }
    *lexx = LEX_COP;
    return( i );
}
