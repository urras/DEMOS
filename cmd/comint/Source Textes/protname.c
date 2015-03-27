/* ГЕНЕРИРУЕТ ИМЯ ФАЙЛА С СУФФИКСОМ .O
   возвращает новое имя */

/*
 * $Log:        protname.c,v $
 * Revision 1.1  89/08/19  18:10:32  rygoff
 * Initial revision
 * 
 *
 */

#include "comint.h"

extern char *index(), *strcat();

char *prot_name( name )
    char        *name;
{
    static   char  prot_name[D_NAME+1];
    register char *p;

    clear( prot_name, D_NAME );
    strncpy( prot_name, name, D_NAME-2 );
    while( p = index(prot_name,'/') )  *p = '+';
    return( strcat( prot_name, ".O" ) );
}
