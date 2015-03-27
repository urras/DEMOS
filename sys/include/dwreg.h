#ifndef _sys_dwreg_h_
#       define  _sys_dwreg_h_

struct dwdevice {
	short   dw_rid;         /* ������� �������������          */
	short   dw_m1;          /* unused                         */
	short   dw_err;         /* ������� ������/��������������� */
	short   dw_sec;         /* ����� �������                  */
	short   dw_data;        /* ������� ������                 */
	short   dw_cyl;         /* ����� ��������                 */
	short   dw_trk;         /* ����� �����������              */
	short   dw_rks2;        /* ��� 2                          */
	short   dw_stat;        /* ������� ���������/���������    */
};

/* bits in dw_err; ������� ���� ������ ��������, ������� - ������ ������� */
/* bits 15-8 �������� ���� ������ */
/* bit 15 is unused */
#define DWER_CCD       0040000          /* ������ ��� ������ */
#define DWER_CCA       0020000          /* ������ ��� ������ */
#define DWER_NXS       0010000          /* ������ ������ ������ */
/* bit 11 is unused */
#define DWER_ERR       0002000          /* ������������� */
#define DWER_SKE       0001000          /* ������ ���������������� */
#define DWER_MKD       0000400          /* ������ ������ ������� ������ */
/* � bits 7-0 ������������ ����� ��������,
   � �������� ���������� ��������������� ���� ������ */
#define DWER_BITS       "\10\17CCD\16CCA\15NXS\13ERR\12SKE\11MKD"

/* bits in dw_rks2; ������� ���� ������ ��������, ������� - ������ ������� */
/* bits 15-8 �������� ��������� ���������� ����� �������� */
/* bit 15 is unknown */
#define DWCS_DRDY       0040000         /* ���������� ����� */
#define DWCS_WRE        0020000         /* ������ ������ */
#define DWCS_INI        0010000         /* ��������� ��������� */
#define DWCS_ZOB        0004000         /* ������ ������ 2 */
/* bits 9-10 are unused */
#define DWCS_ERR        0000400         /* ������ */
/* ���� 7-0 �������� ��� ��������  -- commands */
#define DWCS_SEEK0      0000020         /* ���������������� �� 0 ������� */
#define DWCS_RCOM       0000040         /* read */
#define DWCS_WCOM       0000060         /* write */
#define DWCS_FORMAT     0000120         /* ������ */
#define DWCS_BITS       "\10\17RDY\16WRE\15INIY1\14ZOB\11ERR"

/* bits in dw_stat */
#define DWDS_BSY        0100000         /* ����� */
/* bits 14-9 are unused */
#define DWDS_TYP        0000400         /* ��� ���������� */
#define DWDS_ZOA        0000200         /* ������ ������ 1 */
#define DWDS_IE         0000100         /* ���������� ���������� */
/* bits 5-4 are unused */
#define DWDS_INI        0000010         /* ��������� ��������� */
/* bits 2-1 are unused */
#define DWDS_ORDY       0000001         /* �������� ��������� */

#endif  _sys_dwreg_h_
