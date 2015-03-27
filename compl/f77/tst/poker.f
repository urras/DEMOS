      subroutine pkr
      real*8 rndm
      common/ore1/ b1,d9,p,t,b3,b,raide(30)
      integer t,tt
      character*6 pok(13),tove
      implicit integer (i-n,t)
      integer senior
      character*6 most(4)
      character*6  kort(20)
      character suff(10)
      dimension mpok(5),mmost(5),mp(5),mm(5),kok(5)
     *,                         mpd(5),mmd(5)
     *,   koloda(54),kol2(54),nref(54)
      data suff/'у','ы','ы','ы',' ',' ', ' ', ' ', ' ', ' '/
      data most/'пики','крести','буби','черви'/
      data pok/'2','3','4','5','6','7','8'
     *        ,'9','10','В','Д','К','Т'/
      data kort/'"ничег','о"','"пара"',' '
     *,'"две п','ары"','"тройк','а"','"стрит',
     *'"','"цвет"',' ','"фул"',' ','"каре"'
     *,' ','флеш р','ойяль','"п о к',' е р"'/
c
c
c         Использование  массивов:
c
      data nref(1)/100/
c
c     mpok(5) - старшинство карт игрока
c     mp(5)   - ---------------- соперника
c    mmost(5) - масти карт игрока
c    mm(5)    - ---------- соперника
c
c    "масса" карты = ("старшинство"-1)*4+"масть"
c
c       kok(5) - номера сменяемых карт
      data iquick/1/
c
c     igpa ha 54 kaptax
c
      ttt=t
c
      write(6,"(' будем играть в покер')")
      write(6,"( '    ******    ****   *   *  *****  **** ')")
      write(6,"( '    *    *   *    *  *  *   *      *   *')")
      write(6,"( '    *    *   *    *  ***    ****   **** ')")
      write(6,"( '    *    *   *    *  *  *   *      *    ')")
      write(6,"( '    *    *    ****   *   *  *****  *    ')")
      write(6,"(' начальная ставка 3 дол.')")
      if(nref(1).ne.100) go to 2
      do 10 i=1,54
  10  koloda(i)=i
  2   continue
c-----------------------------------------------
c     начальные засылки $ shuffle
c-----------------------------------------------
      do 20 i=1,54
      nref(i)=i
  20  kol2(i)=koloda(i)
c
      do 40 i=1,54
      j=int(rndm(-1)*54+1)
      if(kol2(j).ne.0)go to 30
      do 26 k=1,53
      j=i+k
      if(j.gt.54)go to 21
      if(kol2(j).ne.0)go to 30
  21  j=i-k
      if(j.le.0)go to 26
      if(kol2(j).ne.0)go to 30
  26  continue
  30  koloda(i)=kol2(j)
  40  kol2(j)=0
c
c-----------------------------------------------
c     выдача  карты  игроку
c-----------------------------------------------
      assign 50 to iret
      do 60 i=1,5
      go to 900
  50  mmost(i)=mactx
  60  mpok(i)=senior
c
      tove=1h
      assign 80 to i ret
      go to 930
801   read(5,"(x)")
  80  write(6,90)
  90  format(' какие хотите менять ?.....\b\b\b\b\b'$)
      read(5,100,err=801)kok
  100 format(5i1)
      if(kok(1).eq.0)goto 200
c
      do 110 i=1,5
      if(kok(i).eq.0.or.kok(i).gt.5)go to 110
      mpok(kok(i))=0
  110 continue
c
      assign 120 to iret
      do 130 i=1,5
      if(mpok(i).ne.0)go to 130
      go to 900
  120 mmost(i)=mactx
      mpok(i)=senior
  130 continue
c
      assign 200 to i ret
      go to 945
c-----------------------------------------------
c   тактика противника при смене карт
c-----------------------------------------------
  200 assign 210 to i ret
      do 220 i=1,5
      go to 900
  210 mm(i)=mactx
  220 mp(i)=senior
c
      ipok=kompkr(mp,mm,5)
c     read(5,221)iret
c 221 format(a1)
c     assign 225 to iret
c     go to 1000
c 225 continue
      k=0
      if(ipok.gt.1)goto(230,22,33,555,555,555,44,555,555)ipok
