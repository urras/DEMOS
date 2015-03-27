#include <ctype.h>

#define TRUE    (1)
#define FALSE   (0)

/*
 * ����������� ����� �� from � dest.
 * ���� �������� ����� ������ - ����������
 * �������� ������� �� ����� size.
 * ���� �������� ����� ������� - ��������.
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
