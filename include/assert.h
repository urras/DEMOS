#ifndef _assert_h_
#       define  _assert_h_

#ifndef NDEBUG
#include <stdio.h>
#include <ediag.h>

static char *_ass_fmt[2] = {
"Assertion failed: file %s, line %d\n",
"Утверждение ложно: файл %s, стр. %d\n"
};
# define _assert(ex) {if(!(ex))fprintf(stderr,_ass_fmt[_ediag],__FILE__,__LINE__);}
# define assert(ex)  {if(!(ex))fprintf(stderr,_ass_fmt[_ediag],__FILE__,__LINE__),exit(1);}
#  else
# define _assert(ex)     ;
# define assert(ex)      ;
#endif

#endif  _assert_h_
