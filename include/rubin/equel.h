/*
 * equel.h
 * (Должен размещаться на /usr/include/rubin/equel.h)
 * Описания основных типов и переменных в EQUEL
 * (точнее, в библиотеке libq.a)
 */

/* 1. Управление печатью ошибок */
extern int  IIerrflag;           /* Признак - были ошибки      */
extern IIret_err(), IIno_err();  /* Заглушки обработки ошибок  */
extern int  (*IIprint_err)();    /* Указатель на п/п обработки */
extern int  (*IIout_err)();      /* Программа выдачи строки диагностики */
				 /* По умолчанию (если NULL)    - puts */
extern  int     (*IIabort)();    /* Программа аварийного завершения.   */
extern  int     (*IIinterrupt)(); /* Программа обработки прерывания */
/* 2. Информационные переменные */
extern  int     IItupcnt;       /* Число считанных/записанных кортежей */
extern  char    *IImainpr;      /* Имя вызываемой системы       */

/* 3. Некоторые макро */
#define         EQ_ERR          (IIerrflag != 0)
#define         EQ_ONERR        IIprint_err = IIret_err
#define         EQ_OFFERR       IIprint_err = IIno_err

/* 4. Типы данных */
typedef short  EQ_i2;
typedef long   EQ_i4, EQ_date, EQ_time;
typedef char   EQ_c;
typedef float  EQ_f4;
typedef double EQ_f8;
