.PHONY: all
all: distance.png position.png output.log

distance.png position.png: graph.plot distance.log position.log
	gnuplot graph.plot

distance.log position.log: a.out animal.dat
	./a.out animal.dat

output.log: a.out animal.dat
	./a.out animal.dat yes > output.log

a.out: SOM.c
	gcc -std=c89 -Wall -lm SOM.c

.PHONY: clean
clean:
	rm *.png *.log a.out output.log

.PHONY: compare
compare:
	gcc -lm -DMAP_SIDE_LENGTH=5 SOM.c && ./a.out animal.dat yes | tail -n 5 > map_5.txt
	gcc -lm -DMAP_SIDE_LENGTH=10 SOM.c && ./a.out animal.dat yes | tail -n 10 > map_10.txt
	gcc -lm -DMAP_SIDE_LENGTH=15 SOM.c && ./a.out animal.dat yes | tail -n 15 > map_15.txt
	rm a.out
