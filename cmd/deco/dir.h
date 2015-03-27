# include <sys/types.h>
# include <sys/stat.h>

# ifdef DIRENT
#    include <dirent.h>
#    ifndef STDDIR
#       define STDDIR
#    endif
# else
#    include <sys/dir.h>
# endif

# define MAXPATHLEN     128     /* max length of directory name */
# define PATSZ          20      /* max length of pattern */

# define SORTNAME       1       /* sort by name */
# define SORTEXT        2       /* sort by extension */
# define SORTTIME       3       /* sort by time */
# define SORTSIZE       4       /* sort by size */
# define SORTSKIP       5       /* unsorted */

struct file {
	char name [15];         /* file name */
	unsigned execable:1;    /* file is executable for me */
	unsigned tag:1;         /* file tagged */
	unsigned link:1;        /* symbolic link */
	unsigned truncname:1;   /* name truncated */
	struct stat fstat;      /* status */
};

struct ddir {
	char cwd [MAXPATHLEN];  /* directory full name */
	char *shortcwd;         /* short printable directory name */
	struct stat dstat;      /* status */
	struct file *cat;       /* catalog */
	int catsz;              /* length of catalog */
	int num;                /* number of files in cat */
	int topfile;            /* top file in the window */
	int curfile;            /* current file */
	char full;              /* full format */
	char status;            /* is it status window */
	char sort;              /* sort kind */
	char typesort;          /* sort by file types */
	char revsort;           /* reverse sorting */
	char alignext;          /* align extensions */
	int basecol;            /* base of window */
	int nfiles;             /* number of regular files */
	long nbytes;            /* number of bytes used */
	int mfiles;             /* number of matched files */
	long mbytes;            /* number of bytes matched */
	int tfiles;             /* number of tagged files */
	long tbytes;            /* number of bytes tagged */
	char pattern [PATSZ];   /* pattern for file names */
};
