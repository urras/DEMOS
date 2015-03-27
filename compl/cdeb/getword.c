/* === CDEBUGGER === ИНТЕРФЕЙС ОТЛАДЧИКА С ОТЛАЖИВАЕМОЙ ПРОГРАММОЙ */

/* полностью локализован в данном модуле.

   точки входа:
        get_word(адрес)         - чтение слова из адресного пространства
                                  отлаживаемой задачи
        get_iword(адрес)        - чтение слова из адресного пространства
                                  отлаживаемой задачи, область команд
                                  (только в режиме "SEPID")
        set_word(адрес,значение)- запись слова в адресное пространство
                                  отлаживаемой задачи
        init_task()     - начальная установка отладчика перед первой
                          строкой отлаживаемой задачи
        run_task()      - запуск задачи с указанным состоянием регистров с
                          текущей точки. останов в точке stp_adr,
                          на следующей команде или по событию, в завис. от
                          stp_flag. по возврате заполняет состояние
                          задачи на момент останова с указанием причины. */

/* #define DEB /* */

#include "cdeb.h"
#include <signal.h>

int  sregval[9];
int  pid;                               /* идентификатор отлажив. задачи */


/* запросы к системному вызову PTRACE */
#define SETTRC  0                       /* об'явить, что тебя трассируют */
#define RUSER   2                       /* чтение из "data" задачи */
#define RIUSER  1                       /* чтение из "text" задачи */
#define WUSER   5                       /* запись в  "data" задачи */
#define RUREGS  3                       /* чтение из сист. обл. данных   */
#define WUREGS  6                       /* запись в  сист. обл. данных   */
#define CONTIN  7                       /* запуск задачи                 */
#define EXIT    8                       /* завершение отлаживаемой задачи*/


/* расположение регистров в системной области данных */
#define PS      -1
#define PC      -2
#define SP      -6
#define R5      -9
#define R4      -10
#define R3      -11
#define R2      -12
#define R1      -5
#define R0      -3
#define a(rr)   2*(512+rr)
int regadr[] = {a(R0),a(R1),a(R2),a(R3),a(R4),a(R5),a(SP),a(PC),a(PS)};

int _exectrap;


/* ЗАПУСК И ИНИЦИАЛИЗАЦИЯ ОТЛАЖИВАЕМОЙ ЗАДАЧИ */

init_task(argc, argv)
    char **argv;  int  argc;
{
#   define T_bit   020
    int onintr();
    int stat, w;
    int Ruid, Rgid;                     /* для изменения приоритета      */

    if( argc < 2 ) fatal_error( 5 );    /* неверные параметры DEBUG      */
    signal(SIGQUIT,SIG_IGN);            /* запретили  ctrl-\ отладчику   */

#ifdef МНОС
    Ruid = getuid()&0377;
    Rgid = getgid()&0377;
#else
    Ruid = getuid();
    Rgid = getgid();
#endif

    pid = fork();                       /* запускаем отлаж. задачу       */
    if( !pid ) {                        /* процесс-сын -> его отлаживают */
        setuid(Ruid); setgid(Rgid);
        signal(SIGQUIT,SIG_DFL);        /* разрешили ctrl-\              */
        _exectrap = 1;
        ptrace(SETTRC, 0, 0, 0);
        execv( argv[1], &argv[1] );     /* запустили отлаж. задачу       */
        fatal_error( 6 );
    }

    signal(SIGINT, onintr);             /* запретили  ctrl-C и           */
    signal(SIGQUIT,onintr);             /*            ctrl-\ отладчику   */

    nice(-5);                           /* повышение приоритета отладчика*/
    setuid(Ruid); setgid(Rgid);

    while( (w = wait(&stat)) != pid  &&  w != -1 );  /* ждем останова    */
    if( w == -1 || ((stat>>8)&0177) != SIGTRAP )     /* системная ошибка */
        fatal_error( 2 );
    regval[8] = (~T_bit) & ptrace( RUREGS, pid, regadr[8], 0);
    if( errno ) fatal_error( 7 );
    set_word( CDEBFLAG, 1 );            /* ставим флаг отладки в 'cdebug'*/

    stp_adr = 0;  run_task();           /* запуск до 1-ой выполн. строки */
}


