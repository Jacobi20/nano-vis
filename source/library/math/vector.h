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

#include "../library.h"

class EVec4;
class EVec3;


/*-----------------------------------------------------------------------------
	4D-Vector
-----------------------------------------------------------------------------*/

class	EVec4
	{
	public:
		union {
			float v[4];
			struct { float x, y, z, w; };
		};

		EVec4			( void );
		EVec4			( float x, float y, float z, float w );
		EVec4			( const float *v );

		operator float* ()				{ return v; }
		operator const float* () const	{ return v; }
		
		float			*Ptr			( void );
		const	float	*Ptr			( void ) const;
						
		EVec4			&operator	+=	( const EVec4 &a );
		EVec4			&operator	-=	( const EVec4 &a );
		EVec4			&operator	*=	( float s );
		EVec4			&operator	*=	( const EVec4 &a );
		EVec4			&operator	/=	( float s );
		
		EVec4			operator	+	( void ) const;
		EVec4			operator	-	( void ) const;

		friend EVec4	operator	+	( const EVec4 &a, const EVec4 &b );
		friend EVec4	operator	-	( const EVec4 &a, const EVec4 &b );
		friend EVec4	operator	*	( const EVec4 &a, float s );
		friend EVec4	operator	*	( const EVec4 &a, const EVec4 &b );
		friend EVec4	operator	*	( float s, const EVec4 &a );
		friend EVec4	operator	/	( const EVec4 &a, float s );
		
		friend bool		operator	==	( const EVec4 &a, const EVec4 &b );
		friend bool		operator	!=	( const EVec4 &a, const EVec4 &b );
	};


inline EVec4::EVec4(void) 
{
	x = y = z = w = 0;
}

inline EVec4::EVec4(float x, float y, float z, float w)
{
	this->x	=	x;
	this->y	=	y;
	this->z	=	z;
	this->w	=	w;
}

inline EVec4::EVec4(const float *v)
{
	this->x	=	v[0];
	this->y	=	v[1];
	this->z	=	v[2];
	this->w	=	v[3];
}


inline float *EVec4::Ptr(void) 
{
	return &x;
}

inline const float *EVec4::Ptr(void) const 
{
	return &x;
}


inline EVec4 &EVec4::operator +=(const EVec4 &a)
{
	x += a.x;	y += a.y;
	z += a.z;	w += a.w;
	return *this;
}

inline EVec4 &EVec4::operator -=(const EVec4 &a)
{
	x -= a.x;	y -= a.y;
	z -= a.z;	w -= a.w;
	return *this;
}

inline EVec4 &EVec4::operator *=(float s)
{
	x *= s;  	y *= s;
	z *= s;  	w *= s;
	return *this;
}

inline EVec4 &EVec4::operator *=(const EVec4 &a)
{
	x *= a.x;  	y *= a.y;
	z *= a.z;  	w *= a.w;
	return *this;
}

inline EVec4 &EVec4::operator /=(float s)
{
	x /= s;  	y /= s;
	z /= s;  	w /= s;
	return *this;
}


inline EVec4 EVec4::operator -(void) const
{
	return EVec4(-x, -y, -z, -w);
}


inline EVec4 EVec4::operator +(void) const
{
	return EVec4(+x, +y, +z, +w);
}




inline EVec4 Vec4Add(const EVec4 &a, const EVec4 &b)
{	  
	return EVec4(a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w);
}


inline EVec4 Vec4Mul(const EVec4 &a, const EVec4 &b)
{	  
	return EVec4(a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w);
}


inline EVec4 Vec4Subtract(const EVec4 &a, const EVec4 &b)
{	  
	return EVec4(a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w);
}


inline EVec4 Vec4Scale(const EVec4 &a, float s)
{
	return EVec4(a.x * s, a.y * s, a.z * s, a.w * s);
}


inline EVec4 Vec4Lerp(const EVec4 &a, const EVec4 &b, float s)
{
	return EVec4 (	a.x*(1-s) +	b.x*s,
					a.y*(1-s) +	b.y*s,
					a.z*(1-s) +	b.z*s,
					a.w*(1-s) +	b.w*s);
}


