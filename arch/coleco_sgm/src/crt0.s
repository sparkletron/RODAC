; @file     crt0.s
; @author   Jay Convertino
; @email    electrobs@gmail.com
; @date     2024.16.9
; @brief    universal crt

   .module crt0
   .globl _main
   .globl _vdp_irq
   .globl _spin_irq

   .area _HEADER(ABS)
   .org 0x8000

   .db 0xAA,0x55           ; logo screen startup ($55, $AA is test mode)
   .ds 8                   ; 8 bytes will be 0, these are pointers
   .dw _init               ; 2 bytes will be the start pointer of the init
   jp 0                   ; reset 0x08
   jp 0                   ; reset 0x10
   jp 0                   ; reset 0x18
   jp 0                   ; reset 0x20
   jp 0                   ; reset 0x28
   jp 0                   ; reset 0x30
   jp _irq_spin           ; reset 0x38
   jp _irq_nmi
   ; The below is line2/line1/YEAR(4 digit)... also no lower case.
   ; up to 60 bytes of game info for coleco screen $1E+$1F = trade mark $1D = copyright
   ; db "HELLO WORLD!",$1E,$1F,"/JAY CONVERTINO",$1D,"/2022"
   .ds 60
   ; .db _GAME_NAME,"/",_AUTHOR_NAME,"/",_GAME_DATE

_init:
   ;; Set stack pointer directly above top of memory.
   ld	sp, #0x7400 ;; taken from libcv/src/crt0.s

   ;; Initialise global variables
   call  gsinit
   call  sgminit
   call  _main
   rst   0x0

   ;; copied from sdcc src/z80/crt0.s
   .area _CODE
   .area _INITIALIZER
   .area _HOME
   .area _GSINIT
   .area _GSFINAL
   .area _DATA
   .area _INITIALIZED
   .area _BSEG
   .area _BSS
   .area _HEAP
   .area _CODE

_irq_nmi:
   push af
   push bc
   push de
   push hl
   push iy
   call _vdp_irq
   pop iy
   pop hl
   pop de
   pop bc
   pop af
   retn

_irq_spin:
   push af
   push bc
   push de
   push hl
   push iy
   call _spin_irq
   pop iy
   pop hl
   pop de
   pop bc
   pop af
   reti

;; copied from sdcc src/z80/crt0.s
   .area   _GSINIT
gsinit::
   ld bc, #l__INITIALIZER
   ld a, b
   or a, c
   jr Z, gsinit_next
   ld de, #s__INITIALIZED
   ld hl, #s__INITIALIZER
   ldir
gsinit_next:

   .area   _GSFINAL
   ret

;test for ram value, set ram to 32k, test sound r/w.
sgminit:
  nop
