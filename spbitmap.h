#ifndef _SP_BITMAP_H_
#define _SP_BITMAP_H_

class SPBitmap {
private:
  unsigned int* bitmap;
  int len;
  
public:
  SPBitmap(unsigned int* bitmap, int len);
  unsigned int XOR(unsigned int op, int initBitPos);
  void PrintBitmap();
};

#endif