inline float Vec4Length(const EVec4 &a)
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z + a.w*a.w);
}


inline float Vec4LengthSqr(const EVec4 &a)
{
	return a.x*a.x  +  a.y*a.y  +  a.z*a.z  +  a.w*a.w;
}


inline EVec4 Vec4Normalize(const EVec4 &a)
{
	float len	=	Vec4Length(a);
	if (len==0) return a;
	return Vec4Scale(a, 1.0f/len);
}


inline bool Vec4Compare(const EVec4 &a, const EVec4 &b, float eps=0)
{
	if (fabs(a.x - b.x)>eps) return false;
	if (fabs(a.y - b.y)>eps) return false;
	if (fabs(a.z - b.z)>eps) return false;
	if (fabs(a.w - b.w)>eps) return false;
	return true;
}


inline float Vec4Dot(const EVec4 &a, const EVec4 &b)
{
	return a.x * b.x  +  a.y * b.y  +  a.z * b.z  +  a.w * b.w;
}


inline EVec4 Vec4Cross(const EVec4 &a, const EVec4 &b, const EVec4 &c)
{
	ASSERT(0 && "Vec4Cross() not yet implemented");
	return EVec4(0,0,0,0);
}




inline EVec4 operator + (const EVec4 &a, const EVec4 &b)
{
	return Vec4Add(a, b);
}

inline EVec4 operator - (const EVec4 &a, const EVec4 &b)
{
	return Vec4Subtract(a, b);
}

inline EVec4 operator * (const EVec4 &a, float s)
{
	return Vec4Scale(a, s);
}

inline EVec4 operator * (float s, const EVec4 &a)
{
	return Vec4Scale(a, s);
}

inline EVec4 operator * (const EVec4 &a, const EVec4 &b)
{
	return Vec4Mul(a, b);
}

inline EVec4 operator / (const EVec4 &a, float s)
{
	return Vec4Scale(a, 1.0f/s);
}

inline bool operator == (const EVec4 &a, const EVec4 &b)
{
	return (a.x==b.x && 
			a.y==b.y &&
			a.z==b.z &&
			a.w==b.w);
}

inline bool operator != (const EVec4 &a, const EVec4 &b)
{
	return !(a==b);
}


/*-----------------------------------------------------------------------------
	3D-Vector
-----------------------------------------------------------------------------*/

class	EVec3
	{
	public:
		union {
			float v[3];
			struct { float x, y, z; };
		};

						EVec3			( void ) {};
						EVec3			( float x, float y, float z );
						EVec3			( const float *v );

		operator float* ()				{ return v; }
		operator const float* () const	{ return v; }

		float			*Ptr			( void );
		const	float	*Ptr			( void ) const;
						
		EVec3			&operator	+=	( const EVec3 &a );
		EVec3			&operator	-=	( const EVec3 &a );
		EVec3			&operator	*=	( float s );
		EVec3			&operator	/=	( float s );
		
		EVec3			operator	+	( void ) const;
		EVec3			operator	-	( void ) const;

		friend EVec3	operator	+	( const EVec3 &a, const EVec3 &b );
		friend EVec3	operator	-	( const EVec3 &a, const EVec3 &b );
		friend EVec3	operator	*	( const EVec3 &a, float s );
		friend EVec3	operator	*	( float s, const EVec3 &a );
		friend EVec3	operator	/	( const EVec3 &a, float s );
	};



inline EVec3::EVec3(float x, float y, float z)
{
	this->x	=	x;
	this->y	=	y;
	this->z	=	z;
}

inline EVec3::EVec3(const float *v)
{
	this->x	=	v[0];
	this->y	=	v[1];
	this->z	=	v[2];
}


inline float *EVec3::Ptr(void) 
{
	return &x;
}


inline const float *EVec3::Ptr(void) const 
{
	return &x;
}



inline EVec3 &EVec3::operator +=(const EVec3 &a)
{
	x += a.x;	y += a.y;
	z += a.z;
	return *this;
}

inline EVec3 &EVec3::operator -=(const EVec3 &a)
{
	x -= a.x;	y -= a.y;
	z -= a.z;
	return *this;
}

