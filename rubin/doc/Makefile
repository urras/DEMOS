SHELL = /bin/sh
NROFF=  hroff
NMACS=  ../titles ../naa
TMACS=  ../titles.lz ../naa
LP   =  LP/
LJ   = LJ/
fnt=tr08
LAZER=lazer -f $(fnt) -m "" -T -S

all:    ref others
lj:    jref jothers

others:
	cd other;make -$(MAKEFLAGS) all

jothers:
	cd other;make -$(MAKEFLAGS) lj

ref:    $(LP)Ref0.lp $(LP)Ref1.lp $(LP)Ref2.lp $(LP)Ref3.lp $(LP)Ref4.lp

$(LP)Ref0.lp : titles naa 0aa.nr 1aa.nr news.nr toc.nr
	($(NROFF) titles naa 0aa.nr 1aa.nr news.nr toc.nr  > naa.lp;mv naa.lp $(LP)Ref0.lp   )

$(LP)Ref1.lp: quel/*.nr naa
	(cd quel; $(NROFF) $(NMACS) *.nr > quel.lp;mv quel.lp ../$(LP)Ref1.lp)

$(LP)Ref2.lp: demos/*.nr naa
	(cd demos; $(NROFF) $(NMACS) *.nr > demos.lp;mv demos.lp ../$(LP)Ref2.lp)

$(LP)Ref3.lp: files/*.nr naa
	(cd files; $(NROFF) $(NMACS) *.nr > files.lp;mv files.lp ../$(LP)Ref3.lp    )

$(LP)Ref4.lp: error/*.nr naa
	(cd error; $(NROFF) $(NMACS) *.nr > error.lp;mv error.lp ../$(LP)Ref4.lp    )

clean:
	rm -f *.lp *.bak *.b core



jref:    $(LJ)Ref0.$(fnt) $(LJ)Ref1.$(fnt) $(LJ)Ref2.$(fnt) $(LJ)Ref3.$(fnt) $(LJ)Ref4.$(fnt)

$(LJ)Ref0.$(fnt) : titles.lz naa 0aa.nr 1aa.nr news.nr toc.nr
	($(LAZER) titles.lz naa 0aa.nr 1aa.nr news.nr toc.nr  > naa.$(fnt);mv naa.$(fnt) $(LJ)Ref0.$(fnt)   )

$(LJ)Ref1.$(fnt): quel/*.nr naa
	(cd quel; $(LAZER) $(TMACS) *.nr > quel.$(fnt);mv quel.$(fnt) ../$(LJ)Ref1.$(fnt))

$(LJ)Ref2.$(fnt): demos/*.nr naa
	(cd demos; $(LAZER) $(TMACS) *.nr > demos.$(fnt);mv demos.$(fnt) ../$(LJ)Ref2.$(fnt))

$(LJ)Ref3.$(fnt): files/*.nr naa
	(cd files; $(LAZER) $(TMACS) *.nr > files.$(fnt);mv files.$(fnt) ../$(LJ)Ref3.$(fnt)    )

$(LJ)Ref4.$(fnt): error/*.nr naa
	(cd error; $(LAZER) $(TMACS) *.nr > error.$(fnt);mv error.$(fnt) ../$(LJ)Ref4.$(fnt)    )

