/*
 * Simple, but general volume copy program
 * Straight copy, no bootstrap or volume-id features.
 *
 */

#include <sys/param.h>

#ifdef STANDALONE
char fromdev[80],todev[80];
char nblocks[80],recsize[80];
char answer[80];
#endif

int fromfd,tofd;
long nblk,rsiz;

main(argc,argv)
int argc;
char **argv;
{
        long numconv();

#ifndef STANDALONE
        argc--;
        if(argc != 4)
                fatal("usage fromdev todev nblocks recsize\n");
        if((fromfd = open(argv[1],0)) < 0)
                fatal("bad from device specification!\n");
        if((tofd = open(argv[2],1)) < 0)
                fatal("bad to device specification!\n");
        if((nblk = numconv(argv[3])) < 0)
                fatal("bad block count specification!\n");
        if((rsiz = numconv(argv[4])) < 0)
                fatal("bad record size specification!\n");
        printf("copying %s to %s, %D blocks at %D blocks per transfer\n",
                argv[1],argv[2],nblk,rsiz);
        copy(fromfd,tofd,nblk,rsiz);
#else
        for(;;) {
                for(;;) {
                        printf("copy from? ");
                        gets(fromdev);
                        if((fromfd = open(fromdev,0)) < 0)
                                printf("bad from device specification!\n");
                        else
                                break;
                }
                for(;;) {
                        printf("copy to?   ");
                        gets(todev);
                        if((tofd = open(todev,1)) < 0)
                                printf("bad to device specification!\n");
                        else
                                break;
                }
                for(;;) {
                        printf("block count? ");
                        gets(nblocks);
                        if((nblk = numconv(nblocks)) < 0)
                                printf("bad block count specification!\n");
                        else
                                break;
                }               
                for(;;) {
                        printf("record size? ");
                        gets(recsize);
                        if((rsiz = numconv(recsize)) < 0)
                                printf("bad record size specification!\n");
                        else
                                break;
                }               
                printf("copy from %s to %s, %D blocks at %D blocks per transfer?",
                        fromdev,todev,nblk,rsiz);
                gets(answer);
                if(eqstr(answer,"yes"))
                        copy(fromfd,tofd,nblk,rsiz);
                close(fromdev);
                close(todev);
                printf("again?");
                gets(answer);
                if(!eqstr(answer,"yes"))
                        break;
        }
#endif
}


#define MAXBLOCKS       40
char buffer[MAXBLOCKS*BSIZE];

copy(from,to,count,chunk)
int from,to;
long count,chunk;
{
        register long m;
        register int n;
        int in,out;

        m = count;
        n = (chunk < MAXBLOCKS) ? chunk : MAXBLOCKS;
        while(m != 0) {
                n = (m < n) ? m : n;
                if((in = read(from,buffer,n*BSIZE)) < 0)
                        fatal("read error: %D\n",(count-m));
                if((out = write(to,buffer,in)) != in)
                        fatal("write error: %D\n",(count-m) + (out/BSIZE));
                m = m - n;
                if((m != 0) && (in != (n*BSIZE)))
                        fatal("copy aborted: %D\n",(count-m));
        }
}
        
long numconv(str)
char *str;
{
        register char *s;
        register long n;

        n = 0;
        for(s=str;*s;s++) {
                if((*s < '0') || (*s > '9'))
                        return((long)-1);
                n = n*10 + (*s - '0');
        }
        return(n);
}

eqstr(str1,str2)
char *str1,*str2;
{
        register char *s1,*s2;

        s1 = str1;
        s2 = str2;
        while(*s1)
                if(*s1++ != *s2++)
                        return(0);
        return(1);
}

fatal(str,a1,a2,a3,a4,a5,a6,a7,a8)
char *str;
{
        printf(str,a1,a2,a3,a4,a5,a6,a7,a8);
        exit(1);
}
