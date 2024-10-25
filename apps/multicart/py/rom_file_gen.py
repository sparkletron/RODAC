#!/usr/bin/env python3
################################################################################
# @file   rom_file_gen.py
# @author Jay Convertino(jayconvertino@outlook.com)
# @date   2024.10.24
# @brief  Generate a rom file in bank sizes up to max size
#
# @license MIT
# Copyright 2024 Jay Convertino
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.
################################################################################
import argparse
import sys
import re
import os

def main():
  args = parse_args(sys.argv[1:])

  bank_size_bytes = args.bank_size * 1024

  max_size_bytes = args.max_size * 1024

  rom_file = None

  rom_buffer = bytes()

  #this is for the max number of additional roms. Minus one for rom_prime.
  max_roms = (args.max_size/args.bank_size)-1

  # get a list of all files from the rom path
  dir_list_all = os.listdir(args.roms_path)

  #sort alphabetically
  dir_list_all.sort()

  #filter out anything that doesn't end in .col'
  dir_list_filtered = [item for item in dir_list_all if item.endswith(".col")]

  #check the number of roms found
  if len(dir_list_filtered) > max_roms:
    del dir_list_filtered[max_roms:]
    print("MAX NUMBER OF ROMS EXCEDED, TRUNCATING")

  if len(dir_list_filtered) < max_roms:
    print("NUMBER OF ROMS LESS THAN MAX")

  try:
    rom_file = open(args.rom_name, 'wb')
  except FileNotFoundError as e:
    print(str(e))
    exit(0)

  #open rom prime, read full amount to buffer, determine and check size, and then add padding to buffer
  rom_buffer += read_file_and_pad(args.rom_prime, bank_size_bytes)

  #open each rom file, read full amount to buffer, determine and size, and then add padding to buffer. f"{args.roms_path}/file_name"
  for file_name in dir_list_filtered:
    rom_buffer += read_file_and_pad(f"{args.roms_path}/{file_name}", bank_size_bytes)

  #write buffer to rom_name file
  rom_file.write(rom_buffer)

  rom_file.close()


  # try:
  #   with open(args.rom_header, 'r') as file:
  #     lines = file.readlines()
  # except FileNotFoundError as e:
  #   print(str(e))
  #
  # for i, line in enumerate(lines):
  #   temp_list = p.findall(line)
  #   if temp_list:
  #     strip_list = list(temp_list[0])
  #     strip_list[2] = dir_string
  #     lines[i] = ''.join(strip_list)
  #
  # try:
  #   with open(args.rom_header, 'w') as file:
  #     file.writelines(lines)
  # except FileNotFoundError as e:
  #   print(str(e))

  exit(0)

# function to read files and pad them with 0 fill up to bank size.
def read_file_and_pad(file_name, bank_size_bytes):
  with open(file_name, 'rb') as file:
    temp_buffer = file.read()
    size = file.tell()
    assert size <= bank_size_bytes, f"Failed the check for size of rom is less than or equal to bank size. ROM size is {size} bytes. Max bank size is {bank_size_bytes} bytes. ROM name is {file_name}"
    temp_buffer += bytes(bank_size_bytes-size)
    return temp_buffer

# parse args for tuning build
def parse_args(argv):
  parser = argparse.ArgumentParser(description='Automate rom generation by finding all files with a .col extension. Then output a concatenated file with all files padded to bank size.')

  parser.add_argument('--bank_size',      action='store',       default=32,               dest='bank_size',    required=False, help='Bank size of multicart in Kilobytes(1024, default 32KB')
  parser.add_argument('--roms_path',      action='store',       default="./",             dest='roms_path',    required=False, help='Path to roms to use for generation (all must be .col extension).')
  parser.add_argument('--rom_prime',      action='store',       default="multicart.bin",  dest='rom_prime',    required=False, help='Name of primary rom to locate at the 0th bank')
  parser.add_argument('--rom_name',       action='store',       default="output.bin",     dest='rom_name',     required=False, help='Name of output rom file, include extension you would like to use.')
  parser.add_argument('--max_size',       action='store',       default=512,              dest='max_size',     required=False, help='Max size of total output rom file in Kilobytes(1024, default 512KB).')

  return parser.parse_args()

# name is main is main
if __name__=="__main__":
  main()
