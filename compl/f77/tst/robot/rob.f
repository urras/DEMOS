c
c
c
c        program robot
c
c       описание массивов.
c
c
	dimension hh(10),bb(10),xc(10),yc(10),zc(10),al(10),q2h(10)
	dimension hhv(10,4,1),eev(10,4,1),bbv(10,4,1),rrv(10,4,1)
	dimension qm(10,4,4),alm(10,4,4),qalm(10,4,4),q(10),q1(10)
	dimension bbv0(10,4,1),hhv0(10,4,1),rrv0(10,4,1),eev0(10,4,1)
	dimension ppp(10,4,4),pppp(10,4,4),qq(10),qh(10),q1h(10)
	dimension omega(10,4,1),centr(10,4,1),skor0(10,4,1),q2(10)
	dimension eps(10,4,1),usug(10,4,1),usl0(10,4,1),uslc(10,4,1)
	dimension bhv(10,4,1),hkor(10,4,1),ckor(10,4,1),skorc(10,4,1)
	dimension verto(10,4,1),vert(10,4,1),silam(10,4,1),sila(10,4,1)
	dimension pppo(10,4,4),ten(10,4,4),usugs(10,4,1),rrvs(10,4,1)
	dimension uslcs(10,4,1),omegas(10,4,1),omegak(10,4,4),ves(10)
	dimension ppppo(10,4,4),qalmo(10,4,4),qmo(10,4,4),almo(10,4,4)

c
c       ввод начальных значений
c
	print 215
215     format(1x,'~islo zwenxew n:')
	read(5,220)n
220     format(i1)
	print 199
199     format(1x,'tyme:')
	read(5,190)tyme
	print 200
200     format(1x,'kwant wremeni dt:')
	read(5,190)dt
190     format(f5.3)
	call vvodv(hh,n,'hh','f5.3')
	call vvodv(bb,n,'bb','f5.3')
	call vvodv(xc,n,'xc','f5.3')
	call vvodv(yc,n,'yc','f5.3')
	call vvodv(zc,n,'zc','f5.3')
	call vvodv(al,n,'al','f9.5')
	call vvodv(q,n,' q','f9.5')
	call vvodv(q1,n,'q1','f9.5')
	call vvodv(q2,n,'q2','f9.5')
	call vvodv(ves,n,'ves','f6.3')
	call vvodm(ten,n,'ten','f6.4')
	tyme=0.
c
c       печать исходных данных
c
	print 999,(i,i=1,n)
999     format(11x,6(9x,i1))
	call print(hh,'hh  ','        ',1,n)
	call print(bb,'вв  ','        ',1,n)
	call print(xc,'хс  ','        ',1,n)
	call print(yc,'yc  ','        ',1,n)
	call print(zc,'zc  ','        ',1,n)
	call print(al,'al  ','        ',1,n)
	call print(q,'q   ','        ',1,n)
	call print(q1,'q1  ','        ',1,n)
	call print(q2,'q2  ','        ',1,n)
c
c
c       формирование векторов hhv,eev,bbv,rrv.
c
	do 40 i=1,n
	hhv(i,1,1)=hh(i)
	hhv(i,2,1)=0.
	hhv(i,3,1)=0.
	hhv(i,4,1)=1.
	eev(i,1,1)=0.
	eev(i,2,1)=0.
	eev(i,3,1)=1.
	eev(i,4,1)=1.
	bbv(i,1,1)=0.
	bbv(i,2,1)=0.
	bbv(i,3,1)=-1.*bb(i)
	bbv(i,4,1)=1.
	rrv(i,1,1)=xc(i)
	rrv(i,2,1)=yc(i)
	rrv(i,3,1)=zc(i)
40      rrv(i,4,1)=1.
c
c
c       формирование матриц alm,qm,qalm=qm*alm.
c
	do 33 i=1,n
	call obmatr(alm,i,4,4)
	call obmatr(qm,i,4,4)
33      continue
	do 34 i=1,n
	alm(i,1,1)=1.
	alm(i,1,4)=0.
	alm(i,2,2)=cos(al(i))
	alm(i,2,3)=-1.*sin(al(i))
	alm(i,3,2)=sin(al(i))
	alm(i,3,3)=cos(al(i))
34      alm(i,4,4)=1.
	do 600 i=1,n
	call prmatr(alm,almo,i,4,4)
	almo(i,2,3)=alm(i,3,2)
600     almo(i,3,2)=alm(i,2,3)
100     do 35 i=1,n
	qm(i,1,1)=cos(q(i))
	qm(i,1,2)=-1.*sin(q(i))
	qm(i,2,1)=sin(q(i))
	qm(i,2,2)=cos(q(i))
	qm(i,3,3)=1.
	qm(i,3,4)=0.
	qm(i,4,4)=1.
	call ummatr(qm,alm,qalm,i,i,i,4,4,4)
35      continue
	do 998 i=1,n
	q1h(i)=q1(i)
