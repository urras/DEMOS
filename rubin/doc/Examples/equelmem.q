# include <stdio.h>
# include <rubin/equel.h>

## char cbuf[25];
## short oklad;

main(ac,av)
## char **av;
int ac;
{
## short i;
## rubin "-b" av[1]
EQ_OFFERR;
## destroy memtst
EQ_ONERR;
## create memtst(name=c24,okl=i2)
## modify memtst to shash where tuples=1000
setbuf(stdout,NULL);
for(i=0;i<50;i++)
{
## char buf[256];
sprintf(buf,"้อั%d",i+100);
## append to memtst (name=buf,okl=i)
printf("\rAppend %.4d ",i);
}
puts("\n");
for(i=0;i<1000;i++)
{
## replace memtst(okl=memtst.okl+i) where memtst.name="้อั10"
printf("\r Replace %.4d",i);
}
## print memtst
## exit
}
