DIRSRC	?= src
DIRARCH	?= arch
DIROBJ 	:= obj

SRC	:= $(wildcard $(DIRSRC)/*.c)
BIN	:= $(notdir $(SRC:.c=.bin))

LIBPATH	:= ../../drivers/tms99XX

ROM_H_GEN	:= ./py/rom_header_gen.py
ROM_DIR 	:= ./roms
ROM_H_DIR	:= ./roms
ROM_H   	:= $(ROM_H_DIR)/roms.h
ROM_F_GEN	:= ./py/rom_file_gen.py

ifeq (coleco,$(MAKECMDGOALS))
include ../../arch/coleco.mk
LIBPATH += ../../drivers/sn76489
BIN    := $(addprefix $(MAKECMDGOALS)_, $(BIN))
DIROBJ := $(addprefix $(MAKECMDGOALS)_, $(DIROBJ))
endif

ifeq (coleco_sgm,$(MAKECMDGOALS))
include ../../arch/coleco_sgm.mk
LIBPATH += ../../drivers/gisnd
LIBPATH += ../../drivers/sn76489
BIN    := $(addprefix $(MAKECMDGOALS)_, $(BIN))
DIROBJ := $(addprefix $(MAKECMDGOALS)_, $(DIROBJ))
endif

ifeq (msx,$(MAKECMDGOALS))
include ../../arch/msx.mk
LIBPATH += ../../drivers/gisnd
BIN    := $(addprefix $(MAKECMDGOALS)_, $(BIN))
DIROBJ := $(addprefix $(MAKECMDGOALS)_, $(DIROBJ))
endif

ifeq (sg1000,$(MAKECMDGOALS))
include ../../arch/sg1000.mk
LIBPATH += ../../drivers/sn76489
BIN    := $(addprefix $(MAKECMDGOALS)_, $(BIN))
DIROBJ := $(addprefix $(MAKECMDGOALS)_, $(DIROBJ))
endif

ROM_OUT		:= $(addprefix complete_, $(BIN))

ifeq (clean,$(MAKECMDGOALS))
include ../../arch/msx.mk
LIBPATH += ../../drivers/gisnd
LIBPATH += ../../drivers/sn76489
endif

ifdef LIBPATH
LIBNAME	:= $(join $(addsuffix /,$(LIBPATH)),$(addsuffix .lib,$(notdir $(LIBPATH))))
endif

# setup a way of changin architecures, maybe different makefiles that creates a default lflags for different code-loc and such.
CRT	:= $(BASELIBPATH)$(BASELIBARCH)/obj/crt0.rel

CC     := sdcc
LFLAGS := -m$(ARCH) -Wl -y --code-loc $(CODELOC) --data-loc $(DATALOC) --std-c99 --no-std-crt0 --iram-size $(IRAMSIZE) $(addprefix -L,$(LIBPATH)) $(addprefix -L,$(BASELIBPATH)$(BASELIBARCH)) $(CRT) $(notdir $(LIBNAME)) $(notdir $(BASELIBNAME))
CFLAGS := -c --std-c99 -m$(ARCH) --no-std-crt0 -D$(SYS_DEFINE) -I$(ROM_H_DIR)
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

.PHONY: clean all $(LIBNAME) $(BASELIBNAME) $(ROM_H) $(ROM_OUT)

all:
	$(error ALL TARGET BUILDS NOTHING, SELECT A SYSTEM AS A TARGET)

coleco: $(BIN) $(ROM_OUT)

coleco_sgm: $(BIN)

msx: $(BIN)

sg1000: $(BIN)

$(BIN): $(SRCIHX)
	$(OBJDMP) -I ihex -O binary $< $@

$(SRCIHX): $(SRCREL) $(BASELIBNAME) $(LIBNAME)
	$(CC) -o $@ $(LFLAGS) $<

$(SRCREL): $(SRC) $(ROM_H) | $(DIROBJ)
	$(CC) -o $@ $(CFLAGS) $<

$(DIROBJ):
	mkdir -p $@

$(ROM_OUT):
	python3 $(ROM_F_GEN) --rom_prime=$(BIN) --roms_path=$(ROM_DIR) --rom_name=$@

$(ROM_H):
	python3 $(ROM_H_GEN) --rom_header=$@ --roms_path=$(ROM_DIR)

$(LIBNAME) :
	$(info BUILDING LIBRARY, $(@F))
	$(MAKE) -B -C $(@D)

$(BASELIBNAME) :
	$(info BUILDING ARCH LIBRARY, $(@F))
	$(MAKE) -B -C $(@D)

clean:
	$(foreach path,$(LIBPATH),$(MAKE) clean -C $(path);)
	rm -rf $(addprefix *_,$(BIN)) $(addprefix *_,$(DIROBJ)) $(ROM_OUT)
