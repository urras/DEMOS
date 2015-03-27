/*
 *  link editor
 */

#include <stdio.h>
#include <signal.h>
#ifdef sparc
#include "/home/mellorn/zaitcev/d22/include/ar.h"
#include "/home/mellorn/zaitcev/d22/include/a.out.h"
#else
#include <ar.h>
#include <a.out.h>
#endif
/* В Демосе symbol называется nlist и живет в a.out.h */
/* #include <symbol.h>. --P3 */
#define symbol   nlist
#define sy_type  n_type
#define sy_name  n_name
#define sy_value n_value
#include <sys/types.h>
#include <sys/stat.h>

/* Handle values in PDP-11 byte order */
#ifdef sparc
#define GET16(v)   ((((v)&0377)<<8)+((v)>>8&0377))
#define PUT16(v)   ((((v)&0377)<<8)+((v)>>8&0377))
#define GET32(v)   ((((long)(v)&0xFF0000)<<8)+((long)(v)>>8&0xFF0000)\
			+(((v)&0377)<<8)+((v)>>8&0377))
#define PUT32(v)   ((((long)(v)&0xFF0000)<<8)+((long)(v)>>8&0xFF0000)\
			+(((v)&0377)<<8)+((v)>>8&0377))
#else
#define GET16(v)   (v)
#define PUT16(v)   (v)
#define GET32(v)   (v)
#define PUT32(v)   (v)
#endif

/*      Layout of a.out file :
 *
 *      header of 8 words       magic number 405, 407, 410, 411
 *                              text size       )
 *                              data size       ) in bytes but even
 *                              bss size        )
 *                              symbol table size
 *                              entry point
 *                              {unused}
 *                              flag set if no relocation
 *
 *
 *      header:         0
 *      text:           16
 *      data:           16+textsize
 *      relocation:     16+textsize+datasize
 *      symbol table:   16+2*(textsize+datasize) or 16+textsize+datasize
 *
 */
#define TRUE    1
#define FALSE   0


#define N_COMM  05      /* internal use only */

#define R_ABS   00
#define R_TEXT  02
#define R_DATA  04
#define R_BSS   06
#define R_EXT   010

#define NOVLY   16
#define RELFLG  01
#define NROUT   256
#define NSYM    1103
#define NSYMPR  1000
#define MSW 100

char    premeof[] = "Premature EOF";
char    goodnm[] = "__.SYMDEF";

/* table of contents stuff */
#define TABSZ   700
struct tab
{       char cname[8];
        long cloc;
} tab[TABSZ];
int tnum;


/* overlay management */

struct swtab {struct symbol *sw_sy, **sw_hs;}
        swittab[MSW], *swsy = swittab, *swend = swittab + MSW;

unsigned Oswb, Oswe;

int     vindex;
struct overlay {
        int     symsav;
        struct liblist  *libsav;
        char    *vname;
        int     tsave;
        struct swtab *swsave;
} vnodes[NOVLY];

/* input management */
struct page {
        int     nuser;
        int     bno;
        int     nibuf;
        short   buff[256];
} page[2];

struct {
        int     nuser;
        int     bno;
} fpage;

struct stream {
        short   *ptr;
        int     bno;
        int     nibuf;
        int     size;
        struct page     *pno;
};

struct stream text;
struct stream reloc;

struct ar_xhdr {
	long	x_date;
	long	x_size;
	char	x_name[16];
	short	x_mode;
};

struct ar_xhdr archdr;
struct exec filhdr;


/* one entry for each archive member referenced;
 * set in first pass; needs restoring for overlays
 */
struct liblist {
        long    loc;
};

struct liblist  liblist[NROUT];
struct liblist  *libp = liblist, *libp0 = liblist;



struct local {
        int locindex;           /* index to symbol in file */
        struct symbol *locsymbol;       /* ptr to symbol table */
};

struct symbol   cursym;                 /* current symbol */
struct symbol   symtab[NSYM];           /* actual symbols */
struct symbol   **symhash[NSYM];        /* ptr to hash table entry */
struct symbol   *lastsym;               /* last symbol entered */
int     symindex;               /* next available symbol table entry */
int     symbegin;               /* begin of non-allocated symbols    */
struct symbol   *hshtab[NSYM+2];        /* hash table for symbols */
struct local    local[NSYMPR];

/* internal symbols */
struct symbol   *p_etext;
struct symbol   *p_edata;
struct symbol   *p_end;
struct symbol   *entrypt;

int     trace;
/* flags */
int     xflag;          /* discard local symbols */
int     Xflag;          /* discard locals starting with 'L' */
int     Sflag;          /* discard all except locals and globals*/
int     rflag;          /* preserve relocation bits, don't define common */
int     arflag;         /* original copy of rflag */
int     sflag;          /* discard all symbols */
int     nflag;          /* pure procedure */
int     Oflag;          /* set magic # to 0405 (overlay) */
int     dflag;          /* define common even with rflag */
int     iflag;          /* I/D space separated */
int     vflag;          /* overlays used */
int     vlflag;         /* Последний узел в overlay дереве */
int     ofilfnd;
char    *ofilename = "l.out";
int     infil;
char    *filname;

