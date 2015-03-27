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
 * - ��������� ��� �������������� ���� � ������� ��
 *   ��������� � ��������� ������
 * ���� � ������� �������� � ���� � 1 ������ 0000 ����
 * ����� �������� � ��������
 * �������������� ������������ �� �������.
 * ������ �������������� �������������
 *
 * �������� ������������
 * char *to_compl(char *s, struct tabform *tb)
 *  - ����������� ������ ������� s � ���������������� ������
 *    - �� ���� ������� ���������� ����� � ������� ������� � �������
 *      ����������� �� 1
 *    - ��� ���� � 1 ���� ������������ D_COMPL, � �� 2-� - �����
 *      ������ ����� ��������������  ���� ��� �������
 * long from_char(char *in, char *s, struct datebase *db)
 *  - ������ ������ �� �������. -1 - ��������� ������
 *
 *  int len_char(char *s)
 *  - ������ ����� ������ ����� �������������� �� ������� s
 *
 * � ������� ����������� ������� ���������� � ������ �������� 0100
 *  �� ����� � ������� ''. ��������� �������, �� ���������� ���������
 *  �������, ������ ���� ��������� � ������� ''.
 */
# define QUOTA '\''
static char c_buf[MAX_LDATE+1];
char *to_compl(p0,tbf)
register char *p0;      /* ��������� �� ������ ������ ��������� */
struct tabform *tbf;
{
	register char *p; /* ��������� �� ��������� ������ */
	register char *n; /* ��������� �� ������� ������ ����� */
	struct tabform *t;/* ��������� �� ������� ������ ������� */
	char *o;          /* ��������� �� ������ ���������� */
	char *eo;         /* ����� ����� ��� ���������      */
	short l=0;        /* ����� ����������������� - ����� ������� */
	short err=0;      /* ������� ������ */
	short c;          /* ������� ������ */
#ifdef xATR1
	if (tTf(32,11) )
		printf("to_compl('%s',%lo)\n",p0,tbf);
#endif
	/* 1. ��������� ��������� */
	o = c_buf;
	eo = o + MAX_LDATE;
	*o++ = D_COMPL;
	*o++ = ' ';
	while(c=(*p0&0377))
	{
		/* 2. �������? */
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
		/* 3. ���������� */
		if ( c >= ' ' && c <= '@')
		{
			*o++ = c;
			p0++;
			if (o > eo) goto Err_OVFLO;
			continue;
		}
		/* 4. ����� ����� � ������� */
		for(t=tbf; (n=t->da_name);t++)
		{
			p = p0;
			while( *n && *p == *n)
				p++, n++;
			if (*n == 0) break; /* ����� */
		}
		/* 5. ���� �� �����, ������ */
		if (!n) goto Err_NOFORM;
		/* 6. ����� ��� �������+1, � ����������� ����� */
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
