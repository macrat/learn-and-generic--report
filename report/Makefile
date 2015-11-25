TEX = platex
DVIPDF = $(shell if type dvipdfmx 2>&1 >>/dev/null; then echo "dvipdfmx"; else echo "dvipdf"; fi)


.PHONY: all
all: report.pdf

report.pdf: report.dvi
	${DVIPDF} $<

report.dvi: report.tex ../Hopfield/Hopfield.c ../BP/BP.c ../SOM/SOM.c ../GA/GA.c
	${TEX} $<
	${TEX} $<
	${TEX} $<

.PHONY: clean
clean:
	-rm *.{dvi,log,aux}

.PHONY: cleanall
cleanall: clean
	-rm *.pdf