/* cumulative sizes set in pass 1 */
int     tsize;
int     dsize;
int     bsize;
int     ssize;

/* symbol relocation; both passes */
int     ctrel;
int     cdrel;
int     cbrel;

int     errlev;
int     delarg  = 4;
char    tfname[] = "/tmp/ldaXXXXX";


/* output management */
struct buf {
        int     fildes;
        int     nleft;
        short   *xnext;
        short   iobuf[256];
};
struct buf      toutb;
struct buf      doutb;
struct buf      troutb;
struct buf      droutb;
struct buf      soutb;

struct symbol   **lookup();
struct symbol   **slookup();
struct symbol   *lookloc();

tdelexit() { delarg = 4; delexit();}
delexit()
{
        unlink("l.out");
        if (delarg==0)
                chmod(ofilename, 0777 & ~umask(0));
        else exit(delarg);
}

main(argc, argv)
char **argv;
{
	int c;
        register int i;
        int num,c0=0,newoflag=0;
        register char *ap, **p;
        int found;
        int vscan;
        char save;

        if (signal(SIGINT, SIG_IGN) != SIG_IGN)
                signal(SIGINT, tdelexit);
        if (signal(SIGTERM, SIG_IGN) != SIG_IGN)
                signal(SIGTERM, tdelexit);
        if (argc == 1)
                exit(4);
        p = argv+1;

        /* scan files once to find symdefs */
        for (c=1; c<argc; c++) {
                if (trace) printf("%s:\n", *p);
                filname = 0;
                ap = *p++;

                if (*ap == '-') {
                        for (i=1; ap[i]; i++) {
                        switch (ap[i]) {
                        case 'o':
                                if (++c >= argc)
                                        error(2, "No output file");
                                ofilename = *p++;
				if (*ofilename == '-')
                                        error(2, "Bad output file");
                                ofilfnd++;newoflag++;
                                continue;

                        case 'u':
                        case 'e':
                                if (++c >= argc)
                                        error(2, "Bad 'use' or 'entry'");
                                enter(slookup(*p++));
                                if (ap[i]=='e')
                                        entrypt = lastsym;
                                continue;
                        case 'V': vlflag = 2;goto vbeg;
                        case 'v': vlflag=0;
vbeg:                           if (++c >= argc)
                                        error(2, "-v: arg missing");
                                if(vflag && (!newoflag))
                                        error(2,"-v without -o ");
                                if(iflag) error(2,"-i is not allow when -v yet");
                                endload(c-c0, argv + c0);
                                vscan = vindex; 
                                found=FALSE;
                                while (found==FALSE && --vscan>=0 )
                                        found = eq(vnodes[vscan].vname, *p);
                                if (found) {
                                        restore(vscan);
                                } else
                                        record(c, *p);
                                p++; c0 = c; vflag =TRUE; nflag = newoflag = 0;
                                if(rflag) error(2, "can not make rellocative -v file");
                                continue;
                        case 'D':
                                if (++c >= argc)
                                        error(2, "-D: arg missing");
                                num = atoi(*p++);
                                if (dsize>num)
                                        error(2, "-D: too small");
                                dsize = num;
                                continue;

                        case 'l':
                                save = ap[--i]; 
                                ap[i]='-';
                                load1arg(&ap[i]); 
                                ap[i]=save;
                                break;

                        case 'x':
                                xflag++;
                                continue;

                        case 'X':
                                Xflag++;
                                continue;

                        case 'S':
                                Sflag++; 
                                continue;

                        case 'r':
                                rflag++;
                                arflag++;
                                continue;

                        case 's':
                                sflag++;
                                xflag++;
                                continue;

                        case 'n':
                                nflag++;
                                continue;

                        case 'd':
                                dflag++;
                                continue;

                        case 'i':
                                iflag++;
                                continue;

                        case 'O':
                                Oflag++;
                                continue;

                        case 't':
                                trace++;
                                continue;

                        default:
                                error(2, "bad flag");
                        } /*endsw*/
                        break;
                        } /*endfor*/ /* по символам флага */
                } else
                        load1arg(ap);
        } /*endfor*/ /* по аргументам */
        endload(argc-c0, argv+c0);   /* уходим на второй проход */
        exit(0);
}

/* used after pass 1 */
int     nsym;
int     torigin;
int     dorigin;
int     borigin;
int     tbegin,bend;

/* 2-й проход: слияние и настройка */
endload(argc, argv)
int argc; 
char **argv;
{
        register int c, i; 
        int dnum;
        register char *ap, **p;
        filname = 0;

	/* Разные интересные вещи: */
	/* 1. commons, "_end"                   */
	/* 2. смещения по оверлейным сегментам  */
	/* 3. прибавление базы сегментов        */
        middle();

        setupout();
        p = argv+1;
        libp = libp0;
        for (c=1; c<argc; c++) {
                ap = *p++;
                if (trace) printf("%s:\n", ap);
                if (*ap == '-') {
                        for (i=1; ap[i]; i++) {
                        switch (ap[i]) {
                        case 'D':
                                for (dnum = atoi(*p); dorigin<dnum; dorigin += 2) {
                                        putw(0, &doutb);
                                        if (rflag)
                                                putw(0, &droutb);
                                }
                        case 'u':
                        case 'e':
                        case 'o':
                        case 'v':case 'V':
                                ++c; 
                                ++p;

                        default:
                                continue;

                        case 'l':
                                ap[--i]='-'; 
                                load2arg(&ap[i]);
                                break;
                        } /*endsw*/
                        break;
                        } /*endfor*/
                } else
                        load2arg(ap);
        }
        finishout();
}

