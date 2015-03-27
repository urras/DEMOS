# include	<defines.h>
# include	<aux.h>
# include	<tree.h>
# include	<symbol.h>
# include	"../decomp/globs.h"
# include	<sccs.h>
# include       <datetime.h>

/*
static char Sccsid[] = "$Header: typecheck.c,v 1.3 89/03/09 17:57:52 rubin Exp $";
 */
/*
** $Log:	typecheck.c,v $
 * Revision 1.3  89/03/09  17:57:52  rubin
 * Выравнивание для i1
 * 
 * Revision 1.2  87/11/25  17:15:15  rubin
 * Вставлена обработка типа i_1, которая в отличие от VAX не сводится
 * к i_2: теперь в дереве преобразуем i_1 в i_2, 
 * а при записи обратно
 * 
** TYPECHECK
**
**	Performs typechecking and conversions where appropriate .
**	Prohibits mixed type expressions.
*/

FAR typecheck(pp1, pp2, opval)
SYMBOL	*pp1, *pp2;
int	opval;
{
	register int	i;
	register SYMBOL	*p1, *p2;
	int t1, t2;
	p1 = pp1;
	p2 = pp2;

	i = (p1->type == CHAR & p2->type == CHAR);	/* c is true only if both are chars */
	switch (opval)
	{
	  case opTOTI:
	  case opTODA:
	  case opCONCAT:
		if (!i)
			ov_err(NUMERIC);	/* numeric in a char operator */
		return;	/* else no further checking is needed */
	  case opDACHAR:
	  case opTICHAR:
		if (p2->type != CHAR) ov_err(NUMERIC);
		if (p1->type == CHAR) ov_err(BADCHAR);
		typecoerce(p1,INT,4);
		return;
	/* ser_set("varname",INT/DATE/TIME) */
	  case opINIT:
		if (p1->type != CHAR) ov_err(NUMERIC);
		if (p2->type != INT && p2->type != DATE && p2->type != TIME)
			ov_err(BADMIX);
		typecoerce(p2,p2->type,4);
		return;
	  case opADD:
	  case opSUB:
	  case opMUL:
	  case opDIV:
	  case opPOW:
	  case opMOD:
		if (i)
			ov_err(BADCHAR);	/* arithmetic operation on two character fields */
		break;
	 default:
		if (i) return;
	}
	t1 = p1->type;
	t2 = p2->type;
	if (t1 == CHAR || t2 == CHAR)
		ov_err(BADMIX);
	/* Теперь остались только численные типы данных */
	if (t1 != t2)
	{
#       define ROK {pp2 = p1; p1 = p2; p2 = pp2; }
		if (t2 == TIME) ROK;
		if (p2->type == DATE) ROK;
		if (p2->type == FLOAT) ROK;
	    /* Теперь старший тип в p1 */
		t1 = p1->type;
		if (t1 == FLOAT)
		{
			itof(p2);
			if (p1->len == 4) f8tof4(p2);
			return;
		}
	   /* Теперь остался только случай (DATE|TIME) -OP- INT */
		p2->type = t1;
	}
	/* А теперь типы совпадают, а длины еще нет */
	if (p1->len == p2->len)
		return;
	if (p2->len > p1->len) ROK;
	/* Типы совпадают и =INT|FLOAT. Меньшая длина в p2 */
	if (t1 != FLOAT)
	{
	/* Приводим i1 к i2 */
		if (p1->len == 1)
		{
			p1->len = 2;
			*(i2type *)&p1->value = *(i1type *)&p1->value;
		}
		if (p2->len == 1)
		{
			p2->len = 2;
			*(i2type *)&p2->value = *(i1type *)&p2->value;
		}
		/* Осталась только комбинация i4 - i2 */
		if (p1->len == 4)
			i2toi4(p2);
		return;
	  }
	   else /* FLOAT: */
	  {
		/* Если f4 OP f8, свести f8 к f4 */
		f8tof4(p1);
		return;
	  }
}
/*
**	Coerce the top of stack symbol to the
**	specified type and length. If the current
**	value is a character then it must be converted
**	to numeric. A user error will occure is the
**	char is not syntaxtically correct.
*/

