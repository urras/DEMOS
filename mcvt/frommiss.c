#include <stdio.h>

int main()
{
    int c;
    while( (c = getchar()) != EOF ){
        if( (c & 0xE0) == 0x60 ){            /* Uppercase russian letters */
            c = c + 0x80;
        }else if( (c & 0xE0) == 0xC0 ){      /* Lowercase english letters */
            c = c - 0x80 + 0x20;
        }else if( (c & 0xE0) == 0xE0 ){      /* Lowercase russian letters */
            c = c - 0x20;
        }
        putchar( c );
    }
    exit(0);
}
