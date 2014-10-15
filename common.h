#ifndef _COMMON_H_
#define _COMMON_H_

typedef enum
{
  BITSEQ_NONE,
  BITSEQ_RRR,
  BITSEQ_SARRAY
} BITSEQ;

typedef struct 
{
  unsigned int* bitmap;
  int len;
} BitmapWrapper;

class Point
{
public:
  Point(int _x, int _y)
  {
      x = _x;
      y = _y;
  }
  
  int x;
  int y;
};

extern BITSEQ pathLenBitSequence;
extern BITSEQ pathNextBitSequence;

#endif