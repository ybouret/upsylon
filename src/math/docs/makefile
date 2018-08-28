all: lsf.pdf

%.pdf:%.tex
	pdflatex $< && pdflatex $<

clean:
	rm -f *.aux *.log *.synctex.gz lsf.pdf

