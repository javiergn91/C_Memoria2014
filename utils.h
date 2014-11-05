#ifndef _UTILS_H_
#define _UTILS_H_
/********************************************************************
 * Author: Javier González N.
 * 
 * Description: Utility functions
 */

#include "common.h"
#include "relation2D.h"
#include <vector>
#include <string>
using namespace std;

class Utils
{
public:
    //Given a vector of 0's and 1's, returns the decimal number representing the bitstring.
    static unsigned long long GetDecimalRepresentation(vector<int>* binaryRepresentation);
    
    //Neccesary bytes to store n bits.
    static unsigned int GetBytesRequired(unsigned int n);
    
    //Given a decimal number, prints his binary representation using std::cout.
    static void PrintBinary(unsigned int decimalNumber, bool bNormal = false);
    
    //Given a string of the form "0011" (for example), returns his decimal representation.
    static unsigned int* CreateBitSequence(string str);
    
    //The three functions below are deprecrated.
    static void CreateQuadCode(int x, int y, BitmapWrapper* bitmapWrapper, int quadLength);
    static void CreateQuadCode2(int x, int y, BitmapWrapper* bitmapWrapper, int quadLength);
    static void CreateQC_Old(int x, int y, BitmapWrapper* bitmapWrapper, int quadLength);
    
    //Given a coordinate (x, y), returns a quadcode. The resultanting quadcode must contain less than 62 (or equal) bits.
    static unsigned long QuadCode(int x, int y);
    
    //Prints binary representation of a long number, using std::cout
    static void PrintLong(unsigned long n);
    
    //Returns most significant bit (position) of an unsigned int.
    static char msb(unsigned int v) {
      unsigned int ret=0;
      if(v==0) return 0;
      asm("bsrl %1, %0" : "=r" (ret) : "r" (v));
      return (char)ret+1;
    }
    
    //Extracts values from certain txt files, don't use it.
    static void ParseTXTFile(string filename, Relation2D &relation2D);
};

#endif // _UTILS_H_
