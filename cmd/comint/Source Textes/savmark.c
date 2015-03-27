/* СМЕНА ТЕКУЩЕГО КАТАЛОГА ==> ЗАПОМИНАЕТ ПОМЕТКУ */

/*
 * $Log:        savmark.c,v $
 * Revision 1.1  89/08/19  18:11:26  rygoff
 * Initial revision
 * 
 *
 */

#include "comint.h"

int savmark() {
    register int          i;
    register struct dir2 *pd;

/*  rspec('q',NULL,NULL,NULL);  немного меняем логику (guba,24.06.87) */
    if( nmarked != 0 ) {
        altcat[0] = 0;
        if( hcat[0] == '/' ) {
            strcpy( altcat, hcat );
            if( arname != NULL )  strcpy( altarname, arname );
            else                  altarname[0] = 0;
            altpath = arpath ? altcat + (arpath-hcat) : NULL ;

            for( i=0, nalt=0; i<nmarked and nalt<maxalt; ++i ) {
                pd = &cat[marked[i]&0377];
                altnam[nalt++] = *pd;
#ifdef dummy
                if( only_x ) {           /* запоминаем зависимые файлы */
                    register int          j;
                    register struct dir2 *pdx;

                    for( j=lind; --j>=0 && nalt<maxalt; ) {
                        pdx = &cat[indcat[j]&0377];
                        if( is_boss( pd->d_name, pdx->d_name ) ) {
                            altnam[nalt++] = *pdx;
                        }
                    }
                }
#endif
            }
        }
    }
}
