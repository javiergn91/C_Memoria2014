#include "quadcodestruct.h"
#include "common.h"
#include "utils.h"
#include <libcdsBasics.h>
#include <algorithm>
#include <stdlib.h>

using namespace cds_utils;

QuadCodeStructure::QuadCodeStructure()
{
    pathBitmap = NULL;
    pathLenBitmap = NULL;
    pathNextBitmap = NULL;
    
    pointList[0] = NULL;
    pointList[1] = NULL;
}

void QuadCodeStructure::PrintBitmaps()
{
  //pathBitmap->PrintBitmap(-1); cout << endl << endl;
  //pathNextBitmap->PrintBitmap(-1); cout << endl << endl;
  //pathLenBitmap->PrintBitmap(-1); cout << endl << endl;
}

void QuadCodeStructure::Save(const char* filename)
{
  string filenameStr = filename;
  ofstream pathFile((filenameStr + ".path").c_str(), ios::binary);
  ofstream pathLenFile((filenameStr + ".lenpath").c_str(), ios::binary);
  
  //cout << "Data: ";
  //cout << universeSize << " " << quadCodeSize << " " << pathVec->len << endl;
  
  pathFile.write((char*)&universeSize, sizeof(int));
  pathFile.write((char*)&quadCodeSize, sizeof(int));
  pathFile.write((char*)&pathVec->len, sizeof(int));
  
  int bytes = pathVec->len / 64 + 1;
  
  for(int i = 0; i < bytes; i++)
  {
    pathFile.write((char*)&pathVec->bitmap[i], sizeof(unsigned long));
  }
  
  for(int i = 0; i <= quadCodeSize; i++)
  {
    //cout << lenVec[i] << endl;
    pathLenFile.write((char*)&lenVec[i], sizeof(int));
  }
  
  for(int i = 0; i < quadCodeSize; i++)
  {
    stringstream ss;
    ss << i;
    string str = ss.str();
    
    ofstream pathNextFile((filenameStr + ".nextpath." + str).c_str());
    /*
    cout << "L" << i << ": ";
    for(int j = 0; j < bitSequence[i]->getLength(); j++)
    {
      if(bitSequence[i]->access(j))
	cout << "1";
      else
	cout << "0";
    }
    cout << endl;
    */
    bitSequence[i]->save(pathNextFile);
    pathNextFile.close();
  }
  
  pathFile.close();
  pathLenFile.close();
}

void QuadCodeStructure::Load(const char* filename)
{ 
  string filenameStr = filename;
  ifstream pathFile((filenameStr + ".path").c_str(), ios::binary);
  ifstream pathLenFile((filenameStr + ".lenpath").c_str(), ios::binary);
  
  pathVec = new SPBitmap();

  pathFile.read((char*)&universeSize, sizeof(int));
  pathFile.read((char*)&quadCodeSize, sizeof(int));
  pathFile.read((char*)&pathVec->len, sizeof(int));

  //cout << "Data: ";
  //cout << universeSize << " " << quadCodeSize << " " << pathVec->len << endl;
  
  
  int bytes = pathVec->len / 64 + 1;
  pathVec->bitmap = new unsigned long[bytes];
  
  //cout << bytes << endl;
  
  pathVec->n = pathVec->len / WL + 1;
  pathVec->lastIdxSize = pathVec->len % WL;

  for(int i = 0; i < bytes; i++)
  {
    pathFile.read((char*)&pathVec->bitmap[i], sizeof(unsigned long));
  }
  
  lenVec = new int[quadCodeSize + 1];
  for(int i = 0; i <= quadCodeSize; i++)
  {
    pathLenFile.read((char*)&lenVec[i], sizeof(int));
    //cout << lenVec[i] << endl;
  }
  
  bitSequence = new BitSequence*[quadCodeSize];
  for(int i = 0; i < quadCodeSize; i++)
  {
    stringstream ss;
    ss << i;
    string str = ss.str();
    
    ifstream pathNextFile((filenameStr + ".nextpath." + str).c_str(), ios::binary);
    bitSequence[i] = BitSequence::load(pathNextFile);
    pathNextFile.close();
  }
  
  pathFile.close();
  pathLenFile.close();  
}

