#ifndef _varargs_h_
#       define  _varargs_h_

typedef char    *va_list;

#define va_dcl                  int va_alist;
#define va_start(list)          (list) = (char *) &va_alist
#define va_end(list)
#define va_arg(list,mode)       ((mode *)((list) += sizeof(mode)))[-1]

#endif  _varargs_h_