c                       2   22 3   s   c  3+2 4  cs  "5"
c     "ничего" => попробуем сменить одну карту
c
  230 ip=ipok
      max=0
      do 250 j=1,5
      jj=1
      do 240 i=1,5
      if(i.eq.j)go to 240
      mmd(jj)=mm(i)
      mpd(jj)=mp(i)
      jj=jj+1
  240 continue
      ipok=kompkr(mpd,mmd,4)
      if(ipok.lt.max)go to 250
      max=ipok
      maxj=j
  250 continue
c
      if(max.gt.1)go to 295
c
c-----------------------------------------------
c  одна карта начего не дала => попробуем две
c-----------------------------------------------
      max=0
      do 280 j=1,4
      ii=j+1
      do 280 i=ii,5
      jj=1
      do 270 k=1,5
      if(k.eq.i.or.k.eq.j)go to 270
      mpd(jj)=mp(k)
      mmd(jj)=mm(k)
      jj=jj+1
  270 continue
c
      ipok=kompkr(mpd,mmd,3)
c     write(6,271)j,i,ipok
c 271 format(' j='i2' i='i2' ipok='i2)
      if(ipok.le.max)go to 280
      max=ipok
      maxi=i
      maxj=j
  280 continue
c
      if(max-1)300,370,290
  290 mp(maxi)=0
  295 mp(maxj)=0
      go to 500
c
c     "ничего"  и  осталось
c
  300 i=int(rndm(-1)*5+1)
      do 340 j=1,i
  330 k=int(rndm(-1)*5+1)
      if(mp(k).eq.0)go to 330
  340 mp(k)=0
      go to 500
c
c          есть "2+2"
c        заходить на "3"  или  "3+2"
c
  22  i=int(rndm(-1)*2)
      if(i)44,370,44
c
c         ectx "3"
c    если есть joker => есть надежда на лучшее
c
  33  if(mmost(3))380,230,380
c
c
c     есть "2"
c
  370 mp(3)=0
c     ectx "3"
 380  mp(4)=0
c     "4" $ "2+2"
  44  mp(5)=0
  500 assign 520 to i ret
c     write(6,510)mp
c 510 format(5i3)
      k=0
      do 530 i=1,5
      if(mp(i).ne.0)go to 530
      go to 900
  520 mm(i)=mactx
      mp(i)=senior
      k=k+1
  530 continue
c
  555 write(6,540)k,suff(k)
  540 format(' соперник сменил'i2' карт'a1)
      ipok=kompkr(mp,mm,5)
c     assign 550 to iret
c     go to 1000
  550 continue
c-----------------------------------------------
c         игра
c-----------------------------------------------
      k1=8
      if(ipok.le.3)k1=ipok+1
c
      ksk=0
      mstake=0
      tt=t
      t=t-3
      iprint=0
      tove=1h
      isum=6
580   istake=int(rndm(-1)*3)+2**ksk
      ksk=ksk+1
      write(6,572)mstake,istake
572   format(' соперник поставил ',i6,' дол.+ ',i4,' дол.')
      isum=isum+mstake+istake
      write(6,576)isum,t
576   format(' банк: ',i6,' дол. - у вас'i6' дол.')
      if(iquick.eq.1) go to 588
 5721 write(6,573)
  573 format(' откр.(0), пас(1), плюс n (n>1) ?'$)
      go to 574
5881  read(5,"(x)")
588   write(6,589)
589   format(' ваша тактика:(0) открывать, (1) пасовать',/
     *' увеличить ставку на n дол.(n>1)?'$)
      iquick=0
  574 read(5,860,err=5881)mstake
      if(mstake.eq.1) go to 639
c-----------------------------------------------
c        открывать $ продолжать
c-----------------------------------------------
      if(t.ge.mstake+istake)go to 585
      write(6,587)
587   format(' у вас нет денег')
      if(t.ge.istake)go to 5721
      goto 639
c
c      денег хватает
c
  585 t=t-mstake-istake
      isum=isum+mstake+istake
      if(mstake.eq.0)go to 600
c-----------------------------------------------
c        продолжать
c-----------------------------------------------
      if(ksk.le.k1.and.mstake.le.2**k1)go to 580
      if(ipok.eq.0.and.mstake.gt.10)go to 460
      isum=isum+mstake+istake
      assign 600 to iret
      tove=':'
