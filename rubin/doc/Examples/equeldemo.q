/*
*  ��� ������� ������ ��������� �� EQUEL. ��� �������� �
*  ���������� �������� � ���� ������ "demo".
*  ����� ������ ��������� ������� ������� ���������, � ��������� ����
*  ������ ��� �����, ���� ��������� � ���, ��� ������ ��������� ���.
*  ������ �������� ����� ��������, ������ "?" � ����� �� ������ �����.
*
*  ��� ������ ������� CTRL/D.
*
*  ���������� � ������ ���� ��������� �� ���� ��������������
*  ���������:
*
*  equel equeldemo.q
*  cc    equeldemo.c -lq
*  a.out
*/
main(ac,av)
int ac;
##char *av[];
{
##	char	NAME[20];
##	int	SAL;
	char	flag;

	if ( ac != 2)
##      rubin demo
	else
##      rubin av[1]
##      range of e is ��������
	while (eread(NAME))
	{
		if(NAME[0] == '?')
		{
##                      retrieve (NAME=e.���)
##			{
				printf("%s\n",NAME);
##			}
			continue;
		}
		flag = 0;
##              retrieve (SAL = e.�����) where
##              e.��� = NAME
##		{
			printf("���. %s ����� ����� %d ���.\n",NAME,SAL);
			flag = 1;
##		}
	if(!flag) printf("���. %s ���������� �������\n",NAME);
	}
##	exit
}

eread(p)
char	*p;
{
	int    c;
	printf("������� ��� ���������:");
	while((c = getchar()) > 0)
	{
		if(c == '\n')
		{
			*p = 0;
			return(1);
		}
		*p++ = c;
	}
	return(0);
}
