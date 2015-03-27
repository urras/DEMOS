/* sequential formatted external common routines*/
#include "fio.h"
extern char *fmtbuf;
e_rsfe()
{       int n,m;
	n=en_fio();
	fmtbuf=NULL;
	m=chkend();
	return(n?n:m);
}
c_sfe(a,flag) register cilist *a; /* check */
{       register unit *p;
	if(a->ciunit >= MXUNIT || a->ciunit<0)
		err(a->cierr,101,"startio");
	p = &units[a->ciunit];
	if(p->ufd==NULL && fk_open(flag,SEQ,FMT,a->ciunit)) err(a->cierr,114,"sfe")
	if(!p->ufmt) err(a->cierr,102,"sfe")
	return(0);
}
e_wsfe()
{	return(e_rsfe());
}
