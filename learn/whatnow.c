#include <stdio.h>
#include "lrnref"
#include <signal.h>
/*   */
whatnow()
{

	if (todo == 0) {
		more=0;
		return;
	}
	if (didok) {
		strcpy(level,todo);
		if (speed<=9) speed++;
	}
	else {
		speed -= 4;
		if (speed <0) speed=0;
	}
	if (wrong) {
		speed -= 2;
		if (speed <0 ) speed = 0;
	}
	if (didok && more) {
		if (flag){ printf("\nВы выполнили урок %s .",level);
			   printf("\nСкорость прохода по курсу %d .\n\n",speed);}
		else     printf("\nGood.  Lesson %s (%d)\n\n",level, speed);
		fflush(stdout);
	}
system("clear");
}







