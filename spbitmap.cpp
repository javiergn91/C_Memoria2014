//#include <libcdsBasics.h>
#include "spbitmap.h"
#include <iostream>
#include "math.h"
#include "utils.h"
#include <algorithm>

using namespace std;
using namespace cds_utils;
using namespace cds_static;

SPBitmap::SPBitmap(unsigned int* bitmap, int len, BITSEQ seq) 
{
  /*
  this->bitmap = bitmap;
  this->len = len;
  
  //cout << len << endl;
  
  if(seq == BITSEQ_NONE)
  {
    bitSeq = NULL;
  }
  else if(seq == BITSEQ_RRR)
  {
    bitSeq = new BitSequenceRRR(bitmap, len);
  }
  else if(seq == BITSEQ_SARRAY)
  {
    bitSeq = new BitSequenceSDArray(bitmap, len);
  }
  */
}

SPBitmap::SPBitmap()
{
  bitSeq = NULL;
}

int SPBitmap::NumberOfOnes()
{
  if(bitSeq == NULL)
    return 0;
  
  return Rank(1, len - 1);
}

int SPBitmap::NumberOfZeros()
{
  if(bitSeq == NULL)
    return 0;
  
  return Rank(0, len - 1);
}

int SPBitmap::Select(int bit, int ocurrence)
{
  if(bitSeq == NULL)
  {
    return -1;
  }
  
  if(bit == 1)
  {
    //cout << "select1(" << ocurrence << ") = " << bsrrr->select1(ocurrence) << endl;
    
    return bitSeq->select1(ocurrence);
  }
  else if(bit == 0)
  {
    //cout << "select0(" << ocurrence << ") = " << bsrrr->select0(ocurrence) << endl;
    
    return bitSeq->select0(ocurrence);
  }
  
  return 0;
}

int SPBitmap::Rank(int bit, int position)
{
  if(bitSeq == NULL)
  {
    return -1;
  }
  
  if(bit == 1)
  {
    //cout << "rank1(" << position << ") = " << bsrrr->rank1(position) << endl;
    
    return bitSeq->rank1(position);
  }
  else if(bit == 0)
  {
    //cout << "rank0(" << position << ") = " << bsrrr->rank0(position) << endl;
    
    return bitSeq->rank0(position);
  }
  
  return 0; 
}
/*
int SPBitmap::XOR(uint* op, int initBitPos, int queryLen)
{
    int initIndex = initBitPos / W;
    int finalIndex = (initBitPos + queryLen) / W;
    
    uint result = 0;
    
    if(initIndex == finalIndex) 
    {
      uint word = bitmap[initIndex];
      uint wordOP = *op;
      
      wordOP = (wordOP & ~(~0 << queryLen));
      word = ((word >> (initBitPos % W)) & ~(~0 << queryLen));
      result = word ^ wordOP;
    }
    else
    {
      //cout << "Second case" << endl;
      
      bool bDebug = false;
      
      uint word1 = bitmap[initIndex];
      uint word2 = bitmap[finalIndex];
      
      if(bDebug)
      {
	cout << "Word1: "; for(int i = 0; i < W; i++) cout << bitget(&word1, i); cout << endl; 
	cout << "Word2: "; for(int i = 0; i < W; i++) cout << bitget(&word2, i); cout << endl;
      }
      
      int word1Size = W - initBitPos % W;
      
      word1 = (word1 >> (initBitPos % W)) & ~(~0 << word1Size);
      
      int word2Size = queryLen - word1Size;
      
      //cout << "word1Size: " << W - word2Size << endl;
      
      //word2 &= ~(~0 << (W - word2Size));
      word2 &= ~(~0 << (word2Size));
      //word2 >>= (len - word1Size);
      
      if(bDebug)
      {
	cout << "===" << endl;
	cout << "Word1: "; for(int i = 0; i < W; i++) cout << bitget(&word1, i); cout << endl; 
	cout << "Word2: "; for(int i = 0; i < W; i++) cout << bitget(&word2, i); cout << endl;
      }
      
      //cout << word2Size << endl;
      //word1 = (word1 << word1Size) | word2;
      word2 <<= word1Size;
      
      word1 |= word2;
      
      if(bDebug)
      {
	cout << "===" << endl;
	cout << "Word1: "; for(int i = 0; i < W; i++) cout << bitget(&word1, i); cout << endl; 
	//cout << "Word2: "; for(int i = 0; i < W; i++) cout << bitget(&word2, i); cout << endl;
      }
      
      uint wordOP = *op;
      wordOP = (wordOP & ~(~0 << queryLen));
      
      if(bDebug)
      {
	//cout << "===" << endl;
	cout << "Op: "; for(int i = 0; i < W; i++) cout << bitget(op, i); cout << endl;
      }
      result = word1 ^ wordOP;
    }
    
    if(result == 0)
    {
      return -1;
      //cout << "OK!" << endl;
    }
    else
    {
      unsigned int failPosition = (result & ~(result - 1)); 
      
      //delete this.
      //initBitPos = 0;
      
      return bits(failPosition) + initBitPos - 1;
      //return floor(log2(failPosition)) + initBitPos;
    }
    
    return 0;
}
*/

