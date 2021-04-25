all: alchemy.pdf

%.pdf:%.tex
	pdflatex $< && pdflatex $<

clean:
	rm -f *.aux *.log *.synctex.gz alchemy.pdf