record(c, nam)
int c; 
char *nam;
{
        register struct overlay *v;
        v = &vnodes[vindex++];
        if(vindex > NOVLY ) error(2,"too deep overlays");
        symbegin =   v->symsav = symindex;
        libp0 = v->libsav = libp;
        v->vname = nam;
        tsize = dsize = bsize =  ssize =0;
        ctrel = cdrel = cbrel = tsize = bsize = dsize = ssize =0;
        tbegin = v->tsave = bend;
        fiOswit("_O_switc","_O_swend");
        v->swsave = swsy;
}

restore(vscan)
int vscan;
{
        register struct overlay *v;
        register int saved;

        v = &vnodes[vscan];
        vindex = vscan+1;
        libp0 = libp = v->libsav;
        ctrel = cdrel = cbrel = tsize = bsize = dsize = ssize =0;
        symbegin = saved = v->symsav;
        tbegin = v->tsave;
        while (symindex>saved)
                *symhash[--symindex]=0;
        while( swsy > v->swsave )
        { swsy--;
          *(swsy->sw_hs) = swsy->sw_sy;
        }
}

/* scan file to find defined symbols */
load1arg(acp)
char *acp;
{
        register char *cp;
        long nloc;

        cp = acp;
        switch ( getfile(cp)) {
        case 0:
                load1(0, 0L);
                break;

        /* regular archive */
        case 1:
                nloc = 1;
                while ( step(nloc))
                        nloc += (archdr.x_size + sizeof(struct ar_hdr) + 1) >> 1;
                break;

        /* table of contents */
        case 2:
                tnum = archdr.x_size / sizeof(struct tab);
                if (tnum >= TABSZ) {
                        error(2, "fast load buffer too small");
                }
                lseek(infil,(long)(sizeof(filhdr.a_magic)+sizeof(struct ar_hdr)), 0);
                read(infil, (char *)tab, tnum * sizeof(struct tab));
#ifdef sparc
		{
			register n;
			for (n = 0; n < tnum; n++) {
				tab[n].cloc = GET32(tab[n].cloc);
			}
		}
#endif
                while (ldrand());
                libp->loc = -1;
                libp++;
                break;
        /* out of date table of contents */
        case 3:
                error(0, "out of date (warning)");
                for(nloc = 1+((archdr.x_size+sizeof(struct ar_hdr)+1) >> 1); step(nloc);
                        nloc += (archdr.x_size + sizeof(struct ar_hdr) + 1) >> 1);
                break;
        }
        close(infil);
}

step(nloc)
long nloc;
{
	struct ar_hdr ab;
        dseek(&text, nloc, sizeof(struct ar_hdr));
        if (text.size <= 0) {
                libp->loc = -1;
                libp++;
                return(0);
        }
        mget((short *)&ab, sizeof ab);
#ifdef sparc
	archdr.x_date = ((long)GET16(ab.ar_date[0])<<16)
			 + (unsigned)GET16(ab.ar_date[1]);
	archdr.x_size = ((long)GET16(ab.ar_size[0])<<16)
			 + (unsigned)GET16(ab.ar_size[1]);
#else
	archdr.x_date = ab.ar_date;
	archdr.x_size = ab.ar_size;
#endif
	archdr.x_mode = GET16(ab.ar_mode);
	memcpy(archdr.x_name, ab.ar_name, 14);
        if (load1(1, nloc + sizeof(struct ar_hdr) / 2)) {
                libp->loc = nloc;
                libp++;
        }
        return(1);
}

ldrand()
{
        int i;
        struct symbol *sp, **pp;
        struct liblist *oldp = libp;
        for(i = 0; i<tnum; i++) {
                if ((pp = slookup(tab[i].cname)) == 0)
                        continue;
                sp = *pp;
                if (sp == 0 || sp->sy_type != N_EXT+N_UNDF)
                        continue;
                step(tab[i].cloc >> 1);
        }
        return(oldp != libp);
}

add(a,b,s)
int a, b;
char *s;
{
        long r;

        r = (long)(unsigned)a + (unsigned)b;
        if (r >= 0200000)
                error(1,s);
        return(r);
}


