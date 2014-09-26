#ifndef _UTILS_H_
#define _UTILS_H_

#include <vector>
using namespace std;

class Utils
{
public:
    static unsigned long long GetDecimalRepresentation(vector<int>* binaryRepresentation);
    static int GetBytesRequired(int n);
};

#endif // _UTILS_H_