int SPBitmap::XOR(unsigned long op, int initBitPos, int queryLen)
{
   //   int n = len / WL + 1;
  //int lastIdxSize = len % WL;
  
    int initIndex = initBitPos / WL;
    int finalIndex = (initBitPos + queryLen - 1) / WL;
    unsigned long result = -1; 
    
    unsigned long one = 1;
    unsigned long zero = 0;    
    
    if(initIndex == finalIndex) 
    {
      unsigned long word = bitmap[initIndex];
     
      
      //cout << "n: " << n << ", lastIdxSize: " << lastIdxSize << endl;
      //cout << "finalIndex: " << finalIndex << endl;
      //Utils::PrintLong(word);
      //Utils::PrintLong(op);
      unsigned long wordOP = op;
       
      int l = initBitPos % WL;
    
      int s = (finalIndex == n - 1) ? lastIdxSize : WL;
      if(l != 0)
      {
	word &= ~(~zero << (s - l));
      }
      
      word >>= (s - queryLen - l);
      word |= one << (queryLen);
      wordOP |= one << (queryLen);
      
      result = word ^ wordOP;
    }
    else
    {
      unsigned long word1 = bitmap[initIndex];
      unsigned long word2 = bitmap[finalIndex];
      
      int l = initBitPos % WL;
      int w1 = WL - l;
      int w2 = queryLen - w1;
      
      if(initBitPos != 0)
	word1 &= ~(~zero << w1);

      if(w2 != WL)
      {
	int s = (finalIndex == n - 1) ? lastIdxSize : WL;
	word2 &= (~zero);
	word2 >>= (s - w2);
      }
      
      word1 = word1 << w2 | word2;	
      
      word1 |= one << (queryLen);
      op |= one << (queryLen);
      
      result = word1 ^ op;
    }
    
    if(result == 0)
    {
      return -1;
    }
    else
    {
      unsigned int firstPart = result >> 32;
      unsigned int secondPart = (result << 32) >> 32; 
      
      if(firstPart == 0)
	return queryLen - Utils::msb(secondPart) + initBitPos;
      else
	return queryLen - (Utils::msb(firstPart) + 32) + initBitPos;
    }
    
    return 0;
}

