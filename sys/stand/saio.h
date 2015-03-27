/*
 * ���� ��������� ��� ������ ��������� ���������� ��������
 *
 * $Log:	saio.h,v $
 * Revision 1.1  86/07/14  19:14:31  avg
 * Initial revision
 * 
 */

/*
 * ���� �/� : ��������
 * I-����, ������ ��� seek, �.�.�,
 * � �����.
 */
struct  iob {
	char    i_flgs;
	struct inode i_ino;
	int i_unit;
	daddr_t i_boff;
	daddr_t i_cyloff;
	off_t   i_offset;
	daddr_t i_bn;
	char    *i_ma;
	int     i_cc;
#ifndef UCB_NKB
	char    i_buf[512];
#else
	char    i_buf[BSIZE];
#endif
};

#define F_READ  01
#define F_WRITE 02
#define F_ALLOC 04
#define F_FILE  010


/*
 * ������������� ���������
 */
struct devsw {
	char    *dv_name;
	int     (*dv_strategy)();
	int     (*dv_open)();
	int     (*dv_close)();
};

struct devsw devsw[];

/*
 * ���� �������. ������ ���� ���������� F_XXX.
 */
#define READ    1
#define WRITE   2


#define NBUFS   4


#ifndef UCB_NKB
char    b[NBUFS][512];
#else
char    b[NBUFS][BSIZE];
#endif
daddr_t blknos[NBUFS];



#define NFILES  4
struct  iob iob[NFILES];

/*
 * ��������� ����� �� ��������� �� 32K ���� ��������� ������������.
 * ������ ��������������� � ���������������� ��������� main.
 */
int     segflag;
