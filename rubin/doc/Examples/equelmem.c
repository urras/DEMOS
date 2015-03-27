# include <stdio.h>
# include <rubin/equel.h>

char cbuf[25];short oklad;


main(ac,av)
char**av;
int ac;
{
short i;{IIrubin("-b",av[1],0);}

EQ_OFFERR;
{IIwrite("destroy memtst");IIsync(0);}
EQ_ONERR;
{IIwrite("create memtst(name=c24,okl=i2)");IIsync(0);}{IIwrite("modify memtst to shash where tuples=1000");IIsync(
0);}
setbuf(stdout,NULL);
for(i=0;i<50;i++)
{
char buf[256];
sprintf(buf,"้อั%d",i+100);
{IIwrite("append memtst(name=");IIcvar(buf,3,0);IIwrite(",okl=");IIcvar(&i,1,2);IIwrite(")");IIsync(0);}
printf("\rAppend %.4d ",i);
}
puts("\n");
for(i=0;i<1000;i++)
{
{IIwrite("replace memtst(okl=memtst.okl+");IIcvar(&i,1,2);IIwrite(")where memtst.name=\"้อั10\"");IIsync(0);}
printf("\r Replace %.4d",i);
}
{IIwrite("print memtst");IIsync(0);}
{IIexit();}
}
