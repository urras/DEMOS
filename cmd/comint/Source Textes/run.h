/* ОПИСАНИЕ ЗАПУЩЕННЫХ В ПАРАЛЛЕЛЬ ПРОЦЕССОВ */

/*
 * $Log:        run.h,v $
 * Revision 1.1  89/08/19  18:11:15  rygoff
 * Initial revision
 * 
 *
 */


#define maxrun 4                 /* число параллельных процессов */
#define x_run  3                 /* позиция имени на экране */
#define w_run  (D_NAME+5)        /* ширина столбца */

int   ch_flag;                   /* проверять ли процессы */

struct run {
    int  r_pid;                  /* дескриптор процесса */
    int  r_code;                 /* код возврата при завершении */
    int  r_mark;                 /* пометка (только -) */
    char r_name[D_NAME+1];       /* имя файла */
} at_run[maxrun];

#ifdef USG
#  undef SIGCHLD
#  define SIGCHLD SIGCLD
#else
#  define fork vfork
#endif
