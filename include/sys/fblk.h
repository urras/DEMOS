#ifndef _sys_fblk_h_
#       define  _sys_fblk_h_

/*
 * Структура данных в блоке из списка свободных.
 */

struct fblk
{
	short   df_nfree;
	daddr_t df_free[NICFREE];
};

#endif _sys_fblk_h_
