/**
 ** Преобразователь статьи формата "B" для PDP-11
 ** в формат "I" общих кросс-систем.
 **/
#include <SysCalls>
#include <FileSystem>
#include <StartOpers>
#define LIBASK  0

#pragma STARTER(), EXIT();
main()
{
    static char pdp_11[8] = { 0x02,0x23,0x61,0xF8,0x0C,0xD0,0,0 };
    static char B_LSI[8]  = { 0x02,0x1F,0xBE,0xA0,0x0C,0xD0,0,0 };
    int lab;
    int lcnt;
    char *fp, *s;
    static FileItem fpar;    FileItem *ip;
    int pleng, bleng;
    int rc;

    fp = AZC;
    *fp++ = 'B';
    *fp++ = HidBit;
    *fp++ = 0;                          /* Место под количество файлов*/
    *fp++ = 1;                          /* Выбираем один файл         */
    lcnt = 0;
    for( lab = 1; lab < 0x7F; lab++ ){
        if( (ip = GetFileItem( lab )) != NULL &&
             (ip->type&0x7F) == 'B' &&
             _cmps( (char*)&ip->processor, B_LSI, 8 ) == 0 )
        {
            if( lcnt > 90 ) break;      /* Чтобы текст поместился     */
            lcnt++;
            *fp++ = (char)lab;
        }
    }
    AZC[2] = (char)lcnt;
    MVS( "\tStand-alone program to convert", 32, fp );
    if( (rc = StartLibr( LIBASK, 0 )) < 1 ) _exit_(rc);
    lab = AZC[1];

    SetFilesWrite( 1 );

    ip = GetFileItem( lab );
    fpar = *ip;
    fpar.type = 'I';
    s = (char*) &ip->info;
    pleng = (s[1]<<8) + (s[0]&0xFF);    /*Размер кода                 */
    pleng += (s[3]<<8) + (s[2]&0xFF);   /*Размер данных               */
    bleng = (s[5]<<8) + (s[4]&0xFF);    /*Размер буферов              */
    s = (char*) &fpar.info;
    *s++ = (char)(pleng>>8);    *s++ = (char)pleng;
    *s++ = (char)(bleng>>8);    *s++ = (char)bleng;
    *s++ = 0x04;    *s++ = 0;
    *s++ = 0x04;    *s++ = 0;
    fpar.processor = *(Fproc*) pdp_11;
    DeleteName( &fpar, WorkCatal );
    _exit_( RenameFile( &fpar, (char)lab, WorkCatal ) );
}
