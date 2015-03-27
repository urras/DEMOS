/* �������� ���������� � ��������� ��������� */

/*
 * $Log:        run.h,v $
 * Revision 1.1  89/08/19  18:11:15  rygoff
 * Initial revision
 * 
 *
 */


#define maxrun 4                 /* ����� ������������ ��������� */
#define x_run  3                 /* ������� ����� �� ������ */
#define w_run  (D_NAME+5)        /* ������ ������� */

int   ch_flag;                   /* ��������� �� �������� */

struct run {
    int  r_pid;                  /* ���������� �������� */
    int  r_code;                 /* ��� �������� ��� ���������� */
    int  r_mark;                 /* ������� (������ -) */
    char r_name[D_NAME+1];       /* ��� ����� */
} at_run[maxrun];

#ifdef USG
#  undef SIGCHLD
#  define SIGCHLD SIGCLD
#else
#  define fork vfork
#endif
