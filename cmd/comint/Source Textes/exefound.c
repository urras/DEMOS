/*
 *      Выполнение командной строки из файла настройки.
 *      Запускает сам себя, если при смене каталога требуется
 *      настройка, т.е. lookcom находит командную строку
 *      при входе и/или выходе в(из) каталог(а).
 *      ( косвенная рекурсия через to_head() и to_tail() )
 *
 * $Log:        exefound.c,v $
 * Revision 2.8  89/11/15  16:44:40  rygoff
 * Немного перетасовал метки (убрал nxfile:).
 * Перед запуском start() запрещаем прерывание по SIGCHLD
 * (ставим SIG_IGN вместо текущего SIG_HOLD).
 * 
 * Revision 2.7  89/11/15  14:42:36  rygoff
 * *** empty log message ***
 * 
 * Revision 2.6  89/11/15  14:19:47  rygoff
 * 
 * Revision 2.5  89/11/11  22:47:37  rygoff
 * Немного поправил _! и _?
 * 
 * Revision 2.4  89/11/09  10:36:31  rygoff
 * Мелкие правки
 * 
 * Revision 2.3  89/11/07  12:43:48  rygoff
 * Добавил встроенные команды _! и _?
 * 
 * Revision 2.2  89/11/03  13:18:39  rygoff
 * После фильтрации пробелов в строке p сделал шаг назад,
 * иначе p указывала на второй символ в строке.
 * 
 * Revision 2.1  89/10/30  19:29:24  rygoff
 * Раньше в инициализирующих и терминирующих действиях
 * использовалась подпрограмма start(), запускаемая через rspec().
 * Поэтому не допускалось использование встроенных (спец) команд
 * comint'а.
 * Теперь вместо start() рекурсивно запускается эта подпрограмма
 * (косвенно, через функции, описанные в change.c
 * )
 * 
 */

#include <signal.h>
#include <tty_codes.h>
#include "comint.h"
#include "run.h"

extern char last_name[];
extern boolean noname;
extern boolean Paint;
#undef pd
extern struct dir2 *pd;
extern int INSYMB;
char answ_symb, answ_line[80];



exefound( pcurr ) register char *pcurr; {
#   define BufLen 256
    char buf[BufLen];  /* Буфер для разрезанных командных строк */
    int pos, len;

    register char *p, *fin, *p2 /* Also used as integer */;
#   define ip2          (short)p2
#   define iip2(x)      (p2 = (char *)(x))

    p = (p2=buf) + BufLen - 1;
    while( (*p2++= *pcurr++)!='\n' ) if(p2>=p) goto err;
    *--p2='\0';
    p=buf;

    while(pcurr=p) {

      add:

        while( (p=index(++p,';')) && *(p-1) == '\\' );/* Ищем ; без экрана */
        if(p) { p2=p++; while(*p++==' ') {;} p--;  } /* Ищем начало след. */

        if( *pcurr != '_' ) {   /* Текущая команда - обычная  */
            if(p) {             /* Есть продолжение строки после ';' : */
               if(*p!='_') goto add;    /* - обычная строка */
               *p2='\0';                /* - спецкоманда - будем резать */
            }
#ifdef BSD
            signal(SIGCHLD, SIG_IGN);
#endif
            start( show_type(pcurr), INSYMB, NO );
            last_mod = 0;
            if(nfiles) strcpy(last_name, item(current)->d_name);
            getcat();
            goto look_oname;

        } else {            /* Текущая - спецкоманда, сразу отрезаем  */

            register c = *++pcurr;

            pcurr++;
            *p2='\0';
            ini_shbuf();
            cm_line(pcurr);
            pcurr = get_shbuf();
            no_esc(pcurr);

            switch( c ) {

              case '!':                       /* Ввод одного символа */
                  if(*pcurr) { dpp(0,ysize-1); dpo(el); dps(pcurr); }
                  INSYMB = answ_symb = dpi();
                  if( not is_print(answ_symb) ) answ_symb=0;
                  continue;

              case '?':                       /* Ввод строки */
                  pos=0; len=xsize-strlen(pcurr);
                  dpp(0,ysize-1); dpo(el); dps(pcurr);
                  fin= (p2=answ_line) + len;
                  for( ;p2<fin; ) *p2++=' ';
                  dpr(answ_line, len, &pos, on_control|MS_mode);
                  for( ;p2>answ_line && *--p2==' '; ) *p2='\0';
                  continue;

              case 'a':                       /* информация о файле */
                  ls(pcurr);
                  current = (current+1)%nfiles;
                  continue;

              case 'b':                       /* показ файла */
                  catf(pcurr);
                  goto newsel;

              case 'c':                       /* переход в каталог */
                  to_tail(pcurr, 0);
                  goto look_oname;

              case 'd':                             /* конец работы */
                  finish(); /*   OR   */ goto newsel;

              case 'e':                       /* подъем по дереву файлов */
                  to_head();
                  goto look_oname;

              case 'f':                       /* вход в файл как в катал.*/
                  if( noname )  goto err;
                  to_tail(pd->d_name, 1);
                  goto look_oname;

              case 'g':                       /* селекция по символу */
                  fselect();
                  goto newsel;

              case 'i':                       /* пометить - с проверкой */
                  if( pd >= &cat[maxcat] ) goto nx_pg;
                  if( pd->d_mark != *pcurr and pd->d_mark != '-' ) {
                      dpo(bl);  dpo(cl);  dpo(pd->d_mark = *pcurr);
                      continue;
                  }

              case 'h':                       /* пометить - */
                 if( pd >= &cat[maxcat] )  goto nx_pg;
                 dpo('-');
                 pd->d_mark = '-';
                 current = (current+1)%nfiles;
                 continue;

              case 'j':                       /* создать новый объект */
                  strcpy( last_name, Mkdir(pcurr) );
                  if( *last_name == 0 ) continue;
                  getcat();
                  goto look_oname;

              case 's':                       /* установка environ'а */
                  setenv(pcurr);
                  continue;

              case 'z':                       /* ошибочная команда */
                  dpo(bl);
                  if(*pcurr) { dpp(0,ysize-1); dpo(el); dps(pcurr); dpo(bl); }
                  continue; /* Все же продолжаем */

              default:                        /* ошибочная команда */
              err:
                  dpo(bl);
                  return;

            } /* End switch */

        } /* End if */

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
        if( *last_name  &&  (iip2(search_ind(last_name))) >= 0 ) {
            iip2( indcat[(short)p2] & 0377 );
            for( current=nfiles;  --current>=0; ) {
                if( (indico[current] & 0377) == ip2 )  break;
            }
        }
        Paint=YES;

    } /* End while */
}
