/* СОЗДАНИЕ ФАЙЛА И КАТАЛОГА */

/*
 * $Log:        Mkdir.c,v $
 * Revision 2.2  89/12/01  09:56:54  rygoff
 * Доопределены внешние переменные
 *
 * Revision 2.1  89/12/01  09:46:26  rygoff
 * Новая ветвь.
 * В связи с изменение структуры управления на верхнем уровне
 * вызов rspec() заменен на явный вызов lookcom()/start()
 *
 * Revision 1.1  89/08/19  18:10:16  rygoff
 * Initial revision
 *
 *
 */

#include "comint.h"
extern char *name_f, *name_g, *inp_name();

char *Mkdir( p )  /* p -> "символ название" */
    register char    *p;
{
    register char *new;

    new = inp_name( select, p+2, 'c' );
    if( new != NULL ) {
	name_f=name_g=NULL; /* rygoff 30.10.89 Вместо rspec() */
	if( p=lookcom( *p|0x0200,new) ) exefound(p);
	else {
	   errno = 0;
	   close( creat(new,0666) );
	   ind_err(new,1);
	}
    }
    return( new );
}


/* ПРОВЕРКА, ЕСТЬ ЛИ УЖЕ В КАТАЛОГЕ ТАКОЕ ИМЯ */

int exist( name )
    char  *name;
{
    register int i;

    for( i=lind; --i>=0; ) {
	if( !strcmp( cat[indcat[i]&0377].d_name, name ) )  return(1);
    }
    return(0);
}
