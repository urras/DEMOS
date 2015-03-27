	program bio
	call getr(ndr,nmr,nyr,nwr,nnr)
	call gett(ndt,nmt,nyt,nwt,nnt)
	k=nnt-nnr
	n=nwr
	nw=nwt
	write( 3,10)nwr
10      format('.nr WR ',i3)
	write( 3,11)k
11      format('.nr DS ',i7)
	call getp(npl,pl,jpl)
	call getf
	type33
33       format('Для контроля :')
	type12,ndr,nmr,nyr
12	format('Ваш день рождения -',i2,',  месяц-',i2,',  год-',i2)
	type13,n
13	format('Вы родились в ',i1,'-й день недели.')
	type14,ndt,nmt,nyt,k
14      format('и на ',i2,'/',i2,'/',i2,'г. вы прожили',i7,'дн.')
	type54
54      format('Дайте подумать ...')
	klun=(nyt-69)*11+10
	klun=klun-(klun/30)*30
	ndx=14
	nmx=2
	n1=n
	if(nmt.gt.7)nyt=nyt+1
	call ndy(nmx,ndx,nmt,ndt,nyt,n1,k1)
	do25,ii=1,90
	if(mod(ii,30).eq.0) type74
74      format('Терпите - я думатю ...')
	sez=(k1-(k1/365)*365)
	cem=(k-(k/28)*28)
	cin=(k-(k/33)*33)
	cph=(k-(k/23)*23)
	cwn=(k-(k/jpl)*jpl)
	pi=6.28218
	pem=sin((pi/28)*cem)
	pin=sin((pi/33)*cin)
	pph=sin((pi/23)*cph)
	pwm=sin((pi/pl)*cwn)
	psez=sin((pi/364.0)*sez)
	sum=psez+pwm+(pph+pem+pin)/2.0
	lun=nmt+ndt+klun
	lun=lun-(lun/30)*30
	call plot(pem,pin,sum,pph,pwm,psaz,ndt,nmt,nw,lun)
	goto(16,17,16,19,16,19,16,16,19,16,19,15),nmt !
15	if(ndt-31)23,20,20
16	if(ndt-31)23,21,21
17	ky1=nyt
	ky1=(ky1/4)*4
	if(ky1.eq.nyt)goto18
	if(ndt-28)23,21,21
18	if(ndt-29)23,21,21
19	if(ndt-30)23,21,21
20	nmt=0
21	nmt=nmt+1
22	ndt=0
23	ndt=ndt+1
	k1=k1+1
	k=k+1
	if(nw.ne.7)goto24
	nw=0
24	nw=nw+1
25	continue
	stop
							end
