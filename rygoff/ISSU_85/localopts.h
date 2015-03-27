/*
 * Локальные параметры системы ( подключаются через whoami.h).
 *
 * $Log:	localopts.h,v $
 * Revision 1.11  88/09/30  21:02:06  root
 * SMALL за-ifdef-но KERNEL-ом
 * 
 * Revision 1.10  88/09/30  17:28:23  root
 * EXMOUNT по-умолчанию теперь включается.
 * 
 * Revision 1.9  88/09/01  14:45:33  korotaev
 * Перенесен один define из whoami.h (NONFP).
 * Изменены умолчания в GENERIC'е (отключена обработка все статистики).
 * 
 * Revision 1.8  88/06/26  19:17:47  korotaev
 * Изменены некоторые умолчания.
 * 
 * Revision 1.7  88/06/23  21:48:41  ache
 * Добавлен 31 оверлей в ядре (IPK_XKOV)
 * 
 * Revision 1.6  88/06/13  16:09:03  korotaev
 * Вставлен IPK_SELECT (from TCP/IP).
 * 
 * Revision 1.5  88/04/02  17:29:37  korotaev
 * Включен новый планировщик и изменены некоторые умолчания.
 * 
 * Revision 1.4  87/12/09  16:49:03  andrew
 * при желании теперь можно иметь 31 оверлей, а не только 7
 * 
 * Revision 1.3  87/12/08  12:01:04  root
 * Вставлена возможнотсь подключать IPK_FLOCK.
 * 
 * Revision 1.2  87/09/23  12:05:32  root
 * Вставлен IPK_TRUNCATE.
 *
 * Revision 1.1  86/04/19  15:02:28  avg
 * Initial revision
 *
 */

#ifndef _sys_localopts_h_
#       define  _sys_localopts_h_

/*
 *      Приняты следующие обозначения в комментариях:
 *      *+ - обычно включены
 *      *- - обычно выключены
 *      *++ - выключать не рекомендуется
 *      *-- - включать не рекомендуется
 *      *++? - включены, выключение не работает
 *      *--? - выключены, включение не работает
 *      *+? - включены, выключение не проверялось
 *      *-? - выключены, включение не проверялось
 */

/*
 * Настройка системы. Режимы, открывающие новые возможности
 * для пользователя
 */

#define CGL_RTP              /*- Возможен 1 суперприоритетный процесс */
/* #define DISKMON              /*- собирать статистику по буферам для iostat */
/* #define UCB_GRPMAST          /*- поддерживается "администратор группы" */
/* #define UCB_LOGIN            /*- есть системный вызов "login" */
/* #define UCB_QUOTAS           /*- включено квотирование дисков */
/* #define TEXAS_AUTOBAUD       /*- автоматический подбор скорости tty */
/* #define UCB_UPRINTF             /*+ сообщения об ошибках посылаются user-у */
/* #define UCB_VHANGUP          /*- При выходе user-a посылается HANGUP */
/* #define UCB_LOAD                /*+ статистика по загрузке системы */
/* #define UCB_METER            /*- статистика по подкачке */
/* #define  FXMETER             /*- статистика по вторичному использ. text */
#define UCB_RENICE              /*+ renice system call */
#define IPK_TRUNCATE            /*++ включен системный вызов [f]truncate */
#define IPK_FLOCK               /*++ включен системный вызов flock */
#define IPK_XOVLY               /*++ включен 31 пользовательский оверлей
				 * требует включения MENLO_OVLY (см. ниже) */
/* #define IPK_SELECT           /*- системный вызов select без UCB_NET'а */
/* #define VIRTUAL_SYSTEM       /*- @VG hacked VS facility. This is only @VG
				 *  debugging tool for DEMOS 3 project.
				 * Using for other needs aren't allowed. */

/*
 * Внутренние флаги системы
 */

