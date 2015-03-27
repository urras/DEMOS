/*
**  LOCK.H -- Concurency structs and global variables
**
**	Version:
**		@(#)lock.h	7.1	2/5/81
*/

# ifndef KEYSIZE

# define	M_SHARE		2
# define	M_EXCL		1
# define	T_CS		0
# define	T_PAGE		1
# define	T_REL		2
# define	T_DB		3
# define	A_RTN		1
# define	A_SLP		2
# define	A_RLS1		3
# define	A_RLSA		4
# define	KEYSIZE		12
#ifndef FCNTL
struct lockreq
{
	char	lract;			/* requested action
					 *	=1 request lock,err return
					 *      =2 request lock,sleep
					 *	=3 release lock
					 *	=release all locks for pid
					 */
	char	lrtype;			/* type of lock:
					 *   =0, critical section lock
					 *   =1, page lock
					 *   =2, logical lock
					 *   =3, data base lock
					 */
	char	lrmod;			/* mode of lock
					 *	=1 exclusive lock
					 *	=2 shared lock
					*/
					/* key for the lock */
	char	dbnode[4];		/* inode of data base */
	char	lrel[4];		/* relation tid */
	char	lpage[4];		/* page address		*/
};
# ifdef        NOSOCKET
extern int errno;
#define LOCKOP(fd,buf,ret) {ret = write(fd,buf,KEYSIZE+3);if(ret<0)ret = errno;errno = 0;}
# else
#define LOCKOP(fd,buf,ret) {ret = write(fd,buf,KEYSIZE+3);read(fd,&ret,(sizeof(int)) );}
# endif
#else /* +FCNTL */
#define DB_OFF 0l
#define CS_OFF(tid) 01000l+((long)tid<<1)
#define RL_OFF(tid) 01001l+((long)tid<<1)
#ifndef FTEST
#include <fcntl.h>
#else /* +FTEST */
struct flock {
    int         l_type;
    short	l_whence;
    long	l_start;
    long	l_len;
    short	l_pid;
    short	l_sysid;
};

#define fcntl(fd,t,arg) printf("Fcntl: fd=%d op=%s mod=%s whence=%d start=%ld len=%ld\n",fd,t,(arg)->l_type, (arg)->l_whence, (arg)->l_start, (arg)->l_len)
#define F_SETLK "F_SETLK"
#define F_SETLKW "F_SETLKW"
#define F_RDLCK "F_RDLCK"
#define F_WRLCK "F_WRLCK"
#define F_UNLCK "F_unlck"
#endif
#endif
extern char	Acclock;		/* locks enabled flag */
extern int	Alockdes;		/* file descriptor for lock device*/
extern int	Lockrel;		/* lock relations flag*/
# endif



