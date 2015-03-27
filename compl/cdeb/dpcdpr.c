/* === CDEBUGGER === ��������� �����/������ �� ������� */

/* ���� ����/����� ��������� ����������� � ������ ������
   ��� ������ ������������ ����� 'outline' � ��������� ��
   ������� ������� � ��� 'pline'. ������� ������� � ������
   ��������� � ���������� 'posit'.

   ����� �����:
        dpc(����� ������)       - ����� ������ �������� �� 0
        dpo(������)             - ����� ������ �������
        dpr(����� �����,�����)  - ���� ������ � �����. � ����� ������
                                  ���������� 0, ���������� ����� (��� 0).
                                  ������ ������������ � ����� ���: '\n'  */

/* ������ ������ ���������� ��������� UNIX'� - ��������� ������
   read(0,...,...)  �  write(1,...,...)                        */

#define MAXBUF  256
char outline[MAXBUF+1], *pline=outline; /* ����� ������                  */
int  posit;                             /* ������� � �������� ������     */
extern int dsp_inp, dsp_out;            /* ������� ��� ����� � ������    */
static int oldtty[3],newtty[3];         /* ��������� �������             */
int ttyflag;
extern int protocol;                    /* ���������� ����� ���������    */


dpo(letter)
    register char letter;
{
    if( !ttyflag ) {
        gtty(dsp_out,oldtty);
        ttyflag = 1;
    }
    *pline++ = letter;
    check_buf();
    if( letter == '\n' ) posit=0;  else  ++posit;
}


dpc(string)
    register char *string;
{
    while( *string ) dpo( *string++ );
}


check_buf()                      /* ��������: �� ������������ �� ����� ? */
{
    if( pline >= &outline[MAXBUF] ) {
        gtty(dsp_out,newtty);
        stty(dsp_out,oldtty);
        write(dsp_out, outline, pline-outline);
        write(protocol,outline, pline-outline);
        stty(dsp_out,newtty);
        pline = outline;
    }
}


dpr(adr,length)
    register char *adr;
    register int  length;
{
    register int  lgt;

    gtty(dsp_out,newtty);
    stty(dsp_out,oldtty);
    write(dsp_out, outline, pline-outline);              /* ����� ������ */
    write(protocol,outline, pline-outline);
    pline = outline;

    lgt = read(dsp_inp, adr, length);                     /* ���� ������ */
    write(protocol, adr, lgt);
    stty(dsp_inp,newtty);

    if( lgt<=0 ) {                                        /* ����� ������*/
        write(dsp_out, "\n", 1);
        write(protocol,"\n", 1);
        exitdeb();
    }
    adr [ --lgt ] = 0;
    return( lgt );
}


exitdeb()
{
    write(dsp_out, outline, pline-outline);              /* ����� ������ */
    write(protocol,outline, pline-outline);
    stty(dsp_out,oldtty);
    exit(0);
}
