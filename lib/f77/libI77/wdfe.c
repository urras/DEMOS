#include "fio.h"
#include "fmt.h"
extern int rd_ed(),rd_ned(),y_getc(),y_putc(),y_err();
extern int y_rev();
extern int w_ed(),w_ned();
s_wdfe(a) register cilist *a;
{
	register int n;
	if(!init) f_init();
	if(n=c_dfe(a,WRITE)) return(n);
	reading=0;
	SETWRITE(curunit);
	putn = y_putc;
	doed = w_ed;
	doned= w_ned;
	dorevert = donewrec = y_err;
	doend = y_rev;
	if(pars_f(fmtbuf)<0)
		err(a->cierr,100,"startwrt");
	fmt_bg();
	return(0);
}
e_wdfe()
{
	en_fio();
	return(chkend());
}
