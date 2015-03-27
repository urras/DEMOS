/*	sqrt - floating-point square root */
/*	Optimized for PDP-11 */
/*	by Bruce R. Julian, U.S. Geological Survey, Menlo Park, CA    22 Nov 1982*/

/*	calls frexp */

#include <whoami.h>
#include <errno.h>
#define SQRT2 1.4142135623730950488

int errno;
double frexp();

double
sqrt(arg)
double arg;
{
	double x, temp;
	int exp;

	/* Test for negative or zero argument */
	if(arg <= 0.) {
		if(arg < 0.)
			errno = EDOM;
		return(0.);
	}

	/* Split into fraction and exponent and estimate sqrt(fraction) */
	x = frexp(arg,&exp);

#ifndef PDP11
	/* Insure fraction is in range (.5,1] */
	/* (Necessary only with non-binary floating point) */
	while (x < 0.5) {
		x *= 2.;
		exp--;
	}
#endif

	temp = 0.59016*x + 0.41731;		/* Minimax relative error on [.5, 1] */

	/* Make exponent even */
	/* NOTE: This won't work on 1's complement machines */
	if(exp & 1) {				/* Exponent odd? */
		temp *= SQRT2;
		exp--;
	}

	/* Get exponent into range [-60, 60] */
	while(exp > 60) {
		temp *= (1L<<30);
		exp -= 60;
	}
	while(exp < -60) {
		temp /= (1L<<30);
		exp += 60;
	}

	/* Multiply temp by sqrt(2**exp) */
	if(exp >= 0)
		temp *= 1L << (exp/2);
	else
		temp /= 1L << (-exp/2);

	/* Newton's method (3 iterations) */
	temp = 0.5*(temp + arg/temp);
	temp = temp + arg/temp;
	return(0.25*temp + arg/temp);
}
