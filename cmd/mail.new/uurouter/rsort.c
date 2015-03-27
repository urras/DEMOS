#include <stdio.h>
#include "def.h"

static int          BackRoute;  /* Number of Backward Route */
static struct RFENT PrevEnt;    /* Previous entry in the route file */
static int          LineNo;     /* Current Line No */
static int          ErrCnt;     /* Error Counter per file */

int    xatoi();

struct ROUTE  Root = { &Root, (struct ROUTE *)NULL, 0, R_EMPTY, "" };     /* Root Domain */

struct SYSTEM *syslist = NULL;  /* List of systems */

char errors_to[200] = "";
char mypassword[80] = "";

/*
 * Print out the local routing table
 */
outtab(f)
	FILE *f;
{
	struct ROUTE *srp[MAXWORDS+1], *scp[MAXWORDS+1];
	register struct ROUTE *rp, *cp;
	int lev, fl, i, hlev;

	if( Root.r_down == NULL ) {
		errmsg("No routes collected???\n");
		return;
	}

	lev = 0;
	rp = Root.r_down;
	cp = rp;
	fl = 0;
	hlev = 0;
again:
	for( ; cp != rp || !fl++ ; cp = cp->r_next ) {
		if( cp->r_nor != R_EMPTY ) {
			fprintf(f, "%d%s", hlev, cp->r_name);
			for( i = 0 ; i < lev-hlev ; i++ )
				fprintf(f, ".%s", scp[lev-i-1]->r_name);
			fprintf(f, cp->r_nor==R_LOCAL? "\t-"
						     : "\t%d", cp->r_nor);
			fprintf(f, "\t%d\n", cp->r_metric);
			hlev = lev;
		}
		if( cp->r_down != NULL ) {
			srp[lev] = rp; scp[lev++] = cp;         /* PUSH */
			fl = 0;
			cp = rp = cp->r_down;
			goto again;
		}
	}
	if( lev > 0 ) {
		rp = srp[--lev]; cp = scp[lev];                 /* POP */
		cp = cp->r_next;
		hlev = lev;
		fl = 1;
		goto again;
	}
}

/*
 * Read a routes file and join with the tree
 *
 *      filename  - route file name
 *      sysfile   - true if it was the system file
 *      rn        - number of route of the given file
 *      addmetric - metric of interface
 */
joinroutes(f, sysfile, rn, addmetric, diagname)
	FILE *f;
	short rn;
	short addmetric;
	char *diagname;
{
	struct RFENT *re, *getrent();
	int i;
	register struct ROUTE *rp, *cptr;

	BackRoute = -17;        /* Strange value - means no back route found */
	PrevEnt.f_nwords = 0;   /* Prev domain is a Universe */
	LineNo = 0;
	ErrCnt = 0;

	/*
	 * Read entries
	 */
	while( (re = getrent(f, diagname, sysfile)) != NULL ) {
		/*
		 * Reject looping routes
		 */
		if( re->f_nor == BackRoute )
			continue;

		/*
		 * Walk along the tree searching the path
		 * and adding new names
		 */
		if( debug ) {
			printf("BR=%d NOR=%d M=%d NW=%d NAME=", BackRoute,
								re->f_nor,
								re->f_metric,
								re->f_nwords);
			for( i = re->f_nwords ; i ; ) {
				printf("%s", re->f_words[--i]);
				putchar(i?'.':'\n');
			}
		}
		cptr = rp = &Root;
		i = 0;  /* level */
search:         while( i < re->f_nwords ) {
			/*
			 * If there is no circular list,
			 * initialiazie it.
			 */
			if( cptr->r_down == NULL ) {
				rp = (struct ROUTE *)malloc(sizeof(struct ROUTE) +
							    strlen(re->f_words[i]));
				cptr->r_down = rp;
				rp->r_next = rp;
				goto addname;
			}

			/*
			 * Scan through list to find the part of name
			 */
			rp = cptr->r_down;
			do {
				if( !strcmp(rp->r_name, re->f_words[i]) ) {
					/* Name found - let's proceed */
					i++;
					cptr = rp;
					goto search;
				}
				rp = rp->r_next;
			} while( rp != cptr->r_down );

			/*
			 * Name not found
			 */
			rp = (struct ROUTE *)malloc(sizeof(struct ROUTE) +
						    strlen(re->f_words[i]));
			rp->r_next = cptr->r_down->r_next;
			cptr->r_down->r_next = rp;
		addname:
			rp->r_down = NULL;
			rp->r_metric = HUGE;
			rp->r_nor    = R_EMPTY;
			strcpy(rp->r_name, re->f_words[i++]);
			cptr = rp;
		}

		/*
		 * Routing tree node found or allocated -
		 * compare routes and get cheaper one.
		 */
		if( re->f_nor == R_LOCAL ) {
			rp->r_metric = re->f_metric;
			rp->r_nor = R_LOCAL;
		} else if( rp->r_metric > re->f_metric + addmetric ) {
			rp->r_metric = re->f_metric + addmetric;
			rp->r_nor = rn;
		}
	}
}

