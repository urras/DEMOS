/* ЖДЕМ ПРОЦЕСС 'pid' И ВОЗВРАЩАЕМ КОД ОШИБКИ (системные < 0 )
   Если pid == 0, то просто проверяет состояние процессов */

/*
 * $Log:        waitpid.c,v $
 * Revision 1.1  89/08/19  18:13:29  rygoff
 * Initial revision
 * 
 *
 */

#include <signal.h>
#include <tty_codes.h>
#include "comint.h"
#include "run.h"
#ifdef BSD
#    include <wait.h>
#endif

int wait_pid( pid )
    int       pid;
{
    register int i, pr;
    register void (*old_trap)();
    int          stt;

    old_trap = signal(SIGINT,SIG_IGN);
    for(;;) {
#ifdef BSD
        pr = pid ? wait(&stt) : wait2(&stt,WNOHANG);
#else
        pr = wait(&stt);
#endif
        if( pr <= 0  ||  pid == pr )  break;
        for( i=maxrun; --i>=0; )  {
            if( at_run[i].r_pid  == pr ) {
                at_run[i].r_pid  = -1;
                pr               = stt & 0177;
                at_run[i].r_code = !pr ? ((stt>>8)&0377) : -pr ;
                break;
            }
        }
    }
    signal(SIGINT,old_trap);
    ch_flag = 0;
    pr      = stt & 0177;
    return( !pr ? ((stt>>8)&0377) : -pr );
}
