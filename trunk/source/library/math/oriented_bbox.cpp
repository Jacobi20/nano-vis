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
#include "../math/oriented_bbox.h"

/*-----------------------------------------------------------------------------
	BBox :
-----------------------------------------------------------------------------*/

EOrientedBBox::EOrientedBBox( void )
{
	SetExtents(0,0,0);
	SetPosition(EVec3(0,0,0));
	SetOrient(QuatIdentity());
}


EOrientedBBox::EOrientedBBox( float sx, float sy, float sz )
{
	SetExtents(sx, sy, sz);
	SetPosition(EVec3(0, 0, 0));
	SetOrient(QuatIdentity());
}


EOrientedBBox::~EOrientedBBox( void )
{

}


void EOrientedBBox::SetExtents( float sx, float sy, float sz )
{
	extents.x	=	sx;
	extents.y	=	sy;
	extents.z	=	sz;
}


void EOrientedBBox::SetPosition( const EVec3 &pos )
{
	center	=	pos;
}


void EOrientedBBox::SetOrient( const EQuat &q )
{
	orient = q;
}