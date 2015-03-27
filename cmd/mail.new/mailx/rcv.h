#

/*
 * Mail -- a mail program
 *
 * This file is included by normal files which want both
 * globals and declarations.
 *
 * $Log:	rcv.h,v $
 * Revision 1.4  90/09/13  13:21:30  ache
 * MS-DOS & Unix together...
 * 
 * Revision 1.3  90/04/20  19:17:21  avg
 * Прикручено под System V
 * 
 * Revision 1.2  88/07/23  20:29:07  ache
 * Русские диагностики
 * 
 * Revision 1.1  87/12/25  16:00:37  avg
 * Initial revision
 * 
 */

/*
 * $Header: rcv.h,v 1.4 90/09/13 13:21:30 ache Exp $
 */

#ifdef  pdp11
#include <whoami.h>
#endif

#ifdef DEMOS
#include <ediag.h>
#else
#define ediag(e,r) (_ediag?(e):(r))
extern _ediag;
#define EDIAG_R 0
#define EDIAG_E 1
#endif

#include "def.h"
#include "glob.h"
