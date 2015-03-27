/*
 * Ассемблер  вер. 1.0
 *    ЭВМ СМ-4   МНОС РЛ 1.2
 *    Автор: Антонов В.Г. ( ВМК МГУ, ИПК Минавтопрома )
 *    Последнее изменение: 09.02.1985
 */

#include <stdio.h>
#include <ediag.h>
#include "as.h"

unsigned UBRaddr[MAXUBR];       /* Адреса команд + 2 */
char     UBRtype[MAXUBR];       /* Тип операнда */
unsigned UBReval[MAXUBR];       /* Значение операнда */
int      uavail = 0;            /* Первый своб. элемент в таблице */
unsigned Dshift = 0,            /* Начальные адреса сегментов DATA и BSS */
     Bshift = 0;

/*
 * Проход 1.5 - коррекция таблицы символов
 *   при обработке команд типа XJBR и XJBC
 */
pass15()
{
    register int cjbt;
    register int I;
    int      addr, shift;
    extern int  avail, lavail, uflag;
    extern int  errcnt, Lfvals[], linecnt;
    extern char Lftypes[];
    extern struct STAB tabsym[];
    extern int address[];
#ifdef UBROPT
    int     oflg;

OLOOP:
    oflg = 0;
#endif UBROPT
    for( cjbt = 0; cjbt < uavail; cjbt++ ) {
        if( (I = UBRtype[cjbt]) & 0200 )
            goto ERS;
        switch( I & (~0100) ) {
        case LEX_UDL :
            addr = Lfvals[ UBReval[cjbt] ]; /* Здесь м.б. использ.
                                индекс -1, когда
                                f-метка неопред. */
            break;
        case LEX_NAME :
            { register struct STAB *stp;
                stp = &tabsym[ UBReval[cjbt] ];
                if( (stp->type & ~EXTERN) == TEXT )
                    addr = stp->value;
                else {
                    stp->name[8] = 0;
                    if( addr == UNDEF )
                        msg( ediag( "'%' undefined",
                               "'%' неопределено" ),
                             stp->name );
                    else
                        msg( ediag(
        "'%': bad type for address of command",
        "'%': плохой тип для адреса команды" ), stp->name );
                    goto ERS;
                }
            }
            break;
        default:
            msg( "IE04", 0 );
            exit( 1 );
        }
        addr -= UBRaddr[cjbt];
        if( addr >= -256 && addr <= 254 ) {
            shift = ( (UBRtype[cjbt]|=0200) & 0100 )? 4 : 2;
            addr = UBRaddr[cjbt];
#ifdef UBROPT
            oflg = 1;
#endif UBROPT
            /* Вот она, гнусная коррекция !! */
            { register int p;

                /* Коррекция таблиц UBR */
                p = cjbt+1;
                I = shift;
                while( p < uavail )
                    UBRaddr[p++] -= I;

                /* Коррекция таблицы символов */
                for( p = 0; p < avail ; p++ )
                    if((tabsym[p].type & ~EXTERN) == TEXT &&
                        ((unsigned)tabsym[p].value) >
                              ((unsigned)addr) )
                        tabsym[p].value -= I;

                /* Коррекция таблицы f-меток */
                for( p = 0; p < lavail ; p++ )
                    if( Lftypes[p] == TEXT &&
                        ((unsigned)Lfvals[p]) > ((unsigned)addr) )
                         Lfvals[p] -= I;

                address[0] -= I;
            }
        }
ERS:            ;
    }
#ifdef UBROPT
    if( oflg )
        goto OLOOP;
#endif UBROPT

    { register int p;

        Dshift = (address[0]+1) & ~01;
        Bshift = Dshift + ((address[1]+1) & ~01);
        for( p = 0; p<avail ; p++ ) {
            register struct STAB *stp;
            switch( (stp = &tabsym[p])->type & 047 ) {
            case UNDEF :
                /* Обработка uflag */
                if( uflag )
                    stp->type = EXTERN | UNDEF;
                break;
            case DATA :
            case DATA | EXTERN :
                stp->value += Dshift;
                break;
            case BSS :
            case BSS | EXTERN :
                stp->value += Bshift;
            }
        }
    }
}
