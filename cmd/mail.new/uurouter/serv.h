/*
 *  Definitions of working files' location in Unix file system.
 *
 *  Copyright 1990, DEMOS cooperative, Moscow, USSR
 *  All rights reserved.
 *
 *  The copyright above and this notice must be preserved in all
 *  copies of this source code. The copyright above does not
 *  evidence any actual or intended publication of this source
 *  code.
 *
 *  This is unpublished proprietary trade secret source code of
 *  DEMOS cooperative. This source code may not be copied,
 *  disclosed, distributed, demonstrated or licensed except as
 *  expressly authorized by DEMOS.
 *
 * $Header: serv.h,v 1.2 90/08/17 18:42:00 avg Exp $
 * $Log:	serv.h,v $
 * Revision 1.2  90/08/17  18:42:00  avg
 * /usr/lib/uucp.routes --> /usr/lib/relcom/uucp.routes
 * 
 * Revision 1.1  90/08/06  20:24:28  avg
 * Initial revision
 * 
 *
 */

/* #define MANUAL_ROUTING       /* Enable manual control of routing */
#define AUTOMATIC_ROUTING       /* Enable automated control of routing */

#ifdef DEMOS
#define ROUTES     "/usr/local/lib/relcom/uucp.routes"
#define ROUTES_DIR "/usr/local/spool/uuroutes"
#define SYSTEMFILE "/usr/local/lib/relcom/Systems"

#define SERV_LIST  "/usr/local/lib/relcom/Services"
#define SERV_DIR   "/usr/local/lib/relcom"
#else
#define ROUTES     "/usr/lib/relcom/uucp.routes" /* File containing MAN-mode table */
#define ROUTES_DIR "/usr/spool/uuroutes"        /* Directory for AUTO files */
#define SYSTEMFILE "/usr/lib/relcom/Systems"    /* Systems file for AUTO mode */

#define SERV_LIST  "/usr/lib/relcom/Services"   /* List of services */
#define SERV_DIR   "/usr/lib/relcom"            /* Location of service utils */
#endif
