/*
*
*  ���������� � ������ ���� ��������� �� ���� ��������������
*  ���������:
*
*  equel equeldate.q
*  cc    equeldate.c ~rubin/lib/libq.a
*  a.out
*/
# include <stdio.h>
# include "../../source/h/datetime.h"
# include "../../source/h/equel.h"
/* ������ ����, ��� ���� ��������� ������ ������������� ����������
 * � EQUEL: = ������� �� ��� �� ������
 */
##      char    NAME[22] =
"";
extern my_print();

main(ac, av)
int	ac;
##char *av[];
{
##      char    bday[22];
##      char    day[40];
##      char    time[40];
##      char    tel[17];
##      char    class[4];
##      EQ_date sdate, bdate;
##      EQ_time stime;
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
    IIout_err = my_print;
    if ( ac != 2)
##      rubin "-dDD-���-YY" demo
    else
##      rubin "-dDD-���-YY" av[1]
/*      EQ_OFFERR;      */
##      create ��(���=i2, ���=c16, �����=date, �����=c2, ���=c16)
##      define ���_�� is 0
	printf("�������� ��������� ����������� ������\n");
	printf("������ ������ ��������� ����������� � '==='\n");
	IIwrite("replace (i=10)");IIsync();
	EQ_ONERR;
        while (1) {
##      retrieve (sdate = sysdate, stime = systime)
##              {
		printf("%s",date_char(sdate,"'�������' DD ������ YYYY '����', �������"));
		printf("%s\n",time_char(stime,NULL));
##              }
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

## append to ��(���=ser_next("���_��"), ���=NAME, �����=sdate,
## �����=class, ���=tel)

            if (IIerrflag) 
                continue;
            i = 0;
## retrieve ( bday=from_date(��.�����,"DD ������ YYYY '����'"), NAME=��.���,
##      class=��.�����, tel=��.���)
##   where (��.��� = ser_last("���_��"))
## {
	    printf(" `%s' ������� `%s' ����� %s ������� `%s'\n",NAME,bday,class,tel);
##}
            if (IIerrflag) 
                continue;
            printf("��������� (y/n �/�)? ");
            gets(NAME);
            if (NAME[0] == 'n' || NAME[0] == '�')
## delete �� where ��.���=ser_last("���_��")
        }
## print ��
## exit
}

my_print(s)
char *s;
{
printf("==========%s========\n",s);
}
