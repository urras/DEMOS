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
char*av[];
{
char NAME[20];int SAL;

	char	flag;

	if ( ac != 2)
{IIrubin("demo",0);}
	else
{IIrubin(av[1],0);}
{IIwrite("range of e=��������");IIsync(0);}
	while (eread(NAME))
	{
		if(NAME[0] == '?')
		{
{IIwrite("retrieve(NAME=e.���)");IIsetup();while(IIn_get(0)){IIn_ret(NAME,3);if(IIerrtest())continue;

				printf("%s\n",NAME);
}IIflushtup(0);IIsync(0);}
			continue;
		}
		flag = 0;
{IIwrite("retrieve(SAL=e.�����)where e.���=");IIcvar(NAME,3,0);IIwrite("");IIsetup();while(IIn_get(0)){IIn_ret(
&SAL,1);if(IIerrtest())continue;

			printf("���. %s ����� ����� %d ���.\n",NAME,SAL);
			flag = 1;
}IIflushtup(0);IIsync(0);}
	if(!flag) printf("���. %s ���������� �������\n",NAME);
	}
{IIexit();}
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