c---------------------------------------------
c     засветка карт противника
c-----------------------------------------------
 1000 write(6,582)kort(ipok*2+1),kort(ipok*2+2),tove
582   format(' соперник открывает карты:',/
     *' он собрал ',a6,a6,a6)
      do 1080 i=1,5
      if(mm(i).ne.0) go to 1060
      write(6,950)i
      go to 1080
 1060 write(6,970)i,pok(mp(i)),most(mm(i))
 1080 continue
      iprint=1
      go to i ret
c-----------------------------------------------
c    какая комбинация у игрока
c-----------------------------------------------
  600 ikt=kompkr(mpok,mmost,5)
      if(iprint.eq.0)go to 700
      if(ikt.eq.ipok)tove='тоже'
  700 write(6,710)kort(ikt*2+1),kort(ikt*2+2),tove
  710 format(' у вас: 'a6,a6,a6)
c-----------------------------------------------
      if(iprint.eq.1)go to 635
      tove=':'
      if(ikt.eq.ipok)tove='тоже:'
      assign 635 to iret
      go to 1000
c-----------------------------------------------
c       кто выйграл ?
c-----------------------------------------------
  635 if(ikt-ipok)639,636,637
c-----------------------------------------------
c       игрок проиграл
c-----------------------------------------------
  639 tt=tt-t
      write(6,6391)tt
 6391 format(' ваш проигрыш составил 'i4' дол.')
      go to 800
c-----------------------------------------------
c       одинаковые комбинации
c-----------------------------------------------
  636 if(mpok(1)-mp(1)) 420,400,440
  400 if(ikt.ne.2) go to 410
      if(mpok(3)-mp(3)) 420,410,440
  410 if(mmost(1).gt.mm(1)) go to 440
c               выиграл соперник
c
  420 write(6,430)kort(ikt*2+1),kort(ikt*2+2)
  430 format(' у соперника 'a6,a6' сильнее')
      go to 639
c               byigpal igpok
  440 write(6,450)kort(ikt*2+1),kort(ikt*2+2)
  450 format(' у вас 'a6,a6' сильнее')
      go to 637
c-----------------------------------------------
c       противник спасовал
c-----------------------------------------------
  460 write(6,470)
  470 format(' противник  пасует')
c-----------------------------------------------
c       igpok  byigpal
c-----------------------------------------------
637   write(6,641)isum
641   format(' поздравляю!!!  вы сорвали банк ',i6,' дол.')
      t=t+isum
c-----------------------------------------------
c              баланс
c-----------------------------------------------
  800 tt=t-ttt
      if(tt)810,830,850
  810 tt=-tt
      write(6,820)tt
  820 format(' пока проигрываете всего'i5' дол.')
      go to 6392
  830 write(6,840)
  840 format(' пока ничья')
      go to 6392
  850 write(6,855)tt
  855 format(' пока дерете противника на'i5' дол.')
6392  write(6,6401)
6401  format(' ну как, сыграем еще (1-да,0-нет)?'$)
      read(5,860,err=63921)i
  860 format(i5)
      if(i.eq.1)goto 2
      return
63921   read(5,"(x)")
	goto 6392
c-----------------------------------------------
c     выдача очередной карты
c-----------------------------------------------
  900 ii=int(rndm(-1)*54+1)
      j=nref(ii)
      if(j.eq.0) go to 900
      nref(ii)=0
      ii=koloda(j)+3
      senior=ii/4
      if(senior.eq.14) go to 910
      mactx=ii-senior*4+1
      go to i ret
  910 mactx=0
      go to iret
c-----------------------------------------------
c     засветка карт игрока
c-----------------------------------------------
  930 write(6,940)
  940 format(' ваши карты:')
  945 do 980 i=1,5
      mmd(i)=mmost(i)
      mpd(i)=mpok(i)
      if(mmost(i).ne.0) go to 960
      write(6,950)i
  950 format(i2'. **'x'joker')
      go to 980
  960 write(6,970)i,pok(mpok(i)),most(mmost(i))
  970 format(i2'. 'a2,x,a6)
  980 continue
