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
	BBox :
-----------------------------------------------------------------------------*/

EBBox::EBBox( void )
{
	MakeSingular();
}


EBBox::EBBox( const EBBox &other )
{
	pmin	=	other.pmin;
	pmax	=	other.pmax;
}


EBBox::EBBox( const EVec4 &p1, const EVec4 &p2 )
{
	MakeSingular();
	Expand( p1 );
	Expand( p2 );
}


EBBox::EBBox( const EVec4 &center, float szx, float szy, float szz )
{
	ASSERT( center.w );
	EVec4	pp = EVec4(center.x/center.w, center.y/center.w, center.z/center.w, 1);
	
	pmin	=	pp;
	pmax	=	pp;

	pmin.x	-=	szx/2.0f;
	pmin.y	-=	szy/2.0f;
	pmin.z	-=	szz/2.0f;

	pmax.x	+=	szx/2.0f;
	pmax.y	+=	szy/2.0f;
	pmax.z	+=	szz/2.0f;
}


EBBox::~EBBox( void )
{

}


void EBBox::MakeSingular( void )
{
	pmin	=	EVec4( MAX_FLOAT_VALUE, MAX_FLOAT_VALUE, MAX_FLOAT_VALUE, 1 );
	pmax	=	EVec4(-MAX_FLOAT_VALUE,-MAX_FLOAT_VALUE,-MAX_FLOAT_VALUE, 1 );
}


void EBBox::Expand( const EVec4 &p )
{
	ASSERT( p.w );
	EVec4	pp = EVec4(p.x/p.w, p.y/p.w, p.z/p.w, 1);
	if (pmin.x > pp.x) pmin.x = pp.x;
	if (pmin.y > pp.y) pmin.y = pp.y;
	if (pmin.z > pp.z) pmin.z = pp.z;

	if (pmax.x < pp.x) pmax.x = pp.x;
	if (pmax.y < pp.y) pmax.y = pp.y;
	if (pmax.z < pp.z) pmax.z = pp.z;
}


EVec4 EBBox::Min( void ) const
{
	return pmin;
}


EVec4 EBBox::Max( void ) const
{
	return pmax;
}


EVec4 EBBox::Center( void ) const 
{
	return  0.5f * (pmin + pmax);
}


bool EBBox::Contains( const EVec4 &p ) const
{
	ASSERT( p.w );
	EVec4	pp = EVec4(p.x/p.w, p.y/p.w, p.z/p.w, 1);
	if (pp.x > pmax.x) return false;
	if (pp.y > pmax.y) return false;
	if (pp.z > pmax.z) return false;
	if (pp.x < pmin.x) return false;
	if (pp.y < pmin.y) return false;
	if (pp.z < pmin.z) return false;
	return true;
}


bool EBBox::Intersects( const EBBox &other ) const
{
	return
		(pmin.x < other.pmax.x) && (pmax.x > other.pmin.x) &&
		(pmin.y < other.pmax.y) && (pmax.y > other.pmin.y) &&
		(pmin.z < other.pmax.z) && (pmax.z > other.pmin.z);
}


EVec3 EBBox::Size( void ) const
{
	EVec4 sz = pmax - pmin;
	return EVec3(sz.x, sz.y, sz.z);
}


/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/

//
//	BBoxAABBTest
//
bool BBoxAABBTest( const EBBox &a, const EBBox &b )
{
	return a.Intersects( b );
}


//
//	BBoxOBBTest
//
bool BBoxOBBTest( const EBBox &a, const EVec4 &ap, const EQuat &ao, const EBBox &b, const EVec4 &bp, const EQuat &bo )
{
	return true;
}

