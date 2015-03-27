c
c
c
	dimension hh(10),bb(10),xc(10),yc(10),zc(10),al(10),q2h(10)
	dimension hhv(10,4,1),eev(10,4,1),bbv(10,4,1),rrv(10,4,1)
	dimension qm(10,4,4),alm(10,4,4),qalm(10,4,4),q(10),q1(10)
	dimension bbv0(10,4,1),hhv0(10,4,1),rrv0(10,4,1),eev0(10,4,1)
	dimension ppp(10,4,4),pppp(10,4,4),qq(10),qh(10),q1h(10)
	dimension omega(10,4,1),centr(10,4,1),skoro(10,4,1),q2(10)
	dimension eps(10,4,1),usug(10,4,1),uslo(10,4,1),uslc(10,4,1)
	dimension bhv(10,4,1),hkor(10,4,1),ckor(10,4,1),skorc(10,4,1)
	dimension vert0(10,4,1),vert(10,4,1),silam(10,4,1),sila(10,4,1)
	dimension ppp0(10,4,4),ten(10,4,4),usugs(10,4,1),rrvs(10,4,1)
	dimension uslcs(10,4,1),omegas(10,4,1),omegak(10,4,1),ves(10)
	dimension pppp0(10,4,1),qalmo(10,4,4),qmo(10,4,4),almo(10,4,4)
c
	dimension hhh(10),bbh(10),xch(10),ych(10),zch(10),alh(10),q2hh(10)
	dimension hhvh(10,4,1),eevh(10,4,1),bbvh(10,4,1),rrvh(10,4,1)
	dimension qmh(10,4,4),almh(10,4,4),qalmh(10,4,4),q1hh(10)
	common hh,qm,bbv0,ppp,omega,centr,pppp0
	read *,hh
	print *,hh
	read *,bb
	print *,bb
	read *,bhy
	print *,bhy
	call testa
	stop
	end