c     ikt=kompkr(mpd,mmd,5)
c     write(6,710)kort(ikt*2+1),kort(ikt*2+2),tove
      go to i ret
c---------------------------------------------
      end
      function kompkr(mpok,mmost,kk)
      implicit integer (i-n,t)
      integer kompkr
c
c-----------------------------------------------
c    какая комбинация у игрока на кк картах
c                                4<kk<5
c-----------------------------------------------
c
c  mpok - старшинство        mmost - масть
c
c    1 - 2                   1 - пики
c    .....                   2 - крести
c   13 - tyz                 3 - буби
c    .....                   4 - черви
c
c                            0 - *joker*
c
c       на выходе:
c
c       kompkr=0 => "ничего"
c       ........     ......
c             =9 => "покер"
c
c       mpok и mmost при kk=5  упорядочиваются
c        по комбинациям
c
      dimension mpok(5),mmost(5),maska(13)
     *         ,mactx(4),mpokd(5),mostd(5)
c
      do 10 i=1,13
  10  maska(i)=0
c
      do 20 i=1,4
  20  mactx(i)=0
c
      max=1
      joker=0
      k=0
      maxm=1
      minord=13
      maxord=1
c
      do 40 i=1,kk
      j=mmost(i)
      if(j.eq.0) go to 30
      mactx(j)=mactx(j)+1
      if(maxm.lt.mactx(j)) maxm=mactx(j)
      j=mpok(i)
      maska(j)=maska(j)+1
      if(maska(j).le.max) go to 200
      max=maska(j)
      maxi=j
  200 if(minord.gt.j) minord=j
      if(maxord.lt.j) maxord=j
      k=k+1
      mpokd(k)=mpok(i)
      mostd(k)=mmost(i)
      go to 40
  30  joker=joker+1
  40  continue
c                  результат: kk = k + joker
c
      go to (111,222,333,444,555) max
c-----------------------------------------------
c     max=1 => m.b. "ничего"(0),"очередь"(4),
c              "цвет"(5),"очередь+цвет"(8)
c-----------------------------------------------
c
c         joker=1 => "2"(1)
c         joker=2 => "3"(3)
c
c
  111 ikt=0
c
c     проверка на очередь
c
      if(maxord-minord.le.k-1) go to 150
      jj=joker
      do 140 i=minord,maxord
      if(maska(i).eq.1) go to 140
      if(jj.eq.0)go to 160
      jj=jj-1
  140 continue
  150 ikt=4
c
c     проверка на цвет
c
  160 if(maxm.eq.k) ikt=ikt*3/4+5
c                    0 => 5
c                    4 => 8
      if(ikt.ne.0) go to 700
      if(joker.ne.0)ikt=2*joker-1
c                    1 => 1
c                    2 => 3
      go to 700
c
c-----------------------------------------------
c     "2"(1) => m.b. "2+2"(2)
c-----------------------------------------------
c
c         joker=1 => "3"(3)
c              =2 => "4"(7)
c
  222 ikt=1
      if(k.le.3)go to 250
c
c      проверка на допустимую пару
c
      max2=0
      do 240 i=minord,maxord
      if(maska(i).eq.2) max2=max2+1
  240 continue
      if(max2.eq.2)go to 260
  250 if(joker.ne.0) ikt=ikt+joker*4-2
c                     1 => +2=3
c                     2 => +6=7
      go to 700
c                 есть "2+2"
  260 ikt=2
      if(joker.eq.0) go to 700
  270 ikt=6
      go to 700
c
c-----------------------------------------------
c     "3"(3) => м.б. "3+2"(6)
c-----------------------------------------------
c
c       joker=1 => "4"(7)
c            =2 => "5"(9)
c
  333 ikt=3
      if(k.eq.5)go to 340
      if(joker.ne.0) ikt=ikt+joker*2+2
c                    1 => +4=7
c                    2 => +6=9
      go to 700
c
c     проверка на дополнительную "пару"
c
  340 do 350 i=minord,maxord
      if(maska(i).eq.2) go to 270
  350 continue
      go to 700
c-----------------------------------------------
c     "4"(7) => м.б. "5"  if  joker
c-----------------------------------------------
c
  444 ikt=7
      if(joker.eq.0) go to 700
