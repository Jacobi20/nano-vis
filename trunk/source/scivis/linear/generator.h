#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdlib.h>
#include <time.h>

#if !defined(VAX)
 static unsigned long jflone=0x3f800000;
 static unsigned long jflmsk=0x007fffff;
#else
 static unsigned long jflone=0x00004080;
 static unsigned long jflmsk=0xffff007f;
#endif

static unsigned long iran;

inline void Randomize()
{//RANDOMIZE
	static int d;
	iran=jflmsk*((int)time(NULL)+d++);
}//RANDOMIZE


inline void Randomize(int d)
{//RANDOMIZE
	iran=jflmsk*((int)time(NULL)+d++);
}//RANDOMIZE


//GENERATOR 0.........max_size
inline int rnd(int max_size)
{//rnd
	iran=1664525L*iran+1013904223L;
	unsigned long temp=jflone|(jflmsk&iran);
	return (int)(((*(float*)&temp)-1.F)*max_size);
}//rnd

//GENERATOR 0.........1
inline float rnd()
{//rnd
	iran=1664525L*iran+1013904223L;
	unsigned long temp=jflone|(jflmsk&iran);
	return (*(float*)&temp)-1.F;
}//rnd

#endif 

