#include "fio.h"
#include "fmt.h"
#include "lio.h"
extern char *fmtbuf;
int (*lioproc)();
do_lio(type,number,ptr,len) ftnint *number,*type; flex *ptr; ftnlen len;
{
	return((*lioproc)(number,ptr,len,*type));
}
