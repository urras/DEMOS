#include "eps.h"
#include <stdio.h>

openvt ()
{
	openpl();
}

int _Color = 1;

openpl()
{
	putchar(033); putchar('A'); putchar(PINS); /* PINS*1/72" shift */
	putchar(033); putchar('E');                /* font ELITA  (1/12") */
}
