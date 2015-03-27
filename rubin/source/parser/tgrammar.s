.comm	_Qt,112
.comm	_i,2
.comm	_aptr,2
.comm	_permbuf,4
.comm	_modbuf,34
.globl
.bss
_hqmbuf:.=.+2
.comm	_yylval,2
.comm	_yyval,2
.globl	_Keyword
.data
_Keyword:L1
466
12
.even
L2
422
0
.even
L3
464
141
.even
L4
470
10
.even
L5
401
177777
.even
L6
466
7
.even
L7
430
0
.even
L8
466
3
.even
L9
470
4
.even
L10
470
5
.even
L11
423
5
.even
L12
467
14
.even
L13
402
177777
.even
L14
466
4
.even
L15
470
0
.even
L16
470
1
.even
L17
403
177777
.even
L18
466
21
.even
L19
416
177777
.even
L20
404
177777
.even
L21
405
177777
.even
L22
466
13
.even
L23
466
17
.even
L24
466
20
.even
L25
424
0
.even
L26
467
15
.even
L27
467
16
.even
L28
406
177777
.even
L29
425
0
.even
L30
407
177777
.even
L31
466
14
.even
L32
466
15
.even
L33
466
16
.even
L34
421
0
.even
L35
426
0
.even
L36
431
5
.even
L37
466
6
.even
L38
470
7
.even
L39
470
6
.even
L40
467
13
.even
L41
410
177777
.even
L42
465
2
.even
L43
432
0
.even
L44
433
0
.even
L45
434
0
.even
L46
464
157
.even
L47
417
0
.even
L48
411
177777
.even
L49
412
177777
.even
L50
413
177777
.even
L51
414
177777
.even
L52
415
177777
.even
L53
466
24
.even
L54
466
23
.even
L55
467
25
.even
L56
466
10
.even
L57
466
11
.even
L58
470
2
.even
L59
470
3
.even
L60
466
22
.even
L61
435
0
.even
L62
467
17
.even
L63
467
20
.even
L64
427
0
.even
.globl
L65
436
0
.even
L66
420
0
.even
L67
437
0
.even
.globl	_Keyent
_Keyent:103
.globl	_Optab
_Optab:L68
450
0
.even
L69
451
0
.even
L70
452
0
.even
L71
453
0
.even
L72
441
0
.even
L73
446
0
.even
L74
447
0
.even
L75
455
0
.even
L76
456
0
.even
L77
454
0
.even
L78
457
0
.even
L79
457
1
.even
L80
460
2
.even
L81
460
3
.even
L82
461
4
.even
L83
462
11
.even
L84
462
12
.even
L85
462
7
.even
L86
462
10
.even
L87
463
6
.even
L88
431
5
.even
0
.=.+4
.even
.globl	_Coptab
_Coptab:L89
0
.byte 143
.byte 2
.even
L90
1
.byte 143
.byte 2
.even
L91
2
.byte 144
.byte 4
.even
L92
3
.byte 155
.byte 4
.even
0
0
.byte 0
.byte 0
.even
.globl	_Tokens
_Tokens:441
455
456
442
443
444
445
440
446
447
.even
.globl	_yyerror
.text
_yyerror:~~yyerror:jsr	r5,csv
~errmessage=4
cmpb	$171,*4(r5)
jeq	L96
clr	(sp)
clr	-(sp)
mov	$5050,-(sp)
jbr	L20001
L96:clr	(sp)
clr	-(sp)
mov	$5360,-(sp)
L20001:jsr	pc,*$_par_err
cmp	(sp)+,(sp)+
jmp	cret
.globl	_par_err
_par_err:~~par_error:jsr	r5,csv
~num=4
~result=6
~a=10
~b=12
~c=14
sub	$36,sp
~buff=177732
~buf=r4
jsr	pc,_resetp
mov	r5,r4
add	$-46,r4
mov	$3,_yyerrfl
tst	_Err_cur
jeq	L101
cmp	$1,6(r5)
jne	L100
jsr	pc,_endgo
clr	(sp)
jsr	pc,*$_error
L101:inc	_Err_fnd
mov	$1,_Err_cur
cmp	$5050,4(r5)
jeq	L10000
cmp	$4704,4(r5)
jne	L102
L10000:mov	r4,10(r5)
clr	12(r5)
mov	_Lastok,r0
jeq	L110
cmp	r0,$441
jeq	L109
cmp	r0,$442
jeq	L105
cmp	r0,$443
jeq	L106
cmp	r0,$444
jeq	L107
cmp	r0,$445
jeq	L108
mov	$L113,(sp)
jsr	pc,*$_syserr
jbr	L103
L105:mov	r4,(sp)
mov	*2+_Lastok,-(sp)
jsr	pc,*$_itoa
L20014:tst	(sp)+
jbr	L103
L106:mov	r4,(sp)
mov	2+_Lastok,r0
mov	2(r0),r1
mov	(r0),r0
mov	r1,-(sp)
mov	r0,-(sp)
jsr	pc,_locv
cmp	(sp)+,(sp)+
mov	r0,-(sp)
L20015:jsr	pc,*$_smove
jbr	L20014
L107:mov	$156,(sp)
mov	$3,-(sp)
mov	$12,-(sp)
mov	r4,-(sp)
movof	*2+_Lastok,r0
L20018:movf	r0,-(sp)
jsr	pc,*$_ftoa
add	$16,sp
jbr	L103
L108:mov	$156,(sp)
mov	$3,-(sp)
mov	$12,-(sp)
mov	r4,-(sp)
movf	*2+_Lastok,r0
jbr	L20018
L109:mov	r4,(sp)
mov	2+_Lastok,-(sp)
jbr	L20015
L110:mov	$L111,10(r5)
L103:add	_Opflag,4(r5)
L102:cmp	$1,6(r5)
clr	(sp)
mov	14(r5),-(sp)
mov	12(r5),-(sp)
mov	10(r5),-(sp)
mov	_yyline,-(sp)
jsr	pc,_iocv
tst	(sp)+
mov	r0,-(sp)
mov	4(r5),-(sp)
jsr	pc,*$_error
add	$12,sp
L100:jmp	cret
.globl	_neederr
_neederr:~~neederr:jsr	r5,csv
~errnum=4
clr	(sp)
clr	-(sp)
mov	4(r5),-(sp)
jsr	pc,*$_par_err
cmp	(sp)+,(sp)+
jmp	cret
.globl
.data
.text
_yyexca:177777
0
0
3
177776
0
177777
1
0
177777
177776
0
177777
46
451
60
177776
64
177777
320
452
207
177776
217
.globl
_yyact:302
507
230
320
317
311
312
313
314
315
316
451
277
356
363
517
364
363
322
365
364
363
367
500
300
306
307
310
320
317
311
312
313
314
315
316
367
372
477
531
434
466
516
322
365
364
363
370
366
75
306
307
310
232
454
433
421
524
365
364
363
370
366
511
.globl
356
365
364
363
334
365
364
363
465
430
104
105
427
376
331
365
364
363
464
232
140
375
365
364
363
162
434
374
521
126
365
364
363
337
325
177
340
324
255
221
220
373
222
125
127
164
263
202
201
132
472
210
453
216
335
211
206
177
410
513
514
121
122
76
.globl
126
275
166
165
167
532
251
117
530
523
504
76
412
462
125
127
426
275
252
170
114
445
113
116
270
76
74
461
401
526
425
515
452
475
422
76
352
76
333
402
403
235
402
353
176
261
254
241
223
242
205
246
250
204
131
100
232
417
354
227
103
101
102
527
.globl
360
160
157
522
347
136
371
214
161
273
137
134
135
153
152
111
107
501
345
154
174
473
110
400
25
52
53
54
55
56
60
70
62
51
63
64
65
66
57
330
150
332
266
257
240
327
351
424
406
460
405
435
436
437
440
146
276
326
145
441
442
443
444
274
.globl
323
147
144
172
171
173
321
234
143
163
175
124
142
73
272
407
411
413
3
414
72
2
271
71
47
26
450
416
420
344
415
343
260
42
61
40
130
37
123
467
36
112
115
34
33
31
247
245
476
404
244
30
231
120
505
510
35
256
133
41
457
456
217
455
.globl
423
346
215
141
43
67
512
503
50
520
200
156
155
46
151
45
106
203
510
525
32
77
502
27
411
471
44
24
23
22
21
207
20
17
16
15
474
14
213
13
212
12
11
10
7
6
5
4
305
304
506
362
224
237
303
225
301
226
233
236
350
262
1
0
.globl
0
0
0
0
0
0
0
0
0
0
243
0
0
0
0
0
0
0
0
0
253
0
0
0
0
0
265
264
0
0
267
0
336
470
341
0
0
0
0
342
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
357
361
355
0
0
0
0
0
.globl
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
237
0
0
0
0
0
236
377
0
0
0
0
0
0
0
0
0
432
0
0
0
0
0
431
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
.globl
0
446
0
0
0
447
0
463
.globl
_yypact:177730
177730
176030
176030
176030
176030
176030
176030
176030
176030
176030
176030
176030
176030
176030
176030
176030
176030
176030
176030
176030
176030
177571
177641
177603
177603
177673
177605
177565
177533
177556
177626
177510
177660
177603
177766
177716
177670
177653
177603
177504
176030
176030
176030
176030
176030
176030
177563
177764
177671
176030
176030
176030
176030
176030
177614
176030
176030
176030
177521
176030
176030
176030
177614
.globl
176030
176030
176030
176030
177507
177506
177614
176030
176030
176030
177521
177625
177622
177520
176030
176030
176030
176030
176030
177513
176030
176030
176030
176030
177517
176030
177603
177614
176030
176030
176030
176030
177652
177515
176030
176030
176030
176030
176030
176030
177620
177614
176030
176030
176030
177504
177614
176030
176030
176030
177635
177631
177611
176030
176030
176030
177723
176030
176030
176030
177617
176030
176030
177603
.globl
177504
177603
177603
177631
176030
176030
177560
176030
177556
177616
177473
177722
177615
177505
177766
177614
176030
176030
176030
177721
177504
177631
176030
177537
176030
177343
176030
177472
176030
177724
176030
177444
176030
177720
176030
177631
177431
177516
177724
177471
177724
176030
176030
176030
176030
176030
177343
176030
177667
176030
177647
177604
176030
176030
177613
177631
176030
176030
177557
176030
176030
176030
177331
177343
.globl
177343
176030
177413
176030
176030
177374
177500
177462
177454
176030
176030
176030
176030
176030
176030
176030
176030
176030
176030
177443
176030
177611
177374
176030
176030
177612
177610
176030
177734
177603
177553
177603
176030
177603
177331
177632
177602
176030
177727
177576
177441
176030
176030
176030
176030
177557
177343
177414
177375
176030
177374
177374
177374
177374
176030
176030
176030
176030
177374
177374
177374
177374
177607
176030
.globl
177422
176030
176030
176030
177564
176030
176030
177724
176030
176030
176030
177724
176030
176030
177600
176030
177514
176030
177412
177735
177573
176030
176030
176030
177604
176030
176030
176030
176030
177422
176030
177335
177340
177457
177447
177440
177426
176030
177553
177603
177512
176030
177674
177602
177601
177631
177666
177666
177550
176030
176030
176030
176030
177374
177374
177424
176030
176030
177600
177531
176030
176030
176030
176030
.globl
176030
177577
176030
176030
177376
177344
177464
176030
177422
176030
176030
176030
176030
177646
177547
176030
177416
177374
177575
177642
176030
176030
176030
177546
177373
177543
176030
.globl
_yypgo:0
576
575
574
356
411
572
407
2
370
570
0
566
563
562
1
406
400
561
560
61
425
422
557
556
555
554
553
552
551
547
545
543
542
541
540
536
535
534
533
532
527
525
412
524
520
517
516
515
514
513
510
505
504
503
502
501
500
477
414
476
475
46
474
.globl
27
473
472
471
470
465
371
464
463
462
461
457
172
456
455
454
415
453
452
450
446
445
444
413
443
442
441
440
437
436
435
434
70
433
432
13
431
430
426
416
377
.globl
_yyr1:0
1
1
1
1
1
25
25
25
25
25
25
25
25
25
25
25
25
25
25
25
25
25
25
43
50
27
51
52
52
52
52
52
32
54
55
55
55
55
44
56
57
57
57
57
45
60
61
61
61
62
62
62
47
63
64
41
65
66
66
73
73
73
73
.globl
73
67
74
74
74
2
2
3
3
4
70
70
71
71
71
72
72
72
72
77
100
75
76
37
101
102
102
102
102
102
103
103
34
104
105
105
53
5
6
6
7
7
7
106
106
10
10
107
11
11
11
11
12
15
15
15
13
13
13
13
13
13
13
13
.globl
13
14
14
16
16
17
20
21
22
22
22
22
22
22
22
22
22
23
30
110
111
111
113
113
114
114
24
115
115
112
112
31
116
33
33
117
121
121
122
122
122
35
35
35
123
125
125
125
124
124
124
127
127
127
126
126
36
130
131
40
132
133
134
134
.globl
136
137
137
140
140
135
135
141
142
142
143
143
120
120
42
144
26
46
145
146
147
147
150
.globl
_yyr2:0
5
3
1
4
2
2
2
2
2
2
2
2
2
2
2
2
2
2
2
2
2
2
3
13
3
13
3
2
2
2
2
0
11
3
2
2
2
0
13
3
2
2
2
0
11
3
5
1
3
2
2
0
7
5
5
21
5
2
6
3
3
3
3
.globl
3
5
2
2
2
7
1
2
7
3
5
5
5
5
1
4
4
4
4
1
1
21
11
13
5
2
2
2
2
0
2
0
4
7
3
3
3
7
2
7
7
3
7
2
2
5
1
3
7
5
7
2
7
2
2
2
2
2
7
7
7
7
5
11
.globl
15
17
13
2
7
3
7
3
3
3
3
3
3
3
3
3
2
3
17
3
2
0
6
12
2
3
3
6
12
3
3
12
3
4
4
3
7
7
3
7
2
4
3
4
3
5
5
5
2
4
6
3
3
3
3
7
11
13
3
14
3
3
4
0
.globl
3
2
6
3
7
4
0
3
2
6
7
7
3
7
4
3
5
10
3
6
2
2
3
.globl
_yychk:176030
177777
177753
177752
177751
177750
177747
177746
177745
177744
177743
177742
177741
177740
177737
177736
177735
177734
177733
177732
177731
400
177634
177727
177670
177662
177724
177661
177657
177674
177655
177653
177650
177677
177646
177713
177730
177722
177720
177633
177715
411
401
402
403
404
405
416
406
177647
410
412
413
414
415
177714
407
177753
177752
177660
441
177754
440
177726
.globl
426
434
435
433
177754
177754
177723
425
433
424
177660
421
417
177656
442
422
177673
442
443
177654
177651
440
422
441
177652
440
451
177676
433
434
425
432
177754
177712
177705
422
414
404
401
413
432
177721
426
425
433
177717
177716
427
426
435
177754
177773
451
420
417
421
440
420
417
421
433
177725
440
450
.globl
177725
451
451
177725
440
440
450
177651
450
450
177660
177725
435
177711
450
177704
433
432
435
440
177725
177773
177725
436
177770
177671
437
177772
177771
440
177760
177757
431
440
177754
177773
177667
177665
177754
177663
177754
177770
442
177651
440
453
177675
431
177645
440
177776
451
177705
177725
431
177773
177770
177632
177631
177754
177630
442
177767
451
.globl
465
177766
177765
177764
177756
177755
466
467
470
442
443
444
445
446
447
441
440
177760
457
177757
453
450
177672
431
423
452
431
177770
453
450
177672
450
453
177672
177767
177644
177642
433
177710
435
177775
177774
440
440
177770
177630
464
177767
177765
177767
177763
461
460
457
463
431
462
177765
451
451
451
451
452
177771
.globl
177765
422
440
440
177666
426
424
177754
177664
177754
441
177754
177754
177643
177637
437
177641
177640
440
177707
430
440
422
453
450
177630
177767
453
453
177765
177765
177765
177765
177765
177765
177765
177765
441
177672
177672
177636
177635
440
450
454
177706
177703
177701
424
440
422
177774
453
450
423
177770
177664
177754
450
431
177640
440
177770
177702
.globl
177700
433
177702
177700
442
177765
177762
177761
177765
453
177635
442
443
440
454
453
177770
450
435
442
453
177761
440
435
442
454
442
.globl
_yydef:177776
177776
2
5
6
7
10
11
12
13
14
15
16
17
20
21
22
23
24
25
26
27
0
40
0
0
46
0
0
0
252
0
0
135
0
0
0
54
177776
0
0
317
33
223
240
42
243
0
254
0
274
31
50
56
322
0
272
1
4
316
320
314
232
0
.globl
34
35
36
37
0
0
0
43
44
45
241
0
0
242
246
250
140
142
143
251
260
263
264
265
253
266
0
0
131
132
133
134
0
0
72
74
75
76
77
100
0
0
51
52
53
0
0
61
62
63
0
156
0
67
71
130
0
255
256
257
0
66
144
0
.globl
0
225
0
156
244
245
0
261
0
0
0
137
0
106
0
0
102
103
104
0
0
156
57
0
65
0
157
0
146
207
151
0
141
0
315
156
0
224
0
0
0
41
247
262
267
270
0
136
277
275
0
0
73
101
0
156
55
321
0
324
325
326
155
0
.globl
0
163
0
170
171
0
0
0
0
210
211
212
213
214
215
216
177776
220
221
0
145
0
0
153
154
0
0
32
0
0
0
0
237
0
127
306
0
300
116
0
0
107
111
30
47
0
0
0
0
161
0
0
0
0
165
166
167
176
0
0
0
0
0
147
.globl
150
152
206
271
0
235
236
0
226
230
231
0
233
273
0
307
276
301
303
123
0
112
113
105
0
323
162
160
175
164
172
173
174
0
0
0
156
222
0
0
305
310
0
0
0
156
124
124
0
114
115
110
177
0
0
0
227
234
0
0
302
304
70
117
.globl
121
0
120
122
0
0
156
203
205
202
311
312
313
0
0
200
0
0
0
0
201
204
126
0
0
0
125
.comm	_yydebug,2
.comm	_yyv,454
.comm	_yys,454
.comm	_yypv,2
.comm	_yyps,2
.comm	_yystate,2
.comm	_yytmp,2
.comm	_yynerrs,2
.comm	_yyerrfl,2
.comm	_yychar,2
.globl	_yyparse
.text
.data
L10004:L142
L147
L147
L151
L10010:L158
L160
L122
L122
L122
L122
L122
L122
L122
L122
L122
L122
L122
L122
L122
L122
L122
L122
L122
L122
L122
L122
L122
L164
L169
L170
L171
L122
L122
L122
L122
L122
L172
L173
L122
L122
L122
L122
L170
L175
L122
L122
L122
L122
L170
L177
L178
L180
L181
L122
L122
L122
L170
L183
L185
L187
L188
L122
L122
L190
L191
L192
L193
L194
L195
L122
L122
L122
L196
L197
L122
L198
L200
L202
L203
L202
L206
L208
L122
L122
L122
L122
L210
L211
L214
L215
L216
L217
L122
L122
L122
L122
L122
L122
L122
L122
L219
L221
L223
L225
L254
L122
L257
L259
L261
L263
L122
L122
L267
L268
L269
L196
L271
L272
L122
L273
L122
L122
L122
L122
L122
L273
L273
L273
L196
L271
L279
L280
L281
L282
L122
L20102
L284
L285
L286
L289
L290
L291
L292
L293
L294
L295
L297
L122
L298
L202
L303
L122
L122
L122
L122
L122
L202
L305
L122
L122
L308
L311
L122
L314
L122
L122
L316
L317
L320
L20129
L20129
L122
L122
L325
L122
L327
L328
L331
L334
L122
L122
L122
L337
L339
L341
L343
L343
L345
L347
L348
L122
L349
L202
L122
L122
L351
L122
L122
L202
L354
L122
L122
L356
L122
L122
L358
L359
L360
L360
L122
L362
L363
L122
L365
L122
L122
L122
L20129
.text
_yyparse:~~yyparse:jsr	r5,csv
sub	$6,sp
~yypvt=r4
mov	$-2+_yyv,_yypv
mov	$-2+_yys,_yyps
clr	_yystate
clr	_yytmp
clr	_yynerrs
clr	_yyerrfl
mov	$-1,_yychar
L122:mov	_yypv,r3
mov	_yyps,r2
mov	_yystate,-12(r5)
L125:add	$2,r2
cmp	$454+_yys,r2
jhi	L126
mov	$L127,(sp)
jsr	pc,*$_yyerror
L20022:mov	$1,r0
L121:jmp	cret
~yy_pv=r3
~yy_ps=r2
~yy_state=177766
~yy_n=177764
mov	_yypv,r3
mov	_yyps,r2
mov	_yystate,-12(r5)
jbr	L124
L126:mov	-12(r5),(r2)
add	$2,r3
mov	_yyval,(r3)
L124:mov	-12(r5),r0
asl	r0
mov	_yypact(r0),r0
mov	r0,-14(r5)
cmp	$-1750,r0
jge	L128
tst	_yychar
jge	L129
jsr	pc,_yylex
mov	r0,_yychar
jge	L129
clr	_yychar
L129:add	_yychar,-14(r5)
jlt	L128
cmp	$1010,-14(r5)
jle	L128
mov	-14(r5),r0
asl	r0
mov	_yyact(r0),r0
mov	r0,-14(r5)
asl	r0
cmp	_yychar,_yychk(r0)
jne	L128
mov	$-1,_yychar
mov	_yylval,_yyval
mov	-14(r5),-12(r5)
tst	_yyerrfl
jle	L125
dec	_yyerrfl
jbr	L125
L128:mov	-12(r5),r0
asl	r0
mov	_yydef(r0),r0
mov	r0,-14(r5)
cmp	$-2,r0
jne	L132
tst	_yychar
jge	L133
jsr	pc,_yylex
mov	r0,_yychar
jge	L133
clr	_yychar
L133:mov	$_yyexca,-16(r5)
~yyxi=177762
jbr	L134
L10002:add	$4,-16(r5)
L134:cmp	$-1,*-16(r5)
jne	L10002
mov	-16(r5),r0
cmp	-12(r5),2(r0)
jne	L10002
L136:add	$4,-16(r5)
tst	*-16(r5)
jlt	L137
cmp	_yychar,*-16(r5)
jne	L136
L137:mov	-16(r5),r0
mov	2(r0),-14(r5)
jge	L132
clr	r0
jbr	L121
L132:tst	-14(r5)
jne	L139
mov	_yyerrfl,r0
cmp	r0,$3
jhi	L139
asl	r0
jmp	*L10004(r0)
L142:mov	$L143,(sp)
jsr	pc,*$_yyerror
jbr	L147
L20021:mov	(r2),r0
asl	r0
mov	_yypact(r0),r0
add	$400,r0
mov	r0,-14(r5)
jlt	L150
cmp	$1010,r0
jle	L150
asl	r0
mov	_yyact(r0),r0
asl	r0
cmp	$400,_yychk(r0)
jne	L150
mov	-14(r5),r0
L20026:asl	r0
mov	_yyact(r0),-12(r5)
jbr	L125
L150:sub	$2,r2
sub	$2,r3
L148:cmp	$_yys,r2
jlos	L20021
jbr	L20022
L151:tst	_yychar
jeq	L20022
mov	$-1,_yychar
jbr	L124
L139:mov	-14(r5),_yytmp
mov	r3,r4
mov	-14(r5),r0
asl	r0
mov	_yyr2(r0),-16(r5)
~yy_len=177762
bit	$1,-16(r5)
jne	L153
asr	-16(r5)
mov	-16(r5),r0
asl	r0
sub	r0,r3
mov	2(r3),_yyval
mov	-16(r5),r0
asl	r0
sub	r0,r2
mov	-14(r5),r0
asl	r0
mov	_yyr1(r0),r0
mov	r0,-14(r5)
asl	r0
mov	_yypgo(r0),r0
add	(r2),r0
inc	r0
mov	r0,-12(r5)
cmp	$1010,r0
jle	L10005
asl	r0
mov	_yyact(r0),r0
mov	r0,-12(r5)
asl	r0
mov	-14(r5),r1
neg	r1
cmp	r1,_yychk(r0)
jeq	L125
L10005:mov	-14(r5),r0
asl	r0
mov	_yypgo(r0),r0
jbr	L20026
L153:asr	-16(r5)
mov	-16(r5),r0
asl	r0
sub	r0,r3
mov	2(r3),_yyval
mov	-16(r5),r0
asl	r0
sub	r0,r2
mov	-14(r5),r0
asl	r0
mov	_yyr1(r0),r0
mov	r0,-14(r5)
asl	r0
mov	_yypgo(r0),r0
add	(r2),r0
inc	r0
mov	r0,-12(r5)
cmp	$1010,r0
jle	L10006
asl	r0
mov	_yyact(r0),r0
mov	r0,-12(r5)
asl	r0
mov	-14(r5),r1
neg	r1
cmp	r1,_yychk(r0)
jeq	L155
L10006:mov	-14(r5),r0
asl	r0
mov	_yypgo(r0),r0
asl	r0
mov	_yyact(r0),-12(r5)
L155:mov	-12(r5),_yystate
mov	r2,_yyps
mov	r3,_yypv
mov	_yytmp,r0
sub	$1,r0
cmp	r0,$325
jhi	L122
asl	r0
jmp	*L10010(r0)
L158:mov	_Opflag,(sp)
jsr	pc,*$_endquel
tst	r0
jge	L122
L20027:mov	$-1,r0
jbr	L121
L160:mov	_Opflag,(sp)
jsr	pc,*$_endquel
tst	r0
jlt	L20027
jbr	L122
L164:mov	(r4),(sp)
mov	$-1,-(sp)
mov	$_Reldesc,-(sp)
jsr	pc,*$_openr
cmp	(sp)+,(sp)+
mov	r0,_i
jge	L165
mov	r0,(sp)
mov	$L166,-(sp)
jsr	pc,*$_syserr
tst	(sp)+
L165:tst	_i
jle	L167
clr	(sp)
mov	(r4),-(sp)
clr	-(sp)
mov	$4105,-(sp)
L20024:jsr	pc,*$_par_err
add	$6,sp
mov	_yypv,r3
mov	_yyps,r2
mov	_yystate,-12(r5)
inc	_yynerrs
L147:mov	$3,_yyerrfl
jbr	L148
L167:mov	$_Reldesc,(sp)
mov	-4(r4),-(sp)
mov	$4,-(sp)
jsr	pc,*$_rngent
L20028:cmp	(sp)+,(sp)+
jbr	L122
L169:mov	$14,_Opflag
jbr	L122
L170:mov	$1,(sp)
mov	$10,-(sp)
mov	$122,-(sp)
mov	(r4),-(sp)
mov	-2(r4),-(sp)
L20031:jsr	pc,*$_tree
add	$10,sp
mov	r0,_Lastree
jbr	L122
L171:mov	$2,_Opflag
jbr	L122
L172:mov	$1,(sp)
mov	$10,-(sp)
mov	$122,-(sp)
mov	(r4),-(sp)
clr	-(sp)
mov	$4,-(sp)
mov	$162,-(sp)
mov	_Tidnode,-(sp)
clr	-(sp)
jsr	pc,_tree
add	$12,sp
mov	r0,-(sp)
jbr	L20031
L173:mov	$4,_Opflag
jbr	L122
L175:mov	$3,_Opflag
jbr	L122
L177:mov	$1,_Opflag
jbr	L122
L178:clr	_Rsdmno
mov	$L179,(sp)
L20063:mov	$2,-(sp)
jsr	pc,*$_setp
tst	(sp)+
mov	_Resrng,r1
mul	$570,r1
add	$_Parrng,r1
mov	r1,(sp)
jsr	pc,*$_trim_re
L20199:mov	r0,(sp)
L20066:mov	$2,-(sp)
L20078:jsr	pc,*$_setp
L20085:tst	(sp)+
jbr	L122
L181:mov	$17,_Opflag
L180:clr	_Rsdmno
mov	$-1,_Resrng
jbr	L122
L183:clr	_Rsdmno
mov	$L184,(sp)
jbr	L20063
L185:mov	$20,_Opflag
tst	_Qrymod
jne	L122
L20090:clr	(sp)
clr	-(sp)
mov	$4133,-(sp)
L20094:jsr	pc,*$_par_err
jbr	L20028
L187:mov	$1,(sp)
mov	$10,-(sp)
mov	$122,-(sp)
mov	(r4),-(sp)
mov	-10(r4),-(sp)
jbr	L20031
L188:mov	$26,_Opflag
tst	_Qrymod
jeq	L20090
jbr	L122
L190:mov	$-1,(sp)
L20095:jsr	pc,*$_permcom
jbr	L122
L191:mov	$1,(sp)
jbr	L20095
L192:mov	$4,(sp)
jbr	L20095
L193:mov	$2,(sp)
jbr	L20095
L194:mov	$3,(sp)
jbr	L20095
L195:mov	_Permcom,(sp)
mov	$1,-(sp)
jsr	pc,*$_setp
tst	(sp)+
clr	_Permcom
mov	_Resrng,r1
mul	$570,r1
add	$_Parrng,r1
mov	r1,(sp)
jsr	pc,*$_trim_re
mov	r0,(sp)
mov	$2,-(sp)
jsr	pc,*$_setp
tst	(sp)+
mov	$2,(sp)
mov	$_permbuf,-(sp)
mov	_Resrng,r1
mul	$570,r1
add	$14+_Parrng,r1
mov	r1,-(sp)
jsr	pc,*$_bmove
cmp	(sp)+,(sp)+
clrb	2+_permbuf
mov	$_permbuf,(sp)
jbr	L20066
L196:mov	-2(r4),_yyval
jbr	L122
L197:clr	_yyval
jbr	L122
L198:tst	_Err_cur
jne	L122
L20102:mov	(r4),(sp)
mov	-4(r4),-(sp)
L20099:jsr	pc,*$_tlprepe
L20097:tst	(sp)+
L20105:mov	r0,_yyval
jbr	L122
L200:tst	_Err_cur
jne	L122
mov	(r4),_Trname
mov	_Trname,(sp)
mov	_Resrng,-(sp)
jsr	pc,*$_attlook
tst	(sp)+
mov	r0,_aptr
mov	r0,(sp)
mov	_Resrng,-(sp)
mov	$6,-(sp)
mov	$166,-(sp)
clr	-(sp)
clr	-(sp)
jsr	pc,*$_tree
add	$12,sp
mov	r0,_yyval
mov	r0,(sp)
L20108:clr	-(sp)
jsr	pc,*$_addresd
jbr	L20097
L203:mov	$L204,(sp)
jbr	L20066
L206:mov	$L207,(sp)
jbr	L20066
L208:mov	$L209,(sp)
jbr	L20066
L210:clr	(sp)
mov	$1,-(sp)
jsr	pc,*$_setp
tst	(sp)+
mov	$2640,(sp)
L20110:mov	$1,-(sp)
jbr	L20078
L211:mov	$L212,(sp)
mov	$2,-(sp)
jsr	pc,*$_setp
tst	(sp)+
mov	$L213,(sp)
jbr	L20066
L214:mov	-10(r4),(sp)
mov	-14(r4),-(sp)
jsr	pc,*$_timeofd
tst	(sp)+
mov	r0,(sp)
mov	$1,-(sp)
jsr	pc,*$_setp
tst	(sp)+
mov	(r4),(sp)
mov	-4(r4),-(sp)
jsr	pc,*$_timeofd
tst	(sp)+
mov	r0,(sp)
jbr	L20110
L215:mov	-4(r4),(sp)
L20113:mov	$2,-(sp)
jsr	pc,*$_setp
tst	(sp)+
L202:mov	(r4),(sp)
jbr	L20066
L216:mov	$1,(sp)
mov	$10,-(sp)
mov	$122,-(sp)
mov	(r4),-(sp)
jsr	pc,_norml
tst	(sp)+
mov	r0,-(sp)
clr	-(sp)
jsr	pc,*$_tree
add	$10,sp
mov	r0,_Lastree
L20124:dec	_Qlflag
jbr	L122
L217:mov	$27,_Opflag
inc	_Qlflag
tst	_Qrymod
jeq	L20090
jbr	L122
L219:mov	$6,_Opflag
mov	$L220,(sp)
mov	$2,-(sp)
jsr	pc,*$_setp
tst	(sp)+
mov	-2(r4),(sp)
jbr	L20066
L221:tst	*(r4)
jge	L222
clr	(sp)
clr	-(sp)
mov	$4142,-(sp)
jsr	pc,*$_par_err
cmp	(sp)+,(sp)+
L222:mov	$151,(sp)
mov	$1,-(sp)
L20135:jsr	pc,*$_setp
tst	(sp)+
L20129:mov	*(r4),(sp)
jsr	pc,*$_iocv
L20126:mov	r0,_i
jbr	L20199
L223:mov	(r4),r0
tst	(r0)
jge	L224
clr	(sp)
clr	-(sp)
mov	$4142,-(sp)
jsr	pc,*$_par_err
cmp	(sp)+,(sp)+
L224:mov	$151,(sp)
mov	$1,-(sp)
L20147:jsr	pc,*$_setp
tst	(sp)+
mov	(r4),r0
mov	2(r0),r1
mov	(r0),r0
mov	r1,-(sp)
mov	r0,-(sp)
jsr	pc,_locv
cmp	(sp)+,(sp)+
jbr	L20126
L225:mov	_Opflag,r0
cmp	r0,$1
jeq	L229
cmp	r0,$2
jeq	L238
cmp	r0,$3
jeq	L251
cmp	r0,$4
jeq	L251
cmp	r0,$20
jeq	L229
cmp	r0,$26
jeq	L247
cmp	r0,$27
jeq	L247
jbr	L122
L229:mov	(r4),(sp)
mov	$-1,-(sp)
mov	$_Reldesc,-(sp)
jsr	pc,*$_openr
cmp	(sp)+,(sp)+
mov	r0,_i
jge	L230
mov	r0,(sp)
mov	$L231,-(sp)
jsr	pc,*$_syserr
tst	(sp)+
L230:tst	_i
jne	L232
mov	$2,(sp)
mov	_Usercod,-(sp)
mov	$14+_Reldesc,-(sp)
jsr	pc,*$_bequal
cmp	(sp)+,(sp)+
tst	r0
jeq	L233
clr	(sp)
mov	(r4),-(sp)
clr	-(sp)
mov	$4127,-(sp)
jbr	L20024
L233:tst	_Err_cur
jne	L232
mov	(r4),(sp)
jsr	pc,*$_rngdel
L232:tst	_Err_cur
jne	L122
mov	$2,(sp)
mov	$14+_Reldesc,-(sp)
mov	_Usercod,-(sp)
jsr	pc,*$_bmove
cmp	(sp)+,(sp)+
mov	$40,(sp)
mov	$14,-(sp)
mov	$_Reldesc,-(sp)
mov	(r4),-(sp)
jsr	pc,*$_pmove
add	$6,sp
mov	$_Reldesc,(sp)
mov	$L237,-(sp)
mov	$3,-(sp)
jsr	pc,*$_rngent
cmp	(sp)+,(sp)+
mov	r0,_Resrng
jbr	L122
L238:tst	_Err_cur
jne	L122
mov	$1,(sp)
mov	(r4),-(sp)
jsr	pc,*$_rnglook
tst	(sp)+
mov	r0,_Resrng
jge	L240
mov	(r4),(sp)
mov	$-1,-(sp)
mov	$_Reldesc,-(sp)
jsr	pc,*$_openr
cmp	(sp)+,(sp)+
mov	r0,_i
jge	L241
mov	r0,(sp)
mov	$L242,-(sp)
jsr	pc,*$_syserr
tst	(sp)+
L241:tst	_i
jeq	L243
clr	(sp)
mov	(r4),-(sp)
clr	-(sp)
mov	$4074,-(sp)
jbr	L20024
L243:mov	$_Reldesc,(sp)
mov	$L244,-(sp)
mov	$3,-(sp)
jsr	pc,*$_rngent
cmp	(sp)+,(sp)+
mov	r0,_Resrng
jbr	L245
L240:mov	_Resrng,(sp)
jsr	pc,*$_ctlmod_
L245:mov	_Resrng,(sp)
jsr	pc,*$_checkup
jbr	L122
L247:mov	$2,(sp)
mov	(r4),-(sp)
jsr	pc,*$_rnglook
tst	(sp)+
mov	r0,_Resrng
jge	L248
L20151:clr	(sp)
mov	(r4),-(sp)
clr	-(sp)
mov	$4075,-(sp)
jbr	L20024
L248:mov	_Resrng,(sp)
L20155:jsr	pc,*$_ctlmod_
jbr	L122
L251:mov	$2,(sp)
mov	(r4),-(sp)
jsr	pc,*$_rnglook
tst	(sp)+
mov	r0,_Resrng
jlt	L20151
mov	r0,(sp)
jsr	pc,*$_ctlmod_
mov	_Resrng,(sp)
jsr	pc,*$_checkup
mov	$_Faketid,(sp)
mov	_Resrng,-(sp)
mov	$6,-(sp)
mov	$166,-(sp)
clr	-(sp)
clr	-(sp)
jsr	pc,*$_tree
add	$12,sp
mov	r0,_Tidnode
jbr	L122
L254:tst	_Patflag
jeq	L255
clr	(sp)
clr	-(sp)
mov	$4110,-(sp)
jsr	pc,*$_par_err
cmp	(sp)+,(sp)+
L255:mov	-2(r4),_yyval
cmp	$3,_Opflag
jne	L122
tst	_Err_cur
jne	L122
mov	_yyval,(sp)
clr	-(sp)
mov	$4,-(sp)
mov	$162,-(sp)
mov	_Tidnode,-(sp)
clr	-(sp)
jsr	pc,_tree
add	$12,sp
mov	r0,-(sp)
jbr	L20099
L257:tst	_Err_cur
jeq	L20102
jbr	L122
L259:mov	-4(r4),_Trname
tst	_Err_cur
jne	L122
L20156:mov	(r4),(sp)
jbr	L20108
L261:tst	_Err_cur
jeq	L20156
jbr	L122
L263:cmp	$3,_Opflag
jne	L264
clr	(sp)
mov	-4(r4),r0
ash	$2,r0
mov	10+_Qt(r0),r0
add	$54,r0
mov	r0,-(sp)
jsr	pc,_trim_re
tst	(sp)+
mov	r0,-(sp)
clr	-(sp)
mov	$4113,-(sp)
jbr	L20024
L264:tst	_Err_cur
jne	L122
mov	-4(r4),(sp)
jsr	pc,*$_xdot
jbr	L20105
L267:mov	(r4),(sp)
jsr	pc,*$_norml
mov	r0,_yyval
jbr	L20124
L268:clr	(sp)
jsr	pc,*$_norml
jbr	L20105
L269:inc	_Qlflag
jbr	L122
L271:mov	-2(r4),(sp)
mov	$2,-(sp)
mov	$125,-(sp)
L20173:mov	(r4),-(sp)
L20175:clr	-(sp)
L20158:jsr	pc,*$_tree
add	$10,sp
jbr	L20105
L272:clr	(sp)
mov	$6,-(sp)
mov	-2(r4),-(sp)
L20184:mov	(r4),-(sp)
mov	-4(r4),-(sp)
jbr	L20158
L273:mov	-2(r4),(sp)
mov	$2,-(sp)
mov	$102,-(sp)
jbr	L20184
L279:mov	-6(r4),(sp)
mov	$2,-(sp)
mov	$125,-(sp)
clr	-(sp)
mov	-2(r4),-(sp)
jbr	L20158
L280:mov	-12(r4),(sp)
mov	$2,-(sp)
mov	$102,-(sp)
mov	-2(r4),-(sp)
mov	-6(r4),-(sp)
jbr	L20158
L281:mov	-4(r4),(sp)
jsr	pc,*$_windup
clr	(sp)
mov	$10,-(sp)
mov	$150,-(sp)
mov	-2(r4),-(sp)
clr	-(sp)
mov	$4,-(sp)
mov	$110,-(sp)
mov	-14(r4),-(sp)
mov	$6,-(sp)
mov	$101,-(sp)
mov	-10(r4),-(sp)
clr	-(sp)
jsr	pc,_tree
add	$12,sp
mov	r0,-(sp)
mov	-4(r4),-(sp)
jsr	pc,_tree
add	$12,sp
mov	r0,-(sp)
jsr	pc,*$_tree
add	$10,sp
mov	r0,_yyval
mov	r0,(sp)
clr	-(sp)
mov	$124,-(sp)
clr	-(sp)
clr	-(sp)
jsr	pc,_tree
add	$10,sp
mov	r0,-(sp)
jsr	pc,*$_tlprepe
jbr	L20085
L282:clr	(sp)
mov	$10,-(sp)
mov	$150,-(sp)
mov	-2(r4),-(sp)
mov	-10(r4),-(sp)
mov	$6,-(sp)
mov	$101,-(sp)
mov	-4(r4),-(sp)
clr	-(sp)
jsr	pc,_tree
add	$12,sp
mov	r0,-(sp)
jbr	L20158
L284:mov	_Rsdmno,(sp)
mov	$4,-(sp)
mov	$162,-(sp)
jbr	L20173
L285:mov	(r4),_Trname
mov	_Trname,(sp)
mov	-4(r4),-(sp)
jsr	pc,*$_attlook
tst	(sp)+
mov	r0,_aptr
mov	r0,(sp)
mov	-4(r4),-(sp)
mov	$6,-(sp)
mov	$166,-(sp)
clr	-(sp)
clr	-(sp)
jsr	pc,*$_tree
add	$12,sp
jbr	L20105
L286:mov	$2,(sp)
mov	(r4),-(sp)
jsr	pc,*$_rnglook
tst	(sp)+
mov	r0,_yyval
jlt	L20151
mov	r0,(sp)
jbr	L20155
L289:mov	(r4),(sp)
mov	$2,-(sp)
L20195:mov	$151,-(sp)
L20196:clr	-(sp)
jbr	L20175
L290:mov	(r4),(sp)
mov	$4,-(sp)
jbr	L20195
L291:mov	(r4),(sp)
mov	$4,-(sp)
L20197:mov	$146,-(sp)
jbr	L20196
L292:mov	(r4),(sp)
mov	$10,-(sp)
jbr	L20197
L293:mov	(r4),(sp)
mov	$4,-(sp)
mov	$144,-(sp)
jbr	L20196
L294:mov	(r4),(sp)
mov	$4,-(sp)
mov	$155,-(sp)
jbr	L20196
L295:mov	(r4),(sp)
jsr	pc,*$_patmat
tst	r0
jeq	L296
tst	_Qlflag
jne	L296
mov	$1,_Patflag
L296:mov	(r4),(sp)
mov	(r4),-(sp)
jsr	pc,_length
tst	(sp)+
mov	r0,-(sp)
mov	$143,-(sp)
jbr	L20196
L297:mov	(r4),(sp)
mov	$2,-(sp)
mov	$103,-(sp)
jbr	L20196
L298:tst	(r4)
jeq	L197
cmp	$1,(r4)
jne	L122
mov	$1,_yyval
jbr	L122
L303:mov	$5,_Opflag
jbr	L122
L305:tst	_Err_cur
jne	L122
mov	(r4),(sp)
mov	$2,-(sp)
jsr	pc,*$_setp
tst	(sp)+
cmp	$7,_Opflag
jeq	L10008
cmp	$6,_Opflag
jne	L122
L10008:mov	(r4),(sp)
jsr	pc,*$_rngdel
jbr	L122
L308:mov	$L309,(sp)
mov	$2,-(sp)
jsr	pc,*$_setp
tst	(sp)+
mov	$L310,(sp)
jbr	L20066
L311:mov	$L312,(sp)
mov	$2,-(sp)
jsr	pc,*$_setp
tst	(sp)+
mov	$L313,(sp)
jbr	L20066
L314:mov	$6,_Opflag
mov	$L315,(sp)
jbr	L20066
L316:mov	$7,_Opflag
jbr	L122
L317:mov	$32,_Opflag
tst	_Qrymod
jne	L318
clr	(sp)
clr	-(sp)
mov	$4133,-(sp)
jsr	pc,*$_par_err
cmp	(sp)+,(sp)+
L318:mov	$L319,(sp)
jbr	L20113
L320:mov	$32,_Opflag
tst	_Qrymod
jne	L321
clr	(sp)
clr	-(sp)
mov	$4133,-(sp)
jsr	pc,*$_par_err
cmp	(sp)+,(sp)+
L321:mov	$L322,(sp)
jbr	L20113
L325:mov	$L326,(sp)
jbr	L20066
L327:mov	$10,_Opflag
jbr	L122
L328:mov	$23,_Opflag
tst	_Qrymod
jne	L329
clr	(sp)
clr	-(sp)
mov	$4133,-(sp)
jsr	pc,*$_par_err
cmp	(sp)+,(sp)+
L329:mov	$_hqmbuf,(sp)
mov	$L330,-(sp)
L20198:jsr	pc,*$_smove
jbr	L20085
L331:mov	$23,_Opflag
tst	_Qrymod
jne	L332
clr	(sp)
clr	-(sp)
mov	$4133,-(sp)
jsr	pc,*$_par_err
cmp	(sp)+,(sp)+
L332:mov	$_hqmbuf,(sp)
mov	$L333,-(sp)
jbr	L20198
L334:mov	$23,_Opflag
tst	_Qrymod
jne	L335
clr	(sp)
clr	-(sp)
mov	$4133,-(sp)
jsr	pc,*$_par_err
cmp	(sp)+,(sp)+
L335:mov	$_hqmbuf,(sp)
mov	$L336,-(sp)
jbr	L20198
L337:mov	$L338,(sp)
jbr	L20113
L339:mov	$L340,(sp)
jbr	L20066
L341:mov	$L342,(sp)
jbr	L20113
L343:mov	$_hqmbuf,(sp)
jbr	L20113
L345:cmp	$6,_Rsdmno
jge	L122
clr	(sp)
clr	-(sp)
mov	$4077,-(sp)
jbr	L20094
L347:clr	_Rsdmno
mov	-4(r4),(sp)
mov	$2,-(sp)
jsr	pc,*$_setp
tst	(sp)+
mov	(r4),(sp)
mov	$2,-(sp)
jsr	pc,*$_setp
tst	(sp)+
mov	(r4),_Indexna
jbr	L122
L348:mov	$11,_Opflag
jbr	L122
L349:mov	$12,_Opflag
clr	_Rsdmno
jbr	L122
L351:mov	$L352,(sp)
jbr	L20066
L354:mov	$_modbuf,(sp)
mov	(r4),-(sp)
mov	$L355,-(sp)
jsr	pc,_ztack
cmp	(sp)+,(sp)+
mov	r0,-(sp)
mov	-4(r4),-(sp)
jsr	pc,*$_concat
cmp	(sp)+,(sp)+
mov	$_modbuf,(sp)
jbr	L20066
L356:mov	$L357,(sp)
jbr	L20066
L358:mov	-4(r4),(sp)
mov	$2,-(sp)
jbr	L20135
L359:mov	-4(r4),(sp)
mov	$2,-(sp)
jbr	L20147
L360:inc	_Rsdmno
jbr	L122
L362:mov	$13,_Opflag
jbr	L122
L363:mov	(r4),(sp)
mov	$L364,-(sp)
jsr	pc,*$_printf
jbr	L20085
L365:mov	$15,_Opflag
jbr	L122
.globl	fltused
.globl
.data
L1:.byte 141,142,163,0
L2:.byte 141,154,154,0
L3:.byte 141,156,144,0
L4:.byte 141,156,171,0
L5:.byte 141,160,160,145,156,144,0
L6:.byte 141,163,143,151,151,0
L7:.byte 141,164,0
L8:.byte 141,164,141,156,0
L9:.byte 141,166,147,0
L10:.byte 141,166,147,165,0
L11:.byte 142,171,0
L12:.byte 143,157,156,143,141,164,0
L13:.byte 143,157,160,171,0
L14:.byte 143,157,163,0
L15:.byte 143,157,165,156,164,0
L16:.byte 143,157,165,156,164,165,0
L17:.byte 143,162,145,141,164,145,0
L18:.byte 144,141,164,145,64,0
L19:.byte 144,145,146,151,156,145,0
L20:.byte 144,145,154,145,164,145,0
L21:.byte 144,145,163,164,162,157,171,0
L22:.byte 145,170,160,0
L23:.byte 146,154,157,141,164,64,0
L24:.byte 146,154,157,141,164,70,0
L25:.byte 146,162,157,155,0
L26:.byte 146,162,157,155,137,144,141,164,145,0
L27:.byte 146,162,157,155,137,164,151,155,145,0
L28:.byte 150,145,154,160,0
L29:.byte 151,156,0
L30:.byte 151,156,144,145,170,0
L31:.byte 151,156,164,61,0
L32:.byte 151,156,164,62,0
L33:.byte 151,156,164,64,0
L34:.byte 151,156,164,145,147,162,151,164,171,0
L35:.byte 151,156,164,157,0
L36:.byte 151,163,0
L37:.byte 154,157,147,0
L38:.byte 155,141,170,0
L39:.byte 155,151,156,0
L40:.byte 155,157,144,0
L41:.byte 155,157,144,151,146,171,0
L42:.byte 156,157,164,0
L43:.byte 157,146,0
L44:.byte 157,156,0
L45:.byte 157,156,164,157,0
L46:.byte 157,162,0
L47:.byte 160,145,162,155,151,164,0
L48:.byte 160,162,151,156,164,0
L49:.byte 162,141,156,147,145,0
L50:.byte 162,145,160,154,141,143,145,0
L51:.byte 162,145,164,162,151,145,166,145,0
L52:.byte 163,141,166,145,0
L53:.byte 163,145,162,137,154,141,163,164,0
L54:.byte 163,145,162,137,156,145,170,164,0
L55:.byte 163,145,162,137,163,145,164,0
L56:.byte 163,151,156,0
L57:.byte 163,161,162,164,0
L58:.byte 163,165,155,0
L59:.byte 163,165,155,165,0
L60:.byte 164,151,155,145,64,0
L61:.byte 164,157,0
L62:.byte 164,157,137,144,141,164,145,0
L63:.byte 164,157,137,164,151,155,145,0
L64:.byte 165,156,151,161,165,145,0
L65:.byte 165,156,164,151,154,0
L66:.byte 166,151,145,167,0
L67:.byte 167,150,145,162,145,0
L68:.byte 54,0
L69:.byte 50,0
L70:.byte 56,0
L71:.byte 51,0
L72:.byte 42,0
L73:.byte 47,0
L74:.byte 140,0
L75:.byte 57,52,0
L76:.byte 52,57,0
L77:.byte 72,0
L78:.byte 53,0
L79:.byte 55,0
L80:.byte 52,0
L81:.byte 57,0
L82:.byte 52,52,0
L83:.byte 76,0
L84:.byte 76,75,0
L85:.byte 74,0
L86:.byte 74,75,0
L87:.byte 41,75,0
L88:.byte 75,0
L89:.byte 144,142,141,0
L90:.byte 165,163,145,162,143,157,144,145,0
L91:.byte 163,171,163,144,141,164,145,0
L92:.byte 163,171,163,164,151,155,145,0
L111:.byte 105,117,106,0
L113:.byte 142,141,144,40,114,141,163,164,157,153,40,146,157,162
.byte 155,141,164,0
L127:.byte 171,141,143,143,40,163,164,141,143,153,40,157,166,145
.byte 162,146,154,157,167,0
L143:.byte 163,171,156,164,141,170,40,145,162,162,157,162,0
L166:.byte 162,145,154,156,141,155,145,72,40,145,162,162,157,162
.byte 40,151,156,40,157,160,145,156,162,40,47,45,144,47,0
L179:.byte 60,0
L184:.byte 60,60,64,60,0
L204:.byte 141,154,154,0
L207:.byte 141,154,154,0
L209:.byte 141,154,154,0
L212:.byte 163,165,156,0
L213:.byte 163,141,164,0
L220:.byte 60,61,60,60,60,60,60,0
L231:.byte 162,145,154,141,164,151,157,156,72,40,145,162,162,40
.byte 157,160,145,156,162,40,47,45,144,47,0
L237:.byte 0
L242:.byte 162,145,154,141,164,151,157,156,72,40,145,162,162,40
.byte 157,160,145,156,162,40,47,45,144,47,0
L244:.byte 0
L309:.byte 0,0
L310:.byte 151,0
L312:.byte 0,0
L313:.byte 146,0
L315:.byte 60,0
L319:.byte 66,0
L322:.byte 65,0
L326:.byte 62,0
L330:.byte 64,0
L333:.byte 65,0
L336:.byte 66,0
L338:.byte 60,0
L340:.byte 63,0
L342:.byte 61,0
L352:.byte 156,141,155,145,0
L355:.byte 72,0
L357:.byte 0,0
L364:.byte 45,163,12,0
