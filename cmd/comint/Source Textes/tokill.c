/* УДАЛЕНИЕ ПАРАЛЛЕЛЬНЫХ ПРОЦЕССОВ */

/*
 * $Log:        tokill.c,v $
 * Revision 2.1  89/10/30  19:59:54  rygoff
 * Новая ветвь.
 * Заменен вызов rspec(). См. правки в Mkdir.c.
 * Фоновые процессы убиваем изо всех сил.
 * Почему-то раньше они иногда долго трепыхались,
 * приходилось их бить в явном виде (киллом из шелла).
 *
 * Revision 1.1  89/08/19  18:13:16  rygoff
 * Initial revision
 *
 *
 */

#include <signal.h>
#include <tty_codes.h>
#include "comint.h"
#include "run.h"

extern char *name_f, *name_g;

int to_kill() {
    register int i, n_run, sig;
    int          current;
    pid_t        pr;
    char *p;

    for( n_run=i=0; i<maxrun; ++i )  {
	if( at_run[i].r_pid > 0 ) ++n_run;
	at_run[i].r_mark = 0 ;
    }
    if( !n_run )  return( -1 );          /* нет параллельных процессов */
    current = 0;

inp:
    switch( dpt(n_run, &current, 1, w_run, x_run, y_run) ) {
	default:         dpo(bl);  goto inp;

	case '?':
	case '"':
	case '2':
	case '1':
			 name_f=name_g=NULL;
			 if( p=lookcom('d'|0x0200,NULL) ) exefound(p);
			 ind_cat();  break;

	case s_help:     help("comint",'&');

	case '(':
	case ')':
	case s_refuse:   break;

	case '-':
	    dpo('-');
	    pr = current;
	    for( i=0; i<maxrun; ++i ) {
		if( at_run[i].r_pid > 0 )  if( !(pr--) )  break;
	    }
	    at_run[i].r_mark = '-';
	    current = (current+n_run-1) % n_run;
	    goto inp;

	case rn:
	case f12:
	case s_erase:
	kl: for( i=maxrun; --i>=0; )  {
		if( at_run[i].r_mark=='-' && (pr=at_run[i].r_pid) > 0 ) {
		    register j;
		    for(j=3;j--;) { /* rygoff 30.10.89 Для надежности */
			kill(pr,SIGINT);
			kill(pr,SIGTERM);
			kill(pr,SIGQUIT);
			kill(pr,SIGKILL);
		    }
		}
	    }
	    break;
    }
    ind_run();
    return(0);
}
