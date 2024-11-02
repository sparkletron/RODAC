# Hello World

This is an example application that puts the system VDP in text mode and prints hello world to the screen.

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
  - make coleco_sgm
  - make msx
  - make sg1000

## details
This application will put the VDP into text mode which is 40 column mode with no sprites.
Then it will print the full character set to the top of the screen. After this it prints
hello_world to the center and scrolls in horizontally, infinitely across the screen.
