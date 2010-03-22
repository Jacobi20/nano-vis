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
#include "quaternion.h"

/*-----------------------------------------------------------------------------
	Quaternion :
	NOTE: i^2 = j^2 = j^2 = ijk = -1
-----------------------------------------------------------------------------*/

//
//	QuatConjugate
//
EQuat QuatConjugate( const EQuat &q )
{
	return EQuat(-q.x, -q.y, -q.z, q.w);
}


//
//	QuatDot
//
float QuatDot( const EQuat &q1, const EQuat &q2 )
{
	return	q1.x * q2.x
		+	q1.y * q2.y
		+	q1.z * q2.z
		+	q1.w * q2.w ;
}


//
//	QuatIdentity
//
EQuat QuatIdentity( void )
{
	return EQuat(0,0,0,1);
}


//
//	QuatInverse
//
EQuat QuatInverse( const EQuat &q )
{
	return QuatConjugate( q ) / QuatLengthSqr( q );
}


//
//	QuatLength
//
float QuatLength( const EQuat &q )
{
	return sqrt( QuatDot(q, q) );
}


//
//	QuatLengthSqr
//
float QuatLengthSqr( const EQuat &q )
{
	return QuatDot(q, q);
}


//
//	QuatMultiply
//
EQuat QuatMultiply( const EQuat &q1, const EQuat &q2 )
{
	EVec3	v1(q1);
	EVec3	v2(q2);
	EVec3	img		=	Vec3Cross(v1,v2) + v1*q2.w + v2*q1.w;
	float	real	=	q1.w * q2.w - Vec3Dot(v1,v2);
	return EQuat(img.x, img.y, img.z, real);

	//	id Software`s :
	//return EQuat(	q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y,
	//				q1.w*q2.y + q1.y*q2.w + q1.z*q2.x - q1.x*q2.z,
	//				q1.w*q2.z + q1.z*q2.w + q1.x*q2.y - q1.y*q2.x,
	//				q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z );
}


//
//	QuatNormalize
//
EQuat QuatNormalize( const EQuat &q )
{
	float len = QuatLength( q );
	if (len!=0) {
		return EQuat(	q.x / len, 		q.y / len, 
						q.z / len, 		q.w / len );
	}
	return q;
}


//
//	QuatRotationAxis
//
EQuat QuatRotationAxis( float angle, const EVec3 &axis )
{
	//	to return normalized quaternion - normalize axis vector :
	EVec3	naxis = Vec3Normalize( axis );

	float s = sin(angle/2);
	float c = cos(angle/2);
	
	return EQuat( naxis.x*s, naxis.y*s, naxis.z*s, c );
}


//
//	QuatRotateVector
//	q = q * v * q^-1
//	this function works with non-normalized quaternions :
//
EVec4 QuatRotateVector( const EVec4 &v, const EQuat &q )
{
	ASSERT( v.w==0 );
	EQuat qi = QuatInverse( q );
	EQuat qv ( v.x, v.y, v.z, 0 );
	
	return EVec4( QuatMultiply( QuatMultiply( q, qv ), qi ) );
}


//
//	QuatSLerp
//	this function works with non-normalized quaternions,
//	and always returns normalized vector.
//
//	iq = (q*sin((1-t)*omega) + q'*sin(t*omega))/sin(omega),
//	where cos(omega) = inner_product(q,q') 
//
EQuat QuatSLerp( const EQuat &q1, const EQuat &q2, float t )
{
	EQuat	from = q1;
	EQuat	to	= q2;
	
	const float DELTA = 0.01f;
	
	float p1[4];
	double omega, cosom, sinom, scale0, scale1;

	//	cosinus :
	cosom = q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;

	if ( cosom < 0.0 ) { 
		cosom = -cosom;
		p1[0] = - q2.x;  p1[1] = - q2.y;
		p1[2] = - q2.z;  p1[3] = - q2.w;
	}
	else {
		p1[0] = q2.x;    p1[1] = q2.y;
		p1[2] = q2.z;    p1[3] = q2.w;
	}

	//	angle is bigger than delta :
	if ( (1.0 - cosom) > DELTA ) {
		omega = acos(cosom);
		sinom = sin(omega);
		scale0 = sin((1.0 - t) * omega) / sinom;
		scale1 = sin(t * omega) / sinom;
	} else {        
		scale0 = 1.0 - t;
		scale1 = t;
	}

	EQuat	result(
		scale0 * q1.x + scale1 * p1[0],
		scale0 * q1.y + scale1 * p1[1],
		scale0 * q1.z + scale1 * p1[2],
		scale0 * q1.w + scale1 * p1[3] );
	
	return result;
}


//
//	QuatToMatrix
//	see: http://www.gamedev.ru/code/articles/?id=4215&page=2
//
EMatrix4 QuatToMatrix( const EQuat q )
{
    float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
    float s  = 2.0f/QuatLengthSqr(q);
    x2 = q.x * s;    y2 = q.y * s;    z2 = q.z * s;
    xx = q.x * x2;   xy = q.x * y2;   xz = q.x * z2;
    yy = q.y * y2;   yz = q.y * z2;   zz = q.z * z2;
    wx = q.w * x2;   wy = q.w * y2;   wz = q.w * z2;

	EMatrix4 m = Matrix4Identity();
	
    m(0,0) = 1.0f - (yy + zz);
    m(1,0) = xy - wz;
    m(2,0) = xz + wy;

    m(0,1) = xy + wz;
    m(1,1) = 1.0f - (xx + zz);
    m(2,1) = yz - wx;

    m(0,2) = xz - wy;
    m(1,2) = yz + wx;
    m(2,2) = 1.0f - (xx + yy);
    
    
    return m;
}


