/* === CDEBUGGER === ���������� ������ === include file */
/*
 * �����        ������ �.�.
 * 19.02.86     ������ �.�. - �������� ����� SEPID ��� �����������-79
 */
#include <stdio.h>
extern int errno;                       /* ���� ��������� ������         */

#ifndef SEPID
#define get_iword get_word
#endif

/* ��������� ��������� ������������ ��������� */
char *pcmn;                             /* ��������� ������� �������     */
int  last_line;                         /* ����� ��������� ���������� ���*/
int  act_line;                          /* ������, ������� ����� ������. */
int  act_adr;                           /* ����� �������� � C-���������  */
FILE *act_file;                         /* ���������� �������.�����.�����*/
int  regval[9];                         /* ���������� ���������          */
int  stp_cause;                         /* ������� �������� ���.���������*/
int  jsr_ign;                           /* ���� - ������� �� � ��������. */
int  stp_adr;                           /* ����� ������� (0-���, 1-����) */
int  dsp_inp, dsp_out;                  /* ������� ��� ����� � ������    */
int  language;                          /* ���� ���������������� ������  */

enum {C, FORTRAN};


/* ������������ ���������� � ������� 'cdebug' ������������ 'cdebflag' */
#define SR0   -16
#define SR1   -14
#define SR2   -12
#define SR3   -10
#define SR4    -8
#define SR5    -6
#define SSP    -4
#define SPC    -2
int     CDEBFLAG;
#define PFRAME  2
#define STPADR  4
#define FROM    6
#define LENG    8
#define VALUE  10
