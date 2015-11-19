STUDENT_ID = C0114265


${STUDENT_ID}.tar.gz: $(shell ls */*.c)
	tar cvzf $@ --exclude a.out `ls | grep -v Makefile | grep -v $@`

.PHONY: clean
clean:
	-rm */a.out
	-rm ${STUDENT_ID}.tar.gz
