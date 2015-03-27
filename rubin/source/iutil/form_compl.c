# include <defines.h>
# include <datetime.h>
# include "../iutil/form_defs.h"
#ifndef EQUEL
#include  <trace.h>
#else
# undef xATR1
#endif

/*
 * form_compl.c
 * - Программы для преобразования даты и времени из
 *   бинарного в текстовый формат
 * Дата в системе хранится в днях с 1 января 0000 года
 * Время хранится в секундах
 * Преобразование производится по шаблону.
 * Шаблон предварительно компилируется
 *
 * Основные подпрограммы
 * char *to_compl(char *s, struct tabform *tb)
 *  - Преобразует строку шаблона s в скомпилированный формат
 *    - то есть шаблоны заменяются кодом с номером шаблона в таблице
 *      увеличенным на 1
 *    - при этом в 1 байт записывается D_COMPL, а во 2-й - длина
 *      строки после форматирования  ПЛЮС код пробела
 * long from_char(char *in, char *s, struct datebase *db)
 *  - вводит строку по формату. -1 - ошибочная строка
 *
 *  int len_char(char *s)
 *  - запрос длины строки после форматирования по шаблону s
 *
 * В шаблоне допускаются символы пунктуации с кодами меньшими 0100
 *  не брать в кавычки ''. Остальные символы, не являющиеся символами
 *  шаблона, должны быть заключены в кавычки ''.
 */
# define QUOTA '\''
static char c_buf[MAX_LDATE+1];
char *to_compl(p0,tbf)
register char *p0;      /* Указатель на первый символ подстроки */
struct tabform *tbf;
{
	register char *p; /* Указатель на очередной символ */
	register char *n; /* Указатель на текущий символ имени */
	struct tabform *t;/* Указатель на текущую строку таблицы */
	char *o;          /* Указатель на строку результата */
	char *eo;         /* Конец места под результат      */
	short l=0;        /* Длина сформатированного - длина шаблона */
	short err=0;      /* Признак ошибки */
	short c;          /* Текущий символ */
#ifdef xATR1
	if (tTf(32,11) )
		printf("to_compl('%s',%lo)\n",p0,tbf);
#endif
	/* 1. Установим указатели */
	o = c_buf;
	eo = o + MAX_LDATE;
	*o++ = D_COMPL;
	*o++ = ' ';
	while(c=(*p0&0377))
	{
		/* 2. Литерал? */
		if(c == QUOTA)
		{
			while((c= *++p0) && c != QUOTA)
			{
c_write:
				*o++ = c;
				if (o > eo) goto Err_OVFLO;
			}
			if (c)
			{
				p0++;
				if (*p0 == QUOTA) goto c_write;
			}
			continue;
		}
		/* 3. Пунктуация */
		if ( c >= ' ' && c <= '@')
		{
			*o++ = c;
			p0++;
			if (o > eo) goto Err_OVFLO;
			continue;
		}
		/* 4. Иначе поиск в таблице */
		for(t=tbf; (n=t->da_name);t++)
		{
			p = p0;
			while( *n && *p == *n)
				p++, n++;
			if (*n == 0) break; /* Нашли */
		}
		/* 5. Если не нашли, ошибка */
		if (!n) goto Err_NOFORM;
		/* 6. Пишем код формата+1, и увеличиваем длину */
#ifdef TEST_FORM
		*o++ = (t-tbf)+'0';
#else
		*o++ = (t-tbf) + 1;
#endif
		l += t->da_flen -1;
		p0 = p;
		if (o > eo) goto Err_OVFLO;
	}
	*o = 0;
	l += (o-c_buf-2);
	c_buf[1] += l;
#ifdef xATR1
	if (tTf(32,11) )
		printf("to_compl return OK %d %d\n",c_buf[0],c_buf[1]);
#endif
	return(c_buf);
Err_OVFLO:
Err_NOFORM:
#ifdef xATR1
	if (tTf(32,11) )
		printf("to_compl return BAD\n");
#endif
	return(NULL);
}

int len_char(s)
char *s;
{
	if (s[0] == D_COMPL) return(s[1]-' ');
	else return(-1);
}
