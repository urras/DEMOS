/*
 * Конфигурация мультиплексоров DH (СМ 8514).
 *
 * $Log:	dh.h,v $
 * Revision 1.3  88/06/29  11:08:07  korotaev
 * Вставлен DH_IOCTL.
 * 
 * Revision 1.2  88/04/02  17:05:20  korotaev
 * Изменения касались некоторых режимов по-умолчанию.
 * 
 * Revision 1.1  86/04/19  15:02:01  avg
 * Initial revision
 *
 *
 * NDH and NDM are in units of boards (16 lines each).
 * LOWDM is the unit number of the first unit with a DM-11
 * (e.g. 16 if the first DH has no DM, the second does have one).
 * All units from LOWDM through LOWDM + (NDM*16) are assumed to have
 * modem control (bit 0200 must be on in
 * their minor device numbers if DH_SOFTCAR is defined).
 */
#define NDH             %NDH%
#define NDM             %NDM%
#define LOWDM           %LOWDM%
#define DH_SOFTCAR
#define DH_IOCTL
/* #define DH_SILO      */