//
//	QuatFromMatrix
//	TODO DHG : Ensure this function returns normalized quaternion.
//	id Software`s math lib :
//
EQuat QuatFromMatrix( const EMatrix4 &m )
{
	EMatrix4	mat	=	Matrix4Transpose(m);

	EQuat		q;
	float		trace;
	float		s;
	float		t;
	int     	i;
	int			j;
	int			k;

	static int 	next[ 3 ] = { 1, 2, 0 };

	trace = mat(0,0) + mat(1,1) + mat(2,2);

	if ( trace > 0.0f ) {

		t = trace + 1.0f;
		s = 0.5 / sqrt( t );

		q[3] = s * t;
		q[0] = ( mat(2,1) - mat(1,2) ) * s;
		q[1] = ( mat(0,2) - mat(2,0) ) * s;
		q[2] = ( mat(1,0) - mat(0,1) ) * s;

	} else {

		i = 0;
		if ( mat(1,1) > mat(0,0) ) {
			i = 1;
		}
		if ( mat(2,2) > mat(i,i) ) {
			i = 2;
		}
		j = next[i];
		k = next[j];

		t = ( mat(i,i) - ( mat(j,j) + mat(k,k) ) ) + 1.0f;
		s = 0.5 / sqrt( t );

		q[i] = s * t;
		q[3] = ( mat(k,j) - mat(j,k) ) * s;
		q[j] = ( mat(j,i) + mat(i,j) ) * s;
		q[k] = ( mat(k,i) + mat(i,k) ) * s;
	}
	return q;
}


//
//	QuatToAngles
//	- id Software's code
//
void QuatToAngles( const EQuat &q, float &yaw, float &pitch, float &roll )
{
	EMatrix4	M	=	QuatToMatrix( q );
	Matrix4ToAngles( M, yaw, pitch, roll );
}


//
//	QuatFromAngles
//	- id Software's code
//
EQuat QuatFromAngles( float yaw, float pitch, float roll )
{
	float sx, cx, sy, cy, sz, cz;
	float sxcy, cxcy, sxsy, cxsy;
	
	sincos( deg2rad(yaw)	* 0.5f, sz, cz );
	sincos( deg2rad(pitch)	* 0.5f, sy, cy );
	sincos( deg2rad(roll)	* 0.5f, sx, cx );

	sxcy = sx * cy;
	cxcy = cx * cy;
	sxsy = sx * sy;
	cxsy = cx * sy;

	return EQuat( cxsy*sz - sxcy*cz, -cxsy*cz - sxcy*sz, sxsy*cz - cxcy*sz, cxcy*cz + sxsy*sz );
}

/*-----------------------------------------------------------------------------
	Quaternion class :
-----------------------------------------------------------------------------*/

EQuat::EQuat( const float *fp )
{
	x = fp[0];
	y = fp[1];
	z = fp[2];
	w = fp[3];
}


EQuat::EQuat( float x, float y, float z, float w )
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}


EQuat::EQuat()
{
	x = y = z = 0;
	w = 1;
}


EQuat::operator float*()
{
	return &x;
}


EQuat::operator const float*() const
{
	return &x;
}


EQuat& EQuat::operator+=( const EQuat &q )
{
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
	return *this;
}


EQuat& EQuat::operator-=( const EQuat &q )
{
	x -= q.x;
	y -= q.y;
	z -= q.z;
	w -= q.w;
	return *this;
}


EQuat& EQuat::operator*=( const EQuat &q )
{
	*this = QuatMultiply(*this, q);
	return *this;
}


EQuat& EQuat::operator/=( const EQuat &q )
{
	*this = QuatMultiply(*this, QuatInverse(q));
	return *this;
}


EQuat& EQuat::operator*=( float a )
{
	x *= a;
	y *= a;
	z *= a;
	w *= a;
	return *this;
}


EQuat& EQuat::operator/=( float a )
{
	x /= a;
	y /= a;
	z /= a;
	w /= a;
	return *this;
}


EQuat EQuat::operator+() const
{
	return EQuat(x,y,z,w);
}


EQuat EQuat::operator-() const
{
	return EQuat(-x,-y,-z,-w);
}


EQuat EQuat::operator+( const EQuat &q ) const
{
	EQuat q1 = *this;
	return q1 += q;
}


EQuat EQuat::operator-( const EQuat &q ) const
{
	EQuat q1 = *this;
	return q1 -= q;
}


EQuat EQuat::operator*( const EQuat &q ) const
{
	return QuatMultiply( *this, q );
}


EQuat EQuat::operator/( const EQuat &q ) const
{
	return QuatMultiply( *this, QuatInverse( q ) );
}


EQuat EQuat::operator*( float a ) const
{
	EQuat q1 = *this;
	return q1 *= a;
}


EQuat operator*( float a, const EQuat &q )
{
	return q * a;
}

EQuat EQuat::operator/( float a ) const
{
	return (*this) * (1.0f/a);
}


bool EQuat::operator==( const EQuat &q ) const
{
	return (x==q.x && y==q.y && z==q.z && w==q.w ); 
}


bool EQuat::operator!=( const EQuat &q ) const
{
	return !(*this==q);
}


//float EQuat::operator[]( uint index ) const
//{
//	ASSERT(index<4);
//	return ( &x )[ index ];
//}
//
//
//float & EQuat::operator[]( uint index )
//{
//	ASSERT(index<4);
//	return ( &x )[ index ];
//}