/* ПОКАЗ ЗАПУЩЕННЫХ В ПАРАЛЛЕЛЬ ПРОЦЕССОВ */

#include <tty_codes.h>
#include "comint.h"
#include "run.h"

ind_run() {
    register int         i, n_run;
    register struct run *p;
    char                *pn;
    int                  i_run, c, fout;
    static int           last_run = 0;

    i_run = 0;  n_run = 0;                /* число запущенных = ? */
    for( i=maxrun; --i>=0; )  if( at_run[i].r_pid )  ++n_run;

    if( n_run ) {
        if( ch_flag ) {              /* проверяем завершение процессов */
#ifndef BSD
            if( (i=fork()) == 0 )  exit(0);
            wait_pid(i);
#else
            wait_pid(0);
#endif
        } else {
            ++ch_flag;
        }

        dpp(0,y_run);  dpo( '(' );

        for( i=0; i<maxrun; ++i ) {             /* показ выполняющихся */
            p = &at_run[i];
            if( p->r_pid > 0 ) {
                dpp( x_run + i_run*w_run, y_run );
                dpo(' ');  dps( p->r_name );
                dpn(w_run-1-strlen(p->r_name),' ');
                ++i_run;
            }
        }

        for( i=0; i<maxrun; ++i ) {             /* показ завершившихся */
            p = &at_run[i];
            if( p->r_pid < 0 ) {
                register int cdd;
                cdd = p->r_code;
                c = !cdd ? '+' : ((cdd>0) ?'?' :'-'); /*вид завершения*/
                dpp( x_run + i_run*w_run, y_run );
                if( cdd )  dps( conv(cdd<0 ? -cdd : cdd ,3,10,' ') );
                else       dps( "   " );
                dpo(c);  dps( p->r_name );
                dpn(w_run-4-strlen(p->r_name),' ');

                dpo(bl);                     /* удаляем файл протокола */
                pn = prot_name(p->r_name);   /*           если он пуст */
                fout = open(pn,0);
                if( fout > 0 and lseek(fout,0l,2)==0 ) {
                    unlink(pn);   last_mod=0;
                }
                close(fout);

                p->r_pid = 0;
                ++i_run;
            }
        }

        dpp(x_run+n_run*w_run,y_run);  dpo( ')' );
    }

    if( last_run != n_run ) {                 /* стираем старые выдачи */
        last_run = n_run;
        if( !n_run )  {                               /* стираем (...) */
            dpp(0,y_run);                     dpo(' ');
            dpp(x_run+last_run*w_run,y_run);  dpo(' ');
        }
        while( last_run > n_run ) {                   /* стираем имена */
            dpp( x_run + n_run*w_run, y_run );  dpn(w_run,' ');
            ++n_run;
        }
    }
}
