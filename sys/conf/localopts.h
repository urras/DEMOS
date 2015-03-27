#ifndef _sys_localopts_h_
#       define  _sys_localopts_h_
/*
 * Локальные параметры системы ( подключаются через whoami.h).
 * Приняты следующие обозначения в комментариях:
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
 * Нижеследующие параметры менять не рекомендуется
 */
/* #define UCB_LOGIN            /*- есть системный вызов "login" */
/* #define UCB_QUOTAS           /*- включено квотирование дисков */
/* #define TEXAS_AUTOBAUD       /*- автоматический подбор скорости tty */
/* #define UCB_VHANGUP          /*- При выходе user-a посылается HANGUP */
#define UCB_RENICE              /*++ renice system call */
#define IPK_XOVLY               /*++ включен 31 пользовательский оверлей
				 * требует включения MENLO_OVLY (см. ниже) */
/* #define VIRTUAL_SYSTEM       /*- @VG hacked VS facility. This is only @VG
				 *  debugging tool for DEMOS 3 project.
				 * Using for other needs aren't allowed. */
/* #define NOKA5                /*-? система меньше чем 0120000 - KA5 не используется при доступе к таблицам */
/* #define UCB_FRCSWAP          /*-- Force swap on expand/fork */
/* #define UCB_CLIST            /*-  Clists выносится из адресов ядра */
/* #define BADSECT              /*-? Bad-sector forwarding */
/* #define DIAGNOSTIC           /*- дополнительные проверки внутри системы */
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
 * Режимы, Зависящие от ЭВМ (как правило)
 *      ТИП ЭВМ type set in whoami.h
 */
#define  MENLO_KOV
#if     PDP11 == GENERIC
#       define  KERN_NONSEP             /* kernel is not separate I/D */
#else
#   if  PDP11 <= 40 || PDP11 == 60
#       define  NONSEPARATE
#       define  KERN_NONSEP             /* kernel is not separate I/D */
#   endif
#endif

#if PDP11 == 40
#       define NONFP                /* if no floating point unit */
#endif

#if ((((PDP11)==(44))||((PDP11)==(70)))||((PDP11)==(24)))||((PDP11)==(GENERIC))
#       define  UNIBUS_MAP
#endif

/*
 *      Профилирование возможно только  на старших моделях.
 *      Для профилирования :splfix script нужно изменить так, чтобы
 *      вместо spl6 использовать spl7 (см. conf/:splfix.profile).
 */
/* #define PROFILE       /*-? Профилирование системы */

/*
 *  UCB_NET - требует подключения ряда файлов, которых здесь нет
 *  Режим экспериментальный, не включать!!!
 */
/* #define UCB_NET              /* TCP/IP Kernel */

/*
 *        Следующие параметры можно изменять:
 */
