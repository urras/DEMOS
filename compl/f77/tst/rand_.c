double rndm_(n)
long *n;
{static flag=0;double f;long time();
 if(flag==0) {srand((int)time(0));flag=1;}
 f=((double)(rand()%32000)/32000.0);
/* printf("%e\t",f);    */
 return(f);
}
