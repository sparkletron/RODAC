SYS_DEFINE := _MSX
ARCH  := z80

BASELIBPATH := ../../arch/
BASELIBARCH := msx
BASELIBNAME := $(BASELIBPATH)$(BASELIBARCH)/base.lib

CODELOC   := 0x4100
DATALOC   := 0xE000
IRAMSIZE  := 8192
