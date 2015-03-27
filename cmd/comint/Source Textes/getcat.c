/* СЧИТЫВАНИЕ КАТАЛОГА */

/*
 * $Log:        getcat.c,v $
 * Revision 1.1  89/08/19  17:44:30  rygoff
 * Initial revision
 * 
 *
 */

#include "comint.h"
#include <sys/stat.h>

struct  dir
{
        ino_t   d_ino;
        char    d_name[DIRSIZ];
};

short largeflag=0;        /* Indicates directory is large */

/* Читаем каталог библиотеки и составляем таблицу индексов файлов */

getcat() {
    nmarked = 0;
    if( file_cat() <= 0 ) {
        for( lind=0; lind<lcat; ++lind ) {
            register struct dir *pd;
            pd = (struct dir*) (&cat[lind]);
            pd->d_ino = 0;
            indcat[lind] = lind;
        }


/* Читаем каталог, составляем таблицу индексов файлов, упорядоченных по
   алфавиту. Обнуляем маркеры и ставим нули на конце имен
   (в следующей статье!). */

    } else {
        register int fd, i;
        struct stat  st;
        errno = 0;
        if( (fd=open(".",0)) == -1 )  goto err;
        if( fstat( fd, &st ) == -1 )  goto err;       /* не читать если*/
        if( st.st_mtime==last_mod ) {                    /* не менялся*/
            for( i=0; i<lcat; ++i ) {
                register struct dir *pd;
                pd = (struct dir*) (&cat[lind]);
                pd->d_ino = 0;
            }
            close(fd);
            return;
        }
        last_mod = st.st_mtime;
        lcat = read( fd, cat, (sizeof (struct dir2)) * maxcat );
        if( lcat < 0 ) {
err:        close(fd);  ind_err(".",0);
            lcat = 0;  lind = 0;
            return;
        }
        lcat /= (sizeof (struct dir2));
        close( fd );
	/*
	 * If the directory is rage enough,
	 * there may be problems
	 */
	if( lcat >= maxcat ) largeflag = 1; else largeflag=0;

        lind = 0;
        for( i=lcat; --i>=0; ) {
            register struct dir *pd;
            register int         put_to;

            pd = (struct dir*) (&cat[i]);
            if( pd->d_ino != 0 ) {
                pd->d_ino  = 0;
                put_to = search_ind( pd->d_name );
                if( put_to < 0 )  insert_ind( i, ~put_to );
            }
        }
    }


/* Восстанавливаем запомненную пометку */

    if( !strcmp( hcat, altcat ) ) {
        register int i, m;
        while( --nalt >= 0 ) {
            register struct dir2 *pd;
            for( i=lcat; --i>=0; ) {
                pd = &cat[i];
                if( !strcmp( pd->d_name, altnam[nalt].d_name ) ) {
                    m = altnam[nalt].d_mark;
                    if( m<'1' or m>'9' ) {
                        pd->d_mark = m;
                    } else {
                        if( pd->d_mark == 0 )  pd->d_mark = '1';
                        else                   pd->d_mark++;
                    }
                    marked[nmarked++] = i;
                }
            }
        }
        nalt = altcat[0] = 0;
    }
}
