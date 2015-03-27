/*
 * Whoami - ��������� ������������ �������
 *
 * $Log:	whoami.h,v $
 * Revision 1.3  88/09/01  14:47:53  korotaev
 * ������� � localopts.h �������� NONFP.
 * 
 * Revision 1.2  88/04/02  17:31:33  korotaev
 * ������� ����� ����������� � �������� ��������� ���������.
 * 
 * Revision 1.1  86/04/19  15:03:03  avg
 * Initial revision
 *
 */

#ifndef _sys_whoami_h_
#define _sys_whoami_h_          /* so param.h won't include us again */

#define %IDENT%
#define MYNAME  "%ident%"       /* for uucp */
#define PDP11   %PDP%

#ifdef  KERNEL
#    include "localopts.h"
#else
#    include <sys/localopts.h>
#endif
#endif _sys_whoami_h_