/* single file or archive member */
load1(libflg, loc)
long loc;
{
        register struct symbol *sp;
        int savindex;
        int ndef, nloc, type, mtype;

        readhdr(loc);
        ctrel = tsize;
        cdrel += dsize;
        cbrel += bsize;
        ndef = 0;
        nloc = sizeof cursym;
        savindex = symindex;
        if ((filhdr.a_flag&RELFLG)==1) {
                error(1, "No relocation bits");
                return(0);
        }
        loc += (sizeof filhdr)/2 + filhdr.a_text + filhdr.a_data;
        dseek(&text, loc, filhdr.a_syms);
        while (text.size > 0) {
                mget((int *)&cursym, sizeof cursym);
                cursym.n_type = GET16(cursym.n_type);
                cursym.n_value = GET16(cursym.n_value);
                type = cursym.sy_type;
                if (Sflag) {
                        mtype = type&037;
                        if (mtype==1 || mtype>4) {
                                continue;
                        }
                }
                if ((type&N_EXT)==0) {
                        if (Xflag==0 || cursym.sy_name[0]!='L')
                                nloc += sizeof cursym;
                        continue;
                }
                symreloc();
                if (enter(lookup()))
                        continue;
                if ((sp = lastsym)->sy_type != N_EXT+N_UNDF)
                    {
                        if(!vflag || libflg || sp->sy_type != N_EXT+N_TEXT)
                                 continue;
                        if((unsigned)(sp->sy_value) < Oswb || (unsigned)(sp->sy_value) > Oswe)
                                 continue;
                        if(cursym.sy_type != N_EXT + N_TEXT &&
                                (!vlflag || cursym.sy_type != N_EXT + N_UNDF)
                                         ) continue;
                        if(swsy == swend) error(2,"overlay switch overflow");
                        swsy->sw_sy = sp;
                        *(swsy->sw_hs =  symhash[sp-symtab]) = 0;
                        if( !enter(swsy->sw_hs)) error(2,"Overlay system error");
                        swsy++;
                        continue;
                }
                if (cursym.sy_type == N_EXT+N_UNDF) {
                        if (cursym.sy_value > sp->sy_value)
                                sp->sy_value = cursym.sy_value;
                        continue;
                }
                if (sp->sy_value != 0 && cursym.sy_type == N_EXT+N_TEXT)
                        continue;
newdef:         ndef++;
                sp->sy_type = cursym.sy_type;
                sp->sy_value = cursym.sy_value;
        }
        if (libflg==0 || ndef) {
                tsize = add(tsize,filhdr.a_text,"text overflow");
                dsize = add(dsize,filhdr.a_data,"data overflow");
                bsize = add(bsize,filhdr.a_bss,"bss overflow");
                ssize = add(ssize,nloc,"symbol table overflow");
                return(1);
        }
        /*
         * No symbols defined by this library member.
         * Rip out the hash table entries and reset the symbol table.
         */
        while (symindex>savindex)
                *symhash[--symindex]=0;
        return(0);
}

middle()
{
        register struct symbol *sp, *symp;
        register t, csize;
        int nund, corigin;

        torigin=tbegin;
        dorigin=0; 
        borigin=0;

        p_etext = *slookup("_etext");
        p_edata = *slookup("_edata");
        p_end = *slookup("_end");
        /*
         * If there are any undefined symbols, save the relocation bits.
         */
        symp = &symtab[symindex];
        if(vflag && p_etext)
                   { p_etext->sy_value =0;p_etext->sy_type = N_EXT+N_UNDF;}
        if(vflag && p_edata)
                   { p_edata->sy_value =0;p_edata->sy_type = N_EXT+N_UNDF;}
        if(vflag && p_end)
                   { p_end->sy_value =0;p_end->sy_type = N_EXT+N_UNDF;}
        if (rflag==0) {
                for (sp = symtab + symbegin; sp<symp; sp++)
                        if (sp->sy_type==N_EXT+N_UNDF && sp->sy_value==0
                                && sp!=p_end && sp!=p_edata && sp!=p_etext) {
                                rflag++;
                                dflag = 0;
                                break;
                        }
        }
        if (rflag)
                nflag = sflag = iflag = Oflag = 0;
        /*
         * Assign common locations.
         */
        csize = 0;
        if (dflag || rflag==0) {
                ldrsym(p_etext, tsize, N_EXT+N_TEXT);
                ldrsym(p_edata, dsize, N_EXT+N_DATA);
                ldrsym(p_end, bsize, N_EXT+N_BSS);
                for (sp = symtab; sp<symp; sp++)
                        if (sp->sy_type==N_EXT+N_UNDF && (t = sp->sy_value)!=0) {
                                t = (t+1) & ~01;
                                sp->sy_value = csize;
                                sp->sy_type = N_EXT+N_COMM;
                                csize = add(csize, t, "bss overflow");
                        }
        }
        /*
         * Now set symbols to their final value
         */
        if (nflag || iflag)
                tsize = (tsize + 077) & ~077;
        dorigin = tsize + torigin;
        if (nflag)
                dorigin = (dorigin+017777) & ~017777;
        if (iflag)
                dorigin = 0;
        corigin = dorigin + dsize;
        borigin = corigin + csize;
        bend = borigin + bsize;
        nund = 0;
        if(vflag){
             if(p_etext && p_etext<symtab+symbegin) p_etext->sy_value += torigin;
             if(p_edata && p_edata<symtab+symbegin) p_edata->sy_value += dorigin;
             if(p_end   && p_end<symtab+symbegin) p_end  ->sy_value += borigin;
        }
        for (sp = symtab+symbegin; sp<symp; sp++) switch (sp->sy_type) {
        case N_EXT+N_UNDF:
                if (arflag==0 && sp->sy_value==0) {
                        errlev |= 01;
                        if (nund==0)
                                printf("%s: Undefined:\n",ofilename);
                        nund++;
                        printf("%.8s\n", sp->sy_name);
                }
                continue;

        case N_EXT+N_ABS:
        default:
                continue;

        case N_EXT+N_TEXT:
                sp->sy_value += torigin;
                continue;

        case N_EXT+N_DATA:
                sp->sy_value += dorigin;
                continue;

        case N_EXT+N_BSS:
                sp->sy_value += borigin;
                continue;

        case N_EXT+N_COMM:
                sp->sy_type = N_EXT+N_BSS;
                sp->sy_value += corigin;
                continue;
        }
        if (sflag || xflag)
                ssize = 0;
        bsize = add(bsize, csize, "bss overflow");
        nsym = ssize / (sizeof cursym);
}