998     q2h(i)=q2(i)
c
c
c************************************************
c       пересчет в нулевую систему координат.
c************************************************
c
c
	call obmatr(ppp,1,4,4)
	do 10 k=1,4
10      ppp(1,k,k)=1.
c
c       вычисление произв.матриц для всех звеньев.
c
	do 20 i=2,n
	call ummatr(ppp,qalm,ppp,i-1,i-1,i,4,4,4)
20      continue
c
c       вычисление вектора bhv
c
	do 505 i=1,n
	call slmatr(hhv,bbv,bhv,i,i,i,4,1)
	bhv(i,4,1)=1.
505     continue
c
c       вычисление векторов в нулевой системе координат.
c
	do 30 i=1,n
	call ummatr(ppp,bbv,bbv0,i,i,i,4,1,4)
	call ummatr(ppp,qm,pppp,i,i,1,4,4,4)
	call ummatr(pppp,hhv,hhv0,1,i,i,4,1,4)
	call ummatr(pppp,rrv,rrv0,1,i,i,4,1,4)
	call ummatr(ppp,eev,eev0,i,i,i,4,1,4)
	call ummatr(pppp,bhv,centr,1,i,i,4,1,4)
30      continue
c
c
c       вычисление вектора omega
c       ************************
c
c
	call umsvek(q1,eev0,omega,1,1,1)
	do 500 i=2,n
	call umsvek(q1,eev0,ppp,i,i,1)
	call slmatr(ppp,omega,omega,1,i-1,i,4,1)
	omega(i,4,1)=1.
500     continue
c
c       вычисление вектора skor0
c       ************************
c
c
	call obmatr(skor0,1,4,1)
	skor0(1,4,1)=1.
	do 510 i=2,n
	call umvekt(omega,centr,skor0,i-1,i-1,i)
	call slmatr(skor0,skor0,skor0,i-1,i,i,4,1)
	skor0(i,4,1)=1.
510     continue
c
c
c       вычисление вектора skorc
c       ************************
c
c
	do 520 i=1,n
	call umvekt(omega,rrv0,skorc,i,i,i)
	call slmatr(skor0,skorc,skorc,i,i,i,4,1)
	skorc(i,4,1)=1.
520     continue
c
c
c       вычисление вектора eps
c       **********************
c
c
	do 530 i=1,n
	call umsvek(q2,eev0,eps,i,i,i)
530     continue
c
c
c       вычисление вектора usug
c       ***********************
c
c
	call obmatr(pppp,1,4,1)
	pppp(1,4,1)=1.
	do 540 j=2,n
	call obmatr(ppp,4,4,1)
	ppp(4,4,1)=1.
	k=j-1
	do 545 i=1,k
	call umvekt(eev0,eev0,ppp,i,j,2)
	qq(1)=q1(i)*q1(j)
	call umsvek(qq,ppp,ppp,1,2,3)
	call slmatr(ppp,ppp,ppp,4,3,4,4,1)
	ppp(4,4,1)=1.
545     continue
	call slmatr(pppp,ppp,pppp,j-1,4,j)
540     continue
	call obmatr(ppp,1,4,1)
	ppp(1,4,1)=1.
	do 550 i=1,n
	call slmatr(ppp,eps,ppp,1,i,1,4,1)
	call slmatr(ppp,pppp,usug,1,i,i,4,1)
	usug(i,4,1)=1.
550     continue
c
c
c       вычисление вектора usl0
c       ***********************
c
c
	call obmatr(usl0,1,4,1)
	usl0(1,4,1)=1.
	do 560 i=2,n
	call umvekt(usug,centr,ppp,i-1,i-1,1)
	call umvekt(omega,centr,ppp,i-1,i-1,2)
	call umvekt(omega,ppp,ppp,i-1,2,3)
	call slmatr(ppp,ppp,usl0,1,3,i,4,1)
	call slmatr(usl0,usl0,usl0,i-1,i,i,4,1)
	usl0(i,4,1)=1.
560     continue
c
c
c       вычисление вектора uslc
c       ***********************
c
c
	do 570 i=1,n
	call umvekt(usug,rrv0,ppp,i,i,1)
	call umvekt(omega,rrv0,ppp,i,i,2)
	call umvekt(omega,ppp,ppp,i,2,3)
	call slmatr(ppp,ppp,uslc,1,3,i,4,1)
	call slmatr(usl0,uslc,uslc,i,i,i,4,1)
	uslc(i,4,1)=1.
570     continue
c
c
c       вычисление координат
c       ********************
c
c
	call prmatr(rrv0,ckor,1,4,1)
	call obmatr(hkor,1,4,1)
	do 997 i=2,n
	call slmatr(hkor,centr,hkor,i-1,i-1,i,4,1)
	call slmatr(hkor,rrv0,ckor,i,i,i,4,1)
	ckor(i,4,1)=1.
	hkor(i,4,1)=1.
997     continue
c
c
c       печать векторов
c       ***************
c
	print 990,tyme
