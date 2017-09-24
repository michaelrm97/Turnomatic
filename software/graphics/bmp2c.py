#!/usr/bin/python3 -u 
import sys, re, math
from img2c import *

def bmp2c(bmp, img, dest = sys.stdout, invert = False, h = None, w = None):

   # Assume that the BITMAPINFOHEADER is used
   offset = bytes_to_int(bmp, 10)

   width = bytes_to_int(bmp, 18)
   height = abs(bytes_to_int(bmp, 22))

   # Form 2D array of image
   i = offset
   image = [[0 for x in range(width)] for y in range(height)]

   x = 0
   y = 0

   padding = (4 - math.ceil(width / 8) % 4) % 4

   while i < len(bmp):
      for j in range(8):
         if ((bmp[i] & (0x80 >> j)) > 0):
            if invert:
               image[y][x] = 0
            else:
               image[y][x] = 1
         else:
            if invert:
               image[y][x] = 1
            else:
               image[y][x] = 0
         x += 1
         if x >= width:
            break

      i += 1

      if x >= width:
         i += padding
         x = 0
         y += 1
      
   printComment(image, height, width, dest)
   print(file=dest)
   printArray(image, img, height, width, dest, h, w)

   return width, height

def main():
   invert = False

   # Default is that white is 1 and black is 0
   # Use --inverted to reverse this
   if "--inverted" in sys.argv:
      sys.argv.remove("--inverted")
      invert = True

   if len(sys.argv) < 2:
      print("Usage: bmp2c.py <source> [destination]")
      sys.exit()

   source = sys.argv[1]
   try:
      destination = sys.argv[2]
      img = re.sub(r"^.*[/\\]", "", destination)
      img = re.sub(r"\..*$", "", img)
   except:
      destination = sys.stdout
      img = re.sub(r"^.*[/\\]", "", source)
      img = re.sub(r"\..*$", "", img)

   try:
      bmp = open(source, "rb").read()
   except:
      print("Error: Cannot open file: %s" % source)
      sys.exit()

   if destination != sys.stdout:
      try:
         dest = open(destination, "w")
      except:
         print("Error: Cannot open file: %s" % destination)
         sys.exit()
   else:
      dest = destination

   print("#include <hw/pmoleds/pm_graphics.h>", file=dest)
   print(file=dest)

   w, h = bmp2c(bmp, '_' + img, dest, invert)
   print(file=dest)
   print("const pm_image %s = {" % img, file=dest)
   print("\t_%s, %d, %d" % (img, w, h), file=dest)
   print("};", file=dest)

   dest.close()

if __name__ == "__main__":
   main()