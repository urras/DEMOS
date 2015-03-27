/*
 * LSI-11 linker
 * ����������� ��'������ ������� � ����������� ������.
 */
#include "LinkerDefs"

static unsigned TextLen, BssLen, LoadAddr;

void Tseta( adload )
    unsigned adload;         /* A���� �������� */
{
    LoadAddr = adload;
}

void Tinit()
{
    TextLen = BssLen = 0;
}

void Tcopy( mp )
    Module *mp;
{
    char *buff;
    unsigned int blen;       /* ����� ����������� ����� (�����) */
    unsigned int clen;       /* ��������� ����� ���������� ����� */
    unsigned int totlen;     /* ����� ����� ������ */
    unsigned int ua;

    mp->Mseek = TextLen;
    mp->Mldbase = TextLen+LoadAddr; /* ��������� ���� � ������ */
    mp->Mbssoff = BssLen;           /* ��������� ������ */
    totlen = mp->Mtextsize + mp->Mdatasize;
    for( clen = 0; clen < totlen; clen += blen ){
        ModRead( &buff, (int *) &blen );
        if( clen+blen >= totlen ) blen = totlen-clen;
        if( blen == 0 ) break;
        OutWrite( buff, blen );
    }
    if( clen < totlen ) Error(1);   /* ������� �������� */

     /* �������� �� ������������ ���������� ��������� */
     /* ��������� ������������ ����������� � Reloc()  */
    ua = TextLen + clen;
    if( ua < TextLen ) Error(2);
    TextLen = ua;
    ua = BssLen + mp->Mbsssize;
    if( ua < BssLen ) Error(2);
    BssLen = ua;

     /* �������� ����� �� ������ ���� */
    if( (BssLen|TextLen) & 1 ) Error(1);
}

void Tget( bp )
    Statistics *bp;
{
    bp->Sladdr = LoadAddr;
    bp->Stlen = TextLen;
    bp->Sdlen = 0;
    bp->Sblen = BssLen;
}

unsigned Tabss()
{
    return LoadAddr+TextLen;
}