c
  555 continue
c-----------------------------------------------
c     "покер"(9)
c-----------------------------------------------
c
      ikt=9
c
  700 if(kk.ne.5)go to 9999
c-----------------------------------------------
c          упорядочивание карт
c-----------------------------------------------
c          1.  без joker"s
c-----------------------------------------------
      ki=k-1
      do 730 i=1,ki
      kj=i+1
c
      do 730 j=kj,k
      if(mpokd(j)-mpokd(i))730,710,720
  710 if(mostd(j).le.mostd(i))go to 730
  720 kkk=mpokd(j)
      mpokd(j)=mpokd(i)
      mpokd(i)=kkk
      kkk=mostd(j)
      mostd(j)=mostd(i)
      mostd(i)=kkk
  730 continue
c
      do 740 i=1,k
      mpok(i)=mpokd(i)
  740 mmost(i)=mostd(i)
c
      kkk=ikt+1
      goto(9999,11,22,33,44,55,66,900,88,900)kkk
c           0   2  22 3  st c  32  4  cs  5
c
c                                    "2" $ "3+2"
  11  continue
  66  if(joker.eq.1) maxi=maxord
      go to 900
c                                          "2+2"
  22  continue
      if(mpok(4).ne.mpok(5))go to 9999
      if(mpok(1).eq.mpok(2))go to 24
c 1,2,2
      do 23 i=2,5
      mpok(i-1)=mpokd(i)
  23  mmost(i-1)=mostd(i)
      mpok(5)=mpokd(1)
      mmost(5)=mostd(1)
      go to 9999
c 2,1,2
  24  do 25 i=1,2
      mpok(i)=mpokd(i)
      mmost(i)=mostd(i)
      mpok(i+2)=mpokd(i+3)
  25  mmost(i+2)=mostd(i+3)
      mpok(5)=mpokd(3)
      mmost(5)=mostd(3)
      go to 9999
c
  33  if(joker.eq.2) maxi=maxord
      go to 900
c
c                     "очередь" $ "цвет+очередь"
  44  continue
  88  if(joker.eq.0)go to 9999
c
c       bctabka jokers to  dypy
c
      j=1
      jj=joker
      kkk=maxord
      do 47 i=1,k
  45  mmost(j)=mostd(i)
      if(mpokd(i).eq.kkk) go to 46
      mmost(j)=0
      jj=jj-1
  46  mpok(j)=kkk
      j=j+1
      kkk=kkk-1
      if(mmost(j-1).eq.0)go to 45
  47  continue
  48  if(jj.eq.0)go to 9999
      if(kkk.eq.0)kkk=1
      mpok(j)=kkk
      mmost(j)=0
      kkk=kkk-1
      j=j+1
      jj=jj-1
      go to 48
c
c                                         "цвет"
  55  if(joker.eq.0) go to 9999
c
      ik=k+1
c      досброс jokers
      do 56 i=ik,5
c
      mmost(i)=0
  56  mpok(i)=1
      go to 9999
c-----------------------------------------------
c          упорядочение карт
c-----------------------------------------------
c    для "2","3","3+2","4","5"   with joker"s
c-----------------------------------------------
c
c        порядок:
c
c    1. max март "maxi" (старшинство комб.)
c    2. joker"s ( как maxi )
c    3. оставшиеся
c
  900 j=1
      do 910 i=1,k
      if(mpokd(i).ne.maxi)go to 910
      mpok(j)=mpokd(i)
      mmost(j)=mostd(i)
      j=j+1
      mpokd(i)=0
  910 continue
c
c    jokers
c
      do 920 i=1,joker
      mpok(j)=maxi
      mmost(j)=0
  920 j=j+1
c
c   остатки
c
  930 do 940 i=1,k
      if(mpokd(i).eq.0)go to 940
      mpok(j)=mpokd(i)
      mmost(j)=mostd(i)
      j=j+1
  940 continue
c
 9999 kompkr=ikt
      return
      end
      program pok
      common/ore1/ b1,d9,p,t,b3,b,raide(30)
      integer t
      t=1000
c     n=asto(t)
c     n=129
c     do 10 i=1,n
c 10  a=rndm(-1)
      call pkr
      end
