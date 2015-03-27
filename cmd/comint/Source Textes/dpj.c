/* ПРЫЖКИ ПО ПОЛЯМ (меню) */
/*
 * $Log:        dpj.c,v $
 * Revision 2.7  89/11/15  16:43:52  rygoff
 * Убрал работу с флагом Paint - она перенесена в indcat()
 *
 * Revision 2.6  89/11/11  22:46:14  rygoff
 * Еще одна правка по позиционированию
 *
 * Revision 2.5  89/11/03  14:23:15  rygoff
 * Revision 2.4  89/11/03  14:01:06  rygoff
 * Исправлена синтаксическая ошибка
 *
 * Revision 2.3  89/11/03  13:54:54  rygoff
 * Все-таки позиционирование надо делать один раз,
 * но для асинхронного и синхронного случаев -
 * в разных местах.
 *
 * Revision 2.2  89/11/03  13:14:41  rygoff
 * Пришлось два раза делать установку позиции:
 *      первая - для асинхронных пометок
 *      вторая - после перерисовки
 * Поэтому завел переменные x и y, чтобы считать один раз.
 *
 * Revision 2.1  89/10/30  19:26:53  rygoff
 * Это почти копия dpj() из библиотеки libdpi.a,
 * модифицированныя для асинхронной работы чарез dpa()
 *
 * Revision 1.1  89/09/23  05:08:26  rygoff
 * Initial revision
 *
 *
 */

#include "tty_codes.h"

extern int Paint;

int dpj( elements, cur, li, width, x0, y0 )
    int  elements,*cur, li, width, x0, y0;
{
    register int c, current;

    current = *cur;
    if( current<0   ||  current>=elements  ||  li>elements ) {
	vdpend();  exit( 100 );
    }

    for(;;) {

	if( (c=dpa())<0 ) {
	    if( Paint ) {
		ind_cat();
	    }
	    dpp( (width*(current/li))+x0, (current%li)+y0 );
	    c=dpi();
	} else {
	    dpp( (width*(current/li))+x0, (current%li)+y0 );
	}

	switch( c ) {
	    case cu:
		if( li<=1 )  goto retdpt;
		if(--current < 0)  current += elements;
		break;
	    case cd:
		if( li<=1 )  goto retdpt;
		if(++current >= elements) current -= elements;
		break;
	    case cr:
		if( li==elements )  goto retdpt;
		if((current += li)>= elements) {
		    current = (current+1) % li;
		}
		break;
	    case cl:
		if( li==elements )  goto retdpt;
		if((current -= li) < 0) {
		    current += ((elements+li-1)/li)*li-1;
		    if(current>=elements)  current -= li;
		}
		break;
	    default:
		goto retdpt;
	}
    }
retdpt:
    *cur = current;
    return( c );
}
