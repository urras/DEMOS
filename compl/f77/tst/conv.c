#include <stdio.h>
main()
{ 
	register ch;
	while((ch=getchar())>=0)
	{ 
		ch &= 0177;
		if(ch != 015)
		{ 
		     /* if(ch>='A' && ch<='Z') ch += 040;
			else */ if(ch>= 0140 && ch<= 0176) ch += ( 0340 - 0140 );
			putchar(ch);
		}
	}
}
