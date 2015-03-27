/* ������ ������������: TEST_DATE TEST_FORM TEST_OUT TEST_IN */
/*
 * datetime.h
 * ��������� ��� ������� �������������� ���� � �������
 * �� �������
 */

# ifndef NULL
# define NULL (char *)0
#endif

/*
 * ��������� ��� ����������� �������
 */
#ifndef TEST_FORM
# define D_COMPL 037 /* ������� ����������������� ������� */
#else
# define D_COMPL '~'
#endif
/* ��������� ������� */
struct tabform {
	char *da_name;  /* ��� ���� */
	int  *da_addr;  /* ����� ���������� */
	char *da_form;  /* ������ �������������� */
	char da_off;    /* ��� ������������ � ���� */
	char da_flen;   /* ����� ���� */
};
/* ��������� �������������� */
struct datebase {
	long (*tolong)();
	int (*fromlong)();
	char *convbuf;
	struct tabform *tb;
	char *dflt;
};
extern struct datebase b_date,b_time;
extern char *to_compl(), *do_compl();
extern char *to_char();
extern long from_char();
extern int len_char();
