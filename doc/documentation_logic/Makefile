#LFLAGS  = -interaction=errorstopmode -halt-on-error
PROJECT = neural_networks
SRC     = $(PROJECT).tex
TARGET  = $(PROJECT).pdf
FILEDIR = files/

FILES = $(PROJECT).aux \
		$(PROJECT).bbl \
		$(PROJECT).blg \
		$(PROJECT).loa \
		$(PROJECT).lof \
		$(PROJECT).log \
		$(PROJECT).lol \
		$(PROJECT).lot \
		$(PROJECT).nav \
		$(PROJECT).out \
		$(PROJECT).pdf \
		$(PROJECT).snm \
		$(PROJECT).toc \
		$(FILEDIR)/bibdb.bib.bak

default:
	pdflatex $(LFLAGS) $(SRC)
	bibtex $(PROJECT)
	pdflatex $(LFLAGS) $(SRC)
	pdflatex $(LFLAGS) $(SRC)
	evince $(PROJECT).pdf

.PHONY: auto

auto:
	pdflatex $(LFLAGS) $(SRC)
#	bibtex $(PROJECT)
#	pdflatex $(LFLAGS) $(SRC)
#	pdflatex $(LFLAGS) $(SRC)

.PHONY: clean

clean:
	@for file in $(FILES); \
		do \
		rm -f $$file; \
		done
