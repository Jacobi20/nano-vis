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


#include "library.h"

/*-----------------------------------------------------------------------------
	Plane stuff declaration :
-----------------------------------------------------------------------------*/

//
//	PlaneFromPointNormal
//
EPlane PlaneFromPointNormal( const EVec4 &point, const EVec4 &normal )
{
	ASSERT(point.w!=0);
	ASSERT(normal.w==0);
	
	EVec4 norm_normal = Vec4Normalize( normal );

	float a = norm_normal.x;
	float b = norm_normal.y;
	float c = norm_normal.z;
	
	float d = - (a * point.x  +  b * point.y  +  c * point.z) / point.w;
	return EPlane(a,b,c,d);
}


//
//	PlaneFromPoints
//
EPlane PlaneFromPoints( const EVec4 &p0, const EVec4 &p1, const EVec4 &p2 )
{
	EVec4	v0	=	p1 - p0;
	EVec4	v1	=	p2 - p0;
	EVec3	n	=	Vec3Cross( EVec3(v0.x, v0.y, v0.z), EVec3(v1.x, v1.y, v1.z) );
	
	return PlaneFromPointNormal(p0, EVec4(n.x, n.y, n.z, 0));
}


//
//	PlaneIntersection
//
bool PlaneIntersection( EVec4 &out_point, const EPlane &p0, const EPlane &p1, const EPlane &p2 )
{
	EMatrix3	d	(
			p0.a,  p0.b,  p0.c,
			p1.a,  p1.b,  p1.c,
			p2.a,  p2.b,  p2.c
		);

	EMatrix3	dx	(
			-p0.d,  p0.b,  p0.c,
			-p1.d,  p1.b,  p1.c,
			-p2.d,  p2.b,  p2.c
		);

	EMatrix3	dy	(
			p0.a,  -p0.d,  p0.c,
			p1.a,  -p1.d,  p1.c,
			p2.a,  -p2.d,  p2.c
		);

	EMatrix3	dz	(
			p0.a,  p0.b,  -p0.d,
			p1.a,  p1.b,  -p1.d,
			p2.a,  p2.b,  -p2.d
		);
		
	float det   = Matrix3Det( d );

	if (det==0) {
		return false;	//	no solution
	}

	float det_x = Matrix3Det( dx );
	float det_y = Matrix3Det( dy );
	float det_z = Matrix3Det( dz );
		
	
	out_point.x = det_x / det;
	out_point.y = det_y / det;
	out_point.z = det_z / det;
	out_point.w = 1;
	
	return true;
}


//
//	PlaneClassify
//
int PlaneClassify( const EPlane &plane, const EVec4 &point )
{
	EVec4 pv(plane.a, plane.b, plane.c, plane.d);
	float dot = Vec4Dot(pv, point);
	if (dot<0)	return -1;
	if (dot>0)	return +1;
	return 0;
}


//
//	PlaneDistance
//
float PlaneDistance( const EPlane &plane, const EVec4 &point )
{
	float a = plane.a;
	float b = plane.b;
	float c = plane.c;
	float d = plane.d;
	
	float length = sqrt(a*a + b*b + c*c);
	
	a /= length;
	b /= length;
	c /= length;
	d /= length;
	
	EVec4 pv(a, b, c, d);
	
	return Vec4Dot(pv, point);
}


//
//	PlaneIsPointEnclosed
//
bool PlaneIsPointEnclosed( const EVec4 &point, uint num_planes, const EPlane *planes, float point_normal_offset )
{
	for (uint i=0; i<num_planes; i++) {
		float dist = PlaneDistance(planes[i], point);
		dist += point_normal_offset;
		
		if (dist>0) {
			return false;
		}
	}
	
	return true;
}





