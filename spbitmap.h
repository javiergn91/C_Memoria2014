#ifndef _SP_BITMAP_H_
#define _SP_BITMAP_H_

#include "common.h"
#include <libcdsBasics.h>
#include <libcdsBitString.h>
#include <BitSequence.h>
#include <BitSequenceRG.h>
#include <BitSequenceRRR.h>
#include <BitSequenceSDArray.h>

using namespace std;
using namespace cds_utils;
using namespace cds_static;

class SPBitmap {
private:
  //unsigned int* bitmap;
  //int len;
  
  //BitSequence* bitSeq;
  
public:
  BitSequence* bitSeq;
  unsigned long* bitmap;
  int len;
  
  int n;
  int lastIdxSize;
  
  SPBitmap(unsigned int* bitmap, int len, BITSEQ seq);
  SPBitmap();
  //unsigned int XOR(unsigned int op, int initBitPos, int leftZeroes);
  int XOR(uint* op, int initBitPos, int queryLen);
  int XOR(unsigned long op, int initBitPos, int queryLen);
  
  void PrintBitmap(int spaceAtPosition);
  uint GetBitAt(int position);
  int Select(int bit, int ocurrence);
  int Rank(int bit, int position);
  int GetLen() { return len; }
  int NumberOfOnes();
  int NumberOfZeros();
  
  int GetSize() { return (bitSeq != NULL) ? bitSeq->getSize() : (len / 8 + 1); }
  bool HasRankSelectStruct() { return bitSeq != NULL; }
};

#endif
