
// Basics

#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "basics.h"


namespace mybasics{

// Memory management

/*#define malloc(n) Malloc(n)
#define free(p) Free(p)
#define realloc(p,n) Realloc(p,n)

	void *Malloc (int n)

	   { void *p;
	     if (n == 0) return NULL;
	     p = (void*) malloc (n);
	     if (p == NULL)
	        { fprintf (stderr,"Could not allocate %i bytes\n",n);
	          exit(1);
	        }
	     return p;
	   }

	void Free (void *p)

	   { if (p) free (p);
	   }

	void *Realloc (void *p, int n)

	   { if (p == NULL) return Malloc (n);
	     if (n == 0) { Free(p); return NULL; }
	     p = (void*) realloc (p,n);
	     if (p == NULL)
	        { fprintf (stderr,"Could not allocate %i bytes\n",n);
	          exit(1);
	        }
	     return p;
	   }*/


 // Data types

/*#ifndef byte
	#define byte unsigned char
#endif

//typedef unsigned char byte;
// typedef unsigned int uint;

typedef int bool;
#define true 1
#define false 0

#define max(x,y) ((x)>(y)?(x):(y))
#define min(x,y) ((x)<(y)?(x):(y))*/

  // Bitstream management

/*#define W (8*sizeof(uint))

#define bitget(e,p) (((e)[(p)/W] >> ((p)%W)) & 1)
	// sets bit p in e
#define bitset(e,p) ((e)[(p)/W] |= (1<<((p)%W)))
	// cleans bit p in e
#define bitclean(e,p) ((e)[(p)/W] &= ~(1<<((p)%W)))*/



        // returns e[p..p+len-1], assuming len <= W

uint bitread (uint *e, uint p, uint len)

   { uint answ;
     e += p/W; p %= W;
     answ = *e >> p;
     if (len == W)
	  { if (p) answ |= (*(e+1)) << (W-p);
	  }
     else { if (p+len > W) answ |= (*(e+1)) << (W-p);
            answ &= (1<<len)-1;
	  }
     return answ;
   }

  	// writes e[p..p+len-1] = s, len <= W

void bitwrite (register uint *e, register uint p,
	       register uint len, register uint s)

   { e += p/W; p %= W;
     if (len == W)
	  { *e |= (*e & ((1<<p)-1)) | (s << p);
            if (!p) return;
            e++;
            *e = (*e & ~((1<<p)-1)) | (s >> (W-p));
	  }
     else { if (p+len <= W)
	       { *e = (*e & ~(((1<<len)-1)<<p)) | (s << p);
		 return;
	       }
	    *e = (*e & ((1<<p)-1)) | (s << p);
            e++; len -= W-p;
            *e = (*e & ~((1<<len)-1)) | (s >> (W-p));
	  }
   }
  	// writes e[p..p+len-1] = 0

void bitzero (register uint *e, register uint p,
	       register uint len)

   { e += p/W; p %= W;
     if (p+len >= W)
	{ *e &= ~((1<<p)-1);
	  len -= p;
	  e++; p = 0;
	}
     while (len >= W)
	{ *e++ = 0;
	  len -= W;
	}
     if (len > 0)
	*e &= ~(((1<<len)-1)<<p);
   }
}
