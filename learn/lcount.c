#include <stdio.h>

main()  /* ������� ����� */
{
	register n, c;

	n = 0;
	while ((c = getchar()) != EOF)
		if (c == '\n')
			n++;
	printf("%d\n", n);
}
