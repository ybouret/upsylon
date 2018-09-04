all: chem.pdf

%.pdf:%.tex
	pdflatex $< && pdflatex $<

clean:
	rm -f *.aux *.log *.synctex.gz chem.pdf

