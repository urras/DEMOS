#include <stdio.h>
#include <sgtty.h>

int     _askplot();

int     (*askplot)() = _askplot;

openvt ()
{
	openpl();
}

openpl()
{
	/*
	 * Установка размера шрифта, первого пишущего эл-та, и.т.п.
	 */
	puts("SC160,80,80MA0,0P1");
	fflush(stdout);
}

unsigned _sizex = 11880,        /* размер области рисования по x */
	 _sizey = 8400;         /* размер области рисования по y */

/*
 * Процедуре askplot передается имя у-ва для чтения
 */
_askplot(devname)
char *devname;
{
	int fd;
	int cfd;
	long l;
	char c;
	int rcnt = 0, wcnt, dig, xcnt;
	unsigned x0, y0, x1, y1;

	fd = open(devname, 2);
	if( fd < 0 )
		return;
Flush:
	if( ioctl(fd, FIONREAD, &l) < 0)
		return;
	if( l != 0l ) {
		read(fd, &c, 1);
		goto Flush;
	}
	xcnt = 0;
	for(;;) {
		wcnt = 0;
		write(fd, "I", 1);
		for(;;) {
			ioctl(fd, FIONREAD, &l);
			if( l == 0l ) {
				sleep(1);
				if( ++wcnt <= 6 )
					continue;
				if( rcnt != 0 )
					goto OK;
				break;
			}
			read(fd, &c, 1);
			rcnt++;
		}
		if( ++xcnt > 10 )
			xcnt = 0;
		if( xcnt == 1 ) {
			cfd = open("/dev/console", 1);
			if( cfd >= 0 ) {
				write(cfd, " WANNA PLOT...\7\r\n", 17);
				close(cfd);
			}
		}
	}

	/*
	 * Read coordinates
	 */
OK:
	write(fd, "SI", 2);

	x0 = 0; dig = 0;
	do {
		read(fd, &c, 1);
		if( '0' <= c && c <= '9' ) {
			dig++;
			x0 = x0*10 + (c-'0');
		}
	} while( !dig || c != '\n' );
	y0 = 0; dig = 0;
	do {
		read(fd, &c, 1);
		if( '0' <= c && c <= '9' ) {
			dig++;
			y0 = y0*10 + (c-'0');
		}
	} while( !dig || c != '\n' );
	x1 = 0; dig = 0;
	do {
		read(fd, &c, 1);
		if( '0' <= c && c <= '9' ) {
			dig++;
			x1 = x1*10 + (c-'0');
		}
	} while( !dig || c != '\n' );
	y1 = 0; dig = 0;
	do {
		read(fd, &c, 1);
		if( '0' <= c && c <= '9' ) {
			dig++;
			y1 = y1*10 + (c-'0');
		}
	} while( !dig || c != '\n' );
	close(fd);

	/*
	 * Check validity
	 */
	if( y1 <= y0 || x1 <= x0 ||
	    x1-x0 > _sizex || y1-y0 > _sizey )
		return;
	_sizex = x1-x0;
	_sizey = y1-y0;
}
