#ifndef _sys_reboot_h_
#       define  _sys_reboot_h_

/*
 * Arguments to reboot system call.
 * These are passed to boot program in r4
 * and on to init.
 */
#define RB_ASKNAME      0000001
#define RB_SINGLE       0000002
#define RB_NOSYNC       0000004
#define RB_HALT         0000010
#define RB_DUMP         0000020
#define RB_NOFSCK       0000040
#define RB_POWRFAIL     0000100
#define RB_FROMTAPE     0000200

/*
 * The name of system to boot on automatic reboots.
 */
#define RB_DEFNAME      "%n(%d,0)conf-%n" /* Это д. быть ком файл */
#define RB_DEFTAPE      "%n(%d,1)"        /* default is standalone Cat */

#endif  _sys_reboot_h_
