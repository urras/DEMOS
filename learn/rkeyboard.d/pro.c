#include <signal.h>
#include <stdio.h>
main()
{ 
	char c='*';
	char d=' ';
	char b=07;
	int i=0;
	printf ("\n * ");
	putchar(b);
	for (i;i<=100 ;i++ ) {
		sleep(1);
		putchar(c);
		putchar(d);
		putchar(b);
	}
	wait(0);
}
