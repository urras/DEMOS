/* НАБОР ПРОГРАММ ДЛЯ РАБОТЫ С ЭКРАНОМ ТЕРМИНАЛА

Tочки входа:
   dpbeg()     инициализация экранного режима
   dpend()     завершение экранного режима
   {тип}=dpd() сообщает параметры терминала:
                  xdim    размер экрана по горизонтали
                  ydim    размер экрана по вертикали
                  tflag   флаги возможностей терминала
   dpo(c)      вывод одного символа
   c=dpi()     ввод одного символа
   c=dpa()     асинхронный ввод одного символа,
                  <0 если символ не был введен
   dpp(x,y)    установ курсора в позицию x,y
   dps(s)      вывод строки символов до '\0'
   dpn(n,c)    вывод 'n' раз символа 'c'
*/

#ifndef    tty_codes_h
#   define tty_codes_h

struct s_dpd   {int xdim, ydim, nf, nb, tflag;}  dpd();
struct s_dpp   {int xpos, ypos;}         dpp();
struct s_color {char cf, cb;};
int    dpbeg(), dpend(), dpo(), dpi(), dpa(), dps(), dpn();

/* КОДИРОВКА СПЕЦСИМВОЛОВ ТЕРМИНАЛА */

#define ctrl(x) (x&037)
#define no_symb (-1)         /* ошибка, символа нет */
#define MAXREPT 255          /* макс. коэффициент повторения */
#define s_spc   0x100        /* признак, что введен s_spec */
#define spec(x) ((x)|s_spc)  /* введен s_spec + символ */

#define bl ctrl('g')  /* гудок */
#define ta ctrl('i')  /* табуляция */
#define lf ctrl('j')  /* перевод курсора в 0-ую поз. след. строки */
#define rn ctrl('m')  /* перевод курсора в 0-ую поз. текущ.строки */

#define ho ctrl('o')  /* то же, что il (см. ниже) */
#define fo ctrl('p')  /* fo,n   - включить шрифт #n */
#define el ctrl('q')  /* стирание конца строки */
#define rd ctrl('r')  /* роллирование экрана вниз (опция) */
#define ru ctrl('s')  /* роллирование экрана вверх */
#define cd ctrl('t')  /* сдвиг курсора вниз   */
#define cu ctrl('u')  /* сдвиг курсора вверх  */
#define cr ctrl('v')  /* сдвиг курсора вправо */
#define cl ctrl('w')  /* сдвиг курсора влево  */
#define ic ctrl('x')  /* вставка  символа (опция) */
#define dc ctrl('y')  /* удаление символа (опция) */
#define il ctrl('z')  /* вставка  строки (опция) */
#define dl ctrl('[')  /* удаление строки (опция) */
#define es ctrl('\\') /* стирание конца экрана */
#define ps ctrl(']')  /* ps,x,y - позиционирование в (x,y) */

/* Эти коды не нужны. Их лучше использовать для других целей
#define rp ctrl('^')  rp,n,c - n раз выводит символ c
#define wt ctrl('_')  wt,n   - задержка на 0.1*n сек
*/
#define pu ctrl('^')  /* предыдущая страница */
#define pd ctrl('_')  /* следующая страница */

#define de 010      /* забой */

#define s_debug  ctrl('d')  /* вызов отладчика */
#define s_break  ctrl('c')  /* аварийный конец работы */

#define s_erase  '`'      /* уничтожение объекта */
#define s_refuse es       /* отказ от действия */
#define s_finish ctrl('f')  /* конец работы */
#define s_spec   ps       /* префикс к составным символам */

/* КОДЫ ДЛЯ PC */

#define f1    0200
#define f2    0201
#define f3    0202
#define f4    0203
#define f5    0204
#define f6    0205
#define f7    0206
#define f8    0207
#define f9    0210
#define f10   0211
#define f11   0212
#define f12   0213

#define s_help   f1       /* префикс к составным символам */
/* ПСЕВДОГРАФИКА

#define h_l  0
#define v_l  1
#define b_l  2
#define b_j  3
#define b_r  4
#define l_j  5
#define c_j  6
#define r_j  7
#define t_l  8
#define t_j  9
#define t_r 10

#define g_sl    0       * простые линии *
#define g_dl    1       * двойные линии *
#define g_dhsv  2       * дв.горизонтальные, пр.вертикальные *
#define g_shdv  3       * пр.горизонтальные, дв.вертикальные *
#define g_arrw  4       * стрелки *
#define g_sq    5       * квадраты *
*/

