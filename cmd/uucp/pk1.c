#define USER    1
#include <stdio.h>
#include <sys/pk.p>
#include <sys/param.h>
#include <sys/pk.h>
#include <sys/buf.h>
#include <setjmp.h>
#include <signal.h>

#define PKTIME 10
int Errorrate;
int Conbad = 0;
int Ntimeout = 0;
#define CONBAD 5
#define NTIMEOUT 50

static char hex[] = "0123456789abcdef";
extern int qnetused, sevenbits;

/*
 * packet driver support routines
 *
 */

struct pack *pklines[NPLINES];

/*
 * start initial synchronization.
 */

struct pack *
pkopen(ifn, ofn)
int ifn, ofn;
{
        struct pack *pk;
        char **bp;
        int i;

        if (++pkactive >= NPLINES)
                return(NULL);
        if ((pk = (struct pack *) malloc(sizeof (struct pack))) == NULL)
                return(NULL);
        pkzero((caddr_t) pk, sizeof (struct pack));
        pk->p_ifn = ifn;
        pk->p_ofn = ofn;
        pk->p_xsize = pk->p_rsize = PACKSIZE;
        pk->p_rwindow = pk->p_swindow = WINDOWS;
        /*  allocate input windows */
        for (i = 0; i < pk->p_rwindow; i++) {
                if ((bp = (char **) GETEPACK) == NULL)
                        break;
                *bp = (char *) pk->p_ipool;
                pk->p_ipool = bp;
        }
        if (i == 0)
                return(NULL);
        pk->p_rwindow = i;

        /* start synchronization */
        pk->p_msg = pk->p_rmsg = M_INITA;
        for (i = 0; i < NPLINES; i++) {
                if (pklines[i] == NULL) {
                        pklines[i] = pk;
                        break;
                }
        }
        if (i >= NPLINES)
                return(NULL);
        pkoutput(pk);

        while ((pk->p_state & LIVE) == 0) {
                PKGETPKT(pk);
        }

        pkreset(pk);
        return(pk);
}


/*
 * input framing and block checking.
 * frame layout for most devices is:
 *      
 *      S|K|X|Y|C|Z|  ... data ... |
 *
 *      where   S       == initial synch byte
 *              K       == encoded frame size (indexes pksizes[])
 *              X, Y    == block check bytes
 *              C       == control byte
 *              Z       == XOR of header (K^X^Y^C)
 *              data    == 0 or more data bytes
 *
 */

int pksizes[] = {
        1, 32, 64, 128, 256, 512, 1024, 2048, 4096, 1
};

#define GETRIES 5
/*
 * Pseudo-dma byte collection.
 */

pkgetpack(ipk)
struct pack *ipk;
{
        int ret, k, tries;
        register char *p;
        struct pack *pk;
        struct header *h;
        unsigned short sum;
        int ifn;
        char **bp;
        char hdchk;

        if (Conbad > CONBAD /* || Ntimeout > NTIMEOUT */)
                pkfail();
        pk = PADDR;
        ifn = pk->p_ifn;

        /* find HEADER */
        for (tries = 0; tries < GETRIES; ) {
                p = (caddr_t) &pk->p_ihbuf;
                if ((ret = pkcget(ifn, p, 1)) < 0) {
                        /* set up retransmit or REJ */
                        tries++;
                        pk->p_msg |= pk->p_rmsg;
                        if (pk->p_msg == 0)
                                pk->p_msg |= M_RR;
                        if ((pk->p_state & LIVE) == LIVE)
                                pk->p_state |= RXMIT;
                        pkoutput(pk);
                        continue;
                }
                if (*p != SYN)
                        continue;
                p++;
                ret = pkcget(ifn, p, HDRSIZ - 1);
                if (ret == -1)
                        continue;
                break;
        }
        if (tries >= GETRIES) {
                PKDEBUG(4, "tries = %d\n", tries);
                pkfail();
        }

