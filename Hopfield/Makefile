.PHONY: all
all: a.out output.log

output.log: a.out
	./a.out 6 20 > output.log

a.out: Hopfield.c
	gcc -std=c89 -Wall Hopfield.c

.PHONY: clean
clean:
	rm a.out output.log
