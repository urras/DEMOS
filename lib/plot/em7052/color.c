#include <stdio.h>

color(col)
{
	if(col > 0)
		col = ((col-1)%4)+1;

	putchar('P');
	putchar('0'+col);
}
