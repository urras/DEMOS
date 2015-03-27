/*
 * Whoami - локальная конфигурация системы
 *
 * $Log:	whoami.h,v $
 * Revision 1.3  88/09/01  14:47:53  korotaev
 * Вынесен в localopts.h параметр NONFP.
 * 
 * Revision 1.2  88/04/02  17:31:33  korotaev
 * Включен новый планировщик и изменены некоторые умолчания.
 * 
 * Revision 1.1  86/04/19  15:03:03  avg
 * Initial revision
 *
 */

#ifndef _sys_whoami_h_
#define _sys_whoami_h_          /* so param.h won't include us again */

#define ISSU_85
#define MYNAME  "LISSU_85"       /* for uucp */
#define PDP11   23

#ifdef  KERNEL
#    include "localopts.h"
#else
#    include <sys/localopts.h>
#endif
#endif _sys_whoami_h_
