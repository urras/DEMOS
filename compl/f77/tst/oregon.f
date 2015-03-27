      SUBROUTINE OREGON
C
C ׳ֵֽׁׂ : ׳׃ֵַֿ=9ֹֽ־04׃ֵֻ, user=4.03, sys=2.04
C ־ױײ־ֿ 900 ֲֻּֿֿ׳ ־ֱ tmp ִּׁ װֱׂ־׃ֳֹֹּׁ
C
      REAL*8 RNDM
      IMPLICIT INTEGER*2 (A-Z)
      COMMON/ORE1/ B1,D9,P,T,B3,B,RAIDE(30)
C
       INTEGER A,G,    B,     C,     M1,   T,      Z
C          גשKי EהA נATPOHש OהEצהA PAתHOE הEHרחי תOלOTO
C
      INTEGER*(2) D9,  M
C     Kל.CTPEלרגש Mילי
C
      INTEGER*2 X,X1,T1,WG,WP,E,P,S5,D(15),S4,G1,G2,D3
      CHARACTER*6 DT(38)
      INTEGER*2 B1,B3,RAIDE,TIME
      CHARACTER*6 VARD(3)
      CHARACTER*15 VADA(9)
      INTEGER*(2) D1,KORT(18),MDOL(20),MSK(20),TMAS(2)
      CHARACTER*12 WEEK(7)
C
      DATA D/6,11,13,15,17,22,32,35,
     *      37,42,44,54,64,69,95/
      DATA VARD/'נAYHי','AנA‏י','CראKCש'/
      DATA VADA/'‏יHחA‏חYK','גOלרûOך HOצ','KOצAHשך‏YלOK',
     *'תOלOTOך KלשK','KPיBOך HOC','גEûEHשך  נEC',
     *'חלAת AלMAת','CKAלרנOPEת','הYûEחYג'/
      DATA WEEK/'נOHEהEלרHיK','BTOPHיK','CPEהA  ',
     *'‏ETBEPח','נסTHידA','CYגגOTA','BOCKPECEHרE'/
      DATA DT/' 12 Aנ','PEלס',' 26 Aנ','PEלס',' 10 MA','ס',' 24 MA','ס',
     *' 7 יאH','ס',' 21 יא','Hס',' 5 יאל','ס',' 19 יא','לס',' 2 ABח',
     *'YCTA',' 16 AB','חYCTA',' 31 AB','חYCTA',' 13 CE','HTסגPס',
     *' 27 CE','HTסגPס',' 11 OK','TסגPס',' 25 OK','TסגPס',' 6 HOס','גPס'
     *,' 22 HO','סגPס',' 6 הEK','AגPס',' 20 הE','KAגPס'/
      I9=TIME(0)
      I9=I9/5000-I9/3000
      N=RNDM(-1)*I9+1
      ITIKR=1-(-1)**N
