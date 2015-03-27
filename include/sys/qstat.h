#ifndef _sys_qstat_h_
#       define  _sys_qstat_h_

struct qstat
{
	int    qs_flag;
	int    qs_count;       /* reference count */
	dev_t   qs_dev;         /* device where inode resides */
	ino_t   qs_number;      /* i number, 1-to-1 with device address */
	u_short qs_mode;
	short   qs_nlink;       /* directory entries */
	short   qs_uid;         /* owner */
	short   qs_gid;         /* group of owner */
	off_t   qs_size;        /* size of file */
	union {
		struct {
			daddr_t _qs_addr[NADDR]; /* if normal file/directory */
			daddr_t _qs_lastr;       /* last logical block read  */
		}_1_;
#                       define  qs_addr     _1_._qs_addr
#                       define  qs_lastr    _1_._qs_lastr
		struct {
			daddr_t _qs_rdev;                /* qs_addr[0] */
		}_2_;
#                       define  qs_rdev     _2_._qs_rdev
		struct {
			daddr_t _qs_qused;
			daddr_t _qs_qmax;
		}_3_;
#                       define  qs_qused    _3_._qs_qused
#                       define  qs_qmax     _3_._qs_qmax
	} qs_un;
	time_t  qs_atime;       /* access time */
	time_t  qs_mtime;       /* modification time */
	time_t  qs_ctime;       /* creation time */
};

#endif  _sys_qstat_h_
