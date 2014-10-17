#include <libcdsBasics.h>
#include <libcdsBitString.h>


#include "utils.h"
#include <vector>
#include <iostream>
#include <algorithm>

using namespace cds_utils;
using namespace std;

unsigned long long Utils::GetDecimalRepresentation(vector<int>* binaryRepresentation)
{
    unsigned long long multBy = 1;
    unsigned long long sum = 0;

    for(int i = 0; i < (int)binaryRepresentation->size(); i++)
    {
        int currDig = binaryRepresentation->at(i);
        sum += (multBy * currDig);
        multBy *= 2;
    }

    return sum;
}

unsigned int Utils::GetBytesRequired(unsigned int n)
{
    //int wordSize = 32;
    unsigned int bytesRequired = n / W;

    if(n % W != 0)
    {
        bytesRequired++;
    }

    return bytesRequired;
}

unsigned int* Utils::CreateBitSequence(string str)
{
    uint* word = (uint*)malloc(sizeof(uint));
    //mybasics::bitzero(word, 0, str.size());
    
    for(int i = 0; i < str.size(); i++) 
    {
      if(str[i] == '1')
      {
	bitset(word, i);
      }
      else
      {
	bitclean(word, i);
      }
    }
    return word;
}

void Utils::PrintBinary(unsigned int decimalNumber, bool bNormal)
{
  if(decimalNumber == 0)
  {
    cout << 0 << endl;
    return;
  }
  
  vector<int> v;
  while(decimalNumber)
  {
   v.push_back(decimalNumber % 2);
   decimalNumber /= 2;
  }
  reverse(v.begin(), v.end());
  for(int i = 0; i < v.size(); i++) 
  {
   cout << v[i]; 
  }
  cout << endl;
}

void Utils::CreateQuadCode(int x, int y, BitmapWrapper* bitmapWrapper, int quadLength)
{
  //unsigned short x1 = x;
  //unsigned short y1 = y;
  
  uint z = MortonTable256[x >> 8]   << 17 | 
           MortonTable256[y >> 8]   << 16 |
           MortonTable256[x & 0xFF] <<  1 | 
           MortonTable256[y & 0xFF];
  
  bitmapWrapper->len = quadLength;//(floor(log2(x)) + 1) + (floor(log2(y) + 1));
  bitmapWrapper->bitmap = new uint[uint_len(bitmapWrapper->len, 1)];
  
  
  for(int i = quadLength - 1; i >= 0; i--)
  {
    //bitset(bitmapWrapper->bitmap, i);
      (z & 1) ? bitset(bitmapWrapper->bitmap, i) : bitclean(bitmapWrapper->bitmap, i);
      z >>= 1;
  }
  
  /*
  set_field(bitmapWrapper->bitmap, bitmapWrapper->len, 0, z);
  for(int i = 0; i < quadLength; i++)
  {
      cout << bitget(bitmapWrapper->bitmap, i);
  }
  cout << endl;
  
  //set_field(bitmapWrapper->bitmap, bitmapWrapper->len, 0, z);
  */
  
  //======================
  /*
  int tmpL = quadLength;
  quadLength--;
  
  //cout << quadLength << endl;
  
  while(x != 0 || y != 0)
  {
    //cout << x << " " << y << endl;
    
    if(y == 0 || y % 2 == 0) bitclean(bitmapWrapper->bitmap, quadLength);
    else bitset(bitmapWrapper->bitmap, quadLength);
 
    if(x == 0 || x % 2 == 0) bitclean(bitmapWrapper->bitmap, quadLength - 1);
    else bitset(bitmapWrapper->bitmap, quadLength - 1);
  
    x >>= 1;
    y >>= 1; 
    
    quadLength -= 2;
  }
  
  //cout << quadLength << endl;
  while(quadLength >= 0)
  {
    bitclean(bitmapWrapper->bitmap, quadLength);
    quadLength--;
  }
  */
  /*
  for(int i = 0; i < tmpL; i++)
  {
      cout << bitget(bitmapWrapper->bitmap, i);
  }
  cout << endl;
  */
  //bitset(bitmapWrapper->bitmap, 0);
  
}
