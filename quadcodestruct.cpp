#include "quadcodestruct.h"
#include "common.h"
#include "utils.h"
#include <libcdsBasics.h>

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
  pathBitmap->PrintBitmap(-1); cout << endl << endl;
  pathNextBitmap->PrintBitmap(-1); cout << endl << endl;
  pathLenBitmap->PrintBitmap(-1); cout << endl << endl;
}

void QuadCodeStructure::Save(const char* filename)
{
  string filenameStr = filename;
  ofstream pathFile((filenameStr + ".path").c_str(), ios::binary);
  ofstream pathNextFile((filenameStr + ".nextpath").c_str());
  ofstream pathLenFile((filenameStr + ".lenpath").c_str());
  
  cout << quadCodeSize << endl;	
  cout << pathBitmap->len << endl;
  
  pathFile.write((char*)&quadCodeSize, sizeof(int));
  pathFile.write((char*)&pathBitmap->len, sizeof(int));
  //pathFile.write((char*)pathBitmap->bitmap, uint_len(pathBitmap->len, 1));
  
  int bytes = uint_len(pathBitmap->len, 1);
  
  for(int i = 0; i < bytes; i++)
  {
    pathFile.write((char*)&pathBitmap->bitmap[i], sizeof(uint));
    //cout << (uint)pathBitmap->bitmap[i] << endl;
  }
  
  pathNextBitmap->bitSeq->save(pathNextFile);
  pathLenBitmap->bitSeq->save(pathLenFile);
  
  pathFile.close();
  pathNextFile.close();
  pathLenFile.close();
}

void QuadCodeStructure::Load(const char* filename)
{ 
  string filenameStr = filename;
  ifstream pathFile((filenameStr + ".path").c_str(), ios::binary);
  ifstream pathNextFile((filenameStr + ".nextpath").c_str());
  ifstream pathLenFile((filenameStr + ".lenpath").c_str());
  
  pathBitmap = new SPBitmap();
  pathNextBitmap = new SPBitmap();
  pathLenBitmap = new SPBitmap();
 
  pathFile.read((char*)&quadCodeSize, sizeof(int));
  pathFile.read((char*)&pathBitmap->len, sizeof(int));
  //cout << "Quadcode size: " << quadCodeSize << endl;
  //cout << "Len: " << pathBitmap->len << endl;
  
  int bytes = uint_len(pathBitmap->len, 1);
  pathBitmap->bitmap = new uint[bytes];
  
  //cout << "Bytes: " << bytes << endl;
  
  //pathFile.read((char*)pathBitmap->bitmap, bytes);
  
  for(int i = 0; i < bytes; i++)
  {
    pathFile.read((char*)&pathBitmap->bitmap[i], sizeof(int));
    //cout << (uint)pathBitmap->bitmap[i] << endl;
  }
  

  pathNextBitmap->bitSeq = BitSequence::load(pathNextFile);
  pathLenBitmap->bitSeq = BitSequence::load(pathLenFile);
  
  pathFile.close();
  pathNextFile.close();
  pathLenFile.close();  
}

bool QuadCodeStructure::RangeEmptyQuery(Point upperLeftPoint, Point bottomRightPoint)
{
  BitmapWrapper P1, P2;
  Utils::CreateQuadCode(upperLeftPoint.x, upperLeftPoint.y, &P1, quadCodeSize);
  Utils::CreateQuadCode(bottomRightPoint.x, bottomRightPoint.y, &P2, quadCodeSize);
  
  uint wordXOR = P1.bitmap[0] ^ P2.bitmap[0];
  //cout << "wordXOR: " << wordXOR << endl;
  wordXOR &= wordXOR & ~(wordXOR - 1);
 
  int nBits = bits(wordXOR) - 1;
  
  if(nBits == 0 && P1.bitmap[0] != P2.bitmap[0])
  {
    //cout << "dsada" << endl;
    return pathBitmap->GetLen() > 0;
  }
  
  if(wordXOR == 0)
  {
    //for(int i = 0; i < quadCodeSize; i++) cout << bitget(P1.bitmap, i); cout << endl;
    return CheckBitmap(P1.bitmap, quadCodeSize, NULL);
  }
  
  
  uint word = P1.bitmap[0] & ~(~0 << (nBits));
  
  /*
  cout << endl;
  for(int i = 0; i < quadCodeSize; i++) cout << bitget(P1.bitmap, i); cout << endl;
  for(int i = 0; i < quadCodeSize; i++) cout << bitget(P2.bitmap, i); cout << endl;
  for(int i = 0; i < quadCodeSize; i++) cout << bitget(&wordXOR, i); cout << endl;
  for(int i = 0; i < nBits; i++) cout << bitget(&word, i); cout << endl;
  */
  return quadCodeSize - nBits > 0 && CheckBitmap(&word, nBits, NULL);
}

