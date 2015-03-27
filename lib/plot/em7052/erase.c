/*
 * Смена листа - ЭМ-7052
 * by @VG
 */

#include <stdio.h>

int _draws = 0;

erase()
{
	if( _draws ) {
		putchar('N');
		_draws = 0;
	}
}