        h = (struct header * ) &pk->p_ihbuf;
        p = (caddr_t) h;
        hdchk = p[1] ^ p[2] ^ p[3] ^ p[4];
        p += 2;
        sum = (unsigned) *p++ & 0377;
        sum |= (unsigned) *p << 8;
        h->sum = sum;
        PKDEBUG(7, "rec h->cntl %o\n", (unsigned) h->cntl);
        k = h->ksize;
        if (hdchk != h->ccntl) {
                /* bad header */
                PKDEBUG(7, "bad header %o,", hdchk);
                PKDEBUG(7, "h->ccntl %o\n", h->ccntl);
                Conbad++;
                return;
        }
        if (k == 9) {
                if (h->sum + h->cntl == CHECK) {
                        pkcntl(h->cntl, pk);
                        Conbad = 0;
                        PKDEBUG(7, "state - %o\n", pk->p_state);
                }
                else {
                        /*  bad header */
                        Conbad++;
                        PKDEBUG(7, "bad header %o\n", h->cntl);
                        pk->p_state |= BADFRAME;
                }
                return;
        }
        if (k && pksizes[k] == pk->p_rsize) {
                pk->p_rpr = h->cntl & MOD8;
                pksack(pk);
                Conbad = 0;
                bp = pk->p_ipool;
                pk->p_ipool = (char **) *bp;
                if (bp == NULL) {
                        PKDEBUG(7, "bp NULL %s\n", "");
                return;
                }
        }
        else {
                Conbad++;
                return;
        }
        ret = pkcget(pk->p_ifn, (char *) bp, pk->p_rsize);
        PKASSERT(ret != -1, "PKGETPKT CAN't READ %d", ret);
        pkdata(h->cntl, h->sum, pk, (char *) bp);
        return;
}


pkdata(c, sum, pk, bp)
char c;
short sum;
register struct pack *pk;
char **bp;
{
register x;
int t;
char m;

        if (pk->p_state & DRAINO || !(pk->p_state & LIVE)) {
                pk->p_msg |= pk->p_rmsg;
                pkoutput(pk);
                goto drop;
        }
        t = next[pk->p_pr];
        for(x=pk->p_pr; x!=t; x = (x-1)&7) {
                if (pk->p_is[x] == 0)
                        goto slot;
        }
drop:
        *bp = (char *)pk->p_ipool;
        pk->p_ipool = bp;
        return;

slot:
        m = mask[x];
        pk->p_imap |= m;
        pk->p_is[x] = c;
        pk->p_isum[x] = sum;
        pk->p_ib[x] = (char *)bp;
        return;
}



/*
 * setup input transfers
 */
pkrstart(pk)
{}

/*
 * Start transmission on output device associated with pk.
 * For asynch devices (t_line==1) framing is
 * imposed.  For devices with framing and crc
 * in the driver (t_line==2) the transfer is
 * passed on to the driver.
 */
pkxstart(pk, cntl, x)
struct pack *pk;
char cntl;
register x;
{
        register char *p;
        int ret;
        short checkword;
        char hdchk;

        p = (caddr_t) &pk->p_ohbuf;
        *p++ = SYN;
        if (x < 0) {
                *p++ = hdchk = 9;
                checkword = cntl;
        }
        else {
                *p++ = hdchk = pk->p_lpsize;
                checkword = pk->p_osum[x] ^ (unsigned)(cntl&0377);
        }
        checkword = CHECK - checkword;
        *p = checkword;
        hdchk ^= *p++;
        *p = checkword>>8;
        hdchk ^= *p++;
        *p = cntl;
        hdchk ^= *p++;
        *p = hdchk;
 /*  writes  */
PKDEBUG(7, "send %o\n", (unsigned) cntl);
        p = (caddr_t) & pk->p_ohbuf;
        {
                char buf[PACKSIZE*2 + HDRSIZ*2], *b;
                int i, k = 0;
                b = buf;
                k = pkq7copy( &b, p, HDRSIZ );
                if( x >= 0 )
                        k += pkq7copy( &b,  pk->p_ob[x], pk->p_rsize );
                GENERROR(buf, k);
                ret = write(pk->p_ofn, buf, k);
                PKASSERT(ret == k, "PKXSTART ret %d", ret);
        }
        if (pk->p_msg)
                pkoutput(pk);
        return;
}