void QuadCodeStructure::GetQuad(int pos, int parentCode, int bitsRemaining)
{
  int x = 0;
  int y = 0;
  
  int mult = 0;
  bool bY = true;
  bool bAddMult = false;
  
  int currParentCode = parentCode;
  //cout << "bits: " << bitsRemaining << endl;
  for(int i = bitsRemaining + pos - 1; i >= pos; i--)
  { 
    int j = pos + (bitsRemaining + pos - 1) - i;;
    
    //cout << j << endl;
    parentCode <<= 1;
    if(pathBitmap->GetBitAt(j))
    {
	parentCode |= 1;
    }
    
    if(pathNextBitmap->GetBitAt(j))
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
      int newPos = pathLenBitmap->Select(1, pathNextBitmap->Rank(1, j)) + 1; 
      
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
  BitmapWrapper quadcode1, quadcode2;
  
  int numPoints = pathNextBitmap->Rank(1, pathNextBitmap->GetLen() - 1) + 1;
  
  pointList[0] = new int[numPoints];
  pointList[1] = new int[numPoints];
  pointListSize = 0;
  
  Utils::CreateQuadCode(x1, y1, &quadcode1, quadCodeSize);
  Utils::CreateQuadCode(x2, y2, &quadcode2, quadCodeSize);
  //cout << numPoints << endl;
  
  GetQuad(0, 0, quadCodeSize);
}

bool QuadCodeStructure::CheckBitmap(uint* bitmap, int len, int* pathPos)
{
  int currPos = 0;
  //int lastPosition = 0;
  
  //cout << endl;
  while(true)
  {
    int position = pathBitmap->XOR(bitmap, currPos, len);
    
    //cout << "Position: " << position << endl;
    
    if(position == -1)
    {
      if(pathPos != NULL)
	*pathPos = currPos + len - 1;
      //cout << "Path pos: " << *pathPos << endl;
      //cout << "true" << endl;
      return true;
    }
    else
    {
	//uint bit = pathNextBitmap->GetBitAt(position);
	uint bit = (pathNextBitmap->bitSeq->access(position)) ? 1 : 0;
      
	if(bit == 1)
	{
	  len -= (position + 1 - currPos);
	  *bitmap >>= (position + 1 - currPos);
	  //lastPosition = position + 1;
	}
	
	//cout << "Length: " << len << endl;
	
	//cout << "Word0: ";
	/*
	for(int i = 0; i < len; i++)
	{
	  cout << bitget(bitmap, i);
	}
	cout << endl;
	*/
	
	if(bit == 0)
	{
	  return false;
	}
	else if(bit == 1 && len <= 0)
	{
	  return true;
	}
	else if(bit == 1)
	{
	  //int numOnes = pathNextBitmap->Rank(1, position); //cout << "ONES: " << numOnes << endl;
	  //currPos = pathLenBitmap->Select(1, numOnes) + 1;
	  
	  int numOnes = pathNextBitmap->bitSeq->rank1(position);//->Rank(1, position); //cout << "ONES: " << numOnes << endl;
	  currPos = pathLenBitmap->bitSeq->select1(numOnes) + 1; //Select(1, numOnes) + 1;
	  
	  //cout << "currPos: " << currPos << endl;
	}
    }
  }
  
  return false;
}

int QuadCodeStructure::GetBytes()
{
  int result = 0;
  
  result += pathBitmap->GetSize();
  result += pathNextBitmap->GetSize();
  result += pathLenBitmap->GetSize();
  
  return result;
}