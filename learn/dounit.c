#include <stdio.h>
#include "lrnref"
#include <signal.h>

dounit()
{
	char tbuff[100];

	if (todo == 0)
		return;
	wrong = 0;
retry:
	start(todo);
	sprintf(tbuff, "../../%s/L%s", sname, todo);	/* script = lesson */
	scrin = fopen(tbuff, "r");
	if (scrin == NULL) {
		if (flag)fprintf( stderr,"Tакого урока еще нет.\n");
		else   fprintf(stderr, "No script.\n");
		wrapup(1);
	}
	copy(0, scrin);
	if (more == 0)
		return;
	copy(1, stdin);
	if (more == 0)
		return;
	copy(0, scrin);

	if (comfile >= 0)
		close(comfile);
	wait(&didok);
	didok = (status == 0);
	if (!didok) {
		wrong++;
		if (flag) printf("\nИзвините , но это%s не верно.Хотите попробовать еще раз?",wrong>1?" все еще " : "");
		else printf("\nSorry, that's %snot right.  Do you want to try again?  ",
		wrong > 1 ? "still " : "");
		fflush(stdout);
		for(;;) {
			gets(tbuff);
			if ((strcmp(tbuff,"yes")==0)||(strcmp(tbuff,"да")==0)){
				if (flag)printf("Попробуйте еще раз. \n");
				else   printf("Try the problem again.\n");
				fflush(stdout);
				goto retry;
			} 
			else if ((strcmp(tbuff, "bye") == 0)||(strcmp(tbuff,"пока") ==0)) {
				wrapup(1);
			} 
			else if ((strcmp(tbuff,"no")==0)||(strcmp(tbuff,"нет")==0)){
				wrong = 0;
				if(flag){
					printf("\nНу,хорошо. Переходим к следующему уроку.\n\n");
					printf("\nВы выполнили урок %s .",todo);
					printf("\nСкорость прохода по курсу %d .\n\n",speed);
				}
				else {    
					printf("\nOK.  Lesson %s (%d)\n", todo, speed);
					printf("Skipping to next lesson.\n\n");  
				}
				fflush(stdout);
				break;
			} 
			else {  
				if (flag) printf("Пожалуйста, ответьте да, нет или пока : ");

				else    printf("Please type yes, no or bye:  ");
				fflush(stdout);
			}
		}
	}
	setdid(todo, sequence++);
}



