#ifndef _sys_text_h_
#       define  _sys_text_h_
/*
 * Text structure.
 * One allocated per pure
 * procedure on swap device.
 * Manipulated by text.c
 */
struct text
{
	short   x_daddr;        /* segment's disk address (relative to swplo) */
	short   x_caddr;        /* core address, if loaded */
	short   x_size;         /* size (clicks) */
	struct  inode *x_iptr;  /* inode of prototype */
	char    x_count;        /* reference count */
	char    x_ccount;       /* number of loaded references */
	char    x_flag;         /* traced, written flags */
};

#ifdef  KERNEL
extern  struct  text    text[];
#endif

#define XTRC    01              /* Text may be written, exclusive use */
#define XWRIT   02              /* Text written into, must swap out */
#define XLOAD   04              /* Currently being read from file */
#define XLOCK   010             /* Being swapped in or out */
#define XWANT   020             /* Wanted for swapping */
#define XFREE   040             /* Сегмент можно освобождать */

/* arguments to xswap: */
#define X_OLDSIZE       (-1)    /* the old size is the same as current */
#define X_DONTFREE      0       /* save core image (for parent in newproc) */
#define X_FREECORE      01      /* free core space after swap */

#ifdef FXMETER
struct fsxmeter {
int fx_cnt,nx_cnt,ns_cnt;
long fx_blk,nx_blk,ns_blk;
};
#endif

#if defined(FXMETER) && defined(KERNEL)
struct fsxmeter fsxmeter;
#define fxcount  fsxmeter.fx_cnt
#define fxblocks fsxmeter.fx_blk
#define nxcount  fsxmeter.nx_cnt
#define nxblocks fsxmeter.nx_blk
#define nscount  fsxmeter.ns_cnt
#define nsblocks fsxmeter.ns_blk
#define xmeter(x) if(x->x_ccount == 0 && x->x_caddr) fxcount++, fxblocks += (unsigned)(x->x_size+63)>>3;else {if(x->x_caddr==0)nxcount++, nxblocks += (unsigned)(x->x_size+63)>>3;};
#ifdef VIRUS_VFORK
#define smeter(p) (nscount++, nsblocks += (p->p_dsize+p->p_ssize+USIZE+63)>>3);
#else
#define smeter(p) (nscount++, nsblocks += (p->p_size+63)>>3);
#endif VIRUS_VFORK
#else
#define xmeter(x) /* */
#define smeter(x) /* */
#endif FXMETER && KERNEL

#endif  _sys_text_h_
