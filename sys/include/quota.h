#ifndef _sys_quota_h_
#       define  _sys_quota_h_

#ifdef	UCB_QUOTAS
#define	isquot(ip)	(((ip)->i_mode & IFMT) == IFQUOT)
#endif

#endif  _sys_quota_h_
