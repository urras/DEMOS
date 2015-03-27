#

/*
 * This file contains definitions of network data used by Mail
 * when replying.  See also:  confdefs.h and optim.c
 *
 * $Log: config.c,v $
 * Revision 1.4  1990/09/13  13:18:55  ache
 * MS-DOS & Unix together...
 *
 * Revision 1.3  90/05/31  19:47:17  avg
 * pRAWLEN NABOR WYDAWAEMYH POLEJ ZAGOLOWKOW.
 * 
 * Revision 1.2  88/01/11  12:36:47  avg
 * iZMENENA TABLICA OB]EIZWESTNYH SETEWYH IMEN, DOBAWLEN NOXSTR U rcsid.
 * 
 * Revision 1.1  87/12/25  15:58:57  avg
 * Initial revision
 *
 */

/*NOXSTR
static char rcsid[] = "$Header: /usr/src/Relcom/relcom/mailx/RCS/config.c,v 1.4 1990/09/13 13:18:55 ache Exp $";
YESXSTR*/

/*
 * The subterfuge with CONFIGFILE is to keep cc from seeing the
 * external defintions in configdefs.h.
 */
#define CONFIGFILE
#include "confdefs.h"

/*
 * Set of network separator characters.
 */
char    *metanet = "!^:%@";

/*
 * Host table of "known" hosts.  See the comment in configdefs.h;
 * not all accessible hosts need be here (fortunately).
 */
struct netmach netmach[] = {
	EMPTY,          EMPTYID,        BN,     /* Filled in dynamically */
	0,              0,              0
};

/*
 * Table of ordered of preferred networks.  You probably won't need
 * to fuss with this unless you add a new network character (foolishly).
 */
struct netorder netorder[] = {
	AN,     '@',
	AN,     '%',
	SN,     ':',
	BN,     '!',
	-1,     0
};

/*
 * Table to convert from network separator code in address to network
 * bit map kind.  With this transformation, we can deal with more than
 * one character having the same meaning easily.
 */
struct ntypetab ntypetab[] = {
	'%',    AN,
	'@',    AN,
	':',    SN,
	'!',    BN,
	'^',    BN,
	0,      0
};

struct nkindtab nkindtab[] = {
	AN,     IMPLICIT,
	BN,     EXPLICIT,
	SN,     IMPLICIT,
	0,      0
};
