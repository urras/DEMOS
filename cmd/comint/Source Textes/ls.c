/* ПОКАЗ ПАРАМЕТРОВ ФАЙЛА */

/*
 * $Log:        ls.c,v $
 * Revision 1.6  89/12/20  15:05:06  rygoff
 * Введено ограничение при подсчете строк в текстовых файлах:
 * если файл большой, до конца не считаем.
 * 
 * Revision 1.5  89/11/22  14:15:26  rygoff
 * *** empty log message ***
 * 
 * Revision 1.4  89/11/22  13:21:16  rygoff
 * Еще изменен формат выдачи. 
 * 
 * Revision 1.3  89/11/16  13:41:13  rygoff
 * Еще изменен формат выдачи. Указание стрелкой отменили.
 * linefeed'ы считаем не fgetc'ем, а в своем буфере.
 * 
 * Revision 1.2  89/11/11  22:54:00  rygoff
 * Для текстовых файлов показывает число строк.
 * Показывает дату обращения и дату модификации.
 * В строке не осталось места для имени файла,
 * поэтому на файл показываем стрелкой.
 * 
 * Revision 1.1  89/08/19  17:55:55  rygoff
 * Initial revision
 * 
 *
 */

#include "comint.h"
#include <sys/stat.h>
#include <a.out.h>

#include <tty_codes.h>
#undef NULL
#include <stdio.h>



ls(       name )
    char *name;
{
    struct stat    st;
#ifndef XENIX
# ifdef SYSV    /* So far compiled only on SCO/386 and AT&T/386 */
    struct aouthdr head;
#   define  a_magic magic
#   define  a_text  tsize
#   define  a_data  dsize
#   define  a_bss   bsize
# else
    struct exec    head;
# endif
#else
    struct xexec   head;
#   define  a_magic x_magic
#   define  a_text  x_text
#   define  a_data  x_data
#   define  a_bss   x_bss
#endif
    register char  *i, *md; /* Also used as integers */
#   define imd (short)md
#   define iimd(x) (md=(char*)(x))
#   define ii  (short)i
#   define iii(x) (i=(char*)(x))
    unsigned long li;
    int f;

    head.a_magic = 0;
    if( (f=open(name,0)) > 0 ) {
        read(f,&head,sizeof head);
    }
    errno = 0;
#ifndef BSD
    stat(name,&st);
#else
    lstat(name,&st);
#endif
    if( ind_err(name,1) )  return;


    dpp( 0, ysize-3 ); dpo(es);
    dpp(10, ysize-3 );
    dps("*** "); dps( name ); dps(" *** ");

    if(
#   ifdef SYSV
	ISCOFF( head.a_magic )
#else
	head.a_magic==A_MAGIC1 or head.a_magic==A_MAGIC2
#   endif
    ) {
        dps(": Object file");
        dpp(0,ysize-2);
	dps10( (unsigned long)head.a_text, 0 );  dpo('+');
	dps10( (unsigned long)head.a_data, 0 );  dpo('+');
	dps10( (unsigned long)head.a_bss,  0 );  dpo('=');
	li = head.a_text+head.a_data+head.a_bss;
        dps10( li, 0 );  dps(" (");
        dps( convl( li, 6, 8, '0' ) );  dpo(')');
    } else {
        for(i= (char *)&head,md=i+sizeof head; i<md; ) {
            register char c= *i++;
            if( not is_print(c) and c!='\t' and c!='\n' )  break;
        }
        if(i==md) { /* Текстовый файл */
	    char buf[1024];
            int nl=0, nr;
            do {
                register char *p, *e;
                e = (p=buf) + (nr=read(f,buf,sizeof buf));
                while(p<e) if(*p++=='\n') nl++;
            } while ( nr == sizeof buf && nl<=1024);
            dps(": Text file");
            dpp(15,ysize-2);
            if(nl>1024) dps("More than ");
            dps10( (unsigned long)nl, 0 ); dps(" lines");
        }
    }
    close(f);

    iimd(st.st_mode);
    dpp( 0, ysize-1 );
    switch( imd & S_IFMT ) {
	case S_IFDIR: iii('d'); break;         /* directory */
	case S_IFCHR: iii('c'); break;         /* character special */
	case S_IFBLK: iii('b'); break;         /* block special */
	case S_IFREG: iii(' '); break;         /* regular */
#ifdef S_IFLNK
	case S_IFLNK: iii('l'); break;         /* symbolic link */
#endif
#ifdef S_IFQUOT
	case S_IFQUOT:iii('q'); break;         /* quota */
#endif
    }
    dpo(ii);
    dpo(imd&S_ISUID ? 'u' : '-');  /* set user id on execution */
    dpo(imd&S_ISGID ? 'g' : '-');  /* set group id on execution */
    dpo(imd&S_ISVTX ? 't' : '-');  /* save swapped text after use */
    dpo(' ');

    acc( imd>>6, ((st.st_uid)&0377)==(getuid()&0377) );
    acc( imd>>3, ((st.st_gid)&0377)==(getgid()&0377) );
    acc( imd, 0 );

    dps10((unsigned long)st.st_nlink,0);  dpo(' ');

    {
	unsigned long size;
	size = (unsigned long)st.st_size;
	if( li = (int)(size/1000) )  {dps10(li,0);  dpo('_');}
	dps( convl( (unsigned long)(size%1000), 3, 10, li?'0':0 ) );
    }

    dpp(45, ysize-3); dps("- Created   "); ind_date(st.st_ctime);
    dpp(45, ysize-2); dps("- Modified  "); ind_date(st.st_mtime);
    dpp(45, ysize-1); dps("- Accessed  "); ind_date(st.st_atime);
}


static acc(      md, flag )
    register int md, flag;
{
    dpo( flag ? '<' : ' ' );
    dpo( md&4 ? 'r' : '-' );
    dpo( md&2 ? 'w' : '-' );
    dpo( md&1 ? 'x' : '-' );
    dpo( flag ? '>' : ' ' );
}


dps10( num, sym )
unsigned long num;
{
        dps( convl(num,7,10,sym) );
}
