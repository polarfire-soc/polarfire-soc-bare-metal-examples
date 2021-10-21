#!/usr/bin/env python3

#==============================================================================
#
# code_cleanup.py
# ivan.griffin@microchip.com
#
# Very simple script to clean up source code
#
#  - will take Unicode punctuation and convert to nearest ASCII equivalent
#  - will replace Microsoft Windows Line endings with Unix Line endings
#  - will remove trailing space
#
#==============================================================================

WindowsLineEnding = '\r\n'
UnixLineEnding = '\n'

from unidecode import unidecode
import sys
import glob

for name in glob.glob("./**/*.[chsS]", recursive=True):
	print("Processing >>" + name + "<<")

	with open(name, "r") as fileIn:
		src = fileIn.read().splitlines()

	with open(name, "w") as fileOut:
		for line in src:
			processed_line = unidecode(line)
			processed_line = processed_line.rstrip()
			processed_line = processed_line.replace(WindowsLineEnding, UnixLineEnding)
			fileOut.write(processed_line + "\n")
