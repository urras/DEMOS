/* ���������� ����� � ���������� */

/*
 * $Log:        gettune.c,v $
 * Revision 2.2  89/12/03  13:42:39  rygoff
 * ����� �������� ������ ���������� ����������, 
 * ������ �������� phead.
 * ������ ����������.
 * 
 * Revision 2.1  89/10/30  19:42:03  rygoff
 * ����� �����.
 * ������ ������ � ����� � ������ �������������.
 * 
 * Revision 1.2  89/09/03  17:01:44  rygoff
 * ����� ��� ������ � ��������� ������� (show_type) 
 * ��� ��� ��� ��������� �� ������ � lookcom, 
 * �� � � main � ����� � ���������� ���������� ����������
 * (��. simultaneous ������ � main.c)
 * 
 * Revision 1.1  89/08/19  17:54:08  rygoff
 * Initial revision
 * 
 *
 */

#include "comint.h"
#include <tty_codes.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <sys/signal.h>
#include <setjmp.h>
#include <sys/utsname.h>
#include <ar.h>
#include <a.out.h>

boolean ch_name();
char   *cvmacro();

static char     *begdescr, *endloc, *enddescr;
static unsigned sdev, sino, udev, uino;

int   thead;                  /* ����: �� ���������� ����� */
int   uhead;                  /* ����: �� ���������� ��� ������������ */
int   mhead;                  /* ����: �� ���������� ����� */
int   phead;                  /* ����: �� ���������� �������� */
char *nhead;                  /* ����� ��� ��������� (���� �� 0) */
int   invisible;              /* ������ ������� ��� ��������� */


gettune() {
    struct stat    st;
    register char *p;
    char           buf[80];   /* ����� ��� ����� ��������� �������� */

    if( begdescr==0 ) {
        begdescr = endloc = enddescr = (char*)sbrk(2);
        /*
           rygoff 22.06.89 ������� ������������ ������ -
                �� /usr/lib ������� � /lib
                ����� �������� � single-user mode ��� /usr
        */
        sgettune( p = "/usr/local/lib/comint/tuner" );  endloc = begdescr;
        if( stat(p,&st)   != -1 ) {sdev = st.st_dev;  sino = st.st_ino;}

        strcpy( buf, getenv("HOME") );  strcat( buf, "/.comint" );
        sgettune( buf );  endloc = begdescr;
        if( stat(buf,&st) != -1 ) {udev = st.st_dev;  uino = st.st_ino;}
    }

    nhead = NULL;  mhead = uhead = thead = phead = NO;
    sgettune( ".comint" );
}


/* ��������� � ������ ������ ���� 'name' */

sgettune(          name )
    register char *name;
{
    register int fd, add, lgt;
    struct  stat st;


    if( (fd=open(name,0)) <= 0                  /* ���� ��� ������ ? */
    or  fstat(fd,&st) == -1
    or  (st.st_dev == sdev  &&  st.st_ino == sino )
    or  (st.st_dev == udev  &&  st.st_ino == uino )  ) {
        fd = -1;  lgt = 0;
    } else {
        lgt = (int)st.st_size;
    }
    move( endloc, begdescr, enddescr-endloc );  /* ������� ������ ���� */
    add = lgt - (endloc - begdescr);
    if( sbrk(add) == -1 )  goto er;
    endloc += add;  enddescr += add;
    move( begdescr, endloc, enddescr-endloc );  /* ������� ����� */
    read( fd, begdescr, lgt );
    *enddescr = 0;
    if( *begdescr == '/' )  gethead(begdescr);
er: close(fd);
}


/* ������������� ��������� */

gethead( p )  register char *p; {
    for( ; ; ++p ) {
        switch( *p ) {
            case 0:
            case '\n': return;
            case 't':  thead++;    break;
            case 'u':  uhead++;    break;
            case 'm':  mhead++;    break;
            case 'p':  phead++;    break;
            case 'n':  nhead=p+1;  return;
        }
    }
}


/* ������ ����� ����� */

static char   bname[D_NAME+1];
char *fname = bname;             /* ��� ��� �������� */
char *fsuff;                     /* ������� ����� */
char *ffull;                     /* ������ ��� ����� */
char  ftype;                     /* ��� ����� (n c s o a d ) */
char  ffirs;                     /* ������ ������ ����� */

/*static*/ filenm(name) char *name; {
    register char *p;

    ffull  = name;
    *fname = 0;
    fsuff  = "";
    ftype  = 0;

    if( name == NULL )  return;

    if( p=rindex(name,'/') )  ++p;         /* �������� � ����� ������� */
    else                      p = name;
    strncpy(fname,p,D_NAME);
    if( (p=rindex(fname,'.')) && p[1] && p!=fname) {*p = 0; fsuff = p+1;}
}


