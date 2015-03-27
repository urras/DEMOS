/* ЗАПУСК КОМАНДЫ 'name', ЗАПУЩЕННОГО СИМВОЛОМ 'symb' */

/*
 * $Log:        start.c,v $
 * Revision 2.8  89/11/15  16:50:39  rygoff
 * 
 * 
 * Revision 2.7  89/11/15  14:35:20  rygoff
 * 
 * 
 * Revision 2.6  89/11/13  10:14:16  rygoff
 * Еще поправил показ нажатого символа
 * 
 * Revision 2.5  89/11/12  16:49:56  rygoff
 * Реакцию на прерывания восстановил,
 * как было у Флерова
 * 
 * Revision 2.4  89/11/11  23:01:07  rygoff
 * Исправлен показ нажатого символа (используется INSYMB)
 * 
 * Revision 2.3  89/11/09  10:39:23  rygoff
 * Мелкие правки
 * 
 * Revision 2.2  89/11/07  12:47:13  rygoff
 * Вернул показ нажатого символа перед файлом,
 * но вместо '+' при нажатии спецсимвола ставится стрелка ->
 * 
 * Revision 2.1  89/10/30  19:56:59  rygoff
 * Новая ветвь.
 * Удалена подпрограмма rspec() - вместо нее повсюду используется
 * явный запуск lookcom() и в случае успеха - exefound() вместо start(),
 * чтобы можно было использовать спецкоманды.
 * 
 * Revision 1.1  89/08/19  18:12:31  rygoff
 * Initial revision
 * 
 *
 */

#include <signal.h>
#include <tty_codes.h>
#include "comint.h"
#include "run.h"

extern int INSYMB;
extern int   invisible;
extern char *fname;
static char *amp_last;

start(    name, symb, shline )
    char *name, symb;
    boolean           shline;
{
#   define      max_ac 50
    int         ac;
    char       *av[max_ac];
    boolean     use_sh, parallel;
    char       *ind_name;
    char        nbuf[D_NAME];
    static char sh[]   = "/bin/sh";
    static char exec[] = "exec ";
#   undef       f0
#   undef       f1
#   undef       f2
    int         f0, f1, f2;

    {                                   /* ищем & на конце */
        register char *p;
        if( amp_last )  *amp_last = '&';
        amp_last = 0;
        for( p=name; *p and *p!='\n'; ++p )  {};
        while( *--p == ' '  and p>=name )  {};
        parallel = NO;
        if( *p   == '&' ) {parallel=YES; amp_last=p; *p=' ';}
        if( symb == '&' )  parallel=YES;
    }

    use_sh = NO;
    ini_shbuf();  cpy_shbuf(exec);
    if( shline ) {sh_line( name );  invisible = 0;}
    else          cm_line( name );
    name = get_shbuf();
    ind_name = name + (sizeof exec-1);
    if( amp_last )  *amp_last = '&';
    if( looksym( ind_name, "()<>{}[]`*?'\"$#" ) )  use_sh=YES;
    if( looksym( ind_name, ";|&"      ) )    {use_sh=YES; name=ind_name;}
    if( not use_sh )  name = ind_name;
    strncpy( nbuf, shline ? ind_name : fname, D_NAME );

    if( not shline and is_print(INSYMB) and INSYMB!=' ' ) dpo(INSYMB);

    if( invisible==0 or invisible==3 ) {          /* показываем запуск */
        dpp( 0, ysize - ((strlen(ind_name)+xsize+1)/xsize) );
        dpo('>');  dps(ind_name);  dpo(el);
        if( parallel )  dpo('&');
    }

    if( not use_sh ) {                  /* формирование argv */
        register char *p;
        ac = 0;
        for( p=name; ; ) {
            while( *p == ' ' )  ++p;
        if( *p == 0 )  break;
            if( ac < max_ac-1 )  av[ac++] = p;
        if( (p=no_esc(p)) == 0 )  break;
            ++p;
        }
        av[ac] = 0;
    }

    if( symb < 0 ) {                          /* запуск с 'трубой' */
        if( fork() == 0 ) {
            f0 = f2 = open("/dev/null",2);
            f1 = -symb;
            goto ex;
        }

    } else if( not parallel ) {               /* запуск с ожиданием */
        if( (use_sh ? strt( sh, "-c", name, 0 ) : strtv( av )) == 0
        and invisible<2 ) {
            dpp(0,ysize-1);  dps( "--- OK --- " );  dpi();
        }

    } else {                                  /* запуск в параллель */
        register int i;
        for( i=3; i<=16; ++i )  close(i); /* закрываем все дескрипторы */
        for( i=0; i<maxrun; ++i ) {         /*    ищем место в таблице */
            if( at_run[i].r_pid == 0 )  goto femp;
        }
        return;                              /*    многовато процессов */
    femp:

        dpo(0);
        f1 = creat( prot_name(nbuf), 0666 );
        if( f1 < 0 )  f1 = open( "/dev/null", 2 );
        if( (at_run[i].r_pid=fork()) == 0 ) {    /* запуск в параллель */
            f0 = open("/dev/null",0);
            f2 = f1;
    ex:     signal(SIGINT, SIG_IGN);
            signal(SIGQUIT,SIG_IGN);
            dup2( f0, 0 );  dup2( f1, 1 );  dup2( f2, 2 );
            close(f0); close(f1); close(f2);
            nice(addnice);
            if( use_sh )  execlp( sh, "sh", "-c", name, 0 );
            else          execvp( av[0], av );
            if( symb < 0 )  write(f1,"",1);
            exit(1);
        }
        close(f1);
        strncpy( at_run[i].r_name, nbuf, D_NAME );
    }
}

