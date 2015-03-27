# include	<defines.h>
# include	<sccs.h>
# include       <access.h>
# include       <code.h>

SCCSID(@(#)rubinname.c	7.1	2/5/81)

/*
**  MAKE RUBIN FILE NAME
**
**	The null-terminated string 'iname' is converted to a
**	file name as used by the rubin relations.  The name
**	of the relation is padded out to be MAXNAME bytes long,
**	and the two-character id 'id' is appended.  The whole
**	thing will be null-terminated and put into 'outname'.
**
**	'Outname' must be at least MAXNAME + 3 bytes long.
*/

rubinname(iname, id, outname)
char	*iname;
char	*id;
char	*outname;
{
	register char	*p;
	extern char	*pmove();

	p = outname;
	p = pmove(iname, p, MAXNAME, ' ');
	bmove(id, p, 2);
	p[2] = NULL;
}

# ifdef NOCYRFILES
/*
 * filename(s)
 *  возвращает имя файла, соответствующего имени s
 */
char *file_name(s)
char *s;
{
	register char *p;
	static char nmbuf[MAXNAME+3];
	if (Admin.adhdr.adflags & A_CYRCNV)
	{
		p = nmbuf;
		while (*s && p< nmbuf+MAXNAME+2)
		{
			*p = to7bits(*s);
			p++;  s++;
		}
		*p = 0;
		return(nmbuf);
	}
	return(s);
}
# endif
