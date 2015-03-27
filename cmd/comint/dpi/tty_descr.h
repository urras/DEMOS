/* СТРУКТУРА ОПИСАНИЯ ТЕРМИНАЛА */

#ifndef    tty_descr_h
#   define tty_descr_h
#   ifndef       tty_codes_h
#       include "tty_codes.h"
#   endif

#undef NULL
#define NULL ((char*)0)
                                     /* КОДИРОВКА УПРАВЛЯЮЩИХ СИМВОЛОВ */
#define cnt     0
#define esc_cnt 1
#define esc     2
#define prf     3
#define prf_cnt 4
#define esc_sgn 5

#define c(x) (('x'&037)+040*cnt)     /*          control-буква  */
#define e(x) (('x'&037)+040*esc)     /* escape + буква          */
#define d(x) (('x'&037)+040*esc_sgn) /* escape + знак (напр. еsc-8) */
#define s(x) (('x'&037)+040*esc_cnt) /* escape + control-буква  */
#define P(x) (('x'&037)+040*prf)     /* префикс + символ       +суффикс*/
#define C(x) (('x'&037)+040*prf_cnt) /* префикс + contr-символ +суффикс*/

static struct s_tdescr {          /* СТРУКТУРА ОПИСАНИЯ ТЕРМИНАЛА */
    char         sp_inp[32];          /* коды спец-символов при вводе */
    char         sp_fnc[12];          /* коды функциональных клавиш */
    char         sp_out[16];          /* коды спец-символов при выводе */
    struct s_dpd param;               /* параметры терминала */
    struct s_color color;
    char        *beg,  *end;          /* строки иниц. и заверш. или 0 */
    char        *pref, *suff;         /* префикс и суффикс или 0 */
    char        *sdpp;                /* строка позиционирования или 0 */
    char         xdpp, ydpp, ldpp;    /*   координаты в ней и ее длина */
    char         escape;              /* код escape или 0 */
    char        *scf;                 /* описатель установки цвета Cf */
    char        *scb;                 /* описатель установки цвета Cb  */
    char        *smf;                 /* описатель цвета Mf */
    char        *smb;                 /* описатель цвета Mb */
    char        *gr_beg, *gr_end;     /* включение и выкл. псевдографики */
    char        *s_g[6];              /* описатели псевдограф. символов */
};

#endif
