#include "video.h"
#if NVI > 0

/*
 * Графические программы для растрового дисплея Электроники-85
 */

struct vireg {
	short   vi_rid;
	short   vi_m1;
	short   vi_rks;
	short   vi_rdc;
	short   vi_m2;
	short   vi_m3;
	short   vi_rsh;
	short   vi_ry;
	short   vi_rx;
	short   vi_rvl;
	short   vi_rbf;
	short   vi_rpa;
};

#define VIADDR ((struct vireg *)0174400)

/* параметры физ. сетки */

#define HX      5       /* шаг по x */
#define HY      2       /* шаг по y */

int     Gx, Gy;  /* позиция маркера */
int     Gfill;   /* режим заполнения */
int     Gbrf;    /* 1 - половинная яркость */
int     Gfx,Gfy; /* Координаты заполнения */
int     FillStyle; /* Стиль заполнения */
int     viinv;   /* Доп. маска для rbf */

int viR, viG, viB;      /* цвет */
int viMODE, vicolor;
int vireset;

/*
 * Рисование прямой с оптимизацией по векторам
 * Начальная точка - в Gx,Gy; концевая - в x2, y2.
 */
Gline( x2, y2 )
{
	int      d1, d2, d3, dd3;
	register dd1, dd2, l;
	int      x1, y1;
	int      dx, dy;
	int      a, b, D, Odir;
	int      sR, sG, sB;

	x1 = Gx; Gx = x2;
	y1 = Gy; Gy = y2;
	if( Gfill == 3 ) {      /* fill from (X,Y) */
		filltri( x1, y1, x2, y2 );
		return;
	}
	if( vicolor && Gbrf && Gfill == 0 ) {
		/* для линии половинной яркости фон - черный */
		sR   = viR; sG   = viG; sB   = viB;
		viR &= ~2;  viG &= ~2;  viR &= ~2;
		RDC(viMODE);
	}
	dx = (x1 > x2)? -1 : 1;
	dy = (y1 > y2)? -1 : 1;
	b = (x2 - x1);
	a = (y2 - y1);
	if( a < 0 ) a = -a;
	if( b > 0 ) b = -b;
	D = 0;
	Odir = 1;
	l = 0;
	while( x1 != x2 || y1 != y2 ) {
		if( vireset ) break;
		dd1 = d1 = D + a;
		dd2 = d2 = D + b;
		dd3 = d3 = D + a + b;
		if( dd1 < 0 ) dd1 = -dd1;
		if( dd2 < 0 ) dd2 = -dd2;
		if( dd3 < 0 ) dd3 = -dd3;
		if( dd1 < dd2 )
			dd1 = (dd3 < dd1)? 2: 0;
		else
			dd1 = (dd3 < dd2)? 2: 1;
		dd3 = 0;
		if( Gfill ) {   /* Нет необходимости строить векторы */
			if( Gfill == 1 ) { /* fill from X */
				if( dd1 != 0 ) vfillv( Gfx, x1, y1 );
			} else {           /* fill from Y */
				if( dd1 != 1 ) hfillv( x1, Gfy, y1 );
			}
		} else if( dd1 != Odir ) {
			if( Odir )
				vector( x1, y1-l, 1, l );
			else
				vector( x1-l, y1, 0, l );
			Odir = (dd1 != 0);
			l = 0;
		} else
			dd3++;
		switch( dd1 ) {
		    case 0:     /* смещение по X */
			x1 += dx;
			D = d1;
			if( dd3 ) l += dx;
			break;

		    case 1:     /* смещение по Y */
			y1 += dy;
			D = d2;
			if( dd3 ) l += dy;
			break;

		    case 2:     /* смещение по X и Y */
			x1 += dx;
			y1 += dy;
			D = d3;
		}
	}
	if( Gfill ) {
		if( Gfill == 1 ) { /* fill from X */
			vfillv( Gfx, x1, y1 );
		} else {           /* fill from Y */
			hfillv( x1, Gfy, y1 );
		}
	} else {
		if( Odir )
			vector( x1, y1-l, 1, l );
		else
			vector( x1-l, y1, 0, l );
		if( vicolor && Gbrf ) {
			viR = sR; viG = sR; viB = sB;
			RDC(viMODE);
		}
	}
}

