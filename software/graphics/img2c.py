#!/usr/bin/python3 -u 
import sys, math

def pixels_to_char(arr, x, y1, y2):
   ret = 0
   for i in range(y2, y1 - 1, -1):
      ret = 2 * ret + arr[i][x]

   return ret

def bytes_to_int (arr, index):
   ret = arr[index] + arr[index + 1] * 256 + arr[index + 2] * 256 * 256 + arr[index + 3] * 256 * 256 * 256
   if arr[index + 3] & 0x80:
      ret -= 2 ** 32
   return ret

def printArray(image, img, height, width, dest = sys.stdout, 
   h = None, w = None):
   if not h:
      h = math.ceil(height / 8)
   if not w:
      w = width
   print("_U08 %s[%s][%s] = {" % (img, h, w), file=dest)
   for row in range(math.ceil(height / 8)):
      print("\t{", end='', file=dest)
      for col in range(width):
         # Get bytes
         print(pixels_to_char(image, col, row * 8, min(height - 1, row * 8 + 7)), end='', file=dest)
         if not (col == width - 1):
            print(', ', end='', file=dest)
         else:
            if not (row == math.ceil(height / 8) - 1):
               print('},', file=dest)
            else:
               print('}', file=dest)
   print("};", file=dest)

def printComment(image, height, width, dest = sys.stdout):
   print("/*", file=dest)
   for i in range(height):
      for j in range(width):
         if image[i][j] == 1:
            print("*", end='', file=dest)
         else:
            print(" ", end='', file=dest)
      print(file=dest)
   print("*/", file=dest)
