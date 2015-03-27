/* === CDEBUGGER ===  ПОКАЗ СТЕКА ВЫЗОВОВ */

#include "cdeb.h"

ind_stack()
{
#   define MAXLEVEL  12
    int level, narg, tr5, callpt;
    unsigned inst;
    char *name, savech, buf[10];

    for( level=0, tr5=regval[5]; tr5 && level<MAXLEVEL; ++level) {
        if( level ) dpo( '\n' );

        callpt = get_word( tr5+2 );         /* ищем, откуда нас вызвали */
        inst   = get_iword( callpt-4 );
        if( inst == 04737 )                     /* jsr pc,*$... */
            narg = 1;
        else if( (inst&0177700) == 04700 )      /* jsr pc,...   */
            narg = 0;
        else
            error( 15 );

        name = look_symbol( (inst==04767?callpt:0)+get_iword(callpt-2), 02);
        dpc( conv( level,2,10,' ') );           /* ищем имя функции */
        dpc( ":  " );
        if( name ) {
            savech = name[7];  name[7] = 0;
            dpc( *name=='~' ? name+1 : name );
            name[7] = savech;
        } else {
            dpc( "???" );
        }

        inst = get_iword( callpt );           /* считаем число параметров */
        if( inst == 05726 )                     /* tst (sp)+ */
            narg++;
        else if( inst == 022626 )               /* cmp (sp)+,(sp)+ */
            narg += 2;
        else if( inst == 062706 )               /* add $N,sp */
            narg += get_iword( callpt+2 ) / 2;

        dpo( '(' );                         /* печатаем параметры вызова */
        inst = tr5+2;
        while( narg-- ) {
            sprintf(buf," 0%o",get_word( inst+=2 ));
            dpc( buf );
            if( narg ) dpo( ',}' );
        }
        dpc( " )" );

        tr5 = get_word( tr5 );                    /* к следующей функции */
    }
}
