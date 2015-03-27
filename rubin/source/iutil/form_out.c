# include <defines.h>
# include <datetime.h>
# include "../iutil/form_defs.h"

/*
 * form_out.c
 * - ��������� ��� �������������� ���� � ������� ��
 *   ��������� � ��������� ������
 * ���� � ������� �������� � ���� � 1 ������ 0000 ����
 * ����� �������� � ��������
 * �������������� ������������ �� �������.
 * ������ �������������� �������������
 *    - ��� ���� � 1 ���� ������������ D_COMPL, � �� 2-� - �����
 *      ������ ����� ��������������  ���� ��� �������
 * char *to_char(long d, char *s, struct datebase *db)
 *  - ������� ������ �� �������
 */
static char o_buf[MAX_LDATE+1];

char *to_char(d, p, db)
long d;
register char *p;
struct datebase *db;
{
	short l=0;                       /* ����� ������ */
	register short c;                /* ��������� ������ ������� */
	register char *o=o_buf;          /* ��������� ������ */
	char *eo        =o_buf+MAX_LDATE;/* ����� ������ ������ */
	int v;                           /* ��������� ��������� �������� */
	register char *f;                /* ������ */
	struct tabform *t;
	/* 1. ���� �� ������������� - �������������� */
	if (!p || *p != D_COMPL)
		p = to_compl(p, db->tb);
	if (!p) return("???");
	/* 2. ������ p - ��������� �� ���������������� ������ */
	l = p[1]-' ';
	if (l <= 0 || l > MAX_LDATE) return("???");
	/* 3. ����������� long � ��������� */
	(*(db->fromlong))(d, db->convbuf);
	/* 4. ������ ���� �� ������� */
	p += 2; /* ���������� ������� � ����� */
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
		/* ������ 2 ������ - ������ ��� ������ */
		if (*f < ' ') /* ������ !! */
		{
			register int i;
			/* � ������ �� ������ ����� ������ 040 */
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
		/* ������!! */
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
 * ������� ����� �� ������� p
 */
char *time_char(d,p)
long d;
char *p;
{
   return(to_char(d,do_compl(p,&b_time),&b_time));
}

/*
 * char *date_char(d,p)
 * ������� ���� �� ������� p
 */
char *date_char(d,p)
long d;
char *p;
{
   return(to_char(d,do_compl(p,&b_date),&b_date));
}

