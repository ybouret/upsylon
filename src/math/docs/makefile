all: lsf.pdf shapes.pdf

%.pdf:%.tex
	pdflatex $< && pdflatex $<

clean:
	rm -f *.aux *.log *.synctex.gz lsf.pdf shapes.pdf ilog.pdf spline.pdf abc.pdf drvs.pdf

