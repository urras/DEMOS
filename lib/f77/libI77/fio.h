#include <stdio.h>
#include "fiodefs.h"

extern int errno;
extern flag init;
extern cilist *elist;	/*active external io list*/
extern flag reading,external,sequential,formatted;
extern int (*getn)(),(*putn)();	/*for formatted io*/
extern FILE *cf;	/*current file*/
extern unit *curunit;	/*current unit*/
extern unit units[];
#define err(f,n,s) {if(f) errno= n; else fatal(n,s); return(n);}
#define SETREAD(x) {if(nowreading(x)) return(errno);}
#define SETWRITE(x) {if(nowwriting(x)) return(errno);}
