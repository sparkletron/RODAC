SYS_DEFINE := _COLECO
ARCH  := z80

BASELIBPATH := ../../arch/
BASELIBARCH := coleco
BASELIBNAME := $(BASELIBPATH)$(BASELIBARCH)/base.lib

CODELOC   := 0x8100
DATALOC   := 0x7000
IRAMSIZE  := 1024
