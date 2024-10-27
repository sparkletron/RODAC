SRCDIR=src
SRC=$(wildcard $(SRCDIR)/*.tex)
PDF=$(notdir $(SRC:.tex=.pdf))
AUX=$(PDF:.pdf=.aux)
LOG=$(PDF:.pdf=.log)
TOC=$(PDF:.pdf=.toc)
OUT=$(PDF:.pdf=.out)
WIKI=Home.md
WIKI_DIR=RODAC.wiki
HTML=index.html
HTML_DIR=html
DOXYGEN=../../drivers/tms99XX ../../drivers/sn76489 ../../drivers/gisnd

.PHONY: $(DOXYGEN) clean $(WIKI_DIR)/$(WIKI) $(PDF) $(HTML_DIR)

all: $(DOXYGEN) $(PDF) $(WIKI_DIR)/$(WIKI) $(HTML_DIR)

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

$(HTML_DIR): $(SRC)
	mkdir -p $(HTML_DIR)
	latex2html $< -dir $@

clean:
	rm -rf $(PDF) dox/$(notdir ${DOXYGEN}) $(WIKI_DIR)/$(WIKI) $(AUX) $(LOG) $(TOC) $(OUT) $(addsuffix .pdf, $(notdir ${DOXYGEN})) $(HTML_DIR)
