#ifndef _timeb_h_
#       define  _timeb_h_
#include <sys/types.h>

/*
 *       Структура возвращаемая ftime().
 */

struct timeb{
	time_t   time;    /* Время в секундах по Гривничу от
			     1-Янв-70 00:00:00                  */
	unsigned millitm; /* Время в милисекундах ( всегда 0 )  */
	short    timezone;/* Местная временная зона ( в минутах
			     в западном направлении от Гривнича */
	short    dstflag; /* Флаг летнего времени               */
};

#endif  _timeb_h_