/*
 * Вектор из x,y в напр. dir длины l яркость bf
 */
vector( x, y, dir, l )
register l, dir;
{
	register struct vireg *V = VIADDR;
	extern int Grks, cshift;
	int bf;

	bf = 052525;
	if( l < 0 ) {
		l = -l;
		if( dir ) y = y-l;
		else      x = x-l;
		if( l & 01 )
			bf = ~bf;
	}
	if( dir ) {     /* горизонтальный вектор */
		/* CLIPPING FOR SCREEN */
		if( y < 0 ) { l += y; y = 0; }
		if( y+l > 1023 ) l = 1023 - y;
		if( l < 0 || x < 0 || x > 255 ) return;

		if( Gbrf ) {
			bf = 052525;
			if( l > 0 && (l & 01) == 0 )
				bf = ~bf;
		} else {
			if( l == 0 )
				l = 1;
			bf = ~0;
		}
	} else {
		/* CLIPPING FOR SCREEN */
		if( x < 0 ) { l += x; x = 0; }
		if( x+l > 255 ) l = 255 - x;
		if( l < 0 || y < 0 || y > 1023 ) return;
	}
	while( !(V->vi_rks & 0100000) );
	if( !dir ) {
		Grks = 1;
		V->vi_rks |= 0400;
		V->vi_rbf = ~0 ^ viinv;
	} else {
		Grks = 0;
		V->vi_rks &= ~0400;
		V->vi_rbf = bf ^ viinv;
	}
	V->vi_rx  = x+cshift;
	V->vi_ry  = y;
	V->vi_rvl = l+1;
	if( !dir && !Gbrf ) {
		while( !(V->vi_rks & 0100000) );
		V->vi_rx  = x+cshift;
		V->vi_ry  = y+1;
		V->vi_rvl = l+1;
	}
}

int     ArcR;   /* Радиус окружности */
static  Cx, Cy; /* Координаты центра в сетке 1/HX, 1/HY */
static  Orient; /* Ориент. треугольника (нач.т., кон.т., центр) */

/*
 * Рисование дуги с радиусом ArcR от начальной точки до конечной
 * Если ArcR > 0 - по малой дуге окружности
 *           < 0 - по большой дуге
 * Радиус считается в горизонтальных единицах
 *
 * fl - true  - по часовой стрелке
 *      false - против часовой стрелки
 */
Garc( x2, y2, fl )
{
	int      x1, y1;

	if( ArcR > 10000 || ArcR < -10000 ) {
		Gline( x2, y2 );
	}
	x1 = Gx; Gx = x2;
	y1 = Gy; Gy = y2;

	/* Выбрать ориентацию треугольника */
	Orient = (ArcR < 0) ^ (fl!=0);

	/* Compute location (Cx,Cy) of center */
	center( x1, y1, x2, y2 );

	/* Draw arc */
	arc( x1, y1, x2, y2, fl?-1:1 );
}

/*
 * Draw circle with center in the previous point
 * and the given point is on the circle.
 */
Gcirc( x2, y2 )
{
	int     SarcR = ArcR;
	int     x1, y1;

	x1 = Gx; Gx = x2;
	y1 = Gy; Gy = y2;
	ArcR   = 0;
	Orient = 0;

	/* Opposite diagonal point */
	x1 = x1 + x1 - x2;
	y1 = y1 + y1 - y2;

	/* Compute location of a center */
	center( x1, y1, x2, y2 );

	/* Draw arc 1 */
	arc( x2, y2, x1, y1, -1 );

	/* Draw arc 2 */
	arc( x1, y1, x2, y2, -1 );
	ArcR = SarcR;
}

/*
 * Рисование дуги от (x1,y1) до (x2,y2) с центром (Cx,Cy)
 * Здесь:  zz =  1 - против час. стрелки
 *              -1 - по час. стрелке
 */
