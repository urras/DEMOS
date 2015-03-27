all:		libfpsim.a libfpsim_sep.a

libfpsim.a:	fpsim.o
		mv fpsim.o libfpsim.a

fpsim.o:	fp1.s fp2.s fp3.s fpx.s
		as -o fpsim.o fp1.s fp2.s fp3.s fpx.s

libfpsim_sep.a:	fpsim_sep.o
		mv fpsim_sep.o libfpsim_sep.a

fpsim_sep.o:	fp1_sep.s fp2.s fp3.s fpx_sep.s
		as -o fpsim_sep.o fp1_sep.s fp2.s fp3.s fpx_sep.s

cp:		libfpsim.a libfpsim_sep.a
		cp libfpsim.a ${DESTDIR}/lib/libfpsim.a
		cp libfpsim_sep.a ${DESTDIR}/lib/libfpsim_sep.a

clean:
		-rm -f *.o libfpsim.a libfpsim_sep.a
