/* קשקןה הבפש מב üכעבמ
 *
 * $Log:        ind_date.c,v $
 * Revision 1.1  89/01/08  19:34:38  guba
 * Initial revision
 * 
 *
 */

#include <time.h>

extern char *conv();

ind_date( date )
    long  date;
{
    struct tm *tms, *localtime();

    tms = localtime( &date );

    dps2(tms->tm_mday);   dpo('.');
    dps2(tms->tm_mon+1);  dpo('.');
    dps2(tms->tm_year);   dpo(' ');
    dps2(tms->tm_hour);   dpo(':');
    dps2(tms->tm_min);    dpo('\'');
    dps2(tms->tm_sec);
}

static dps2(num) { dps(conv(num,2,10,'0')); }
