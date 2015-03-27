#include "form_defs.h"
#include "../h/datetime.h"

#ifdef TEST_DATE
#include <stdio.h>
main()
{
	static struct cd_date cd0, cd1;
	t_date d0, d1;
	char buf[128];
	setbuf(stdout,NULL);
	while(1)
	{
a:
		printf("������ ����: ���� �� ��?");
		gets(buf);
		if(!buf[0]) break;
		if(sscanf(buf,"%d %d %d",&cd0.cd_year, &cd0.cd_month, &cd0.cd_day) != 3)
			goto a;
b:
		printf("������ ����: ���� �� ��?");
		gets(buf);
		if(sscanf(buf,"%d %d %d",&cd1.cd_year, &cd1.cd_month, &cd1.cd_day) != 3)
			goto b;
		d0 = cd_date(&cd0);
		d1 = cd_date(&cd1);
		date_cd(d0, &cd0);
		date_cd(d1, &cd1);
		printf("����1=%ld ����2=%ld ��������=%ld\n", d0, d1, d1-d0);
		printf("%d.%d.%d ���� ������ %d;%d.%d.%d ���� ������ %d\n",
		    cd0.cd_day, cd0.cd_month, cd0.cd_year, cd0.cd_week,
		    cd1.cd_day, cd1.cd_month, cd1.cd_year, cd1.cd_week);
	}
	exit(0);
}
#endif


#ifdef TEST_OUT
#include <stdio.h>
main()
{
	static struct cd_date cd0;
	static struct cd_time cd1;
	t_date d0, d1;
	extern long cd_date(), cd_time();
	char buf[128],form[128],*p, torm[128],*t;
	setbuf(stdout,NULL);
	while(1)
	{


a:
		printf("����: ���� �� ��?");
		gets(buf);
		if(!buf[0]) break;
		if(sscanf(buf,"%d %d %d",&cd0.cd_year, &cd0.cd_month, &cd0.cd_day) != 3)
			goto a;
b:
		printf("����� ����: hh mi ss?");
		gets(buf);
		if(sscanf(buf,"%d %d %d",&cd1.cd_hh, &cd1.cd_mm, &cd1.cd_ss) != 3)
			goto b;
		d0 = cd_date(&cd0);
		d1 = cd_time(&cd1);
		printf("������ ������ ����? ");
		gets(form);
		if (!form[0]) p = NULL;
		else p = form;
		printf("������ ������ �������? ");
		gets(torm);
		if (!torm[0]) t = NULL;
		else t = torm;
		printf("����=%ld �����=%ld\n", d0, d1);
		printf("����1=`%s'\n",date_char(d0,p));
		printf("�����=`%s'\n",time_char(d1,t));
	}
	exit(0);
}
#endif


#ifdef TEST_FORM
#include <stdio.h>
main()
{
	char buf[128],*p;
	setbuf(stdout,NULL);
	while(1)
	{
a:
		printf("������ ������ ����?");
		gets((p=buf));
		if(!buf[0]) p = NULL;
		printf("������� ������:`%s'\n",do_compl(p,&b_date));
		printf("������ ������ �������?");
		gets((p=buf));
		if(!buf[0]) p = NULL;
		printf("������� ������:`%s'\n",do_compl(p,&b_time));
	}
	exit(0);
}
#endif

#ifdef TEST_IN
#include <stdio.h>
main()
{
	static struct cd_date cd0;
	static struct cd_time cd1;
	t_date d0, d1;
	extern long cd_date(), cd_time();
	char buf[128],form[128],*p, torm[128],*t;
	setbuf(stdout,NULL);
	while(1)
	{


		printf("������ ����� ����? ");
		gets(form);
		if (!form[0]) p = NULL;
		else p = form;
		printf("����: ");
		gets(buf);
		if(!buf[0]) break;
		d0 = char_date(buf,p);
		printf("D0= %ld %s\n",d0,date_char(d0,NULL) );
		printf("������ ����� �������?");
		gets(form);
		if (!form[0]) p = NULL;
		else p = form;
		printf("�����: ");
		gets(buf);
		if(!buf[0]) break;
		d1 = char_time(buf,p);
		printf("T0= %ld %s\n",d0,time_char(d1,NULL) );

	}
	exit(0);
}
#endif

