.PHONY: all
all: error.png

error.png: learning.log graph.plot
	gnuplot graph.plot

learning.log: a.out xor.dat
	./a.out xor.dat

a.out: BP.c
	gcc -std=c89 -Wall -lm BP.c

.PHONY: clean
clean:
	rm a.out learning.log error.png