/* ������ ���� ����� */

/*static*/ filetp() {
    register int   fd, lread;
    struct   stat  st;
    union {
        unsigned short word;
        char           byte[4];
    }              first;

    ffirs  = 0;

    if( ffull == NULL
    or  (    stat(ffull,&st) == -1
#ifdef BSD
        and lstat(ffull,&st) == -1
#endif
    )   ) {
        ftype = 'n';                                /* ���������� ��� */

    } else if( (st.st_mode & S_IFMT)==S_IFDIR ) {
        ftype = 'c';                                        /* ������� */

#ifdef S_IFLNK
    } else if( (st.st_mode & S_IFMT)==S_IFLNK ) {
        ftype = 'l';                                        /* symlink */
#endif

#ifdef S_IFQUOT
    } else if( (st.st_mode & S_IFMT)==S_IFQUOT ) {
        ftype = 'q';                                        /* quota */
#endif

    } else {
        fd = open(ffull,0);
        first.word = 0;
        lread = read( fd, &first, sizeof first );
        close(fd);

        if( first.word == ARMAG
            || !strncmp(first.byte,"!<ar",4)
            || !strncmp(first.byte,"<ar>",4)
          ) {
            ftype = 'a';                                 /* ���������� */
#ifdef SYSV
        } else if( ISCOFF(first.word) ) {
#else
        } else if( first.word==A_MAGIC1  ||  first.word==A_MAGIC2
                || first.word==A_MAGIC3  ||  first.word==A_MAGIC4 ) {
#endif
            ftype = 'o';                                 /* ��������� */
        } else {
            ftype = 's';                                 /* ���������? */
            for( fd=lread; --fd>=0; )  {
                register char c;
                c = first.byte[fd];                      /* ������ */
                if( !is_print(c) && c!='\t' && c!='\n' )  ftype = 'd';
            }
        }

        ffirs = first.byte[0];                  /* ������ ������ ����� */
    }
}


/* ����� ������� �� ������ 'symb', ������� ����� ����� 'name' */

char *lookcom( symb, name )
    int        symb;
    char            *name;
{
    register char    *p;
    register int      level;
    register boolean  ifhead;

    filenm(name);
    invisible = 0;
    level = 0;
    ifhead = NO;

lk: for( p=begdescr; *p; ) {
        while( *p == ' ' )  ++p;                    /* ������ �������� */
        switch( *p ) {
            case '*':
            case '\n':  goto nx;                    /* ����������� */
            case '(':                               /* IF */
                if( level )  {++level;  goto nx;}
                ifhead = YES;
                filenm(cvmacro(p[1]));
                break;
            case '<':
                if( level )  {++level;  goto nx;}
                if( symb != cvsymb(p+1) )  level = 1;
                goto nx;
            case '+':                               /* ELSE */
                if(      level==1 )  level = 0;
                else if( level==0 )  level = 1;
                goto nx;
            case ')':                               /* ENDIF */
                if( level )  --level;
                goto nx;
            default:                                /* ������� ������ */
                if( level )  goto nx;
                if( symb != cvsymb(p) )  goto nx;
                break;
        }
        if( level )  goto nx;               /* ������� ������ IF'� */
        while( *p!=' ' ) ++p;
        while( *p==' ' ) ++p;

        if( *p == '=' ) {                   /* ��������������� ������� */
            symb = cvsymb( p+2 );
            goto lk;
        }

        if(      *p=='_' )  ++p;            /* �������� �� ����� */
        else if( *p!=' ' ) {
            register int n;
            n = nmarked + nalt;
            if(      *p == 'n' )  {if( n != 0 )        goto nx;}
            else if( *p == 'y' )  {if( n == 0 )        goto nx;}
            else                  {if( n != *p -'0' )  goto nx;}
            ++p;
        }

        if(      *p=='_' )  ++p;            /* ��� ����� */
        else if( *p!=' ' ) {
            if( ftype==0 )  filetp();
            if( *p++ != ftype )  goto nx;
        }

        if(      *p=='_' )  ++p;            /* ������ ����� */
        else if( *p!=' ' and access(ffull,*p++ -'0')==-1 )  goto nx;

        if(      *p=='_' )  ++p;            /* ������ ������ ����� */
        else if( *p!=' ' ) {
            if( ftype==0 )  filetp();
            if( *p++ != ffirs )  goto nx;
        }

        if(      *p=='_' )  ++p;            /* ��� ����� */
        else if( *p!=' ' and not ch_name(p,ffull) )  goto nx;

        while( *p!=' ' ) ++p;               /* ������� � ���� ������� */
        while( *p==' ' ) ++p;

        if( not ifhead )                          /* �� - ������� */
            return(p);
        goto fnxl;                            /* ��������� ������� */

nx:     if( ifhead ) {                        /* �� ��������� ������� */
            level = 1;
fnxl:       filenm(name);
            ifhead = NO;
        }
        while( *p++ != '\n' ) {};             /* ������� � ����.������ */
    }
    return(0);
}


