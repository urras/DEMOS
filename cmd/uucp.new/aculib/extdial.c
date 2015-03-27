#ifndef lint
static char Rcsid[] = "$Header$";
#endif !lint

#include "../condevs.h"
#ifdef  USG
#include        <fcntl.h>
#endif USG

/*
 *      extdopen(telno,flds,dev) - use external dialer to open line
 *	char *flds[], *dev[];
 *
 *	return codes:
 *		>0  -  file number  -  ok
 *		CF_DIAL,CF_DEVICE  -  failed
 */
#define RC_FAIL 0x80

static char speed[10];
static char dialer[MAXFULLNAME];

/* ARGSUSED */
extdopen(telno, flds, dev)
char *telno;
char *flds[];
struct Devices *dev;
{
	int	dh = -1;
	char *ii;
	extern errno;
	char dcname[20];
	char dstr[10];
	int c, pid, errflag;

	sprintf(dcname, "/dev/%s", dev->D_line);
	DEBUG(4, "dc - %s\n", dcname);
	if (setjmp(Sjbuf)) {
		logent(dcname, "TIMEOUT");
		if (dh >= 0)
			extdclose(dh);
		return CF_DIAL;
	}
	signal(SIGALRM, alarmtr);
	getnextfd();
	alarm(10);
#ifndef USG
	dh = open(dcname, 2); /* read/write */
#else
	dh = open(dcname, O_RDWR | O_NDELAY); /* read/write */
#endif  USG
	alarm(0);

	/* modem is open */
	next_fd = -1;
	sprintf(speed,"%d",dev->D_speed);
	strcpy(dialer,dev->D_brand);
	if (dh >= 0) {
		if ((pid = fork()) == 0) {
			chdir(UULIB);
			if( Debug ) {
			   sprintf(dstr,"-x%d",Debug);
			   execl(dialer, dialer, dstr, dcname, telno, speed, NULL);
			} else
			   execl(dialer, dialer, dcname, telno, speed, NULL);
			DEBUG(4, "can't exec %s\n", dialer);
			exit(-1);
		}
		while (((c = wait(&errflag)) != pid) && c != -1);
		errflag >>= 8;
		if( (errflag) & RC_FAIL ) {
			DEBUG(4,"external dialer failed=%d\n",errflag);
			close(dh);
			return CF_DIAL;
		}
		fixline(dh, dev->D_speed);
	}
	if (dh < 0) {
		logent(dcname, "CAN'T OPEN");
		return dh;
	}
	DEBUG(4, "external dialer ok\n", CNULL);
	return dh;
}

extdclose(fd)
int fd;
{
	char dcname[20];
	char dstr[10];
	int errflag,c,pid;

	if (fd > 0) {
		sprintf(dcname, "/dev/%s", devSel);
		DEBUG(4, "Hanging up fd = %d\n", fd);
		sleep(1);
		if ((pid = fork()) == 0) {
			chdir(UULIB);
			if( Debug ) {
			   sprintf(dstr,"-x%d",Debug);
			   execl(dialer, dialer, dstr, "-h", dcname, speed, NULL);
			} else
			   execl(dialer, dialer, "-h", dcname, speed, NULL);
			DEBUG(4, "can't exec %s\n", dialer);
			exit(-1);
		}
		while (((c = wait(&errflag)) != pid) && c != -1);
		errflag >>= 8;
		if( (errflag) & RC_FAIL ) {
			DEBUG(4,"external dialer failed=%d\n",errflag);
		}
		close(fd);
		delock(devSel);
	}
}
