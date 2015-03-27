/*
 * Конфигурация RP дисков
 *
 * $Log:	rp.h,v $
 * Revision 1.2  88/04/02  17:12:37  korotaev
 * Изменения касались некоторых режимов по-умолчанию.
 * 
 * Revision 1.1  86/04/19  15:02:49  avg
 * Initial revision
 * 
 */

#define NRP     0
#define RP_DKN       1
#define RP_WCHECK   /* Контроль записи */
#define RP_SPL      /* Устанавливать в rpintr низкий приоритет для kl */
#define RP_TIMEOUT  /* Таймоут */
