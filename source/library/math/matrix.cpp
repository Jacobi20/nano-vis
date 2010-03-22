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


/*-----------------------------------------------------------------------------
	MATRIX 3x3 Stuff :
-----------------------------------------------------------------------------*/

EMatrix3 Matrix3Identity( void )
{
	return EMatrix3();
}


float Matrix3Det( const EMatrix3 &A )
{
	return 
		+ A.M[0][0] * A.M[1][1] * A.M[2][2]
		+ A.M[0][1] * A.M[1][2] * A.M[2][0]
		+ A.M[0][2] * A.M[1][0] * A.M[2][1]
		
		- A.M[0][2] * A.M[1][1] * A.M[2][0]
		- A.M[0][0] * A.M[1][2] * A.M[2][1]
		- A.M[0][1] * A.M[1][0] * A.M[2][2];
/*	return (
		A.M[0][0] * (A.M[1][1]*A.M[2][2] - A.M[2][1]*A.M[1][2]) -
		A.M[0][1] * (A.M[1][0]*A.M[2][2] - A.M[2][0]*A.M[1][2]) +
		A.M[0][2] * (A.M[1][0]*A.M[2][1] - A.M[2][0]*A.M[1][1]) );	*/
}


float Matrix3Minor( const EMatrix3 &A, uint row, uint col )
{
	ASSERT(row<3);
	ASSERT(col<3);
	float m2[3][3];
	
	for (uint r=0, rm=0; r<3; r++,rm++)
	for (uint c=0, cm=0; c<3; c++,cm++)
	{
		if (r==row) r++;
		if (c==col) c++;
		m2[rm][cm] = A.M[r][c];
	}
	
	return m2[0][0] * m2[1][1] - m2[0][1] * m2[1][0];
}


EMatrix3 Matrix3Inverse( const EMatrix3 &A )
{
	float d = Matrix3Det( A );
	
	ASSERT(d!=0);
	
	EMatrix3	temp;

	for (int i=0; i<3; i++)
	for (int j=0; j<3; j++)
		temp.M[i][j] = Matrix3Minor(A, i, j) * (1-((i+j)&1)*2);

	temp	=	Matrix3Transpose(temp);
	temp	=	temp * ( 1.0f/d );
	
	return temp;
}



EMatrix3 Matrix3Transpose( const EMatrix3 &A )
{
	EMatrix3	temp(A);
	
	Swap<float>(temp.M[0][1], temp.M[1][0]);
	Swap<float>(temp.M[0][2], temp.M[2][0]);
	Swap<float>(temp.M[1][2], temp.M[2][1]);
	
	return temp;
}


EVec3 Matrix3Transform( const EVec3 &a, const EMatrix3 &T )
{
	EVec3	temp;
	temp.v[0] = T.M[0][0]*a.v[0] + T.M[0][1]*a.v[1] + T.M[0][2]*a.v[2];
	temp.v[1] = T.M[1][0]*a.v[0] + T.M[1][1]*a.v[1] + T.M[1][2]*a.v[2];
	temp.v[2] = T.M[2][0]*a.v[0] + T.M[2][1]*a.v[1] + T.M[2][2]*a.v[2];
	return temp;
}


/*-----------------------------------------------------------------------------
	MATRIX 4x4 Stuff :
-----------------------------------------------------------------------------*/

//
//	Matrix4Translate
//
EMatrix4 Matrix4Translate(float x, float y, float z)
{
	return	EMatrix4(	1,	0,	0,	0,
						0,	1,	0,	0,
						0,	0,	1,	0,
						x,	y,	z,	1	);
}


//
//	Matrix4Translate
//
EMatrix4 Matrix4Translate(const EVec4 &dir)
{
	return Matrix4Translate(dir.x, dir.y, dir.z);
}


//
//	Matrix4Scale
//
EMatrix4 Matrix4Scale(float x, float y, float z)
{
	return	EMatrix4(	x,	0,	0,	0,
						0,	y,	0,	0,
						0,	0,	z,	0,
						0,	0,	0,	1	);
}


