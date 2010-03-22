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
	Plane stuff declaration :
-----------------------------------------------------------------------------*/

class EPlane 
	{
	public:
				EPlane		( void );
				EPlane		( float a, float b, float c, float d );
				EPlane		( const float *p );
				EPlane		( const EPlane &other );
		EPlane	&operator =	( const EPlane &other );
		bool	operator ==	( const EPlane &other );
		bool	operator !=	( const EPlane &other );
		
	public:
		float a, b, c, d;
	};
	

EPlane	PlaneFromPointNormal	( const EVec4 &point, const EVec4 &normal );
EPlane	PlaneFromPoints			( const EVec4 &p0, const EVec4 &p1, const EVec4 &p2 );
bool	PlaneIntersection		( EVec4 &out_point, const EPlane &p0, const EPlane &p1, const EPlane &p2 );
int		PlaneClassify			( const EPlane &plane, const EVec4 &point );
float	PlaneDistance			( const EPlane &plane, const EVec4 &point );
bool	PlaneIsPointEnclosed	( const EVec4 &point, uint num_planes, const EPlane *planes, float point_normal_offset );


/*-----------------------------------------------------------------------------
	Plane inline implementation :
-----------------------------------------------------------------------------*/

inline EPlane::EPlane( void )
{
	a = 0;
	b = 0;
	c = 1;
	d = 0;
}

inline EPlane::EPlane( float a, float b, float c, float d )
{
	this->a = a;
	this->b = b;
	this->c = c;
	this->d = d;
}


inline EPlane::EPlane( const float *p )
{
	a = p[0];
	b = p[1];
	c = p[2];
	d = p[3];
}


inline EPlane::EPlane( const EPlane &other )
{
	a = other.a;
	b = other.b;
	c = other.c;
	d = other.d;
}


inline EPlane	& EPlane::operator=( const EPlane &other )
{
	a = other.a;
	b = other.b;
	c = other.c;
	d = other.d;
	return *this;
}


inline bool EPlane::operator==( const EPlane &other )
{
	return (a == other.a &&
			b == other.b &&
			c == other.c &&
			d == other.d );
}


inline bool EPlane::operator!=( const EPlane &other )
{
	return ((*this)!=other);
}