#ifndef _UTILS_H_
#define _UTILS_H_

#include <vector>
using namespace std;

class Utils
{
public:
    static unsigned long long GetDecimalRepresentation(vector<int>* binaryRepresentation);
    static unsigned int GetBytesRequired(unsigned int n);
    static void PrintBinary(unsigned int decimalNumber, bool bNormal = false);
};

#endif // _UTILS_H_
