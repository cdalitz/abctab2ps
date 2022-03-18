#
# makefile for abctab2ps examples
#

# set paths
ABCTAB2PS   = ../../src/abctab2ps
#ABCTAB2PS   = abctab2ps


# from here on, no alterations
# should be necessary
#--------------------------------

EXAMPLES = kuehnel.ps \
	regnart.ps \
	victoria.ps \
	ortiz.ps \
	vallet.ps \
	francisque.ps \
	daCrema.ps \
	kapsberger.ps \
	telemann.ps \
	banjo.ps \
	attaignant.ps \
	carcassi.ps \
	molinaro.ps \
	bataille.ps \
	sanz.ps \
	adriaenssen.ps

all: $(EXAMPLES)

%.ps: %.abc
	$(ABCTAB2PS) -O $@ $<
	#mv $@ $@.old

clean:
	rm -f $(EXAMPLES)

