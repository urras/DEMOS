/* === CDEBUGGER === ������ DUMP'A ������� ������ */

/* ������ ���� ������ �� ��������� 'adr' 'lgt' ����.
   � ����� ����� ������� ������ �� ���������. */


cdump (adr, lgt)
    register char *adr;                 /* ����� ������ ������� ������   */
    int  lgt;                           /* ����� ������� ������/16       */
{
    extern int errno;
    register i;
    register unsigned word;
    char byte;

    adr &= 0177776;
    for(; lgt>0; adr+=16, lgt-=16) {
        dpc( conv(adr,6,8,'0') );       /* ������ ����� ������ */
        dpo( '=' );

        for(i=0; i<16; i += 2)  {                     /* ... ���� */
            word = get_word( adr+i );
            if( errno )  {
                dpc( "------" );
                errno=0;
            } else
                dpc( conv( word, 6,8,'0') );
            dpo( ' ' );
        }

        for(i=0; i<16; ++i) {                         /* ... ����� */
            byte = get_byte( adr+i );
            dpo ( ( (byte>=' ' && byte<0177) || (byte&0300)==0300 )
                  ? byte : '.' );
        }

        dpo( '\n' );
    }
}
