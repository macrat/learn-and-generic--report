.PHONY: all
all: result.png advance.png output.log

result.png advance.png: graph.plot result.log advance.log
	gnuplot graph.plot

result.log advance.log: a.out
	./a.out

output.log: a.out
	./a.out > output.log

a.out: GA.c
	gcc -std=c89 -Wall GA.c

.PHONY: clean
clean:
	-rm *.log *.png a.out output.log
	-rm -r compare

.PHONY: compare
compare:
	-mkdir compare
	gcc -DOVERRIDE_PARAMS \
		-DGENE_LENGTH=100 -DGENE_NUM=40 -DMUTATION_RATE=0.01 \
		-DLOOP_NUM=100000 \
		-DCROSS_TYPE=0 -DCHOICE_TYPE=0 \
		-USHOW_VERBOSE -DSTOP_WHEN_DONE \
		GA.c
	./a.out > compare/one-roullette.log
	gnuplot graph.plot
	mv advance.png compare/one-roullette.png
	gcc -DOVERRIDE_PARAMS \
		-DGENE_LENGTH=100 -DGENE_NUM=40 -DMUTATION_RATE=0.01 \
		-DLOOP_NUM=100000 \
		-DCROSS_TYPE=1 -DCHOICE_TYPE=0 \
		-USHOW_VERBOSE -DSTOP_WHEN_DONE \
		GA.c
	./a.out > compare/two-roullette.log
	gnuplot graph.plot
	mv advance.png compare/two-roullette.png
	gcc -DOVERRIDE_PARAMS \
		-DGENE_LENGTH=100 -DGENE_NUM=40 -DMUTATION_RATE=0.01 \
		-DLOOP_NUM=100000 \
		-DCROSS_TYPE=2 -DCHOICE_TYPE=0 \
		-USHOW_VERBOSE -DSTOP_WHEN_DONE \
		GA.c
	./a.out > compare/rand-roullette.log
	gnuplot graph.plot
	mv advance.png compare/rand-roullette.png
	\
	gcc -DOVERRIDE_PARAMS \
		-DGENE_LENGTH=100 -DGENE_NUM=40 -DMUTATION_RATE=0.01 \
		-DLOOP_NUM=100000 \
		-DCROSS_TYPE=0 -DCHOICE_TYPE=1 \
		-USHOW_VERBOSE -DSTOP_WHEN_DONE \
		GA.c
	./a.out > compare/one-tournament.log
	gnuplot graph.plot
	mv advance.png compare/one-tournament.png
	gcc -DOVERRIDE_PARAMS \
		-DGENE_LENGTH=100 -DGENE_NUM=40 -DMUTATION_RATE=0.01 \
		-DLOOP_NUM=100000 \
		-DCROSS_TYPE=1 -DCHOICE_TYPE=1 \
		-USHOW_VERBOSE -DSTOP_WHEN_DONE \
		GA.c
	./a.out > compare/two-tournament.log
	gnuplot graph.plot
	mv advance.png compare/two-tournament.png
	gcc -DOVERRIDE_PARAMS \
		-DGENE_LENGTH=100 -DGENE_NUM=40 -DMUTATION_RATE=0.01 \
		-DLOOP_NUM=100000 \
		-DCROSS_TYPE=2 -DCHOICE_TYPE=1 \
		-USHOW_VERBOSE -DSTOP_WHEN_DONE \
		GA.c
	./a.out > compare/rand-tournament.log
	gnuplot graph.plot
	mv advance.png compare/rand-tournament.png
