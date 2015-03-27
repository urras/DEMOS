/* ��������������� �����: ����� --> ��� ��������� ������������� */
/* �� ������: ��������� �� ������ ������ 'length', ����������� \000,
              ���������� ��������������� �� ������ ������� ��������� �����,
              ������������ ������� ��������� 'begins' */

/*
 * $Log:        conv.c,v $
 * Revision 1.1  89/08/19  17:37:25  rygoff
 * Initial revision
 * 
 *
 */

char *conv (value, length, radix, begins)
    register unsigned int  value;       /* ������������� �������� */
    int length;                         /* ����� ��������� ���� */
    register radix;                     /* ��������� ������� ��������� */
    char begins;                        /* ��� ��������� ����� ������� */
{
    char *ptr;
    static char buf[16+1];
    register i;

    buf[16]='\000';
    for(i=0; i<16; ++i)  buf[i]=begins;
    ptr = &buf[16];

    do {
        *--ptr = (i=value%radix) < 10 ? i+'0' : i-10+'A';
        value /= radix;
     } while(value);

    return( begins ? &buf[16-length] : ptr );
}

char *convl (value, length, radix, begins)
    register unsigned long value;       /* ������������� �������� */
    int length;                         /* ����� ��������� ���� */
    register radix;                     /* ��������� ������� ��������� */
    char begins;                        /* ��� ��������� ����� ������� */
{
    char *ptr;
    static char buf[16+1];
    register i;

    buf[16]='\000';
    for(i=0; i<16; ++i)  buf[i]=begins;
    ptr = &buf[16];

    do {
        *--ptr = (i=value%radix) < 10 ? i+'0' : i-10+'A';
        value /= radix;
     } while(value);

    return( begins ? &buf[16-length] : ptr );
}

