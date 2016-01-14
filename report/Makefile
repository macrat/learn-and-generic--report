TEX = platex
DVIPDF = $(shell if type dvipdfmx 2>&1 >>/dev/null; then echo "dvipdfmx"; else echo "dvipdf"; fi)
SOURCODES = $(shell ls ../*/*.c)
OUTPUT_LOGS = $(shell ls ../*/*.c | sed -e 's/[^/]*$$/output.log/')


.PHONY: all
all: report.pdf

report.pdf: report.dvi
	${DVIPDF} $<

report.dvi: report.tex ${SOURCODES} ${OUTPUT_LOGS} BP.tex GA.tex Hopfield.tex SOM.tex BP/analog_xor_head.dat
	${TEX} $<
	${TEX} $<
	${TEX} $<

${OUTPUT_LOGS}: ${SOURCODES}
	cd ../BP/ && make output.log
	cd ../GA/ && make output.log
	cd ../Hopfield/ && make output.log
	cd ../SOM/ && make output.log

BP.tex: ../BP/BP.c docstring.py
	python docstring.py ../BP/BP.c > BP.tex

GA.tex: ../GA/GA.c docstring.py
	python docstring.py ../GA/GA.c > GA.tex

Hopfield.tex: ../Hopfield/Hopfield.c docstring.py
	python docstring.py ../Hopfield/Hopfield.c > Hopfield.tex

SOM.tex: ../SOM/SOM.c docstring.py
	python docstring.py ../SOM/SOM.c > SOM.tex

BP/analog_xor_head.dat: ../BP/analog_xor.dat
	head -n 10 $< > BP/analog_xor_head.dat

.PHONY: clean
clean:
	-rm *.{dvi,log,aux,toc} BP.tex GA.tex Hopfield.tex SOM.tex

.PHONY: cleanall
cleanall: clean
	-rm *.pdf
