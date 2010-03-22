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

/*-----------------------------------------------------------------------------
	Quaternion :
	NOTE: i^2 = j^2 = j^2 = ijk = -1
-----------------------------------------------------------------------------*/

class	EQuat	{
	public:
		EQuat();
		EQuat( const float * );
		EQuat( float x, float y, float z, float w );

		// casting
		operator float* ();
		operator const float* () const;

		//	TODO DHG : type casting or overloaded []?
		//float  operator [] ( uint index ) const;
		//float &operator [] ( uint index );

		// assignment operators
		EQuat& operator += ( const EQuat &q );
		EQuat& operator -= ( const EQuat &q );
		EQuat& operator *= ( const EQuat &q );
		EQuat& operator /= ( const EQuat &q );
		EQuat& operator *= ( float );
		EQuat& operator /= ( float );

		// unary operators
		EQuat operator + () const;
		EQuat operator - () const;

		// binary operators
		EQuat operator + ( const EQuat &q ) const;
		EQuat operator - ( const EQuat &q ) const;
		EQuat operator * ( const EQuat &q ) const;
		EQuat operator / ( const EQuat &q ) const;
		EQuat operator * ( float a ) const;
		EQuat operator / ( float a ) const;

		friend EQuat operator * ( float a, const EQuat &q );

		bool operator == ( const EQuat &q ) const;
		bool operator != ( const EQuat &q ) const;
		
	public:
		float x,y,z;	// i,j,k
		float w;		// scalar
	};
	


EQuat		QuatConjugate		( const EQuat &q );
float		QuatDot				( const EQuat &q1, const EQuat &q2 );
EQuat		QuatIdentity		( void );
EQuat		QuatInverse			( const EQuat &q );
float		QuatLength			( const EQuat &q );
float		QuatLengthSqr		( const EQuat &q );
EQuat		QuatMultiply		( const EQuat &q1, const EQuat &q2 );
EQuat		QuatNormalize		( const EQuat &q );
EQuat		QuatRotationAxis	( float angle, const EVec3 &axis );
EVec4		QuatRotateVector	( const EVec4 &v, const EQuat &q );
EQuat		QuatSLerp			( const EQuat &q1, const EQuat &q2, float t );
EMatrix4	QuatToMatrix		( const EQuat q );
EQuat		QuatFromMatrix		( const EMatrix4 &m );
void		QuatToAngles		( const EQuat &q, float &yaw, float &pitch, float &roll );
EQuat		QuatFromAngles		( float yaw, float pitch, float roll );


/*-----------------------------------------------------------------------------
	Implementation :	
-----------------------------------------------------------------------------*/


