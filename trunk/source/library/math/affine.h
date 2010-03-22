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

class EAffine4
	{
	public:

	static	EMatrix4	Identity	( void );
	static	EMatrix4	RotateX		( float phi );
	static	EMatrix4	RotateY		( float phi );
	static	EMatrix4	RotateZ		( float phi );
	static	EMatrix4	Rotate		( float phi, float x, float y, float z);
	static	EMatrix4	Scale		( float sx, float sy, float sz );
	static	EMatrix4	Translate	( float tx, float ty, float tz );
	static	EMatrix4	Frustum		( float left, float right, float bottom, float top, float znear, float zfar );
	static	EMatrix4	Ortho		( float left, float right, float bottom, float top, float znear, float zfar );
	};


inline EMatrix4 EAffine4::Identity(void) {
	return EMatrix4();
}


inline EMatrix4 EAffine4::RotateX(float phi) {
	float c = cosf(phi/180.0f*PI), s = sinf(phi/180.0f*PI);
	return EMatrix4(	1,	0,	0,	0,
						0,	c, -s,	0,
						0,	s,	c,	0,
						0,	0,	0,	1	);
};


inline EMatrix4 EAffine4::RotateY(float phi) {
	float c = cosf(phi/180.0f*PI), s = sinf(phi/180.0f*PI);
	return EMatrix4(	c,  0,	s,	0,
						0,  1,  0,	0,
						-s,	0,	c,	0,
						0,	0,	0,	1	);
};


inline EMatrix4 EAffine4::RotateZ(float phi) {
	float c = cosf(phi/180.0f*PI), s = sinf(phi/180.0f*PI);
	return EMatrix4(	c, -s,	0,	0,
						s,  c,  0,	0,
						0,	0,	1,	0,
						0,	0,	0,	1	);
};


inline EMatrix4 EAffine4::Rotate(float phi, float x, float y, float z) {
	EVec3		ax(x,y,z);
	float		c = cosf(phi), 
				s = sinf(phi);
	ax.Normalize();
	x = ax.x, y = ax.y, z = ax.z;

	return EMatrix4(	c+(1-c)*x*x,	      (1-c)*y*x-s*z,	  (1-c)*z*x+s*y,	0,
						(1-c)*x*y+s*z,		c+(1-c)*y*y,	      (1-c)*z*y-s*x,	0,
						(1-c)*x*z-s*y,		  (1-c)*y*z+s*x,	c+(1-c)*z*z,		0,
									0,				0,				0,				1);
}


inline EMatrix4 EAffine4::Scale(float sx, float sy, float sz) {
	return EMatrix4(sx, 0,  0,  0,
					0, sy,  0,  0,
					0,  0, sz,  0,
					0,  0,  0,  1 );
}


inline EMatrix4 EAffine4::Translate(float tx, float ty, float tz) {
	return EMatrix4(1,  0,  0,  tx,
					0,  1,  0,  ty,
					0,  0,  1,  tz,
					0,  0,  0,  1 );
}


inline EMatrix4 EAffine4::Frustum( float left, float right, float bottom, float top, float znear, float zfar )
{
	float	A	=	(right+left)/(right-left);
	float	B	=	(top+bottom)/(top-bottom);
	
	float	C	=	-(zfar+znear)/(zfar-znear);
	float	D	=	-2*(zfar*znear)/(zfar-znear);
	
	float	E	=	2*(znear)/(right-left);
	float	F	=	2*(znear)/(top-bottom);
	
	return EMatrix4(E,  0,  A,  0,
					0,  F,  B,  0,
					0,  0,  C,  D,
					0,  0, -1,  0 );
}


inline EMatrix4 EAffine4::Ortho( float left, float right, float bottom, float top, float znear, float zfar )
{
	float	A	=	 2.0f/(right-left);
	float	B	=	 2.0f/(top-bottom);
	float	C	=	-2.0f/(zfar-znear);
	
	float	T1	=	-(right+left)/(right-left);
	float	T2	=	-(top+bottom)/(top-bottom);
	float	T3	=	-(zfar+znear)/(zfar-znear);
	
	return EMatrix4(A,  0,  0, T1,
					0,  B,  0, T2,
					0,  0,  C, T3,
				    0,  0,  0,  1 );
}

















