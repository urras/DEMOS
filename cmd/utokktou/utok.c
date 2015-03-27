/* ko = 4 - was not kirill letters, 2 - not u-kod file, 0 - O key */
#include <stdio.h>
#include <sys/code.h>
#ifdef KOI8
char *tabuk="áâ÷çäåöúéêëìíîïðòóôõæèãþûýßùøüàñÁÂ×ÇÄÅÖÚÉÊËÌÍÎÏÐÒÓÔÕÆÈÃÞÛÝßÙØÜÀÑ"
;
#else
char *tabuk="ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþß"
;
#endif
main()
{ register int i, kod=4;
  while((i=getchar())>=0)
	{if (i>=0300 && i<=0377)
	 {i = tabuk[i-0300];if(kod==4)kod=0;}
	 else if (i==0 || (i>0177 && i<0277)) { kod=2; }
	  putchar(i);
	}
if (kod == 2 ) fprintf(stderr,"this not text file !!!\n");
exit(kod);
}
