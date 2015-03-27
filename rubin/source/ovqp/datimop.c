# include <defines.h>
# include <datetime.h>
# include <aux.h>
# include <symbol.h>
# include <tree.h>
# include "../decomp/globs.h"

/*
 *
 * Файл datimop.c содержит подпрограмму datimop
 * которая выполняет операции opDACHAR, opTICHAR,
 * opTODA, opTOTI
 *
 * Трассировочные флаги:
 * 82.5-82.8
 *
 */

/*
 * datimop(op, tos, v1, v2)
 * Выполняет операцию op над v1 и v2 (типы должны быть правильными)
 * и помещает результат в tos
 */
datimop( op, tos, v1, v2)
int	op;
SYMBOL *tos;
register ANYTYPE *v1, *v2;
{
    char	*s;
    register char	*p;
    extern char *need();
    int	i;
    long	l;
    p = v2->cptype;
    switch (op) {
    case opDACHAR:
# ifdef xOTR3
	if (tTf(82,5))
		printf("Date-char(%d,'%s')\n",v1->i4type,p);
# endif
	i = date_len( p );
	s = date_char(v1->i4type, p);
	goto w_char;
    case opTICHAR:
# ifdef xOTR3
	if (tTf(82,5))
		printf("TIME-char(%d,'%s')\n",v1->i4type,p);
# endif
	i = time_len(p);
	s = time_char(v1->i4type, p);
w_char:
# ifdef xOTR3
	if (tTf(82,6) )
		printf("Date/Time-char = '%s' len=%d\n",s,i);
# endif

	p = tos->value.sym_data.cptype = need(De.ov_ovqpbuf, i);
	bmove(s, p, i);
	tos->type = CHAR;
	tos->len  = i;
	return;
    case opTOTI:
# ifdef xOTR3
	if (tTf(82,7)  )
		printf("CHAR_time(%s,'%s')\n",v1->cptype,p);
# endif
	l = char_time(v1->cptype, p);
	tos->type = TIME;
	goto w_long;
    case opTODA:
# ifdef xOTR3
	if (tTf(82,7)   )
		printf("CHAR_date(%s,'%s')\n",v1->cptype,p);
# endif
	l = char_date(v1->cptype, p);
	tos->type = DATE;
w_long:
# ifdef xOTR3
	if (tTf(82,8)    )
		printf("CHAR_time/date(%s)=%ld\n",v1->cptype,l);
# endif
	if (l < 0)
	    ov_err(CHARCONVERT);
	tos->len = 4;
	tos->value.sym_data.i4type = l;
	return;
    }
}
