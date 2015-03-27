#ifndef lint
static char sccsid[] = "@(#)mnp5.c     1.1 (KIAE) 9/9/90";
#endif !lint

#include "../condevs.h"

#ifdef MNP5
/*
 *      mnp5popn(telno, flds, dev) connect to mnp-5 smartmodem (pulse call)
 *      mnp5topn(telno, flds, dev) connect to mnp-5 smartmodem (tone call)
 *	char *flds[], *dev[];
 *
 *	return codes:
 *		>0  -  file number  -  ok
 *		CF_DIAL,CF_DEVICE  -  failed
 */

mnp5popn(telno, flds, dev)
char *telno, *flds[];
struct Devices *dev;
{
	return mnp5opn(telno, flds, dev, 0);
}

mnp5topn(telno, flds, dev)
char *telno, *flds[];
struct Devices *dev;
{
	return mnp5opn(telno, flds, dev, 1);
}

/* ARGSUSED */
mnp5opn(telno, flds, dev, toneflag)
char *telno;
char *flds[];
struct Devices *dev;
int toneflag;
{
	register        errcnt = 0;
	int	dh = -1;
	char *ii;
	extern errno;
	char dcname[20];
	char dialtel[MAXPH];

	sprintf(dcname, "/dev/%s", dev->D_line);
	DEBUG(4, "dc - %s\n", dcname);
	if (setjmp(Sjbuf)) {
		logent(dcname, "TIMEOUT");
		if (dh >= 0)
			mnp5cls(dh);
		return CF_DIAL;
	}
	signal(SIGALRM, alarmtr);
	getnextfd();
	alarm(10);
	dh = open(dcname, 2); /* read/write */
	alarm(0);

	for (ii = telno; *ii; ii++)
		if (*ii == '-')
		    *ii = ',';

	/* modem is open */
	next_fd = -1;
	if (dh >= 0) {
		fixline(dh, dev->D_speed);
		sendthem("P_ZERO", dh);
		errcnt = 0;
reset:
		sendthem("\\r+++\\dATHZ", dh);
		if (expect("OK\r\n~4", dh) != 0) {
			errcnt++;
			if ((errcnt & 07) < 3)
				goto reset;
abort:
			logent(dcname, "MNP5 not responding OK");
			mnp5cls(dh);
			return CF_DIAL;
		}
		errcnt = ((errcnt >> 3) + 1) << 3;
		if (errcnt > 030)
			goto abort;
		sendthem("\\d\\dAT&F", dh);
		if (expect("OK\r\n~4", dh) != 0)
			goto reset;
		sendthem("\\d\\dATS0=0BELM1X3&D3&P1#E2\\\\A1\\\\C1", dh);
		if (expect("OK\r\n~2", dh) != 0)
			goto reset;
		sendthem("\\d\\dAT\\\\J\\\\N3\\\\Q3\\\\T2\\\\V1%C1#M2", dh);
		if (expect("OK\r\n~2", dh) != 0)
			goto reset;
		sprintf(dialtel, "\\d\\dATD%c%s",
			toneflag ? 'T' : 'P', telno);
		sendthem(dialtel, dh);
		if (expect("CONNECT", dh) != 0) {
			logent("MNP5 no carrier", _FAILED);
			strcpy(devSel, dev->D_line);
			mnp5cls(dh);
			return CF_DIAL;
		}

	}
	if (dh < 0) {
		logent(dcname, "CAN'T OPEN");
		return dh;
	}
	DEBUG(4, "mnp5 ok\n", CNULL);
	return dh;
}

mnp5cls(fd)
int fd;
{
	char dcname[20];
	register errcnt = 0;

	if (fd > 0) {
		sprintf(dcname, "/dev/%s", devSel);
		DEBUG(4, "Hanging up fd = %d\n", fd);
/*
 * Since we have a getty sleeping on this line, when it wakes up it sends
 * all kinds of garbage to the modem.  Unfortunatly, the modem likes to
 * execute the previous command when it sees the garbage.  The previous
 * command was to dial the phone, so let's make the last command reset
 * the modem.
 */
again:
		sendthem("\\r+++\\dATS0=0H1", fd);
		sendthem("\\d\\d\\r+++\\dATHZ", fd);
		if (expect("OK\r\n~2", fd) != 0) {
			if (++errcnt < 2) {
				ioctl(fd, TIOCCDTR, (char *) 0);
				sleep(2);
				ioctl(fd, TIOCSDTR, (char *) 0);
				goto again;
			}
		}
		close(fd);
		delock(devSel);
	}
}
#endif MNP5