/* ��������� ������� 'sample' � ������ ����� 'name' � ������ '*' � '?' */

/*static*/ boolean ch_name( sample, name )
    register char      *sample,*name;
{
#   define sample_end    ' '
#   define name_end      '\00'
#   define is_sample_end (*sample == sample_end)
#   define is_name_end   (*name   == name_end)
#   define OK            return(YES)
#   define FAIL          return(NO)

    for( ; not is_sample_end; ++sample, ++name ) {
        switch( *sample ) {
          case '*':
            if( sample[1] == sample_end )  OK; /* ������ '*' � ������� */
            ++sample;
            while( not is_name_end ) {
                if( ch_name( sample, name ) )  OK;
             /* if( *name == '/' )  FAIL;       /* '*' �� �����. � '/' */
                ++name;
            }
            FAIL;
          case '?':
            if( is_name_end )  FAIL;
            break;
          default:
            if( *name != *sample )  FAIL;
            break;
        }
    }
    return(is_name_end);
}


/* �������������� ������� �� ������ 'pp' �� ���������� ���� � ��� */

/*static*/ int cvsymb( pp )
    register char *pp;
{
    register int   c;
    register char *p;
    static   int   last_c;
    static   char  ico[] = {
        'i','c', ic,   'd','c', dc,   'i','l', il,   'd','l', dl,
        'l','f', lf,   'r','n', rn,   'r','e', 32,   'b','l', bl,
        'e','s', es,   'e','l', el,   't','a', ta,   'd','e', 32,
        'f','i', s_finish,  'h','e', s_help,   's','p', s_spec,
        'e','r', s_erase,   'b','r', s_break,  'd','b', s_debug,
        'p','d', pd,   'p','u', pu,
        0
    };

    if( *pp=='.' )        return( last_c );         /* . */
    c = 0;
    if( *pp=='_' )       {c=0x0200; ++pp;}          /* _a ... */
    if( *pp=='!' )       {c=0x0100; ++pp;}          /* spec */
    if( *pp=='\'' )      {c += pp[1] & 0x1f;}       /* control */
    else if( *pp=='\\' ) {c += pp[1];}              /* escape */
    else {
        for( p=ico; *p; p+=3 ) {                    /* ������. */
            if(*p==*pp && p[1]==pp[1])  {c+=p[2]; goto fn;}
        }
        c += *pp;
    fn: ;
    }
    return( last_c = c );
}


char *show_type(p) char *p; {       /* rygoff 03.09.89 */
    static   char  im[] = ":,\001'";             /* ��� ������ */
    register char *s;
    invisible = 2;
    if( s=index(im,*p) )  {++p;  invisible = s - im;}
    return( p );
}



#ifdef BSD
extern char *getwd();
#else
char *getwd(s)
register char *s;
{
    register char *p;
    register int i, j;
    int pipdes[2], k;

    pipe(pipdes);
    if ((i = fork()) == 0) {
        close(1);
        dup(pipdes[1]);
        execl("/bin/pwd", "pwd", 0);
        execl("/usr/bin/pwd", "pwd", 0);
        dps("comint: pwd failed!");
        exit(1);
    }
    while( (j=wait(&k))!=-1 && j!=i )
        ;
    if(k)
        return((char*)0);
    read(pipdes[0], s, 50);
    p = s;
    while(*p != '\n')
        p++;
    *p = '\0';
    close(pipdes[0]);
    close(pipdes[1]);
    return(s);
}
#endif

char *getuname()
{
    struct utsname name;
    struct utsname *p = &name;

    p->nodename[0] = p->sysname[0] = '\0';


    if( uname(p) == EFAULT )
        return("localhost");
    else
    if( ! strlen(p->nodename) )
        return(p->nodename);
    else
        return(p->sysname);
}





extern jmp_buf on_break;
extern boolean Paint;
extern int INSYMB;

void onbreak() {
    signal(SIGINT,SIG_IGN);
    if( (INSYMB=dpa()) < 0 ) Paint=YES;
    longjmp( on_break, 1 );
}


