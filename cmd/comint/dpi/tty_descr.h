/* ��������� �������� ��������� */

#ifndef    tty_descr_h
#   define tty_descr_h
#   ifndef       tty_codes_h
#       include "tty_codes.h"
#   endif

#undef NULL
#define NULL ((char*)0)
                                     /* ��������� ����������� �������� */
#define cnt     0
#define esc_cnt 1
#define esc     2
#define prf     3
#define prf_cnt 4
#define esc_sgn 5

#define c(x) (('x'&037)+040*cnt)     /*          control-�����  */
#define e(x) (('x'&037)+040*esc)     /* escape + �����          */
#define d(x) (('x'&037)+040*esc_sgn) /* escape + ���� (����. �sc-8) */
#define s(x) (('x'&037)+040*esc_cnt) /* escape + control-�����  */
#define P(x) (('x'&037)+040*prf)     /* ������� + ������       +�������*/
#define C(x) (('x'&037)+040*prf_cnt) /* ������� + contr-������ +�������*/

static struct s_tdescr {          /* ��������� �������� ��������� */
    char         sp_inp[32];          /* ���� ����-�������� ��� ����� */
    char         sp_fnc[12];          /* ���� �������������� ������ */
    char         sp_out[16];          /* ���� ����-�������� ��� ������ */
    struct s_dpd param;               /* ��������� ��������� */
    struct s_color color;
    char        *beg,  *end;          /* ������ ����. � ������. ��� 0 */
    char        *pref, *suff;         /* ������� � ������� ��� 0 */
    char        *sdpp;                /* ������ ���������������� ��� 0 */
    char         xdpp, ydpp, ldpp;    /*   ���������� � ��� � �� ����� */
    char         escape;              /* ��� escape ��� 0 */
    char        *scf;                 /* ��������� ��������� ����� Cf */
    char        *scb;                 /* ��������� ��������� ����� Cb  */
    char        *smf;                 /* ��������� ����� Mf */
    char        *smb;                 /* ��������� ����� Mb */
    char        *gr_beg, *gr_end;     /* ��������� � ����. ������������� */
    char        *s_g[6];              /* ��������� ����������. �������� */
};

#endif
