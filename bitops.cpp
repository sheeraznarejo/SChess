#include "defs.h"
#include <iostream>
using namespace std;

int BitTable[64] = {
  63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
  51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
  26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
  58, 20, 37, 17, 36, 8
};

int PopBit(BITMAP *bb) { // function will return the index of the least significant one bit and pop that 1 bit
  BITMAP b = *bb ^ (*bb - 1);
  unsigned int fold = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
  *bb &= (*bb - 1);
  return BitTable[(fold * 0x783a9b23) >> 26];
}

int CountBits(BITMAP b) {
  int r;
  for(r = 0; b; r++, b &= b - 1);
  return r;
}


void DisplayBitmap(BITMAP a, BITMAP b, ofstream& f)
{
  int i = 64;
  while(i >= 8)
  {
    for(int k =8; k > 0; k--)
    {
      if((BITMAP(1) << i - k) & a) f <<  "X ";
      else f << ". ";
    }
    f << "  ";
    for(int j =8; j > 0; j--)
    {
      if((BITMAP(1) << i - j) & b) f <<  "X ";
      else f << ". ";
    }

    f << endl;
    i -= 8;
  }
  
}

void DisplayBitmap(BITMAP b)
{
  int i = 64;
  while(i >= 8)
  {
    for(int k =8; k > 0; k--)
    {
      if((BITMAP(1) << i - k) & b) cout <<  "X ";
      else cout << ". ";
    }
    cout << endl;
    i -= 8;
  }
}


void DisplayBitmap(BITMAP a, ofstream& f)
{
  int i = 64;
  while(i >= 8)
  {
    for(int k =8; k > 0; k--)
    {
      if((BITMAP(1) << i - k) & a) f <<  "X ";
      else f << ". ";
    }
    f << endl;
    i -= 8;
  }
  
}

