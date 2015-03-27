# include <defines.h>
# include <aux.h>
# include <access.h>
# include <code.h>

/*
 * SERIAL.c
 * ��������� ��� ������ � �����������
 * ���������� �������� � ������� � ���� ������-���������
 * ��� ������� � ��������� �������� relations � ����� �������
 * S_SERIAL
 * ������� �������� ���������� �������� � ���� reltups
 *
 * ����� ������ �������� ��� ������, ��������� � �������
 * ��� ��� ������� ��� ����������� ������ � �������� � RELATION
 * � ��� ���� ��������� ��������, ������ � �����������
 *
 * ������� ������  �������������� ����� ��, ��� � ��� ����������
 * - ������� ������ ��������� ������������, � ���� ��� - �� ABD
 */

/*
 * ��� ����������� ���������� ��� �������� -
 * 1. ������ ��������� �������� � ��������� �� 1.
 * 2. ������ ��������, ���������� � ��������� ���
 * 3. ������ ��������� �������� � ��������� ���������� �������� ��������
 * ����� �������:
 * - 32
 * ������� ��������� ��������
 */
struct str_ser {     /* ���� ser_name[0]==0, ������� ��������  */
    char	ser_name[MAXNAME];  /* ��� ���������� */
    char	ser_owner[2];       /* �������� */
    long	ser_value;          /* ��������� ���������� �������� */
    tid_type    ser_tid;        /* tid ���������� � relation */
    short	ser_kod;           /* ��� � �������. 0 - ����� ������ */
} sertab[MAXSERLAST], *es = sertab + (MAXSERLAST -1);

/*
 * ������� ser_find (name)
 * ���������� ��������� �� ������ �������, ���� ����� ����������
 * ���� NULL. ������������ ����.
 */
static struct str_ser *ser_find(ns)
char	*ns;
{
    char	name[MAXNAME+1];
    register struct str_ser *sp, *pp;
    register int	i;
    /*
     * 1. ��������� ���
     */
# ifdef xATR1
        if ( tTf(32, 1))
        printf("str_ser: ns = %s\n", ns);
# endif
# ifdef xATR3
    if ( tTf(32, 2))
        for (sp = sertab; sp <= es; sp++)
            if (sp->ser_name[0]) {
                printf("%d:%.14s %.2s val=%ld tid=%ld kod=%d\n",
                    sp - sertab, sp->ser_name, sp->ser_owner,
		    sp->ser_value, sp->ser_tid.ltid, sp->ser_kod);
            }
# endif
    pmove(ns, name, MAXNAME, ' ');
    for (sp = sertab; sp <= es; sp++)
    {
        if ( bequal(sp->ser_name, name, MAXNAME)) 
            goto fnd;
    }
# ifdef xATR1
    if (tTf(32, 3))
        printf("ser_find: return NULL\n");
# endif
    return(NULL);
fnd:
    i = sp->ser_kod;
    if ( sp->ser_kod > 0)
        for (pp = sertab; pp <= es; pp++)
            if (pp->ser_name[0] && pp->ser_kod < i)
                pp->ser_kod++;
    sp->ser_kod = 0;
# ifdef xATR1
    if (tTf(32, 3))
        printf("ser_find: find %d\n", sp - sertab);
# endif
    return(sp);
}

/*
 * ser_new()
 * ���������� �� �� ��������� �����
 * ���� ������ ��� - �������� ����� ������ ����������
 * ���� ���������������
 */
static struct str_ser *ser_new()
{
    register struct str_ser *sp, *pp;
# ifdef xATR1
    if (tTf(32, 4))
        printf("ser_new()\n");
# endif
    pp = sertab;
    for (sp = sertab; sp <= es; sp++) {
        if ( sp->ser_name[0] == 0) {
            pp = sp;
            break;
        }
        if (pp->ser_kod < sp->ser_kod) 
            pp = sp;
    }
# ifdef xATR1
    if (tTf(32, 4) && sp->ser_name[0])
        printf("Find OLDEST\t");
# endif
    pp->ser_name[0] = 0;
    for (sp = sertab; sp <= es; sp++)
        if (sp->ser_name[0]) 
            sp->ser_kod++;
    pp->ser_kod = 0;
# ifdef xATR1
    if (tTf(32, 4))
        printf("ser_new Returned %d\n", sp - sertab);
# endif
    return(pp);
}


/*
 * ser_del (sp) - ��� ������ �����
 * ��� ������ �����, � ������� ���� �� �� �������������
 */
# define ser_del(sp) { sp->ser_name[0] = 0; }

