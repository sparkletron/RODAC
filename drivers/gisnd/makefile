DIRSRC ?= src
DIROBJ := obj

SRC    := $(wildcard $(DIRSRC)/*.c)
HEADER := $(notdir $(SRC:.c=.h))

CC     := sdcc
CFLAGS := $(if $(CFLAGS),$(CFLAGS) -I. -I$(BASELIBPATH) -I$(BASELIBPATH)$(BASELIBARCH),)
AR     := sdar
ARFLAGS:= -rc

SRCREL := $(addprefix $(DIROBJ)/, $(notdir $(SRC:.c=.rel)))

LIB    := $(notdir $(SRC:.c=.lib))

DOXYGEN_GEN = doxygen
DOXYGEN_CFG = dox.cfg

.PHONY: clean all dox_gen

ALL: $(LIB)

$(LIB): $(SRCREL)
	$(AR) $(ARFLAGS) $@ $^

$(SRCREL): $(SRC) | $(DIROBJ)
	$(CC) -o $@ $(CFLAGS) $<

$(DIROBJ):
	mkdir -p $@

dox_gen:
	$(DOXYGEN_GEN) $(DOXYGEN_CFG) $(HEADER)

clean:
	rm -rf $(LIB) $(DIROBJ)
