/*
 * ���������� libplot, ��������� �������
 *
 * ����� R/G/B � ��������� �� 0 �� 1023
 */

#include <stdio.h>

colormap(color, R, G, B)
{
	register cm;

	putc('M', stdout);
	putsi(color);
	putsi(R);
	putsi(G);
	putsi(B);
}
