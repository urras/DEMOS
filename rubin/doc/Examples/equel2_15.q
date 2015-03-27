#include <rubin/datetime.h>
#include <rubin/equel.h>
/*
* Тест для иллюстрации новых возможностей СУБД РУБИН 2.15
* Используются функции для работы с датой и временем
* и функции для работы с переменными
* Программа вводит данные в небольшое отношение уч-еники
* Это же отношение независимо используется в тесте Test2_15
*
*  Трансляция и запуск этой программы на счет осуществляются
*  командами:
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
##      create уч(ном=i2, фио=c16, деньр=date, класс=c2, тел=c16)
##      define ном_шк is 0
	IIprint_err = IIret_err;
	while (1) {
##      retrieve (day=from_date(sysdate,"'Сегодня' DD МЕСЯЦА YYYY 'года', ДЕНЬНЕД"),
##                time=ascii(systime))
##              {
			printf ("%s, %s\n",day,time);
##              }
	    printf("Имя?................\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
	    gets(NAME);
	    if (!NAME[0])
		break;
	    printf("Дата рождения: DD.MM.YYYY\b\b\b\b\b\b\b\b\b\b");
	    gets(bday);
	    printf("Класс?? ..\b\b");
	    gets(class);
	    printf("Телефон? OOO-tt-ee\b\b\b\b\b\b\b\b\b");
	    gets(tel);

## append to уч(ном=ser_next("ном_шк"), фио=NAME, деньр=to_date(bday,"DD.MM.YYYY"),
## класс=class, тел=tel)

	    if (IIerrflag)
		continue;
	    i = 0;
## retrieve ( bday=from_date(уч.деньр,"DD МЕСЯЦА YYYY 'года'"), NAME=уч.фио,
##      class=уч.класс, tel=уч.тел)
##   where (уч.ном = ser_last("ном_шк"))
## {
	    printf(" `%s' родился `%s' класс %s телефон `%s'\n",NAME,bday,class,tel);
##}
	    if (IIerrflag)
		continue;
	    printf("Правильно (y/n д/н)? ");
	    gets(NAME);
	    if (NAME[0] == 'n' || NAME[0] == 'н')
## delete уч where уч.ном=ser_last("ном_шк")
	}
## print уч
## exit
}


was_error(i)
{
printf("\n\n Try again\n\n");
exit(1);
}
