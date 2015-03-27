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
*  Трансляция и запуск этой программы на счет осуществляются
*  командами:
*
*  equel equeldate.q
*  cc    equeldate.c ~rubin/lib/libq.a
*  a.out
*/
/* Пример того, как надо правильно писать инициализацию переменных
 * в EQUEL: = пишется на той же строке
 */
    IIout_err = my_print;
    if ( ac != 2)
{IIrubin("-dDD-МЕС-YY","demo",0);}
    else
{IIrubin("-dDD-МЕС-YY",av[1],0);}
/*      EQ_OFFERR;      */
{IIwrite("create уч(ном=i2,фио=c16,деньр=date,класс=c2,тел=c16)");IIsync(0);}{IIwrite("define ном_шк=0");IIsync(
0);}
	IIwrite("replace (i=10)");IIsync();
	EQ_ONERR;
        while (1) {
{IIwrite("retrieve(sdate=sysdate,stime=systime)");IIsetup();while(IIn_get(0)){IIn_ret(&sdate,6);IIn_ret(&stime,
6);if(IIerrtest())continue;
		printf("%s",date_char(sdate,"'Сегодня' DD МЕСЯЦА YYYY 'года', ДЕНЬНЕД"));
		printf("%s\n",time_char(stime,NULL));
}IIflushtup(0);IIsync(0);}
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

{IIwrite("append уч(ном=ser_next(\"ном_шк\"),фио=");IIcvar(NAME,3,0);IIwrite(",деньр=");IIcvar(&sdate,6,4);IIwrite("");
IIwrite(",класс=");IIcvar(class,3,0);IIwrite(",тел=");IIcvar(tel,3,0);IIwrite(")");IIsync(0);}

            if (IIerrflag) 
                continue;
            i = 0;
{IIwrite("retrieve(bday=from_date(уч.деньр,\"DD МЕСЯЦА YYYY 'года'\"),NAME=уч.фио,class=уч.класс,tel=уч.тел)where");
IIwrite("(уч.ном=ser_last(\"ном_шк\"))");IIsetup();while(IIn_get(0)){IIn_ret(bday,3);IIn_ret(NAME,3);IIn_ret(class
,3);IIn_ret(tel,3);if(IIerrtest())continue;

	    printf(" `%s' родился `%s' класс %s телефон `%s'\n",NAME,bday,class,tel);
}IIflushtup(0);IIsync(0);}
            if (IIerrflag) 
                continue;
            printf("Правильно (y/n д/н)? ");
            gets(NAME);
            if (NAME[0] == 'n' || NAME[0] == 'н')
{IIwrite("delete уч where уч.ном=ser_last(\"ном_шк\")");IIsync(0);}
        }
{IIwrite("print уч");IIsync(0);}
{IIexit();}
}

my_print(s)
char *s;
{
printf("==========%s========\n",s);
}
