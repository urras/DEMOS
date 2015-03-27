#include <stdio.h>

/* Программа, улучшающая y.tab.c после работы yacc7
   Если указано 2 параметра, то также действия разбиваются на
   необходимые для 1 и 2 проходов : P1B ... P1E - только 1 проход
				    PaB ... PaE - для обоих проходов
   В файл 2 прохода выдается только переключатель действий
   Предварительно следует обозначить начало и конец
   переключателя действий в y.tab.c псевдокомментариями
  Begin и End (с 1 позиции)
  Программа пытается об'единить одинаковые строки в списке действий
  параметр - имя исходного файла, результат поступает в stdout
  Автор сей затычки - Руднев, 16.11.84, ИАЭ */
#define NCASE 400  /* Макс> число операторов case*/
#define LLINEM 120
#define NLINES 30
#define KBEG "/*Begin*/"
#define OBEG "/*Obegin*/"
#define KEND "/*End*/"
#define KCASE "case "
#define KLINE "# line "
#define MCASE 500       /* Макс. номер действия в case */

/* Определения для символов начала и конца групп подстановки в 1 проход */
#define PaB(s) (s[0]=='P' && s[1]=='a' && s[2]=='B')
#define PaE(s) (s[0]=='P' && s[1]=='a' && s[2]=='E')
#define P1B(s) (s[0]=='P' && s[1]=='1' && s[2]=='B')
#define P1E(s) (s[0]=='P' && s[1]=='1' && s[2]=='E')

int ncase=0,flgopt=0;
struct casedef {
	char flagp,inpass1; 
	long begc; 
	int nlines,skc;
} 
tabc[NCASE];
long fseek(),ftell();
#define PERR1(x,y) {fprintf(stderr,x,y);putc('\n',stderr); exit(1);}
#define PERR(x) {fprintf(stderr,x);putc('\n',stderr); exit(1);}

char bufdo[NLINES][LLINEM], bufs[LLINEM], bufs0[LLINEM];
FILE *filein,*filep2;
int fonep = 1;
main (ac,av)
int ac; 
char *av[];
{   long stend; char *kbeg;
	if(*av[1]=='-' && *(av[1]+1) =='O') {av++; ac--; flgopt=1;}
	if(!(filein = freopen(av[1],"r",stdin))) PERR1("Нет файла %s\n",av[1]);
	if(ac<3) filep2 = stdout; 
	else
	{ 
		if(!(filep2=fopen(av[2],"w"))) PERR1("Не могу открыть %s\n",av[2]);
		fonep = 0;
	}
	kbeg = (flgopt?OBEG:KBEG);
	while(!eqstr(gets(bufs),kbeg)&& !feof(stdin)) puts(bufs);
	if(!feof(stdin))
	    {
		puts(bufs);
		doanl();
		stend = ftell(stdin);
		docomp();
		fseek(stdin,stend,0);
		puts(KEND);
		while(gets(bufs), !feof(stdin)) puts(bufs);
		if(flgopt) puttab();
	} 
	else PERR1("Не найдена метка начала %s",kbeg);
exit(0);
}

eqstr(s1,s2)
register char *s1,*s2;
{ 
	while(*s2) {
		if(*s1++ != *s2++) return(0);
	}
	return(1);
}

eqstr0(s1,s2)
register char *s1,*s2;
{ 
	while(*s2) {
		if(*s1++ != *s2++) return(0);  
	}
	return(*s1?0:1);
}

ksum(s)
register char *s;
{
	register int ks=0;
	while(*s) ks += *s++;
	return(ks);
}