bool QuadCodeStructure::RangeEmptyQuery(Point upperLeftPoint, Point bottomRightPoint)
{
  return false;
}

void QuadCodeStructure::GetQuad(int pos, int parentCode, int bitsRemaining)
{
  int x = 0;
  int y = 0;
  
  int mult = 0;
  bool bY = true;
  bool bAddMult = false;
  
  int currParentCode = parentCode;
  //cout << "bits: " << bitsRemaining << ", pos: " << pos << endl;
  for(int i = bitsRemaining + pos - 1; i >= pos; i--)
  { 
    int j = pos + (bitsRemaining + pos - 1) - i;;
    
    //cout << j << endl;
    parentCode <<= 1;
    if(pathBitmap->GetBitAt(j))
    {
	parentCode |= 1;
    }
    
    if(pathNextBitmap->bitSeq->access(j))
    {
      int lastBit = parentCode & 1;
      int newParentCode = parentCode;
      if(lastBit)
      {
	newParentCode = newParentCode & (~0 << 1);
      }
      else
      {
	newParentCode |= 1;
      }
      //int newPos = pathLenBitmap->Select(1, pathNextBitmap->Rank(1, j)) + 1; 
      int newPos = pathLenBitmap->bitSeq->select1(pathNextBitmap->bitSeq->rank1(j)) + 1;
      //cout << "newPos: " << newPos << endl;
      //cout << "D: " << newPos << " " << bitsRemaining - j - 1 + pos << endl;
      GetQuad(newPos, newParentCode, bitsRemaining - j - 1 + pos);
    }
    //cout << endl;
    
    if(pathBitmap->GetBitAt(i))
    {
      if(bY)
      {
	y += (1 << mult);
      }
      else
      {
	x += (1 << mult);
      }
    }
     
    if(bAddMult)
      mult++;
    
    bY = !bY;
    bAddMult = !bAddMult;
  }    
  
  //cout << endl;
  while(currParentCode)
  {
    if(currParentCode & 1)
    {	
      if(bY)
      {
	y += (1 << mult);
      }
      else
      {
	x += (1 << mult);
      }
    }
    
    if(bAddMult)
      mult++;
    bAddMult = !bAddMult;
    
    bY = !bY;
    
    currParentCode >>= 1;
  }
  
  //cout << pointListSize << endl;
  pointList[0][pointListSize] = x;
  pointList[1][pointListSize] = y;
  pointListSize++;
  //cout << x << " " << y << endl;
}

void QuadCodeStructure::PrintPointList()
{
  for(int i = 0; i < pointListSize; i++)
  {
    cout << "(" << pointList[0][i] << ", " << pointList[1][i] << ")" << endl;
  }
}

void QuadCodeStructure::GetPoints(int x1, int y1, int x2, int y2)
{
  /*
  if(x1 == x2 && y1 == y2)
  {
    BitmapWrapper bw;
    Utils::CreateQuadCode(x1, y1, &bw, quadCodeSize);
    if(CheckBitmap(bw.bitmap, quadCodeSize, NULL))
    {
      pointList[0] = new int[1];
      pointList[1] = new int[1];
      pointList[0][0] = x1;
      pointList[1][0] = y1;
      pointListSize = 1;
    }
    
    return;
  }
  
  BitmapWrapper quadcode1, quadcode2;
  
  int numPoints = pathNextBitmap->bitSeq->countOnes() - 1;
  
  //cout << "NumPoints: " << numPoints << endl;
  
  pointList[0] = new int[numPoints];
  pointList[1] = new int[numPoints];
  pointListSize = 0;
  
  Utils::CreateQuadCode(x1, y1, &quadcode1, quadCodeSize);
  Utils::CreateQuadCode(x2, y2, &quadcode2, quadCodeSize);
  //cout << numPoints << endl;
  //pathBitmap->PrintBitmap(-1); cout << endl;
  
  
  //for(int i = 0; i < quadCodeSize; i++) cout << bitget(quadcode1.bitmap, i); cout << endl;
  //for(int i = 0; i < quadCodeSize; i++) cout << bitget(quadcode2.bitmap, i); cout << endl;
  
  quadcode1.bitmap[0] = quadcode1.bitmap[0] ^ quadcode2.bitmap[0];
  quadcode1.bitmap[0] &= ~(quadcode1.bitmap[0] - 1);
  
  int failAt = bits(quadcode1.bitmap[0]) - 1;
  
  quadcode1.bitmap[0] = quadcode2.bitmap[0] & ~(~0 << failAt);
  
  //cout << "failtAt = " << failAt << endl;
  
  //for(int i = 0; i < quadCodeSize; i++) cout << bitget(quadcode1.bitmap, i); cout << endl;
  
  uint parentCode = 0;
  for(int j = 0, i = failAt - 1; j < failAt; j++, i--)
  {
    //cout << "Bit: " << bitget(quadcode1.bitmap, j) << " " << i << endl;
    parentCode += (bitget(quadcode1.bitmap, j)) ? (1 << i) : 0;
  }
  
  int p = 0;
  
  if(!CheckBitmap(quadcode1.bitmap, failAt, &p))
  {
      return;
  }
  
  //cout << "bitcode: " << quadcode1.bitmap[0] << endl;
  //cout << "p: " << p << endl;
  

  //cout << "Parent Code: " << parentCode << endl;
  GetQuad(p + 1, parentCode, quadCodeSize - failAt);
  */
}