inline EVec3 &EVec3::operator *=(float s)
{
	x *= s;  	y *= s;
	z *= s;
	return *this;
}

inline EVec3 &EVec3::operator /=(float s)
{
	x /= s;  	y /= s;
	z /= s;
	return *this;
}



inline EVec3 EVec3::operator -(void) const
{
	return EVec3(-x, -y, -z);
}


inline EVec3 EVec3::operator +(void) const
{
	return EVec3(+x, +y, +z);
}


inline EVec3 Vec3Add(const EVec3 &a, const EVec3 &b)
{	  
	return EVec3(a.x+b.x, a.y+b.y, a.z+b.z);
}


inline EVec3 Vec3Subtract(const EVec3 &a, const EVec3 &b)
{	  
	return EVec3(a.x-b.x, a.y-b.y, a.z-b.z);
}


inline EVec3 Vec3Scale(const EVec3 &a, float s)
{
	return EVec3(a.x * s, a.y * s, a.z * s);
}


inline EVec3 Vec3Lerp(const EVec3 &a, const EVec3 &b, float s)
{
	return EVec3 (	a.x*(1-s) +	b.x*s,
					a.y*(1-s) +	b.y*s,
					a.z*(1-s) +	b.z*s);
}


inline float Vec3Length(const EVec3 &a)
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}


inline float Vec3LengthSqr(const EVec3 &a)
{
	return a.x*a.x  +  a.y*a.y  +  a.z*a.z;
}


inline EVec3 Vec3Normalize(const EVec3 &a)
{
	float len	=	Vec3Length(a);
	if (len==0) return a;
	return Vec3Scale(a, 1.0f/len);
}


inline bool Vec3Compare(const EVec3 &a, const EVec3 &b, float eps=0)
{
	if (fabs(a.x - b.x)>eps) return false;
	if (fabs(a.y - b.y)>eps) return false;
	if (fabs(a.z - b.z)>eps) return false;
	return true;
}


inline float Vec3Dot(const EVec3 &a, const EVec3 &b)
{
	return a.x * b.x  +  a.y * b.y  +  a.z * b.z;
}


inline EVec3 Vec3Cross(const EVec3 &a, const EVec3 &b)
{
	return EVec3 (	a.y*b.z - a.z*b.y,	
					a.z*b.x - a.x*b.z,	
					a.x*b.y - a.y*b.x );	
}




inline EVec3 operator + (const EVec3 &a, const EVec3 &b)
{
	return Vec3Add(a, b);
}

inline EVec3 operator - (const EVec3 &a, const EVec3 &b)
{
	return Vec3Subtract(a, b);
}

inline EVec3 operator * (const EVec3 &a, float s)
{
	return Vec3Scale(a, s);
}

inline EVec3 operator * (float s, const EVec3 &a)
{
	return Vec3Scale(a, s);
}

inline EVec3 operator / (const EVec3 &a, float s)
{
	return Vec3Scale(a, 1.0f/s);
}


/*-----------------------------------------------------------------------------
	2D-Vector
-----------------------------------------------------------------------------*/

class	EVec2
	{
	public:
		union {
			float v[2];
			struct { float x, y; };
		};

						EVec2			( void ) {};
						EVec2			( float x, float y );
						EVec2			( const float *v );

		operator float* ()				{ return v; }
		operator const float* () const	{ return v; }
		
		float			*Ptr			( void );
		const	float	*Ptr			( void ) const;
						
		EVec2			&operator	+=	( const EVec2 &a );
		EVec2			&operator	-=	( const EVec2 &a );
		EVec2			&operator	*=	( float s );
		EVec2			&operator	/=	( float s );
		
		EVec2			operator	+	( void ) const;
		EVec2			operator	-	( void ) const;

		friend EVec2	operator	+	( const EVec2 &a, const EVec2 &b );
		friend EVec2	operator	-	( const EVec2 &a, const EVec2 &b );
		friend EVec2	operator	*	( const EVec2 &a, float s );
		friend EVec2	operator	*	( float s, const EVec2 &a );
		friend EVec2	operator	/	( const EVec2 &a, float s );
	};



inline EVec2::EVec2(float x, float y)
{
	this->x	=	x;
	this->y	=	y;
}

