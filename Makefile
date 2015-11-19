C0114265.tar.gz: $(ls */*.c)
	-rm */a.out 2>>/dev/null
	tar cvzf $@ `ls | grep -v Makefile | grep -v tar.gz`
