/**
 ** Hash test
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int hash( char* );

main(){
    static char line[ 256 ];
    int h;
    char *p;
    while( fgets( line, 256, stdin ) != NULL ){
        line[255] = 0;
        if( (p = strchr( line, '\n' )) != 0 ) *p = 0;
        h = hash( line );
        printf( "%s %d\n", line, h );
    }
    exit(0);
}

int hash( char *s0 ){
    /* static int b[5] = { 0, 7, 11, 13, 17 }; */
    static int b[5] = { 13, 11, 7, 5, 0 };
    static int t1[5] = { 0, 1, 2, 3, 4 };
    static int t2[5];
    static char buf[6];
    int l = strlen( s0 );
    int h = 0;
    int x;
    int *t;
    char *s;
    if( l <= 5 ){
        t = t1;
        for( x = 0; x < l; x++ ){
            buf[x] = s0[x];
        }
        for( ; x < 5; x++ ){
            buf[x] = 0;
        }
        s = buf;
    }else{
        t = t2;
        t2[0] = 0;
        t2[1] = l/2 & ~1;
        t2[2] = l-3;
        t2[3] = l-2;
        t2[4] = l-1;
        s = s0;
    }
    for( x = 5; x-- != 0; ){
        /* printf( "%d ", t[x] ); */
        h = h * b[x] + (s[t[x]] & 0x1F);
    }
    return h % 8251;
}
