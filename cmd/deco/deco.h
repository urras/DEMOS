/* width of directory window */

# define PAGEWID        (fullwin ? 77 : 39)

/* number of displayable files in directory window */

# define PAGELEN(c)     pagelen ((c)->full)

# define BASECOL(c)     (fullwin ? 0 : (c)->basecol)

# define CMDLEN 512                     /* length of command string */

extern struct dir left, right;          /* left and right directories */
extern struct dir *cur;                 /* current directory */
extern fullwin;                         /* full window regime */
extern H;                               /* file window height */
extern cmdreg;                          /* command line regime */
extern char command [];                 /* command line */
extern cpos;                            /* command line cursor */
extern char *user;                      /* user name */
extern char *group;                     /* user group name */
extern char *tty;                       /* tty name */
extern char *machine;                   /* machine name */
extern visualwin;                       /* cursor in command line */
extern gid;                             /* real group id */
extern uid;                             /* real user id */
extern ppid;                            /* real parent id */