inline EVec2::EVec2(const float *v)
{
	this->x	=	v[0];
	this->y	=	v[1];
}


inline float *EVec2::Ptr(void) 
{
	return &x;
}


inline const float *EVec2::Ptr(void) const 
{
	return &x;
}



inline EVec2 &EVec2::operator +=(const EVec2 &a)
{
	x += a.x;	y += a.y;
	return *this;
}

inline EVec2 &EVec2::operator -=(const EVec2 &a)
{
	x -= a.x;	y -= a.y;
	return *this;
}

inline EVec2 &EVec2::operator *=(float s)
{
	x *= s;  	y *= s;
	return *this;
}

inline EVec2 &EVec2::operator /=(float s)
{
	x /= s;  	y /= s;
	return *this;
}



inline EVec2 EVec2::operator -(void) const
{
	return EVec2(-x, -y);
}


inline EVec2 EVec2::operator +(void) const
{
	return EVec2(+x, +y);
}


inline EVec2 Vec2Add(const EVec2 &a, const EVec2 &b)
{	  
	return EVec2(a.x+b.x, a.y+b.y);
}


inline EVec2 Vec2Subtract(const EVec2 &a, const EVec2 &b)
{	  
	return EVec2(a.x-b.x, a.y-b.y);
}


inline EVec2 Vec2Scale(const EVec2 &a, float s)
{
	return EVec2(a.x * s, a.y * s);
}


inline EVec2 Vec2Lerp(const EVec2 &a, const EVec2 &b, float s)
{
	return EVec2 (	a.x*(1-s) +	b.x*s,
					a.y*(1-s) +	b.y*s);
}


inline float Vec2Length(const EVec2 &a)
{
	return sqrt(a.x*a.x + a.y*a.y);
}


inline float Vec2LengthSqr(const EVec2 &a)
{
	return a.x*a.x  +  a.y*a.y;
}


inline EVec2 Vec2Normalize(const EVec2 &a)
{
	float len	=	Vec2Length(a);
	if (len==0) return a;
	return Vec2Scale(a, 1.0f/len);
}


inline bool Vec2Compare(const EVec2 &a, const EVec2 &b, float eps=0)
{
	if (fabs(a.x - b.x)>eps) return false;
	if (fabs(a.y - b.y)>eps) return false;
	return true;
}


inline float Vec2Dot(const EVec2 &a, const EVec2 &b)
{
	return a.x * b.x  +  a.y * b.y;
}


inline EVec2 operator + (const EVec2 &a, const EVec2 &b)
{
	return Vec2Add(a, b);
}

inline EVec2 operator - (const EVec2 &a, const EVec2 &b)
{
	return Vec2Subtract(a, b);
}

inline EVec2 operator * (const EVec2 &a, float s)
{
	return Vec2Scale(a, s);
}

inline EVec2 operator * (float s, const EVec2 &a)
{
	return Vec2Scale(a, s);
}

inline EVec2 operator / (const EVec2 &a, float s)
{
	return Vec2Scale(a, 1.0f/s);
}


/*-----------------------------------------------------------------------------
	Vector capping functions :
-----------------------------------------------------------------------------*/

inline EVec3 Vec3Cap( const EVec3 &v, float cap ) {
	if ( Vec3Length(v) > cap ) {
		return Vec3Normalize(v) * cap;
	}
	return v;
}

inline EVec3 Vec3CapXY( const EVec3 &v, float cap ) {
	EVec2	v2(v.x, v.y);
	if ( Vec2Length(v2) > cap ) {
		v2 = Vec2Normalize(v2) * cap;
	}
	return EVec3(v2.x, v2.y, v.z);
}


inline EVec4 Vec4Cap( const EVec4 &v, float cap ) {
	if ( Vec4Length(v) > cap ) {
		return Vec4Normalize(v) * cap;
	}
	return v;
}

inline EVec4 Vec4CapXY( const EVec4 &v, float cap ) {
	EVec2	v2(v.x, v.y);
	if ( Vec2Length(v2) > cap ) {
		v2 = Vec2Normalize(v2) * cap;
	}
	return EVec4(v2.x, v2.y, v.z, v.w);
}

