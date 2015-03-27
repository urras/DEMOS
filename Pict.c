/*
 * LSI-11 Linker
 * Вывод картинки
 */
#include <Display>

void Pict( ProgName )
    char *ProgName;
{
    static char outbuff[] = {
        ER, LF, SM,10,CL,'?','?','?','?','?','?','?','?','?','?',
        HO, SC,FI_HIGHL,0,
        '*','L','S','I','-','1','1',' ','L','i','n','k','e','r','*',
        SC,0,0,  HO,
        0
    };
    char *cp;
    int i;

    cp = & outbuff[5];
    for( i = 0; i < 10; i++ ){
        *cp++ = (*ProgName)? *ProgName++: ' ';
    }
    dpc( outbuff );
}
