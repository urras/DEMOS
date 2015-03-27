/*
 * datetime.h
 * �������� �������� �������� ��������������
 * ���� � �������
 */
# ifndef MAX_LDATE
# define        MAX_LDATE       48      /* ����. ����� ������ */
typedef long t_date,t_time; /* ���� � 000 ����, 1 ������ ��� ����� �� */
/*
 * �������� ��� datetime.h � ../h
 *
 * 1. ������� ��� �������������� �� ����/������� � ������ � �������
 */
extern char *date_char(), *time_char();
extern t_date char_date();
extern t_time char_time();
/*
 * 2. ������� ������ ����� ���� ����/�������
 */
extern int date_len(), time_len();
/*
 * ���� ������ (������ ������ RUBIN)
 */
# define        BADDATE        2144    /* ������ ������ ������� */
# define        BADTIME        2140    /* ������ ������ ���� */

/*
 * ������� ������� ���������� ������� � ����
 */
extern newdati();
extern long sys_date,sys_time;
# define get_date() (sys_date<0? (newdati(), sys_date):sys_date)
# define get_time() (sys_time<0? (newdati(), sys_time):sys_time)
# define new_datime()  {sys_date = sys_time = -1;}

/*
 * ��������� ��� ����������� ���� � �������
 * ��������������� ������� ������� ���� ����
 */
struct cd_date {
	int cd_year;    /* ���   0000  */
	int cd_month;   /* ����� (1-12)*/
	int cd_day;     /* ����  (1-31)*/
	int cd_2year;   /* ��� ����    */
	int cd_week;    /* ���� ������ */
};
struct cd_time {
	int cd_hh;      /* ���         */
	int cd_mm;      /* ������      */
	int cd_ss;      /* �������     */
};

extern date_cd(/* t_date dd,struct cd_date *cd */ );
extern time_cd(/* t_time dd,struct cd_time *cd */ );
extern t_date cd_date( /* struct cd_date *cd */ );
extern t_time cd_time( /* struct cd_time *cd */ );

/*
 * ��������������� (��� ��������� ��� equel)
 */
# ifdef EQUEL
# define length IIlength
# define smove  IIsmove
# endif
#endif
