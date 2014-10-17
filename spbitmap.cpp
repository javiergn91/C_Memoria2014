//#include <libcdsBasics.h>
#include "spbitmap.h"
#include <iostream>
#include "math.h"
#include "utils.h"

using namespace std;
using namespace cds_utils;
using namespace cds_static;

SPBitmap::SPBitmap(unsigned int* bitmap, int len, BITSEQ seq) 
{
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
}

SPBitmap::SPBitmap()
{
  
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

int SPBitmap::XOR(uint* op, int initBitPos, int queryLen)
{
    int initIndex = initBitPos / W;
    int finalIndex = (initBitPos + queryLen) / W;
    
    uint result = 0;
    
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

uint SPBitmap::GetBitAt(int position)
{
  return bitget(bitmap, position);
}

void SPBitmap::PrintBitmap(int spaceAtPosition)
{
  for(int i = 0; i < len; i++) 
  {
    if(spaceAtPosition == i) cout << " ";
    //if(i == 32 || i == 29) cout << " ";
    
    cout << bitget(bitmap, i);
  }
  //cout << endl;
}