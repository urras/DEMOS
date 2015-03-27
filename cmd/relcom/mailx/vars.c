#

#include "rcv.h"
#ifdef  MSDOS
extern char *upperstr();
#endif

/*
 * Mail -- a mail program
 *
 * Variable handling stuff.
 *
 * $Log: vars.c,v $
 * Revision 1.6  1990/09/29  18:22:37  ache
 * <ctype.h> kicked out...
 *
 * Revision 1.5  90/09/21  22:00:50  ache
 * MS-DOS extends + some new stuff
 * 
 * Revision 1.4  90/09/13  13:21:06  ache
 * MS-DOS & Unix together...
 * 
 * Revision 1.3  88/07/23  20:39:18  ache
 * rUSSKIE DIAGNOSTIKI
 * 
 * Revision 1.2  88/01/11  12:46:17  avg
 * dOBAWLEN NOXSTR U rcsid.
 * 
 * Revision 1.1  87/12/25  16:01:10  avg
 * Initial revision
 * 
 */

/*
 * Assign a value to a variable.
 */

assign(name, value)
	char name[], value[];
{
	register struct var *vp;
	register int h;

	h = hash(name);
	vp = lookup(name);
	if (vp == NOVAR) {
		vp = (struct var *) calloc(sizeof *vp, 1);
		vp->v_name = vcopy(name);
		vp->v_link = variables[h];
		variables[h] = vp;
	}
	else
		vfree(vp->v_value);
	vp->v_value = vcopy(value);
}

/*
 * Free up a variable string.  We do not bother to allocate
 * strings whose value is "" since they are expected to be frequent.
 * Thus, we cannot free same!
 */

vfree(cp)
	register char *cp;
{
	if (!equal(cp, ""))
		cfree(cp);
}

/*
 * Copy a variable value into permanent (ie, not collected after each
 * command) space.  Do not bother to alloc space for ""
 */

char *
vcopy(str)
	char str[];
{
	register char *top, *cp, *cp2;

	if (equal(str, ""))
		return("");
	if ((top = calloc(strlen(str)+1, 1)) == NULL)
		panic (ediag("Out of memory","mALO PAMQTI"));
	cp = top;
	cp2 = str;
	while (*cp++ = *cp2++)
		;
	return(top);
}

/*
 * Get the value of a variable and return it.
 * Look in the environment if its not available locally.
 */

char *
value(name)
	char *name;
{
	register struct var *vp;

	if ((vp = lookup(name)) == NOVAR) {
#ifdef MSDOS
		char *s = upperstr(name);

		if (s != NOSTR)
			name = s;
#endif
		return(getenv(name));
	}

	return(vp->v_value);
}

/*
 * Locate a variable and return its variable
 * node.
 */

struct var *
lookup(name)
	char name[];
{
	register struct var *vp;
	register int h;

	h = hash(name);
	for (vp = variables[h]; vp != NOVAR; vp = vp->v_link)
#ifdef  MSDOS
		if (icequal(vp->v_name, name))
#else
		if (equal(vp->v_name, name))
#endif
			return(vp);
	return(NOVAR);
}

/*
 * Locate a group name and return it.
 */

struct grouphead *
findgroup(name)
	char name[];
{
	register struct grouphead *gh;
	register int h;

	h = hash(name);
	for (gh = groups[h]; gh != NOGRP; gh = gh->g_link)
		if (equal(gh->g_name, name))
			return(gh);
	return(NOGRP);
}

/*
 * Print a group out on stdout
 */
void
printgroup(name)
	char name[];
{
	register struct grouphead *gh;
	register struct group *gp;

	if ((gh = findgroup(name)) == NOGRP) {
		printf(ediag(
"\"%s\": not a group\n",
"\"%s\": NE GRUPPA\n"),
name);
		return;
	}
	printf("%s\t", gh->g_name);
	for (gp = gh->g_list; gp != NOGE; gp = gp->ge_link)
		printf(" %s", gp->ge_name);
	printf("\n");
}

/*
 * Hash the passed string and return an index into
 * the variable or group hash table.
 */

#ifdef  MSDOS
#define THESAME(c) toupper(c)
#else
#define THESAME(c) (c)
#endif

hash(name)
	char *name;
{
	register unsigned h;
	register char *cp;

	for (cp = name, h = 0; *cp; h = (h << 2) + THESAME(*cp++))
		;
	if (h < 0)
		h = -h;
	if (h < 0)
		h = 0;
	return(h % HSHSIZE);
}
