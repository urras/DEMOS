/*
 * Whoami - локальная конфигурация системы
 */
#ifndef _sys_whoami_h_
# define _sys_whoami_h_          /* so param.h won't include us again */

#define MYNAME  "%ident%"       /* for uucp */
#define PDP11   %PDP%
#ifdef  KERNEL
#include "h/localopts.h"
#else
#    include <sys/localopts.h>
#endif

#endif _sys_whoami_h_
