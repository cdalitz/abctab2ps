#
# Makefile for abc2ps user's guide
#
# Targets:
#
#	make dvi      (makes DVI output from TEX-file)
#	make ps       (makes Postscript Output from DVI-File)
#	make pdf      (makes PDF Output from DVI-File)
#	make html     (makes HTML Output from TEX-File)
#	make abc      (makes all music examples)
#	make tar      (makes archive)
#

UGUIDE = userguide
# subdirectory for HTML version
HTMLDIR = html

# sample files
FORMATFILE = samples.fmt
SAMPLES = sample1.eps sample2.eps sample3.eps sample4a.eps sample4b.eps\
	sample5a.eps sample5b.eps sample6a.eps sample6b.eps sample6c.eps\
	sample7a.eps sample7b.eps sample7c.eps

# Options for abctab2ps
ABCTAB2PS = ../../src/abctab2ps
ABCFLAGS = -F $(FORMATFILE) -E

# directory with latex2html icons
L2HICONS = /sw/share/lib/latex2html/icons

ps: $(UGUIDE).ps

pdf: $(UGUIDE).pdf

dvi: $(UGUIDE).dvi

abc: $(SAMPLES)

html: $(HTMLDIR)/$(UGUIDE).html

tar:
	tar cvf $(UGUIDE).tar $(FORMATFILE) *.abc $(SAMPLES) $(UGUIDE).dvi $(UGUIDE).tex makefile
	gzip $(UGUIDE).tar

%.dvi: %.tex
	latex $*.tex
	latex $*.tex
	makeindex $*.idx
	latex $*.tex

$(UGUIDE).ps: dvi
	dvips -o $@ $(UGUIDE).dvi

$(UGUIDE).pdf: $(UGUIDE).tex
	pdflatex $<
	pdflatex $<
	makeindex $<
	pdflatex $<

%.eps: %.abc $(FORMATFILE)
	$(ABCTAB2PS) $< -O =
	ps2eps -l -f $*.ps
	epstopdf $*.eps
	rm $*.ps

$(HTMLDIR)/$(UGUIDE).html: $(UGUIDE).tex
	# note that -no_transparent and -white have no effect for some images
	# instead you need the following line in /etc/latex2html.conf
	# (respectively /sw/lib/latex2html/l2hconf.pm for Fink):
	#  $LATEX_COLOR = "\\pagecolor[white]{1.0}";
	latex2html \
		-split 4 -link 3 \
		-dir $(HTMLDIR) -local_icons \
		-index_in_navigation -no_footnode -info 0 \
		-white \
		$(UGUIDE).tex
	# image creation is broken in homebrew version of latex2html
	rm -f html/image.png html/img*.png html/img*.old
	cp pngimages-saved/*.png html/

clean:
	rm -f $(UGUIDE).ps $(UGUIDE).pdf $(UGUIDE).aux \
		$(UGUIDE).idx $(UGUIDE).ilg $(UGUIDE).dvi $(UGUIDE).log






