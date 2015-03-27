#ifndef _ctype_h_
#       define  _ctype_h_

#define _U      01      /* upper */
#define _L      02      /* lower */
#define _N      04      /* numeric (0-9) */
#define _S      010     /* space */
#define _P      020     /* punct */
#define _C      040     /* control */
#define _X      0100    /* xdigit (a-f|A-F) */
#define _R      0200    /* cyrill */

#define _Z(c)   ((c)&0377)

extern  char    _ctype_[];      /* in /usr/src/lib/c/gen/ctype_.c */

#define isalpha(c)      ((_ctype_+1)[_Z (c)]&(_U|_L))
#define isupper(c)      ((_ctype_+1)[_Z (c)]&_U)
#define islower(c)      ((_ctype_+1)[_Z (c)]&_L)
#define isdigit(c)      ((_ctype_+1)[_Z (c)]&_N)
#define isxdigit(c)     ((_ctype_+1)[_Z (c)]&(_N|_X))
#define isspace(c)      ((_ctype_+1)[_Z (c)]&_S)
#define ispunct(c)      ((_ctype_+1)[_Z (c)]&_P)
#define isalnum(c)      ((_ctype_+1)[_Z (c)]&(_U|_L|_N))
#define isprint(c)      ((_ctype_+1)[_Z (c)]&(_P|_U|_L|_N))
#define iscntrl(c)      ((_ctype_+1)[_Z (c)]&_C)
#define isascii(c)      (!((c)&~0377))
#define toupper(c)      ((c)^040)
#define tolower(c)      toupper(c)
#define toascii(c)      _Z (c)

#define isrus(c)        ((_ctype_+1)[_Z (c)]&_R)
#define islat(c)        (!((_ctype_+1)[_Z (c)]&(_N|_S|_P|_C|_R)))

#define iscyrill(c)     isrus(c)        /* DEMOS COMPATIBLE */
#define islatin(c)      islat(c)

#endif _ctype_h_
