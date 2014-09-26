#include "quadcode.h"
#include <algorithm>
#include <iostream>
using namespace std;

void QuadCode::CreateQuadCode(int numBits, vector<bool>* bitArray, int coordX, int coordY)
{
    bitArray->clear();

    vector<bool> binaryCoordX, binaryCoordY;
    binaryCoordX.clear();
    binaryCoordY.clear();

    while(coordX)
    {
        binaryCoordX.push_back((coordX % 2 == 0) ? false : true);
        coordX /= 2;
    }

    int bitsRemaining = numBits - binaryCoordX.size();

    while(bitsRemaining--)
    {
        binaryCoordX.push_back(0);
    }

    while(coordY)
    {
        binaryCoordY.push_back((coordY % 2 == 0) ? false : true);
        coordY /= 2;
    }

    bitsRemaining = numBits - binaryCoordY.size();

    while(bitsRemaining--)
    {
        binaryCoordY.push_back(0);
    }

    reverse(binaryCoordX.begin(), binaryCoordX.end());
    reverse(binaryCoordY.begin(), binaryCoordY.end());

    for(int i = 0; i < (int)binaryCoordX.size(); i++)
    {
        bitArray->push_back(binaryCoordY[i]);
        bitArray->push_back(binaryCoordX[i]);
    }
}
