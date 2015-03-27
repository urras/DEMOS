/* Write mark on device    Serg A.Usikow   84/02/15  */

#include <stdio.h>

int boot[16]={05,0112700,024,0105737,0177564,0100375,0112037,0177566,01372,0,067125,067542,072157,061141,062554,020072};
char mark[32];
char dev[]={"/dev/___"};

main(argc,argv)
int argc;
char *argv[];
{
        int c,i,j,fd;
        if(argc!=3) err("usage:  mark special_file directory_for_mount\n","");

        for(j=0;(c= *(argv[1]+j))!=0;++j) ;
        for(i=5,j=j-3;i<8;++i,++j) *(dev+i)= *(argv[1]+j);

        if((fd=open(dev,1))<1) err("can't open %s\n",dev);
        if(write(fd,boot,32)!=32) err("can't write boot to %s\n",dev);
        for(i=0;((c= *(argv[2]+i))!=0)&&i<32;++i) mark[i]=c;
        mark[i]=0;
        if(i==32) err("label too math : %s\n",argv[2]);
        if(write(fd,mark,32)!=32) err("can't write mark to %s\n",dev);
        system("/bin/sync");
        exit(0);
}

err(s1,s2)
char *s1,*s2;
{
        fprintf(stderr,"---Mark:  ");
        fprintf(stderr,s1,s2);
        exit(1);
}
