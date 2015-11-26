STUDENT_ID = C0114265

.PHONY: tar
tar: ${STUDENT_ID}.tar.gz

.PHONY: all
all:
	cd BP && make
	cd GA && make
	cd Hopfield && make
	cd SOM && make
	cd report && make

.PHONY: a.out
a.out:
	cd BP && make a.out
	cd GA && make a.out
	cd Hopfield && make a.out
	cd SOM && make a.out

.PHONY: report
report:
	cd report && make

${STUDENT_ID}.tar.gz: Makefile $(shell ls */*.c */*.dat */*.plot */Makefile report/*.tex report/*.sty)
	cd ../ && tar cvzf $(shell pwd)/$@ `find ./$(shell basename `pwd`) -name *.c -or -name *.dat -or -name *.plot -or -name Makefile -or -name report.tex -or -name *.sty | grep -v '\./Makefile'`

.PHONY: clean
clean:
	-rm ${STUDENT_ID}.tar.gz
	-cd BP && make clean
	-cd GA && make clean
	-cd Hopfield && make clean
	-cd SOM && make clean
	cd report && make cleanall
