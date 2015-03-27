#ifdef SYSV

#include <stdio.h>
#include <sys/fcntl.h>

struct rdbuf {
    int   rd_nread;
    char  rd_ndelay;
    char  rd_char;
    char  rd_fd;
 };
static struct rdbuf rd0, rdx;
static struct rdbuf *getrdb ();

empty (fd)
int fd;
{
    struct rdbuf *rdb;

    rdb = getrdb (fd);

    if (rdb->rd_nread > 0)
        return 0;
    if (!rdb->rd_ndelay) {
        (void) fcntl (fd, F_SETFL, O_RDONLY | O_NDELAY);
        rdb->rd_ndelay = 1;
    }
    return (rdb->rd_nread = read (fd, &rdb->rd_char, 1)) <= 0;
}

int
xread (fd, buf, count)
int fd;
char *buf;
int count;
{
    int retval;
    struct rdbuf *rdb;

    rdb = getrdb (fd);
    if (count == 0)
        return 0;
    if (rdb->rd_nread > 0) {
        buf[0] = rdb->rd_char;
        count--;
    }
    else if (rdb->rd_ndelay) {
        (void) fcntl (fd, F_SETFL, O_RDONLY);
        rdb->rd_ndelay = 0;
    }
    retval = rdb->rd_nread + read (fd, &buf[rdb->rd_nread], count);
    rdb->rd_nread = 0;
    return retval;
}

static
void
xflush (fd)
int fd;
{
    struct rdbuf *rdb;

    rdb = getrdb (fd);
    rdb->rd_nread = 0;
}

static
struct rdbuf *
getrdb (fd)
int fd;
{
    struct rdbuf *rdb;

    if (fd == fileno (stdin)) {
        rdb = &rd0;
        rd0.rd_fd = fd;
    }
    else {
        if (fd != rdx.rd_fd && rdx.rd_fd != 0) exit(1);
        rdb = &rdx;
        rdx.rd_fd = fd;
    }
    return rdb;
}

#else

/*
 * Имитатор системного вызова empty (МНОС РЛ 1.2) в ДЕМОС 2.0
 */

#include <sgtty.h>

empty(chn)
{
#ifdef VENIX

    struct sgttyb arg;

    ioctl(chn, TIOCQCNT, &arg);
    return( arg.sg_ispeed==0 );

#else /* BSD */

    long NR;

    ioctl(chn, FIONREAD, &NR);
    return( NR==0l );

#endif
}

#endif
