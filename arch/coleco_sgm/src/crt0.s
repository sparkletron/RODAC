; @file     crt0.s
; @author   Jay Convertino
; @email    electrobs@gmail.com
; @date     2024.16.9
; @brief    universal crt

    _SGM_RAM_ENA_PORT     .equ 0x0053
    _SGM_BIOS_SWAP_PORT   .equ 0x007F
    _NMI_SIZE             .equ (_irq_nmi_end - _irq_nmi + 1)
    _SPIN_SIZE            .equ (_irq_spin_end - _irq_spin + 1)

   .module crt0
   .globl _main
   .globl _vdp_irq
   .globl _spin_irq

   .area _HEADER(ABS)
   .org 0x8000

   .db 0xAA,0x55           ; no logo screen startup ($55, $AA is test mode)
   .ds 8                   ; 8 bytes will be 0, these are pointers
   .dw _init               ; 2 bytes will be the start pointer of the init
   jp 0                   ; reset 0x08
   jp 0                   ; reset 0x10
   jp 0                   ; reset 0x18
   jp 0                   ; reset 0x20
   jp 0                   ; reset 0x28
   jp 0                   ; reset 0x30
   jp 0                   ; reset 0x38
   jp 0
   ; The below is line2/line1/YEAR(4 digit)... also no lower case.
   ; up to 60 bytes of game info for coleco screen $1E+$1F = trade mark $1D = copyright
   ; db "HELLO WORLD!",$1E,$1F,"/JAY CONVERTINO",$1D,"/2022"
   .ds 60
   ; .db _GAME_NAME,"/",_AUTHOR_NAME,"/",_GAME_DATE
_init:
  ;setup for super game module.
  ld a,#0x01
  out (_SGM_RAM_ENA_PORT), a
  ld a,#0x0D
  out (_SGM_BIOS_SWAP_PORT), a
  ;setup ram locations with irq vectors?, 0x8 etc.
  ;reti
  ld a,#0xED
  ld (0x08),a
  ld (0x10),a
  ld (0x18),a
  ld (0x20),a
  ld (0x28),a
  ld (0x30),a
  ld a,#0x4D
  ld (0x09),a
  ld (0x11),a
  ld (0x19),a
  ld (0x21),a
  ld (0x29),a
  ld (0x31),a
  ;copy code to ram
  ;nmi first
  ld bc, #_NMI_SIZE
  ld hl, #_irq_nmi
  ld de, #0x0066
  ldir
  ;spin
  ld bc, #_SPIN_SIZE
  ld hl, #_irq_spin
  ld de, #0x0038
  ldir
  ld	sp, #0x7400 ;; taken from libcv/src/crt0.s
  call gsinit
  call  _main
  rst   0x0

  ;; copied from sdcc src/z80/crt0.s
  .area _HOME
  .area _CODE
  .area _INITIALIZER
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
_irq_nmi_end:

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
_irq_spin_end:

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
