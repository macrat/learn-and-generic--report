.PHONY: all
all: a.out output.log error.png

output.log: a.out
	./a.out 6 20 > output.log

a.out: Hopfield.c
	gcc -std=c89 -Wall Hopfield.c

error.png: graph.plot error.txt
	gnuplot graph.plot

error.txt: Makefile a.out
	:> $@
	for err in `seq 0 2 100`; do echo "$${err}%"; echo "$${err} `./a.out 6 $${err} 2000 | grep -o '[0-9][0-9.]*'`" >> $@; done

.PHONY: clean
clean:
	rm a.out output.log
