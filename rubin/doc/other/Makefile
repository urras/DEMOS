#
# Форматирование документации по СУБД РУБИН
#

NROFF=hroff
MACS=../titles nmacs
TMACS=../titles.lz nmacs
ME=-me
LP=../LP/
lp=lp
fntp=jr08
fntn=lg08
LJ=../LJ/
LAZER=lazer -m "" -f $(fntp) -T -S
LAZERO=lazer -m "" -f $(fntn) -T

all: user sys

user  : $(LP)Intro.$(lp) $(LP)Equel.$(lp)  $(LP)tutorial.$(lp) $(LP)maintain.$(lp)
sys:    $(LP)Notes386.$(lp) $(LP)Notes286.$(lp) $(LP)Notes32.$(lp)
# sys   : $(LP)Setup.$(lp) $(LP)lockdev.$(lp)

$(LP)Intro.$(lp): Intro.me
	$(NROFF) -me ../titles Intro.me > %Intro.$(lp)
	mv %Intro.$(lp) $(LP)Intro.$(lp)

$(LP)Equel.$(lp): equel.me
	$(NROFF) -me ../titles equel.me > %equel.$(lp)
	mv %equel.$(lp) $(LP)Equel.$(lp)

$(LP)design.$(lp): design.roff
	$(NROFF) design.roff > %design.$(lp)
	mv %design.$(lp) $(LP)design.$(lp)

$(LP)Tutorial.$(lp): nmacs tutorial.nr
	$(NROFF) $(MACS) tutorial.nr > %tutorial.$(lp)
	mv %tutorial.$(lp) $(LP)Tutorial.$(lp)

$(LP)Maintain.$(lp): maintain.nr $(MACS)
	$(NROFF) $(MACS) maintain.nr > %maintain.$(lp)
	mv %maintain.$(lp) $(LP)Maintain.$(lp)

$(LP)Notes286.$(lp): Notes286.me
	$(NROFF) -me Notes286.me> %setup.$(lp)
	mv %setup.$(lp) $(LP)Notes286.$(lp)

$(LP)Notes32.$(lp): Notes32.me
	$(NROFF) -me   Notes32.me > %setup.$(lp)
	mv %setup.$(lp) $(LP)Notes32.$(lp)

$(LP)Notes386.$(lp): Notes386.me
	$(NROFF) -me   Notes386.me > %setup.$(lp)
	mv %setup.$(lp) $(LP)Notes386.$(lp)

$(LP)Setup.$(lp): setup.nr
	$(NROFF) $(MACS) setup.nr> %setup.$(lp)
	mv %setup.$(lp) $(LP)Setup.$(lp)

$(LP)lserv.$(lp): lserv.nr $(MACS)
	$(NROFF) $(MACS) lserv.nr > %lserv.$(lp)
	mv %lserv.$(lp) $(LP)lserv.$(lp)

$(LP)lockdev.$(lp): lockdev.nr $(MACS)
	$(NROFF) $(MACS) lockdev.nr > %lockdev.$(lp)
	mv %lockdev.$(lp) $(LP)lockdev.$(lp)

lj: juser jsys

juser  : $(LJ)Intro.$(fntp) $(LJ)Equel.$(fntp)  $(LJ)Tutorial.$(fntp) $(LJ)Maintain.$(fntn)
jsys   : $(LJ)Notes386.$(fntp) $(LJ)Notes32.$(fntp)
# jsys   : $(LJ)Setup.$(fntn) $(LJ)lockdev.$(fntn)

$(LJ)Intro.$(fntp): Intro.me
	$(LAZER) -m -me ../titles.lz Intro.me > %Intro.$(fntp)
	mv %Intro.$(fntp) $(LJ)Intro.$(fntp)

$(LJ)Equel.$(fntp): equel.me
	$(LAZER) -m -me ../titles.lz equel.me > %equel.$(fntp)
	mv %equel.$(fntp) $(LJ)Equel.$(fntp)

$(LJ)design.$(fntn): design.roff
	$(LAZERO) design.roff > %design.$(fntn)
	mv %design.$(fntn) $(LJ)design.$(fntn)

$(LJ)Tutorial.$(fntp): nmacs tutorial.nr
	$(LAZER) $(TMACS) tutorial.nr > %tutorial.$(fntp)
	mv %tutorial.$(fntp) $(LJ)Tutorial.$(fntp)

$(LJ)Maintain.$(fntn): maintain.nr $(TMACS)
	$(LAZERO) $(TMACS) maintain.nr > %maintain.$(fntn)
	mv %maintain.$(fntn) $(LJ)Maintain.$(fntn)

$(LJ)Notes386.$(fntp): Notes386.me
	$(LAZER) -m -me Notes386.me> %setup.$(fntp)
	mv %setup.$(fntp) $(LJ)Notes386.$(fntp)

$(LJ)Notes32.$(fntp): Notes32.me
	$(LAZER) -m -me Notes32.me> %setup.$(fntp)
	mv %setup.$(fntp) $(LJ)Notes32.$(fntp)

$(LJ)Notes286.$(fntp): Notes286.me
	$(LAZER) -m -me Notes286.me> %setup.$(fntp)
	mv %setup.$(fntp) $(LJ)Notes286.$(fntp)

$(LJ)Setup.$(fntn): setup.nr
	$(LAZERO) $(TMACS) setup.nr> %setup.$(fntn)
	mv %setup.$(fntn) $(LJ)Setup.$(fntn)

$(LJ)lserv.$(fntn): lserv.nr $(TMACS)
	$(LAZERO) $(TMACS) lserv.nr > %lserv.$(fntn)
	mv %lserv.$(fntn) $(LJ)lserv.$(fntn)

$(LJ)lockdev.$(fntn): lockdev.nr $(TMACS)
	$(LAZERO) $(TMACS) lockdev.nr > %lockdev.$(fntn)
	mv %lockdev.$(fntn) $(LJ)lockdev.$(fntn)



