
// Basics

#ifndef BASICSINCLUDED
#define BASICSINCLUDED

// Includes

#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

namespace mybasics{

// Data types
#ifndef byte
  #define byte unsigned char
#endif

#ifndef uchar
  #define uchar unsigned char
#endif


#define max(x,y) ((x)>(y)?(x):(y))
#define min(x,y) ((x)<(y)?(x):(y))

#define W (8*sizeof(uint))
#define WW 64

/* 4 bits factorization*/
#define mask31 0x0000001F
#define Wminusone 31
/* Fin 4 bits*/

/** bits needed to represent a number between 0 and n */
inline uint bits(uint n){
  uint b = 0;
  while (n) { b++; n >>= 1; }
  return b;
}

// reads bit p from e
#define bitget(e,p) (((e)[(p)/W] >> ((p)%W)) & 1)
// sets bit p in e
#define bitset(e,p) ((e)[(p)/W] |= (1<<((p)%W)))
// cleans bit p in e
#define bitclean(e,p) ((e)[(p)/W] &= ~(1<<((p)%W)))

#define bytewrite(e,p,v) ((e)[(p)/W] |= (v<<((p)%W)))

#define byteread(e,p) (((e)[(p)/W] >> ((p)%W)) & 0xFF)


// bits needed to represent a number between 0 and n
uint bits (uint n);
// returns e[p..p+len-1], assuming len <= W
uint bitread (uint *e, uint p, uint len);
// writes e[p..p+len-1] = s, assuming len <= W
void bitwrite (uint *e, uint p, uint len, uint s);
// writes e[p..p+len-1] = 0, no assumption on len
/**/ //FARI. WITH ASSUMPTION ON LEN, OR IT CRASHES
//NOt WORKING UPON THE LIMIT OF THE STARTING uint.
void bitzero (uint *e, uint p, uint len);

/** uints required to represent e integers of n bits each */
//#define uint_len(e,n) (((e)*(n))/W+(((e)*(n))%W > 0))
inline uint uint_len(uint e, uint n) {
  return ((unsigned long long)e*n/W+((unsigned long long)e*n%W>0));
}

/** popcount array for uchars */
const unsigned char __popcount_tab[] = {
  0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,
};

/** select array for uchars */
const unsigned char select_tab[] = {
  0, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
  6, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
  7, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
  6, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
  8, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
  6, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
  7, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
  6, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
};

/** prev array for uchars */
const unsigned char prev_tab[] = {
  0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
};

/********** some definition for bp_gonzalo ***********/
// returns e[p..p+len-1], assuming len <= W
#define bitsW 5 // OJO

inline uint bitget_go(uint *e, uint p, uint len){
  uint answ;
  e += p >> bitsW; p &= (1<<bitsW)-1;
  answ = *e >> p;
  if (len == W)
  {
    if(p)
      answ |= (*(e+1)) << (W-p);
  }
  else{
    if (p+len > W)
      answ |= (*(e+1)) << (W-p);
    answ &= (1<<len)-1;
  }
  return answ;
}

// writes e[p..p+len-1] = s, len <= W
inline void bitput (register uint *e, register uint p, register uint len, register uint s){
  e += p >> bitsW;
  p &= (1<<bitsW)-1;
  if (len == W){
    *e |= (*e & ((1<<p)-1)) | (s << p);
    if (!p)
      return;
    e++;
    *e = (*e & ~((1<<p)-1)) | (s >> (W-p));
  }
  else{
    if (p+len <= W){
      *e = (*e & ~(((1<<len)-1)<<p)) | (s << p);
      return;
    }
    *e = (*e & ((1<<p)-1)) | (s << p);
    e++;
    len -= W-p;
    *e = (*e & ~((1<<len)-1)) | (s >> (W-p));
  }
}

/****************************************************/
// returns e[p..p+len-1], assuming len <= W
inline uint bitget2(uint *e, uint p, uint len){
  register uint i=(p>>5), j=p&0x1F;
  if (j+len <= W)
    return (e[i] << (W-j-len)) >> (W-len);
  else
    return (e[i] >> j) | ((e[i+1] << (W-j-len)) >> (W-len));
}

#define bitget1(e,p) ((e)[(p)/W] & (1<<((p)%W)))

/***************************************************/

/** Counts the number of 1s in x */
inline uint popcount(int x){
  return __popcount_tab[(x >>  0) & 0xff]  + __popcount_tab[(x >>  8) & 0xff]
  + __popcount_tab[(x >> 16) & 0xff] + __popcount_tab[(x >> 24) & 0xff];
}

/** Retrieve a given bitsequence from array A
* @param A Array
* @param ini Starting position
* @param fin Retrieve until end-1
*/
inline uint get_var_field(uint *A, uint ini, uint fin) {
  if(ini==fin+1) return 0;
  uint i=ini/W, j=ini-W*i, result;
  uint len = (fin-ini+1);
  if (j+len <= W)
    result = (A[i] << (W-j-len)) >> (W-len);
  else {
    result = A[i] >> j;
    result = result | (A[i+1] << (WW-j-len)) >> (W-len);
  }
  return result;
}

/** Stores a given bitsequence into array A
* @param A Array
* @param ini Starting position
* @param fin Store until end-1
* @param x Value to be stored
*/
inline void set_var_field(uint *A, uint ini, uint fin, uint x) {
  if(ini==fin+1) return;
  uint i=ini/W, j=ini-i*W;
  uint len = (fin-ini+1);
  uint mask = ((j+len) < W ? ~0u << (j+len) : 0)
  | ((W-j) < W ? ~0u >> (W-j) : 0);
  A[i] = (A[i] & mask) | x << j;
  if (j+len>W) {
    mask = ((~0u) << (len+j-W));
    A[i+1] = (A[i+1] & mask)| x >> (W-j);
  }
}

/** Retrieve a given index from array A where every value uses len bits
* @param A Array
* @param len Length in bits of each field
* @param index Position to be retrieved
*/
inline uint get_field_64(uint *A, uint len, uint index) {
  if(len==0) return 0;
  uint result;
  long long i=1, j=1;
  i=i*index*len/W, j=j*index*len-W*i;
  if (j+len <= W)
    result = (A[i] << (W-j-len)) >> (W-len);
  else {
    result = A[i] >> j;
    result = result | (A[i+1] << (WW-j-len)) >> (W-len);
  }
  return result;
}

inline uint get_field(uint *A, uint len, uint index) {
  if(len==0) return 0;
  register uint i=index*len/W, j=index*len-W*i, result;
  if (j+len <= W)
    result = (A[i] << (W-j-len)) >> (W-len);
  else {
    result = A[i] >> j;
    result = result | (A[i+1] << (WW-j-len)) >> (W-len);
  }
  return result;
}


/** Store a given value in index into array A where every value uses len bits
* @param A Array
* @param len Length in bits of each field
* @param index Position to store in
* @param x Value to be stored
*/
inline void set_field(uint *A, uint len, uint index, uint x) {
  if(len==0) return;
  uint i=index*len/W, j=index*len-i*W;
  uint mask = ((j+len) < W ? ~0u << (j+len) : 0)
  | ((W-j) < W ? ~0u >> (W-j) : 0);
  A[i] = (A[i] & mask) | x << j;
  if (j+len>W) {
    mask = ((~0u) << (len+j-W));
    A[i+1] = (A[i+1] & mask)| x >> (W-j);
  }
}

inline void set_field_64(uint *A, uint len, uint index, uint x) {
  if(len==0) return;
  long long i=1, j=1;
  i= i*index*len/W, j= j*index*len-i*W;
  uint mask = ((j+len) < W ? ~0u << (j+len) : 0)
  | ((W-j) < W ? ~0u >> (W-j) : 0);
  A[i] = (A[i] & mask) | x << j;
  if (j+len>W) {
    mask = ((~0u) << (len+j-W));
    A[i+1] = (A[i+1] & mask)| x >> (W-j);
  }
}

/** Counts the number of 1s in the first 8 bits of x */
inline uint popcount8(int x){
  return __popcount_tab[x & 0xff];
}

}

#endif
