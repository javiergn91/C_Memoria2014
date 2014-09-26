#ifndef _RELATION2D_H_
#define _RELATION2D_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include "trie.h"

using namespace std;

class Relation2D
{
public:
    Relation2D();
    void SetCellSize(float x, float y);
    void AddPair(float element1, float element2);
    void DetermineArrayLimits();
    void FillTrie(Trie* trie);
    void SetLogStream(ofstream* newStream);

private:
    float minDegreeX;
    float minDegreeY;
    float maxDegreeX;
    float maxDegreeY;
    int xArraySize;
    int yArraySize;
    float xCellSize;
    float yCellSize;
    vector< pair<float, float> > elementList;

    ofstream *logStream;
};

#endif // _RELATION2D_H_