//
//	Matrix4RotateX
//
EMatrix4 Matrix4RotateX(float a)
{
	float c	=	cosf(a);
	float s	=	sinf(a);
	return	EMatrix4(	1,	0,	0,	0,
						0,	c,	s,	0,
						0,	-s,	c,	0,
						0,	0,	0,	1	);
}


//
//	Matrix4RotateY
//
EMatrix4 Matrix4RotateY(float a)
{
	float c	=	cosf(a);
	float s	=	sinf(a);
	return	EMatrix4(	c,	0,	-s,	0,
						0,	1,	0,	0,
						s,	0,	c,	0,
						0,	0,	0,	1	);
}


//
//	Matrix4RotateZ
//
EMatrix4 Matrix4RotateZ(float a)
{
	float c	=	cosf(a);
	float s	=	sinf(a);
	return	EMatrix4(	c,	s,	0,	0,
						-s,	c,	0,	0,
						0,	0,	1,	0,
						0,	0,	0,	1	);
}


//
//	Matrix4PerspectiveRH
//
EMatrix4 Matrix4PerspectiveRH(float w, float h, float zn, float zf)
{
	return	EMatrix4(
			2*zn/w,  0,       0,              0,
			0,       2*zn/h,  0,              0,
			0,       0,       zf/(zn-zf),    -1,
			0,       0,       zn*zf/(zn-zf),  0
	);
}


//
//	Matrix4PerspectiveLH
//
EMatrix4 Matrix4PerspectiveLH(float w, float h, float zn, float zf)
{
	return EMatrix4(
			2*zn/w,  0,       0,             0,
			0,       2*zn/h,  0,             0,
			0,       0,       zf/(zf-zn),    1,
			0,       0,       zn*zf/(zn-zf), 0
	);
}


//
//	Matrix4PerspectiveFovRH
//
EMatrix4 Matrix4PerspectiveFovRH(float fovy, float aspect, float zn, float zf)
{
	float y_scale	=	1.0f/tan(fovy/2.0f);
	float x_scale	=	aspect * y_scale;
	return EMatrix4(
			x_scale, 0,       0,             0,
			0,       y_scale, 0,             0,
			0,       0,       zf/(zn-zf),    -1,
			0,       0,       zn*zf/(zn-zf), 0
	);
}


//
//	Matrix4PerspectiveFovLH
//
EMatrix4 Matrix4PerspectiveFovLH(float fovy, float aspect, float zn, float zf)
{
	float y_scale	=	1.0f/tan(fovy/2.0f);
	float x_scale	=	aspect * y_scale;
	return EMatrix4(
			x_scale, 0,       0,             0,
			0,       y_scale, 0,             0,
			0,       0,       zf/(zf-zn),    1,
			0,       0,       zn*zf/(zn-zf), 0
	);
}


//
//	Matrix4OrthoRH
//
EMatrix4 Matrix4OrthoRH(float w, float h, float zn, float zf)
{
	return EMatrix4
		(	2/w,	0,		0,			0,
			0,		2/h,	0,			0,
			0,		0,		1/(zn-zf),	0,
			0,		0,		zn/(zn-zf),	1	);
}


//
//	Matrix4OrthoLH
//	
EMatrix4 Matrix4OrthoLH(float w, float h, float zn, float zf)
{
	return EMatrix4
		(	2/w,	0,		0,			0,
			0,		2/h,	0,			0,
			0,		0,		1/(zf-zn),	0,
			0,		0,		zn/(zn-zf),	1	);
}


//
//	Matrix4Identity
//
EMatrix4 Matrix4Identity( void )
{
	return EMatrix4();
}


//
//	Matrix4Det
//	
float Matrix4Det( const EMatrix4 &A )
{
	float d1	=	Matrix4Minor(A, 0,0) * A.M[0][0];
	float d2	=	Matrix4Minor(A, 0,1) * A.M[0][1];
	float d3	=	Matrix4Minor(A, 0,2) * A.M[0][2];
	float d4	=	Matrix4Minor(A, 0,3) * A.M[0][3];
	return d1 - d2 + d3 - d4;
}


