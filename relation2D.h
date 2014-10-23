#ifndef _RELATION2D_H_
#define _RELATION2D_H_

#include "common.h"
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
    void FillTriePointsDefined(Trie* trie);
    void SetLogStream(ofstream* newStream);
    int GetQuadCodeSize() { return numBits; }
    
    void PrintRandomTestCaseCheckPoint(int n);
    void PrintPointList();
    void PreprocessPointListUniqueValue();
    bool IsInRelation(int x, int y);
    vector< Point > GetPointVector() { return points; }
    
    void WriteBinaryFile(const char* filename);
    void ReadBinaryFile(const char* filename);
    
    int getN() { return N; }
    long getNumElements() { return numElements; }
    
    int N;
    int universeSize;
    
private:
    float minDegreeX;
    float minDegreeY;
    float maxDegreeX;
    float maxDegreeY;
    int xArraySize;
    int yArraySize;
    int numBits;
    float xCellSize;
    float yCellSize;
    //int N;
    long numElements;
    vector< pair<float, float> > elementList;
    vector< Point > points;

    ofstream *logStream;
};

#endif // _RELATION2D_H_
