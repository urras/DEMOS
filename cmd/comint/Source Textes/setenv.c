/*
 * setenv - установка значений переменных среды.
 * 
 * $Log:        setenv.c,v $
 * Revision 1.4  89/12/18  19:39:19  rygoff
 * Сняты ограничения. Все работает.
 * 
 * Revision 1.3  89/12/02  13:32:05  rygoff
 * Упрощен алгоритм.
 * Введено ограничение: могут изменяться только те переменные,
 * которые ранее присутствовали в среде.
 * Новые переменные вводить нельзя.
 * 
 */
#include <tty_codes.h>
#include <string.h>
#include "comint.h"
#define ENVSIZE 64        /* Max. число переменных в среде */
#define VARLEN 1024       /* Длина имени=значения */

extern  char **environ;
char env1[ENVSIZE][VARLEN];
char *new_env[ENVSIZE];

int inienv() {
    register i;
    for(i=0; i<ENVSIZE;) new_env[i++]=0;
    for(i=0; i<ENVSIZE and environ[i]; i++) {
        if( strlen(environ[i])<VARLEN )
            new_env[i]=strcpy(env1[i], environ[i]);
    }
    environ=new_env;
}



char *setenv(s) char *s; {
    register int i;
    int llen=index(s,'=')-s+1;    /* Длина имени переменной с '=' */

    if( ! llen ) return((char *)-1);
    for(i=0; environ[i]; i++) { /* Ищем такую переменную в среде */
        if(!strncmp(environ[i],s,llen)) {  /* Нашли */
            if(strlen(s)>=VARLEN) {
                dpp(0, 0); dpo(es);
                dpp(0, ysize-1);
                dps("Слишком длинное имя: "); dps(s); dpi();
                return((char *)-1);
            }
            environ[i]=strcpy(environ[i],s);
            return((char *)0);
        }
    }
                                            /* Не нашли */
    if(i<ENVSIZE) return( environ[i]=strcpy(env1[i],s) ); else {
        dpp(0, 0); dpo(es);
        dpp(0, ysize-1);
        dps("Среда переполнена "); dpi();
        return((char *)-1);
    }
}
