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

class	EBBox {
	public:
					EBBox		( void );
					EBBox		( const EBBox &other );
					EBBox		( const EVec4 &p1, const EVec4 &p2 );
					EBBox		( const EVec4 &center, float szx, float szy, float szz );
					~EBBox		( void );
					
		void		MakeSingular( void );
					
		void		Expand		( const EVec4 &p );
		EVec4		Min			( void ) const;
		EVec4		Max			( void ) const;
		EVec4		Center		( void ) const;
		EVec3		Size		( void ) const;

		bool		Contains	( const EVec4 &p ) const;		
		bool		Intersects	( const EBBox &other ) const;
		
	protected:
		EVec4		pmin;
		EVec4		pmax;
	};
	

bool	BBoxAABBTest( const EBBox &a, const EBBox &b );		
bool	BBoxOBBTest	( const EBBox &a, const EVec4 &ap, const EQuat &ao,	const EBBox &b, const EVec4 &bp, const EQuat &bo );
						