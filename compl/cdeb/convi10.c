/* === CDEBUGGER === ВВОД ЧИСЛА

   вводит целое число с указателя 'pcmn'.
   на выходе: число, pcmn сдвинуто.
   при ошибке указатель 'pcmn' не изменяется.
   формат числа:  12   012  0x12
*/

extern char *pcmn;

convi10()
{
    register int toadd, value, base;
    char *oldpcmn;

    value=0;  base = 10;   oldpcmn = pcmn;

    if( *pcmn == '0'  ) {            /* определяем систему счисления */
        base = 8;  ++pcmn;
        if( *pcmn == 'x'  ||  *pcmn == 'X' ) {base = 16;  ++pcmn;}
    }

    while( *pcmn ) {
        if     ( *pcmn>='a' && *pcmn<='f' )
            toadd = *pcmn - 'a' + 10;
        else if( *pcmn>='A' && *pcmn<='F' )
            toadd = *pcmn - 'A' + 10;
        else if( *pcmn>='0' && *pcmn<='9' )
            toadd   = *pcmn - '0';
        else
            break;
        if( toadd >= base )  {pcmn = oldpcmn;  return;}
        value = (value * base) + toadd;
        ++pcmn;
    }

    return( value );
}


/* ВВОД ЦЕЛОГО ЧИСЛА С ОБРАБОТКОЙ + - '...' t f T F */

int conv_int()
{
    register char *p;
    register int i, j;

    p = pcmn;
    if( *p == '\'' ) {
        ++pcmn;
        i=0;  j=1;
        while( *pcmn && *pcmn != '\'' )  {i |= j*get_char();  j <<= 8;}
        ++pcmn;

    } else if( *p == 't'  ||  *p == 'T' ) {
        i = 1;  ++pcmn;

    } else if( *p == 'f'  ||  *p == 'F' ) {
        i = 0;  ++pcmn;

    } else {
        if( *p == '+'  ||  *p == '-' ) ++p, ++pcmn;
        i = convi10();
        if( p == pcmn )  --pcmn;
        if( *--p == '-' )  i = -i;
    }

    return( i );
}
