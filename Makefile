TGT=strl-scanner.cc srtl-parser.tab.hh

all:srtl-parser.tab.cc srtl-parser.tab.hh srtl-scanner.cc Makefile2
	make -f Makefile2

Makefile2:specviz.pro

specviz.pro:
	./qmaker.sh
	qmake -o Makefile2

srtl-scanner.cc : srtl-scanner.ll srtl-parser.tab.hh
	flex -o srtl-scanner.cc srtl-scanner.ll
	
srtl-parser.tab.hh : srtl-parser.tab.cc

srtl-parser.tab.cc : srtl-parser.yy
	bison --report=state -b srtl-parser -dv srtl-parser.yy

.PHONY : clean
clean: 
	rm srtl-parser.tab.cc srtl-parser.tab.hh srtl-scanner.cc *.o *.*~ *.pro Makefile2 gccproj_test specviz *~