//
//	Matrix4Minor
//
float Matrix4Minor( const EMatrix4 &A, uint row, uint col )
{
	ASSERT(row<4);
	ASSERT(col<4);
	
	uint r, c;		// index in base matrix 
	uint r1, c1;	// index in minor
	EMatrix3 minor;

	for (r=0,r1=0; r<4; r++)
	{
		if (r==row) continue;
		for (c=0,c1=0; c<4; c++)
		{
			if (c==col) continue;
			minor.M[r1][c1] = A.M[r][c];
			c1++;
		}
		r1++;
	}
	return Matrix3Det(minor);
}


//
//	Matrix4Inverse
//
EMatrix4 Matrix4Inverse( const EMatrix4 &A )
{
	EMatrix4	temp;
	
	float d = Matrix4Det(A);
	
	ASSERT(d!=0);

	for (int i=0; i<4; i++) {
		for (int j=0; j<4; j++)	{
			temp.M[i][j] = Matrix4Minor(A,i,j) * (1-((i+j)&1)*2);
		}
	}

	temp	=	Matrix4Transpose(temp);
	temp	=	temp * ( 1.0f/d );
	
	return temp;
}


//
//	Matrix4Transpose
//
EMatrix4 Matrix4Transpose( const EMatrix4 &A )
{
	EMatrix4	temp(A);
	
	Swap<float>( temp.M[0][1], temp.M[1][0] );
	Swap<float>( temp.M[0][2], temp.M[2][0] );
	Swap<float>( temp.M[0][3], temp.M[3][0] );
	Swap<float>( temp.M[2][1], temp.M[1][2] );
	Swap<float>( temp.M[3][1], temp.M[1][3] );
	Swap<float>( temp.M[3][2], temp.M[2][3] );
	
	return temp;
}


//
//	Matrix4Transform
//
EVec4 Matrix4Transform( const EVec4 &a, const EMatrix4 &T )
{
	EVec4	temp;
	temp.v[0] =  T.M[0][0]*a.v[0]  +  T.M[1][0]*a.v[1]  +  T.M[2][0]*a.v[2]  +  T.M[3][0]*a.v[3];
	temp.v[1] =  T.M[0][1]*a.v[0]  +  T.M[1][1]*a.v[1]  +  T.M[2][1]*a.v[2]  +  T.M[3][1]*a.v[3];
	temp.v[2] =  T.M[0][2]*a.v[0]  +  T.M[1][2]*a.v[1]  +  T.M[2][2]*a.v[2]  +  T.M[3][2]*a.v[3];
	temp.v[3] =  T.M[0][3]*a.v[0]  +  T.M[1][3]*a.v[1]  +  T.M[2][3]*a.v[2]  +  T.M[3][3]*a.v[3];
	return temp;
}


//
//	Matrix4ToAngles
//	- id Software's code
//
void Matrix4ToAngles( const EMatrix4 &A, float &yaw, float &pitch, float &roll )
{
	double		theta;
	double		cp;
	float		sp;

	sp = A(0, 2);

	// cap off our sin value so that we don't get any NANs
	if ( sp > 1.0f ) {
		sp = 1.0f;
	} else if ( sp < -1.0f ) {
		sp = -1.0f;
	}

	theta = -asin( sp );
	cp = cos( theta );

	if ( cp > 8192.0f * FLOAT_EPSILON ) {
		pitch	= rad2deg( theta );
		yaw		= rad2deg( atan2( A( 0, 1 ), A( 0, 0 ) ) );
		roll	= rad2deg( atan2( A( 1, 2 ), A( 2, 2 ) ) );
	} else {
		pitch	= rad2deg( theta );
		yaw		= rad2deg( -atan2( A( 1, 0 ), A( 1, 1 ) ) );
		roll	= 0;
	}
}











