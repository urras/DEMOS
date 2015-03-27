/* ВВОД ИМЕНИ ФАЙЛА С ПРОВЕРКОЙ, ЧТО ЭТО НОВОЕ ИМЯ */

/*
 * $Log:        inp_name.c,v $
 * Revision 1.4  89/12/02  15:46:33  rygoff
 * Исправлена плюха с затиранием старого файла по s_erase.
 * 
 * Revision 1.3  89/12/02  15:10:47  rygoff
 * Если есть селекция, файл создавался сразу - без запроса имени.
 * Это почти всегда неожиданность.
 * Теперь запрашивается имя с готовой "затравкой", равной селекции.
 * 
 * Revision 1.2  89/11/11  22:52:41  rygoff
 * Введен режим MS_mode (Редактирование в стиле MicroSoft)
 * 
 * Revision 1.1  89/08/19  17:55:41  rygoff
 * Initial revision
 * 
 *
 */

#include <tty_codes.h>
#include "comint.h"

char *inp_name( old, ask, symhelp )
    char       *old,*ask, symhelp;
{
    register int c, i;
    int          pos;
    static char  new[D_NAME+1];
    static char  chnm[] = "rename ";

    strcpy( new, old );

    for(;;) {
        dpp(0,ysize-1);
        if( exist(new) ) {
ren:        dpo(bl); dpp(0,ysize-1); dps(chnm);
        }

        dps(ask);  dps(" >");  dpn(D_NAME,' ');  dpo('<');  dpo(el);
        dpn(D_NAME+1,cl);  dps(new);
        if( (pos=strlen(new)) >= D_NAME )  {--pos;  dpo(cl);}

in:     for( i=strlen(new); i<D_NAME; ++i )  new[i] = ' ';
        c = dpr( new, D_NAME, &pos, on_control|MS_mode );
        for( i=D_NAME; --i>=0 and new[i]==' ';  new[i]=0 )  ;
        switch( c ) {
            case s_finish:  /* rygoff 25.07.89 */
            case s_refuse:  goto ref;
            case s_help:    help("comint",symhelp);  continue;
            case rn:        if( exist(new) ) goto ren; return(new);
	    case f12:
            case s_erase:   srm(new); if(errno) goto ref; return(new);
            default:        dpo(bl); goto in;
        }
    }
    /* return( new );   NEVER reached ! */

ref:
    dpp(0,ysize-1);  dpo(el);
    return(NULL);
}
