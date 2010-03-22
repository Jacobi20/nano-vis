/*
	The MIT License

	Copyright (c) 2010 IFMO/GameDev Studio

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
*/

#pragma once

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#ifdef WIN32
	#include <conio.h>
#endif
#ifdef LINUX
	#include <ncurses.h>
#endif
#include <math.h>

#include <iostream>
#include <fstream>
#include <memory>
#include <cstring>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <list>

using namespace std;


#pragma warning (disable : 4996)	//	'function' : was declared deprecated
#pragma warning (disable : 4244)	//	'variable' : conversion from 'double' to 'float', possible loss of data

/*-----------------------------------------------------------------------------
	Types
-----------------------------------------------------------------------------*/

#ifdef WIN32
	typedef	unsigned char			byte;
	typedef	unsigned char			uchar;
	typedef	unsigned short int		word;
	typedef	unsigned long int		dword;
	typedef	unsigned int			uint;
	typedef	unsigned long			ulong;

	typedef	signed char				int8_t;
	typedef	short signed int		int16_t;
	typedef __int32					int32_t;
	typedef __int64					int64_t;

	typedef	unsigned char			uint8_t;
	typedef	short unsigned int		uint16_t;
	typedef	long unsigned int		uint32_t;
	typedef	unsigned __int64		uint64_t;
	typedef long unsigned int		bool_t;
#else 
    typedef  unsigned char byte;
#endif

#ifdef LINUX
	typedef	unsigned char			byte;
	typedef	unsigned char			uchar;
	typedef	unsigned short int		word;
	typedef	unsigned long int		dword;
	//typedef	unsigned long int		uint;
	typedef	unsigned long			ulong;

	typedef	signed char				int8_t;
	typedef	short signed int		int16_t;
	//typedef	long signed int			int32_t;
	//typedef	signed __int64			int64_t;

	typedef	unsigned char			uint8_t;
	typedef	short unsigned int		uint16_t;
	//typedef	long unsigned int		uint32_t;
	//typedef	unsigned __int64		uint64_t;

	typedef long unsigned int		bool_t;
#endif


/*-----------------------------------------------------------------------------
	Misc.
-----------------------------------------------------------------------------*/

#ifdef interface
#undef interface
#endif




#define	CLASS_FORBID_COPY(cls)			\
	private:							\
			cls(cls &other);			\
	void	operator=(cls &other);	


/*-----------------------------------------------------------------------------
	ASSERT / VERIFY
-----------------------------------------------------------------------------*/

void Lib_Assertion		( const char *expr, const char *func, const char *file, uint line );
void Lib_Signal			( const char *msg, const char *func, const char *file, uint line );
void Lib_Fatal			( const char *msg, const char *func, const char *file, uint line );
void Lib_LogException	( const char *msg, const char *func, const char *file, uint line );

#undef ASSERT
#undef VERIFY
#undef SIGNAL
#undef FATAL
#undef ASSERT_SLOW
#undef VERIFY_SLOW

#define	ASSERT(expr)	if (!(expr))	Lib_Assertion(#expr, __FUNCTION__, __FILE__, __LINE__);
#define VERIFY(expr)	if (!(expr))	Lib_Assertion(#expr, __FUNCTION__, __FILE__, __LINE__);
#define	SIGNAL(msg)		Lib_Signal(msg, __FUNCTION__, __FILE__, __LINE__);
#define	FATAL(msg)		Lib_Fatal(msg, __FUNCTION__, __FILE__, __LINE__);

#ifdef _DEBUG
	#define	ASSERT_SLOW(expr)	if (!(expr))	Lib_Assertion(#expr, __FUNCTION__, __FILE__, __LINE__);
	#define VERIFY_SLOW(expr)	if (!(expr))	Lib_Assertion(#expr, __FUNCTION__, __FILE__, __LINE__);
#else
	#define	ASSERT_SLOW(expr)
	#define VERIFY_SLOW(expr)	expr
#endif


#define	RAISE_EXCEPTION(dscr)										\
	do {															\
		Lib_LogException(dscr, __FUNCTION__, __FILE__, __LINE__);	\
		throw exception(dscr);										\
	} while (0);


#define HRCALL(expr)				if (FAILED(expr))	SIGNAL(#expr)
#define HRCALL_THROW(expr)			if (FAILED(expr))	RAISE_EXCEPTION(#expr)


/*-----------------------------------------------------------------------------
	misc function
-----------------------------------------------------------------------------*/
template<class Type> void Swap(Type &a, Type &b)
{
	Type t = a;
	a = b;
	b = t;
}

template<class Type> Type Clamp(Type a, Type low, Type high)
{
	if (a > high) a = high;
	if (a < low) a = low;
	return a;
}


inline bool IsPOT(uint64_t num)
{
	if (num==0) return false;
	if ((num & (num-1))==0) return true;
	return false;
}


inline float Frac(float a) {
	return a - floor(a);
}

inline float FRand(float min=0, float max=1) {
	float fr = rand() / (float)RAND_MAX;
	return min + fr * (max-min);
}


/*-----------------------------------------------------------------------------
	Constants
-----------------------------------------------------------------------------*/

#define LIB_MAKEFOURCC(ch0, ch1, ch2, ch3)									\
                ((uint)(uchar)(ch0) << 0   | ((uint)(uchar)(ch1) << 8  )  |	\
                ((uint)(uchar)(ch2) << 16) | ((uint)(uchar)(ch3) << 24 ))

/*-----------------------------------------------------------------------------
	Lib headers
-----------------------------------------------------------------------------*/

#include "math/math.h"
#include "math/vector.h"
#include "math/matrix.h"
#include "math/angles.h"
#include "math/plane.h"
#include "math/quaternion.h"
#include "math/bbox.h"
#include "math/sphere.h"
#include "math/frustum.h"

#include "images/image.h"
#include "images/image_tga.h"
#include "images/image_proc.h"

#include "singleton.h"
#include "string.h"
#include "va.h"
#include "args.h"
#include "state.h"
#include "array.h"
#include "attr_list.h"
/*#include "math\affine.h"*/
/*#include "math\complex.h"
#include "math\quaternion.h"*/

bool	Math_RayTriangleIntersection(EVec3 &result, float &fraction, const EVec3 &start, const EVec3 &dir, const EVec3 &v0,  const EVec3 &v1,  const EVec3 &v2 );
EVec3	Math_GetRadialDistribution3(void);
EVec2	Math_GetRadialDistribution2(void);
float	Filter_Recursive(float current, float target, float tau, float dt);


//
//	End of file :
//

