/* УДАЛЕНИЕ ФАЙЛА (КАТАЛОГА) И ВСЕХ ЗАВИСИМЫХ ФАЙЛОВ (может быть) */

/*
 * $Log:        rm.c,v $
 * Revision 2.1  89/10/30  19:53:17  rygoff
 * Новая ветвь.
 * Вызов rspec() заменен. См. синхронные правки в Mkdir.c
 *
 * Revision 1.1  89/08/19  18:10:59  rygoff
 * Initial revision
 *
 *
 */

#include "comint.h"
#include <sys/stat.h>

rm() {
    register int  i, j;
    register char *p;
    struct   dir2 *pd;

    for(i=nfiles; --i>=0; )  {
	pd = item(i);
	if( pd->d_mark == '-' ) {
	    if( only_x ) {              /* удаляем зависимые файлы */
		for( j=lind; --j>=0; ) {
		    p = cat[indcat[j]&0377].d_name;
		    if( is_boss( pd->d_name, p ) )  srm( p );
		}
	    }
	    srm( pd->d_name );          /* ... и сам файл */
	}
    }
}

extern char *name_f, *name_g;

srm(               name )
    register char *name;
{
    register char *p;
    name_f=name_g=NULL; /* rygoff 30.10.89 Вместо rspec() */
    if( p=lookcom('b'|0x0200,name) ) {
	exefound(p);
	errno = 0;
    } else {
	errno = 0;
	unlink( name );
	ind_err(name,1);
    }
}
