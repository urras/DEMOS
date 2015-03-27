/*
 * Конфигурация RK дисков
 *
 * $Log:	rk.h,v $
 * Revision 1.3  88/04/02  17:11:49  korotaev
 * Изменения касались некоторых режимов по-умолчанию.
 * 
 * Revision 1.2  86/12/11  21:00:08  alex
 * Несколько контроллеров.
 * 
 * Revision 1.1  86/04/19  15:02:41  avg
 * Initial revision
 * 
 */

#define NRK     %NRK%
#define NRKC    %NRKC% /* Число контроллеров */
#define      RK_DKN  0 /* */
/* #define RK_WCHECK      /* Контроль записи */
#define RK_SPL         /* Устанавливать в rkintr низкий приоритет для kl */
