#ifndef _QUADCODESTRUCT_H_
#define _QUADCODESTRUCT_H_

#include "spbitmap.h"

class QuadCodeStructure
{
public:
    QuadCodeStructure();
  
    void setPathBitmap(SPBitmap* bitmap) { pathBitmap = bitmap; }
    void setPathLenBitmap(SPBitmap* bitmap) { pathLenBitmap = bitmap; }
    void setPathNextBitmap(SPBitmap* bitmap) { pathNextBitmap = bitmap; }
    void setQuadCodeSize(int size) { quadCodeSize = size; }
    
    SPBitmap* getPathBitmap() { return pathBitmap; }
    SPBitmap* getPathLenBitmap() { return pathLenBitmap; }
    SPBitmap* getPathNextBitmap() { return pathNextBitmap; }
    
    int GetBytes();
    
    void PrintBitmaps();
    
    void GetPoints(int x1, int y1, int x2, int y2);
    
    //Check if bitmap is part of the path.
    bool CheckBitmap(uint* bitmap, int len, int* pathPos);
    bool RangeEmptyQuery(Point upperLeftPoint, Point bottomRightPoint);
    
    void PrintPointList();
    
    void Save(const char* filename);
    void Load(const char* filename);
    
    int quadCodeSize;
    int universeSize;
    
    SPBitmap* pathBitmap;
    SPBitmap* pathLenBitmap;
    SPBitmap* pathNextBitmap;
    
private:
    int pointListSize;
    int* pointList[2];
    
    void GetQuad(int pos, int parentCode, int bitsRemaining);
};

#endif