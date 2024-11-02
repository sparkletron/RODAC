# Multicart

This application will print a list of ROMS to the screen, and when selected read memory address that bank switches the ROM requested.

author: Jay Convertino

data: 2024.11.02

license: MIT

## Release Versions
### Current
  - pre-alpha

### Past
  - none

## Requirements
  - sdcc v4.0.0 or greater

## Building
  - make coleco

## details
This application will put the VDP into text mode which is 40 column mode with no sprites.
Then it will print the header and all of the ROMS for selection to the screen. The text is
inverted(highlighted) to show which one is the active selection. Once a ROM is selected the
program will then read a memory address of E001 + the offset. This read location indicates
the ROM to bank switch out. For the coleco this is handled by an PIC microcontroller. That
design is in a separate repo call Colecovision_Multicart available at github.com/sparkletron.
Once the selection is made the application will halt and wait for reset. The user is notified
with text on screen of this request.
