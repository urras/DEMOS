/*
 * LSI-11 linker
 * Копирование об'ектных модулей в загрузочный модуль.
 */
#include "LinkerDefs"

static unsigned TextLen, BssLen, LoadAddr;

void Tseta( adload )
    unsigned adload;         /* Aдрес загрузки */
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
    unsigned int blen;       /* Длина переданного блока (байты) */
    unsigned int clen;       /* Суммарная длина переданной части */
    unsigned int totlen;     /* Общая длина модуля */
    unsigned int ua;

    mp->Mseek = TextLen;
    mp->Mldbase = TextLen+LoadAddr; /* Размещаем коды и данные */
    mp->Mbssoff = BssLen;           /* Размещаем буфера */
    totlen = mp->Mtextsize + mp->Mdatasize;
    for( clen = 0; clen < totlen; clen += blen ){
        ModRead( &buff, (int *) &blen );
        if( clen+blen >= totlen ) blen = totlen-clen;
        if( blen == 0 ) break;
        OutWrite( buff, blen );
    }
    if( clen < totlen ) Error(1);   /* Слишком короткий */

     /* Проверка не переполнение логических сегментов */
     /* Суммарное переполнение проверяется в Reloc()  */
    ua = TextLen + clen;
    if( ua < TextLen ) Error(2);
    TextLen = ua;
    ua = BssLen + mp->Mbsssize;
    if( ua < BssLen ) Error(2);
    BssLen = ua;

     /* Нечетных длинн не должно быть */
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
