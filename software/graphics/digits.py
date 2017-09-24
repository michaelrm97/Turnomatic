#!/usr/bin/python3 -u
import datetime, glob, sys
from bmp2c import *

if len(sys.argv) > 1:
   filename = sys.argv[1]
else:
   filename = "digits.c"
try:
   dest = open(filename, "w")
except:
   print("Error: Cannot open file: %s", filename)
   sys.exit()

NUM_DIGITS = 10
digits = ["NULL" for i in range(NUM_DIGITS)]

print("""/*
 * digits.c
 *
 *  Created on: %s
 *      Author: Michael
 */
 """ % (datetime.date.today().strftime("%b %d, %Y")), file=dest)

print("#include <chip.h>", file=dest)
print("#include <pm_graphics.h>", file=dest)
print(file=dest)

for filename in glob.glob("digits/*.bmp"):
   if filename == "digits/slash1.bmp":
      continue
   try:
      bmp = open(filename, "rb").read()
   except:
      print("Error: Cannot open file: %s" % filename)
      continue

   img = re.sub(r"^.*[/\\]", "", filename)
   img = re.sub(r"\..*$", "", img)

   num = ord(img[0]) - ord('0')
   digits[num] = "digit_{}".format(img)

   w, h = bmp2c(bmp, 'digit_' + img, dest, True, "DIGIT_ROWS", "DIGIT_WIDTH")
   print(file=dest)

print("const pm_image digits[NUM_DIGITS] = {", file=dest)
print("\t{", end='', file=dest)
print(", DIGIT_WIDTH, DIGIT_HEIGHT},\n\t{".join(digits), file=dest, end='')
print(", DIGIT_WIDTH, DIGIT_HEIGHT}", file=dest)
print("};", file=dest)
print(file=dest)

filenames = ["digits/slash1.bmp"]

for filename in filenames:
   try:
      bmp = open(filename, "rb").read()
      img = re.sub(r"^.*[/\\]", "", filename)
      img = re.sub(r"\..*$", "", img)

      w, h = bmp2c(bmp, '_' + img, dest, True, "DIGIT_ROWS", "DIGIT_WIDTH")
      print(file=dest)
      print("const pm_image %s = {" % img, file=dest)
      print("\t_%s, DIGIT_WIDTH, DIGIT_HEIGHT" % (img), file=dest)
      print("};", file=dest)
   except:
      print("Error: Cannot open file: %s" % filename)
      continue

dest.close()