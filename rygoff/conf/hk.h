/*
 * Конфигурация дисков HK (СМ 5408).
 *
 * $Log:	hk.h,v $
 * Revision 1.2  88/04/02  17:08:00  korotaev
 * Изменения касались некоторых режимов по-умолчанию.
 * 
 * Revision 1.1  86/04/19  15:02:11  avg
 * Initial revision
 * 
 */
#define NHK     %NHK%

#define HKSEEK                          /* Параллельный подвод на неск. hk */
#define HK_DKN       2                  /* drive # for iostat disk monitoring */
#define HK_DUMP                         /* include dump routine */
#define HK_SPL                          /* Понижать приоритет, когда возможно*/
#define HK_TIMOUT                       /* Таймаут на HK --- @VG */
