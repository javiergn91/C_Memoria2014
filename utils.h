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
};

#endif // _UTILS_H_
