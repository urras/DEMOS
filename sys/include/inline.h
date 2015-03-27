#ifndef _sys_inline_h_
#       define  _sys_inline_h_

/*
 * Definitions of inline expansions used for speed,
 * and replacements for them if UNFAST is defined.
 */

#ifndef UNFAST

#define plock(ip) \
{\
	while ((ip)->i_flag & ILOCK) { \
		(ip)->i_flag |= IWANT; \
		sleep((caddr_t)(ip), PINOD); \
	} \
	(ip)->i_flag |= ILOCK; \
}

#define prele(ip) \
{ \
	(ip)->i_flag &= ~ILOCK; \
	if ((ip)->i_flag&IWANT) { \
		(ip)->i_flag &= ~IWANT; \
		wakeup((caddr_t)(ip)); \
	} \
}

/*
 * The GETF macro is used in a conditional, e.g.
 *      if (GETF(fp,fd)) {
 *              u.u_error = EBADF;
 *              return;
 *      }
 */
#define GETF(fp, fd) \
	((unsigned)(fd) >= NOFILE || ((fp) = u.u_ofile[fd]) == NULL)

#ifdef  UCB_FSFIX
#define IUPDAT(ip, t1, t2, waitfor) { \
	if (ip->i_flag&(IUPD|IACC|ICHG)) \
		iupdat(ip, t1, t2, waitfor); \
}
#else
#define IUPDAT(ip, t1, t2) { \
	if (ip->i_flag&(IUPD|IACC|ICHG)) \
		iupdat(ip, t1, t2); \
}
#endif  UCB_FSFIX


#else   UNFAST

#ifdef  UCB_FSFIX
#define IUPDAT(ip, t1, t2, waitfor)     iupdat(ip, t1, t2, waitfor)
#else
#define IUPDAT(ip, t1, t2)      iupdat(ip, t1, t2)
#endif  UCB_FSFIX


#endif  UNFAST

/*
 *  Other macros that are always used, or replace null routines.
 */

#define ISSIG(p)        ((p)->p_sig && \
	((p)->p_flag&STRC || ((p)->p_sig & ~(p)->p_ignsig)) && issig())

#ifndef INTRLVE
#define dkblock(bp)     ((bp)->b_blkno)
#define dkunit(bp)      ((minor((bp)->b_dev) >> 3) & 07)
#endif  INTRLVE

#endif  _sys_inline_h_
