#include "lint.h"
#ifndef lint
static char	sccs_id[] = "@(#)pointers.c	2.1	7/6/82";
#endif lint

#include <ape.h>

PMINT padd(a,b)
PMINT a,b;
{
	PMINT c;

	new(&c);
	madd(a,b,c);
	return (c);
}

PMINT pmult(a,b)
PMINT a,b;
{
	PMINT c;

	new(&c);
	mult(a,b,c);
	return (c);
}

PMINT psub(a,b)
PMINT a,b;
{
	PMINT c;

	new(&c);
	msub(a,b,c);
	return (c);
}

PMINT pgcd(a,b)
PMINT a,b;
{
	PMINT c;

	new(&c);
	gcd(a,b,c);
	return (c);
}

PMINT pdiv(a,b)
PMINT a,b;
{
	PMINT c, junk;

	new(&c);
	new(&junk);
	mdiv(a,b,c,junk);
	afree (junk);
	return (c);
}

PMINT pmod(a,b)
PMINT a,b;
{
	PMINT d, junk;

	new(&d);
	new(&junk);
	mdiv(a,b,junk,d);
	afree (junk);
	return (d);
}

PMINT psdiv(a,n)
PMINT a;
int n;
{
	PMINT c;
	short *junk = 0;

	new(&c);
	sdiv(a,n,c,junk);
	shfree (junk);
	return (c);
}

smod(a,n)
PMINT a;
int n;
{
	PMINT  junk;
	short d = 0;

	new(&junk);
	sdiv(a,n,junk,&d);
	afree (junk);
	return (d);
}

PMINT psqrt(a)
PMINT a;
{
	PMINT b,r;

	new(&b);
	new(&r);
	ignore(msqrt(a,b,r));
	afree(r);
	return(b);
}

PMINT remsqrt(a)
PMINT a;
{
	PMINT b,r;

	new(&b);
	new(&r);
	ignore(msqrt(a,b,r));
	afree(b);
	return(r);
}

PMINT ppow(a,b,c)
PMINT a,b,c;
{
	PMINT d;

	new(&d);
	pow(a,b,c,d);
	return(d);
}

PMINT prpow(a,n)
PMINT a;
int n;
{
	PMINT b;

	new(&b);
	rpow(a,n,b);
	return(b);
}
