/*
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
 *
 *      The hs, rp, hk and dvhp probes have not been tested.
 */

#include        "uprobe.h"
#include        <sys/param.h>
#include        <sys/autoconfig.h>

#include        "../include/hkreg.h"
#include        "../include/hpreg.h"
#include        "../include/hsreg.h"
#include        "../include/rkreg.h"
#include        "../include/rlreg.h"
#include        "../include/rpreg.h"
#include        "../include/rxreg.h"
#include        "../include/dwreg.h"
#include        "../include/myreg.h"

int     xpprobe(), hkprobe(), hsprobe(), rlprobe(), rkprobe(), rpprobe();
int     htprobe(), tmprobe(), tsprobe();
int raprobe();
int     dnprobe(), klprobe(), dzprobe(), dhprobe(), dmprobe(), djprobe();
int     lpprobe(), wlprobe(),
#ifdef UNDEF
vpprobe(),
#endif UNDEF
	rxprobe(), pcprobe(), dhuprobe(), pkprobe();
int     ksprobe(), dwprobe(), myprobe();        /* Chech&Co */
#ifdef  VIRUS
int     caryprobe();
#endif
extern  int errno;

struct uprobe uprobe[] = {
	/*
	 *      Disks
	 */
	"xp",   xpprobe,
	"rm",   xpprobe,
	"hp",   xpprobe,
	"hk",   hkprobe,
	"hs",   hsprobe,
	"rl",   rlprobe,
	"rk",   rkprobe,
	"rp",   rpprobe,
	"rx",   rxprobe,
	"dw",   dwprobe,        /* Винчестер ДВК-4 */
	"my",   myprobe,        /* гибкий диск MY (Qbus22) */
	"ra",   raprobe,

	/*
	 *      Tapes
	 */
	"ht",   htprobe,
	"tm",   tmprobe,
	"ts",   tsprobe,

	/*
	 *      Communication interfaces
	 */
	"dn",   dnprobe,
	"kl",   klprobe,
	"dz",   dzprobe,
	"dh",   dhprobe,
	"dm",   dmprobe,
	"dj",   djprobe,
	"ks",   ksprobe,        /* DL/Kisi - один канал, 2 байта */
	"mk",   ksprobe,        /* DL/Kisi - мыльтиплексор */
	"dhu",  dhuprobe,
	"pk",   pkprobe,
	"wl",   wlprobe,
	/*
	 *      Printers
	 */
	"lp",   lpprobe,
#ifdef UNDEF
	"vp",   vpprobe,
#endif UNDEF
	/*
	 * Paper tapes
	 */
	"pc",   pcprobe,

	0,      0
};

xpprobe(addr)
struct hpdevice *addr;
{
	stuff(HP_IE | HP_RDY, &(addr->hpcs1.w));
	DELAY(10);
	stuff(0, &(addr->hpcs1.w));
	return(ACP_IFINTR);
}

hkprobe(addr)
struct hkdevice *addr;
{
	stuff(HK_CDT | HK_IE | HK_CRDY, (&(addr->hkcs1)));
	DELAY(10);
	stuff(HK_CDT, (&(addr->hkcs1)));
	return(ACP_IFINTR);
}

hsprobe(addr)
struct hsdevice *addr;
{
	stuff(HS_IE | HS_DCLR | HS_GO, (&(addr->hscs1)));
	DELAY(10);
	stuff(0, (&(addr->hscs1)));
	return(ACP_IFINTR);
}

rlprobe(addr)
struct rldevice *addr;
{
	stuff(RL_GETSTATUS | RL_IE, (&(addr->rlcs)));
	DELAY(100);
	stuff(RL_CRDY, (&(addr->rlcs)));
	return(ACP_IFINTR);
}

rkprobe(addr)
struct rkdevice *addr;
{
	stuff( RKCS_RESET | RKCS_GO, (&(addr->rkcs)));
	DELAY(10);
	stuff(RKCS_IDE , (&(addr->rkcs)));
	DELAY(10);
	stuff(0, (&(addr->rkcs)));
	return(ACP_IFINTR);
}


rpprobe(addr)
struct rpdevice *addr;
{
	stuff( RP_IDLE | RP_GO, (&(addr->rpcs)));
	DELAY(10);
	stuff( RP_IDE , (&(addr->rpcs)));
	DELAY(10);
	return(ACP_IFINTR);
}

rxprobe(addr)
struct rxdevice *addr;
{
	stuff( RX_INIT, (&(addr->rxcs)));
	sleep(4);
	stuff( RX_INTR , (&(addr->rxcs)));
	DELAY(500);
	return(ACP_IFINTR);
}

/* Tест контроллера Винчестера ДВК-4
 * Тест прерываний через собственную "пробу" реализовать не удается,
 * т.к. контроллер не сбрасывает запрос прерывания с шины даже после
 * отработки его. Необходимо сбрасывать бит разр.прер-я устройства
 * сразу в программе обработки прерывания, это можно сделать только
 * поместив пробу непосредственно в ядро. Возможно, что это дыра
 * только нашего контроллера...
 * Чечик И.Е., Качулин Н.Г., ЛГУ,Физ.фак.
 */
dwprobe(addr)
struct dwdevice *addr;
{
	register cnt;
	for (cnt = 10; grab (&(addr->dw_stat)) & DWDS_BSY; cnt--) {
		if (!cnt)
			return(ACP_NXDEV);
		sleep (1);
	}
	if (grab (&(addr->dw_rid)) != 0401)
		return(ACP_NXDEV);
	return(ACP_EXISTS);
}

/* Tест контроллера гибкого диска MY (для Qbus22, напр. ДВК-4) */
myprobe(addr)
struct mydevice *addr;
{
	stuff (MY_RESET | MY_GO, (&(addr->my_csr)));
	DELAY (500);
	stuff (MY_IE | MY_RSTATE | MY_GO, (&(addr->my_csr)));
	DELAY (500);
	stuff (0, (&(addr->my_csr)));
	return(ACP_IFINTR);
}

/*
 * Must be only one.
 * Interupt vector write always.
 * (very difficult probe this device)
 */
raprobe(addr)
{
	return(ACP_EXISTS);
}
