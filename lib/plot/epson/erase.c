/*
 * Очистка экрана - Epson-80
 * by @VG
 */

#include "eps.h"

erase()
{
	register int *p, *q;

	p = (int *)_Field;
	q = (int *)((char *)_Field + sizeof _Field);
	while( p < q )
		*p++ = 0;
}
