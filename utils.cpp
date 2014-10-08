#include "basics.h"
#include "utils.h"
#include <vector>
#include <iostream>
#include <algorithm>
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
    mybasics::bitzero(word, 0, str.size());
    
    for(int i = 0; i < str.size(); i++) 
    {
      if(str[i] == '1')
      {
	bitset(word, i);
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
