#ifndef _timeb_h_
#       define  _timeb_h_
#include <sys/types.h>

/*
 *       ��������� ������������ ftime().
 */

struct timeb{
	time_t   time;    /* ����� � �������� �� �������� ��
			     1-���-70 00:00:00                  */
	unsigned millitm; /* ����� � ������������ ( ������ 0 )  */
	short    timezone;/* ������� ��������� ���� ( � �������
			     � �������� ����������� �� �������� */
	short    dstflag; /* ���� ������� �������               */
};

#endif  _timeb_h_
