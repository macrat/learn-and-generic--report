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

${STUDENT_ID}.tar.gz: $(shell ls */*.c */*.dat */*.plot */Makefile)
	tar cvzf $@ --exclude a.out --exclude *.log --exclude *.png `ls | grep -v Makefile | grep -v $@`

.PHONY: clean
clean:
	-rm ${STUDENT_ID}.tar.gz
	-cd BP && make clean
	-cd GA && make clean
	-cd Hopfield && make clean
	-cd SOM && make clean
	cd report && make cleanall
