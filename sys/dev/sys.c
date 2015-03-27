/*
 * $Log:	sys.c,v $
 * Revision 22.2  89/04/28  15:22:24  avg
 * Убран #ifdef IPK_SELECT
 * 
 * Revision 22.1  89/04/12  14:35:30  korotaev
 * "param.h" ==> <sys/param.h>
 * 
 * Revision 22.0  89/03/25  12:33:22  korotaev
 * Отсюда начинается версия 2.2
 * 
 * Revision 1.2  88/06/06  14:32:21  korotaev
 * Вставлен IPK_SELECT (from TCP/IP).
 * 
 * Revision 1.1  86/04/19  17:55:04  avg
 * Initial revision
 * 
 */

/*
 *      indirect driver for controlling tty.
 */
#include <sys/param.h>
#include <sys/conf.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/tty.h>
#include <sys/proc.h>


syopen(dev, flag)
dev_t dev;
{
	if(u.u_ttyp == NULL) {
		u.u_error = ENXIO;
		return;
	}
	(*cdevsw[major(u.u_ttyd)].d_open)(u.u_ttyd, flag);
}

syread(dev)
dev_t dev;
{
	if (u.u_ttyp == NULL) {
		u.u_error = ENXIO;
		return;
	}
	(*cdevsw[major(u.u_ttyd)].d_read)(u.u_ttyd);
}

sywrite(dev)
dev_t dev;
{
	if (u.u_ttyp == NULL) {
		u.u_error = ENXIO;
		return;
	}
	(*cdevsw[major(u.u_ttyd)].d_write)(u.u_ttyd);
}

sysioctl(dev, cmd, addr, flag)
dev_t dev;
caddr_t addr;
{
	if (cmd == TIOCNOTTY) {
		u.u_ttyp = 0;
		u.u_ttyd = 0;
		u.u_procp->p_pgrp = 0;
		return;
	}
	if (u.u_ttyp == NULL) {
		u.u_error = ENXIO;
		return;
	}
	(*cdevsw[major(u.u_ttyd)].d_ioctl)(u.u_ttyd, cmd, addr, flag);
}

syselect(dev, flag)
{
	if (u.u_ttyp == NULL) {
		u.u_error = ENXIO;
		return (0);
	}
	return ((*cdevsw[major(u.u_ttyd)].d_select)(u.u_ttyd, flag));
}
