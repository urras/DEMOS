# include       <defines.h>
# include	<aux.h>
# include	<symbol.h>
# include	<access.h>
# include	<lock.h>
# include	<sccs.h>

SCCSID(@(#)rhash.c	7.2	5/31/83)

/*
**  RHASH -- perform a randomizing hash on the full key.
**
**	Trace Flags:
**		26.12-13
*/

long
rhash(d, key)
register DESC	*d;
char		key[MAXTUP];
{
	register int	i;
	register char	*cp;
	long		bucket;
	char		tmp;
	int             j, knt, f_type;
	int rfill;
	long ww;
	bucket = 0;
	knt = 0;
	rfill = d->reldum.relfill;
	for (i = 1; i <= d->reldum.relatts; i++)
		if (d->relxtra[i])
		{
			/* form pointer to field */
			cp = &key[d->reloff[i]];
			f_type = d->relfrmt[i];
			if ( !rfill || (f_type != INT && f_type != DATE && f_type != TIME) ) {
			    for (j = 0; j < (d->relfrml[i] & I1MASK); j++)
				    if (((ww = *cp++) != ' ') || f_type != CHAR)
					    addhash(ww&0377, &bucket, knt++);
			} else {
			    j = (d->relfrml[i] & I1MASK);
			    if (j == 1) ww = ((union anytype *)(cp))->i1type;
			    if (j == 2) ww = ((union anytype *)(cp))->i2type;
			    if (j == 4) ww = ((union anytype *)(cp))->i4type;
			    addhash(ww,&bucket,knt);
			    knt += j;
			}
		}
	/* remove sign bit from bucket the hard way */
	bucket &= 07777777777;
#	ifdef xATR3
	if (tTf(19, 12))
		printf("rhash:hval=%ld", bucket);
#	endif
	if (rfill ) bucket /= rfill;    /* j - число подряд идущих записей в листе */
	bucket %= d->reldum.relprim;
#	ifdef xATR3
	if (tTf(19, 12))
		printf(",returning %ld\n", bucket);
#	endif
	return (bucket);
}
/*
** ADDHASH is used to map a long key into a four byte integer.
** As bytes are added, they are first rotated, then exclusive ored
** into the existing key.
*/

addhash(w, word, knt1)
long    w;
long	*word;
int	knt1;
{
	register int	knt;
	long		i;

	knt = knt1;
	i = w;  /*get rid of any sign extension*/
	knt += 8 * (knt & 3);	/*alternately add 0, 8, 16 or 24 to knt */
	knt &= 037;
	*word ^= (i << (knt) | i >> (32 - knt));
}