doanl()
{
	register char *ss;
	int nl,kc;
	long slast;
	register struct casedef *pt = tabc;
	slast = ftell(stdin);
	gets(bufs);
	if( ! eqstr(bufs,KCASE) && (puts(bufs),slast = ftell(stdin), !eqstr(gets(bufs),KCASE)))
				 PERR("После метки начала нет карты case\n");
	do
	    { 
		pt->begc = slast; 
		pt->inpass1 = 0;
		nl = 0; 
		kc = 0;
		do
		    { 
nexts: 
			slast = ftell(stdin); 
			gets(bufs);
			if(feof(stdin)) PERR("Неожиданный конец файла до карты /*End*/\n");
			if(eqstr(bufs,KLINE)) goto nexts;
			if(eqstr(bufs,KCASE) || eqstr(bufs,KEND)) break;
			nl++;
			kc += ksum(bufs);
			if (!fonep && !pt->inpass1 ) {
				ss = bufs;
				do
				    if(PaB(ss) || P1B(ss)) {
					pt->inpass1 = 1;
					break;  
				}
				while (*ss++ && *(ss+2));
			}       
		}
		while(1);
		pt->skc = kc; 
		pt->nlines = nl; 
		pt->flagp = 1;
		pt++;
		if((pt-tabc)>NCASE) PERR("Слишком большой switch\n");
		if(eqstr(bufs,KEND)) break;
	}
	while (1);
	ncase = pt - tabc;
}

docomp()
{
	register struct casedef *pcur;
	struct casedef  *pchk;
	register int i;
	register char *ss; 
	int fp1, fp2,nc;
	struct casedef *pend;
	pend = tabc + ncase;
	for(pcur=tabc; pcur != pend; pcur++)
		if(pcur->flagp) {
			fseek(stdin,pcur->begc,0);
			gets(bufs); if(flgopt) nc=optcase(bufs,0); /* bufs меняется*/
			fputs(bufs, filep2);
			if( pcur->inpass1)
				puts(bufs); /* case xx: */
			for(i=0; i<pcur->nlines; i++)
				while(eqstr(gets(bufdo[i]),KLINE))fputs(bufdo[i],filep2);
			for(pchk = pcur+1; pchk != pend; pchk++)
				if(pchk->skc == pcur->skc && pchk->nlines == pcur->nlines)
				{
					fseek(stdin, pchk->begc,0);
					gets(bufs0);if(flgopt)optcase(bufs0,nc);
					for(i=0;  i< pchk->nlines; i++)
					{
						while(eqstr(gets(bufs),KLINE));
						if( !eqstr0(bufs,bufdo[i])) goto nextchk;
					}
					pchk->flagp = 0;
					fputs(bufs0,filep2);
					if(  pcur->inpass1)puts(bufs0);
nextchk: 
					;
				}
			if (pcur->inpass1) puts("{"); 
			fp2 = 1; 
			fp1 = 0;
			for(i=0; i<pcur->nlines; i++)
				if(!pcur->inpass1)fputs(bufdo[i],filep2);
				else
				{ 
					ss = bufdo[i];
					while(*ss)
					{
						if(PaB(ss)) { 
							ss += 3; 
							fp1=1; 
							fp2=1;
							continue;
						}
						if(P1B(ss)) { 
							ss += 3; 
							fp1=1; 
							fp2=0;
							continue;
						}
						if(PaE(ss)|| P1E(ss)) { 
							ss += 3; 
							fp1=0; 
							fp2=1;
							continue;
						}
						if(fp1) putchar(*ss);
						if(fp2) putc(*ss,filep2);
						ss++;
					}
					putc(012,filep2);
				}
					if(pcur->inpass1) puts("}break;\n");
		}
}

fputs(s,f)
register FILE *f; 
register char *s;
{ 
	while(*s) putc(*s++,f);
	putc('\012',f);
}

/* Сжатие переключателя для двухпроходного парсера "-O"
 * заметим, что нужен другой парсер
*/
int tcase[MCASE], ntc=0, maxc=0;
optcase(buf,nc)
char *buf; 
int nc;
{
	char cases[LLINEM];
	int n= -1, i;
	if(sscanf(buf,"%s %d",cases,&n) != 2 || !eqstr0("case", cases) || n<=0)
		PERR1("Плохой case: %s", buf);
	if(n>MCASE-1) PERR1("Слишком большой номер в %s",buf);
	if(n>maxc) maxc = n;
	i = tcase[n] = (nc?nc:++ntc);
	if(!nc) sprintf(buf, "case %d: /* %d */", i,n);
	else sprintf(buf, "/* case %d */",n);
	return(i);
}

puttab()
{
	register int i,j;
	printf("/* Таблица кодов действий */\n");
	printf("int ytcase[%d] = { \n",maxc+2);
	j=maxc+1;
	for(i=0;i<j;i++)
		printf("%d, %c",tcase[i], i%5?'\t':'\n');
	printf(" 0};\n");
}
