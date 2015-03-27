#define KOI8
# define to_lower(c) (((c)>='A' && (c)<='Z')?((c)+('a'-'A')):((c)>=0340&&(c)<=0377? (c)-040:(c)))
# define to7bits(c)     ((c)&0177)
# define to8bits(c)     ((c)> 0100 && (c) < 0137? (c) + 0200 : (c) )

#ifndef NOCYRFILES
#define file_name(s) s
#else
extern  char    *file_name();
#endif
