#ifndef _sys_mount_h_
#       define  _sys_mount_h_

/*
 * Mount structure.
 * One allocated on every mount.
 * Used to hold the super block.
 */
struct  mount
{
	dev_t   m_dev;                  /* device mounted */
	struct  inode   *m_inodp;       /* pointer to mounted on inode */
#ifndef EXMOUNT
	struct  filsys  m_filsys;       /* superblock data */
#else
	struct  buf     *m_filsys;      /* pointer to superblock in cache */
#endif
};

# ifdef KERNEL
extern  struct  mount   mount[];
# endif

#endif  _sys_mount_h_