arc( x1, y1, x2, y2, zz )
{
	long      d1, d2, d3, D;
	long      dd1, dd2, dd3;
	register dir, l;
	int      df, Odir;
	int      dx, dy;
	int      ox = x1, oy = y1;
	int      sR, sG, sB;
	int      ctr = 0;

	D = 0;
	Odir = 1;
	l = 0;
	if( vicolor && Gbrf && Gfill == 0 ) {
		/* для линии половинной яркости фон - черный */
		sR   = viR; sG   = viG; sB   = viB;
		viR &= ~2;  viG &= ~2;  viR &= ~2;
		RDC(viMODE);
	}
	for(;;) {
		/* Целочисленый алгоритм может промахнуться на одну точку */
		dir = x1-x2;
		if( dir < 0 ) dir = -dir;
		d1  = y1-y2;
		if( d1  < 0 ) d1  = -d1;
		dir += d1;
		if( vireset ) break;
		if( dir == 0 ) break;
		if( dir <= 1 && ctr++ ) break;

		dy = ((x1*HX) < Cx)? -zz: zz;
		dx = ((y1*HY) < Cy)? zz: -zz;
		dd1 = d1 = D + (HX*HX) + (2*HX)*(long)(dx*(x1*HX - Cx));
		dd2 = d2 = D + (HY*HY) + (2*HY)*(long)(dy*(y1*HY - Cy));
		dd3 = d3 = dd1 + dd2 - D;
		if( dd1 < 0 ) dd1 = -dd1;
		if( dd2 < 0 ) dd2 = -dd2;
		if( dd3 < 0 ) dd3 = -dd3;
		if( dd1 < dd2 )
			dir = (dd3 < dd1)? 2: 0;
		else
			dir = (dd3 < dd2)? 2: 1;
		df = 0;
		if( Gfill ) {   /* не нужно строить векторы */
			switch( Gfill ) {
			    case 1:     /* fill from X */
				if( dir != 0 ) vfillv( Gfx, x1, y1 );
				break;
			    case 2:     /* fill from Y */
				if( dir != 1 ) hfillv( x1, Gfy, y1 );
				break;
			    case 3:     /* fill from (X,Y) */
				fillvect( x1, y1 );
			}
		} else if( dir != Odir ) {
			if( Odir )
				vector( x1, y1-l, 1, l );
			else
				vector( x1-l, y1, 0, l );
			Odir = (dir != 0);
			l = 0;
		} else
			df++;
		switch( dir ) {
		    case 0:     /* смещение по X */
			x1 += dx;
			D = d1;
			if( df ) l += dx;
			break;

		    case 1:     /* смещение по Y */
			y1 += dy;
			D = d2;
			if( df ) l += dy;
			break;

		    case 2:     /* смещение по X и Y */
			x1 += dx;
			y1 += dy;
			D = d3;
		}
		if( ox == x1 && oy == y1 )
			break;
	}
	if( Gfill ) {   /* не нужно строить векторы */
		switch( Gfill ) {
		    case 1:     /* fill from X */
			if( dir != 0 ) vfillv( Gfx, x1, y1 );
			break;
		    case 2:     /* fill from Y */
			if( dir != 1 ) hfillv( x1, Gfy, y1 );
			break;
		    case 3:     /* fill from (X,Y) */
			fillvect( x1, y1 );
		}
	} else {
		if( Odir )
			vector( x1, y1-l, 1, l );
		else
			vector( x1-l, y1, 0, l );
		if( vicolor && Gbrf ) {
			viR = sR; viG = sR; viB = sB;
			RDC(viMODE);
		}
	}
}

/*
 * Вычисление координат центра дуги
 */
