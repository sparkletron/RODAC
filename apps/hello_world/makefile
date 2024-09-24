DIRSRC	?= src
DIRARCH	?= arch
DIROBJ 	:= obj

SRC	:= $(wildcard $(DIRSRC)/*.c)
BIN	:= $(notdir $(SRC:.c=.bin))

ifeq (coleco,$(MAKECMDGOALS))
include ../../arch/coleco.mk
BIN    := $(addprefix $(MAKECMDGOALS)_, $(BIN))
DIROBJ := $(addprefix $(MAKECMDGOALS)_, $(DIROBJ))
endif

# setup a way of changin architecures, maybe different makefiles that creates a default lflags for different code-loc and such.
CRT	:= $(BASELIBPATH)$(BASELIBARCH)/obj/crt0.rel

LIBPATH	= ../../drivers/tms99XX
ifdef LIBPATH
LIBNAME	:= $(addsuffix .lib,$(join $(LIBPATH)/,$(notdir $(LIBPATH))))
endif

CC     := sdcc
LFLAGS := -m$(ARCH) -Wl -y --code-loc $(CODELOC) --data-loc $(DATALOC) --std-c99 --no-std-crt0 --iram-size $(IRAMSIZE) $(addprefix -L,$(LIBPATH)) $(addprefix -L,$(BASELIBPATH)$(BASELIBARCH)) $(CRT) $(notdir $(LIBNAME)) $(notdir $(BASELIBNAME))
CFLAGS := -c --std-c99 -m$(ARCH) --no-std-crt0 -D$(SYS_DEFINE)
OBJDMP := sdobjcopy

ifneq ($(LIBPATH),)
CFLAGS += $(addprefix -I,$(LIBPATH)) $(addprefix -I,$(BASELIBPATH)) $(addprefix -I,$(BASELIBPATH)$(BASELIBARCH))
else
CFLAGS += -I$(BASELIBPATH)
endif

export BASELIBPATH
export BASELIBARCH
export CFLAGS

SRCREL := $(addprefix $(DIROBJ)/, $(notdir $(SRC:.c=.rel)))
SRCIHX := $(addprefix $(DIROBJ)/, $(notdir $(SRC:.c=.ihx)))

.PHONY: clean coleco all $(LIBNAME) $(BASELIBNAME)

all:
	$(error ALL TARGET BUILDS NOTHING, SYSTEM MUST BE THE TARGET)

coleco: $(BASELIBNAME) $(LIBNAME) $(BIN)

$(BIN): $(SRCIHX)
	$(OBJDMP) -I ihex -O binary $< $@

$(SRCIHX): $(SRCREL)
	$(CC) -o $@ $(LFLAGS) $^

$(SRCREL): $(SRC) | $(DIROBJ)
	$(CC) -o $@ $(CFLAGS) $<

$(DIROBJ):
	mkdir -p $@

$(LIBNAME) :
	$(MAKE) -C $(@D)

$(BASELIBNAME) :
	$(MAKE) -C $(@D)

clean:
	$(foreach path,$(LIBPATH),$(MAKE) clean -C $(path))
	rm -rf $(addprefix *_,$(BIN)) $(addprefix *_,$(DIROBJ))
