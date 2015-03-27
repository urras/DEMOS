# include <defines.h>
# include <aux.h>
# include <access.h>
# include <code.h>

/*
 * SERIAL.c
 * Программы для работы с переменными
 * Переменные хранятся в системе в виде псевдо-отношений
 * Они описаны в системном каталоге relations и имеют признак
 * S_SERIAL
 * Текущее значение переменной хранится в поле reltups
 *
 * Такой способ хранения был выбран, поскольку в системе
 * все уже сделано для разделяемой работы с записями в RELATION
 * и уже есть программы создания, поиска и уничтожения
 *
 * Правила поиска  принадлежности такие же, как и для отногшений
 * - сначала ищется отношение пользователя, а если нет - то ABD
 */

/*
 * Над переменными определены две операции -
 * 1. выдать очередное значение и увеличить на 1.
 * 2. выдать значение, полученное в последний раз
 * 3. выдать очередное значение и присвоить переменной заданную величину
 * Флаги отладки:
 * - 32
 * Таблица последних значений
 */
struct str_ser {     /* если ser_name[0]==0, позиция свободна  */
    char	ser_name[MAXNAME];  /* Имя переменной */
    char	ser_owner[2];       /* Владелец */
    long	ser_value;          /* Последнее полученное значение */
    tid_type    ser_tid;        /* tid переменной в relation */
    short	ser_kod;           /* Вес в таблице. 0 - самая свежая */
} sertab[MAXSERLAST], *es = sertab + (MAXSERLAST -1);

/*
 * Функция ser_find (name)
 * возвращает указатель на строку таблицы, если нашли переменную
 * либо NULL. Переставляет веса.
 */
static struct str_ser *ser_find(ns)
char	*ns;
{
    char	name[MAXNAME+1];
    register struct str_ser *sp, *pp;
    register int	i;
    /*
     * 1. Дополняем имя
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
 * Возвращает ук на свободное место
 * если такого нет - вычищает самую старую переменную
 * Веса пересчитываются
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
 * ser_del (sp) - это просто макро
 * это бывает редко, и поэтому веса мы не пересчитываем
 */
# define ser_del(sp) { sp->ser_name[0] = 0; }

/*
 * long ser_last(name)
 * - выдает значение функции ser_last(имя)
 *   либо -1, если переменная не имеет текущего значения
 *   (полученного по функции ser_set)
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
 * 1. Выдает текущее значение переменной nm.
 *    увеличивает ее значение в базе на 1, если val == -1
 *    присваивает переменной значение val в противном случае
 *    Выданное значение запоминается для
 *    функции ser_last(nm)
 *
 *  А л г о р и т м
 *
 * 1. Если имени нет в таблице, то
 *      найдем строку с таким именем в relation
 *      проверим, что это - переменная, а не отношение
 *      запросим место в таблице последних
 *      запишем ее туда
 * 2. Открыть
 *    считать по tid
 *    проверить имя и тип
 *    запомнить значение
 *    увеличить на 1, если val >=0, иначе присвоить val
 *    записать в relation назад
 *    все закрыть
 *    записать новое значение в таблицу
 *    вернуть это значение
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
     * 1. Есть ли в таблице?
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
        sp->ser_value = -1;     /* Пока еще не увеличили */
    }
    /* Переменная есть - будем увеличивать */
    /* Этот кусок просто переписан из noclose */
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
 * Программа для перевода имени в нижний регистр
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


