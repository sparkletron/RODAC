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

  rom_file = None

  p = re.compile(r'^([^=]*)( = )(.*)')

  dir_list_all = os.listdir(args.roms_path)

  dir_list_all.sort()

  dir_list_filtered = [os.path.splitext(item)[0].ljust(39)[:39] for item in dir_list_all if item.endswith(".col")]

  if len(dir_list_filtered) > args.max_roms:
    del dir_list_filtered[args.max_roms:]
    print("MAX NUMBER OF ROMS EXCEDED")

  if len(dir_list_filtered) < args.max_roms:
    print("NUMBER OF ROMS LESS THAN MAX")

  dir_string = "{\"" + '", "'.join(dir_list_filtered) + "\"};\n"

  try:
    with open(args.rom_header, 'r') as file:
      lines = file.readlines()
  except FileNotFoundError as e:
    print(str(e))

  for i, line in enumerate(lines):
    temp_list = p.findall(line)
    if temp_list:
      strip_list = list(temp_list[0])
      strip_list[2] = dir_string
      lines[i] = ''.join(strip_list)

  try:
    with open(args.rom_header, 'w') as file:
      file.writelines(lines)
  except FileNotFoundError as e:
    print(str(e))

  exit(0)

# parse args for tuning build
def parse_args(argv):
  parser = argparse.ArgumentParser(description='Automate rom generation by finding all files with a .col extension. Then output a concatenated file with all files padded to bank size.')

  parser.add_argument('--bank_size',      action='store',       default=32,               dest='rom_header',   required=False, help='Bank size of multicart in Kilobytes(1024, default 32KB')
  parser.add_argument('--roms_path',      action='store',       default="./",             dest='roms_path',    required=False, help='Path to roms to use for generation (all must be .col extension).')
  parser.add_argument('--rom_prime',      action='store',       default="multicart.bin",  dest='rom_prime',    required=False, help='Name of primary rom to locate at the 0th bank')
  parser.add_argument('--rom_name',       action='store',       default="output.bin",     dest='rom_name',     required=False, help='Name of output rom file, include extension you would like to use.')
  parser.add_argument('--max_size',       action='store',       default=512,              dest='max_roms',     required=False, help='Max size of total output rom file in Kilobytes(1024, default 512KB).')

  return parser.parse_args()

# name is main is main
if __name__=="__main__":
  main()
