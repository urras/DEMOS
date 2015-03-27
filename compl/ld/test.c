#include <stdio.h>
int baza[20000];
main(argc, argv)
 int argc; char *argv[];
 { int i=0, j=0;
   printf("Start ov test\n");
   O_start(open(argv[1],0));
   printf("Mallock(100)= %o\n", malloc(100));
   test2(1,argv);
   for(i=0;i<argc;i++)
     j = test1(argv[i]);
   printf("call of test 2: %d\n",j);
   test2(argc,argv);
   printf("The end\n");
}
ttest(i)
int i;
{test1(0);printf("Ttest:%d\n",i);return(i);}
