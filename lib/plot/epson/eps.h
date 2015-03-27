/*
 * Параметры для печати графиков на Epson-80
 */

#define NXS     500     /* Число точек по X */
#define NYS     500     /* Число точек по Y */

#define PINS 7          /* число используемых игл */

#define NFL    ((NYS+PINS-1)/PINS)

char _Field[NFL][NXS];
int  _Color;

#define SP(x,y)     _Field[(y)/PINS][x] |=   1<<(PINS-1-((y)%PINS))
#define CP(x,y)     _Field[(y)/PINS][x] &= ~(1<<(PINS-1-((y)%PINS)))

#define SETPIX(x,y) {if(_Color) SP(x,NYS-1-(y)); else CP(x,NYS-1-(y));}
