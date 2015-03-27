#ifndef setexit
/*
 * ������ �������� �� 4.2 BSD
 *
 * setexit/reset
 * ���������� �� � ������� setjump / longjump
 */
#include <setjmp.h>
jmp_buf rst_buf;

#define setexit() setjmp(rst_buf)
#define reset(v)    longjmp(rst_buf,v)

#endif
