.PHONY: all
all: result.png advance.png

result.png advance.png: graph.plot result.log advance.log
	gnuplot graph.plot

result.log advance.log: a.out
	./a.out

a.out: GA.c
	gcc -std=c89 -Wall GA.c

.PHONY: clean
clean:
	-rm *.log *.png a.out