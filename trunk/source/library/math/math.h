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

#ifndef PI
	#define	PI				3.14159265358f
#endif

const float MAX_FLOAT_VALUE	=	3.40E38f;
const float MIN_FLOAT_VALUE	=	1.18E-38f;
const float	FLOAT_EPSILON	=	1.192092896e-07f;	//	smallest positive number such that 1.0+FLT_EPSILON != 1.0


inline float sqr( float a ) { return a*a; }

#define DEG2RAD(ang)	(ang/180.0f*PI)
#define RAD2DEG(ang)	(ang*180.0f/PI)

inline float	deg2rad	( float a ) { return a/57.295779513260928129089516840402f; }
inline float	rad2deg	( float a ) { return a*57.295779513260928129089516840402f; }


inline float MinPositive(float a, float b) {
	if (a<0 && b<0) return MAX_FLOAT_VALUE;
	if (a<0) return b;
	if (b<0) return a;
	if (a>b) return b;
	return a;
}


inline void sincos(float a, float &s, float &c) {
	s	=	sin(a);
	c	=	cos(s);
}


uint GetBitPosition(uint64_t num);

