# include	<sccs.h>
# include       <code.h>

SCCSID(@(#)scompare.c	7.1	2/5/81)

/*
**  STRING COMPARE
**
**	The strings 'a_ptr' and 'b_ptr' are compared.  Blanks are
**	ignored.  The first string may be no longer than 'a_len'
**	bytes, and the second string may be no longer than 'b_len'
**	bytes.  If either length is zero, it is taken to be very
**	long.  A null byte also terminates the scan.
**
**	Compares are based on the ascii ordering.
**
**	Shorter strings are less than longer strings.
**
**	Return value is positive one for a > b, minus one for a < b,
**	and zero for a == b.
**
**	Examples:
**		"abc" > "ab"
**		"  a bc  " == "ab  c"
**		"abc" < "abd"
*/
#define U(x) ((x)&0377)

scompare(a_ptr, a_len, b_ptr, b_len)
char	*a_ptr;
int	a_len;
char	*b_ptr;
int	b_len;
{
	char		*ap;
	char		*bp;
	register char	a;
	char		b;
	register int	al;
	register int	bl;

	ap = a_ptr;
	bp = b_ptr;
	al = a_len;
	if (al == 0)
		al = 32767;
	bl = b_len;
	if (bl == 0)
		bl = 32767;

	while (1)
	{

		/* supress blanks in both strings */
		while ((a = *ap) == ' ' && al > 0)
		{
			al--;
			ap++;
		}
		if (al == 0)
			a = 0;
		while (*bp == ' ' && bl > 0)
		{
			bl--;
			bp++;
		}
		if (bl == 0)
			b = 0;
		else
			b = *bp;
		if (a != b) {
#ifdef KOI8
			a = cvtU(a);
			b = cvtU(b);
#endif
			return( U(a) > U(b) ? 1 : -1 );
		}
		if (a == 0)
			return (0);

		/* go on to the next character */
		ap++;
		al--;
		bp++;
		bl--;
	}
}

#ifdef KOI8
/*
** ������������� �� ���-8 � U-��� ��� ��������� ��������.
*/

static char UTAB[] = {
/*      �       �       �       �       �       �       �       �       */
	0376,   0340,   0341,   0366,   0344,   0345,   0364,   0343,
/*      �       �       �       �       �       �       �       �       */
	0365,   0350,   0351,   0352,   0353,   0354,   0355,   0356,
/*      �       �       �       �       �       �       �       �       */
	0357,   0377,   0360,   0361,   0362,   0363,   0346,   0342,
/*      �       �       �       �       �       �       �       �       */
	0374,   0373,   0347,   0370,   0375,   0371,   0367,   0372,
/*      �       �       �       �       �       �       �       �       */
	0336,   0300,   0301,   0326,   0304,   0305,   0324,   0303,
/*      �       �       �       �       �       �       �       �       */
	0325,   0310,   0311,   0312,   0313,   0314,   0315,   0316,
/*      �       �       �       �       �       �       �       �       */
	0317,   0337,   0320,   0321,   0322,   0323,   0306,   0302,
/*      �       �       �       �       �       �       �       0       */
	0334,   0333,   0307,   0330,   0335,   0331,   0327,   0
};

cvtU(x)
char x;
{
	if ( x == 0177 ) return( 0377); /* DEL - max char! */
	return(((x&0300)!=0300)? U(x) : UTAB[x & 077] );
}
#endif
