#include <stdio.h>

#define FSCANSIZE  16   /* Number of scan lines in data file */
#define USESCAN    11   /* Number of lines which are in use actualy */

int nextchar( /* char* */ );
void printline( /* char */ );

main()
{
    static char cbuff[ FSCANSIZE ];
    int count = 0;
    while( nextchar( cbuff ) != -1 ){
        int i;
        printf( "@%X\n", count );
        for( i = 0; i < USESCAN; i++ ){
            printline( cbuff[i] );
        }
        count++;
    }
    exit(0);
}

int nextchar( buff ) char* buff; {
    int i;
    int c;
    for( i = 0; i < FSCANSIZE; i++ ){
        if( (c = getchar()) == EOF ){
            if( i != 0 ){
                fprintf( stderr, "Unexpected EOF\n" );
                exit(1);
            }
            return -1;
        }
        buff[i] = c;
    }
    return 0;
}

void printline( linec )  char linec; {
    int i;
    for( i = 0; i < 8; i++ ){
        putchar( (linec & (1 << 7-i))? '*': ' ' );
    }
    putchar( '\n' );
}
