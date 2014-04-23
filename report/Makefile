TEX=pdflatex
FILES=abstract.tex implementation.tex operation.tex main.bib title.tex intro.tex main.tex problems.tex ui.tex future.tex class.tex howtorun.tex software.tex classdiag.png expand.png logo.png mainui.png collapse.png

all:main.pdf

main.pdf:$(FILES)
	pdflatex main

.PHONY: clean
clean:
	rm *.aux *.log *.backup *.blg *.toc *~ *.*~ main.pdf main.dvi 2>/dev/null
