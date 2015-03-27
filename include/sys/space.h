/*
 *      $Log:	space.h,v $
 * Revision 1.4  89/05/13  15:32:10  abs
 * копия данного файла должна попадать в /usr/include/sys
 * 	после каждой генерации ядра.
 * 
 * Revision 1.3  89/05/11  18:01:23  abs
 * из msg.h и sem.h константы перетащены сюда.
 *
 * Revision 1.2  89/05/05  19:52:00  korotaev
 * В этот файл вставлен MAXUSERS.
 *
 * Revision 1.1  89/04/24  17:03:17  korotaev
 * Initial revision
 *
 */

#ifndef _sys_space_h_
# define _sys_space_h_

/*
 *      Все инициализирующие и инициализирующиеся константы
 *      вынесены в этот файл (изначально получен разбиением param.c)
 */

#ifdef KERNEL
#include        "h/video.h"
#endif

#if    NVI > 0
#define HZ      64      /* У Электроники-85 такая частота */
#else
#define HZ      50                 /* Ticks/second of the clock */
#endif NVI

#define TIMEZONE ((-3) * 60) /* Minutes westward from Greenwich */
#define DSTFLAG  1             /* Daylight Saving Time applies here */
#define MAXUSERS 5

#define NBUF    220             /* size of buffer cache, must be <=256*/
#define NMOUNT  8           /* number of mountable file systems */

#ifdef  UCB_CLIST
#   ifdef UNIBUS_MAP
#       define NCLIST   500        /* number of clists, must be <= 512 */
#   else
#       define NCLIST   200        /* number of clists */
#   endif
#else   UCB_CLIST
#       define NCLIST   100        /* number of clists */
#endif  UCB_CLIST

#define NPROC   90            /* max number of processes */
#define NTEXT   45            /* max number of pure texts */
#define NINODE  140           /* number of in-core inodes */
#define NFILE   160            /* number of file structures */
#define NCALL   30            /* max simultaneous time callouts */
#define NDISK   3                  /* number of disks to monitor */

#define NFLOCK  580                /* Кол-во слотов таблицы flock */

/* --------- IPC semaphore facility (see sys/sem.h) ----------------- */
#define SEMMNS  20      /* число семафоров в системе (примерно 20) */
			      /* если не нужны, то установите 0 */
#if SEMMNS > 0
#define SEMMAP  (SEMMNS/2+1)  /* worst case */
#define SEMMNI  (SEMMNS/3)      /* число групп семафоров */
#define SEMMNU  ((SEMUME*SEMMNS)/6)      /* всего процессов, которые могут ожидать
			       * открытия семафоров с флагом UNDO
			       */
#define SEMMSL   13  /* количество семафоров, над которыми одновременно
		      * можно выполнить операцию
		      */
#define SEMOPM  (SEMUME+1) /* SEMOPM примерно равно SEMUME */
#define SEMUME  (3+MAXUSERS/5)   /* макс. число процессов, которые могут ждать
			    * по одному семафору с флагом UNDO
			    */
#define SEMVMX   32000  /* макс. значение семафора */
#define SEMAEM   255    /* макс. приращение семафора */
#endif SEMMNS

/* ---------- IPC message facility (see sys/msg.h) --------------------- */
#define MSGMNI  2  /* число очередей сообщений (возможно 2 и больше) */
			  /* 0, если вам не нужен механизм messages */
#if MSGMNI > 0
/*
 ** local implementation constants
 **   PDP-11  dependent
 */

#define TOTAL    8192
/* общее число памяти под тексты сообщений, кратное 64 */

/* максимальный размер текста одного сообщения. На pdp11 не должен
 * превышать размера 1 листа памяти
 */
#if (TOTAL/2) > 8192
# define MSGMAX 8192
#else
# define MSGMAX (TOTAL/2)
#endif

#define MSGTQL (MAXUSERS*3)     /* число сообщений в системе */
#define MSGMAP (MSGTQL/3)
	/* хотя по-хорошему надо бы (для худшего случая) (MSGTQL/2+1) */
#define MSGSSZ   64
	/* размер одного сегмента памяти.
	 * На PDP-11 это размер клика - 64 байта
	 */
#define MSGMNB   TOTAL
	/* максимальная суммарная длина текстов сообщений
	 * в одной очереди. Логично было бы (TOTAL/MSGMNI),
	 * но я сделаю, что даже если активна одна очередь
	 * - то доступна вся память
	 */

#define MSGSEG (TOTAL/MSGSSZ)
	/* число сегментов памяти, выделенных под тексты */

#endif MSGMNI

/* ---------- UNIBUS MAP ------------------------------------------------- */
#ifndef UNIBUS_MAP
#   define CMAPSIZ (NPROC*2)              /* size of core allocation map */
#   define SMAPSIZ (NPROC*2+(5*NTEXT/10)) /* size of swap allocation map */
#else
#   define CMAPSIZ (NPROC+(8*NTEXT/10)) /* size of core allocation map */
#   define SMAPSIZ (NPROC+(8*NTEXT/10)) /* size of swap allocation map */
#endif

#endif _sys_space_h_