ldrsym(asp, val, type)
struct symbol *asp;
{
        register struct symbol *sp;

        if ((sp = asp) == 0)
                return;
        if (sp->sy_type != N_EXT+N_UNDF || sp->sy_value) {
                printf("%.8s: ", sp->sy_name);
                error(1, "Multiply defined");
        if( ! vflag) return;
        }
        sp->sy_type = type;
        sp->sy_value = val;
}

setupout()
{
        tcreat(&toutb, 0);
        mktemp(tfname);
        tcreat(&doutb, 1);
        if (sflag==0 || xflag==0)
                tcreat(&soutb, 1);
        if (rflag) {
                tcreat(&troutb, 1);
                tcreat(&droutb, 1);
        }
        filhdr.a_magic = ((vflag || Oflag) ? A_MAGIC4 :( iflag ? A_MAGIC3 : ( nflag ? A_MAGIC2 : A_MAGIC1 )));
        filhdr.a_text = tsize;
        filhdr.a_data = dsize;
        filhdr.a_bss = bsize;
        filhdr.a_syms = sflag? 0: (ssize + (sizeof cursym)*symindex);
        if (entrypt) {
                if (entrypt->sy_type!=N_EXT+N_TEXT)
                        error(1, "Entry point not in text");
                else
                        filhdr.a_entry = entrypt->sy_value | 01;
        } else
                filhdr.a_entry=0;
        filhdr.a_unused = torigin;
        filhdr.a_flag = (rflag==0);
#ifdef sparc
	filhdr.a_magic = PUT16(filhdr.a_magic);
	filhdr.a_text = PUT16(filhdr.a_text);
	filhdr.a_data = PUT16(filhdr.a_data);
	filhdr.a_bss = PUT16(filhdr.a_bss);
	filhdr.a_syms = PUT16(filhdr.a_syms);
	filhdr.a_entry = PUT16(filhdr.a_entry);
	filhdr.a_flag = PUT16(filhdr.a_flag);
#endif
        mput(&toutb, (int *)&filhdr, sizeof filhdr);
}

tcreat(buf, tempflg)
struct buf *buf;
{
        register int ufd; 
        char *nam;
        nam = (tempflg ? tfname : ofilename);
        if ((ufd = creat(nam, 0666)) < 0)
                error(2, tempflg?"cannot create temp":"cannot create output");
        close(ufd); 
        buf->fildes = open(nam, 2);
        if (tempflg)
                unlink(tfname);
        buf->nleft = sizeof(buf->iobuf)/sizeof(int);
        buf->xnext = buf->iobuf;
}

load2arg(acp)
char *acp;
{
        register char *cp;
        register struct liblist *lp;

        cp = acp;
        if (getfile(cp) == 0) {
                while (*cp)
                        cp++;
                while (cp >= acp && *--cp != '/');
                mkfsym(++cp);
                load2(0L);
        } else {        /* scan archive members referenced */
                for (lp = libp; lp->loc != -1; lp++) {
			struct ar_hdr ab;
                        dseek(&text, lp->loc, sizeof(struct ar_hdr));
                        mget((int *)&ab, sizeof ab);
#ifdef sparc
			archdr.x_date = ((long)GET16(ab.ar_date[0])<<16)
					 + (unsigned)GET16(ab.ar_date[1]);
			archdr.x_size = ((long)GET16(ab.ar_size[0])<<16)
					 + (unsigned)GET16(ab.ar_size[1]);
#else
			archdr.x_date = ab.ar_date;
			archdr.x_size = ab.ar_size;
#endif
			archdr.x_mode = GET16(ab.ar_mode);
                        mkfsym(archdr.x_name);
                        load2(lp->loc + sizeof(struct ar_hdr) / 2);
                }
                libp = ++lp;
        }
        close(infil);
}

