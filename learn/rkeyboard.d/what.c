#include <stdio.h>
#include <sgtty.h>
char c;
main (argc, argv)
int argc;
char **argv;
{
	struct sgttyb strterm;
	struct tchars strctl;
	char fdo;
	char fdi;
	char *s;
	fdi=fileno(stdin);
	ioctl(fdi,TIOCGETP, &strterm);
	ioctl(fdi,TIOCGETC, &strctl);
	while (--argc >0  &&  (*++argv)[0]=='-')
		for (s = argv[0] + 1; *s != '\0' ; s++)
			switch (*s) {
			case 'e':
				c = strterm.sg_erase;
				output();
				break;
			case 'k':
				c = strterm.sg_kill;
				output();
				break;
			case 'i' :
				c = strctl.t_intrc;
				output();
				break;
			case 'q' :
				c = strctl.t_quitc;
				output();
				break;
			case 's' :
				c = strctl.t_startc;
				output();
				break;
			case 'p' :
				c = strctl.t_stopc;
				output();
				break;
			case 'f' :
				c = strctl.t_eofc;
				output();
				break;
			}  
	fflush(stdout);
}
output()
{
	if (c >= 0 && c < 040 ) {
		c |= 0100 ; 
		printf(" <ctrl>  и клавишу %c \n", c); 
	}
	else{
		if (c == 0177)   printf("<del>\n");
		else    printf("%c \n", c);
	}       
}
