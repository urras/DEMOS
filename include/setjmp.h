#ifndef _setjmp_h_
#       define _setjmp_h_
/*
 * 4 слова - для оверлейных программ, неоверлейные
 * программы используют только 3 слова. @VG
 */
typedef int     jmp_buf[4];

#endif _setjmp_h_
