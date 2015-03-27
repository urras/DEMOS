#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include "fio.h"
#include <ediag.h>
#define STR(x) (x==NULL?"":x)

/* @AR -  ИАЭ
 * 25.10.85 - проведено устранение ошибок, связанных с тем,
 * что не проверялось право чтения или записи при обменах
 * Функции nowreading и nowwriting теперь:
 *      1) возвращают код ответа,
 *      2) если признак чтения/записи уже есть,
 *         не делают ничего;
 * Соответственно проправлены обращения к ним, а также введены
 * макро: SETREAD(x), SETWRITE(x)
 */
/*global definitions*/
unit units[MXUNIT];	/*unit table*/
flag init;	/*0 on entry, 1 after initializations*/
cilist *elist;	/*active external io list*/
flag reading;	/*1 if reading, 0 if writing*/
flag cplus,cblank;
char *fmtbuf;
flag external;	/*1 if external io, 0 if internal */
int (*doed)(),(*doned)();
int (*doend)(),(*donewrec)(),(*dorevert)();
flag sequential;	/*1 if sequential io, 0 if direct*/
flag formatted;	/*1 if formatted io, 0 if unformatted*/
int (*getn)(),(*putn)();	/*for formatted io*/
FILE *cf;	/*current file*/
unit *curunit;	/*current unit*/
int recpos;	/*place in current record*/
int cursor,scale;

ftnint nocore_; /* Признак - при ошибке не писать core */

/*error messages*/
extern char *f_errlist[];
extern char *f_rerrlist[];
extern int f_nerr;
#define MAXERR (f_nerr + 100)
fatal(n,s) char *s;
{
	if(n<100 && n>=0) perror(s); /*SYSDEP*/
	else if(n>=(int)MAXERR)
	{       fprintf(stderr,ediag("%s: illegal error number %d\n","%s: неизвестная ошибка %d\n"),s,n);
	}
	else if(n<0) fprintf(stderr,ediag("%s: end of file %d\n","%s: конец файла %d\n"),s,n);
	else
		fprintf(stderr,"%s: %s\n",s,ediag(f_errlist[n-100],f_rerrlist[n-100]));
	fprintf(stderr,ediag("apparent state: unit %d named %s\n","Состояние: канал %d связан с %s\n"),curunit-units,
		STR(curunit->ufnm));
	fprintf(stderr,ediag("last format: %s\n","пред.формат: %s\n"),STR(fmtbuf));
	fprintf(stderr,ediag("lately %s %s %s %s IO\n","обмен: %s %s %s %s \n"),reading?ediag("reading","чтение"):ediag("writing","запись"),
		sequential?ediag("sequential","последовательный"):ediag("direct","прямой"),formatted?ediag("formatted","форматный"):ediag("unformatted","неформатный"),
		external?ediag("external","внешний"):ediag("internal","внутренний"));
	_cleanup();
	if(!nocore_) abort();
	exit(1);
}
/*initialization routine*/
f_init()
{       register unit *p;
	init=1;
	p= &units[0];
	p->ufd=stderr;
	p->useek=canseek(stderr);
	p->ufmt=1;
	p->uwrt=1;
	p = &units[5];
	p->ufd=stdin;
	p->useek=canseek(stdin);
	p->ufmt=1;
	p->uwrt=0;
	p= &units[6];
	p->ufd=stdout;
	p->useek=canseek(stdout);
	p->ufmt=1;
	p->uwrt=1;
}
canseek(f) FILE *f; /*SYSDEP*/
{	struct stat x;
	fstat(fileno(f),&x);
	if(x.st_nlink > 0 /*pipe*/ && !isatty(fileno(f)))
	{
		return(1);
	}
	return(0);
}
nowreading(x) register unit *x;
{
	long loc;
	if(x->uwrt == 0 ) return(0);
	x->uwrt=0;
	loc=ftell(x->ufd);
	if(!(x->ufd=freopen(x->ufnm,"r",x->ufd))) err(elist->cierr,117,"access");
	fseek(x->ufd,loc,0);
}
nowwriting(x) register unit *x;
{
	long loc;
	if(x->uwrt == 1 ) return(0);
	loc=ftell(x->ufd);
	x->uwrt=1;
	if(!(x->ufd=freopen(x->ufnm,"a",x->ufd))) err(elist->cierr,118,ediag("access","доступ"));
	fseek(x->ufd,loc,0);
}

chkend()
{
	if(!ferror(cf)) return(0);
	clearerr(cf);
	err(elist->cierr,118-reading,ediag("i/o end","конец обмена"));
}
