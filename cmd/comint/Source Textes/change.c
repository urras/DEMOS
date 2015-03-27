/*
 *      Смена каталога/библиотеки с выполнение инициализирующих
 *      и терминирующих действий.
 *
 *      Смена селекции
 *
 *      Конец работы
 *
 * $Log:        change.c,v $
 * Revision 2.2  89/11/22  16:21:55  rygoff
 * Исправлена плюха с подъемом с первого уровня на нулевой.
 * (например, при подъеме из /tmp неправилино работало 
 * копирование помеченных файлов - портилось полное имя)
 * 
 * Revision 2.1  89/10/30  19:12:47  rygoff
 * В связи с ревизией 2.1 (см. main.c, exefound.c) 
 * вынес сюда некоторые действия, чтобы упростить структуру 
 * управления в exefound.c и main.c
 * Это рекурсивная процедура (косвенная рекурсия через exefound())
 * 
 */
#include <tty_codes.h>
#include "comint.h"

extern int     is_shell, INSYMB, com_com, shell_com;
extern char    hcat[], pattern[], last_name[], *name_f, *name_g,
               *rindex(),
               *lookcom();

static int level=0;


to_head() {/* Прощание с каталогом и подъем по дереву в вышестоящий каталог */
        register char *p, *s;

        if( nmarked and hcat[0]!='/' ) { ind_lerr(1,0); dpo(bl); return; }
        for(p=s=hcat; *p; ++p) if( *p=='/' )  s = p;
        if( *s != '/' ) {
            if( chdir("..") == -1 )  { dpo(bl); return; }
            strcpy(hcat,".");
        } else {
            char letter=0;                /* rygoff 22.11.89 */
            strcpy(last_name,s+1);
            if( s == hcat ) letter= *++s; /* rygoff 22.11.89 */
            if( *s==0 )  { dpo(bl); return; }  /* нельзя подняться из "/" */
            if( arname==NULL ) {
                register i;
                name_f=name_g=NULL;
                if(i=(int)lookcom('q'|0x0200,NULL)) exefound((char*)i);
                *s = 0;
                i = chdir( hcat[0]=='/' ? hcat : ".." );
                *s = letter ? letter :  '/';
                if( i == -1 )  { dpo(bl); return; }
            } else {
                --level;
                if( (p=rindex(pattern,'/')) == NULL )
                    pattern[0] = '\0';
                else
                    *p = '\0';
            }
            savmark();
            *s = 0;
            if( arpath ) {if( arpath>=s )  arpath = NULL;}
            else          arname = NULL;
        }
        gettune();
        name_f=name_g=NULL;
        if(s=lookcom('s'|0x0200,NULL)) exefound(s);
        last_mod = ico_from = *select = selsuf = 0;
        getcat();
}


to_tail(p, LIB ) register char *p;/* Прощание с каталогом и спуск по дереву */
                 register boolean LIB;     /* Или вход в файл как в каталог */
{
        register i; /* Also used as *char */

        if( nmarked and hcat[0]!='/' ) {ind_lerr(1,0); dpo(bl); return; }
        i = *p=='/' ? 0 : strlen(hcat);
        if( strlen(p) + 1 + (int)i > lname ) { dpo(bl); return; }   /*длинное имя*/

        if(not LIB) {
            level=0;
            pattern[0]='\0';
            if( chdir(p) == -1 ) { ind_err(NULL,0); dpo(bl); return; }
            arname = NULL;  arpath = NULL;
        } else {
            level++;
            if(level>1) {
                if(level>2) strcat(pattern,"/");
                strcat(pattern, p );
            }
            if(arname==NULL)
                arname=strncpy(arbuf,p,D_NAME);
            else if(arpath==NULL)
                arpath=hcat + (int)i + 1;
        }
        savmark();
        name_f=name_g=NULL;
        if(i=(int)lookcom('t'|0x0200,NULL)) exefound((char*)i);
        if(*p == '/') hcat[0] = 0;
        else if(strcmp(hcat,"/")) strcat(hcat,"/");
        strcat(hcat,p);
        gettune();
        nfiles=0;
        name_f=name_g=NULL;
        if(not LIB and (p=lookcom('p'|0x0200,NULL)) ) exefound(p);
        last_mod = ico_from = *select = selsuf = 0;
        getcat();
}



fselect() {      /* Селекция имен */

        if( not is_print(INSYMB&0xff) ) { dpo(bl); return; }
        if( INSYMB & 0x100 ) {  /* селекция по суффиксу */
            selsuf = INSYMB & 0xff;
        } else {
            register i;
            if( (i=strlen(select)) >= D_NAME ) { dpo(bl); return; }
            select[i] = (char)INSYMB;
            select[i+1] = 0;
        }
        ico_from = 0;
}

finish() {
      register char *p;

      if( is_shell ) {
          dpp(0,y_head);      dps("   Вы закончили ? ");
          if( not index("yYдД",dpi()) ) return;  /* No ! */
          dpp(0,y_head);  dps("До свидания !     ");
          dps10((unsigned long)com_com,0);  dps(" (");
          dps10((unsigned long)shell_com,0);  dpo(')');
      }

      name_f=name_g=NULL;
      if(p=lookcom('o'|0x0200,NULL)) exefound(p);
      vdpend();
      exit(0);
}
