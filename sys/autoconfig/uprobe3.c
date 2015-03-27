/*
 * User-level probe routines to make devices interrupt.
 * One per device; entered through uprobe table.
 * Return values:
 *	ACP_NXDEV	device doesn't exist
 *	ACP_IFINTR	OK if device has interrupted by now
 *	ACP_EXISTS	OK, not checking interrupt
 *
 * NOTES:
 *	Reads and writes to kmem (done by grab, stuff)
 *	are currently done a byte at a time in the kernel.
 *	Beware!
 *
 *	The hs, rp, hk and dvhp probes have not been tested.
 */

#include	"uprobe.h"
#include	<sys/param.h>
#include	<sys/autoconfig.h>

#include        "../include/htreg.h"
#undef	b_repcnt
#undef  b_command
#include        "../include/tmreg.h"
#undef	b_repcnt
#undef  b_command
#include        "../include/tsreg.h"

int	htprobe(), tmprobe(), tsprobe();
extern	int errno;

htprobe(addr)
struct htdevice *addr;
{
	stuff(HT_SENSE | HT_IE | HT_GO, (&(addr->htcs1)));
	DELAY(10);
	stuff(0, (&(addr->htcs1)));
	return(ACP_IFINTR);
}

/*
 * TM-11 probe routine.
 * Also check one of the more distant registers
 * to make sure this isn't a TS-11.
 */
tmprobe(addr)
struct tmdevice *addr;
{
	stuff(0, &(addr->tmcs));
	DELAY(10);
	stuff(TM_IE, &(addr->tmcs));
	errno = 0;
	grab(&(addr->tmba));
	if (errno != 0)
		return(ACP_NXDEV);
	return(ACP_IFINTR);
}

/*
 * TS-11 probe.
 * Assume that the device exists if there's no TM-11 there.
 */
tsprobe(addr)
struct tsdevice *addr;
{
	int i;

	errno = 0;
	i = grab(&((struct tsdevice *)addr->tssr));
	if( errno == 0 && (i & (TS_NBA | TS_OFL)) )
		return(ACP_EXISTS);
	return(ACP_NXDEV);
}