int SPBitmap::XOR(uint* op, int initBitPos, int queryLen)
{
    int initIndex = initBitPos / W;
    int finalIndex = (initBitPos + queryLen) / W;
    /*
    cout << queryLen << endl;
    for(int i = 0; i < queryLen; i++)
    {
	cout << bitget(op, i);
    }
    cout << endl;
    */
    
    unsigned long result = 0; 
    
    if(initIndex == finalIndex) 
    {
      uint word = bitmap[initIndex];
      uint wordOP = *op;
      /*
      for(int i = 0; i < W; i++)
      {
	cout << bitget(&word, i);
      }
      cout << endl;
      
      for(int i = 0; i < queryLen; i++)
      {
	cout << bitget(&wordOP, i);
      }
      cout << endl;
      */
      
      wordOP = (wordOP & ~(~0 << queryLen));
      word = ((word >> (initBitPos % W)) & ~(~0 << queryLen));
      result = word ^ wordOP;
    }
    else
    {
      cout << "Second case" << endl;
      
      bool bDebug = false;
      
      unsigned long word1 = bitmap[initIndex];
      unsigned long word2 = bitmap[finalIndex];
      
      if(bDebug)
      {
	cout << "Word1: "; for(int i = 0; i < W; i++) cout << bitget(&word1, i); cout << endl; 
	cout << "Word2: "; for(int i = 0; i < W; i++) cout << bitget(&word2, i); cout << endl;
      }
      
      int word1Size = W - initBitPos % W;
      
      word1 = (word1 >> (initBitPos % W)) & ~(~0 << word1Size);
      
      int word2Size = queryLen - word1Size;
      
      //cout << "word1Size: " << W - word2Size << endl;
      
      //word2 &= ~(~0 << (W - word2Size));
      word2 &= ~(~0 << (word2Size));
      //word2 >>= (len - word1Size);
      
      if(bDebug)
      {
	cout << "===" << endl;
	cout << "Word1: "; for(int i = 0; i < W; i++) cout << bitget(&word1, i); cout << endl; 
	cout << "Word2: "; for(int i = 0; i < W; i++) cout << bitget(&word2, i); cout << endl;
      }
      
      //cout << word2Size << endl;
      //word1 = (word1 << word1Size) | word2;
      word2 <<= word1Size;
      
      word1 |= word2;
      
      if(bDebug)
      {
	cout << "===" << endl;
	cout << "Word1: "; for(int i = 0; i < W; i++) cout << bitget(&word1, i); cout << endl; 
	//cout << "Word2: "; for(int i = 0; i < W; i++) cout << bitget(&word2, i); cout << endl;
      }
      
      uint wordOP = *op;
      wordOP = (wordOP & ~(~0 << queryLen));
      
      if(bDebug)
      {
	//cout << "===" << endl;
	cout << "Op: "; for(int i = 0; i < W; i++) cout << bitget(op, i); cout << endl;
      }
      result = word1 ^ wordOP;
    }
    
    if(result == 0)
    {
      return -1;
      //cout << "OK!" << endl;
    }
    else
    {
      unsigned int failPosition = (result & ~(result - 1)); 
      
      //delete this.
      //initBitPos = 0;
      
      return bits(failPosition) + initBitPos - 1;
      //return floor(log2(failPosition)) + initBitPos;
    }
    
    return 0;
}


uint SPBitmap::GetBitAt(int position)
{
  return bitget(bitmap, position);
}

void SPBitmap::PrintBitmap(int spaceAtPosition)
{
  int n = len / WL + 1;
  int lastIdxSize = len % WL;
  
  int a = spaceAtPosition;;
  
  for(int i = 0; i < n; i++)
  {
    unsigned long m = bitmap[i];
    
    vector<int> v;
    while(m)
    {
	v.push_back(m % 2);
	m /= 2;
    }
    
    reverse(v.begin(), v.end());
    
    int l = WL - v.size();
    
    if(i + 1 == n)
      l = lastIdxSize - v.size();
    
    for(int j = 0; j < l; j++) 
      cout << "0";
    
    for(int j = 0; j < v.size(); j++)
      cout << v[j];
  }
  
  cout << endl;
  
  /*
  for(int i = 0; i < len; i++) 
  {
    if(spaceAtPosition == i) cout << " ";
    //if(i == 32 || i == 29) cout << " ";
    
    cout << bitget(bitmap, i);
  }
  */
  //cout << endl;
}