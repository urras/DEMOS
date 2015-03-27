#include <stdio.h>

closevt()
{
	putch(033); putch('0');
	fflush(stdout);
}

closepl()
{
	putch(033); putch('0');
	fflush(stdout);
}
