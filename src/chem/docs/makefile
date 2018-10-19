all: chem.pdf spectro.pdf fluo.pdf

%.pdf:%.tex
	pdflatex $< && pdflatex $<

clean:
	rm -f *.aux *.log *.synctex.gz chem.pdf spectro.pdf fluo.pdf