center( x1, y1, x2, y2 )
{
	int     x0, y0;
	long    alpha, delta;
	register unsigned s, sp, sm;
	int     r = ArcR;

	x1 *= HX; x2 *= HX;
	y1 *= HY; y2 *= HY;
	r  *= HY;
	x0 = (x1+x2)/2;
	y0 = (y1+y2)/2;
	delta = ((long)(x2-x1))*(x2-x1) + ((long)(y2-y1))*(y2-y1);
	alpha = ((4l*r)*r - delta);

	/* Integral sqrt */
	if( alpha > 32764l )
		sp = 32764;
	else
		sp = (int)(alpha/2) + 1;
	sm = 0;
	do {
		s = (sp+sm+1)/2;
		if( (long)s*s > alpha ) {
			if( sp == s )
				s--;
			sp = s;
		} else
			sm = s;
		if( vireset ) return;
	} while( sm < sp );
	alpha = (long) s;

	/* Integral sqrt */
	if( delta > 32764l )
		sp = 32764;
	else
		sp = (int)(delta/2) + 1;
	sm = 0;
	do {
		s = (sp+sm+1)/2;
		if( (long)s*s > delta ) {
			if( sp == s )
				s--;
			sp = s;
		} else
			sm = s;
		if( vireset ) return;
	} while( sm < sp );
	delta = (long) s;

	if( Orient )
		alpha = -alpha;

	Cx = x0 + (int)((alpha*(y1-y2))/(delta*2));
	Cy = y0 + (int)((alpha*(x2-x1))/(delta*2));
}

/*
 * Произвольный вектор заполнения от Gfx, Gfy до x, y
 */
fillvect( x2, y2 )
{
	int      x1 = Gfx, y1 = Gfy;
	int      d1, d2;
	register dd1, dd2, l;
	int      dx, dy;
	int      a, b, D, Odir;

	dx = (x1 > x2)? -1 : 1;
	dy = (y1 > y2)? -1 : 1;
	b = (x2 - x1);
	a = (y2 - y1);
	if( a < 0 ) a = -a;
	if( b > 0 ) b = -b;
	D = 0;
	Odir = 1;
	l = 0;
	while( x1 != x2 || y1 != y2 ) {
		if( vireset ) return;
		dd1 = d1 = D + a;
		dd2 = d2 = D + b;
		if( dd1 < 0 ) dd1 = -dd1;
		if( dd2 < 0 ) dd2 = -dd2;
		dd1 = ( dd1 >= dd2 );
		dd2 = 0;
		if( dd1 != Odir ) {
			if( Odir )
				hfillv( x1, y1-l, y1 );
			else
				vfillv( x1-l, x1, y1 );
			Odir = dd1;
			l = 0;
		} else dd2++;
		if( !dd1 ) {
			/* смещение по X */
			x1 += dx;
			D = d1;
			if( dd2 ) l += dx;
		} else {
			/* смещение по Y */
			y1 += dy;
			D = d2;
			if( dd2 ) l += dy;
		}
	}
	if( Odir )
		hfillv( x1, y1-l, y1 );
	else
		vfillv( x1-l, x1, y1 );
}

/*
 * Горизонтальный вектор заполнения
 */
hfillv( x, y1, y2 )
{
	register struct vireg *V = VIADDR;
	extern int Grks, FillStyle, Gbrf, cshift;
	register int bf;
	static Filltab[4][4] = {
	0177777,        0177777,        0177777,        0177777,
	0125252,        0052525,        0125252,        0052525,
	0031463,        0114631,        0146314,        0063146,
	0021042,        0010421,        0104210,        0042104,
	};

	if( y2 < y1 ) {
		bf = y1;
		y1 = y2;
		y2 = bf;
	}
	/* CIPPING FOR SCREEN */
	if( y1 < 0 ) y1 = 0;
	if( y2 > 1023 ) y2 = 1023;
	if( y2 < y1 || x < 0 || x > 255 ) return;

	bf = y1+x;
	if( FillStyle ) bf += x;
	bf &= 03;
	bf = Filltab[FillStyle+FillStyle+Gbrf][bf];
	while( !(V->vi_rks & 0100000) );
	Grks = 0;
	V->vi_rks &= ~0400;
	V->vi_rbf = bf ^ viinv;
	V->vi_rx  = x+cshift;
	V->vi_ry  = y1;
	V->vi_rvl = y2-y1+1;
};

/*
 * Вертикальный вектор заполнения
 */
