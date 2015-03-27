#ifndef lint
/*NOXSTR*/
static char rcsid[] = "$Header: alloc.c,v 2.2 86/11/26 17:24:17 dmitry Exp $";
/*YESXSTR*/
#endif  lint

/*
 *      $Author: dmitry $
 *      $Date: 86/11/26 17:24:17 $
 *      $State: Exp $
 * Специальный вариант malloc для Си-Компилятора.
 * Отличается тем, что память берется из буфера длиной "BLOCK" байтов.
 *
 *      $Log:	alloc.c,v $
 *	Revision 2.2  86/11/26  17:24:17  dmitry
 *	Исправлена ошибка -
 *	Headerы не должны обрабатываться xstrом.
 *	
 *      Revision 2.1  86/11/26  16:50:00  alex
 *      *** empty log message ***
 *
 */
#include "c0.h"
#undef ALIGN
#undef INT

#define BLOCK BUFNAME      /* Размер области под имена переменных */

#ifdef debug
#include <stdio.h>
#define ASSERT(p) if(!(p))botch("p");else
botch(s)
char *s;
{
	fprintf(stderr,"assertion botched: %s\n",s); fflush(stderr);
	abort();
}
#else
#define ASSERT(p)
#endif

/*      avoid break bug */
#ifdef pdp11
#define GRANULE 64
#else
#define GRANULE 0
#endif
/*      C storage allocator
 *
 *      a different implementation may need to redefine
 *      ALIGN, NALIGN, BLOCK, BUSY, INT
 *      where INT is integer type to which a pointer can be cast
*/
#define INT int
#define ALIGN int
#define NALIGN 1
#define WORD sizeof(union store)
#define BUSY 1
#define NULL 0
#define testbusy(p) ((INT)(p)&BUSY)
#define setbusy(p) (union store *)((INT)(p)|BUSY)
#define clearbusy(p) (union store *)((INT)(p)&~BUSY)

union store { union store *ptr;
	      ALIGN dummy[NALIGN];
	      int calloc;       /*calloc clears an array of integers*/
};

static  union store allocs[2];  /*initial arena*/
static  union store *allocp;    /*search ptr*/
static  union store *alloct;    /*arena top*/
static  union store *allocx;    /*for benefit of realloc*/
static flag;
static char buf_all[BLOCK+WORD*4];  /* Буфер для выделения памяти */

char *
malloc(nbytes)
unsigned nbytes;
{
	register union store *p, *q;
	register nw;
	static temp;    /*coroutines assume no auto*/

	if(allocs[0].ptr==0) {  /*first time*/
		allocs[0].ptr = setbusy(&allocs[1]);
		allocs[1].ptr = setbusy(&allocs[0]);
		alloct = &allocs[1];
		allocp = &allocs[0];
	}
	nw = (nbytes+WORD+WORD-1)/WORD;
	ASSERT(allocp>=allocs && allocp<=alloct);
	ASSERT(allock());
	for(p=allocp; ; ) {
		for(temp=0; ; ) {
			if(!testbusy(p->ptr)) {
				while(!testbusy((q=p->ptr)->ptr)) {
					ASSERT(q>p&&q<alloct);
					p->ptr = q->ptr;
				}
				if(q>=p+nw && p+nw>=p)
					goto found;
			}
			q = p;
			p = clearbusy(p->ptr);
			if(p>q)
				ASSERT(p<=alloct);
			else if(q!=alloct || p!=allocs) {
				ASSERT(q==alloct&&p==allocs);
				return(NULL);
			} else if(++temp>1)
				break;
		}
		if(flag++)
			return(NULL);
		temp = (BLOCK/WORD);
		q = (union store *)buf_all;     /* Память взяли из буфера */
		ASSERT(q>alloct);
		alloct->ptr = q;
		if(q!=alloct+1)
			alloct->ptr = setbusy(alloct->ptr);
		alloct = q->ptr = q+temp-1;
		alloct->ptr = setbusy(allocs);
	}
found:
	allocp = p + nw;
	ASSERT(allocp<=alloct);
	if(q>allocp) {
		allocx = allocp->ptr;
		allocp->ptr = p->ptr;
	}
	p->ptr = setbusy(allocp);
	return((char *)(p+1));
}

/*      freeing strategy tuned for LIFO allocation
*/
free(ap)
register char *ap;
{
	register union store *p = (union store *)ap;

	ASSERT(p>clearbusy(allocs[1].ptr)&&p<=alloct);
	ASSERT(allock());
	allocp = --p;
	ASSERT(testbusy(p->ptr));
	p->ptr = clearbusy(p->ptr);
	ASSERT(p->ptr > allocp && p->ptr <= alloct);
}

/*      realloc(p, nbytes) reallocates a block obtained from malloc()
 *      and freed since last call of malloc()
 *      to have new size nbytes, and old content
 *      returns new location, or 0 on failure
*/

char *
realloc(p, nbytes)
register union store *p;
unsigned nbytes;
{
	register union store *q;
	union store *s, *t;
	register unsigned nw;
	unsigned onw;

	if(testbusy(p[-1].ptr))
		free((char *)p);
	onw = p[-1].ptr - p;
	q = (union store *)malloc(nbytes);
	if(q==NULL || q==p)
		return((char *)q);
	s = p;
	t = q;
	nw = (nbytes+WORD-1)/WORD;
	if(nw<onw)
		onw = nw;
	while(onw--!=0)
		*t++ = *s++;
	if(q<p && q+nw>=p)
		(q+(q+nw-p))->ptr = allocx;
	return((char *)q);
}

#ifdef debug
allock()
{
#ifdef longdebug
	register union store *p;
	int x;
	x = 0;
	for(p= &allocs[0]; clearbusy(p->ptr) > p; p=clearbusy(p->ptr)) {
		if(p==allocp)
			x++;
	}
	ASSERT(p==alloct);
	return(x==1|p==allocp);
#else
	return(1);
#endif
}
#endif
