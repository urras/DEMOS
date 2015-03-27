#ifndef _sys_ustat_h_
# define _sys_ustat_h_

# ifndef _sys_types_h_
#  include <sys/types.h>
# endif

struct ustat{
	daddr_t f_tfree;        /* free blocks (1K) */
	ino_t   f_tinode;       /* free inodes */
	char    f_fname[12];    /* file system label */
				/* mount point name in BSD */
	char f_fpack[6];        /* disk pack label */
};

#endif _sys_ustat_h_
