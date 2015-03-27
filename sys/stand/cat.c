/*
 * ���������� cat
 *
 * $Log:	cat.c,v $
 * Revision 1.2  88/03/28  10:27:48  korotaev
 * ��������� ����� ����� �������� � AZLK.
 * 
 * Revision 1.1  86/07/14  19:14:23  avg
 * Initial revision
 * 
 */
#define FILE_DFL "tm(0,2)"
#define MAXLINES 16
#define MAXLLINES 66
char module[] = "cat";          /* ��� ������ -- ��� trap */

int lpout;
main()
{
	int c, i;
	char *buf,buffer[50];
	int line=0;
	buf = buffer;
	printf("%s: �������� � ������ ������\n",module);
	do {
		buf = buffer;
		printf("���� (%s �� ���������):\17 ",FILE_DFL);
		buffer[0] = 0;
		gets(buffer);
		if(*buf == 0 || *buf == '\n') buf = FILE_DFL;
		printf("\n\n ���� %s\n", buf);
		i = open(buf, 0);
	} 
	while (i <= 0);
	printf("�����: (lp - ����, tty - ��������) (tty): \17");
	gets(buffer);
	if(buffer[0] == 'l') {
		lpout=1;
		printf("�������� ���� � ������� <��>\n");
		gets(buffer);
	}
	while ((c = getc(i)) > 0)
	{
		if(lpout) {
		lputchar(c);
			if ( c == '\n' && ++line > MAXLLINES )
			{
				line = 0;
				lputchar('\f');
			}
		}
		else {
			putchar(c);
			if ( c == '\n' && ++line > MAXLINES )
			{
				line = 0;
				printf("cont?"); 
				if(getchar()=='n') exit(1);
				printf("\b\b\b\b\b\b      \b\b\b\b\b\b");
			}
		}
	}
	if(lpout) lputchar('\f');
	exit(0);
}