FAR typecoerce(tosx, ntype, nlen)
SYMBOL	*tosx;
int	ntype;
int	nlen;
{
	register SYMBOL	*tos;
	register char	*cp;
	register int	*val;
	int		ret;
	char		temp[256];

	tos = tosx;
	if (tos->type == ntype && tos->len == nlen) return;
	if (tos->type == CHAR)
	{
		val = (int *) &tos->value.sym_data;
		cp = temp;
		bmove(tos->value.sym_data.cptype, cp, tos->len & I1MASK);
		cp[tos->len & I1MASK] = '\0';
		ret = 0;
		switch (ntype) {
		case FLOAT:
			ret = Atof(cp, val);
			break;
		case INT:
			ret = atol(cp, val);
			if (nlen == 4)
				i4toi2(tos);
			break;
		case DATE:
			if ((tos->value.sym_data.i4type =
			     char_date( cp,NULL) ) < 0)
				ret = -1;
			break;
		case TIME:
			if ((tos->value.sym_data.i4type =
			     char_time( cp, NULL) ) < 0)
				ret = -1;
			break;
		}
		if (ret < 0)
			ov_err(CHARCONVERT);
		tos->type = ntype;
		tos->len = nlen;
	}
	else
		rcvt(tos, ntype, nlen);
}


FAR i2toi4(pp)
SYMBOL *pp;
{
	register SYMBOL	*p;

#	ifdef xOTR3
	if (tTf(87, 0))
	{
		printf("i2toi4: ");
		prsym(pp);
	}
#	endif
	p = pp;

	*(i4type *)&p->value = *(i2type *)&p->value;
	p->len = 4;
#	ifdef xOTR3
	if (tTf(87, 0))
	{
		printf("i2toi4 rets: ");
		prsym(p);
	}
#	endif
}


FAR i4toi2(pp)
SYMBOL *pp;
{
	register SYMBOL	*p;

#	ifdef xOTR3
	if (tTf(87, 1))
	{
		printf("i4toi2: ");
		prsym(pp);
	}
#	endif
	p = pp;

	*(i2type *)&p->value = *(i4type *)&p->value;
	p->len = 2;
#	ifdef xOTR3
	if (tTf(87, 1))
	{
		printf("i4toi2 rets: ");
		prsym(p);
	}
#	endif
}


FAR itof(p)
register SYMBOL *p;
{

#	ifdef xOTR3
	if (tTf(87, 2))
	{
		printf("itof: ");
		prsym(p);
	}
#	endif

	if  (p->len == 4)
		p->value.sym_data.f8type = p->value.sym_data.i4type;
	else
		p->value.sym_data.f8type = p->value.sym_data.i2type;
	p->type = FLOAT;
	p->len= 8;
#	ifdef xOTR3
	if (tTf(87, 2))
	{
		printf("itof rets: ");
		prsym(p);
	}
#	endif
}


FAR ftoi2(p)
register SYMBOL *p;
{
#	ifdef xOTR3
	if (tTf(87, 3))
	{
		printf("ftoi: ");
		prsym(p);
	}
#	endif

	p->value.sym_data.i2type = p->value.sym_data.f8type;
	p->type = INT;
	p->len = 2;
#	ifdef xOTR3
	if (tTf(87, 3))
	{
		printf("ftoi rets: ");
		prsym(p);
	}
#	endif
}


FAR ftoi4(p)
register SYMBOL *p;
{
#	ifdef xOTR3
	if (tTf(87, 4))
	{
		printf("ftoi4: ");
		prsym(p);
	}
#	endif

	p->value.sym_data.i4type = p->value.sym_data.f8type;
	p->type = INT;
	p->len = 4;
#	ifdef xOTR3
	if (tTf(87, 4))
	{
		printf("ftoi4 rets: ");
		prsym(p);
	}
#	endif
}


FAR f8tof4(pp)
SYMBOL	*pp;
{
	pp->len = 4;
	pp->value.sym_data.f4type = pp->value.sym_data.f8type;
}

FAR f4tof8(pp)
SYMBOL	*pp;
{
	pp->len = 8;
	pp->value.sym_data.f8type = pp->value.sym_data.f4type;
}

