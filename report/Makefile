TEX = platex
DVIPDF = $(shell if type dvipdfmx 2>&1 >>/dev/null; then echo "dvipdfmx"; else echo "dvipdf"; fi)
SOURCODES = $(shell ls ../*/*.c)
OUTPUT_LOGS = $(shell ls ../*/*.c | sed -e 's/[^/]*$$/output.log/')


.PHONY: all
all: report.pdf

report.pdf: report.dvi ${SOURCODES} ${OUTPUT_LOGS}
	${DVIPDF} $<

report.dvi: report.tex
	${TEX} $<
	${TEX} $<
	${TEX} $<

${OUTPUT_LOGS}: ${SOURCODES}
	cd ../BP/ && make output.log
	cd ../GA/ && make output.log
	cd ../Hopfield/ && make output.log
	cd ../SOM/ && make output.log

.PHONY: clean
clean:
	-rm *.{dvi,log,aux,toc}

.PHONY: cleanall
cleanall: clean
	-rm *.pdf
