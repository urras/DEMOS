/* ��������������� �����: ����� --> ��� ��������� ������������� */
/* �� ������: ��������� �� ������ ������ 'length', ����������� \000,
              ���������� ��������������� �� ������ ������� ��������� �����,
              ������������ ������� ��������� 'begins' */

char *conv (value, length, radix, begins)
    register unsigned value;            /* ������������� �������� */
    int length;                         /* ����� ��������� ���� */
    register radix;                     /* ��������� ������� ��������� */
    char begins;                        /* ��� ��������� ����� ������� */
{
    char *ptr;
    static char buf[16+1];
    register i,j;

    buf[16]='\000';
    for(i=0; i<16; ++i)  buf[i]=begins;
    ptr = &buf[16];

    do {
        *--ptr = (i=value%radix) < 10 ? i+'0' : i-10+'A';
        value /= radix;
     } while(value);

    return(&buf[16-length]);
}
