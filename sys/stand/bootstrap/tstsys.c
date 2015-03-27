/* Программы для имитации stand-alone system */

#include <stdio.h>

#define putw(w,f) (putc(w&0377,f),putc((w>>8)&0377,f))
int sep=1, segflag,cputype=40, ksep=0, sep_id=0, bootopts=0, checkword = 0;
int video=0;
FILE *tomem;
unsigned addrto;
long baseto;
setseg(click)
{
	long l;
	if(tomem== NULL && (tomem = fopen("mem","w+")) == NULL)
	{ 
		perror(":mem:"); 
		exit(8);
	}
	baseto = (long)click*64;
	addrto = 0;
	fseek(tomem, baseto,0);
	fprintf(stderr,"setseg:%o\n",click*64);
}

mtpi(w,a)
unsigned a;
{
	if(a != addrto) { 
		addrto = a; 
		fseek(tomem,baseto+addrto,0); 
	}
	putw(w,tomem); 
	addrto += 2;
}

unsigned mfpi(a)
unsigned a;
{unsigned i;
		addrto = a; 
		fseek(tomem,baseto+addrto,0); 
	addrto += 2;
	return(i=getc(tomem),i|(getc(tomem)<<8));
}

clrseg(a,siz)
unsigned a,siz;
{  unsigned siz0=siz;
	while(siz-- && siz--)
	{ 
		mtpi(0,a); 
		a += 2; 
	}
	fprintf(stderr,"clrseg:%o l=%o\n", a, siz0);
}

getw(id)
int id;
{       
	int i;
	if(read(id,&i,2) != 2) perror(" getw ");
	return(i);
}
/*
long lseek(),tell();
static char buf[512]; 
static int lun= -1,offset;
static long seek;
getw(id)
{       
	int i;
	if( id != lun ) {
		lseek(lun,seek+offset,0);
		lun=id; 
		seek = tell(id);
		offset= -1;
	}
	if(offset<0 || offset>512)
	{
		offset=read(id,buf,512); 
		if(offset<=0)
			perror("getw:");
		seek=seek-512+offset; 
		offset = 512 - offset;
	}
	i = (buf[offset++]&0377) | buf[offset++]<<8;
	return(i);
}
*/

#undef getc
getc(id)
int id;
{       
	int i;char c;
	if(read(id,&c,1) != 1) perror(" getc ");
	return(i=(c&0377));
}
