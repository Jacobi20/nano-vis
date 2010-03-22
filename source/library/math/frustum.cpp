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
#include "frustum.h"

/*-----------------------------------------------------------------------------
	Frustum :
-----------------------------------------------------------------------------*/

EFrustum::EFrustum( void )
{
	SetFrustum(1,1,1);
	SetPosition( EVec4(0,0,0,1) );
	SetOrient( QuatIdentity() );
}


EFrustum::~EFrustum( void )
{

}


void EFrustum::SetFrustum( float w, float h, float zf )
{
	this->w		=	w;
	this->h		=	h;
	this->zf	=	zf;
	Recompute();
}


void EFrustum::SetPosition( const EVec4 &pos )
{
	this->pos	=	pos;
	Recompute();
}


void EFrustum::SetOrient( const EQuat &q )
{
	this->orient	=	q;
	Recompute();
}


/*-----------------------------------------------------------------------------
	Culling stuff :
-----------------------------------------------------------------------------*/

//
//	EFrustum::Recompute
//	frustum directed along local X-axis
//
void EFrustum::Recompute( void )
{
	EMatrix4	T	=	QuatToMatrix( orient ) * Matrix4Translate(pos);
//	EMatrix4	T	=	Matrix4Translate(pos) * QuatToMatrix( orient );

	EVec4	top	=	EVec4(0, 0, 0, 1);
	EVec4	tr	=	EVec4(  w/2.0f, h/2.0f, -zf, 1 );
	EVec4	tl	=	EVec4( -w/2.0f, h/2.0f, -zf, 1 );
	EVec4	br	=	EVec4(  w/2.0f,-h/2.0f, -zf, 1 );
	EVec4	bl	=	EVec4( -w/2.0f,-h/2.0f, -zf, 1 );
	
	top	=	Matrix4Transform( top	, T );	
	tr	=	Matrix4Transform( tr	, T );	
	tl	=	Matrix4Transform( tl	, T );	
	br	=	Matrix4Transform( br	, T );	
	bl	=	Matrix4Transform( bl	, T );	

	// CCW winding :
	this->right		=	PlaneFromPoints( top, br, tr );	
	this->left		=	PlaneFromPoints( top, tl, bl );
	this->top		=	PlaneFromPoints( top, tr, tl );
	this->bottom	=	PlaneFromPoints( top, bl, br );
	this->far_plane	=	PlaneFromPoints( tr, bl, tl );
}


//
//	EFrustum::CullPoint
//
bool EFrustum::CullPoint( const EVec4 &point ) const
{
	if ( PlaneDistance( right		, point ) > 0 ) 	return true;
	if ( PlaneDistance( left		, point ) > 0 ) 	return true;
	if ( PlaneDistance( top			, point ) > 0 ) 	return true;
	if ( PlaneDistance( bottom		, point ) > 0 ) 	return true;
	if ( PlaneDistance( far_plane	, point ) > 0 ) 	return true;
	return false;
}


//
//	EFrustum::CullSphere
//
bool EFrustum::CullSphere( const ESphere &sphere ) const
{
	EVec4	center	=	sphere.GetPosition();
	float	radius	=	sphere.GetRadius();

	if ( PlaneDistance( right		, center ) > radius) 	return true;
	if ( PlaneDistance( left		, center ) > radius) 	return true;
	if ( PlaneDistance( top			, center ) > radius) 	return true;
	if ( PlaneDistance( bottom		, center ) > radius) 	return true;
	if ( PlaneDistance( far_plane	, center ) > radius) 	return true;
	return false;
}


//
//	EFrustum::GetCircumSphere
//	see: wiki
//
ESphere EFrustum::GetCircumSphere( void ) const
{
	EVec4	top	=	EVec4(0, 0, 0, 1);
	EVec4	tr	=	EVec4(  w/2.0f, h/2.0f, -zf, 1 );
	EVec4	tl	=	EVec4( -w/2.0f, h/2.0f, -zf, 1 );
	EVec4	br	=	EVec4(  w/2.0f,-h/2.0f, -zf, 1 );
	EVec4	bl	=	EVec4( -w/2.0f,-h/2.0f, -zf, 1 );

	EVec4	dir	=	0.5f * ((tr - top) + (tl - top));
	float	h	=	Vec4Length( dir );
	
	dir			=	Vec4Normalize( dir );
	
	float	a	=	Vec4Length( tr - top );
	float	b	=	a;
	float	c	=	Vec4Length( tr - tl );
	
	float	S	=	0.5 * h * c;
	float	R	=	a*b*c / 4 / S;
	
	EVec4	center	=	top + dir * R;

	return ESphere(center, R);
}