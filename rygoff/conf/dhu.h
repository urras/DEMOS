/*
 * Конфигурация мультиплексоров DHU
 *
 * $Log:	dhu.h,v $
 * Revision 1.1  88/01/04  20:49:38  korotaev
 * Initial revision
 * 
 * Revision 1.1  86/04/19  15:02:01  avg
 * Initial revision
 *
 *
 * NDHU and NDM are in units of boards (16 lines each).
 * LOWDM is the unit number of the first unit with a DM-11
 * (e.g. 16 if the first DH has no DM, the second does have one).
 * All units from LOWDM through LOWDM + (NDM*16) are assumed to have
 * modem control (bit 0200 must be on in
 * their minor device numbers if DH_SOFTCAR is defined).
 */
#define NDHU            %NDHU%
#define NDM             %NDM%
#define LOWDM           %LOWDM%
#define DH_SOFTCAR
#define DHU_IOCTL
