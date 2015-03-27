/*
	computes a^b.
	uses log and exp
*/
/*! Modified by Bruce R. Julian, USGS Menlo Park, Calif.  16 NOv 1982 */
/*! to use multiplication for integral exponents, using binary method */
/*! (Knuth, Seminumerical Algorithms, First ed., sec. 4.6.3)          */

#include	<errno.h>
int errno;
double log(), exp();

double
pow(arg1,arg2)
double arg1, arg2;
{
	double	temp;
	long	l;
	int	n, odd;

	if ((n=arg2) == arg2 && n != 0) {		/* Integral exponent */
		if (n < 0.) {
			if (arg1 == 0.)
				goto domain;
			n = -n;
			arg1 = 1./arg1;
		}
		temp = 1.;
		for(;;) {
			odd = n & 1;
			n >>= 1;	/* n /= 2; */
			if(odd) {
				temp *= arg1;
				if (n == 0)
					return(temp);
			}
			arg1 *= arg1;
		}
	}

	/* Non-integral or very large exponent */
	if(arg1 <= 0.) {
		if(arg1 == 0.) {
			if(arg2 <= 0.)
				goto domain;
			return(0.);
		}
		l = arg2;
		if(l != arg2)
			goto domain;
		temp = exp(arg2 * log(-arg1));
		if(l & 1)
			temp = -temp;
		return(temp);
	}
	return(exp(arg2 * log(arg1)));

domain:
	errno = EDOM;
	return(0.);
}
