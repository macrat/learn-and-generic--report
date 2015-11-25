TEX = platex
DVIPDF = $(shell if type dvipdfmx 2>&1 >>/dev/null; then echo "dvipdfmx"; else echo "dvipdf"; fi)

SOURCES = $(shell ls *.tex)
CHILDREN = $(shell grep -o '\\\(input\|include\){.*\?}' *.tex | sed -e 's/.*\?{//' -e 's/}.*//' | sort -u | xargs -I{} echo {}.tex)
BASES = $(shell python -c 'print(" ".join(set("${SOURCES}".split(" ")) - set("${CHILDREN}".split(" "))))')

PDFS = $(shell echo ${BASES} | sed "s/.tex/.pdf/g")
DVIS = $(shell echo ${BASES} | sed "s/.tex/.dvi/g")
PLOTTERS = $(shell ls *.plt 2>/dev/null)

PLOTS = $(shell grep output *.plt 2>/dev/null | grep -o '".*"' | sed -e 's/"//g')
GRAPHICS = $(shell grep includegraphics *.tex | grep -o '{.*}' | sed -e 's/[{}]//g')
DEPENDENCIES = $(shell python -c 'print(" ".join(set("${GRAPHICS}".split(" ")) - set("${PLOTS}".split(" "))))')

.PHONY: all
all: environ check ${PDFS};

.PHONY: check
check: environ
	@flg=0; \
	if [ "`ls *.tex`" == "" ]; then \
		echo 'error: tex file not found.' >&2; flg=1; \
	fi; \
	if [ "${DEPENDENCIES}" != "" ]; then \
		for x in ${DEPENDENCIES}; do \
			if [ ! -f $$x ]; then \
				echo "error: no such image file: $$x" >&2; flg=1; \
			fi; \
		done; \
	fi; \
	nolabels="$(shell python -c 'print(" ".join(set(r"$(shell grep -o '\\ref{[^}]*}' *.tex | sed -e 's/\\ref{//' -e 's/}//')".split()) - set(r"$(shell grep -o '\\label{[^}]*}' *.tex | sed -e 's/\\label{//' -e 's/}//')".split())))')"; \
	if [ "$$nolabels" != "" ]; then \
		echo "error: label used but not defined: $$nolabels"; flg=1; \
	fi; \
	notfounds="$(shell python -c 'print(" ".join(set("${CHILDREN}".split(" ")) - set("${SOURCES}".split(" "))))')"; \
	if [ "$$notfounds" != "" ]; then \
		echo "error: included tex not found: $$notfounds"; flg=1; \
	fi; \
	dontuse="$(shell python -c 'print(" ".join(set("${PLOTS}".split(" ")) - set("${GRAPHICS}".split(" "))))')"; \
	if [ "$$dontuse" != "" ]; then \
		echo "warning: plot graph but don't use: $$dontuse"; \
	fi; \
	manyuse="`python -c 'ls=r"${GRAPHICS}".split(); print(" ".join(filter(lambda x:ls.count(x)>1, set(ls))))'`"; \
	if [ "$$manyuse" != "" ]; then \
		echo "warning: image file is many used: $$manyuse"; \
	fi; \
	if [ $$flg -ne 0 ]; then \
		! : ; \
	fi

.PHONY: help
help:
	@echo report template
	@echo '        MIT License (c)2015 MacRat'
	@echo
	@echo make all : make report file.
	@echo make clean : cleanup temporary files.
	@echo make cleanall : cleanup all compiled file.
	@echo make environ : show directory information.
	@echo make check : validate tex file.
	@echo make help : show this.

.PHONY: environ
environ:
	@echo TEX: ${TEX}
	@echo DVIPDF: ${DVIPDF}
	@echo PLOTTERS: $(PLOTTERS)
	@echo
	@echo tex sources: ${SOURCES}
	@echo base sources: ${BASES}
	@echo included sources: ${CHILDREN}
	@echo
	@echo graph with gnuplot: ${PLOTS}
	@echo dependency graphics: ${DEPENDENCIES}
	@echo output: ${PDFS}
	@echo

.SUFFIXES: .tex .dvi .pdf

${PDFS}: ${DVIS} ${DEPENDENCIES}
	${DVIPDF} $*

${DVIS}: ${PLOTS} $(shell ls *.tex 2>/dev/null) ../Hopfield/Hopfield.c ../BP/BP.c ../SOM/SOM.c ../GA/GA.c
	${TEX} $*
	${TEX} $*
	${TEX} $*

${PLOTS}: $(PLOTTERS)
	gnuplot $(PLOTTERS)

.PHONY: clean
clean:
	-rm *.{dvi,log,aux}

.PHONY: cleanall
cleanall: clean
	-rm *.pdf ${PLOTS}
