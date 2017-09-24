#!/usr/bin/python3 -u
import datetime, glob, sys
from ascii2c import *

if len(sys.argv) > 1:
   filename = sys.argv[1]
else:
   filename = "font.c"
try:
   dest = open(filename, "w")
except:
   print("Error: Cannot open file: %s", filename)
   sys.exit()

NUM_CHARS = 128
letters = ["NULL" for i in range(NUM_CHARS)]

print("""/*
 * font.c
 *
 *  Created on: %s
 *      Author: Michael
 */
 """ % (datetime.date.today().strftime("%b %d, %Y")), file=dest)

print("#include <chip.h>", file=dest)
print("#include <pm_graphics.h>", file=dest)
print(file=dest)


for filename in glob.glob("font/*"):
   try:
      lines = [x.strip('\n') for x in open(filename, "r").readlines()]
   except:
      print("Error: Cannot open file: %s" % filename)
      continue

   img = re.sub(r"^.*[/\\]", "", filename)
   img = re.sub(r"\..*$", "", img)

   char = lines[0]
   lines = lines[1:]
   
   if len(char) == 1:
      num = ord(char[0])
   else:
      num = int(char)
   if (num >= 0 and num < NUM_CHARS):
      letters[num] = "&{}".format(img)

   ascii2c(lines, img, dest, "CHAR_ROWS", "CHAR_WIDTH")
   print(file=dest)

print("_U08 (*font[NUM_CHARS])[CHAR_ROWS][CHAR_WIDTH] = {", file=dest)
print("\t", end='', file=dest)
print(",\n\t".join(letters), file=dest)
print("};", file=dest)

dest.close()