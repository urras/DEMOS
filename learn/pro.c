#include <stdio.h>
main()
{ 
	char c='*';
	char d=' ';
	char b=07;
	printf ("\n * ");
	putchar(b);
	for ( ; ; ) {
		sleep(1);
		putchar(c);
		putchar(d);
		putchar(b);
	}
}
