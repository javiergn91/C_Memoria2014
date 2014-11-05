#include "quadcodestruct.h"
#include "common.h"
#include "utils.h"
#include <libcdsBasics.h>
#include <algorithm>
#include <queue>
using namespace std;

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
  ofstream pathNextFile((filenameStr + ".nextpath").c_str());
  ofstream pathLenFile((filenameStr + ".lenpath").c_str());
  
  cout << universeSize << endl;
  cout << quadCodeSize << endl;	
  cout << pathBitmap->len << endl;
  
  pathFile.write((char*)&universeSize, sizeof(int));
  pathFile.write((char*)&quadCodeSize, sizeof(int));
  pathFile.write((char*)&pathBitmap->len, sizeof(int));
  //pathFile.write((char*)pathBitmap->bitmap, uint_len(pathBitmap->len, 1));
  
  int bytes = pathBitmap->len / 64 + 1;//uint_len(pathBitmap->len, 1);
  
  for(int i = 0; i < bytes; i++)
  {
    pathFile.write((char*)&pathBitmap->bitmap[i], sizeof(unsigned long));
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
  
  pathFile.read((char*)&universeSize, sizeof(int));
  pathFile.read((char*)&quadCodeSize, sizeof(int));
  pathFile.read((char*)&pathBitmap->len, sizeof(int));
  
  //cout << "Quadcode size: " << quadCodeSize << endl;
  //cout << "Len: " << pathBitmap->len << endl;
  
  int bytes = pathBitmap->len / 64 + 1;//uint_len(pathBitmap->len, 1);
  pathBitmap->bitmap = new unsigned long[bytes];
  
  pathBitmap->n = pathBitmap->len / WL + 1;
  pathBitmap->lastIdxSize = pathBitmap->len % WL;

  //cout << bytes << endl;
  //pathBitmap->bitmap = new uint[bytes];
  
  //cout << "Bytes: " << bytes << endl;
  
  //pathFile.read((char*)pathBitmap->bitmap, bytes);
  
  for(int i = 0; i < bytes; i++)
  {
    pathFile.read((char*)&pathBitmap->bitmap[i], sizeof(unsigned long));
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

vector< pair<int, int> > QuadCodeStructure::PrintFirstPoints(int n)
{
  vector< pair<int, int> > v;
  
  //long m = 832 | (1 << 1);
  //cout << "m: " << m << endl;
  
  //pathBitmap->PrintBitmap(-1); cout << endl << endl;
  //pathNextBitmap->PrintBitmap(-1); cout << endl << endl;
  //pathLenBitmap->PrintBitmap(-1); cout << endl << endl;
  
  //int currQuad = quadCodeSize;
  
  queue< pair< pair<long, int>, int > > Q;
  Q.push(make_pair( make_pair(0, quadCodeSize), 0));
  
  //cout << "quadCodeSize: " << quadCodeSize << endl;
  
  while(n)
  {
      pair< pair<long, int>, int> curr = Q.front();
      Q.pop();
      int position = curr.second;
      int quadS = curr.first.second;
      long parentCode = curr.first.first;
      
      //cout << "Position: " << position << ", quadS: " << quadS << ", parentCode: " << parentCode << endl;  
      
      unsigned int n1 = 0;
      unsigned int n2 = 0;
      
      long tmpParent = parentCode;
      //cout << "tmpParent: " << tmpParent << endl;
      
      bool a = true;
      if((quadCodeSize - quadS) % 2 != 0)
	a = false;
      
      vector<int> v1, v2;
      while(tmpParent)
      {
	 if(a)
	 {
       	  v1.push_back(tmpParent % 2);
	  tmpParent /= 2;
	 }
	 else
	 {
	  v2.push_back(tmpParent % 2);
	  tmpParent /= 2;
	 }
	 
	 a = !a;
      }
      
      reverse(v1.begin(), v1.end());
      reverse(v2.begin(), v2.end());
      //cout << "v1.size: " << v1.size() << endl;
      for(int i = 0; i < v1.size(); i++)
      {
	  //cout << v1[i];
	  n1 <<= 1;
	  if(v1[i])
	    n1 |= 1;
      }
      //cout << endl;
      //cout << "v2.size: " << v2.size() << endl;
      for(int i = 0; i < v2.size(); i++)
      {
	//cout << v2[i];
	 n2 <<= 1;
	 if(v2[i])
	   n2 |= 1;
      }
      //cout << endl;
      //cout << "QuadS: " << position << " " << quadS << endl;
      for(int i = position; i < position + quadS; i++)
      {
	if(pathNextBitmap->bitSeq->access(i))
	{
	  //cout << i << endl;
	  int numOnes = pathNextBitmap->bitSeq->rank1(i);
	  int nextPos = pathLenBitmap->bitSeq->select1(numOnes) + 1; 
	  
	  //cout << "nextPos: " << nextPos << endl;
	  
	  long newParentCode = parentCode;
	  
	  newParentCode <<= 1;
	  if(!GetPathBitAt(i))
	  {
	    newParentCode |= 1;
	  }
	  //cout << quadS << " " << i << " " << position << " " << quadS - (i - position) - 1 << endl;
	  Q.push(make_pair( make_pair(newParentCode, quadS - (i - position) - 1), nextPos));  
	}
	
	parentCode <<= 1;
	if(GetPathBitAt(i))
	{
	  parentCode |= 1;
	}
      }
      
      
      bool b = true;
      if(quadS % 2 != 0)
	b = false;
      //n1 = n2 = 0;
      int n3 = 0;
      for(int i = position; i < position + quadS; i++)
      {
	//cout << GetPathBitAt(i) << "(" << i << ")";
	
	n3 <<= 1;
	if(GetPathBitAt(i))
	  n3 |= 1;
	
	if(b)
	{
	    n2 <<= 1;
	    if(GetPathBitAt(i))
	      n2 |= 1;
	}
	else
	{
	    n1 <<= 1;
	    if(GetPathBitAt(i))
	      n1 |= 1;	  
	}
	
	b = !b;
      }
      //cout << endl;
      //cout << n2 << " " << n1 << endl;// << n3 << endl;
      v.push_back(make_pair(n2, n1));
      n--;
  }
  
  return v;
}

int QuadCodeStructure::GetPathBitAt(int position)
{
  int idx = position / WL;
  int np  = position % WL;
  
  unsigned long m = pathBitmap->bitmap[idx];
  
  int tmpIdx = pathBitmap->len / WL;
  
  int lastLen = pathBitmap->len - (WL * tmpIdx);
  
  //if(pathBitmap->len % WL != 0)
  //  tmpIdx++;
  
  int offset = WL - np;
  if(idx == tmpIdx)
    offset = lastLen - np;
  
  
  int r = 0;
  while(offset-- > 0) 
  {
     r = m % 2;
     
     //cout << "offset: " << offset << endl;
     
     //cout << r;
     m /= 2;
  }
  //cout << endl;
  
  return r;
  
  //cout << "idx: " << idx << ", np: " << np << endl;
  
  //return (pathBitmap->bitmap[idx] >> (WL - np - 1)) & 1;
}

void QuadCodeStructure::GetPoints(int x1, int y1, int x2, int y2)
{
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
}

bool QuadCodeStructure::CheckPoint(unsigned long bitmap, int len)
{
  /*
  cout << "QUADCODE: ";
  vector<int> v;
  unsigned long tmp = bitmap;
  while(tmp)
  {
      v.push_back(tmp % 2);
      tmp /= 2;
  }
  int l = len - v.size();
  while(l--)
    cout << "0";
  reverse(v.begin(), v.end());
  for(int i = 0; i < v.size(); i++)
    cout << v[i];
  cout << endl << endl;
    */
  int currPos = 0;
  int position;
  
  unsigned long zero = 0;
  int cnt = 0;
  while(true)
  { 
    //cout << "CurrPos: " << currPos << endl;
    position = pathBitmap->XOR(bitmap, currPos, len);
    //cout << "Position: " << position << endl;
    
    cnt++;
    if(position == -1)
    {
      //cout << "C: " << cnt << endl;
      return true;
    
      
    }
    else
    {
	uint bit = (pathNextBitmap->bitSeq->access(position)) ? 1 : 0;
      
	if(bit == 1)
	{
	  int offset = (position + 1 - currPos);
	  len -= offset;
	  bitmap &= ~(~zero << len);
	}
	
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
	  int numOnes = pathNextBitmap->bitSeq->rank1(position);
	  currPos = pathLenBitmap->bitSeq->select1(numOnes) + 1; 
	}
    }
    
  }
  
  return false;
}

bool QuadCodeStructure::CheckBitmap(uint* bitmap, int len, int* pathPos)
{
  int currPos = 0;
 
  while(true)
  { 
    int position = 0;
    
    if(len > W)
    {
      position = pathBitmap->XOR(bitmap[0], currPos, W);
      if(position == -1)
      {
	position = pathBitmap->XOR(bitmap[1], currPos + W, len - W);
      }
    }
    else
    {
      position = pathBitmap->XOR(bitmap[0], currPos, len);
    }
     
    if(position == -1)
    {
      if(pathPos != NULL)
	*pathPos = currPos + len - 1;
    
      return true;
    }
    else
    {
	uint bit = (pathNextBitmap->bitSeq->access(position)) ? 1 : 0;
      
	if(bit == 1)
	{
	  int offset = (position + 1 - currPos);
	    
	  if(offset >= W)
	  {
	      offset -= W;
	      bitmap[0] = bitmap[1];
	  }
	  
	  if(len > W)
	  {  
	    bitmap[0] >>= offset;
	    
	    bitmap[0] = (bitmap[1] & ~(~0 << (offset))) << (W - offset) | bitmap[0];// | (bitmap[0] >> offset);// (W - offset);
	    bitmap[1] >>= offset; 		      
	  }
	  else
	  {
	    *bitmap >>= (position + 1 - currPos);
	  }
	   
	  len -= (position + 1 - currPos);
	}
	
	if(bit == 0)
	{
	  return false;
	}
	else if(bit == 1 && len <= 0)
	{
	  if(pathPos != NULL)
	  {
	    int numOnes = pathNextBitmap->bitSeq->rank1(position);//->Rank(1, position); //cout << "ONES: " << numOnes << endl;
	    *pathPos = pathLenBitmap->bitSeq->select1(numOnes);
	  }
	  return true;
	}
	else if(bit == 1)
	{
	  int numOnes = pathNextBitmap->bitSeq->rank1(position);//->Rank(1, position); //cout << "ONES: " << numOnes << endl;
	  currPos = pathLenBitmap->bitSeq->select1(numOnes) + 1; //Select(1, numOnes) + 1;
	}
    }
  }
  
  return false;
}

int QuadCodeStructure::GetBytes()
{
  int result = 0;
  
  //cout << pathBitmap->len << endl;
  result += pathBitmap->GetSize();
  result += pathNextBitmap->GetSize();
  result += pathLenBitmap->GetSize();
  
  return result;
}
