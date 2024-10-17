DIRSRC ?= src
DIROBJ := obj

SRC    := $(wildcard $(DIRSRC)/*.c)

CC     := sdcc
CFLAGS := $(if $(CFLAGS),$(CFLAGS) -I. -I$(BASELIBPATH) -I$(BASELIBPATH)$(BASELIBARCH),)
AR     := sdar
ARFLAGS:= -rc

SRCREL := $(addprefix $(DIROBJ)/, $(notdir $(SRC:.c=.rel)))

LIB    := $(notdir $(SRC:.c=.lib))

.PHONY: clean all

ALL: $(LIB)

$(LIB): $(SRCREL)
	$(AR) $(ARFLAGS) $@ $^

$(SRCREL): $(SRC) | $(DIROBJ)
	$(CC) -o $@ $(CFLAGS) $<

$(DIROBJ):
	mkdir -p $@

clean:
	rm -rf $(LIB) $(DIROBJ)