load2(loc)
long loc;
{
        register struct symbol *sp;
        register struct local *lp;
        register int symno;
        int type, mtype;

        readhdr(loc);
        ctrel = torigin;
        cdrel += dorigin;
        cbrel += borigin;
        /*
         * Reread the symbol table, recording the numbering
         * of symbols for fixing external references.
         */
        lp = local;
        symno = -1;
        loc += (sizeof filhdr)/2;
        dseek(&text, loc + filhdr.a_text + filhdr.a_data, filhdr.a_syms);
        while (text.size > 0) {
                symno++;
                mget((int *)&cursym, sizeof cursym);
                cursym.n_type = GET16(cursym.n_type);
                cursym.n_value = GET16(cursym.n_value);
                symreloc();
                type = cursym.sy_type;
                if (Sflag) {
                        mtype = type&037;
                        if (mtype==1 || mtype>4) continue;
                }
                if ((type&N_EXT) == 0) {
                        if (!sflag&&!xflag&&(!Xflag||cursym.sy_name[0]!='L')) {
                		cursym.n_type = PUT16(cursym.n_type);
                		cursym.n_value = PUT16(cursym.n_value);
                                mput(&soutb, (int *)&cursym, sizeof cursym);
			}
                        continue;
                }
                if ((sp = *lookup()) == 0)
                        error(2, "internal error: symbol not found");
		/* Bill Shannon fixed this line first. I got this from printed listing. --P3 */
                if (cursym.sy_type == N_EXT+N_UNDF || cursym.sy_type == N_EXT+N_TEXT) {
                        if (lp >= &local[NSYMPR])
                                error(2, "Local symbol overflow");
                        lp->locindex = symno;
                        lp++->locsymbol = sp;
                        continue;
                }
                if (cursym.sy_type!=sp->sy_type || cursym.sy_value!=sp->sy_value) {
                        printf("%.8s: ", cursym.sy_name);
                        error(1, "Multiply defined");
                }
        }
        dseek(&text, loc, filhdr.a_text);
        dseek(&reloc, loc + half(filhdr.a_text + filhdr.a_data), filhdr.a_text);
        load2td(lp, ctrel, &toutb, &troutb);
        dseek(&text, loc+half(filhdr.a_text), filhdr.a_data);
        dseek(&reloc, loc+filhdr.a_text+half(filhdr.a_data), filhdr.a_data);
        load2td(lp, cdrel, &doutb, &droutb);
        torigin += filhdr.a_text;
        dorigin += filhdr.a_data;
        borigin += filhdr.a_bss;
}

load2td(lp, creloc, b1, b2)
struct local *lp;
struct buf *b1, *b2;
{
        register r, t;
        register struct symbol *sp;

        for (;;) {
                /*
                         * The pickup code is copied from "get" for speed.
                         */

                /* next text or data word */
                if (--text.size <= 0) {
                        if (text.size < 0)
                                break;
                        text.size++;
                        t = get(&text);
                } else if (--text.nibuf < 0) {
                        text.nibuf++;
                        text.size++;
                        t = get(&text);
                } else
                        t = *text.ptr++;
#ifdef sparc
		t = GET16(t);
#endif

                /* next relocation word */
                if (--reloc.size <= 0) {
                        if (reloc.size < 0)
                                error(2, "Relocation error");
                        reloc.size++;
                        r = get(&reloc);
                } else if (--reloc.nibuf < 0) {
                        reloc.nibuf++;
                        reloc.size++;
                        r = get(&reloc);
                } else
                        r = *reloc.ptr++;
#ifdef sparc
		r = GET16(r);
#endif

                switch (r&016) {

                case R_TEXT:
                        t += ctrel;
                        break;

                case R_DATA:
                        t += cdrel;
                        break;

                case R_BSS:
                        t += cbrel;
                        break;

                case R_EXT:
                        sp = lookloc(lp, r);
                        if (sp->sy_type==N_EXT+N_UNDF) {
                                r = (r&01) + ((nsym+(sp-symtab))<<4) + R_EXT;
                                break;
                        }
                        t += sp->sy_value;
                        r = (r&01) + ((sp->sy_type-(N_EXT+N_ABS))<<1);
                        break;
                }
                if (r&01)
                        t -= creloc;
#ifdef sparc
		t = PUT16(t);
#endif
                putw(t, b1);
                if (rflag) {
#ifdef sparc
			r = PUT16(r);
#endif
                        putw(r, b2);
		}
        }
}

finishout()
{
        register n;
	register struct symbol *p;

        if (nflag||iflag) {
                n = torigin;
                while (n&077) {
                        n += 2;
                        putw(0, &toutb);
                        if (rflag)
                                putw(0, &troutb);
                }
        }
        copy(&doutb);
        if (rflag) {
                copy(&troutb);
                copy(&droutb);
        }
        if (sflag==0) {
                if (xflag==0)
                        copy(&soutb);
                for (p = symtab; p < &symtab[symindex]; p++) {
        		mput(&toutb, p->n_name, 8);
                        putw(PUT16(p->n_type), &toutb);
                        putw(PUT16(p->n_value), &toutb);
		}
        }
        flush(&toutb);
        close(toutb.fildes);
        if (!ofilfnd) {
                unlink("a.out");
                link("l.out", "a.out");
                ofilename = "a.out";
        }
        delarg = errlev;
        delexit();
}

copy(buf)
struct buf *buf;
{
        register n;
	register short *p;
	int f;

        flush(buf);
        lseek(f = buf->fildes, (long)0, 0);
        while ((n = read(f, (char *)doutb.iobuf, sizeof(doutb.iobuf))) > 1) {
                n >>= 1;
                p = doutb.iobuf;
                do {
                        putw(*p++, &toutb);
                } while (--n);
        }
        close(f);
}