990     format(//20x,'wremq = ',f5.2,' sek.'/20x,16('*')/)
	print 999,(i,i=1,n)
	call print(bbv0,'bbv0','        ',4,n)
	call print(hhv0,'hhv0','        ',4,n)
	call print(rrv0,'rrv0','        ',4,n)
	call print(eev0,'eev0','        ',4,n)
	call print(omega,'omeg','        ',4,n)
	call print(centr,'cent','        ',4,n)
	call print(skor0,'skr0','        ',4,n)
	call print(skorc,'skrc','        ',4,n)
	call print(eps,'eps ','        ',4,n)
	call print(usug,'usug','        ',4,n)
	call print(usl0,'usl0','       ',4,n)
	call print(uslc,'uslc','       ',4,n)
	write(1,1000)
	write(1,1001) tyme
	write(1,1000)
1000    format(5x,15('*'),119('-'))
1001    format(5x,'time=',f10.4,' n',6x,'xyz0',7x,'xyzc',11x,
&       'w',10x,'e',9x,'v0',9x,'a0',9x,'vc',9x,'ac',8x,'f',10x,'mom')
1002    format(/)
1003    format(20x,i2,10(f11.3))
c
c
c       вычисление новых значений q,q1,tyme
c       ***********************************
c
c
	do 580 i=1,n
	q1(i)=q1(i)+(q2h(i)+q2(i))*dt
	q(i)=q(i)+(q1h(i)+q1(i))*dt/2
580     continue
	tyme=tyme+dt
c
c      формирование матриц qmo,qalmo=qmo*almo
c
	do 650 i=1,n
	call obmatr(qmo,i,4,4)
650     continue
	do 660 i=1,n
	qmo(i,1,1)=cos(q(i))
	qmo(i,1,2)=sin(q(i))
	qmo(i,2,1)=-1.*sin(q(i))
	qmo(i,2,2)=cos(q(i))
	qmo(i,3,3)=1.
	qmo(i,4,4)=1.
	call ummatr(qmo,almo,qalmo,i,i,i,4,4,4)
660     continue
c
c************************************************
c       ПЕРЕСЧЕТ В СОБСТВЕННУЮ СИСТЕМУ КООРДИНАТ
c************************************************
	call obmatr(pppo,1,4,4)
	do 670 k=1,4
670     pppo(1,k,k)=1.
c
c       ВЫЧИСЛЕНИЕ ПРОИЗ.МАТРИЦ ДЛЯ ВСЕХ ЗВЕНЬЕВ
c
	do 680 i=2,n
	call ummatr(pppo,qalmo,pppo,i-1,i-1,i,4,4,4)
680     continue
c
c       ВЫЧИСЛЕНИЕ ВЕКТОРОВ В СОБСТВЕННУЮ СИСТЕМУ КООР.
c
	do 690 i=1,n
	call ummatr(pppo,qmo,ppppo,i,i,1,4,4,4)
	call ummatr(ppppo,rrv0,rrvs,1,i,i,4,1,4)
	call ummatr(ppppo,omega,omegas,1,i,i,4,1,4)
	call ummatr(ppppo,usug,usugs,1,i,i,4,1,4)
	call ummatr(ppppo,uslc,uslcs,1,i,i,4,1,4)
 690    continue
c
c       РАСЧЕТ МОМЕНТОВ
c
	do 700 i=1,n
	call obmatr(omegak,i,4,4)
	omegak(i,1,2)=-1.*omegas(i,3,1)
	omegak(i,1,3)=omegas(i,2,1)
	omegak(i,2,1)=omegas(i,3,1)
	omegak(i,2,3)=-1.*omegas(i,1,1)
	omegak(i,3,1)=-1.*omegas(i,2,1)
	omegak(i,3,2)=omegas(i,1,1)
700     omegak(i,4,4)=1.
	do 702 i=1,n
	call umsvek(ves,uslcs,sila,i,i,i)
	call umvekt(sila,rrvs,silam,i,i,i)
	call ummatr(omegak,ten,ppp,i,i,i,4,4,4)
	call ummatr(ppp,omegas,ppp,i,i,i,4,1,4)
	call ummatr(ten,usugs,pppo,i,i,i,4,1,4)
	call slmatr(ppp,pppo,vert,i,i,i,4,1)
	vert(i,4,1)=1.
702     continue
	i=n-1
699     continue
	call slmatr(silam,vert,verto,i,i,i,4,1)
	call slmatr(verto,verto,verto,i,i+1,i,4,1)
	verto(i,4,1)=1.
	i=i-1
	if(i.ne.0)go to 699
	call print(rrvs,'rrvs','        ',4,n)
	do 996 i=1,n
	do 995 j=1,3
	write(1,1003) i,hkor(i,j,1),ckor(i,j,1),omega(i,j,1),
&                     usug(i,j,1),skor0(i,j,1),usl0(i,j,1),
&           skorc(i,j,1),uslc(i,j,1),sila(i,j,1),verto(i,j,1)
995     continue
	write(1,1002)
996     continue
	go to 100
	end
