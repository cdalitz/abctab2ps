#
# GNU makefile for abctab2ps
#
# version for GNU C compiler
# and other Unix C compilers
#=========================================


# compiler settings
# (adjust to your system environment)
#-----------------------------------------

# uncomment the following line for universal binary on OSX
#ARCH = -arch i386 -arch ppc

# settings for GNU C compiler
CC = g++
CFLAGS = -Wimplicit -Wformat -g -fpermissive $(ARCH)
#CFLAGS = -Wimplicit -Wformat -Os -fpermissive 
LDFLAGS = -lstdc++ -lm $(ARCH)
LDSTATIC = --static


# uncomment the following line for djgpp
#LDFLAGS = -lstdcxx -lm

# uncomment the following line on MacOS X < 10.2
#CC = cc

# uncomment the following lines on MINT
#OSVARIANT = MINT
#CFLAGS = -Wimplicit -Wformat -g -m68020-60
#LDFLAGS = -m68020-60 -lstdc++

# uncomment the following lines on SGI IRIX
#CC = cc
#CFLAGS = -DEBUG
#LDFLAGS = 
#LDSTATIC = 


# installation directories
# (adjust to your preferences)
#-----------------------------------------

# install root
PREFIX = /usr/local
# directory for the binary
BINDIR = $(PREFIX)/bin
# directory for the man page
MANDIR =  $(PREFIX)/man/man1
# directory for the documentation
DOCDIR =  $(PREFIX)/share/doc/abctab2ps
# directory for the font library
FONTDIR =  $(PREFIX)/share/abctab2ps



# from here on, no alterations
# should be necessary
#-----------------------------------------

PROGRAM = abctab2ps

FONTS = fonts/frBaroqml.ps fonts/frFrancisque.ps \
	fonts/itBorrono.ps fonts/itTimes.ps \
	fonts/deCourier.ps fonts/deFraktur.ps

OBJECTS = abctab2ps.o parse.o syms.o style.o buffer.o \
	format.o pssubs.o subs.o util.o music.o tab.o oscompat.o


$(PROGRAM): $(OBJECTS)
	$(CC) -o $(PROGRAM) $(OBJECTS) $(LDFLAGS)

# MINT needs stacksize reset
ifeq ($(OSVARIANT),MINT)
	stack -S 128k $(PROGRAM)
endif

# windows needs special library
win32: $(OBJECTS)
	$(CC) -o $(PROGRAM) $(OBJECTS) $(LDFLAGS) -lnetapi32

static: $(OBJECTS)
	$(CC) -o $(PROGRAM) $(OBJECTS) $(LDFLAGS) $(LDSTATIC)
	strip $(PROGRAM)

%.o: %.cpp %.h abctab2ps.h
	$(CC) $(CFLAGS) -c $*.cpp

tags: *.cpp *.h
	ctags  --defines --global -T --no-warn  *.cpp *.h

TAGS: *.cpp *.h
	etags *.cpp *.h

test:
	ABCTABFONTS=./fonts ./$(PROGRAM) sample.abc

install:
	@# plausi check on old font installation
	@test \( ! -z "$$ABCTABFONTS" \) -a \( "$$ABCTABFONTS" != "$(FONTDIR)" \) && \
		echo "There seems to be an old installation of $(PROGRAM)" &&\
		echo "Its font directory ABCTABFONTS is set to $$ABCTABFONTS" &&\
		echo "Please uninstall the old version first and unset ABCTABFONTS" &&\
		echo "Remark:" &&\
		echo "The environment variable ABCTABFONTS is no longer necessary" &&\
		echo "unless you install the fonts in an odd location" &&\
		exit 1 || :

	@# actual installation
	install -m 755 -s $(PROGRAM) $(BINDIR)/$(PROGRAM)
	install -d $(MANDIR)
	install -m 644 ../doc/manpages/$(PROGRAM).1 $(MANDIR)/$(PROGRAM).1
	install -d $(FONTDIR)
	install -m 644 $(FONTS) $(FONTDIR)
	install -d $(DOCDIR)
	install -m 644 ../doc/faq.html ../README ../CHANGES ../LICENSE $(DOCDIR)
	install -m 644 ../doc/manpages/abctab2ps-man.html $(DOCDIR)
	install -d $(DOCDIR)/userguide/html
	install -m 644 ../doc/userguide/html/*.html $(DOCDIR)/userguide/html
	install -m 644 ../doc/userguide/html/*.png $(DOCDIR)/userguide/html
	install -m 644 ../doc/userguide/html/*.css $(DOCDIR)/userguide/html
	install -d $(DOCDIR)/examples
	install -m 644 ../doc/examples/Readme ../doc/examples/*.abc $(DOCDIR)/examples

	@# inform user about post install tasks
	@test $(FONTDIR) = /usr/share/abctab2ps -o $(FONTDIR) = /usr/local/share/abctab2ps || {\
		echo "+++++++++++++++++++++++++++++++++++++++++++++++";\
		echo "Please add the environment variable ABCTABFONTS";\
		echo "to your shell's profile and set it to:";\
		echo "    $(FONTDIR)";\
		echo "+++++++++++++++++++++++++++++++++++++++++++++++"; }

uninstall:
	@# uninstall abctab2ps
	rm -f $(BINDIR)/$(PROGRAM)
	rm -f $(MANDIR)/$(PROGRAM).1
	@for f in $(FONTS); do \
		echo "rm -f $(FONTDIR)/`basename $$f`"; \
		rm -f "$(FONTDIR)/`basename $$f`"; \
	done
	rmdir $(FONTDIR) || :
	rm -f $(DOCDIR)/faq.html $(DOCDIR)/abctab2ps-man.html 
	rm -f $(DOCDIR)/README $(DOCDIR)/CHANGES $(DOCDIR)/LICENSE
	rm -f $(DOCDIR)/userguide/html/*
	rmdir $(DOCDIR)/userguide/html || :
	rmdir $(DOCDIR)/userguide || :
	rm -f $(DOCDIR)/examples/*
	rmdir $(DOCDIR)/examples || :

rpm: $(PROGRAM)
	strip $(PROGRAM)
	epm -v -n -f rpm abctab2ps

deb: $(PROGRAM)
	strip $(PROGRAM)
	@test  `id -u` = 0 || { echo "Must be root for 'make deb'"; exit 1; }
	epm -v -n -f deb abctab2ps

osx: $(PROGRAM)
	@test  `id -u` = 0 || { echo "Must be root for 'make osx' (try 'sudo make osx')"; exit 1; }
	epm -v -f osx abctab2ps

universal:
	ARCH="-arch i386 -arch ppc"; export ARCH; make

windist:
	strip abctab2ps.exe
	cd ..; \
	ln -s src/abctab2ps.exe abctab2ps.exe; \
	ln -s src/fonts fonts; \
	zip -ur abctab2ps-win32.zip CHANGES Readme.win32 LICENSE doc abctab2ps.exe fonts/it*.ps fonts/fr*.ps; \
	rm abctab2ps.exe fonts;

clean:
	rm -f *.o $(PROGRAM) Out.ps

