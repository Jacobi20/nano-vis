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

#include "../library.h"


//
//	GetBitPosition
//
uint GetBitPosition(uint64_t num)
{
	if (!IsPOT(num)) {
		return 0;
	}
	
	if (!num) {
		return 0;
	}
	
	for (uint i=0; i<64; i++) {
		if (num & ((int64_t)1<<i)) return i;
	}
	return 0;
}



//
//	Filter_Recursive
//
float Filter_Recursive ( float current, float target, float tau, float dt)
{
	return	current + (target - current) * (1 - exp(-dt / tau));
}



//
//	Math_GetRadialDistribution3
//
EVec3	Math_GetRadialDistribution3(void) 
{
	for (;;) {
		float x = FRand();
		float y = FRand();
		float z = FRand();

		EVec3 dir(x,y,z);

		if (Vec3Length(dir)>1.0f) continue;

		EVec3 dir2 = Vec3Normalize(dir);
											   
		return dir2;
	}
}


//
//	Math_GetRadialDistribution2
//
EVec2	Math_GetRadialDistribution2(void) 
{
	for (;;) {
		float x = (rand()%2000)/1000.0f - 1.0f;
		float y = (rand()%2000)/1000.0f - 1.0f;

		EVec2 dir(x,y);

		if (Vec2Length(dir)>1.0f) continue;

		EVec2 dir2 = Vec2Normalize(dir);
											   
		return dir2;
	}
}



//
//	Math_RayTriangleIntersection
//	
bool Math_RayTriangleIntersection(EVec3 &result, float &fraction, const EVec3 &start, const EVec3 &dir, 
												const EVec3 &v0,  const EVec3 &v1,  const EVec3 &v2 )
{
	EVec3	v0v1	=	v1 - v0;
	EVec3	v0v2	=	v2 - v0;
	EVec3	v2v1	=	v1 - v2;
	EVec3	N		=	Vec3Normalize( Vec3Cross(v0v1, v0v2) );
	float	d		=	- Vec3Dot(N, v0);
	
	/*result = start + dir;
	fraction = 1;
	return true;*/

	
	float NdotD		=	Vec3Dot(N, dir);

	fraction = 0;	
	if (fabs(NdotD)<0.00001) return false;	// triangle and ray are parallel

	fraction = - ((d + Vec3Dot(N, start)) / NdotD);
	
	if ( fraction<0.0f ) return false;				// triangle is behind the ray
	if ( fraction>1.0f ) return false;				// ray can`t hit the triangle

	EVec3	P	=	start + dir * fraction;
	
	EVec3	v0P	=	P - v0;
	EVec3	v2P	=	P - v2;
	
	// stupid algorithm using cross products :
	float sign_v0v1 = Vec3Dot(N, Vec3Cross(v0v1, v0P));	// should be positive! (+)
	float sign_v0v2 = Vec3Dot(N, Vec3Cross(v0v2, v0P));	// should be negative! (-)
	float sign_v2v1 = Vec3Dot(N, Vec3Cross(v2v1, v2P));	// should be negative! (-)

	if (sign_v0v1>=0 && sign_v0v2<=0 && sign_v2v1<=0) {
		result = P;
		return true;
	} else {
		return false;
	}
}