/*
 * Read an entry of route/systems file
 */
struct RFENT *getrent(f, dn, sys)
	FILE *f;
	char *dn;
	int   sys;      /* true for Systems file */
{
	char line[300];
	struct RFENT t;
	struct SYSTEM *sy;
	register char *p, *q;
	int i, j;
	int tailwords;  /* N of words from previous entry */
	static int firsttime = 1;

	/* Add an ID of local machine as default */
	if( sys && firsttime ) {
		firsttime = 0;
		gethost(line, 200);
		strcat(line, "\t0\t@");  /* Local delivery, metric 0 */
		goto skipfgets;
	}

nextline:
	LineNo++;
	line[299] = '\0';
	if( fgets(line, 300, f) == NULL )
		return NULL;    /* No more entries */
	if( line[299] != '\0' ) {
		p = "line too long";
	synterr:
		if( ErrCnt++ > 5 )
			return NULL;
		errmsg("ERR in %s, line %d: %s\n", dn, LineNo, p);
		goto nextline;
	}
	if( (p = index(line, '\n')) != NULL )
		*p = '\0';      /* Cut LF */
	if( (p = index(line, '#')) != NULL )
		*p = '\0';      /* Strip out comment */
skipfgets:
	p = line;
	while( *p == ' ' || *p == '\t' ) p++;   /* Skip leading spaces */

	if( *p == '\0' )
		goto nextline;  /* skip blank line */

	/*
	 * Parse % commands in System file
	 */
	if( *p == '%' ) {
		p++;
		switch( *p++ ) {
		    case 'b':
		    case 'B':
			if( sys ) {
				p = "%b in Systems?";
				goto synterr;
			}
			while( *p == ' ' || *p == '\t' ) p++;
			i = xatoi(p);
			if( i == -1 ) {
				p = "illegal integer";
				goto synterr;
			}
			BackRoute = i;
			break;

		    case 'p':
		    case 'P':
			while( *p == ' ' || *p == '\t' ) p++;
			if( sys )
				strcpy(mypassword, p);
			break;

		    case 'e':
		    case 'E':
			while( *p == ' ' || *p == '\t' ) p++;
			if( sys )
				strcpy(errors_to, p);
			break;

		    default:
			p = "illegal % command";
			goto synterr;
		}
		goto nextline;
	}

	/*
	 * Read inheritance counter
	 */
	if( '0' > *p || *p > '9' ) {
		if( sys ) {
			tailwords = 0;
			goto fhost;
		}
		p = "no inheritance counter";
		goto synterr;
	}
	if( sys ) {
		sy = (struct SYSTEM *)malloc(sizeof(struct SYSTEM));
		/* Read number of route */
		if( (sy->s_nor = xatoi(p)) == -1 ) {
			p = "illegal route #";
			goto synterr;
		}

		/* Read metric value */
		while( '0' <= *p && *p <= '9' ) p++;
		while( *p == ' ' || *p == '\t' ) p++;
		if( (sy->s_metric = xatoi(p)) == -1 ) {
			p = "illegal metric value";
			goto synterr;
		}

		/* Read path to remote machine */
		while( '0' <= *p && *p <= '9' ) p++;
		while( *p == ' ' || *p == '\t' ) p++;
		if( *p == '\0' ) {
			p = "missed path to remote machine";
			goto synterr;
		}
		q = t.f_WRKS;
		while( *p != '\0' && *p != ' ' && *p != '\t' )
			*q++ = *p++;
		*q = '\0';
		if( (q = index(t.f_WRKS, '/')) != NULL )
			*q = '\0';
		sy->s_path = (char *)malloc(strlen(t.f_WRKS) + 1);
		strcpy(sy->s_path, t.f_WRKS);
		if( index(sy->s_path, '@') == NULL &&
		    index(sy->s_path, '!') == NULL &&
		    index(sy->s_path, '%') == NULL ) {
			sprintf(t.f_WRKS, "illegal network path (%s)", sy->s_path);
			p = t.f_WRKS;
			goto synterr;
		}

		/* Read password for Remote Control messages */
		while( *p == ' ' || *p == '\t' ) p++;
		if( *p == '\0' ) {
			sy->s_passwd = NULL;
		} else {
			q = t.f_WRKS;
			while( *p != '\0' && *p != ' ' && *p != '\t' )
				*q++ = *p++;
			*q++ = '\0';
			sy->s_passwd = (char *)malloc(strlen(t.f_WRKS) + 1);
			strcpy(sy->s_passwd, t.f_WRKS);
		}

		/* Add the system in the list */
		sy->s_next = syslist;
		syslist = sy;
		goto nextline;
	}
	tailwords = *p++ - '0';
	if( tailwords > PrevEnt.f_nwords ) {
		p = "too many words inherited";
		goto synterr;
	}

	/*
	 * Fetch host/domain name
	 */
fhost:
	if( *p == 0 || *p == ' ' || *p == '\t' ) {
		p = "missed host/domain name";
		goto synterr;
	}
	q = t.f_WRKS;
	while( *p != '\0' && *p != ' ' && *p != '\t' )
		*q++ = *p++;
	*q++ = '\0';

	/*
	 * Fetch number of route
	 */
	if( !sys ) {
		while( *p == ' ' || *p == '\t' ) p++;
		if( *p == '-' && (p[1] == ' ' || p[1] == '\t') ) {
			p++;
			t.f_nor = R_EXPLICIT;
		} else {
			i = xatoi(p);
			if( i == -1 ) {
				p = "bad route #";
				goto synterr;
			}
			t.f_nor = i;
			while( '0' <= *p && *p <= '9' ) p++;
		}
	} else
		t.f_nor = R_LOCAL;

	/*
	 * Finally get the metric value
	 */
	while( *p == ' ' || *p == '\t' ) p++;
	if( (t.f_metric = xatoi(p)) == -1 ) {
		p = "bad metric value";
		goto synterr;
	}

	/*
	 * Copy tail words to the beginning of f_words
	 */
	/* now q points to the end of used space in t.f_WKS */
	for( i = 0 ; i < tailwords ; ) {
		p = PrevEnt.f_words[i];
		t.f_words[i++] = q;
		while( *q++ = *p++ );
	}

	/*
	 * Split host/domain name into words
	 */
	p = t.f_WRKS;
	while( *p ) p++;        /* move p to the end of string */
	for(;;) {
		while( p > t.f_WRKS && *--p != '.' ) ;
		if( i >= MAXWORDS ) {
			p = "too many words";
			goto synterr;
		}
		if( p == t.f_WRKS ) {   /* that's all? */
			t.f_words[i++] = p;     /* host name */
			break;
		} else
			t.f_words[i++] = p+1;   /* skip the dot */
		*p = '\0';
	}
	t.f_nwords = i;

	/* Check words */
	for( i = 0 ; i < t.f_nwords ; i++ ) {
		if( t.f_words[i][0] == '\0' ) {
			p = "null word?";
			goto synterr;
		}
		if( t.f_words[i][0] == '*' && (t.f_words[i][1] != '\0' ||
					       i != t.f_nwords-1) ) {
			p = "illegal wildcard";
			goto synterr;
		}
	}

	/* Save the structure */
	PrevEnt = t;

	/* adjust pointers */
	for( i = 0 ; i < t.f_nwords ; i++ )
		PrevEnt.f_words[i] -= ((char *)&t - (char *)&PrevEnt);
	return &PrevEnt;
}

/*
 * xatoi - like an ordinary atoi but returns -1 on error
 */
int xatoi(s)
	register char *s;
{
	register int l;

	l = 0;
	while( '0' <= *s && *s <= '9' )
		l = l*10 + (*s++ - '0');
	if( *s != '\0' && *s != ' ' && *s != '\t' )
		return -1;
	return l;
}
