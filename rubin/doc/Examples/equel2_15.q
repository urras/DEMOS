#include <rubin/datetime.h>
#include <rubin/equel.h>
/*
* ���� ��� ����������� ����� ������������ ���� ����� 2.15
* ������������ ������� ��� ������ � ����� � ��������
* � ������� ��� ������ � �����������
* ��������� ������ ������ � ��������� ��������� ��-�����
* ��� �� ��������� ���������� ������������ � ����� Test2_15
*
*  ���������� � ������ ���� ��������� �� ���� ��������������
*  ���������:
*
*  equel equel2_15.q
*  cc    equel2_15.c ~rubin/lib/libq.a
*  a.out
*/

main(ac, av)
int	ac;
##char *av[];
{
##      char    NAME[22];
##      char    bday[22];
##      char    day[40];
##      char    time[40];
##      char    tel[17];
##      char    class[4];
    extern int	IIerrflag;
    extern IIret_err(), IIno_err();
    extern int	(*IIprint_err)();
    char	lbuf[256];
    register    int    i;
    extern int was_error();
printf("******* ******  ****  *******          ****       *  ****** \n");
printf("   *    *      *    *    *            *    *     **  *      \n");
printf("   *    *      *         *                 *    * *  *****  \n");
printf("   *    ****   *         *                *       *       * \n");
printf("   *    *      *         *              *         *       * \n");
printf("   *    *      *    *    *             *     **   *  *    * \n");
printf("   *    ******  ****     *            ****** **  ***  ****  \n");
    IIabort =   was_error;
    if ( ac != 1)
##      rubin av[1]
    else
##      {
##      char buf[256];
	printf("Enter name of base:");
	gets(buf);
##      rubin buf
##      }
	IIprint_err = IIno_err;
##      create ��(���=i2, ���=c16, �����=date, �����=c2, ���=c16)
##      define ���_�� is 0
	IIprint_err = IIret_err;
	while (1) {
##      retrieve (day=from_date(sysdate,"'�������' DD ������ YYYY '����', �������"),
##                time=ascii(systime))
##              {
			printf ("%s, %s\n",day,time);
##              }
	    printf("���?................\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
	    gets(NAME);
	    if (!NAME[0])
		break;
	    printf("���� ��������: DD.MM.YYYY\b\b\b\b\b\b\b\b\b\b");
	    gets(bday);
	    printf("�����?? ..\b\b");
	    gets(class);
	    printf("�������? OOO-tt-ee\b\b\b\b\b\b\b\b\b");
	    gets(tel);

## append to ��(���=ser_next("���_��"), ���=NAME, �����=to_date(bday,"DD.MM.YYYY"),
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


was_error(i)
{
printf("\n\n Try again\n\n");
exit(1);
}
