/* ДОПОЛНИТЕЛЬНЫЕ ФУНКЦИИ */

#include <stdio.h>
#undef NULL
#define NULL ((char *)0)

/* Пересылка строки символов */

move( from, to, lgt )
char *from, *to;
{
    if( from > to ) {
        while( --lgt >= 0 ) *to++ = *from++;
    } else if( from < to ) {
        to += lgt;      from += lgt;
        while( --lgt >= 0 ) *--to = *--from;
    }
}

/* Поиск символа в строке с конца */

char *rindex( p, c )
register char *p, c;
{
    register char *s;

    s = p + strlen( p );
    while( --s > p ) {
        if( *s == c ) return( s );
    }
    return( NULL );
}

/* Очистка строки */

clear( p, n )
char *p;
{
    for( ; --n; ) *p++ = '\0';
}
