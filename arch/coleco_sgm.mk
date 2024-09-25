SYS_DEFINE := _COLECO_SGM
ARCH  := z80

BASELIBPATH := ../../arch/
BASELIBARCH := coleco_sgm
BASELIBNAME := $(BASELIBPATH)$(BASELIBARCH)/base.lib

CODELOC   := 0x8100
DATALOC   := 0x0100
IRAMSIZE  := 32768