pkmove(p1, p2, count, flag)
char *p1, *p2;
int count, flag;
{
        char *s, *d;
        int i;
        if (flag == B_WRITE) {
                s = p2;
                d = p1;
        }
        else {
                s = p1;
                d = p2;
        }
        for (i = 0; i < count; i++)
                *d++ = *s++;
        return;
}


/***
 *      pkcget(fn, b, n)        get n characters from input
 *      char *b;                - buffer for characters
 *      int fn;                 - file descriptor
 *      int n;                  - requested number of characters
 *
 *      return codes:
 *              n - number of characters returned
 *              0 - end of file
 */

jmp_buf Getjbuf;
cgalarm() { longjmp(Getjbuf, 1); }

pkcget(fn, b, n)
int fn, n;
char *b;
{
        int nchars, ret;

        if (setjmp(Getjbuf)) {
                Ntimeout++;
                PKDEBUG(4, "alarm %d\n", Ntimeout);
                return(-1);
        }
        signal(SIGALRM, cgalarm);

        if( !sevenbits )
                for (nchars = 0; nchars < n; nchars += ret) {
                        alarm(PKTIME);
                        ret = read(fn, b, n - nchars);
                        if (ret == 0) {
                                alarm(0);
                                return(-1);
                        }
                        PKASSERT(ret > 0, "PKCGET READ %d", ret);
                        b += ret;
                }
        else
        {       
                char buf[PACKSIZE*2+HDRSIZ*2], *bf, *kb, byte;
                int bl = 0;
                for( kb = b+n ; b<kb ; )
                {   
                        alarm(PKTIME);
                        ret = read(fn, buf, (kb-b)*2);
                        if (ret == 0) {
                                alarm(0);
                                return(-1);
                        }
                        PKASSERT(ret > 0, "PKCGET READ %d", ret);
                        bf = buf;
                        for( ; ret ; ret--, bf++ )
                        {
                                *bf &= 0177;
                                if( *bf == ':' )
                                        *b++ = SYN;
                                else
                                {      if( *bf < 'a' )
                                                byte = *bf - '0';
                                       else
                                                byte = *bf - 'a' + 10;
                                       if( bl )
                                       {
                                                bl = 0;
                                                *b++ = *b | byte;
                                       }
                                       else
                                       {
                                                bl++;
                                                *b = byte << 4;
                                       }
                                }
                         }
                }
        }
        alarm(0);
        return(0);
}


generror(p, s)
char *p;
int s;
{
        int r;
        if (Errorrate != 0 && (rand() % Errorrate) == 0) {
                r = rand() % s;
fprintf(stderr, "gen err at %o, (%o), ", r, (unsigned) *(p + r));
                *(p + r) += 1;
        }
        return;
}


pkq7copy(b, p, s)
char **b;
register char *p;
int s;
{
        register int i;
        register char *b1 = *b;
        register int k = 0;

        if( sevenbits )
                for( i=0 ; i<s ; i++, k += 2, p++ )
                {
                        if( *p == SYN )
                        {       *b1++ = ':';
                                k--;
                                continue;
                        }
                        *b1++ = hex[ (*p >> 4) & 017 ];
                        *b1++ = hex[ *p & 017 ];
                }
        else if( qnetused )
                for( i=0 ; i<s ; i++, k++, p++ )
                {
                        if( ((*b1++ = *p) & 0177) == 2 )
                        {       
                                *b1++ = *p;
                                k++;
                        }
                }
        else
                for( i=0 ; i<s ; i++, k++ )
                        *b1++ = *p++;
        *b = b1;
        return(k);
}
