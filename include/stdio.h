#ifndef _stdio_h_
#       define  _stdio_h_

#define UCB_LINEBUF

#define BUFSIZ  512
#define _NFILE  20
#ifndef FILE
extern  struct  _iobuf {
	char    *_ptr;
	int     _cnt;
	char    *_base;
#ifdef  UCB_LINEBUF
	short   _flag;
#else
	char    _flag;
#endif
	char    _file;
} _iob[_NFILE];
# endif

#define _IOREAD 01
#define _IOWRT  02
#define _IONBF  04
#define _IOMYBUF        010
#define _IOEOF  020
#define _IOERR  040
#define _IOSTRG 0100
#define _IORW   0200
#ifdef  UCB_LINEBUF
#define _IOLBF  0400
#endif  UCB_LINEBUF

#ifndef NULL
#define NULL    0
#endif
#define FILE    struct _iobuf
#define EOF     (-1)

#define stdin   (&_iob[0])
#define stdout  (&_iob[1])
#define stderr  (&_iob[2])
#define getc(p)         (--(p)->_cnt>=0? *(p)->_ptr++&0377:_filbuf(p))
#define getchar()       getc(stdin)
#define putc(x,p) (--(p)->_cnt>=0? (*(p)->_ptr++=(x))&0377:_flsbuf((x),p))
#define putchar(x)      putc(x,stdout)
#define feof(p)         (((p)->_flag&_IOEOF)!=0)
#define ferror(p)       (((p)->_flag&_IOERR)!=0)
#define fileno(p)       ((p)->_file)
#ifdef  UCB_LINEBUF
#define setlinebuf(p)   ((p)->_flag |= _IOLBF)
#endif  UCB_LINEBUF

FILE    *fopen();
FILE    *fdopen();
FILE    *freopen();
long    ftell();
char    *fgets();

#endif  _stdio_h_