mkfsym(s)
char *s;
{

        if (sflag || xflag)
                return;
        cp8c(s, cursym.sy_name);
        cursym.sy_type = PUT16(037);
        cursym.sy_value = PUT16(torigin);
        mput(&soutb, (int *)&cursym, sizeof cursym);
}

mget(aloc, an)
short *aloc;
{
        register short *loc, n;
        register short *p;

        n = an;
        n >>= 1;
        loc = aloc;
        if ((text.nibuf -= n) >= 0) {
                if ((text.size -= n) > 0) {
                        p = text.ptr;
                        do
                                *loc++ = *p++;
                        while (--n);
                        text.ptr = p;
                        return;
                } else
                        text.size += n;
        }
        text.nibuf += n;
        do {
                *loc++ = get(&text);
        } 
        while (--n);
}

mput(buf, aloc, an)
struct buf *buf; 
short *aloc;
{
        register short *loc;
        register n;

        loc = aloc;
        n = an>>1;
        do {
                putw(*loc++, buf);
        } 
        while (--n);
}

dseek(asp, aloc, s)
long aloc;
struct stream *asp;
{
        register struct stream *sp;
        register struct page *p;
        /* register */ long b, o;
        int n;

        b = aloc >> 8;
        o = aloc & 0377;
        sp = asp;
        --sp->pno->nuser;
        if ((p = &page[0])->bno!=b && (p = &page[1])->bno!=b)
                if (p->nuser==0 || (p = &page[0])->nuser==0) {
                        if (page[0].nuser==0 && page[1].nuser==0)
                                if (page[0].bno < page[1].bno)
                                        p = &page[0];
                        p->bno = b;
                        lseek(infil, (aloc & ~0377L) << 1, 0);
                        if ((n = read(infil, (char *)p->buff, 512)>>1) < 0)
                                n = 0;
                        p->nibuf = n;
        } else
                error(2, "No pages");
        ++p->nuser;
        sp->bno = b;
        sp->pno = p;
        sp->ptr = p->buff + o;
        if (s != -1)
                sp->size = half(s);
        if ((sp->nibuf = p->nibuf-o) <= 0)
                sp->size = 0;
}

half(i)
{
        return((i>>1)&077777);
}

get(asp)
struct stream *asp;
{
        register struct stream *sp;

        sp = asp;
        if (--sp->nibuf < 0) {
                dseek(sp, (long)(sp->bno + 1) << 8, -1);
                --sp->nibuf;
        }
        if (--sp->size <= 0) {
                if (sp->size < 0)
                        error(2, premeof);
                ++fpage.nuser;
                --sp->pno->nuser;
                sp->pno = (struct page *)&fpage;
        }
        return(*sp->ptr++);
}

/*
 * On return:
 *  0 - object file
 *  1 - regular archive
 *  2 - table of contents
 *  3 - out of date table of contents
 */
getfile(acp)
char *acp;
{
        register char *cp;
        register int c;
        struct stat x;

        cp = acp; 
        infil = -1;
        archdr.x_name[0] = '\0';
        filname = cp;
        if (cp[0]=='-' && cp[1]=='l') {
                if(cp[2] == '\0')
                        cp = "-la";
                filname = "/usr/lib/libxxxxxxxxxxxxxxx";
                for(c=0; c<13 && cp[c+2]; c++)
                        filname[c+12] = cp[c+2];
                filname[c+12] = '.';
                filname[c+13] = 'a';
                filname[c+14] = '\0';
                if ((infil = open(filname+4, 0)) >= 0) {
                        filname += 4;
                }
        }
        if (infil == -1 && (infil = open(filname, 0)) < 0)
                error(2, "cannot open");
        page[0].bno = page[1].bno = -1;
        page[0].nuser = page[1].nuser = 0;
        text.pno = reloc.pno = (struct page *)&fpage;
        fpage.nuser = 2;
        dseek(&text, 0L, 2);
        if (text.size <= 0)
                error(2, premeof);
        if(get(&text) != PUT16(ARMAG))
                return(0);      /* regular file */
        dseek(&text, 1L, sizeof(struct ar_hdr));  /* word addressing */
        if(text.size <= 0)
                return(1);      /* regular archive */
	{
		struct ar_hdr ab;
       		mget((int *)&ab, sizeof ab);
#ifdef sparc
		archdr.x_date = ((long)GET16(ab.ar_date[0])<<16)
				 + (unsigned)GET16(ab.ar_date[1]);
		archdr.x_size = ((long)GET16(ab.ar_size[0])<<16)
				 + (unsigned)GET16(ab.ar_size[1]);
#else
		archdr.x_date = ab.ar_date;
		archdr.x_size = ab.ar_size;
#endif
		archdr.x_mode = GET16(ab.ar_mode);
		memcpy(archdr.x_name, ab.ar_name, 14);
	}
        if(strncmp(archdr.x_name, goodnm, 14) != 0)
                return(1);      /* regular archive */
        else {
                fstat(infil, &x);
                if(x.st_mtime > archdr.x_date)
                {
                        return(3);
                }
                else return(2);
        }
}

