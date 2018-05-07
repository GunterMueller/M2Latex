#
#       Makefile for Modula2toLaTeX on Sun4
#       Copyright (C) 1991 by joke

CC      = cc
CFLAGS  = -O2
LDFLAGS = -s

#       Flex should be Version 2.3 or higher
LEX     = flex

TROFF   = psroff


################## M2LaTeX #################################################

m2latex:        m2latex.o getopt1.o getopt.o version.o
	$(CC) $(LDFLAGS) -o m2latex m2latex.o getopt1.o getopt.o version.o

m2latex.o: m2latex.l main.c

demo:   demo.mi
	m2latex -h -F it demo.mi
	latex demo.mi.tex

all:    m2latex demo


################### System Set-up ##########################################

#       m2latex and a link called p2latex get here:
BINDIR  =       ../../bin

#       The fancyheadings.sty file gets here:
TEXDIR  =       ../../tex/inputs

#       The manual page gets here:
MANDIR  =       ../../man/man1

install: deinstall
	cp m2latex $(BINDIR)
	ln -s $(BINDIR)/m2latex $(BINDIR)/p2latex
	cp fancyheadings.sty $(TEXDIR)
	cp m2latex.1 $(MANDIR)

deinstall:
	rm $(BINDIR)/m2latex
	rm $(BINDIR)/p2latex
	rm $(TEXDIR)/fancyheadings.sty
	rm $(MANDIR)/m2latex.1


################### Packaging ###############################################
SRC     =       .
VERS    =       -1.0.1

tape:   m2latex.c man
	rm -f Modula2LaTeX$(VERS).tar.Z
	tar cf Modula2LaTeX$(VERS).tar $(SRC)/LICENSE \
		$(SRC)/Makefile \
		$(SRC)/README \
		$(SRC)/main.c $(SRC)/demo.mi \
		$(SRC)/m2latex.man \
		$(SRC)/m2latex.1 $(SRC)/m2latex.c $(SRC)/m2latex.l \
		$(SRC)/fancyheadings.sty $(SRC)/getopt.c \
		$(SRC)/getopt.h $(SRC)/getopt1.c $(SRC)/version.c
	compress Modula2LaTeX$(VERS).tar


################### Manuals #################################################
psman:
	$(TROFF) -man m2latex.1 >m2latex.man.ps

man catman:
	nroff -man m2latex.1 >m2latex.man


################## Cleanup ##################################################
clean:
	rm -f m2latex.c *.mi.tex *.aux *.log *.mi.dvi *.man *.ps *.Z

clobber: clean
	rm -f *.o m2latex

