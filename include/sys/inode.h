#ifndef _sys_inode_h_
#       define  _sys_inode_h_

/*
 * The inode is the focus of
 * file activity in unix. There is a unique
 * inode allocated for each active file,
 * each current directory, each mounted-on
 * file, text file, and the root. An inode is 'named'
 * by its dev/inumber pair. (iget/iget.c)
 * Data, from mode on, is read in
 * from permanent inode on volume.
 *
 * Added IPK_SDADDR feature - @VG: 05/30/86
 * Added AVG_CALLS (call the executable with pipes (like popen))
 *       feature & IFCALL file type - @VG 11/03/87
 */

#ifdef KERNEL
#  ifdef IPK_SDADDR
	typedef unsigned IDADDR;
#  else
	typedef daddr_t IDADDR;
#  endif
#else  -KERNEL
	typedef daddr_t IDADDR;
#endif KERNEL

#ifdef  UCB_NKB
#define NADDR   7
#else   UCB_NKB
#define NADDR   13
#endif

#define PIPSIZ ((NADDR-3)*BSIZE)

struct  inode
{
	int     i_flag;
	char    i_count;        /* reference count */
	char    i_lockc;        /* for flock syscall */
	dev_t   i_dev;          /* device where inode resides */
	ino_t   i_number;       /* i number, 1-to-1 with device address */
	u_short i_mode;
	short   i_nlink;        /* directory entries */
	short   i_uid;          /* owner */
	short   i_gid;          /* group of owner */
	off_t   i_size;         /* size of file */
	union {
		struct {        /* if normal file/directory */
			IDADDR  I_addr[NADDR];
			IDADDR  I_lastr;        /* last logical block read (for read-ahead) */
		} i_f;
		struct {        /* used if fifo */
			IDADDR  I_addR[NADDR-3];
			u_short I_frptr;
			u_short I_fwptr;
			char    I_frcnt;
			char    I_fwcnt;
		} i_fa;
		struct  {       /* if blk/chr-dev-file */
#ifndef IPK_SDADDR
			u_short I_unused;       /* при преобразовании
						   3-х байтного в long
						   в iget сюда запишется
						   старшее слово от
						   I_addr[0]  */
			/* ЭТО МАШИННО-ЗАВИСИМО */
#endif  IPK_SDADDR
			dev_t  I_rdev;         /* i_addr[0] */
		} i_d;
#ifdef  UCB_QUOTAS
		struct  {
			daddr_t I_qused;
			daddr_t I_qmax;
		} i_q;
#endif
	} i_un;
#ifdef  UCB_QUOTAS
	struct inode    *i_quot;/* pointer to quota inode */
#endif
#ifdef  UCB_IHASH
	struct  inode *i_link;  /* link in hash chain (iget/iput/ifind) */
#endif
	struct proc *i_rsel;
	struct proc *i_wsel;
	struct mflock *i_pflock;
};

#define i_addr  i_f.I_addr
#define i_lastr i_f.I_lastr
#define i_rdev  i_d.I_rdev
#define i_frptr i_un.i_fa.I_frptr
#define i_fwptr i_un.i_fa.I_fwptr
#define i_frcnt i_un.i_fa.I_frcnt
#define i_fwcnt i_un.i_fa.I_fwcnt
#ifdef  UCB_QUOTAS
#define i_qused i_q.I_qused
#define i_qmax  i_q.I_qmax
#endif

#ifdef  KERNEL
extern struct inode inode[];    /* The inode table itself */
#endif

/* flags */
#define ILOCK   01              /* inode is locked */
#define IUPD    02              /* file has been modified */
#define IACC    04              /* inode access time to be updated */
#define IMOUNT  010             /* inode is mounted on */
#define IWANT   020             /* some process waiting on lock */
#define ITEXT   040             /* inode is pure text prototype */
#define ICHG    0100            /* inode has been changed */
#define IQUOT   0200            /* directory that has original quota pointer */
#define ISHLOCK 0400            /* shared lock of inode */
#define IEXLOCK 01000           /* exclusive lock of inode */
#define ILWAIT  02000           /* ожидание разблокированного файла (flock)*/
#define IFIR    04000           /* открывая fifo ожидаем    */
#define IFIW    010000          /* открытия с другого конца */
#define ISRCOLL 020000          /* больше 1-го proc ждет select на read */
#define ISWCOLL 040000          /* больше 1-го proc ждет select на write */
#define IL1WAIT 0100000         /* ожидание разблокированного файла (fcntl)*/

/* modes */
#define IFMT    0170000         /* type of file */
#define         IFIFO   0010000 /* fifo special */
#define         IFCHR   0020000 /* character special */
#define         IFMPC   0030000 /* multiplexed char special */
#define         IFDIR   0040000 /* directory */
#define         IFBLK   0060000 /* block special */
#define         IFMPB   0070000 /* multiplexed block special */
#define         IFREG   0100000 /* regular */
#define         IFLNK   0120000 /* symbolic link */
#ifdef AVG_CALLS
#define         IFCALL  0130000 /* @VG call for executable */
#endif
#define         IFQUOT  0140000 /* quota */
#define ISUID   04000           /* set user id on execution */
#define ISGID   02000           /* set group id on execution */
#define ISVTX   01000           /* save swapped text even after use */
#define IREAD   0400            /* read, write, execute permissions */
#define IWRITE  0200
#define IEXEC   0100

#ifdef  UCB_GRPMAST
#define grpmast()       (u.u_uid == u.u_gid)
#endif

#endif  _sys_inode_h_
