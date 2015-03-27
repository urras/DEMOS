/* СЧИТЫВАНИЕ ТЕКУЩЕГО ПСЕВДО-КАТАЛОГА
   Возврат:  0 - ОK,  1 - это не библиотека,  -1 - ошибка чтения */

/*
 * $Log:        file_cat.c,v $
 * Revision 2.1  89/11/03  17:01:23  rygoff
 * Новая ветвь.
 * Перед запуском команды, читающей каталог, проверяем вид показа.
 * 
 * Revision 1.1  89/08/19  17:43:04  rygoff
 * Initial revision
 *
 */
#include <stdio.h>
#ifndef BUFSIZ
#define BUFSIZ 1024
#endif
#include <tty_codes.h>
#include "comint.h"
#undef pd

static int _filbuf(p)
register FILE *p;
{
    if( (p->_cnt=read(p->_file,p->_base,BUFSIZ)) <= 0 ) {
        if(p->_cnt)
            p->_flag |= _IOERR;
        return(EOF);
    }
    --p->_cnt;
    p->_ptr = p->_base;
    return( 0xff & (int) (*(p)->_ptr++) );
}

struct  dir
{
        ino_t   d_ino;
        char    d_name[DIRSIZ];
};

char pattern[lname];

int file_cat() {
    register char       *p;
    register unsigned int ch;
    register struct dir *pd;

#   define               pl ((FILE *)pd)
    FILE                 pipeline;
    unsigned char        buf[BUFSIZ];
#   undef getchar
#   define getchar()     getc(pl)       /* from pipeline */

    char                *s, *old_name, *old_path;
    int                  pip[2];
#   define               endcat ((char*)(&cat[maxcat]))
    int                  lgt;

    for(p=s=hcat; *p; ++p) if( *p=='/' )  s = p;  /* отступ в $c $a $p */
    if( arname==NULL or !strcmp(hcat,"/") or *s!='/' )  return(1);
    *s = 0;
    old_name = arname;  old_path = arpath;
    if( arpath ) {if( arpath>=s )  arpath = NULL;}
    else          arname = NULL;

    p = lookcom( rn, s+1 );                /* ищем команду для входа */
    *s = '/';                              /* восстанавливаем $c $a $p */
    arname = old_name;  arpath = old_path;
    if( p == 0 )  return(1);

    pip[0] = pip[1] = -1;                        /* запускаем процесс, */
    pipe( pip );                                 /*   читающий каталог */
    start( show_type(p+2), -pip[1], NO );
    close( pip[1] );

    lcat = 0;

    pl = &pipeline;
    pl->_file = pip[0];
    pl->_base = &buf[0];
    pl->_flag = _IOREAD;
    pl->_cnt  = 0;

    if( (ch=getchar()) == '/' ) {
        static char arhead[85];
#       define bhead (&arhead[0])
#       define ehead (&arhead[sizeof arhead] - 1)
        for( s = bhead; (*s++=getchar()) != '\n' ; ) {
            if( s >= ehead ) {
                *--s = 0;
                while( getchar() != '\n' )
                    ;
                break;
            }
        }
        gethead(bhead);
    } else {
        --pl->_ptr;
        ++pl->_cnt;
    }

    s = (char*) (&cat[0]);                      /* чтение каталога */
    for( ; ; ) {
        p = &pattern[0];
        while( (ch=getchar()) == *p++ )
            ;
        if(ch == EOF)
            break;
        if( *--p == '\0' ) {
            if(ch == '/')
                ch=getchar();
            while( ch != '/'
                && ch != EOF
                && (*s++ = ch) != '\n'
                && s < endcat
                )
                ch=getchar();
            if(ch == '/')
                *s++ = '\n';
        }
        while( ch !='\n'  &&  ch != EOF )
            ch=getchar();
        if(ch == EOF  ||  s >= endcat)
            break;

    }
    *s='\0';
    lgt = s - (char*)(&cat[0]);
    close( pip[0] );
    if(ferror(pl))
        return(-1);

    move( &cat[0], p = endcat - lgt, lgt );    /* преобразование строк */
    if( *p == 0 )  return(0);                  /*    в формат каталога */
    for( pd = (struct dir *)&cat[0]; lcat<maxcat and p<endcat;
         ++pd, ++lcat ) {

        /* register int i;          */
        /* register struct dir *px; */   /* Экономим регистры */

        clear( pd->d_name, D_NAME );

#       define i  ch
        for( i = 0;  *p!='\n' and p<endcat;  ++p ) {
            if( (int)i < D_NAME )  pd->d_name[i++] = *p;
        }
#       undef i
        ++p;
        pd->d_ino = 1;

        /*
         * А может, такое имя уже есть на этом уровне ?
         * При работе с образом ленты это вполне возможно
         */
#       define px ((struct dir *)ch)
        for (
                ch=(unsigned int)(&cat[0]);
                (struct dir *)ch < pd;
                ch += sizeof pd
            ) {
            if( strncmp(px->d_name,pd->d_name,DIRSIZ) == 0 )  {
                --pd;
                --lcat;
                break;
            }
        }
#       undef px
    }
    return(0);
}


