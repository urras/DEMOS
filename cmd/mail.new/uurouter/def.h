/*
 * Structure of routes database node
 */
struct ROUTE {
	struct ROUTE *r_next;     /* Next entry (with the same depth) */
	struct ROUTE *r_down;     /* Down link */
	short         r_metric;   /* Metric of the route */
	short         r_nor;      /* Number of the route */
	char          r_name[1];  /* Word of a name */
};

/* Special value for Number of the Route field */
#define R_EMPTY         (-1)            /* It's a pure node */
#define R_EXPLICIT      (-2)            /* Remote local route */
#define R_LOCAL         (-3)            /* Local route (see Systems file) */

#define HUGE    32767   /* huge value for metric */

#define MAXWORDS 10     /* Max words per domain name */

/*
 * Route file entry structure
 */
struct RFENT {
	long    f_metric;                       /* Current Metric */
	short   f_nor;                   /* Number of route */
	short   f_nwords;                       /* Number of words */
	char   *f_words[MAXWORDS];              /* Word Length */
	char    f_WRKS[256];   /* Workspace */
};

/*
 * System file entry
 */
struct SYSTEM {
	struct SYSTEM  *s_next;         /* Next entry */
	short           s_nor;          /* Number of route */
	short           s_metric;       /* Metric value */
	char           *s_path;         /* Pathname of the remote system */
	char           *s_passwd;       /* Password of the remote system */
};

/*
 * Common defines
 */
#ifdef M_SYSV
#define index  strchr
#define rindex strrchr
#endif

char    *index(), *rindex();

/*
 * Global definitions
 */
extern struct SYSTEM *syslist;  /* List of systems */
extern int            debug;    /* Debugging options */
extern char           errors_to[];  /* List of network administrators */
extern char           mypassword[]; /* Local password for uurouter control */

#ifdef NOSM
#ifdef DEMOS
#   define MAILER  "/usr/local/lib/uumail"
#else
#   define MAILER  "/usr/lib/uumail"
#endif
#else
#ifdef DEMOS
#   define MAILER  "/usr/local/lib/sendmail"
#else
#   define MAILER  "/usr/lib/sendmail"
#endif
#endif

#define UUNAME  "uuname -l"

#define UU_USER "UUrouter"

#define RFNAME  "R.%04d"
#define TRFNAME "T.%04d"
