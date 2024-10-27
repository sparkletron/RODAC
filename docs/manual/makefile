SRCDIR=src
SRC=$(wildcard $(SRCDIR)/*.tex)
PDF=$(notdir $(SRC:.tex=.pdf))
AUX=$(PDF:.pdf=.aux)
LOG=$(PDF:.pdf=.log)
TOC=$(PDF:.pdf=.toc)
OUT=$(PDF:.pdf=.out)
WIKI=Home.md
WIKI_DIR=RODAC.wiki
DOXYGEN=../../drivers/tms99XX ../../drivers/sn76489 ../../drivers/gisnd

.PHONY: $(DOXYGEN) clean $(WIKI_DIR)/$(WIKI) $(PDF)

all: $(DOXYGEN) $(PDF) $(WIKI_DIR)/$(WIKI)

$(DOXYGEN):
	make -C $@ dox_gen
	make -C dox/$(notdir $@)/latex
	mv dox/$(notdir $@)/latex/refman.pdf $(notdir $@).pdf

$(PDF): $(SRC)
	pdflatex -shell-escape -interaction=batchmode $<
	pdflatex -shell-escape -interaction=batchmode $<
	rm $(AUX) $(LOG) $(TOC) $(OUT)

$(WIKI_DIR)/$(WIKI): $(SRC)
	pandoc -f latex -t gfm -s -o $@ $<

clean:
	rm -rf $(PDF) dox/$(notdir ${DOXYGEN}) $(WIKI_DIR)/$(WIKI) $(AUX) $(LOG) $(TOC) $(OUT) $(notdir ${DOXYGEN}).pdf

# 	latex2html src/RODAC.tex -dir ../
