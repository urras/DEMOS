#ifndef _sys_callout_h_
#       define  _sys_collout_h_

/*
 * The callout structure is for
 * a routine arranging
 * to be called by the clock interrupt
 * (clock.c) with a specified argument,
 * in a specified amount of time.
 * Used, for example, to time tab
 * delays on typewriters.
 */

struct  callout
{
	short   c_time;                 /* incremental time */
	caddr_t c_arg;                  /* argument to routine */
	short   (*c_func)();            /* routine */
};

#ifdef  KERNEL
struct  callout callout[];
struct  callout *callNCALL;
#endif

#endif  _sys_callout_h_
