all: lsf.pdf shapes.pdf zero.pdf vspline.pdf

%.pdf:%.tex
	pdflatex $< && pdflatex $<

clean:
	rm -f *.aux *.log *.synctex.gz lsf.pdf shapes.pdf ilog.pdf spline.pdf  drvs.pdf conics.pdf zero.pdf newt.pdf vspline.pdf trajectories.pdf *.bbl *.blg diffusion.pdf protodiff.pdf

