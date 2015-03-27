#ifndef _sys_file_h_
#       define  _sys_file_h_

#include <sys/types.h>
#include <sys/fcntl.h>

/*
 * One file structure is allocated
 * for each open/creat/pipe call.
 * Main use is to hold the read/write
 * pointer associated with each open
 * file.
 */
struct  file
{
	char    f_flag;
	char    f_count;        /* reference count */
	struct  inode   *f_inode;       /* pointer to inode structure */
	union {
		off_t   f_offset;       /* read/write character pointer */
		struct  chan *f_chan;   /* mpx channel pointer */
#ifdef UCB_NET
		struct  socket *f_Socket;
#endif
	} f_un;
};
#ifdef  UCB_NET
#define f_socket        f_un.f_Socket
#endif

#ifdef  KERNEL
extern  struct file file[];     /* The file table itself */
#endif

/* flags */
#define FREAD   01
#define FWRITE  02
#define FNDELAY 04
#define FAPPEND 010
#define FSYNC   020
#ifdef  UCB_NET
#define FSOCKET 040     /* descriptor of a socket */
#endif
#define FSHLOCK 0100    /* shared lock file */
#define FEXLOCK 0200    /* exclusive lock file */
#define FMASK   (FREAD|FWRITE|FNDELAY|FAPPEND|FSYNC)
			/* bit-mask for file flag */

/* flock call */
#define LOCK_SH 1       /* shared lock */
#define LOCK_EX 2       /* exclusive lock */
#define LOCK_NB 4       /* опрашивание блокирования */
#define LOCK_UN 8       /* unlock */

#endif  _sys_file_h_
