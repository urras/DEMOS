#include <stdio.h>
char data1[7000];
test1(s) char *s;
{ 
        if (!s) {printf("Test1(ttest(Test1))"); return(10);}
        printf("Test 1: arg=%s\n",s); 
        printf("Ttest return %d\n",ttest(12));
        return(20);
}
