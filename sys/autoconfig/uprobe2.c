/*
 * uprobe2.c: continuation of uprobes
 * (separated into two files because the include files overflow cpp otherwise).
 *
 * User-level probe routines to make devices interrupt.
 * One per device; entered through uprobe table.
 * Return values:
 *      ACP_NXDEV       device doesn't exist
 *      ACP_IFINTR      OK if device has interrupted by now
 *      ACP_EXISTS      OK, not checking interrupt
 *
 * NOTES:
 *      Reads and writes to kmem (done by grab, stuff)
 *      are currently done a byte at a time in the kernel.
 *      Beware!
 */

#include        "uprobe.h"
#include        <sys/param.h>
#include        <sys/autoconfig.h>
#include        <sgtty.h>

#include        "../include/djreg.h"
#include        "../include/dhreg.h"
#include        "../include/pcreg.h"
#include        "../include/dnreg.h"
#undef  BITS6
#undef  BITS7
#undef  BITS8
#undef  TWOSB
#undef  PENABLE
#undef  OPAR
#include        "../include/dhureg.h"
#undef  BITS6
#undef  BITS7
#undef  BITS8
#undef  TWOSB
#undef  PENABLE
#include        "../include/dzreg.h"
#include        "../include/klreg.h"
#include        "../include/lpreg.h"

extern  int errno;

klprobe(addr)
struct dldevice *addr;
{
	int i;

	stuff(grab(&(addr->dlxcsr)) | DLXCSR_TIE, &(addr->dlxcsr));
	for (i = 0; i < 7; i++)
		DELAY(10000);
	/*
	 *  Leave TIE enabled; kl.c never turns it off
	 *  (and this could be the console).
	 */
	return(ACP_IFINTR);
}

wlprobe(addr)
short *addr;
{
	int csr;

	errno = 0;
	csr = grab(addr);
	if( errno == 0 ){
		stuff( csr | 0140, addr );
		csr = grab(addr);
		stuff( 0, addr );
		if( csr & (0100000 | 0200) )
			return(ACP_EXISTS);
	}
	return(ACP_NXDEV);
}

pkprobe(addr)
struct dldevice *addr;
{
      int i;

      stuff(grab(&(addr->dlxcsr)) | DLXCSR_TIE, &(addr->dlxcsr));
      for (i = 0; i < 7; i++)
	      DELAY(10000);
      /*
       *  Leave TIE enabled; kl.c never turns it off
       *  (and this could be the console).
       */
      return(ACP_IFINTR);
}

pcprobe(addr)
struct pcdevice *addr;
{
	stuff(0,                    &(addr->pcrcsr));
	stuff(PC_RDRENB|PC_IENABLE, &(addr->pcrcsr));
	DELAY(10);
	stuff(0, &(addr->pcrcsr));
	return(ACP_IFINTR);
}

dzprobe(addr)
struct dzdevice *addr;
{
	register int i;

	stuff(grab(&(addr->dzcsr)) | DZ_TIE | DZ_MSE, &(addr->dzcsr));
	stuff(1, &(addr->dztcr));
	for (i = 0; i < 7; i++)
		DELAY(10000);
	stuff(DZ_CLR, &(addr->dzcsr));
	return(ACP_IFINTR);
}


dhprobe(addr)
struct dhdevice *addr;
{
	register int i;

	stuff(DH_TIE, &(addr->dhcsr));
	DELAY(5);
	stuff((B9600 << 10) | (B9600 << 6) | BITS7|PENABLE, &(addr->dhlpr));
	stuff(-1, &(addr->dhbcr));
	stuff(0, &(addr->dhcar));
	stuff(1, &(addr->dhbar));
	for (i = 0; i < 7; i++)
		DELAY(10000);
	stuff(0, &(addr->dhcsr));
	return(ACP_IFINTR);
}

dhuprobe(addr)
struct dhudevice *addr;
{
      errno = 0;
      grab(&(addr->un.dhucsr));
      if(errno = 0)
	      return(ACP_NXDEV);
      return(ACP_EXISTS);
}

djprobe(addr)
struct djdevice *addr;
{
	register int i;

	stuff(DJ_MTSE | DJ_TIE | DJ_RIE | DJ_RE, &(addr->djcsr));
	DELAY(5);
	stuff(-1, &(addr->djtcr));
	for (i = 0; i < 7; i++)
		DELAY(10000);
	stuff(0, &(addr->djcsr));
	return(ACP_IFINTR);
}

dmprobe(addr)
struct dmdevice *addr;
{
	stuff(grab(&(addr->dmcsr)) | DM_DONE | DM_IE, &(addr->dmcsr));
	DELAY(20);
	stuff(0, &(addr->dmcsr));
	return(ACP_IFINTR);
}

/*
 *  Try to make the first unit of a DN-11 interrupt.
 */
dnprobe(addr)
struct dndevice *addr;
{
	stuff(DN_MINAB | DN_INTENB | DN_DONE, (&(addr->dnisr[0])));
	DELAY(5);
	stuff(0, (&(addr->dnisr[0])));
	return(ACP_IFINTR);
}

lpprobe(addr)
struct lpdevice *addr;
{
	stuff(0, &(addr->lpcs));
	DELAY(10);
	stuff(grab(&(addr->lpcs)) | LP_IE, &(addr->lpcs));
	DELAY(10);
	stuff(0, &(addr->lpcs));
	return(ACP_IFINTR);
}

#ifdef  VIRUS
/*
 * Can't make the cary interrupt unless it's in fixed-wavelength mode.
 */
/*ARGSUSED*/
caryprobe(addr)
{
	return(ACP_EXISTS);
}
#endif

ksprobe(addr)
struct dldevice *addr;
{

	stuff(0, &(addr->dlxcsr));
	stuff(DLKR_RESET, &(addr->dlrcsr));
	DELAY(10);
	stuff(grab(&(addr->dlxcsr)) | DLKX_TIE, &(addr->dlxcsr));
	DELAY(1000);
	stuff(0, &(addr->dlxcsr));
	return(ACP_IFINTR);
}