bool QuadCodeStructure::CheckPoint(unsigned long bitmap, int len)
{
  //Utils::PrintLong(bitmap); cout << endl;
  
  int currPos = 1;
  int currH = 0;
  int height = len - 1; //not always true. 
  //cout << "H: " << height << endl;
  
  //lenVec[7] = 221;
  while(1)
  {
      
    
      int position = pathVec->XOR(bitmap, currPos, len);
      //cout << "Position: " << position << ", currPos: " << currPos << " Quad: " << len << endl;
      
      if(position == -1)
	return true;
      
      int offset = position - currPos;
      int lastLen = len;
      
      //cout << "offset: " << offset << endl;
      //cout << "====" << endl;
      //Utils::PrintLong(bitmap); cout << endl;
      len -= offset;
      bitmap &= ~(~0 << len);
      //Utils::PrintLong(bitmap); cout << endl;
      //cout << "====" << endl;
      //cout << "len: " << offset << endl;
      
      //bitmap &= ~(~0 << len);
      
      //bitmap <<= offset;
      //bitmap >>= offset;
      /*
      cout << "lenVec[currH]: " << lenVec[currH] << endl;
      
      int L = lenVec[currH + 1] - lenVec[currH];
      cout << "Len: " << L << endl;
      
      int L2 = L - (position - 1 - lenVec[currH]);
      L -= L2;
      */
      
      int L = (height - 1) - (lastLen - (position - currPos) - 1) + 1;
      
      if(currH == 0)
	L = position - 1;
      
      //cout << "L: " << L << endl;
      
   //   if(currH != 0)
	//L++;
      
      //cout << "currH:"  << currH << endl;
      
      //cout << "L: " << L << endl;
      int bit = bitSequence[L]->access(currH);
      
      
      //cout << "bit: " << bit << endl;
      
      if(bit == 0)
	return false;
      
      //cout << "asda#" << endl;
      int rank = bitSequence[L]->rank1(currH);
      //cout << "RANK: " << rank << endl;
      
      //currH++;
      
      //currH = L;
      
      //cout << "len: " << lenVec[L + 1] << ", rank: " << (rank - 1) * len << endl;
      currPos = lenVec[L + 1] + (rank - 1) * len;// + (lenVec[L + 2] - lenVec[L + 1] - 1) * (rank - 1);
      
      currH = bitSequence[L]->getLength() + rank - 1;
      
      //currH = L + rank;
      //cout << "currPos: " << currPos << endl;
      //return false;
      //cout << "========" << endl;
      //return false;
  }
  
  return false;
}

int QuadCodeStructure::GetBytes()
{
  int result = 0;
  
  //cout << pathBitmap->len << endl;
  result += pathVec->GetSize();
  result += quadCodeSize * sizeof(int);
  
  for(int i = 0; i < quadCodeSize; i++)
  {
      result += bitSequence[i]->getSize();
  }
  
  return result;
}
