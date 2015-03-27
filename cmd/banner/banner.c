/*
 * МНОС РЛ 1.2
 * banner - печать большими буквами
 *
 * Главная программа
 *
 *       Автор: Антонов В.Г. ( ИПК Минавтопрома, ВМК МГУ )
 *       Последнее изменение: 28.09.85
 */

#include <ediag.h>
#include <stdio.h>

#define MAXOUTL 160     /* макс. ширина вых. строки */
#define MAXINL  80      /* макс. длина вх. строки */
#define SPACEW  8       /* ширина пробела */
#define CHARS   1       /* ширина промежутка между буквами */

char    outb[MAXOUTL+8];
int     outl;
char    inline[MAXINL+1];
char   *syms = "*";

struct FONT { char _c; char _k[7]; };

extern struct FONT font[];

main( c, v )
char **v;
{
        if( c != 1 ) {
                if( v[1][0] == '-' && v[1][1] == 'c' ) {
                        if( v[1][2] == '\0' ) {
                                usage:
                                        fprintf( stderr, ediag(
                                          "Usage: %s [-c<chars>] [line]\n",
                                          "Вызов: %s [-c<символы>] [строка]\n"), v[0] );
                                        exit(1);
                        }
                        c--;
                        v++;
                        syms = &(v[0][2]);
                }
        }
        if( c > 1 ) {
                if( c > 2 )
                        goto usage;
                ban( v[1] );
                exit(0);
        }
        if( getline() ) {
                ban( inline );
                while( getline() ) {
                        printf( "\n\n" );
                        ban( inline );
                }
        }
}

getline()
{
        register char *p = inline;
        register char *q = &inline[MAXINL];
        register c;

        if( feof( stdin ) )
                return(0);
        while( (c = getchar()) != EOF &&
                p <= q && c != '\n' )
                *p++ = c;
        *p = 0;
        while( c != '\n' && c != EOF )
                c = getchar();
        return( c != EOF || *inline != '\0' );
}

ban( s )
char *s;
{
        register struct FONT *f;
        register char c, *q;
        char    *p = s-1, *R;
        register int i;

        for( q = outb; q < &outb[MAXOUTL+8] ; *q++ = 0 );
        q = outb;
        while( *++p ) {
                if( *p == ' ' ) {
                        q += SPACEW;
                        goto EOS;
                }
                c = *p;
                f = font;
                while( f->_c ) {
                        if( f->_c == c )
                                goto FND;
                        f++;
                }
                continue;
            FND:
                q += setchar( q, f->_k ) + CHARS;
            EOS:
                if( q >= &outb[MAXOUTL] )
                        break;
        }

        for( i = 1; i < 0200 ; i <<= 1 ) {
                q = &outb[MAXOUTL-1];
                while( !(*q & i) )
                        q--;
                R = q;
                p = syms;
                while( *p ) {
                        for( q = outb; q <= R; q++ )
                                putchar( (*q & i)? (*p): ' ' );
                        if( *++p )
                                putchar( '\r' );
                }
                putchar( '\n' );
        }
}

setchar( q, k )
char *q;
char  k[7];
{
        register int i, j ;
        register char c;
        char m;

        i = 0200;
        j = 0;
        m = k[0] | k[1] | k[2] | k[3] | k[4] | k[5] | k[6];
        while( i ) {
                c = 0;
                if( k[0] & i )
                        c |= 01;
                if( k[1] & i )
                        c |= 02;
                if( k[2] & i )
                        c |= 04;
                if( k[3] & i )
                        c |= 010;
                if( k[4] & i )
                        c |= 020;
                if( k[5] & i )
                        c |= 040;
                if( k[6] & i )
                        c |= 0100;
                *q++ = c;
                j++;
                m &= ~i;
                if( !m )
                        break;
                i >>= 1;
        }
        return( j );
}