vfillv( x1, x2, y )
{
	register struct vireg *V = VIADDR;
	extern int Grks, FillStyle, Gbrf, cshift;
	register int bf;
	static Filltab[4][4] = {
	0177777,        0177777,        0177777,        0177777,
	0125252,        0052525,        0125252,        0052525,
	0052525,        0052525,        0125252,        0125252,
	0000000,        0052525,        0000000,        0125252,
	};

	if( x2 < x1 ) {
		bf = x1;
		x1 = x2;
		x2 = bf;
	}
	/* CLIPPING FOR SCREEN */
	if( x1 < 0 )   x1 = 0;
	if( x2 > 255 ) x2 = 255;
	if( x2 < x1 || y < 0 || y > 1023 ) return;

	bf = x1+y;
	if( FillStyle ) bf += x1;
	bf &= 03;
	bf = Filltab[FillStyle+FillStyle+Gbrf][bf];
	while( !(V->vi_rks & 0100000) );
	Grks = 1;
	V->vi_rks |= 0400;
	V->vi_rbf = bf ^ viinv;
	V->vi_rx  = x1+cshift;
	V->vi_ry  = y;
	V->vi_rvl = x2-x1+1;
};

#define abs(x) (((t = (x))<0)? -t : t)

/*
 * Заполнение треугольной области
 */
filltri(x1, y1, x2, y2)
register y1;
{
	extern int Gfx, Gfy;
	int     x3 = Gfx, y3 = Gfy;
	register t, sy;
	int a1, a2, b1, b2, dy1, dy2;
	int D1, D2;
	int d1, d2, d3;

	/* Sort by x's */
    Sa:
	if( x1 > x2 ) {
		t = x1; x1 = x2; x2 = t;
		t = y1; y1 = y2; y2 = t;
	}
	if( x2 > x3 ) {
		t = x2; x2 = x3; x3 = t;
		t = y2; y2 = y3; y3 = t;
		goto Sa;
	}

	/* Fill a top of a triagle */
	dy1 = (y1 > y2)? -1 : 1;
	dy2 = (y1 > y3)? -1 : 1;
	a1  = y2 - y1;
	b1  = x2 - x1;
	a2  = y3 - y1;
	b2  = x3 - x1;
	if( a1 < 0 ) a1 = -a1;
	if( a2 < 0 ) a2 = -a2;
	D1 = D2 = 0;
	sy = y1;
	while( x1 < x2 ) {
		do {
			if( vireset ) return;
			if( y1 == y2 )
				break;
			d1 = abs(D1-b1);
			d2 = abs(D1-b1+a1);
			d3 = abs(D1+a1);
			if( d3 <= d1 && d3 <= d2 )
				break;
			D1 -= b1;
			y1 += dy1;
		} while( d1 < d2 );
		do {
			if( vireset ) return;
			if( sy == y3 )
				break;
			d1 = abs(D2-b2);
			d2 = abs(D2-b2+a2);
			d3 = abs(D2+a2);
			if( d3 <= d1 && d3 <= d2 )
				break;
			D2 -= b2;
			sy += dy2;
		} while( d1 < d2 );
		hfillv( x1, y1, sy );
		x1++;
		D1 += a1;
		D2 += a2;
	}

	/* Draw a bottom of a triangle */
	dy1 = (y2 > y3)? -1 : 1;
	a1  = y3 - y2;
	b1  = x3 - x2;
	if( a1 < 0 ) a1 = -a1;
	D1 = 0;
	y1 = y2;
	do {
		hfillv( x1, y1, sy );
		do {
			if( vireset ) return;
			if( y1 == y3 )
				break;
			d1 = abs(D1-b1);
			d2 = abs(D1-b1+a1);
			d3 = abs(D1+a1);
			if( d3 <= d1 && d3 <= d2 )
				break;
			D1 -= b1;
			y1 += dy1;
		} while( d1 < d2 );
		do {
			if( vireset ) return;
			if( sy == y3 )
				break;
			d1 = abs(D2-b2);
			d2 = abs(D2-b2+a2);
			d3 = abs(D2+a2);
			if( d3 <= d1 && d3 <= d2 )
				break;
			D2 -= b2;
			sy += dy2;
		} while( d1 < d2 );
		x1++;
		D1 += a1;
		D2 += a2;
	} while( x1 <= x3 );
}

#endif NVI
