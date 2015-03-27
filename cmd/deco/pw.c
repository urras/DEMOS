# include <pwd.h>
# include <grp.h>
# include "mem.h"

# define NAMLEN 9
# define QUANT  20

struct table {
	int key;
	char name [NAMLEN];
};

static struct table *uname;
static struct table *grname;
static nuname, ngrname;

static char root[] = "/";
char *home = root;
extern uid;

extern struct passwd *getpwent ();
extern struct group *getgrent ();
extern char *strcpy (), *strncpy ();

#ifndef pdp11
static
#endif
compare (a, b)
struct table *a, *b;
{
	register ak = a->key, bk = b->key;

	return (ak<bk ? -1 : ak>bk ? 1 : 0);
}

static initpw ()
{
	register t, tt;
	register struct passwd *pw;
	register struct group *gr;

	setpwent ();
	for (t=0; pw=getpwent(); ) {
		MemCheckIndex (uname, struct table *, nuname, QUANT, t);
		if (pw->pw_uid == uid) {
			if (home != root)
				MemFree ((mem *) home);
			home = (char *) MemAlloc (strlen (pw->pw_dir) + 1);
			strcpy (home, pw->pw_dir);
		}
		for (tt=0; tt<t; ++tt)
			if (uname[tt].key == pw->pw_uid)
				break;
		if (tt < t)
			continue;	/* ignore entry with same uid */
		uname[t].key = pw->pw_uid;
		strncpy (uname[t].name, pw->pw_name, NAMLEN-1);
		uname[t].name[NAMLEN-1] = 0;
		++t;
	}
	endpwent ();
	nuname = t;
	MemTrunc ((mem *) uname, nuname * (int) sizeof (*uname));
	qsort ((char *) uname, (unsigned) nuname, sizeof (struct table), compare);

	setgrent ();
	for (t=0; gr=getgrent(); ) {
		MemCheckIndex (grname, struct table *, ngrname, QUANT, t);
		for (tt=0; tt<t; ++tt)
			if (grname[tt].key == gr->gr_gid)
				break;
		if (tt < t)
			continue;       /* ignore entry with same gid */
		grname[t].key = gr->gr_gid;
		strncpy (grname[t].name, gr->gr_name, NAMLEN-1);
		grname[t].name[NAMLEN-1] = 0;
		++t;
	}
	endgrent ();
	ngrname = t;
	MemTrunc ((mem *) grname, ngrname * (int) sizeof (*grname));
	qsort ((char *) grname, (unsigned) ngrname, sizeof (struct table), compare);
}

static struct table *search (key, base, nel)
int key;                        /* Key to be located */
struct table *base;             /* Beginning of table */
int nel;                        /* Number of elements in the table */
{
	struct table *last;     /* Last element in table */
	register struct table *p;

	last = base + (nel - 1);
	while (last >= base) {
		p = base + ((last - base)/2);
		if (p->key == key)
			return (p);	/* Key found */
		if (p->key > key)
			last = p - 1;
		else
			base = p + 1;
	}
	return ((struct table *) 0);    /* Key not found */
}

char *username (u)
register u;
{
	register struct table *t;
	static char buf [NAMLEN];

	if (! nuname)
		initpw ();
	if (t = search (u, uname, nuname))
		return (t->name);
	sprintf (buf, "%d", u);
	return (buf);
}

char *groupname (g)
register g;
{
	register struct table *t;
	static char buf [NAMLEN];

	if (! ngrname)
		initpw ();
	if (t = search (g, grname, ngrname))
		return (t->name);
	sprintf (buf, "%d", g);
	return (buf);
}
