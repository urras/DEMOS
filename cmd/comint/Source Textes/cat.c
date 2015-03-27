/* ПОКАЗ ФАЙЛА В ТЕКСТОВОМ ВИДЕ */

/*
 * $Log:        cat.c,v $
 * Revision 2.2  89/11/11  22:37:10  rygoff
 * Многоточие ставим в правом нижнем углу
 * 
 * Revision 2.1  89/11/03  13:07:35  rygoff
 * Новая ветвь.
 * В конце файла не пишем слово END - это не всегда удобно
 * (например, если выводятся графические файлы)
 * Ставим многоточие и ждем.
 * 
 * Revision 1.1  89/08/19  17:36:39  rygoff
 * Initial revision
 * 
 *
 */

#include <tty_codes.h>
#include "comint.h"

extern int errno;

catf(     name )
    char *name;
{
    register int fd, lgt;
    register char *p, *s;
    int nl;
    char         buf[512+1];
    char ch;

    errno = 0;
    if( (fd=open(name,0)) >= 0 ) {
        dpp(0,ysize-1); dps("\n--- ");  dps(name);  dps(" ---");
        dpo('\n');
        dpo('\r');
        nl = 0;
        while( (lgt=read(fd,buf,512)) > 0 ) {
            p = s = &buf[0];
            while( p < &buf[lgt] ) {
                if( *p++ == '\n' ) {
                    if( nl++ == (ysize-2) ) {
                        ch = dpi();
                        if(ch == s_finish || ch == s_refuse) {
                            close(fd);
                            return;
                        }
                        nl=0;
                    }
                    ch = *p;
                    *p = '\0';
                    dps(s);
                    dpo('\r');
                    s = p;
                    *p = ch;
                }
            }
            *p = '\0';
            dps(s);
        }
        if( not ind_err(name,1) )  {
            dpp(xsize-7,ysize-1);
            dps(" ... ");
            dpi();
        }
    }
    close(fd);
}
