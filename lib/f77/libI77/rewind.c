#include "fio.h"
f_rew(a) register alist *a;
{
	register unit *b;
	if(a->aunit>=MXUNIT || a->aunit<0) err(a->aerr,101,"rewind");
	b = &units[a->aunit];
	if(b->ufd == NULL) return(0);
	if(!b->useek) err(a->aerr,106,"rewind")
	if(b->uwrt)
	{       SETREAD(b);
/*              t_runc(b);      /* пока выключили */
	}
	rewind(b->ufd);
	b->uend=0;
	return(0);
}