/*
 * long ser_last(name)
 * - ������ �������� ������� ser_last(���)
 *   ���� -1, ���� ���������� �� ����� �������� ��������
 *   (����������� �� ������� ser_set)
 */
long    ser_last(name)
char    *name;
{
    register struct str_ser *sp;
    char nm[MAXNAME+1];
    lower(name,nm);
    sp = ser_find(nm);
    if (sp == NULL) {
# ifdef xATR1
        if (tTf(32, 5))
            printf("ser_last(%s) = -1\n", nm);
# endif
        return(-1);
    }
# ifdef xATR1
    if (tTf(32, 5))
        printf("ser_last(%s) = %ld\n", nm, sp->ser_value);
# endif
    return(sp->ser_value);
}

/*
 * long ser_set(char *nm,long val)
 * 1. ������ ������� �������� ���������� nm.
 *    ����������� �� �������� � ���� �� 1, ���� val == -1
 *    ����������� ���������� �������� val � ��������� ������
 *    �������� �������� ������������ ���
 *    ������� ser_last(nm)
 *
 *  � � � � � � � �
 *
 * 1. ���� ����� ��� � �������, ��
 *      ������ ������ � ����� ������ � relation
 *      ��������, ��� ��� - ����������, � �� ���������
 *      �������� ����� � ������� ���������
 *      ������� �� ����
 * 2. �������
 *    ������� �� tid
 *    ��������� ��� � ���
 *    ��������� ��������
 *    ��������� �� 1, ���� val >=0, ����� ��������� val
 *    �������� � relation �����
 *    ��� �������
 *    �������� ����� �������� � �������
 *    ������� ��� ��������
 */
long    ser_set(nm,val)
char    *nm;
long val;
{
    struct relation serrel;
    register struct str_ser *sp;
    char name[MAXNAME+1];
    int i;
    lower(nm,name);
# ifdef xATR1
    if (tTf(32, 6))
        printf("ser_next(%s)\n", name);
# endif
    /*
     * 1. ���� �� � �������?
     */
    if ( (sp = ser_find(name)) == NULL) {
        int	i;
        sp = ser_new();
        if ((i = get_rtupl(&serrel, name, &sp->ser_tid)) != 0) {
# ifdef xATR1
            if ( tTf(32, 7))
                printf("ser_next -NOT find = %d\n", i);
# endif
            return(-1);
        }
        if ( !serrel.relstat & S_SERIAL ) {
# ifdef xATR1
            if ( tTf(32, 7))
                printf("ser_next -BAD STATUS= %o\n", serrel.relstat);
# endif
            return(-2);
        }
	bmove( serrel.relid, sp->ser_name, MAXNAME);
	bmove( serrel.relowner, sp->ser_owner, 2);
        sp->ser_value = -1;     /* ���� ��� �� ��������� */
    }
    /* ���������� ���� - ����� ����������� */
    /* ���� ����� ������ ��������� �� noclose */
    Admin.adreld.relopn = (Admin.adreld.relfp + 1) * -5;
    i = 0;
    if (i = get_page(&Admin.adreld, &sp->ser_tid))
        goto SysErr;
    get_tuple(&Admin.adreld, &sp->ser_tid, (char *) & serrel);
    if ( !bequal(serrel.relid, sp->ser_name, MAXNAME) ||
	!bequal(serrel.relowner, sp->ser_owner, 2)) {
# ifdef xATR1
        if ( tTf(32, 7))
	    printf("ser_next - CHANGED NAME OWNER sp=%.14s rel=%.14s\n", sp->ser_name, serrel.relid);
# endif
        ser_del(sp);
        i = resetacc(Acc_head);
        if (i < 0) 
            goto SysErr;
        return(-3);
    }
    sp->ser_value = serrel.reltups;
    if (val < 0)
	    serrel.reltups += 1;
    else
	    serrel.reltups = val;
    put_tuple(&sp->ser_tid, (char *) & serrel, sizeof serrel);
    i = resetacc(Acc_head);
    if (i < 0) 
        goto SysErr;
# ifdef xATR1
    if ( tTf(32, 8))
        printf("ser_next(%s) = %ld\n", name, sp->ser_value);
# endif
    return(sp->ser_value);
SysErr:
    syserr("ser_next: get_page=%d\n", i);
}

/*
 * ��������� ��� �������� ����� � ������ �������
 */
extern int	Dcase;
lower(no, nn)
register char	*no, *nn;
{
    register int	i = MAXNAME;
    int cc;
    while (*no && i--) {
        if (!Dcase) 
            *nn++ = *no;
        else
	{
	cc = (*no++ & 0377);
	*nn++ = to_lower( cc);
	}
    }
    *nn++ = 0;
    return;
}


