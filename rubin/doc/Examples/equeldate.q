/*
*
*  Трансляция и запуск этой программы на счет осуществляются
*  командами:
*
*  equel equeldate.q
*  cc    equeldate.c ~rubin/lib/libq.a
*  a.out
*/
# include <stdio.h>
# include "../../source/h/datetime.h"
# include "../../source/h/equel.h"
/* Пример того, как надо правильно писать инициализацию переменных
 * в EQUEL: = пишется на той же строке
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
##      rubin "-dDD-МЕС-YY" demo
    else
##      rubin "-dDD-МЕС-YY" av[1]
/*      EQ_OFFERR;      */
##      create уч(ном=i2, фио=c16, деньр=date, класс=c2, тел=c16)
##      define ном_шк is 0
	printf("Проверка перехвата диагностики ошибок\n");
	printf("Сейчас должна появиться диагностика в '==='\n");
	IIwrite("replace (i=10)");IIsync();
	EQ_ONERR;
        while (1) {
##      retrieve (sdate = sysdate, stime = systime)
##              {
		printf("%s",date_char(sdate,"'Сегодня' DD МЕСЯЦА YYYY 'года', ДЕНЬНЕД"));
		printf("%s\n",time_char(stime,NULL));
##              }
            printf("Имя?................\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
            gets(NAME);
            if (!NAME[0]) 
                break;
	    do {
		    printf("Дата рождения: DD.MM.YYYY\b\b\b\b\b\b\b\b\b\b");
		    gets(bday);
		    sdate = char_date(bday,"DD.MM.YYYY");
	   } while (sdate < 0);
	    /* Попробуем спрогнозировать класс */
	    /* 1. Получили 1 сентября года рождения */
	    date0 = char_date(date_char(sdate,"01.09.YYYY"),"DD.MM.YYYY");
	    /* 2. Получили разность номера текущего года и года рождения */
	    years = atoi(date_char(get_date(),"YYYY"))
		  - atoi(date_char(sdate,"YYYY"));
	    /* 3. В школу идут в 7 лет, 1 сентября */
	    klass = years - 7;
	    if (date0 < sdate) klass++;
	    /* Если сейчас начало года, то в новый класс он еще не перешел */
	    if (char_date(date_char(get_date(),"01.09.YYYY"),"DD.MM.YYYY")
		> get_date()) klass--;
	    printf("Вероятный класс = %d\n",klass);
            printf("Класс?? ..\b\b");
            gets(class);
	    printf("Телефон? OOO-tt-ee\b\b\b\b\b\b\b\b\b");
            gets(tel);

## append to уч(ном=ser_next("ном_шк"), фио=NAME, деньр=sdate,
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

my_print(s)
char *s;
{
printf("==========%s========\n",s);
}
