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
	BBox :
-----------------------------------------------------------------------------*/


class	EOrientedBBox {
	public:
					EOrientedBBox	( void );
					EOrientedBBox	( float sx, float sy, float sz );
					~EOrientedBBox	( void );

		void		SetExtents		( float sx, float sy, float sz );
		void		SetPosition		( const EVec3 &pos );
		void		SetOrient		( const EQuat &q );
					
		EVec3		GetExtents		( void ) const { return extents; }
		EVec3		GetPosition		( void ) const { return center; }
		EQuat		GetOrient		( void ) const { return orient; }
					
	protected:
		EVec3	extents;	// size of the box: width, height, depth
		EVec3	center;
		EQuat	orient;
	};
	
	
bool			OBBIntersection ( const EOrientedBBox	&a, const EOrientedBBox &b );
EOrientedBBox	OBBFromBBox		( const EBBox &box );