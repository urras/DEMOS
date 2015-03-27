/* === CDEBUGGER === ГЛОБАЛЬНЫЕ ДАННЫЕ === include file */
/*
 * Автор        Флеров М.В.
 * 19.02.86     Руднев А.П. - вставлен режим SEPID для Электроники-79
 */
#include <stdio.h>
extern int errno;                       /* флаг системных ошибок         */

#ifndef SEPID
#define get_iword get_word
#endif

/* описатели состояния отлаживаемой программы */
char *pcmn;                             /* указатель текущей команды     */
int  last_line;                         /* номер последней показанной стр*/
int  act_line;                          /* строка, которая будет выполн. */
int  act_adr;                           /* адрес останова в C-программе  */
FILE *act_file;                         /* дескриптор отлажив.текст.файла*/
int  regval[9];                         /* содержимое регистров          */
int  stp_cause;                         /* причина передачи упр.отладчику*/
int  jsr_ign;                           /* флаг - входить ли в подпрогр. */
int  stp_adr;                           /* адрес стопора (0-нет, 1-пуск) */
int  dsp_inp, dsp_out;                  /* дисплей для ввода и вывода    */
int  language;                          /* язык программирования модуля  */

enum {C, FORTRAN};


/* расположение переменных в функции 'cdebug' относительно 'cdebflag' */
#define SR0   -16
#define SR1   -14
#define SR2   -12
#define SR3   -10
#define SR4    -8
#define SR5    -6
#define SSP    -4
#define SPC    -2
int     CDEBFLAG;
#define PFRAME  2
#define STPADR  4
#define FROM    6
#define LENG    8
#define VALUE  10
