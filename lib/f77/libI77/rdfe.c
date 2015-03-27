#include "fio.h"
#include "fmt.h"
extern int rd_ed(),rd_ned(),y_getc(),y_putc(),y_err();
extern int y_rev();
extern int w_ed(),w_ned();
s_rdfe(a) register cilist *a;
{
	register int n;
	if(!init) f_init();
	if(n=c_dfe(a,READ))return(n);
	reading=1;
	SETREAD(curunit);
	getn = y_getc;
	doed = rd_ed;
	doned = rd_ned;
	dorevert = donewrec = y_err;
	doend = y_rev;
	if(pars_f(fmtbuf)<0)
		err(a->cierr,100,"startio");
	fmt_bg();
	return(0);
}
e_rdfe()
{
	en_fio();
	return(chkend());
}
