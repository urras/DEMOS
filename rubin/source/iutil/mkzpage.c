# include	<defines.h>
# include	<access.h>
# include	<sccs.h>

SCCSID(@(#)mkzpage.c   7.1     2/5/81)

/*
 * mk_zpage(b)
 * ������� ������ ��������, ������� ��������� �� ���� � �����
 * ����������: � ������ S_NOZERO ��� ������� ����� ������ ���������
 * �����, � ������� "mainpg = thispage+1" (�� ���� ���, �����
 * ����������), �� ���� �� �������. ��� ������ ��� ���������
 * �� "mainpg == 0" (��� ������ ��������� ����� 0 ����������
 * �� MAIN_ZERO).
 * ������ ��������� ��������������� ����� ���� � ���������� ���������.
 */
mk_zpage(b)
register struct accbuf *b;
{
    b->mainpg = b->thispage + 1;
    b->linetab[0] = (int)b->firstup - (int)b;
    /*      b->nxtlino = 0;         � ��� 0 */
    /*      b->ovflopg = 0;         � ��� 0 */
}
