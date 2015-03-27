SRC = .test.c
DST = .test
CMP = cc
COMPILE = ${CMP} -g -o ${DST} ${SRC}

start: ${DST}
	-@echo "Запуск программы ...";echo "";echo ""
	-@${DST}

debug: ${DST}
	-@echo "Отладка программы ...";echo "";echo ""
	-@sdb ${DST}

${DST}: ${SRC}
	-@echo "Программа модифицировалась !"
	-@until ( echo "Компилируем ... ";  ${COMPILE} ); do \
		echo ""; echo "Исправим ... "; red ${SRC}; (sync;exit 0);\
	  done
	-@echo "                 ... all right !"