struct symbol **lookup()
{
        int i; 
        int clash;
        register struct symbol **hp;
        register char *cp, *cp1;

        i = 0;
        for (cp = cursym.sy_name; cp < &cursym.sy_name[8];)
                i = (i<<1) + *cp++;
        for (hp = &hshtab[(i&077777)%NSYM+2]; *hp!=0;) {
                cp1 = (*hp)->sy_name; 
                clash=FALSE;
                for (cp = cursym.sy_name; cp < &cursym.sy_name[8];)
                        if (*cp++ != *cp1++) {
                                clash=TRUE; 
                                break;
                        }
                if (clash) {
                        if (++hp >= &hshtab[NSYM+2])
                                hp = hshtab;
                } else
                        break;
        }
        return(hp);
}

struct symbol **slookup(s)
char *s;
{
        cp8c(s, cursym.sy_name);
        cursym.sy_type = N_EXT+N_UNDF;
        cursym.sy_value = 0;
        return(lookup());
}

enter(hp)
struct symbol **hp;
{
        register struct symbol *sp;

        if (*hp==0) {
                if (symindex>=NSYM)
                        error(2, "Symbol table overflow");
                symhash[symindex] = hp;
                *hp = lastsym = sp = &symtab[symindex++];
                cp8c(cursym.sy_name, sp->sy_name);
                sp->sy_type = cursym.sy_type;
                sp->sy_value = cursym.sy_value;
                return(1);
        } else {
                lastsym = *hp;
                return(0);
        }
}

symreloc()
{
        switch (cursym.sy_type) {

        case N_TEXT:
        case N_EXT+N_TEXT:
                cursym.sy_value += ctrel;
                return;

        case N_DATA:
        case N_EXT+N_DATA:
                cursym.sy_value += cdrel;
                return;

        case N_BSS:
        case N_EXT+N_BSS:
                cursym.sy_value += cbrel;
                return;

        case N_EXT+N_UNDF:
                return;
        }
        if (cursym.sy_type&N_EXT)
                cursym.sy_type = N_EXT+N_ABS;
}

error(n, s)
char *s;
{
        if (errlev==0)
                printf("ld:");
        if (filname) {
                printf("%s", filname);
                if (archdr.x_name[0])
                        printf("(%.14s)", archdr.x_name);
                printf(": ");
        }
        printf("%s\n", s);
        if (n > 1)
                delexit();
        errlev = n;
}

struct symbol *
lookloc(alp, r)
struct local *alp;
{
        register struct local *clp, *lp;
        register sn;

        lp = alp;
        sn = (r>>4) & 07777;
        for (clp = local; clp<lp; clp++)
                if (clp->locindex == sn)
                        return(clp->locsymbol);
        error(2, "Local symbol botch");
}

readhdr(loc)
long loc;
{
        register st, sd;

        dseek(&text, loc, sizeof filhdr);
        mget((int *)&filhdr, sizeof filhdr);
#ifdef sparc
	filhdr.a_magic = GET16(filhdr.a_magic);
	filhdr.a_text = GET16(filhdr.a_text);
	filhdr.a_data = GET16(filhdr.a_data);
	filhdr.a_bss = GET16(filhdr.a_bss);
	filhdr.a_syms = GET16(filhdr.a_syms);
	filhdr.a_entry = GET16(filhdr.a_entry);
	filhdr.a_flag = GET16(filhdr.a_flag);
#endif
        if (filhdr.a_magic != A_MAGIC1)
                error(2, "Bad format");
        st = (filhdr.a_text+01) & ~01;
        filhdr.a_text = st;
        cdrel = -st;
        sd = (filhdr.a_data+01) & ~01;
        cbrel = - (st+sd);
        filhdr.a_bss = (filhdr.a_bss+01) & ~01;
}

fiOswit(begn,endn)
char *begn, *endn;
{ 
        register struct symbol *sp, *symp;
        symp = &symtab[symindex];
        for(sp=symtab;sp<symp;sp++)
        {       if(eq(begn,&(sp->sy_name[0]))) Oswb = (unsigned)(sp->sy_value);
                if(eq(endn,&(sp->sy_name[0]))) Oswe = (unsigned)(sp->sy_value);
         }
        if(!Oswb || !Oswe) error(0, "Overlay switch not found");
}

cp8c(from, to)
char *from, *to;
{
        register char *f, *t, *te;

        f = from;
        t = to;
        te = t+8;
        while ((*t++ = *f++) && t<te);
        while (t<te)
                *t++ = 0;
}

eq(s1, s2)
char *s1; 
char *s2;
{
        while (*s1==*s2++)
                if ((*s1++)==0)
                        return(TRUE);
        return(*s1==0);
}

putw(w, b)
register struct buf *b;
{
        *(b->xnext)++ = w;
        if (--b->nleft <= 0)
                flush(b);
}

flush(b)
register struct buf *b;
{
        register n;
        if ((n = (char *)b->xnext - (char *)b->iobuf) > 0)
                if (write(b->fildes, (char *)b->iobuf, n) != n)
                        error(2, "output error");
        b->xnext = b->iobuf;
        b->nleft = sizeof(b->iobuf)/sizeof(int);
}
