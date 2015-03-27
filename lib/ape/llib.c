	/* LINTLIBRARY */

Ignore(a)
int a;
{
	a=a;
}

Ignors(s)
char *s;
{
	s=s;
}

Forget(s)
char *s;
{
	Ignors(s);
}
#include <stdio.h>

/* VARARGS */
FILE *popenp(a,b,c)
char *a, *b, *c;
{
	a=a;b=b;if (c==NULL) printf("have a nice day!\n");
	return(stdin);
}
