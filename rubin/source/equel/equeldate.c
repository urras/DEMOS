# include <stdio.h>
# include "../../source/h/datetime.h"
# include "../../source/h/equel.h"
char NAME[22]=
"";
extern my_print();

main(ac, av)
int	ac;
char*av[];
{
char bday[22];char day[40];
char time[40];
char tel[17];
char class[4];
EQ_date sdate,bdate;
EQ_time stime;

    long   date0, years; int klass;
    char	lbuf[256];
    register    int    i;
printf("******* ******  ****  *******          ****       *  ****** \n");
printf("   *    *      *    *    *            *    *     **  *      \n");
printf("   *    *      *         *                 *    * *  *****  \n");
printf("   *    ****   *         *                *       *       * \n");
printf("   *    *      *         *              *         *       * \n");
printf("   *    *      *    *    *             *     **   *  *    * \n");
printf("   *    ******  ****     *            ****** **  ***  ****  \n");
/*
*
*  ���������� � ������ ���� ��������� �� ���� ��������������
*  ���������:
*
*  equel equeldate.q
*  cc    equeldate.c ~rubin/lib/libq.a
*  a.out
*/
/* ������ ����, ��� ���� ��������� ������ ������������� ����������
 * � EQUEL: = ������� �� ��� �� ������
 */
    IIout_err = my_print;
    if ( ac != 2)
{IIrubin("-dDD-���-YY","demo",0);}
    else
{IIrubin("-dDD-���-YY",av[1],0);}
/*      EQ_OFFERR;      */
{IIwrite("create ��(���=i2,���=c16,�����=date,�����=c2,���=c16)");IIsync(0);}{IIwrite("define ���_��=0");IIsync(
0);}
	IIwrite("replace (i=10)");IIsync();
	EQ_ONERR;
        while (1) {
{IIwrite("retrieve(sdate=sysdate,stime=systime)");IIsetup();while(IIn_get(0)){IIn_ret(&sdate,6);IIn_ret(&stime,
6);if(IIerrtest())continue;
		printf("%s",date_char(sdate,"'�������' DD ������ YYYY '����', �������"));
		printf("%s\n",time_char(stime,NULL));
}IIflushtup(0);IIsync(0);}
            printf("���?................\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
            gets(NAME);
            if (!NAME[0]) 
                break;
	    do {
		    printf("���� ��������: DD.MM.YYYY\b\b\b\b\b\b\b\b\b\b");
		    gets(bday);
		    sdate = char_date(bday,"DD.MM.YYYY");
	   } while (sdate < 0);
	    /* ��������� ��������������� ����� */
	    /* 1. �������� 1 �������� ���� �������� */
	    date0 = char_date(date_char(sdate,"01.09.YYYY"),"DD.MM.YYYY");
	    /* 2. �������� �������� ������ �������� ���� � ���� �������� */
	    years = atoi(date_char(get_date(),"YYYY"))
		  - atoi(date_char(sdate,"YYYY"));
	    /* 3. � ����� ���� � 7 ���, 1 �������� */
	    klass = years - 7;
	    if (date0 < sdate) klass++;
	    /* ���� ������ ������ ����, �� � ����� ����� �� ��� �� ������� */
	    if (char_date(date_char(get_date(),"01.09.YYYY"),"DD.MM.YYYY")
		> get_date()) klass--;
	    printf("��������� ����� = %d\n",klass);
            printf("�����?? ..\b\b");
            gets(class);
	    printf("�������? OOO-tt-ee\b\b\b\b\b\b\b\b\b");
            gets(tel);

{IIwrite("append ��(���=ser_next(\"���_��\"),���=");IIcvar(NAME,3,0);IIwrite(",�����=");IIcvar(&sdate,6,4);IIwrite("");
IIwrite(",�����=");IIcvar(class,3,0);IIwrite(",���=");IIcvar(tel,3,0);IIwrite(")");IIsync(0);}

            if (IIerrflag) 
                continue;
            i = 0;
{IIwrite("retrieve(bday=from_date(��.�����,\"DD ������ YYYY '����'\"),NAME=��.���,class=��.�����,tel=��.���)where");
IIwrite("(��.���=ser_last(\"���_��\"))");IIsetup();while(IIn_get(0)){IIn_ret(bday,3);IIn_ret(NAME,3);IIn_ret(class
,3);IIn_ret(tel,3);if(IIerrtest())continue;

	    printf(" `%s' ������� `%s' ����� %s ������� `%s'\n",NAME,bday,class,tel);
}IIflushtup(0);IIsync(0);}
            if (IIerrflag) 
                continue;
            printf("��������� (y/n �/�)? ");
            gets(NAME);
            if (NAME[0] == 'n' || NAME[0] == '�')
{IIwrite("delete �� where ��.���=ser_last(\"���_��\")");IIsync(0);}
        }
{IIwrite("print ��");IIsync(0);}
{IIexit();}
}

my_print(s)
char *s;
{
printf("==========%s========\n",s);
}
