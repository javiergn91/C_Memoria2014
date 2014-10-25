#ifndef _UTILS_H_
#define _UTILS_H_

#include "common.h"
#include <vector>
#include <string>
using namespace std;

class Utils
{
public:
    static unsigned long long GetDecimalRepresentation(vector<int>* binaryRepresentation);
    static unsigned int GetBytesRequired(unsigned int n);
    static void PrintBinary(unsigned int decimalNumber, bool bNormal = false);
    static unsigned int* CreateBitSequence(string str);
    static void CreateQuadCode(int x, int y, BitmapWrapper* bitmapWrapper, int quadLength);
    static void CreateQuadCode2(int x, int y, BitmapWrapper* bitmapWrapper, int quadLength);
    static void CreateQC_Old(int x, int y, BitmapWrapper* bitmapWrapper, int quadLength);
    static unsigned long QuadCode(int x, int y);
    
    static void PrintLong(unsigned long n);
    
    static char msb(unsigned int v) {
      unsigned int ret=0;
      if(v==0) return 0;
      asm("bsrl %1, %0" : "=r" (ret) : "r" (v));
      return (char)ret+1;
    }
};

#endif // _UTILS_H_
