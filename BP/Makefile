.PHONY: all
all: error.png output.log

error.png: learning.log graph.plot
	gnuplot graph.plot

output.log: a.out xor.dat
	./a.out xor.dat > output.log

learning.log: a.out xor.dat
	./a.out xor.dat

a.out: BP.c
	gcc -std=c89 -Wall -lm BP.c

.PHONY: clean
clean:
	rm a.out learning.log error.png output.log
