all:

%.pdf:%.tex
	pdflatex $< && pdflatex $<

clean:
	rm -f *.aux *.log *.synctex.gz histogram.pdf

