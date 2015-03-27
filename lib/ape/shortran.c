#include "lint.h"
#ifndef lint
static char	sccs_id[] = "@(#)fortran.c	2.1	7/6/82";
#endif lint
	/* These routines mediate linkage of the ape
	 * routines by f77 programs. */
#include <ape.h>

#ifdef	I2
typedef	fortint	short;
#else
#ifdef	I4
typedef	fortint	long;
#endif
#endif
typedef	fortlen	long;

	/* initialization: */

new_(n)
fortint *n;
{
	PMINT local;

	new(&local);
	*n = (fortint)local;
}

itom_(n,a)
fortint *n, *a;
{	*a = (fortint) itom(*n);	}

stom_(str,a,strlength)
char *str;
fortint *a;
fortlen strlength;
{	*(str+(int)strlength) = '\0'; /* Is this necessary? */
	*a = (fortint) stom(str);
}


	/* removal: */

xfree_(a)
fortint *a;
{	xfree((PMINT)(*a));	}

afree_(a)
fortint *a;
{	afree((PMINT)(*a));	}


	/* operations: */

madd_(a,b,c)
fortint *a, *b, *c;
{	madd((PMINT)(*a),(PMINT)(*b),(PMINT)(*c));	}

msub_(a,b,c)
fortint *a, *b, *c;
{	msub((PMINT)(*a),(PMINT)(*b),(PMINT)(*c));	}

mult_(a,b,c)
fortint *a, *b, *c;
{	mult((PMINT)(*a),(PMINT)(*b),(PMINT)(*c));	}

mdiv_(a,b,c,d)
fortint *a, *b, *c, *d;
{	mdiv((PMINT)(*a),(PMINT)(*b),(PMINT)(*c),(PMINT)(*d));	}

sdiv_(a,b,c,d)
fortint *a, *b, *c, *d;
{	sdiv((PMINT)(*a),(int)*b,(PMINT)(*c),(short *)*d);	}

gcd_(a,b,c)
fortint *a, *b, *c;
{	gcd((PMINT)(*a),(PMINT)(*b),(PMINT)(*c));	}

pow_(a,b,c,d)
fortint *a, *b, *c, *d;
{	pow((PMINT)(*a),(PMINT)(*b),(PMINT)(*c),(PMINT)(*d));	}

rpow_(a,b,c)
fortint *a, *b, *c;
{	rpow((PMINT)(*a),(int)(*b),(PMINT)(*c));	}


	/* Input and output:
	 * Because I don't feel up to worrying
	 * about how f77 does file manipulations,
	 * I'm only providing I/O from std(in/out),
	 * for bases 10 and 8. */
#include <stdio.h>

minput_(a,i)	/* Name can't be "min" because of Fortran min function! */
fortint *a, *i;
{	*i = m_in((PMINT)(*a),10,stdin);	}

omin_(a,i)
fortint *a,*i;
{	*i = m_in((PMINT)(*a),8,stdin);	}

mout_(a)
fortint *a;
{	m_out((PMINT)(*a),10,stdout);	}

omout_(a)
fortint *a;
{	om_out((PMINT)(*a),stdout);	}

	/* conversions: */

mtovec_(a,length,vect)
fortint *a, *length;
short *vect;
{
	int abslen, i;
	PMINT local;

	local = (PMINT)*a;
	*length = local->len;
	abslen = (*length >= 0 ? *length : -*length);
	for (i=0; i<abslen; ++i)
		vect[i] = local->val[i];
}

vectom_(length,vect,a)
fortint *length, *a;
short *vect;
{
	int abslen, i;
	PMINT local;

	new(&local);
	if ((local->len = (int)*length) == 0)
		{
		*a = (fortint)local;
		return;
		}
	abslen = (*length >= 0 ? *length : *length);
	local->val = xalloc(abslen, "vec-to-m");
	for (i=0; i < abslen; ++i)
		local->val[i] = vect[i];
	mcan(local);
	*a = (fortint) local;
	return;
}
