/*
char id_qsort[] = "@(#)qsort_.c	1.1";
 *
 * quick sort interface
 *
 * calling sequence:
 *	external compar
 *	call qsort (array, len, isize, compar)
 *	----
 *	integer*2 function compar (obj1, obj2)
 * where:
 *	array contains the elements to be sorted
 *	len is the number of elements in the array
 *	isize is the size of an element, typically -
 *		4 for integer, float
 *		8 for double precision
 *		(length of character object) for character arrays
 *	compar is the name of an integer*2 function that will return -
 *		<0 if object 1 is logically less than object 2
 *		=0 if object 1 is logically equal to object 2
 *		>0 if object 1 is logically greater than object 2
 */

#include	"../libI77/fiodefs.h"

qsort_(array, len, isize, compar)
ftnint *len, *isize;
ftnint *array;
int (*compar)(); /* may be problematical */
{
	qsort(array, (int)*len, (int)*isize, compar);
}