C-----------------------------------------------
1     WRITE(6,"(' גYהETE יחPATר (1),HET (0)')")
      Z=0
      READ(5,860)I1
      IF(I1.NE.1)RETURN
      I1=TIME(0)
      WRITE(6,"(' BAM HYצHש נPABילA ? (1- הA,0- HET)')")
      READ(5,860)N
      IF(N.NE.1)GO TO 701
      WRITE(6,"(//,' נYTEûECTBיE נO OPEחOHCKOך TPOנE')")
      WRITE(6,"(' ית ûTATA MיCCYPי B OPEחOH B 1847 חOהY')")
      WRITE(6,"(' Bש הOלצHש נPEOהOלETר 2040 Mילר')")
      WRITE(6,"(' תA 5-6 MECסדEB - ECלי OCTAHETECר B ציBשX',/)")
      WRITE(6,"('Y BAC ECTר 900 הOללAPOB, י Bש TOלרKO ‏TO')")
      WRITE(6,"(' תAנלATילי תA זYPחOH 200 הOל.')")
      WRITE(6,"(' BAM HYצHO נOTPATיTר OCTAלרHשE הEHרחי')")
      WRITE(6,"(' HA CלEהY‎יE BE‎י:')")
      WRITE(6,"(' גשKי- Bש MOצETE נOTPATיTר HA HיX 200-300 ה.')")
      WRITE(6,"(' ‏EM גOלרûE Bש תAנלATיTE,TEM גשCTPEE גYהETE EXATר')")
      WRITE(6,"(' EהA- ‏EM גOלרûE Y BAC EE גYהET,TEM MEHרûE')")
      WRITE(6,"(' BEPOסTHOCTר תAגOלETר')")
      WRITE(6,"(' OPYציE - תA 1 הOל.MOצHO KYניTר 50 נATPOHOB')")
      WRITE(6,"(' OHי HYצHש הלס תA‎יTש,OXOTש י T.נ.')")
      WRITE(6,"(' OהEצהA - OCOגEHHO BAצHO B XOלOהHYא נOחOהY')")
      WRITE(6,"(' PAתHשE נPינACש- לEKAPCTBA י נPO‏יE BE‎י')")
      WRITE(6,"(' HEOגXOהיMש נPי גOלEתHי י HEנPEהBיהEHHשX CלY‏AסX')")
      WRITE(6,"( ' Bש MOצETE נOTPATיTר BCE הEHרחי הO HA‏AלA ילי')")
      WRITE(6,"(' OCTABיTר B KACCE HA נYTEBשE PACXOהש נPי OCTAHOBKAX')")
      WRITE(6,"('B CEלEHיסX')")
      WRITE(6,"(' OהHAKO,TAM BCE CTOיT הOPOצE.MOצHO TAKצE')")
      WRITE(6,"(' EהY הOגשBATר נYTEM OXOTש-HAהO HAנE‏ATATר ',
     *'KOPOTKOE CלOBO')")
      WRITE(6,"(' ‏EM גשCTPEE HAנE‏ATAETE TEM לY‏ûE ',
     *'גYהET PEתYלרTAT',//)")
      WRITE(6,"(' צEלAEM YCנEXA !!!')")
C-----------------------------------------------
 701  WRITE(6,
     *"(/' HACKOלרKO XOPOûO Bש CTPEלסETE?',/
     *' (1) OTלי‏HO,(2) XOPOûO,(3) גOלEE MEHEE,')")
      WRITE(6,
     *"(' (4) HE O‏EHר,(5) נלOXO'//
     *' ‏EM לY‏ûE Bש CEגס תAPEKOMEHהYETE,TEM CKOPEE')")
      WRITE(6,"(' BAM נPיהETCס נPיגEחATר K OPYציא')")
      READ(5,860)D9
      I2=TIME(0)
      I1=(I2-I1)
      DO 180 I=1,I1
 180  I2=RNDM(-1)
C-----------------------------------------------
      IF(D9.NE.7)GO TO 30
      D9=5
      GOTO 811
30    IF(D9.LE.3)GO TO 811
      IF(D9.GT.5)D9=5
      IF(ITIKR.GE.1)GO TO 811
      ITIKR=0
      IF(RNDM(-1).GE.0.8)ITIKR=1
      WRITE(6
     *,"(' A TEנEPר נPOBEPיM BAûי BOתMOצHOCTי',/
     *' HAהO CהEלATר 3 BשCTPEלA - לY‏ûיE הBA ית HיX',/
     *' גYהYT תA‏ETHשMי B BAûEך KBAליזיKAדיי')")
      IB3=0
      IKAT=0
      DO 203 I=1,3
      CALL D61
      IF(B1.GT.1)GO TO 200
      IKAT=IKAT+3
      IB3=IB3+B3
      GOTO 203
200   IF(B1.GE.4)GO TO 202
      IKAT=IKAT+2
      IB3=IB3+B3
      GOTO 203
202   IKAT=IKAT+1
      IB3=IB3+B3
203   CONTINUE
      IF(IKAT-5)205,206,207
205   WRITE(6,"(' O‏EHר CלAגO')")
      IX=-2+IB3/3
      WRITE(6,"(' Bש CTPEלסלי HA',I5,' KATEח.,A תAPEKOMEHהOBAליCר HA',I5,/
     *' TAKיM HA OPEחOHCKיX TPOנAX HE MECTO')")IX,D9
220   WRITE(6,"(' נOתOBיTE CלEהYא‎EחO הOגPOBOלרדA',//)")
      GOTO 1
C-----------------------------------------------
206   WRITE(6,"(' נOCלEהHיך ûAHC BAM - CTPEלסלי HE CילרHO')")
      IX=IB3/3-1/2
      WRITE(6,"(' BAûA KATEח.',I2,'. CTPEלסךTE נOCלEהHיך PAת')")IX
      CALL D61
      IF(B1.GE.4)GO TO 208
207   WRITE(6,"(' Bש CנPABיליCר C KBAליזיKAדיEך')")
      IX=IB3/3
      IF(D9-IX)211,212,213
211   WRITE(6,
     *"(' OהHAKO Bש XBACTYH: BAûA KATEח.',I2,' A CKAתAלי ‏TO',I2,/
     *' A XBACTYHAM TOצE HE MECTO HA TPOנAX')")IX,D9
      GOTO 220
212   WRITE(6,"(' C‏ACTליBOחO נYTי')")
      WRITE(6,216)D9
216   FORMAT(' BAûA KATEח.',I1)
      GOTO 811
213   WRITE(6,"(' Bש HACTOס‎יך גYהY‎יך ניOHEP',/
     *' ECלי נOBEתET, TO נOTOM נOEהEM B KלOHהAךK')")
      WRITE(6,216)D9
      GOTO 811
208   WRITE(6,"(' OנסTר CלAגOBATO - Hי‏EחO HE נOהEלAEûר',/
     *' נPיהETCס נOךTי B TיP נOTPEHיPOBATרCס')")
      GOTO 220
C-----------------------------------------------
811   X1=-1
      K8=0
      S4=0
      G1=0
      G2=0
      M=0
      M9=0
      D3=0
850   WRITE(6,"(/' CKOלרKO Bש XOTיTE תAנלATיTר תA גשKOB ?')")
      READ(5,860)A
860   FORMAT(I5)
      IF(A.GE.200)GO TO 901
      WRITE(6,"(' MAלO')")
      GOTO 850
901   IF(A.LE.300)GO TO 9930
      WRITE(6,"(' CליûKOM MHOחO')")
      GOTO 850
C
9930  WRITE(6,"(' CKOלרKO Bש XOTיTE נOTPATיTר HA EהY ?')")
      READ(5,860)G
      IF(G.GE.0)GO TO 980
      WRITE(6,"(' üTO HEBOתMOצHO')")
      GOTO 9930
C
980   WRITE(6,"(' CKOלרKO Bש XOTיTE נOTPATיTר HA נATPOHש ?')")
      READ(5,860)B
      IF(B.GE.0)GO TO 1030
      WRITE(6,"(' üTO HEBOתMOצHO')")
      GOTO 980
C
1030  WRITE(6,"(' CKOלרKO Bש XOTיTE נOTPATיTר HA OהEצהY ?')")
      READ(5,860)C
      IF(C.GE.0)GO TO 1080
      WRITE(6,"(' üTO HEBOתMOצHO')")
      GOTO 1030
C
1080  WRITE(6,"(' CKOלרKO Bש XOTיTE נOTPATיTר',/
     *' HA PAתHשE נPינACש ?')")
      READ(5,860)M1
      IF(M1.GE.0)GO TO 1130
      WRITE(6,"(' üTO HEBOתMOצHO')")
      GOTO 1080
C
1130  T=700-A-G-B-C-M1
      IF(T.GE.0)GO TO 1170
      WRITE(6,11150)
11150 FORMAT(' Y BAC HE XBATיT הEHEח - הABAךTE CHA‏AלA')
      GOTO 850
C-----------------------------------------------
1170  B=50*B
      WRITE(6,
     *"(' נOCלE BCEX תATPAT Y BAC OCTAלOCר ',I6,' הOללAPOB')")T
      WRITE(6,"(/,' נOHEהEלרHיK, 29 MAPTA 1847 ח.')")
      GOTO 1750
C-----------------------------------------------
1230  IF(M.GE.2040)GO TO 5430
      D3=D3+1
      IF(D3.NE.20)GOTO 18401
      WRITE(6,"(' Bש נYTEûECTBOBAלי CליûKOM הOלחO',/
     *' י נOחיגלי OT תיMHיX XOלOהOB')")
      GOTO 5170
18401 IQ=(D3-1)*2+1
      WRITE(6,
     *"(/'נOHEהEלרHיK'/A6,A6/'1847 ח.'/)")DT(IQ),DT(IQ+1)
C---  Q=D3+64 -----Q HE יCנOלרתYETCס
C-----------------------------------------------
1750  IF(G.LT.0)  G=0
      IF(B.LT.0)  B=0
      IF(C.LT.0)  C=0
      IF(M1.LT.0) M1=0
      IF(G.LT.13)
     * WRITE(6,"(' BAM HYצHO הOCTATר Eהש, י KAK MOצHO CKOPEE')")
      M2=M
      IF(S4.EQ.1)GO TO 1950
      IF(K8.NE.1)GO TO 1990
1950  T=T-20
      IF(T.LT.0)GO TO 5080
      WRITE(6,"(' נלATA הOKTOPY 20 הOל.')")
      K8=0
      S4=0
C-----------------------------------------------
1990  IF(M9.EQ.1)GO TO 2020
      WRITE(6,
     *"(' נPOךהEHO ',I6,' Mילר')")M
      GOTO 2040
2020  WRITE(6,"(' נPOךהEHO 950 Mילר')")
      M9=0
2040  WRITE(6,12040) G,B,C,M1,T,Z
12040 FORMAT(/'   EהA נATPOHש OהEצהA PAתHOE  KACCA תOלOTO',/
     *I6,I6,I7,I7,I8,I6)
      IF(X1.EQ.-1)GO TO 2170
      X1=X1*(-1)
2080  WRITE(6,"(' XOTיTE (1)ןףפAHOBיTרCס B CEלEHיי',/
     *' (2) נOOXOTיTרCס ילי (3) נPOהOלצATר נYTר?')")
      READ(5,860)X
      IF(X.GT.2)GO TO 2150
      IF(X.GE.1)GO TO 2270
2150  X=3
      GO TO 2270
2170  WRITE(6,"(' XOTיTE (1) נOOXOTיTרCס ילי (2) נPOהOלצATר נYTר?')")
      READ(5,860)X
      IF(X.NE.1)X=2
      X=X+1
      IF(X.GE.3)GO TO 2260
      IF(B.GT.39)GO TO 2260
      WRITE(6,"(' Y BAC CליûKOM MAלO נATPOHOB הלס OXOTש')")
      GOTO 2170
C-----------------------------------------------
2260  X1=-X1
2270  IF(X-2)2290,2540,2720
2290  IZ=RNDM(-1)*10+1
      WRITE(6,
     *"('MOצETE נPOהATר תOלOTO ! תA YHדיא ',I6,' הOל.')")IZ
      READ(5,860)IZD
      IF(Z.LE.0)GO TO 2390
      IF(IZD.LE.0)GO TO 2301
      IF(IZD.LE.Z)GO TO 2293
      WRITE(6,"(' CTOלרKO HE יMEETE - BOתרMEM CKOלרKO ECTר ')")
      T=T+IZ*Z
      Z=0
      GOTO 2301
2293  WRITE(6,"(' נPOהAלי...')")
      Z=Z-IZD
      T=T+IZ*IZD
      GOTO 2301
2390  IF(IZD.LE.0)GO TO 2301
      WRITE(6,"(' CנEPBA HAהO HAךTי תOלOTO...')")
2301  WRITE(6,"(' CKOלרKO Bש XOTיTE נOTPATיTר:',/,' HA EהY?')")
      CALL D24
      G=G+(20*P/30)*P
      WRITE(6,"(' HA נATPOHש?')")
      CALL D24
      B=B+(2*P*50/3)
      WRITE(6,"(' HA OהEצהY?')")
      CALL D24
      C=C+(2*P/3)
      WRITE(6,"(' HA PAתHשE נPינACש?')")
      CALL D24
      M1=M1+(2*P/3)
      M=M-55
      GOTO 2720
2540  IF(B.GT.39)GO TO 2570
      WRITE(6,"(' Y BAC CליûKOM MAלO נATPOHOB הלס OXOTש')")
      GOTO 2080
2570  M=M-55
      CALL D61
      IF(B1.LE.1)GO TO 2660
      IF(100*RNDM(-1).LT.13*B1)GO TO 2710
      G=G+48-2*B1
      WRITE(6,"(' HEנלOXOך BשCTPEל - נPיסTHOחO AננETיTA !')")
      B=B-10-3*B1
      GOTO 2720
2660  WRITE(6,"(' נPסMO MEצהY חלAת - OTלי‏HO !!!',/
     *' KPYנHAס הOגש‏A !')")
      G=G+52+6*RNDM(-1)
      B=B-10-4*RNDM(-1)
      GOTO 2720
2710  WRITE(6,"(' Bש נPOMAXHYליCר י BAû OגEה YגEצAל...')")
2720  IF(G.LT.13)GO TO 5060
2750  WRITE(6,
     *"(' KAK Bש XOTיTE ניTATרCס:(1) üKOHOMHO,',/
     *' (2) YMEPEHHO ילי (3) XOPOûO?')")
      READ(5,860)E
      IF(E.GT.3)GO TO 2750
      IF(E.LT.1)GO TO 2750
      G=G-8-5*E
      IF(G.GE.0)GO TO 2860
      G=G+8+5*E
      WRITE(6,
     *"(' CTOלר XOPOûO Bש ניTATרCס HE MOצETE')")
      GOTO 2750
2860  M=M+200+(A-220)/5.+10*RNDM(-1)
      L1=0
      C1=0
      W1=((M/100.-4)**2+12)
      IF(10.*RNDM(-1).GT.((M/100.-4.)**2+72.)/W1+1)GO TO 3550
2901  WRITE(6,"(' BנEPEהי BCAהHיKי. OHי')")
      S5=0
      IF(RNDM(-1).LT.0.8)GO TO 2950
      WRITE(6,"(' HE')")
      S5=1
2950  WRITE(6,"(' נOXOצי HA BPAחOB')")
      WRITE(6,"(' BAûA TAKTיKA? ')")
2970  WRITE(6,
     *"(' (1) YגEחATר,(2) ATAKOBATר,(3) נPOהOלצATר נYTר,',/
     *' (4) תA‎י‎ATרCס?')")
      IF(RNDM(-1).LE.0.2)S5=1-S5
      READ(5,860)T1
      IF(T1.LT.1)GO TO 2970
      IF(T1.GT.4)GO TO 2970
      IF(S5.EQ.1)GO TO 3330
      IF(T1.GT.1)GO TO 3110
      M=M+20
      M1=M1-15
      B=B-150
      A=A-40
      GOTO 3470
3110  IF(T1.GT.2)GO TO 3240
      CALL D61
      B=B-B1*40-80
3140  IF(B1.GT.1)GO TO 3170
      WRITE(6,"(' HEנלOXOך BשCTPEל - Bש יX נPOחHAלי')")
      GOTO 3470
3170  IF(B1.LE.4)GO TO 3220
      WRITE(6,"(' נלOXO CTPEלסETE...י נOלY‏ילי YהAP HOצOM...')")
      K8=1
      WRITE(6,"(' BAM HEOגXOהיM הOKTOP')")
      GOTO 3470
3220  WRITE(6,"(' HEנלOXO CTPEלסETE...')")
      GOTO 3470
3240  IF(T1.GT.3)GO TO 3290
      IF(RNDM(-1).GT.0.8)GO TO 3450
      B=B-160
      M1=M1-15
      GOTO 3470
3290  CALL D61
      B=B-B1*30-80
      M=M-50
      GOTO 3140
3330  IF(T1.GT.1)GO TO 3370
      M=M+15
      A=A-10
      GOTO 3470
3370  IF(T1.GT.2)GO TO 3410
      M=M-15
      B=B-100
      GOTO 3470
3410  IF(T1.LT.3)GO TO 3470
      M=M-40
3450  WRITE(6,"(' OHי HA BAC HE HAנAלי')")
      GOTO 3550
3470  IF(S5.EQ.0)GO TO 3501
      WRITE(6,"(' BCAהHיKי גשלי MיPHשMי')")
      GOTO 3550
3501  WRITE(6,"(' BCAהHיKי גשלי BPAחAMי - נPOBEPסךTE BAûי נOTEPי')")
      IF(B.GE.0)GO TO 3550
      WRITE(6,"(' Y BAC KOH‏יליCר נATPOHש - OHי BAC נPיKOH‏ילי')")
      GOTO 5170
3550  D1=0
      N=100*RNDM(-1)
3580  D1=D1+1
      IF(D1.GE.16)GO TO 4670
      IF(N.GT.D(D1))GO TO 3580
      GO TO(3660,3701,3740,3790,3820,
     *      3850,3890,3960,4130,4190,
     *      4220,4290,4340,4560,4670)D1
3625  WRITE(6,"('KOûMAP')")
C------------------------------------------ D1=1
3660  WRITE(6,"(' CלOMAלCס זYPחOH - נOTEPסHO BPEMס')")
      M=M-25-5*RNDM(-1)
      M1=M1-8
      GOTO 4710
C------------------------------------------ D1=2
3701  WRITE(6,"(' גשK נOBPEהיל HOחY - YMEHרûילACר CKOPOCTר')")
      M=M-35
      A=A-20
      GOTO 4710
C------------------------------------------ D1=3
3740  WRITE(6,"(' Bש CלOMAלי PYKY - HYצHO HAלOציTר ûיHY')")
      M=M-15-4*RNDM(-1)
      M1=M1-2-3*RNDM(-1)
      GOTO 4710
C------------------------------------------ D1=4
3790  WRITE(6,"(' Y BAC YûEל גשK.Bש נOTEPסלי BPEMס HA נOיCKי')")
      M=M-37
      GOTO 4710
C------------------------------------------ D1=5
3820  WRITE(6,"(' Bש נOTEPסלי הOPOחY י נOלהHס EE יCKAלי')")
      M=M-30
      GOTO 4710
C------------------------------------------ D1=6
3850  WRITE(6,"(' Bש נOTPATילי BPEMס HA נOיCKי BOהש')")
      M=M-30*RNDM(-1)-2
      GOTO 4710
C------------------------------------------ D1=7
3890  WRITE(6,"(' CילרHשך ליBEHר - נOTEPסHO BPEMס י ‏ACTר תAנACOB')")
      G=G-10
      B=B-500
      M1=M1-15
      M=M-30*RNDM(-1)-5
      GOTO 4710
C------------------------------------------ D1=8
3960  WRITE(6,"(' HAנAהEHיE גAHהיTOB')")
      CALL D61
      B=B-20*B1
      IF(B.GT.0)GO TO 4030
      WRITE(6,"(' Y BAC KOH‏יליCר נATPOHש - Y BAC תAגPAלי הEHרחי')")
      T=T/3.
      GOTO 4040
4030  IF(B1.LE.1)GO TO 4101
4040  WRITE(6,"(' BAM נPOCTPEלילי HOחY י תAגPAלי OהHOחO גשKA')")
      K8=1
      WRITE(6,"(' לY‏ûE, ECלי הOKTOP נOCMOTPיT BAûY PAHY')")
      M1=M1-5
      A=A-20
      GOTO 4710
4101  WRITE(6,"(' Bש OTלי‏HO CTPEלסETE י BCEX PAתOחHAלי',/
     *' BCE B נOPסהKE')")
      GOTO 4710
C------------------------------------------ D1=9
4130  WRITE(6,"(' B BAûEM זYPחOHE גשל נOצAP',
     *' - נPOנAלA ‏ACTר יMY‎ECTBA')")
      G=G-40
      B=B-400
      M1=M1-8*RNDM(-1)-3
      M=M-35
      GOTO 4710
C----------------------------------------- D1=10
4190  WRITE(6,"(' Bש נOTEPסלי הOPOחY B TYMAHE - נOTEPסHO BPEMס')")
      M=M-30-5*RNDM(-1)
      GOTO 4710
C----------------------------------------- D1=11
4220  WRITE(6,"(' BAC YKYCילA סהOBיTAס תMEס')")
      B=B-10
      M1=M1-5
      IF(M1.GE.0)GOTO 4710
      WRITE(6,"(' Y BAC HET לEKAPCTB - Bש YMיPAETE')")
      GOTO 5170
C----------------------------------------- D1=12
4290  WRITE(6,"(' זYPחOH OנPOKיHYלCס נPי נEPEנPABE ‏EPEת PEKY')")
      G=G-30
      C=C-20
      M=M-40-20*RNDM(-1)
      GOTO 4710
C----------------------------------------- D1=13
4340  WRITE(6,"(' HA BAC HAנAלי היKיE תBEPי')")
      CALL D61
      IF(B.GT.39)GO TO 4410
      WRITE(6,"(' Y BAC גשלO CליûKOM MAלO נATPOHOB',/
     *' OT BAC OCTAליCר POצKי הA HOצKי')")
      K8=1
      GOTO 5120
4410  IF(B1.GT.2)GO TO 4440
      WRITE(6,"(' XOPOûEך CTPEלרגOך Bש יX PAתOחHAלי')")
      GOTO 4450
4440  WRITE(6,"(' OHי BCE-TAKי הOגPAליCר הO BAûיX תAנACOB')")
4450  B=B-20*B1
      C=C-B1*4
      G=G-B1*8
      GOTO 4710
4490  WRITE(6,"(' XOלOהHAס נOחOהA - גP-P-P. Y BAC')")
      IF(C-22.GT.4*RNDM(-1))GO TO 4530
      WRITE(6,"(' HE')")
      C1=1
4530  WRITE(6,"(' הOCTATO‏HO TEנלAס OהEצהA')")
      IF(C1.EQ.0)GO TO 4710
      GOTO 6301
4560  WRITE(6,"(' CילרHAס גYPס - נPOנAלA ‏ACTר תAנACOB')")
      M=M-25-10*RNDM(-1)
      B=B-200
      M1=M1-4-3*RNDM(-1)
      GOTO 4710
C----------------------------------------- D1=15
4610  IF(E.EQ.1)GO TO 6301
      IF(E.EQ.3)GO TO 4650
      IF(RNDM(-1)-0.25)4710,4710,6301
C----------------------------------------- D1=14
4650  IF(RNDM(-1)-0.5)6301,4710,4710
C----------------------------------------- D1=16
4670  WRITE(6,"(' יHהEךדש נOKAתAלי BAM,חהE הOגשTר Eהש')")
      G=G+14
4710  IF(RNDM(-1).GT.0.55)GO TO 404
      IP9=RNDM(-1)*11.+1
      GOTO(3960,3625,3740,3820,4220,4340,4490,2901,4670,4130,3850)IP9
C ********* חOPש  *************
4714  IF(M.LE.950)GO TO 1230
      W=9-((M/100.-15.)**2+72.)/((M/100.-15.)**2+12.)
      IF(10*RNDM(-1)-W)4730,4730,4860
4730  WRITE(6,"(' Bש HAXOהיTECר B חOPAX')")
      IF(RNDM(-1)-0.1)4750,4750,4780
4750  WRITE(6,"(' Bש CגיליCר C הOPOחי י נOTEPסלי BPEMס HA EE נOיCK')")
      M=M-100
      GOTO 4860
4780  IF(RNDM(-1)-0.11)4790,4790,4840
4790  WRITE(6,"(' זYPחOH נOBPEצהEH')")
      M1=M1-5
      B=B-200
      M=M-40-30*RNDM(-1)
      GOTO 4860
4840  WRITE(6,"(' YMEHרûילACר CKOPOCTר נEPEהBיצEHיס')")
      M=M-65-(RNDM(-1))/0.02
4860  IF(G1.EQ.1)GO TO 4901
      G1=1
      IF(RNDM(-1)-0.8)4970,4890,4890
4890  WRITE(6,"(' BAM YהA‏HO YהAלOCר נPEOהOלETר חOPש')")
4901  IF(M.LT.1700)GO TO 4940
      IF(G2.EQ.1)GO TO 4940
      G2=1
      IF(RNDM(-1).LT.0.7)GO TO 4970
4940  IF(M.GT.950)GO TO 1230
      M9=1
      GOTO 1230
4970  WRITE(6,"(' CHEצHAס גYPס B חOPAX ',
     *' - נOTEPסHO BPEMס י ‏ACTר תAנACOB')")
      L1=1
      G=G-25
      M1=M1-10
      B=B-300
      M=M-40-40*RNDM(-1)
      IF(C-18-2*RNDM(-1))6301,4490,4490
C ********** CMEPTר **********
5060  WRITE(6,"(' Y BAC KOH‏ילACר EהA - Bש YMEPלי OT חOלOהA')")
      GOTO 5170
5080  T=0
      WRITE(6,"(' BAM HE‏EM תAנלATיTר הOKTOPY')")
      GOTO 5120
C
5110  WRITE(6,"(' Y BAC KOH‏יליCר לEKAPCTBA')")
5120  WRITE(6,"(' Bש YMEPלי OT')")
      IF(K8.EQ.1)GO TO 5160
      WRITE(6,"(' BOCנAלEHיס לEחKיX'/)")
      GOTO 5170
5160  WRITE(6,"(' PAH'/)")
5170  WRITE(6,"(/,' BBיהY TAKOך CיTYAדיי',/
     *' ECTר HECKOלרKO HEגOלרûיX זOPMAלרHOCTEך')")
      WRITE(6,"(*' XOTיTE לי Bש KPACיBשך חPOג?')")
      READ(5,860)WG
      WRITE(6,"(' XOTיTE לי Bש נשûHשE נOXOPOHש?')")
      READ(5,860)WP
      WRITE(6,"(' COOג‎יTר לי BAûיM POהCTBEHHיKAM?')")
      READ(5,860)N
      IF(N.GE.1)GO TO 5310
      WRITE(6,"(' OHי גYהYT גECנOKOיTרCס O BAC',/)")
      GOTO 5320
5310  WRITE(6,"(' תA TEלEחPAMMY C BAC 4.5 הOל.'/)")
5320  IF(WG.NE.0)GO TO 5324
      WRITE(6,"(' BAC נOהBEPחלי KPEMAדיי',/)")
      GOTO 5325
5324  WRITE(6,"(' BAC נOלOצילי B דיHKOBשך חPOג',/)")
5325  IF(WP.NE.0)GO TO 5328
      IF(WG.NE.0)GO TO 5329
      WRITE(6,"(' BAû נEנEל PAתBEסלי נO BETPY',/)")
      GOTO 5330
5328  WRITE(6,"(' COCTOסליCר נשûHשE נOXOPOHש',/)")
      GOTO 5330
5329  WRITE(6,"(' חPOג BשCTABלEH HA ',
     *' דEHTPAלרHOך נלO‎Aהי B ûTATE OPEחOH')")
5330  WRITE(6,"(' גOלרûOE CנACיגO תA יHזOPMAדיא')"      )
      WRITE(6,"(' MOצET גשTר BAM גOלרûE נOBEתET B CלEהY‎יך PAת',//
     *' C YBAצEHיEM',/' BלACTי ûTATA OPEחOH',/)")
      GOTO 1
C-----------------------------------------------
5430  G9=(2040.-M2)/(M-M2)
      G=G+(1-G9)*(8+5*E)
      WRITE(6,"(/,' Bש HAKOHEד נPיגשלי B OPEחOH',/
     *' נPEOהOלEB 2040 Mילר',//' Bש HACTOס‎יך ניOHEP',//)")
      G9=G9*14
      D3=D3*14+G9
      G9=G9+1
      IF(G9.GE.8)G9=G9-7
      WRITE(6,"(/XA12)")WEEK(G9)
      IF(D3.GT.124)GO TO 5740
      D3=D3-93
      WRITE(6,
     *"(3X,I5,' יאלס 1847 ח.')")D3
      GOTO 5920
5740  IF(D3.GT.155)GO TO 5780
      D3=D3-124
      WRITE(6,
     *"(3X,I5,' ABחYCTA 1847 ח.')")D3
      GOTO 5920
5780  IF(D3.GT.185)GO TO 5820
      D3=D3-155
      WRITE(6,
     *"(3X,I5,' CEHTסגPס 1847 ח.')")D3
      GOTO 5920
5820  IF(D3.GT.216)GO TO 5860
      D3=D3-185
      WRITE(6,
     *"(3X,I5,' OKTסגPס 1847 ח.')")D3
      GOTO 5920
5860  IF(D3.GT.246)GO TO 5901
      D3=D3-216
      WRITE(6,
     *"(3X,I5,' HOסגPס 1847 ח.')")D3
      GOTO 5920
5901  D3=D3-246
      WRITE(6,
     *"(3X,I5,' הEKAגPס 1847 ח.')")D3
5920  IF(B.LE.0)   B=0
      IF(C.LE.0)   C=0
      IF(M1.LE.0) M1=0
      IF(T.LE.0)   T=0
      IF(G.LE.0)   G=0
      WRITE(6,12040)G,B,C,M1,T,Z
      WRITE(6,"(' נPEתיהEHT נEPEהAET BAM CBOE נOתהPABלEHיE',/
     *' י צEלAET BAM YCנEXA HA HOBOM MECTE')")
      GOTO 1
C-----------------------------------------------
6301  IF(100*RNDM(-1)-10.LT.35*(E-1))GO TO 6370
      IF(100.*RNDM(-1).LT.100+(40./(4.**(E-1))))GO TO 6410
      WRITE(6,"(' Bש CEPרEתHO תAגOלEלי',/
     *' HYצHO OCTAHOBיTרCס י נOלE‏יTרCס')")
      M1=M1-10
      S4=1
      GOTO 6440
6370  WRITE(6,"(' Y BAC לEחKOE HEהOMOחAHיE')")
      M=M-25
      M1=M1-2
      GOTO 6440
6410  WRITE(6,"(' Bש נלOXO CEגס ‏YBCTBYETE')")
      M=M-25
      M1=M1-5
6440  IF(M1.LT.0)GO TO 5110
      IF(L1.EQ.1)GO TO 4940
      GOTO 4710
C ***** הEPEBHס ******
404   IF(RNDM(-1).LE.0.8)GO TO 181
      IV1=RNDM(-1)*3+1
      IV2=VARD(IV1)
      I1=(RNDM(-1)*(27-IV1)+1)/4.
      I2=(RNDM(-1)*(27-IV1)+1)/4.
      I3=(RNDM(-1)*(27-IV1)+1)/4.
      WRITE(6,
     *"(/,' Bש BOûלי B לAחEPר יHהEךדOB נלEMEHי ',A6,/
     *' גYהרTE OCOגEHHO BHיMATEלרHש!')")IV2
      WRITE(6,"(' נOXOצE ‏TO יHהEךדש HACTPOEHש')")
      IS5=0
      IF(RNDM(-1).LT.0.6)GO TO 102
      WRITE(6,"(' HE')")
      IS5=1
102   WRITE(6,"(' הPYצEלאגHO')")
      IV0=RNDM(-1)*9+1
      WRITE(6,
     *"(//,' BAC BשûEל נOנPיBETCTBOBATר BOצהר נלEMEHי',/
     *10X,' *** ',A12,' *** ',/)")VADA(IV0)
      IV1=IV1*IV0
      IF(RNDM(-1).LT.0.8)GO TO 110
      WRITE(6,"(' K BAM נOהOûEל ûAMAH.',/
     *' XOTיTE תA 10 הOל. YתHATר CBOE גYהY‎EE ?')")
      READ(5,860)I
      IF(I.EQ.1)GO TO 109
      WRITE(6,"(' KAK YחOהHO...גEPEחיCר')")
      GOTO 110
109   T=T-10
      IF(IV1.LE.9)GO TO 113
      IF(IV1-12)115,115,116
113   WRITE(6,"(' Bש נOנAלי B MיPHOE נלEMס.תA 10 נATPOHOB',/
     *' MOצHO BשMEHסTר:')")
      WRITE(6,119)I1,I2,I3
119   FORMAT(' OהEצהש תA',I6,' הOל.',/
     *' לEKAPCTB תA ',I6,' הOל.',/
     *' Eהש תA ',I6,' הOל.')
      GOTO 110
115   WRITE(6,"(' Bש HAXOהיTECר B נלEMEHי, תAKOH‏יBûיM BOךHY.',/
     *' תA 10 נATPOHOB MOצHO BשMEHסTר:')")
      WRITE(6,119)I1,I2,I3
      GOTO 110
116   WRITE(6,"(' Bש...BOû...û...ûלי B BOיHCTBEHHOE נלEMס.',/
     *' לY‏ûE HE MEHסTרCס י YגEחATר')")
110   WRITE(6,
     *"(' BAûA TAKTיKA:(1) YגEחATר,(2) ATAKOBATר,',/
     *' (3) תA‎י‎ATרCס,(4) PAתחOBAPיBATר?')")
      READ(5,860)I
      IF(I-2)122,123,124
124   IF(I-4)125,126,126
C--- YגEחATר
122   IF(RNDM(-1).LT.0.8)GO TO 127
      WRITE(6,"(' Bש C HEKOTOPשMי נOTEPסMי CנACלי CBOך CKAלרנ')")
      B=B-20*RNDM(-1)
      GOTO 181
127   WRITE(6,"(' יHהEךדש CXBATילי BAC.נPיהETCס הATר BשKYנ')")
      G=G-20
      B=B-40
      C=C-10
      M1=M1-5
      M=M-25
      GOTO 181
C---  ATAKA
123   CALL D61
      B=B-10*RNDM(-1)
      CALL PATRON
      IF(P.NE.0)GOTO 5170
      IF(B1.GT.1)GO TO 141
      WRITE(6,"(' BOצהר YגיT.נAHיKA B נלEMEHי')")
      GOTO 132
141   WRITE(6,"(' יHהEEד HATסחיBAET לYK')")
      CALL D61
      B=B-10*RNDM(-1)-2
      CALL PATRON
      IF(P.NE.0)GOTO 5170
      IF(B1.LE.1)GO TO 132
133   WRITE(6,"(' נPOMAX...Bש PAHEHש...CTPEלA נPOûלA HACKBOתר')")
      B=B-10*RNDM(-1)
      M1=M1-5
      WRITE(6,"(' ית-תA הEPEBרEB נOKAתAלACר PYKA C KOנרEM')")
      CALL D61
      B=B-10*RNDM(-1)
      CALL PATRON
      IF(P.NE.0)GOTO 5170
      IF(B1.LT.1)GO TO 132
      WRITE(6,"(' HA CBOך BEK נOCTPEלסלי - נOPA י נOMיPATר')")
      GOTO 5170
132   WRITE(6,"(' HEנלOXO CTPEלסEûר - נYTר CBOגOהEH...גEחי...')")
      M=M+10
      GOTO 181
C---  תA‎יTA
125   IF(IS5.NE.1)GO TO 142
      IF(RNDM(-1).GT.0.8)GO TO 145
144   CALL D61
      B=B-15*RNDM(-1)-2
      CALL PATRON
      IF(P.NE.0)GOTO 5170
      IF(B1.LE.1)GO TO 132
      GOTO 133
145   WRITE(6,"(' יHהEךדש גשלי MיPHשE')")
      WRITE(6,
     *"(' BAûA TAKTיKA:(1) YגEחATר,(2) ATAKOBATר,',/
     *' (3) תA‎י‎ATרCס,(4) PAתחOBAPיBATר?')")
      READ(5,860)I
      IF(I-1)178,178,126
142   IF(RNDM(-1)-0.8)145,145,144
C     PAתחOBAPיBATר
126   IF(IS5.EQ.0)GOTO 293
      IF(RNDM(-1)-0.8)290,290,294
290   WRITE(6,"(' ‏TO-TO יHהEךדAM HE XO‏ETCס PAתחOBAPיBATר !')")
      GOTO 123
293   IF(RNDM(-1).LE.0.2)GO TO 290
294   WRITE(6,"(' ‏EM גOלרûE CYMEETE CKAתATר יHהEךCKיX CלOB,TEM',/
     *' לY‏ûיך PAתMEH MOצHO נPOיתBECTי')")
      K2=0
      K1=0
      DO 150 I=1,5
      CALL D61
      IF(B1.GT.1)GO TO 9152
      K1=K1+1
      WRITE(6,"(' CKAתAHO CלOB=',I6)")K1
      GOTO 150
9152  K2=K2+1
      IF(K2-2)150,152,152
C     IF(K1.GE.4)GO TO 156
152   WRITE(6,"(' CלAגO PAתחOBAPיBAEûר...נOהY‏יCר',/
     *' TPYהHO גYהET MEHסTרCס...')")
      GOTO 155
150   CONTINUE
155   IF(K1.LT.4)GO TO 157
156   WRITE(6,"(' XOPOûO חOBOPיûר - יHהEךדש נPEהלAחAאT',/
     *' MEHסTר תOלOTO HA נATPOHש (10 נATPOHOB-1 YHדיס)')")
      WRITE(6,"(' HA CKOלרKO MEHסEûר?')")
      CALL DT24
      Z=Z+P/10.
157   I1=I1+K1/2.
      I2=I2+K1/2.
      I3=I3+K1/2.
      IP=0
      WRITE(6,
     *"(' CKOלרKO נATPOHOB Bש XOTיTE נOMEHסTר?',/
     *' תA 10 נATP. - Eהש HA ',I6,' הOל.')")I1
      CALL DT24
      IF(P.EQ.0)GOTO 173
      IP=1
173   G=G+I1*P/10.
      WRITE(6,
     *"(' תA 10 נATP. - OהEצהש HA ',I6,' הOל.')")I2
      CALL DT24
      IF(P.EQ.0)GOTO 174
      IP=1
174   C=C+I2*P/10.
      WRITE(6,
     *"(' תA 10 נATP. - PAתHOE HA ',I6,' הOל.')")I3
      CALL DT24
      IF(P.EQ.0)GOTO 175
      IP=1
175   M1=M1+I3*P/10.
      IF(IV1.LE.12)GO TO 176
      IF(IP.NE.1)GOTO 178
179   WRITE(6,"(' Bש יX PAתOתלילי')")
      GOTO 123
176   IF(IP.EQ.0)GOTO 179
178   WRITE(6,"(' Bש CKYPילי TPYגKY MיPA,',/
     *' יHהEךדש KAצETCס BAMי')")
      IS6=0
      IF(IS5.NE.1)GO TO 163
      IF(RNDM(-1).GT.0.8)GO TO 166
165   WRITE(6,"(' HE')")
      IS6=1
      GOTO 166
163   IF(RNDM(-1).GT.0.8)GO TO 165
166   WRITE(6,"(' הOBOלרHש')")
      IF(IS6.NE.1)GO TO 169
      WRITE(6,171)
171   FORMAT(' נPיהETCס PAתחOBAPיBATר "הPYחיM סתשKOM"')
      GOTO 123
169   WRITE(6,1691)
1691  FORMAT(' BAC נPOBOצAET BCE נלEMס',/
     *' BOצהר HA הOPOחY נOהAPיל',/
     *' OCליHHOE YXO HA C‏ACTרE',/)
      WRITE(6,"(' יHהEךCKAס הEPEBHס נOתAהי...',//)")
181   IF(RNDM(-1)-0.3)499,499,183
183   I=RNDM(-1)*6+1
      GOTO(184,186,188,9212,9220,236)I
184   WRITE(6,"(' חOPיT נPEPיס...Bש CBEPHYלי C נYTי...')")
      M=M-45
      GOTO 499
186   WRITE(6,"(' סהOBיTAס PEKA...ניTר HEלרתס...Bש OCלAגלי.')")
      M=M-30
      M1=M1-10
      GOTO 499
188   WRITE(6,
     *"(' נPOנACTר...‏TO הEלATר...',/
     *' (1) נPשחATר,(2) B OגXOה,(3) ‏EPEת YתKיך MOCT?')")
      READ(5,860)I
      IF(I-2)9201,9202,9203
9201  IF(RNDM(-1).GT.0.5)GO TO 9205
      WRITE(6,"(' BAM YהAלOCר...HO BCE יMY‎ECTBO HE נEPEגPOCילי')")
      B=B-20
      G=G-5
      C=C-5
      M=M+15
      GOTO 499
9205  WRITE(6,"(' üX...HEYהA‏A...HO BCE צE CXBATיליCר תA BETKY',/
     *' ...נOTEPסHO ‏ACTר יMY‎ECTBA')")
      B=B-40
      G=G-10
      C=C-10
      M=M+5
      GOTO 499
9202  WRITE(6,"(' XOה HE CMEלשך,HO HAהEצHשך - נOTEPסHO MHOחO BPEMEHי')")
      M=M-150
      GOTO 499
9203  IF(RNDM(-1).LT.0.6)GO TO 9208
      WRITE(6,"(' C‏ACTליBO נPOûלי...')")
      M=M+15
      GOTO 499
9208  WRITE(6,"(' KAצETCס MOCTיK HE BשהEPצAל',
     *' ...גOלרûAס נOTEPס יMY‎ECTBA')")
      B=B-60
      G=G-10
      C=C-10
      M1=M1-5
      M=M+5
      GOTO 499
9212  WRITE(6,"(' ...PE‏KA...HAהO נEPEנלשTר.')")
      WRITE(6,"(' HAנAהEHיE AלליחATOPOB...')")
      CALL D61
      B=B-25*RNDM(-1)
      CALL PATRON
      IF(P.NE.0)GOTO 5170
      IF(B1.GT.1)GO TO 9216
      WRITE(6,"(' ...XOPOûO...נOנAלי...AלליחATOPש OגEהAאT YגיTשM')")
      M=M+10
      GOTO 499
9216  WRITE(6,"(' נPOMAX...AלליחATOP KOE-‏TO OTKYCיל...',////)")
      WRITE(6,"(' ית תAנACOB KOHE‏HO...A Bש ‏TO נOהYMAלי?..',//)")
      G=G-5
      C=C-5
      M1=M1-3
      M=M+5
      GOTO 499
9220  WRITE(6,"(' OתEPO ûיPOKOE - לY‏ûE גש נEPEנלשTר')")
      WRITE(6,"(' BAûי הEךCTBיס:(1) נלשTר,(2) B OגXOה?')")
      READ(5,860)I
      IF(I.GE.2)GO TO 9202
      IF(RNDM(-1).GE.0.6)GO TO 225
      WRITE(6,"(' HEYהAלOCר...TOHיTE...‏ACTר תAנACOB YTOHYלA',/
     *' ...יHהEךדש C ניPOחOך BשTA‎ילי BAC')")
      B=B-30
      G=G-5
      C=C-10
      M1=M1-2
      M=M+5
      WRITE(6,"(/,' CKOלרKO נATPOHOB נOהAPיTE יHהEךדAM תA CנACEHיE?')")
      READ(5,860)I
      B=B-I
      I1=25*RNDM(-1)+10
      IF(I.GT.I1)GO TO 233
      WRITE(6,"(' OגיצAEûר HA‏AלרHיK...גEPEחיCר')")
      CALL D61
      B=B-10*RNDM(-1)-5
      CALL PATRON
      IF(P.NE.0)GOTO 5170
      IF(B1.GT.1)GO TO 833
      WRITE(6,"(' Bש יX Yגילי...')")
      GOTO 499
833   WRITE(6,"(' נPOMAX..Bש PAHEHש..יHהEךדש תAגPAלי ‏ACTר תAנACOB')")
      B=B-50
      G=G-6
      C=C-5
      M1=M1-5
      M=M+5
      GOTO 499
233   WRITE(6,"(' OHי נPיHסלי BAû הAP')")
      GOTO 499
225   WRITE(6,"(' Bש C‏ACTליBO נEPEנPABיליCר',
     *' - HO יCנOPTילי ‏ACTר Eהש')")
      G=G-5
      M=M+25
      GOTO 499
236   WRITE(6,"(' YתKAס חOPHAס TPOנA...‏ACTר יMY‎ECTBA COPBAלACר...')")
      B=B-25
      G=G-5
      C=C-5
      M1=M1-3
      M=M+5
      IF(RNDM(-1)-0.8)239,239,499
239   WRITE(6,"(' נPשחAאT סחYAPש...')")
      CALL D61
      B=B-10
      CALL PATRON
      IF(P.NE.0)GOTO 5170
      IF(B1.GT.1)GO TO 242
      WRITE(6,"(' OTלי‏HO...MOצETE נOהAPיTר ûKYPKי POהHשM')")
      GOTO 499
242   WRITE(6,"(' נלOXO CTPEלסETE...גשK תAPEתAH')")
      M=M-100
      B=B-10*RNDM(-1)-2
C ******* חOPOהOK הצO ********
499   IF(RNDM(-1).LE.0.6)GO TO 4714
      WRITE(6,"(' Bש BOûלי B חOPOהOK י תAûלי B PECTOPAH‏יK',/
     *'          גEûEHHOחO הצO ')")
      WRITE(6,"(' XOTיTE CשחPATר:(1) B KAPTש,(2) B PYלETKY,',/
     *' (3) נOCTPEלסTר B TיPE,(4) BשךTי י נPOהOלצATר נYTר?')")
      READ(5,860)I
      IF(I.EQ.0.OR.I.GT.4)I=4
      GO TO (501,502,503,560)I
  501 CALL POKER
      GO TO 560
C..............................................
C        P Y ל E T K A
C..............................................
502   WRITE(6,"(' נPABילA HYצHש? (1-הA,0-HET)')")
      READ(5,860)I
      IF(I.NE.1)GO TO 508
      WRITE(6,5021)
5021  FORMAT(' CTABרTו X הOל. HA ‏יCלO C 0-36',/
     *' O - גEPET BECר גAHK י 36 PAת YBEלי‏יBAET CTABKY',/
     *' 1-36  - 36 PAת YBEלי‏יBAET BשיחPשû')
508   K=0
911   WRITE(6,"(' CTABKA B הOל.')")
      CALL D24
      IF(P.LT.0)GO TO 508
      IF(K.GE.20)GO TO 912
      K=K+1
514   WRITE(6,"(' HA KAKOE ‏יCלO?')")
      READ(5,860)ISK
      IF(ISK.GT.36)GO TO 514
      MDOL(K)=P
      MSK(K)=ISK
      WRITE(6,"(' CTABKי תAKOH‏EHש? (1-הA,0-HET)')")
      READ(5,860)I
      IF(I.NE.1)GO TO 911
912   I2=RNDM(-1)*37
      WRITE(6,"(' גAHK - BשיחPAלO ‏יCלO ',I4/)")I2
      I3=0
      DO 523 I=1,K
      IF(MSK(I).NE.I2)GO TO 523
      I3=MDOL(K)*36
      IF(I2.EQ.0)I3=I3*10
      T=T+I3
      WRITE(6,"(' נOתהPABלסא Bש COPBAלי גAHK ',I6,' הOל.')")I3
      GOTO 527
523   CONTINUE
      WRITE(6,"(' צAלר HE BשיחPAלי '/
     *' XOTיTE E‎E? (1),HET (0)')")
      READ(5,860)I
      IF(I.EQ.1)GO TO 508
C----------------------------------------------
560   IF(RNDM(-1).GT.0.8)GO TO 535
527   WRITE(6,5271)
5271  FORMAT(' הצO נPEהלAחAET BAM נOBECEליTרCס'/
     *' BAûA TAKTיKA:(1) BשניTר,(2) נOךTי K הEBYûKAM,',/
     *' (3) BשךTי י נPOהOלצATר נYTר?')
      READ(5,860)I
      IF(I-2)931,532,533
931   WRITE(6,"(' ‏TO גYהEM ניTר:(1) MAPTיHי-2 הOל.,(2) גPüHהי-3 הOל.,',/
     *' (3) BיCKי-5 הOל. ?')")
      READ(5,860)I
      IF(I-2)834,835,836
834   T=T-2
      WRITE(6,"(' CלAגO נרEûר!!!')")
      GOTO 838
835   T=T-3
      GOTO 838
836   T=T-5
838   WRITE(6,8381)
8381  FORMAT(' Bש נO‏Tי נרסHש ,HO PAתBE HAנרEûCס',/
     *' KAK CBיHרס תA TAKיE הEHרחי!')
      IF(RNDM(-1).GE.0.6)GO TO 527
535   WRITE(6,"(' Bש נPOהOלצAETE נYTר',/)")
      GOTO 4714
532   WRITE(6,5321)
5321  FORMAT(' KAKיE BAM HPABסTCס:(1) TOלCTשE,(2) XYהשE',/
     *' (3) CPEהHיE?')
      READ(5,860)IST
      WRITE(6,
     *"(' KAKיE BOלOCש:(1) CBETלשE,(2) ‏EPHשE,(3) KOPי‏HEBשE?')")
      READ(5,860)ISP
      IST1=RNDM(-1)*3+1
      ISP1=RNDM(-1)*3+1
      IM=0
      IF(IST1.EQ.IST) IM=1
      IF(ISP1.EQ.ISP) IM=IM+1
      IF(IM-1)545,544,543
545   T=T-25
      WRITE(6,"(' üTO CTOילO BAM 25 הOל.')")
      GOTO 535
544   WRITE(6,"(' üTO BEהר צEHA הצO...גEPEחיCר..')")
556   WRITE(6,"(' XOתסיH BשHיMAET KOלרT..')")
      CALL D61
      B=B-10
      CALL PATRON
      IF(P.NE.0)GOTO 5170
      IF(B1.LE.1)GO TO 548
      WRITE(6,"(' נלOXO CTPEלסEûר...נOלY‏יל 2 CBיHדA B PYKY,',/
     *' נלATA הOKTOPY 20 הOל.')")
      T=T-20
      GOTO 535
548   WRITE(6,"(' XOPOûO CTPEלסEûר - PAHיל XOתסיHA...י YגEחAEûר')")
      GOTO 535
543   WRITE(6,"(' ‏TO צE Bש הEלAETE? - üTO BEהר TO‏HAס KOניס',/
     *' BAûEך צEHש - נלATיTר HE HAהO !',///)")
      GOTO 535
533   IF(RNDM(-1).GT.0.8)GO TO 535
      WRITE(6,"(' Bש OגיהEלי XOתסיHA...')")
      GOTO 556
503   WRITE(6,"(' BשCTPEל - 1 הOל.,נPית תA 3 נOנAהAHיס -10 הOל.')")
      I1=0
C
      DO 559 I=1,3
      CALL D61
      B=B-10
      T=T-1
      IF(B1.LE.1)I1=I1+1
559   CONTINUE
      IF(I1.NE.3)GOTO 535
      WRITE(6,"(' Bש BשחPAלי נPית !')")
      T=T+10
      GOTO 560
      END
      SUBROUTINE D24
      REAL*8 RNDM
      IMPLICIT INTEGER*2 (A-Z)
      COMMON/ORE1/ B1,D9,P,T,B3,B,RAIDE(30)
2401  READ(5,11860)P
11860 FORMAT(I5)
      IF(P.LE.0)GO TO 2405
      T=T-P
      IF(T.GE.0)GO TO 2405
      WRITE(6,12430)
12430 FORMAT(' Y BAC HE XBATAET הEHEח')
      T=T+P
      P=0
      GOTO 2401
2405  CONTINUE
      RETURN
      END
      SUBROUTINE DT24
      REAL*8 RNDM
      IMPLICIT INTEGER*2 (A-Z)
      COMMON/ORE1/ B1,D9,P,T,B3,B,RAIDE(30)
2401  READ(5,11860)P
11860 FORMAT(I5)
      IF(P.LE.0)GO TO 2405
      B=B-P
      IF(B.GE.0)GO TO 2405
      WRITE(6,12430)
12430 FORMAT(' Y BAC HE XBATAET נATPOHOB')
      B=B+P
      P=0
      GOTO 2401
2405  CONTINUE
      RETURN
      END
      SUBROUTINE PATRON
      REAL*8 RNDM
      IMPLICIT INTEGER*2 (A-Z)
      COMMON/ORE1/ B1,D9,P,T,B3,B,RAIDE
      P=0
      IF(B.GE.0)GO TO 301
      WRITE(6,302)
302   FORMAT(' Y BAC KOH‏יליCר נATPOHש - OHי BAC נPיKOH‏ילי')
      P=5170
301   CONTINUE
      RETURN
      END
      SUBROUTINE D61
      REAL*8 RNDM
      IMPLICIT INTEGER (A-Z)
      COMMON/ORE1/ B1,D9,P,T,B3,B,RAIDE
      INTEGER*(2) D9,B1,B3,SY(4),CY(4),RAIDE(30)
      DATA RAIDE/'A','ג','B','ח','ה','E','ז','ת','צ','X','י',
     *'ך','K','ל','M','H','O','נ','P','C','T','Y','ד',
     *'û','‎','‏','ü','ש','ר','א'/
6101  WRITE(6,16101)
16101 FORMAT(' נE‏ATAךTE')
      DO 16102 I=1,4
      IRAID=INT(30*RNDM(-1)+1)
      SY(I)=RAIDE(IRAID)
16102 CONTINUE
      WRITE(6,16110)(SY(I),I=1,4)
6108  WRITE(6,16108)
16108 FORMAT(/)
      B3=TIME(0)
6110  READ(5,16110)(CY(I),I=1,4)
16110 FORMAT(4A1)
      B1=TIME(0)
      WRITE(6,16108)
      IBK=(B1-B3)/35.
      B1=IBK-(D9-1.)
      B3=IBK
      IF(B1.LT.0)B1=0
      DO 16116 I=1,4
      IF(CY(I).NE.SY(I))B1=9
16116 CONTINUE
      RETURN
      END
