#include <stdio.h>
char data2[9000];
test2(ac,av) int ac; char *av[];
{ 
       fprintf(stdout,"Test 2: argc=%d argv[ac-1]=%s\n",ac,av[ac-1]);
}
