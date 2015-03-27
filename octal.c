#include <stdio.h>

main()
{
    int pc;
    short word;

    freopen( "ttt", "rO", stdin );
    freopen( "DUMP", "w", stdout );
    for( pc = 0; pc < 0x42; pc += 2 ){
         fread( (char*)&word, 1, 2, stdin );
         printf( "%06o\n", word );
    }
    exit(0);
}
