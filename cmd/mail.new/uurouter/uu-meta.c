/*
 * UU-META    - Meta service caller - use it with SENDMAIL
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
 *
 * Usage:       uu-meta servname reply_to
 *
 * $Log:	uu-meta.c,v $
 * Revision 1.2  90/08/02  17:47:17  avg
 * *** empty log message ***
 * 
 * Revision 1.1  90/08/01  21:47:13  avg
 * Initial revision
 * 
 */

#include "serv.h"

#include <stdio.h>
#include <sysexits.h>

#ifndef lint
static char rcsid[] = "$Header: uu-meta.c,v 1.2 90/08/02 17:47:17 avg Exp $";
#endif

#ifdef M_SYSV
#define index strchr
#endif
char    *index();

main(c, v)
	char **v;
{
	static char buf[100] = SERV_DIR;
	char sn[100], *p;
	FILE *sf;

	if( c != 3 ) {
		fprintf(stderr, "Usage: uu-meta service reply_to\n");
		exit(EX_USAGE);
	}
	if( (sf = fopen(SERV_LIST, "r")) == NULL )
		exit(EX_UNAVAILABLE);
	while( fgets(sn, sizeof sn, sf) != NULL ) {
		sn[99] = '\0';
		if( (p = index(sn, '\n')) != NULL )
			*p = '\0';
printf("read sn='%s'\n", sn);
		p = sn;
		while( *p == ' ' || *p == '\t' ) p++;
		if( *p == '#' || *p == '\0' )
			continue;
		if( !strcmp(p, v[1]) )
			goto found;
	}
	exit(EX_UNAVAILABLE);
found:  fclose(sf);
	buf[sizeof SERV_DIR - 1 ] = '/';
	strcpy(&buf[sizeof SERV_DIR], v[1]);
	execl(buf, v[1], "-r", v[2], 0);
	exit(EX_UNAVAILABLE);
}
