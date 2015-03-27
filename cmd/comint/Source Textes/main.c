/*
 * Comint main routine
 * 
 * $Log:        main.c,v $
 * Revision 2.11  89/12/18  19:34:36  rygoff
 * Исправлена установка курсора при входе в каталог 
 * (всегда встает к последнему файлу)
 * 
 * Revision 2.10  89/12/02  15:08:59  rygoff
 * При запуске копируем часть среды в свои буфера.
 * 
 * Revision 2.9  89/11/22  14:14:20  rygoff
 * *** empty log message ***
 * 
 * Revision 2.8  89/11/22  13:27:55  rygoff
 * Если не можем найти $HOME, переходим в корневой каталог.
 * 
 * Revision 2.7  89/11/22  13:18:42  rygoff
 * Уменьшено число файлов на экране, чтобы освободить внизу 
 * еще одну строку. Теперь свободных строк три.
 * 
 * Revision 2.6  89/11/16  13:39:04  rygoff
 * Еще правки с сигналами и в связи с отменой указания стрелкой в ls()
 * 
 * Revision 2.5  89/11/15  18:05:31  rygoff
 * Еще правлена работа с сигналами (при инициализации)
 * 
 * Revision 2.4  89/11/15  16:48:49  rygoff
 * Убрал работу с флагом Paint (перенес в indcat()).
 * Немного перетасовал метки.
 * 
 * Revision 2.3  89/11/15  14:32:23  rygoff
 * Правлена работа с потомками.
 * Введен дополнительный хандлер для SIGCHLD,
 * установлен отдельный longjmp, изменены места установки
 * реакции; вместо SIG_IGN используется SIG_HOLD
 * 
 * Revision 2.2  89/11/03  13:23:13  rygoff
 * В связи с удалением подпрограммы setpos() 
 * используется явный вызов dpp()
 * 
 * Revision 2.1  89/10/30  19:08:58  rygoff
 * .pp
 * Существенно изменена структура управления в связи с
 * необходимостью использовать в инициализирующих и
 * терминирующих действиях спецкоманды, чередуя их с обычными
 * командами в произвольном порядке.  Поскольку спецкоманды
 * (например, переход в другой каталог) могут также вызывать
 * терминирующие (в связи с выходом из текущего) и
 * инициализирующие действия, обработку командной строки
 * пришлось вынести в отдельную рекурсивную подпрограмму. См.
 * комментарии в exefound.c
 * 
 * 
 */

#include <setjmp.h>
#include <signal.h>
#include <tty_codes.h>
#undef pd
#include "comint.h"
#include "run.h"


/* инициализация общих переменных */

char last_name[D_NAME+1];
int com_com, shell_com;  /* число команд comint'а и shell'а */
char *name_f, *name_g, *name_F;
struct dir2 *pd;
boolean noname;
char ini[lname]="_c";  /* rygoff 18.10.89 Выбор каталога при инициализации */
int  INSYMB= rn;       /* Только что введенный символ */
boolean Paint=YES;     /* Флаг - перерисовывать ли каталог */

int     is_shell;
extern char *getwd();
extern int   phead;

extern void (*onbreak)();
jmp_buf on_break, on_child;

#ifdef BSD
onchild() {
    sigsys(SIGCHLD,SIG_HOLD);
    if( !phead ) ind_run();
    longjmp( on_child, 1 );
}
#else
#   define onchild onbreak
#endif
/**/

extern char **environ;

/*********************************/

