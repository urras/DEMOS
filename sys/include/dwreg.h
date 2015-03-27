#ifndef _sys_dwreg_h_
#       define  _sys_dwreg_h_

struct dwdevice {
	short   dw_rid;         /* регистр идентификации          */
	short   dw_m1;          /* unused                         */
	short   dw_err;         /* регистр ошибок/предкомпенсации */
	short   dw_sec;         /* номер сектора                  */
	short   dw_data;        /* регистр данных                 */
	short   dw_cyl;         /* номер цилиндра                 */
	short   dw_trk;         /* номер поверхности              */
	short   dw_rks2;        /* РКС 2                          */
	short   dw_stat;        /* регистр состояния/установки    */
};

/* bits in dw_err; старший байт только читается, младший - только пишется */
/* bits 15-8 содержат коды ошибок */
/* bit 15 is unused */
#define DWER_CCD       0040000          /* ошибка КЦК данных */
#define DWER_CCA       0020000          /* ошибка КЦК адреса */
#define DWER_NXS       0010000          /* ошибка поиска адреса */
/* bit 11 is unused */
#define DWER_ERR       0002000          /* неисправность */
#define DWER_SKE       0001000          /* ошибка позиционирования */
#define DWER_MKD       0000400          /* ошибка поиска маркера данных */
/* В bits 7-0 записывается адрес цилиндра,
   с которого начинается предкомпенсация тока записи */
#define DWER_BITS       "\10\17CCD\16CCA\15NXS\13ERR\12SKE\11MKD"

/* bits in dw_rks2; старший байт только читается, младший - только пишется */
/* bits 15-8 отражает состояние устройства после операции */
/* bit 15 is unknown */
#define DWCS_DRDY       0040000         /* накопитель готов */
#define DWCS_WRE        0020000         /* ошибка записи */
#define DWCS_INI        0010000         /* установка завершена */
#define DWCS_ZOB        0004000         /* запрос данных 2 */
/* bits 9-10 are unused */
#define DWCS_ERR        0000400         /* ошибка */
/* биты 7-0 содержат код операции  -- commands */
#define DWCS_SEEK0      0000020         /* позиционирование на 0 дорожку */
#define DWCS_RCOM       0000040         /* read */
#define DWCS_WCOM       0000060         /* write */
#define DWCS_FORMAT     0000120         /* формат */
#define DWCS_BITS       "\10\17RDY\16WRE\15INIY1\14ZOB\11ERR"

/* bits in dw_stat */
#define DWDS_BSY        0100000         /* занят */
/* bits 14-9 are unused */
#define DWDS_TYP        0000400         /* тип накопителя */
#define DWDS_ZOA        0000200         /* запрос данных 1 */
#define DWDS_IE         0000100         /* разрешение прерывания */
/* bits 5-4 are unused */
#define DWDS_INI        0000010         /* начальная установка */
/* bits 2-1 are unused */
#define DWDS_ORDY       0000001         /* операция завершена */

#endif  _sys_dwreg_h_
