#include "fio.h"
#include "fmt.h"
extern int rd_ed(),rd_ned(),y_getc(),y_putc(),y_err();
extern int y_rev();
extern int w_ed(),w_ned();
c_dfe(a,flag) register cilist *a;
{
	sequential=0;
	formatted=external=1;
	elist=a;
	cursor=scale=recpos=0;
	if(a->ciunit>MXUNIT || a->ciunit<0)
		err(a->cierr,101,"startchk");
	curunit = &units[a->ciunit];
	if(curunit->ufd==NULL && fk_open(flag,DIR,FMT,a->ciunit))
		err(a->cierr,104,"dfe");
	cf=curunit->ufd;
	if(!curunit->ufmt) err(a->cierr,102,"dfe")
	if(!curunit->useek) err(a->cierr,104,"dfe")
	fmtbuf=a->cifmt;
	fseek(cf,(long)curunit->url * (a->cirec-1),0);
	curunit->uend = 0;
	return(0);
}
y_getc()
{
	int ch;
	if(curunit->uend) return(-1);
	if((ch=getc(cf))!=EOF)
	{
		recpos++;
		if(curunit->url>=recpos ||
			curunit->url==1)
			return(ch);
		else	return(' ');
	}
	if(feof(cf))
	{
		curunit->uend=1;
		errno=0;
		return(-1);
	}
	err(elist->cierr,errno,"readingd");
}
y_putc(c)
{
	recpos++;
	if(recpos <= curunit->url || curunit->url==1)
		putc(c,cf);
	else
		err(elist->cierr,110,"dout");
	return(0);
}
y_rev(sy)char sy;
{	/*what about work done?*/
	if(curunit->url==1 || recpos==curunit->url)
		return(0);
	while(recpos<curunit->url)
		(*putn)(' ');
	recpos=0;
	return(0);
}
y_err()
{
	err(elist->cierr, 110, "dfe");
}











































































































































