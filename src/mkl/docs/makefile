all: lsf.pdf shapes.pdf zero.pdf

%.pdf:%.tex
	pdflatex $< && pdflatex $<

clean:
	rm -f *.aux *.log *.synctex.gz lsf.pdf shapes.pdf ilog.pdf spline.pdf abc.pdf drvs.pdf c2arc.pdf min.pdf conics.pdf zero.pdf

