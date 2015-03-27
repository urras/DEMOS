#include <stdio.h>
main()
{ int c;
 c=getchar();
 while (c!= EOF){
	if (c==' ' || c=='\t')
	       putchar('\n');
	else putchar(c);
  c=getchar();
  }
}
