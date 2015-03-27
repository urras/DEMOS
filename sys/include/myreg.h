#ifndef _sys_myreg_h_
#       define  _sys_myreg_h_

struct mydevice {
	short   my_csr;         /* ������� ������ */
	short   my_dat;         /* ������� ������ */
};

/* bits in my_csr */
#define MY_ERR          0100000         /* ������ */
#define MY_RESET        0040000         /* ��������� � ���.��������� */
/* bits 13-8 �������� ��� ������� ���������� */
#define MY_TR           0000200         /* ���������� ������ */
#define MY_IE           0000100         /* ���������� ���������� */
#define MY_DONE         0000040         /* ���������� */
/* bits 4-1 �������� ������� */
#define MY_GO           0000001         /* ����� */

/* commands */
#define MY_READ         0000000         /* ������ */
#define MY_WRITE        0000002         /* ������ */
#define MY_READL        0000004         /* ������ � ������ */
#define MY_WRITL        0000006         /* ������ � ������ */
#define MY_READT        0000010         /* ������ ������� */
#define MY_READH        0000012         /* ������ ��������� */
#define MY_FORMAT       0000014         /* ������ */
#define MY_SEEK         0000016         /* ���������������� */
#define MY_SET          0000020         /* ��������� ���������� */
#define MY_RSTATE       0000022         /* ������ �������� ��������� */
#define MY_LOAD         0000036         /* �������� */

/* ���� �������� ��������� (����� ������ ��� � my_dat) */
#define MYDS_WLO        0000001         /* ������ ������ ��� crc �� ������ */
#define MYDS_CRC        0000002         /* crc �� ������ ���� ��������� */
#define MYDS_INI        0000004         /* ���������� ��������� ��������� */
#define MYDS_SK0E       0000010         /* ������ ���-�� �� 0-������� */
#define MYDS_SKE        0000020         /* ������ ������ ������� */
#define MYDS_NXS        0000040         /* ������ ������ ������� */
#define MYDS_LAB        0000100         /* �������� ��� �������� � ������ */
#define MYDS_ROT        0000200         /* ������� ��������� */
/* ���� 8-9 ��������� ����� ���������� ���������� */
/* ��� 10 ��������� ����� ��������� ����������� */
#define MYDS_NXM        0004000         /* �������������� ����� */
#define MYDS_MKA        0010000         /* ��� ������� ������ */
#define MYDS_MKD        0020000         /* ��� ������� ������ */
#define MYDS_FMT        0040000         /* ������������� �������� */
#define MYDS_ERR        0100000         /* ������ ����������� */

#define MYDS_BITS \
"\10\20ERR\17FMT\16MKD\15MKA\14NXM\10ROT\7LAB\6NXS\5SKE\4SK0E\3INI\2CSE\1WLO"

#endif  _sys_myreg_h_
