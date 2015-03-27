#ifndef _sys_filsys_h_
#       define  _sys_filsys_h_

/*
 * Structure of the super-block
 */
struct  filsys
{
	u_short s_isize;                /* size in blocks of i-list */
	daddr_t s_fsize;                /* size in blocks of entire volume */
	short   s_nfree;                /* number of addresses in s_free */
	daddr_t s_free[NICFREE];        /* free block list */
	short   s_ninode;               /* number of i-nodes in s_inode */
	ino_t   s_inode[NICINOD];       /* free i-node list */
	char    s_flock;                /* lock during free list manipulation */
	char    s_ilock;                /* lock during i-list manipulation */
	char    s_fmod;                 /* super block modified flag */
	char    s_ronly;                /* mounted read-only flag */
	time_t  s_time;                 /* last super block update */
	daddr_t s_tfree;                /* total free blocks*/
	ino_t   s_tinode;               /* total free inodes */
	short   s_dinfo[2];             /* interleave stuff */
#define s_m     s_dinfo[0]
#define s_n     s_dinfo[1]
	char    s_fsmnt[12];            /* ordinary file mounted on */
	ino_t   s_lasti;                /* start place for circular search */
	ino_t   s_nbehind;              /* est # free inodes before s_lasti */
	char    s_fpack[6];             /* volume label */
};

#ifdef KERNEL
# ifdef EXMOUNT
struct  buf    *getfs();
# else
struct  filsys *getfs();
# endif EXMOUNT
#endif

#endif  _sys_filsys_h_
