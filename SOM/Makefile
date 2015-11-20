.PHONY: all
all: distance.png position.png

distance.png position.png: graph.plot distance.log position.log
	gnuplot graph.plot

distance.log position.log: a.out animal.dat
	./a.out animal.dat

a.out: SOM.c
	gcc -std=c89 -Wall -lm SOM.c

.PHONY: clean
clean:
	rm *.png *.log a.out
