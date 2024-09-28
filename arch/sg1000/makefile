DIRSRC ?= src
DIRBASE?= base
DIROBJ := obj

SRC    := $(wildcard $(DIRSRC)/*.c)
CRTSRC := $(wildcard $(DIRSRC)/*.s)

CC     := sdcc
CFLAGS := $(if $(CFLAGS),$(CFLAGS) -I. -I../,-c --std-c99 -mz80 --nostdinc --no-std-crt0 --nostdlib -I. -I../)
AR     := sdar
ARFLAGS:= -rc
ASSM   := sdasz80
AFLAGS := -xlos -g

CRTREL := $(addprefix $(DIROBJ)/, $(notdir $(CRTSRC:.s=.rel)))
SRCREL := $(addprefix $(DIROBJ)/, $(notdir $(SRC:.c=.rel)))

LIB    := $(notdir $(SRC:.c=.lib))

.PHONY: clean all

ALL: $(CRTREL) $(LIB)

$(LIB): $(SRCREL)
	$(AR) $(ARFLAGS) $@ $^

$(SRCREL): $(SRC)
	$(CC) -o $@ $(CFLAGS) $<

$(CRTREL): $(CRTSRC) | $(DIROBJ)
	$(ASSM) $(AFLAGS) $@ $<

$(DIROBJ):
	mkdir -p $@

clean:
	rm -rf $(LIB) $(DIROBJ)
