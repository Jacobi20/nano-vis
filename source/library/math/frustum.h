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

class	EFrustum {
	public:
					EFrustum		( void );
					~EFrustum		( void );
		
		void		SetFrustum		( float w, float h, float zf );
		void		SetPosition		( const EVec4 &pos );
		void		SetOrient		( const EQuat &q );
		
		EVec4		GetPosition		( void ) const { return pos; }
		EQuat		GetOrient		( void ) const { return orient; }
		
		bool		CullPoint		( const EVec4 &point ) const;
		bool		CullSphere		( const ESphere &sphere ) const;
		ESphere		GetCircumSphere	( void ) const;
	
	protected:
		void		Recompute	( void );
	
		float	w, h, zf;
		EVec4	pos;
		EQuat	orient;
		EPlane	right, left, top, bottom, far_plane;
	};