#define hl1   0200
#define hl2   0201
#define vl1   0202
#define vl2   0203
#define bl1   0204
#define bl2   0205
#define bl21  0206
#define bl12  0207
#define bj1   0210
#define bj2   0211
#define bj21  0212
#define bj12  0213
#define br1   0214
#define br2   0215
#define br21  0216
#define br12  0217
#define lj1   0220
#define lj2   0221
#define lj21  0222
#define lj12  0223
#define cj1   0224
#define cj2   0225
#define cj21  0226
#define cj12  0227
#define rj1   0230
#define rj2   0231
#define rj21  0232
#define rj12  0233
#define tl1   0234
#define tl2   0235
#define tl21  0236
#define tl12  0237
#define tj1   0240
#define tj2   0241
#define tj21  0242
#define tj12  0243
#define tr1   0244
#define tr2   0245
#define tr21  0246
#define tr12  0247
#define ar0   0250
#define ar1   0251
#define ar2   0252
#define ar3   0253
#define ar4   0254
#define ar5   0255
#define ar6   0256
#define ar7   0257
#define ar8   0260
#define ar9   0261
#define sq0   0262
#define sq1   0263
#define sq2   0264
#define sq3   0265
#define sq4   0266
#define sq5   0267
#define sq6   0270

/* МАКРОСЫ ДЛЯ АНАЛИЗА СИМВОЛОВ */

#define is_digit(c)   ((c)>='0' && (c)<='9')
#define is_control(c) ((unsigned)(c) < ' ')
#if ('А'&037) != ('A'&037)
#   define is_lower(c) (((c)&0140)==0140)
#   define is_upper(c) (((c)&0140)==0100)
#   define is_rus(c)   (((c)&0300)==0300)
#   define is_print(c) (((c)>=' ' && (unsigned)(c)<0177) || is_rus(c))
#   define to_lower(c) ((c)|040)
#   define to_upper(c) ((c)&(~040))
#else
#   define is_lower(c) ( ((c)&0200) ? !((c)&040) : (((c)&0140)==0140) )
#   define is_upper(c) ( ((c)&0200) ?  ((c)&040) : (((c)&0140)==0100) )
#   define is_rus(c)   ( ((c)&0300)==0300 && ((c)&0377)!=0377 )
#   define is_print(c) ( ((c)>=' ' && (c)<0177) || is_rus(c) )
#   ifdef INTERNATIONAL
#     define to_lower(c) ( ((c)&0200) ? (c)+'а'-'А' : (c)+'a'-'A' )
#     define to_upper(c) ( ((c)&0200) ? (c)-'а'+'А' : (c)-'a'+'A' )
#   else
#     define to_lower(c) ( ((c)&0200) ? (c)+0xc1-0xe1 : (c)+0x61-0x41 )
#     define to_upper(c) ( ((c)&0200) ? (c)-0xc1+0xe1 : (c)-0x61+0x41 )
#   endif
#endif

/* ФЛАГИ ВОЗМОЖНОСТЕЙ ТЕРМИНАЛА */

#define a_icdc   1   /* можно выводить ic-dc */
#define a_ildl   2   /* можно выводить il-dl */
#define a_rd     4   /* можно выводить rd */
#define a_lower  8   /* есть маленькие буквы */
#define a_font  16   /* есть разные шрифты */
#define a_c1    32   /* признак установки цвета C1 */
#define a_c2    64   /* признак установки цвета C2 */
#define a_gr   128   /* есть графический режим */

/* ФЛАГИ УПРАВЛЕНИЯ РЕДАКТОРОМ СТРОКИ*/

#define on_control   1   /* конец по любому контр. символу*/
#define on_border    2   /* конец по выходу за границы стр*/
#define on_space     4   /* конец по пробелу, табуляции,ic*/
#define on_icdc      8   /* конец по вставке, удалению сим*/
#define on_no_digit 16   /* конец по не цифре */
#define MS_mode     32   /* включить режим вставки (grb) */

/* Названия цветов */

#define black 0
#define blue 1
#define green 2
#define cyan 3
#define red 4
#define magenta 5
#define brown 6
#define white 7
#define gray 8
#define lt_blue 9
#define lt_green 10
#define lt_cyan 11
#define lt_red 12
#define lt_magenta 13
#define yellow 14
#define hi_white 15

#endif