main( argc, argv ) char  **argv; {

/*********************************/

    register int          i;
    register char         *p /* Also used as integer */,  *pcmd;
#   define   ip           (short)p
#   define   iip(x)       (p=(char*)(x))
    struct s_dpd          dp_par;

    signal(SIGINT,SIG_IGN);
#ifdef BSD
    signal(SIGCHLD,SIG_IGN);
#endif
    if( argc > 1 )  exit(1);                      /* для снятия t бита */

    {
        register int Ruid, Rgid;
        Ruid = getuid();       Rgid = getgid();
        addnice = 10;
        if( nice(-addnice) == -1 )  addnice = 0;
        setuid(Ruid); setgid(Rgid);
    }

    umask(022);
    vdpbeg();                                    /* распределение полей */
    dp_par  = dpd();                            /*           на экране */
    xsize   = dp_par.xdim;  ysize = dp_par.ydim;
    y_run   = 0;
    y_head  = y_run  + (ysize>16 ? 2 : 1);
    y_name  = y_head + 1;
    y_table = y_name + 2;
    maxico  = (xsize/wcolumn) * (ysize - y_table - 3); /* rygoff 22.11.89 */
                                                       /* (Вместо " - 2") */
    strcpy( cat[maxcat].d_name,   ")----->" );
    strcpy( cat[maxcat+1].d_name, "<-----(" );

    for( i=3; i<=16; ++i )  close(i);     /* закрываем все дескрипторы */
    inienv(); /* Настроились на работу со средой */
    only_x=1;
    gettune();
    name_f=name_g=NULL;
    if(p=lookcom('n'|0x0200,NULL)) exefound(p);

    p = (p=getenv("HOME")) ? p : "/";

/* Если запущен как shell, входим в $HOME, иначе - в текущий */
    if( **argv == '-' ) {
        is_shell = YES;
        if( chdir(p) != -1 )  {
            strcpy(hcat,p);
        } else
            strcpy(hcat, "/");
    } else { /* Where am I ? */
        if( hcat != getwd(hcat) )
           strcpy(hcat, "/");
    }
#ifdef SYSV
    p = rindex(p,'/') + 1;
    {
        char buf[8+DIRSIZ];
	strcpy(buf,"SYSTEM=");
	strcat(buf,getuname());
	setenv(buf);
	if( getenv("USER") == NULL) {
	    char *v;
	    if( (v=getenv("LOGNAME")) == NULL) {
		p = rindex(p,'/') + 1;
		strcpy(buf,"LOGNAME=");
		strcat(buf,p);
		setenv(buf);
	    } else {
		p = v;
	    }
	    strcpy(buf,"USER=");
	    strcat(buf,p);
	    setenv(buf);
	}
    }
#endif
    chdir(hcat);
    ini_shbuf(); mcpy_shbuf(hcat);
    exefound( strcat(ini, get_shbuf()) );

    getcat();
    get_suit();
    current = nfiles ? nfiles-1 : 0;

    ind_cat();

    if( setjmp(on_break) ) {
        if( nfiles )  strcpy( last_name, item(current)->d_name );
    }
    vdpbeg();
    Paint=YES;

#ifdef BSD
    sigsys(SIGCHLD,onchild); /* Сбросили прерывания */
#endif

    for(;;) {

        *last_name = 0;
        if( current >= nfiles or current < 0 ) current = nfiles? nfiles-1 : 0;

	signal( SIGINT, onbreak );

#       ifdef BSD
        if( setjmp(on_child) ) {
            if( nfiles )  strcpy( last_name, item(current)->d_name );
        }
#       endif

        if( nfiles ) {  /* Вводим символ (по возможности, асинхронно) */
            static int sq[] = {0, 4, 9, 16, 25};
            for( columns = sizeof sq/sizeof sq[0]; columns--; ) { /* формат */
                if( nfiles >= sq[columns] )  break;
            }
            ++columns;
            x0    = (xsize - columns*wcolumn) / (columns+1) + 1;
            width = x0 + wcolumn - 1;
            lines = (nfiles+(columns-1))/columns;
            INSYMB = dpj(nfiles, &current, lines, width, x0, y_table);
            pd = item(current);
        } else {
            if( (INSYMB=dpa()) < 0 ) {
                if(Paint) ind_cat();
                dpp( xsize/2-1, y_table );  INSYMB = dpi();
            }
        }
#       ifdef BSD
        sigsys(SIGCHLD,SIG_HOLD);    /* Не реагируем на завершение фоновых */
#       endif


        if( INSYMB == s_spec )  INSYMB = spec(dpi());
        ++com_com;
        noname = !nfiles or pd>=&cat[maxcat];    /* нет имени файла */
        /* Ищем реакцию на введенный символ */
        pcmd   = lookcom( INSYMB, noname ? 0 : pd->d_name );

        if( pcmd ) exefound(pcmd);
                                                                        /**/
        else {
            switch( INSYMB ) {

                case ctrl('A'):              /* перерисовка экрана */
                    vdpbeg();
                    goto newcat2;

                case ctrl('F'):              /* A B S A Z */
                    finish();                   /* or */
                    goto newcat2;

                case s_refuse:             /* отказ от действия */
                    p = 0;                        /* от пометки */
                    for(i=nfiles; --i>=0; ) {
                        if( item(i)->d_mark != 0 ) {
                            item(i)->d_mark = 0;
                            dpp(x0+(i/lines)*width-1, y_table+i%lines);
                            dps("  ");
                            ++p;
                        }
                    }
                    for( i=lind; --i>=0; ) {
                        pd = &cat[indcat[i]&0377];
                        if( pd->d_mark )  {++p;  pd->d_mark = 0;}
                    }
                    altcat[0] = nmarked = 0;
                    if( nalt )  {nalt = 0;  goto newsel;}
                    if( p != 0 ) continue;
                    else goto newsel;

                case de:
                    if( *select or selsuf ) {     /* от селекции */
                        ico_from = *select = selsuf = 0;
                        goto newsel;
                    }
                    dpo(bl); /* По ЗАБОЮ не вываливаемся наверх ! */
                    continue;

                case dc:                   /* отступ в селекции */
                    if( selsuf ) {
                        selsuf = 0;
                    } else {
                        if(*select==0) { dpo(bl); continue; }
                        select[strlen(select)-1] = 0;
                    }
                    ico_from = 0;
                    goto newsel;

                case s_erase:              /* уничтожение файлов */
                case f12:
                    rm();
                    goto newcat2;

                case '=':                  /* переименование объекта */
                    strcpy( last_name, chname( pd->d_name ) );
                    getcat();
                    goto look_oname;

                case ic:  case il:         /* копирование объекта */
                    strcpy(last_name,copy(noname?NULL:pd->d_name,INSYMB==il));
                    altcat[0] = 0;  nalt = 0;
                    last_mod = 0;
                    getcat();
                    goto look_oname;

                case '(':                  /* удаление параллельных */
                case ')':
                case '8':
                case '9':
                    i = to_kill();
                    if( i > 0 )  goto newcat2;
                    if( i < 0 )  dpo(bl);
                    continue;

                case ctrl('B'):              /* пометка ненужн.файлов */
                    for(i=nfiles; --i>=0; )  {
                        pd = item(i);
                        if( (p=lookcom('c'|0x200,pd->d_name)) and *p=='+' ) {
                            pd->d_mark = '-';
                            dpp(x0+(i/lines)*width-1, y_table+i%lines);
                            dps(" -");
                        }
                    }
                    continue;
                                                                        /**/
                case '+':                  /* смена режима показа */
                case ';':
                    only_x = !only_x;
                    goto newsel;

                case '>':
                case '<':
                case '*':                  /* пометка объекта */
                case ':':
                    iip(INSYMB==':' ? '*' : INSYMB);
                    if( pd >= &cat[maxcat] )  goto nx_pg;
                    if( noname or nmarked >= maxmark ) {
                        dpo(bl); continue;
                    }
                    marked[nmarked++] = indico[current];
                    if( ip == '*' ) {
                        i = pd->d_mark;
                        if( i<'1' || i>'9' ) i='0';
                        if( i=='9' )  { dpo(bl); continue; }
                        pd->d_mark = ++i;
                        if( i > '1' ) {dpo(cl);  dpo(i);}
                    } else {
                        pd->d_mark = ip;
                    }
                    dpo(p);
                    current = (current+1)%nfiles;
                    continue;

                case ' ':                  /* стирание 1 пометки */
                    if(!nfiles) { dpo(bl); continue; }
                    if( pd >= &cat[maxcat] )  goto nx_pg;
                    if( noname ) { current=(current+1)%nfiles; continue; }
                    iip(pd->d_mark);
                    if( p ) {
                        iip( ip>='2' && ip<='9' ? ip-1 : 0 );
                        pd->d_mark = ip;
                        if( nmarked ) {
                            for(;;) { /* ?? */
                              for( i=0; i<nmarked; ++i ) {
                                if(marked[i]==indico[current]) goto del_mark;
                              }
                              break;
                            del_mark:
                              for( ; i<nmarked; ++i ) marked[i] = marked[i+1];
                              --nmarked;
                            }
                        }
                    }
                    pd->d_mark = 0;
                    dpo(cl); dps("  ");
                    current = (current+1)%nfiles;
                    continue;

                case s_help:               /* выдать help */
                    help("comint",'*');
                    goto newsel;

                case '1':
                case '!':
                    ++shell_com;
                    shell();
                    nalt = 0;  altcat[0] = 0;
                    last_mod = 0;
                    goto newcat2;

                default:                   /* селекция имен */
                        fselect();
                        goto newsel;

                                                                        /**/
            newcat2:
                if( nfiles ) strcpy(last_name, item(current)->d_name);
                getcat();
                goto look_oname;

            nx_pg:
                if( pd == &cat[maxcat] ) {                        /* )---> */
                    ico_from += ico_from ? maxico-2 : maxico-1 ;
                } else {                                          /* <---( */
                    ico_from -= maxico-1;
                    if( ico_from ) ++ico_from;
                }
            newsel: /* изменение селекции (пытаемся сохранить позицию) */
                if(nfiles) strcpy(last_name, item(current)->d_name);
            look_oname:
                get_suit();
                current = nfiles;
                if( *last_name  &&  (i=search_ind(last_name)) >= 0 ) {
                    i = indcat[i] & 0377;
                    for( current=nfiles;  --current>=0; ) {
                        if( (indico[current] & 0377) == i )  break;
                    }
                }
                Paint=YES;

            } /* End switch */

        } /* End if (else) */

    } /* End for */

}
