#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include "fio.h"
#define DIRSIZE	14
g_char(a,alen,b) char *a,*b; ftnlen alen;
{       register char *x=a+alen-1,*y=b+alen-1;
	*(y+1)=0;
	for(;x>=a && *x==' ';x--) *y--=0;
	for(;x>=a;*y--= *x--);
}
b_char(a,b,blen) char *a,*b; ftnlen blen;
{       register int i;
	for(i=0;i<blen && *a!=0;i++) *b++= *a++;
	for(;i<blen;i++) *b++=' ';
}
inode(a) char *a;
{	struct stat x;
	if(stat(a,&x)<0) return(-1);
	return(x.st_ino);
}
