#ifndef _ar_h_
#       define  _ar_h_

#define ARMAG   0177545

struct  ar_hdr {
	char    ar_name[14];      /* 0x00 */
#ifdef sparc
	short	ar_date[2];       /* 0x0e  P3: Misaligned */
#else
	long    ar_date;
#endif
	char    ar_uid;           /* 0x12 */
	char    ar_gid;           /* 0x13 */
	short   ar_mode;          /* 0x14 */
#ifdef sparc
	short	ar_size[2];
#else
	long    ar_size;          /* 0x16 */
#endif
};

#endif _ar_h_