/* #define NOKA5                /*-? система меньше чем 0120000 - KA5 не используется при доступе к таблицам */
/* #define UCB_FRCSWAP          /*-- Force swap on expand/fork */
#define UCB_BHASH               /*+  хэш при доступе к буферам */
/* #define UCB_CLIST            /*-  Clists выносится из адресов ядра */
/* #define UCB_DEVERR              /*+ развернутуе сообщения об ошибках устройств */
/* #define UCB_ECC                 /*+ Коррекция исправимых ошибок на дисках */
/* #define BADSECT              /*-? Bad-sector forwarding */
#define UCB_FSFIX               /*+ Улучшенный порядок сброса буферов на диск*/
#define UCB_IHASH               /*+? hashed inode table */
#define UCB_ISRCH               /*+? circular inode search */
#define EXMOUNT                 /*+ таблица mount вынесена из памяти ядра */
/* #define UNFAST               /*++? Использовать функции вместо макро (для экономии места) */
#ifdef KERNEL
#define SMALL                   /*+ малая система (малые хэш-таблицы) */
#endif KERNEL
#define IPK_SDADDR              /*+ 2-byte адрес на диске для малых систем */
#define NOACCI               /*- не записывать время обращения к файлу */
/* #define ACCT                 /*- статистика по командам */
#define INSECURE             /*- не чистить setuid, setgid биты при записи в файл */
/* #define DIAGNOSTIC           /*- дополнительные проверки внутри системы */
/* #define SCHED                /*- новый планировщик реального времени (для памяти порядка >= 1Mб) */
#define IPK_XKOV             /*- включен 31 оверлей в ядре
				* требует включения MENLO_KOV (см. ниже) */

/*
 * Режимы, Зависящие от ЭВМ (как правило)
 *      ТИП ЭВМ type set in whoami.h
 */
#if     PDP11 == GENERIC
#       define  MENLO_KOV
#       define  KERN_NONSEP             /* kernel is not separate I/D */
#else
#   if  PDP11 <= 40 || PDP11 == 60
#       define  MENLO_KOV
#       define  NONSEPARATE
#       define  KERN_NONSEP             /* kernel is not separate I/D */
#   endif
#endif

#if PDP11 == 40
#       define NONFP                /* if no floating point unit */
#endif

#if     PDP11 == 44 || PDP11 == 70 || PDP11 == 24 || PDP11 == GENERIC || defined(ENABLE34)
#       define  UNIBUS_MAP
#endif

/*
 *      Профилирование возможно только  на старших моделях.
 *      Для профилирования :splfix script нужно изменить так, чтобы
 *      вместо spl6 использовать spl7 (см. conf/:splfix.profile).
 */
/* #define PROFILE       /*-? Профилирование системы */

/* Параметры, изменение которых не рекомендуется,
 * так как потребует перетрансляцию ряда программ
 * или приведет к непредсказуемым эффектам
 */

/* #define UCB_PGRP             /*-- Учитывать процессы по группам */
#define UCB_SCRIPT              /*++ При запуске ком.файлов понимается #!shell */
/* #define UCB_SUBM             /*--? "submit", не имеет смысла при UCB_JCL */
#define UCB_SYMLINKS            /*++ "Символьные" связи файлов */
#define UCB_AUTOBOOT            /*++ Системный вызов "reboot" */
/* #define OLDTTY               /*--? старый стиль работы с терминалом */
#define UCB_NTTY                /*++? работа с терминалами по новому   */
#define MENLO_JCL               /*++? Job Control (не выключать!!!) */
#define MENLO_OVLY              /*++ Пользовательские оверлеи */
#define VIRUS_VFORK             /*++ vfork system call */
#define UCB_NKB         1       /*++ "n" KB размер листа на диске (если /*, то 512)*/
/* #define DISPLAY              /*-? 11/70 or 45 display routine */
/* #define MPX_FILS             /*--? Мультиплексные файлы */
/*
 *  UCB_NET - требует подключения ряда файлов, которых здесь нет
 *  Режим экспериментальный, не включать!!!
 */
/* #define UCB_NET              /* TCP/IP Kernel */

#ifdef SCHED
#       undef   CGL_RTP
#endif SCHED

#ifdef UCB_NET
#       define  IPK_SELECT
#endif UCB_NET

#ifdef KERNEL
#   include "pty.h"
#   if NPTY > 0 && !defined(IPK_SELECT)
#       define  IPK_SELECT
#   endif
#endif KERNEL

#if defined(IPK_XKOV) && !defined(IPK_XOVLY)
#       define MENLO_OVLY
#endif

#endif  _sys_localopts_h_
