# include <defines.h>
# include <datetime.h>
# include "../iutil/form_defs.h"

/*
 * form_out.c
 * - Программы для преобразования даты и времени из
 *   бинарного в текстовый формат
 * Дата в системе хранится в днях с 1 января 0000 года
 * Время хранится в секундах
 * Преобразование производится по шаблону.
 * Шаблон предварительно компилируется
 *    - при этом в 1 байт записывается D_COMPL, а во 2-й - длина
 *      строки после форматирования  ПЛЮС код пробела
 * char *to_char(long d, char *s, struct datebase *db)
 *  - Выводит строку по формату
 */
static char o_buf[MAX_LDATE+1];

char *to_char(d, p, db)
long d;
register char *p;
struct datebase *db;
{
	short l=0;                       /* Длина вывода */
	register short c;                /* Очередной символ шаблона */
	register char *o=o_buf;          /* Указатель вывода */
	char *eo        =o_buf+MAX_LDATE;/* Конец буфера вывода */
	int v;                           /* Очередное выводимое значение */
	register char *f;                /* Формат */
	struct tabform *t;
	/* 1. Если не скопмилирован - скомпилировать */
	if (!p || *p != D_COMPL)
		p = to_compl(p, db->tb);
	if (!p) return("???");
	/* 2. Теперь p - указатель на скомпилированную строку */
	l = p[1]-' ';
	if (l <= 0 || l > MAX_LDATE) return("???");
	/* 3. Преобразуем long в структуру */
	(*(db->fromlong))(d, db->convbuf);
	/* 4. Крутим цикл пр шаблону */
	p += 2; /* Пробросили признак и длину */
	while (c = (*p++ & 0377))
	{
		if ( c > 037)
		{
			*o++ = c;
			if (o > eo) goto Err_OVFLO;
			continue;
		}
		t = &(db->tb[c-1]);
		f = t->da_form;
		if (o + t->da_flen > eo) goto Err_OVFLO;
		v = *(t->da_addr) + t->da_off;
		/* Теперь 2 случая - формат или список */
		if (*f < ' ') /* Список !! */
		{
			register int i;
			/* В именах не бывает кодов меньше 040 */
			while (*f && *f != v) f++;
			if (!*f)
			{
				for(i=t->da_flen; i>0; i--)
					*o++ = '?';
				continue;
			}
			while (  (c= *++f&0377) >= ' ')
				*o++ = c;
			continue;
		}
		/* Формат!! */
		sprintf(o, f, v);
		while (*o && o<eo) o++;
	}
	l -= (o-o_buf);
	while (l-- > 0) *o++ = ' ';
	*o = 0;
	return(o_buf);
Err_OVFLO:
	if (o > eo) o = eo;
	*o++ = '?';
	*o = 0;
	return(o_buf);
}

/*
 * char *time_char(d,p)
 * выводит время по формату p
 */
char *time_char(d,p)
long d;
char *p;
{
   return(to_char(d,do_compl(p,&b_time),&b_time));
}

/*
 * char *date_char(d,p)
 * выводит дату по формату p
 */
char *date_char(d,p)
long d;
char *p;
{
   return(to_char(d,do_compl(p,&b_date),&b_date));
}

