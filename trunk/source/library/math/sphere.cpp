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
	Sphere :
-----------------------------------------------------------------------------*/

//
//	ESphere::ESphere
//
ESphere::ESphere( void )
{
	SetPosition( EVec4(0,0,0,1) );
	SetRadius( 0 );
}


//
//	ESphere::ESphere
//
ESphere::ESphere( const EVec4 &pos, float r /*= 0 */ )
{
	SetPosition( pos );
	SetRadius( r );
}


//
//	ESphere::SetPosition
//
void ESphere::SetPosition( const EVec4 &pos )
{
	ASSERT(pos.w);
	position	=	pos;
	position	/=	pos.w;
}


//
//	ESphere::GetPosition
//
EVec4 ESphere::GetPosition( void ) const
{
	return position;
}


//
//	ESphere::SetRadius
//
void ESphere::SetRadius( float r )
{
	radius	=	r;	
}


//
//	ESphere::GetRadius
//
float ESphere::GetRadius( void ) const
{
	return radius;
}


//
//	ESphere::IsPointInside
//
bool ESphere::IsPointInside( const EVec4 &point )  const
{
	ASSERT(point.w);
	
	float dist = Vec4Length( point - position );
	if (dist<=radius) {
		return true;
	} 
	return false;
}


//
//	ESphere::Intersects
//
bool ESphere::Intersects( const ESphere &other ) const
{
	float	distance	=	Vec4Length( position - other.position );
	
	if (distance <= radius + other.radius) {
		return true;
	}
	return false;
}

