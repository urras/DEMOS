/*
 * Копирование информации с устройства на устройство.
 * $Header: vcopy.c,v 1.4 88/11/02 21:49:20 dvolodin Exp $
 * $Log:	vcopy.c,v $
 * Revision 1.4  88/11/02  21:49:20  dvolodin
 * *** empty log message ***
 * 
 * Revision 1.3  86/11/25  02:43:12  alex
 * Размер блока = 512.
 * 
 * Revision 1.2  86/11/24  23:31:26  alex
 * rcsid
 * 
 * Revision 1.1  86/11/24  23:00:44  alex
 * Initial revision
 * 
 *
 */

#define BSIZE 512

#ifdef STANDALONE
char fromdev[80],todev[80];
char nblocks[80],recsize[80];
char answer[80];
#endif

char rcsid[]="$Header: vcopy.c,v 1.4 88/11/02 21:49:20 dvolodin Exp $";
int fromfd,tofd;
long nblk,rsiz;
char module[]="vcopy";

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
	printf("Vcopy - volume copy %s\n",rcsid+7);
	for(;;) {
		for(;;) {
			printf("copy from (читать с)?");
			gets(fromdev);
			if((fromfd = open(fromdev,0)) < 0)
				printf("bad from device specification!\n");
			else
				break;
		}
		for(;;) {
			printf("copy to (копировать на)?   ");
			gets(todev);
			if((tofd = open(todev,1)) < 0)
				printf("bad to device specification!\n");
			else
				break;
		}
		for(;;) {
			printf("block count (число блоков)? ");
			gets(nblocks);
			if((nblk = numconv(nblocks)) < 0)
				printf("bad block count specification!\n");
			else
				break;
		}		
		for(;;) {
			printf("record size (размер блока при переписи)? ");
			gets(recsize);
			if((rsiz = numconv(recsize)) < 0)
				printf("bad record size specification!\n");
			else
				break;
		}		
		printf("copy from %s to %s, %D blocks at %D blocks per transfer?",
			fromdev,todev,nblk,rsiz);
		printf("копирование с %s на %s, %D блоков по %D блоков за обмен?",
			fromdev,todev,nblk,rsiz);
		gets(answer);
		if(eqstr(answer,"yes"))
			copy(fromfd,tofd,nblk,rsiz);
		close(fromdev);
		close(todev);
		printf("again (повторить)?");
		gets(answer);
		if(!eqstr(answer,"yes"))
			break;
	}
#endif
}


#define MAXBLOCKS	40
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