get_reg_value()                         /* считывает значения регистров  */
{                                       /* и точку останова (адр., стр.) */
    register int i;
    errno = 0;
    for( i=0; i<8; ++i)
        sregval[i] = regval[i] = get_word( CDEBFLAG - 16 + 2*i );
    regval[6] += 2;
    sregval[7] = 0;
    act_adr    = regval[7];
    act_line   = get_iword( act_adr-2 );

#ifdef DEB
printf("act_line=%06o \n",act_line);
printf("from=%06o leng=%06o value=%06o\n", get_word( CDEBFLAG+FROM ),
get_word( CDEBFLAG+LENG ), get_word( CDEBFLAG+VALUE ) );
#endif

    language   = act_line>>14;
    act_line  &= (1<<14)-1;
}


run_task()
{
    register int i, w;
    int stat;
    static int iniflag=0, setregflag=0;

    errno = 0;                      /* устанавливаем измененные регистры */
    for(i=0; i<9; ++i) {            /*          (всегда pc и sp)         */
        if( setregflag  ||  regval[i] != sregval[i] ) {
            ptrace( WUREGS, pid, regadr[i], regval[i] );
            sregval[i] = regval[i];
        }
    }
    if( errno )  fatal_error( 2 );
    setregflag=0;

    set_word( CDEBFLAG+STPADR , stp_adr );      /* ставим адрес останова */
    if( !jsr_ign )                              /* не входить в функции  */
        set_word( CDEBFLAG+PFRAME, regval[5] );
    else if( jsr_ign < 0 )                      /* выйти из функции      */
        set_word( CDEBFLAG+PFRAME, regval[5]+1 );
    else                                        /* входить и выходить    */
        set_word( CDEBFLAG+PFRAME, 0 );

    errno = 0;

#ifdef DEB
printf("cdebflag=%06o pframe=%06o stpadr=%06o\n", get_word( CDEBFLAG ),
get_word( CDEBFLAG+PFRAME ), get_word( CDEBFLAG+STPADR ) );
#endif

    ptrace(CONTIN, pid, 1, 0);                      /* запускаем процесс */
    if( errno )  fatal_error( 2 );
    while( (w = wait(&stat)) != pid  &&  w != -1 );    /* и ждем стопора */
    if( w == -1  ) fatal_error( 2 );

    get_reg_value();
    stp_cause = (stat>>8)&0177;                      /* причина останова */
    if( stp_cause == SIGTRAP )
        stp_cause = 0;                               /*      свой стопор */
    else
        setregflag = 1;                              /*      событие     */

    set_word( CDEBFLAG+FROM, 0 );

    look_file(act_adr);
}

#ifdef SEPID
get_iword(adr)
{
    register int word;
    errno=0;
    word = ptrace(RIUSER, pid, adr, 0);
    if( errno ) {
        errno = 0;                               /* задача завершилась ? */
        ptrace(RUSER, pid, 0, 0);
        if( errno )  exitdeb();                          /* да  */
        error( 6 );                                      /* нет */
    }
    return( word );
}
#endif

get_word(adr)
{
    register int word;
    errno=0;
    word = ptrace(RUSER, pid, adr, 0);
    if( errno ) {
        errno = 0;                               /* задача завершилась ? */
        ptrace(RUSER, pid, 0, 0);
        if( errno )  exitdeb();                          /* да  */
        error( 6 );                                      /* нет */
    }
    return( word );
}


set_word(adr,value)
{
    errno=0;
    ptrace( WUSER, pid, adr, value );
    if( errno )  error( 9 );
}