/* УДАЛЯЕТ ИЗ СТРОКИ 'p' ESCAPE-СИМВОЛЫ.
   Вернет адрес 1-ого пробела или 0 если конец строки */

char *no_esc(      p )
    register char *p;
{
    register char *s, *q;

    s = p;
    while( *p and *p!=' ' ) {
        if( *p != '\\' ) {
            *s++ = *p++;
        } else {
            static char esc[]             = "tnrf0";
            if( q=index(esc,*++p) )  *s++ = "\t\n\r\f\0"[q-esc];
            else                     *s++ = *p;
            ++p;
        }
    }
    if( *p == 0 )  p = 0;
    *s = 0;
    return( p );
}


/* ПОИСК ОДНОГО ИЗ СИМВОЛОВ 'symset' В СТРОКЕ 'str'. ВЕРНЕТ 1 ЕСЛИ ЕСТЬ*/

int looksym(       str, symset )
    register char *str,*symset;
{
    while( *symset )  if( index(str,*symset++) )  return(1);
    return(0);
}


/* ДОБАВЛЯЕМ К ЗАТРАВКЕ 'p' КОМАНДНОЙ СТРОКИ ПОМЕЧЕННЫЕ ФАЙЛЫ */

#undef pd
sh_line(           p )
    register char *p;
{
    register int   i;
    struct   dir2 *pd;

    while( *p && *p!='|' && *p!='>' )  put_sh( *p++ );  /* до |, > */

    for( i=0; i<nmarked; ++i )  {     /* пересылка имен входных файлов */
        pd = &cat[marked[i]&0377];
        if( pd->d_mark != '>' )  add_name(pd,0);
    }
    for( i=0; i<nalt; ++i ) {
        pd = &altnam[i];
        if( pd->d_mark != '>' )  add_name(pd,1);
    }

    cpy_shbuf( p );                         /* остаток команды */

    for( i=0; i<nmarked; ++i )  {           /* выходной файл */
        pd = &cat[marked[i]&0377];
        if( pd->d_mark == '>' )  {add_name(pd,0); break;}
    }
    for( i=0; i<nalt; ++i ) {
        pd = &altnam[i];
        if( pd->d_mark == '>' )  {add_name(pd,1); break;}
    }
}
