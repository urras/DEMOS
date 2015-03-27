#include <stdio.h>
#include <string.h>

#define FSCANSIZE  16   /* Number of scan lines in data file */
#define USESCAN    11   /* Number of lines which are in use actualy */
#define NCHARS    128
#define IMAGESIZE (NCHARS*FSCANSIZE)

void getscan();

main(){
    static char image[ IMAGESIZE ];
    static char line[ 81 ];
    char *cp = NULL;
    int ll;
    int scanx;

    memset( image, 0xFF, IMAGESIZE );
    while( fgets( line, 81, stdin ) != NULL ){
        if( (ll = strlen( line )) && line[ll-1] == '\n' ){
            line[ --ll ] = 0;
        }
        if( line[0] == '@' ){
            int x;
            sscanf( line+1, "%X", &x );
            if( (unsigned)x >= NCHARS ){
                fprintf( stderr, "Wrong char index %d\n", x );
                exit(1);
            }
            cp = image + x*FSCANSIZE;
            memset( cp, 0, USESCAN );
            scanx = 0;
        }else{
            if( cp == NULL ){
                fprintf( stderr, "No char index\n" );
                exit(1);
            }
            if( scanx >= USESCAN ){
                fprintf( stderr, "Too large matrix\n" );
                exit(1);
            }
            getscan( cp + scanx++, line );
        }
    }
    if( fwrite( image, 1, IMAGESIZE, stdout ) != IMAGESIZE ){
        fprintf( stderr, "Write error\n" );
        exit(1);
    }
    exit(0);
}

void getscan( scan, textline )
    char *scan;
    char *textline;
{
    int rv = 0;
    int bit = 0x80;
    while( *textline && bit ){
        if( *textline++ != ' ' ) rv |= bit;
        bit >>= 1;
    }
    *scan = (char) rv;
}
