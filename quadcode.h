#ifndef _QUADCODE_H_
#define _QUADCODE_H_

#include <vector>
using namespace std;

class QuadCode
{
public:
    static void CreateQuadCode(int numBits, vector<bool>* bitArray, int coordX, int coordY);
};

#endif // _QUADCODE_H_
