#ifndef _grp_h_
#       define  _grp_h_

struct  group { /* see getgrent(3) */
	char    *gr_name;
	char    *gr_passwd;
	int     gr_gid;
	char    **gr_mem;
};

#endif _grp_h_
