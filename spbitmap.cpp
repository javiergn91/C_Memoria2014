#include "basics.h"
#include "spbitmap.h"
#include <iostream>
#include "math.h"
#include "utils.h"
using namespace std;

SPBitmap::SPBitmap(unsigned int* bitmap, int len) 
{
  this->bitmap = bitmap;
  this->len = len;
}

unsigned int SPBitmap::XOR(unsigned int op, int initBitPos)
{ 
  //op = 5;
  //initBitPos = 30;
  
  //cout << len << endl;
  
  //cout << "Code: ";
  //Utils::PrintBinary(op, false);
  
  //cout << "Starting position: " << initBitPos << endl;
  
  int numBits = floor(log2(op)) + 1;
  
  int initIndex = initBitPos / W;
  int finalIndex = (initBitPos + numBits) / W;
  
  uint result = 0;
  
  //binary code is inside a word.
  if(initIndex == finalIndex) 
  {
    uint word1 = *(bitmap + initIndex);
    word1 = (word1 >> (initBitPos % W)) & ~(~0 << numBits);
    //cout << "Subcode Quadcode: ";
    //Utils::PrintBinary(word1);
    //cout << "XOR result: ";
    //Utils::PrintBinary(word1^op);
    result = word1^op;
    
    //int shiftRightBits = initBitPos % W;
    //word1 >>= shiftRightBits;
    //Utils::PrintBinary(word1);
    //word1 &= ~(1 << numBits);
    //Utils::PrintBinary(word1);
    //word1 <<= 1;
    //Utils::PrintBinary(word1^op);
    //cout << word1 << endl;
    
  }
  //binary code is between two words.
  else
  {  
    uint word1 = bitmap[initIndex];
    uint word2 = bitmap[finalIndex];
    
    //cout << numBits - (W - initBitPos) << endl;
    
    word1 = (word1 >> (initBitPos % W)) & ~(~0 << (W - initBitPos));
    word2 = word2 & ~(~0 << (numBits - (W - initBitPos)));
    word1 = word1 << (numBits - (W - initBitPos)) | word2;
    //cout << "W1: ";
    //Utils::PrintBinary(word1);
    //cout << "W2: ";
    //Utils::PrintBinary(word2);
    
    result = word1^op;
  }
 
  if(result == 0)
  {
    cout << "Contained" << endl;
    return -1; //code contained.
  }
  else
  {
    //if(op == 0)
    //{
    //  cout << "Fails at position: " << numBits - bitsNotContained << endl;
    //  return numBits - bitsNotContained;
    //}
    //else
    //{
      int bitsNotContained = floor(log2(result)) + 1;
      cout << "Fails at position: " << numBits - bitsNotContained << endl;
      return numBits - bitsNotContained;
    //}
  }
}

void SPBitmap::PrintBitmap()
{
  for(int i = 0; i < len; i++) 
  {
    cout << bitget(bitmap, i);
  }
  cout << endl;
}