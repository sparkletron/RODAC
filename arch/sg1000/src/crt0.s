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
   .org 0x0000
   di
   im 1
   jp _init

   .org 0x0038
   jp _irq_spin

   .org 0x0066
   jp _irq_nmi


   .org 0x0080
_init:
   ;; Set stack pointer directly above top of memory.
   ld	sp, #0xC3FF

   ;; Initialise global variables
   call  gsinit
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
