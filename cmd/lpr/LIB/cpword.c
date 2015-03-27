#include <ctype.h>

#define TRUE    (1)
#define FALSE   (0)

/*
 * Копирование слова из from в dest.
 * Если исходное слово короче - дополнение
 * нулевыми байтами до длины size.
 * Если исходное слово длиннее - обрезаем.
 */

cpword (dest, from, size)
register char  *dest,
               *from;
register int    size;
{
    char pading = FALSE;

    while (--size) {
        if (pading)
            *dest++ = '\0';
        else
            if (!*from || isspace (*from))
                pading = TRUE;
            else
                *dest++ = *from++;
    }
    *dest = '\0';
}
