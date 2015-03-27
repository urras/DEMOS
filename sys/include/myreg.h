#ifndef _sys_myreg_h_
#       define  _sys_myreg_h_

struct mydevice {
	short   my_csr;         /* регистр команд */
	short   my_dat;         /* регистр данных */
};

/* bits in my_csr */
#define MY_ERR          0100000         /* ошибка */
#define MY_RESET        0040000         /* установка в исх.состояние */
/* bits 13-8 содержат РАШ массива параметров */
#define MY_TR           0000200         /* требование данных */
#define MY_IE           0000100         /* разрешение прерывания */
#define MY_DONE         0000040         /* готовность */
/* bits 4-1 содержат команду */
#define MY_GO           0000001         /* старт */

/* commands */
#define MY_READ         0000000         /* чтение */
#define MY_WRITE        0000002         /* запись */
#define MY_READL        0000004         /* чтение с меткой */
#define MY_WRITL        0000006         /* запись с меткой */
#define MY_READT        0000010         /* чтение дорожки */
#define MY_READH        0000012         /* чтение заголовка */
#define MY_FORMAT       0000014         /* формат */
#define MY_SEEK         0000016         /* позиционирование */
#define MY_SET          0000020         /* установка параметров */
#define MY_RSTATE       0000022         /* чтение регистра состояния */
#define MY_LOAD         0000036         /* загрузка */

/* биты регистра состояния (после чтения его в my_dat) */
#define MYDS_WLO        0000001         /* защита записи или crc по чтению */
#define MYDS_CRC        0000002         /* crc по чтению зоны заголовка */
#define MYDS_INI        0000004         /* завершение начальной установки */
#define MYDS_SK0E       0000010         /* ошибка уст-ки на 0-дорожку */
#define MYDS_SKE        0000020         /* ошибка поиска дорожки */
#define MYDS_NXS        0000040         /* ошибка поиска сектора */
#define MYDS_LAB        0000100         /* операция над сектором с меткой */
#define MYDS_ROT        0000200         /* дискета вращается */
/* биты 8-9 указывают номер выбранного накопителя */
/* бит 10 указывает номер выбранной поверхности */
#define MYDS_NXM        0004000         /* несуществующий адрес */
#define MYDS_MKA        0010000         /* нет маркера адреса */
#define MYDS_MKD        0020000         /* нет маркера данных */
#define MYDS_FMT        0040000         /* нестандартная разметка */
#define MYDS_ERR        0100000         /* ошибка контроллера */

#define MYDS_BITS \
"\10\20ERR\17FMT\16MKD\15MKA\14NXM\10ROT\7LAB\6NXS\5SKE\4SK0E\3INI\2CSE\1WLO"

#endif  _sys_myreg_h_
