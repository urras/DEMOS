#include <stdio.h>
#include <string.h>

#define FSCANSIZE  16   /* Number of scan lines in data file */
#define USESCAN    11   /* Number of lines which are in use actualy */
#define NCHARS    128
#define IMAGESIZE (NCHARS*FSCANSIZE)

/*  #define NYPOS       4	*/
/*  #define NXPOS      16	*/
/*  #define BASECHAR   0x40	*/
#define NYPOS       8
#define NXPOS      16
#define BASECHAR   0x0

int revbits( b )
    int b;
{
    int r = 0;
    int i;
    for( i = 0; i < 8; i++ ){
        r |= (b & 1) << 7-i;
        b >>= 1;
    }
    return r;
}

main(){
    static char image[ IMAGESIZE ];
    static char line[ 81 ];
    char *cp = NULL;
    int row, xpos, ypos;
    int cval;

    if( fread( image, 1, IMAGESIZE, stdin ) != IMAGESIZE ){
        fprintf( stderr, "Error while reading image\n" );
        exit(1);
    }

    printf( "#define font_width %d\n", 8*NXPOS );
    printf( "#define font_height %d\n", NYPOS*USESCAN );
    printf( "static char font_bits[] = {\n" );
    for( ypos = 0; ypos < NYPOS; ypos++ ){
        for( row = 0; row < USESCAN; row++ ){
            for( xpos = 0; xpos < NXPOS; xpos++ ){
                cval = BASECHAR + ypos*NXPOS + xpos;
                printf( " 0x%02x,",
                    revbits( image[ cval*FSCANSIZE + row ] ) & 0xFF );
            }
        }
        printf( "\n" );
    }
    printf( "}\n" );
    exit(0);
}
