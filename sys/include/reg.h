#ifndef _sys_reg_h_
#       define  _sys_reg_h_

/*
 * Расположение сохраненных пользовательских регистров
 * в ситемено стеке относительно R0.
 * Используется как u.u_ar0[XX].
 */
#define R0      (0)
#define R1      (-3)
#define R2      (-11)
#define R3      (-10)
#define R4      (-9)
#define R5      (-7)
#define R6      (-4)
#define R7      (1)
#define PC      (1)
#define RPS     (2)

#endif  _sys_reg_h_
