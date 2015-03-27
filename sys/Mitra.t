
  Структура ядра и его данных для машин Mitra

 При вызове syscall():

 syscall(dummy1, sp, r1, dummy2, dummy3, r0, pc, ps);   /* PDP-11 */
 syscall(rp, rl, ri, re, ra, rx);                    /* Mitra-225 */
