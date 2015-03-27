/*
**      Автоматическое монтирование файловых систем.
**
**      automou -[nf] имя_устройства ...
**
**      Стандартное местоположение в системе:  /etc
**
**      Метод сборки:  cc -O -n -s
**
**      $Header$
**      $Log$
**
*/

static  char Rcs_id[] = "$Header$";

#include <stdio.h>

#define esac ;break

char dev[]={"/dev/___"};
char der[]={"/dev/r___"};
char fsc[]={"/etc/fsck -p"};
char fbb[]={"/etc/fbb  -s"};
char mnt[]={"/etc/mount  "};
char umo[]={"/etc/umount "};
char nul[]={">/dev/null 2>/dev/null"};

char cmd[128],mark[64];

int f_b=0,f_f=1,f_s=0;

main(argc,argv)
int argc;
char *argv[];
{
        char c;
        int i,j,k,n_f=0,fd,st,n;

        for(i=1;i<argc;++i){
                if(*argv[i]!='-') argv[n_f++]=argv[i];
                else{ 
                        k=i;
                        for(j=1;(c= *(argv[k]+j))!=0;++j){
                                switch(c){
                                case 'B':
                                case 'b':
                                        f_b=1 esac;
                                case 'N':
                                case 'n':
                                        f_f=0 esac;
                                case 'S':
                                case 's':
                                        f_s=1 esac;
                                default:
fatal:                                  
                                        printf("---Automou:  usage: automou [-nf] special_files ... \n");
                                        exit(1);
                                }
                        }
                }
        }

        if(n_f<1) goto fatal;

        if(f_s){
                if(!d_o()) {
                printf("can not open /dev/kmem\n");
                exit (1);
                }
                if((d_r(0177570)&0000100)==0) exit(1);
        }



        for(n=0;n<n_f;++n) {

                for(j=0;(c= *(argv[n]+j))!=0;++j) ;
                for(i=5,j=j-3;i<8;++i,++j) { 
                        *(dev+i)= *(argv[n]+j);
                        *(der+i+1)= *(argv[n]+j); 
                }

                if((fd=open(dev,0))<1) {
                        printf("---Automou: %s can't open\n",dev);
                        goto stop; 
                }

                if(read(fd,mark,64)!=64) {
                        printf("---Automou: can't read from %s\n",dev);
                        goto stop; 
                }

                if(mark[32]!='/') {
                        printf("---Automou: %s not marked\n",dev);
                        goto stop; 
                }

                cat(umo,dev,"");

                if(f_s){ 
                        if((d_r(0177570)&0000010)!=0) if(cat(fsc,der,"")!=0) goto stop;
                        if((d_r(0177570)&0000001)!=0) if(cat(fbb,der,"")!=0) goto stop;
                }
                else{
                        if(f_f) if(cat(fsc,der,"")!=0) goto stop;
                        if(f_b) if(cat(fbb,der,"")!=0) goto stop;
                }

                if(cat(mnt,dev,&mark[32])!=0) {
                        printf("---Automou: can't mount\n");
                        goto stop; 
                }                       

stop:  
                ;
        }

        exit(0);
}


cat(s1,s2,s3)
char *s1,*s2,*s3;
{
        char c;
        int i,j,st;
        for(i=0,j=0;(c= *(s1+j))!=0;++i,++j) *(cmd+i)=c;  
        *(cmd+i++)=' ';
        for(j=0;(c= *(s2+j))!=0;++i,++j) *(cmd+i)=c;
        *(cmd+i++)=' ';
        for(j=0;(c= *(s3+j))!=0;++i,++j) *(cmd+i)=c;
        *(cmd+i++)=' ';
        if(*(s1+5)!='u') { 
                *(cmd+i)=0; 
                printf("%s\n",cmd); 
                *(cmd+i)=' '; 
        }                    
        *(cmd+i)=' ';
        if(*(s1+5)=='m'||*(s1+5)=='u') for(j=0;(c= *(nul+j))!=0;++i,++j) *(cmd+i)=c;         
        *(cmd+i)=0;
        st=system(cmd);
        return(st);
}



int fmem;

d_r(addr)
unsigned addr;
{
        int data;
        lseek(fmem, (long)addr,0);
        read(fmem, &data,(sizeof data));
        return(data);
}

d_o()
{
fmem = open("/dev/kmem",0);
return(fmem >= 0);
}
