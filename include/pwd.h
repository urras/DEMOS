#ifndef _pwd_h_
#       define  _pwd_h_

struct  passwd { /* see getpwent(3) */
	char    *pw_name;
	char    *pw_passwd;
	int     pw_uid;
	int     pw_gid;
	int     pw_quota;
	char    *pw_comment;
	char    *pw_gecos;
	char    *pw_dir;
	char    *pw_shell;
};

#endif _pwd_h_
