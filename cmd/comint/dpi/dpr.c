/* ЭКРАННЫЙ РЕДАКТОР СТРОКИ (Имитатор) */
/*
 * $Log:        dpr.c,v $
 * Revision 1.3  89/09/23  08:01:08  rygoff
 * Исправлена работа "ЗАБОЙ" на конце строки в MS_mode
 * 
 * Revision 1.2  89/09/23  06:12:27  rygoff
 * Добавлен режим MS_mode - редактирование строки "в стиле MicroSoft":
 *      "ЗАБОЙ" работает именно как "ЗАБОЙ", а не control-символ;
 *      внутри строки - режим INSERTMODE (как в редакторе red)
 * Пока некорректно работает "ЗАБОЙ" на конце строки - съедает
 * лишний символ, если курсор убежал в пробелы справа.
 * 
 * Revision 1.1  89/09/23  05:49:16  rygoff
 * Initial revision
 * 
 *
 */

#include <tty_codes.h>
#define and &&
#define or  ||
#define not !

int dpr(     buf, lgt, pos, mode )
    char    *buf;                           /* редактируемый буфер */
    int           lgt;                      /* его длина */
    int               *pos;                 /* смещение */
    unsigned                mode;           /* флаги */
{
#   define       offset  (*pos)
#   define       isf(x)  (mode&(x))
    register int newpos, c, k;

    for(;;) {
        for(k=lgt;--k>=0;) if(buf[k]!=' ') break; /* ищем смысл. конец */
        ++k;

        if( (c=dpi()) == s_spec )  c = spec(dpi());
        switch( c ) {
            case rn:                           /* конец редактирования */
                goto ok;

            case spec(lf):                      /* на конец строки */
                if( isf(on_border) )  goto ok;
onen:           if( (newpos=k) >= lgt )  --newpos;
                goto posit;

            case lf:                            /* на начало строки */
                if( isf(on_border) )  goto ok;
beglin:         newpos = 0;
posit:          if( newpos >= offset )  dpn(newpos - offset,cr);
                else                    dpn(offset - newpos,cl);
                offset = newpos;
                break;

            case spec(ta):                      /* назад по 10 позиций */
                newpos = (offset+9)/10*10 - 10;
                if( newpos < 0 ) {
                    if( isf(on_border) )  goto ok;
                    newpos = 0;
                }
                goto posit;

            case ta:                           /* вперед по 10 позиций */
                newpos = (offset+10)/10*10;
                if( newpos>k and isf(on_space|on_no_digit) ) goto beglin;
                if( newpos >= lgt ) {
                    if( isf(on_border) )  goto ok;
                    newpos = lgt - 1;
                }
                goto posit;

            case cl:                            /* сдвиг влево */
                if( offset )  {newpos = offset - 1;  goto posit;}
                else           goto ch_border;

            case cr:                            /* сдвиг вправо */
                if( (newpos = offset+1) < lgt ) {
                    if( newpos>k and isf(on_space|on_no_digit) ) goto ok;
                    goto posit;
                }
                --newpos;
ch_border:      if( isf(on_border) )  goto ok;
                break;

            case dl:                          /* стирание конца строки */
                if( isf(on_space|on_icdc) )  goto ok;
                for( c = offset;  c < k;  ++c )  buf[c] = ' ';
                goto rewrite;

            case ic:                            /* вставка символа */
                if( isf(on_icdc|on_space) )  goto ok;
                if( buf[c=lgt-1]!=' ' and isf(on_border|on_space)) {
                    c = ic;  goto ok;
                }
                while( --c >= offset )  buf[c+1] = buf[c];
                buf[offset] = ' ';
                if( k >= lgt )  --k;
rewrite:        newpos = offset;
                if( k<lgt )  ++k;
                for( ; offset<k; ++offset )  dpo( buf[offset] );
                goto posit;

            case de: /*            rygoff 25.07.89
                      *    "ЗАБОЙ" (он же обычно BackSpace)
                      *      ДОЛЖЕН РАБОТАТЬ КАК "ЗАБОЙ" !
                      *   ибо не все начинали в системе MISS ...
                      */
                if( not isf(MS_mode) ) {
                    if( isf(on_control) ) goto ok;
                    /* else */ break;
                }
                if(offset==0) { dpo(bl); break; }
                /* else */
                if( --offset==k ) ++offset; else dpo(cl);
            case dc:                               /* удаление символа */
                if( isf(on_icdc) )  goto ok;
                if( offset > k ) {                      /* за концом */
                    goto onen;
                } else if( offset == k ) {              /* на конце */
                    if(offset) {
                        newpos = offset-1;  dpo(cl);
                        dpo(buf[newpos]=' ');
                        goto posit;
                    } else {
                        if( isf(on_border) ) goto ok;
                    }
                } else {                              /* в середине */
                    for( c = offset; ++c<k; )  buf[c-1] = buf[c];
                    buf[k-1]=' ';
                    goto rewrite;
                }
                break;

            default:                            /* обычный символ */
                if( c==' '  and  isf(on_space) )  goto ok;
                if( isf(on_no_digit) and not is_digit(c) )  goto ok;
                if( (c&0xff00)  or  not is_print(c) ) {
                    if( isf(on_control) )  goto ok;
                } else {
                    if( isf(MS_mode) ) { /* rygoff 23.09.89 */
                        register int jj;
                        for(jj=(k<lgt?++k:k); --jj>=offset; )
                                buf[jj]=buf[jj-1];
                    }
                    dpo( buf[offset++] = c );
                    if( offset >= lgt )  {    /* на конце строки */
                        --offset;  dpo(cl);
                        c=cr;  goto ch_border;
                    }
                }
                if( isf(MS_mode) ) goto rewrite; /* rygoff 23.09.89 */
                break;
        }
    }
ok: return( c );
}