/*
bool ZCollision::OOBBCollision(ZOOBB &a,ZOOBB &b)
{
	Matrix4 matB = b.m_matRot*a.m_matRotInverse;
	Vector3 vPosB = a.m_matRotInverse * (b.m_vCenter - a.m_vCenter);

	EVec3 XAxis(matB._11,matB._21,matB._31);
	EVec3 YAxis(matB._12,matB._22,matB._32);
	EVec3 ZAxis(matB._13,matB._23,matB._33);

	//15 tests
	//1 (Ra)x
	if(fabs(vPosB.x) > a.m_vBounds.x + b.m_vBounds.x * fabs(XAxis.x) + b.m_vBounds.y * fabs(XAxis.y) + b.m_vBounds.z * fabs(XAxis.z))
		return false;
	//2 (Ra)y
	if(fabs(vPosB.y) > a.m_vBounds.y + b.m_vBounds.x * fabs(YAxis.x) + b.m_vBounds.y * fabs(YAxis.y) + b.m_vBounds.z * fabs(YAxis.z))
		return false;
	//3 (Ra)z
	if(fabs(vPosB.z) > a.m_vBounds.z + b.m_vBounds.x * fabs(ZAxis.x) + b.m_vBounds.y * fabs(ZAxis.y) + b.m_vBounds.z * fabs(ZAxis.z))
		return false;

	//4 (Rb)x
	if(fabs(vPosB.x*XAxis.x+vPosB.y*YAxis.x+vPosB.z*ZAxis.x) >
		(b.m_vBounds.x+a.m_vBounds.x*fabs(XAxis.x) + a.m_vBounds.y * fabs(YAxis.x) + a.m_vBounds.z*fabs(ZAxis.x)))
		return false;
	//5 (Rb)y
	if(fabs(vPosB.x*XAxis.y+vPosB.y*YAxis.y+vPosB.z*ZAxis.y) >
		(b.m_vBounds.y+a.m_vBounds.x*fabs(XAxis.y) + a.m_vBounds.y * fabs(YAxis.y) + a.m_vBounds.z*fabs(ZAxis.y)))
		return false;
	//6 (Rb)z
	if(fabs(vPosB.x*XAxis.z+vPosB.y*YAxis.z+vPosB.z*ZAxis.z) >
		(b.m_vBounds.z+a.m_vBounds.x*fabs(XAxis.z) + a.m_vBounds.y * fabs(YAxis.z) + a.m_vBounds.z*fabs(ZAxis.z)))
		return false;

	//7 (Ra)x X (Rb)x
	if(fabs(vPosB.z*YAxis.x-vPosB.y*ZAxis.x) > a.m_vBounds.y*fabs(ZAxis.x) + 
		a.m_vBounds.z*fabs(YAxis.x) + b.m_vBounds.y*fabs(XAxis.z) + b.m_vBounds.z*fabs(XAxis.y))
		return false;
	//8 (Ra)x X (Rb)y
	if(fabs(vPosB.z*YAxis.y-vPosB.y*ZAxis.y) > a.m_vBounds.y*fabs(ZAxis.y) + 
		a.m_vBounds.z*fabs(YAxis.y) + b.m_vBounds.x*fabs(XAxis.z) + b.m_vBounds.z*fabs(XAxis.x))
		return false;
	//9 (Ra)x X (Rb)z
	if(fabs(vPosB.z*YAxis.z-vPosB.y*ZAxis.z) > a.m_vBounds.y*fabs(ZAxis.z) + 
		a.m_vBounds.z*fabs(YAxis.z) + b.m_vBounds.x*fabs(XAxis.y) + b.m_vBounds.y*fabs(XAxis.x))
		return false;

	//10 (Ra)y X (Rb)x
	if(fabs(vPosB.x*ZAxis.x-vPosB.z*XAxis.x) > a.m_vBounds.x*fabs(ZAxis.x) + 
		a.m_vBounds.z*fabs(XAxis.x) + b.m_vBounds.y*fabs(YAxis.z) + b.m_vBounds.z*fabs(YAxis.y))
		return false;
	//11 (Ra)y X (Rb)y
	if(fabs(vPosB.x*ZAxis.y-vPosB.z*XAxis.y) > a.m_vBounds.x*fabs(ZAxis.y) + 
		a.m_vBounds.z*fabs(XAxis.y) + b.m_vBounds.x*fabs(YAxis.z) + b.m_vBounds.z*fabs(YAxis.x))
		return false;
	//12 (Ra)y X (Rb)z
	if(fabs(vPosB.x*ZAxis.z-vPosB.z*XAxis.z) > a.m_vBounds.x*fabs(ZAxis.z) + 
		a.m_vBounds.z*fabs(XAxis.z) + b.m_vBounds.x*fabs(YAxis.y) + b.m_vBounds.y*fabs(YAxis.x))
		return false;

	//13 (Ra)z X (Rb)x
	if(fabs(vPosB.y*XAxis.x-vPosB.x*YAxis.x) > a.m_vBounds.x*fabs(YAxis.x) + 
		a.m_vBounds.y*fabs(XAxis.x) + b.m_vBounds.y*fabs(ZAxis.z) + b.m_vBounds.z*fabs(ZAxis.y))
		return false;
	//14 (Ra)z X (Rb)y
	if(fabs(vPosB.y*XAxis.y-vPosB.x*YAxis.y) > a.m_vBounds.x*fabs(YAxis.y) + 
		a.m_vBounds.y*fabs(XAxis.y) + b.m_vBounds.x*fabs(ZAxis.z) + b.m_vBounds.z*fabs(ZAxis.x))
		return false;
	//15 (Ra)z X (Rb)z
	if(fabs(vPosB.y*XAxis.z-vPosB.x*YAxis.z) > a.m_vBounds.x*fabs(YAxis.z) + 
		a.m_vBounds.y*fabs(XAxis.z) + b.m_vBounds.x*fabs(ZAxis.y) + b.m_vBounds.y*fabs(ZAxis.x))
		return false;
	return true;
}

*/





