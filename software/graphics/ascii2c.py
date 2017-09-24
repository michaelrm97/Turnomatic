#!/usr/bin/python3 -u 
import sys, re, math
from img2c import *

def ascii2c(lines, img, dest = sys.stdout, h = None, w = None):
   width = max([len(line) for line in lines])
   height = len(lines)

   image = [[0 for x in range(width)] for y in range(height)]
   
   y = 0
   for line in lines:
      x = 0
      for c in line:
         if c != ' ':
            image[y][x] = 1
         else:
            image[y][x] = 0
         x += 1
      y += 1

   printComment(image, height, width, dest)
   printArray(image, img, height, width, dest, h, w)

   return width, height

def main():
   letter = False
   if "--letter" in sys.argv:
      letter = True
      sys.argv.remove("--letter")

   if len(sys.argv) < 2:
      print("Usage: ascii2c.py <source> [destination]")
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
      lines = [x.strip('\n') for x in open(source, "r").readlines()]
   except:
      print("Error: Cannot open file: %s" % source)
      sys.exit()

   if letter:
      lines = lines[1:]

   if destination != sys.stdout:
      try:
         dest = open(destination, "w")
      except:
         print("Error: Cannot open file: %s" % destination)
         sys.exit()
   else:
      dest = destination

   print("#include <pm_graphics.h>", file=dest)
   print(file=dest)

   w, h = ascii2c(lines, '_' + img, dest)
   print(file=dest)
   print("const pm_image %s = {" % img, file=dest)
   print("\t_%s, %d, %d" % (img, w, h), file=dest)
   print("};", file=dest)

   dest.close()

if __name__ == "__main__":
   main()