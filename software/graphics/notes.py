#!/usr/bin/python3 -u
import datetime, glob, sys
from bmp2c import *

if len(sys.argv) > 1:
   filename = sys.argv[1]
else:
   filename = "note_images.c"
try:
   dest = open(filename, "w")
except:
   print("Error: Cannot open file: %s", filename)
   sys.exit()

NUM_NOTES = 7
notes = ["NULL" for i in range(NUM_NOTES)]

print("""/*
 * note_images.c
 *
 *  Created on: %s
 *      Author: Michael
 */
 """ % (datetime.date.today().strftime("%b %d, %Y")), file=dest)

print("#include <chip.h>", file=dest)
print("#include <pm_graphics.h>", file=dest)
print(file=dest)

for filename in glob.glob("notes/?.bmp"):
   try:
      bmp = open(filename, "rb").read()
   except:
      print("Error: Cannot open file: %s" % filename)
      continue

   img = re.sub(r"^.*[/\\]", "", filename)
   img = re.sub(r"\..*$", "", img)

   num = ord(img[0]) - ord('A')
   notes[num] = "note_{}".format(img)

   w, h = bmp2c(bmp, 'note_' + img, dest, True, "NOTE_ROWS", "NOTE_WIDTH")
   print(file=dest)

print("const pm_image note_images[NUM_NOTES] = {", file=dest)
print("\t{", end='', file=dest)
print(", NOTE_WIDTH, NOTE_HEIGHT},\n\t{".join(notes), file=dest, end='')
print(", NOTE_WIDTH, NOTE_HEIGHT}", file=dest)
print("};", file=dest)
print(file=dest)

filenames = ["notes/sharp.bmp", "notes/flat.bmp"]

for filename in filenames:
   try:
      bmp = open(filename, "rb").read()
      img = re.sub(r"^.*[/\\]", "", filename)
      img = re.sub(r"\..*$", "", img)

      w, h = bmp2c(bmp, '_' + img, dest, True, "NOTE_ROWS", "NOTE_WIDTH")
      print(file=dest)
      print("const pm_image %s = {" % img, file=dest)
      print("\t_%s, NOTE_WIDTH, NOTE_HEIGHT" % (img), file=dest)
      print("};", file=dest)
   except:
      print("Error: Cannot open file: %s" % filename)
      continue

dest.close()