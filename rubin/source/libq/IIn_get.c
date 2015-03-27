# include	<defines.h>
# include	<symbol.h>
# include	"IIglobals.h"
# include	<sccs.h>

SCCSID(@(#)IIn_get.c	7.2	2/16/81)


/*
**  IIn_get -- Routine to check if there is a next
**	tuple coming up the data pipe in a retrieve.
**	Loads IIr_sym with the length and type of
**	the next domain. 
**
**	Returns:
**		0 -- on error or end of tuple
**		1 -- otherwise
**
*/


IIn_get(file_name, line_no)
char		*file_name;
int		line_no;
{
	register struct retsym	*ret;

	if (IIproc_name = file_name)
		IIline_no = line_no;

#	ifdef xATR1
	if (IIdebug)
	{
		printf("%s ent ", IIproc_name ? IIproc_name : "");
		printf("IIn_get : IIdomains %d IIr_sym type, len %d %d IIerrflag %d\n",
		IIdomains, IIr_sym.type, IIr_sym.len, IIerrflag);
	}
#	endif

	while (IIpb.pb_type != PB_REG)
	{
		IIreadinput(&IIpb);
		if (bitset(IIpb.pb_stat, PB_INFO))
			IIpb_prime(&IIpb, PB_NOTYPE);
		else
			return (0);
	}

	IIdomains = 0;
	/* in case of overflow error, a tuple has been skipped */
	if (IIerrflag == 1001)
		IIerrflag = 0;
	if (IIerrflag)
		return (0);
	
	ret = &IIr_sym;
	if (ret->type && ret->type != EOTUP)
		IIsyserr("bad type in IIn_get : %d", ret->type);
	
	/* read next type, length pair */
	if (IIpb_get(&IIpb, ret, 2) != 2)
		IIsyserr("IIrdpipe error in IIn_get");

	if (ret->type == EXIT)
		return (0);

	return (1);
}

/*
**  IIerrtest -- Test if an error occurred within a retrieve.
**
**	Returns:
**		0 -- if not
**		error that occurred if not
**
**	Called By:
**		USER -- used to skip over C_CODE in a retrieve {}
**			when an error ocurred in getting the tuple.
**			A call to this routine is generated by equel,
**			because IIerrflag should not need to be vi-
**			sible from inside a users program.
**
*/

IIerrtest()
{
	return (IIerrflag);
}
