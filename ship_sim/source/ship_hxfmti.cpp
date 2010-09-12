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

#include "sci_local.h"
#include "ship.h"

/*-----------------------------------------------------------------------------
	Ship hydrodynamic and hydrostatic microtriangle hull integration
	HXFMTI
-----------------------------------------------------------------------------*/

//
//	EShip::BuildSurfaceElements
//
void EShip::BuildSurfaceDXDY( const EString path, float density )
{
	LOGF("building surface elements : %s", path.CStr());
	LOGF("density : %g", density);
	
	if (density <= 0) {
		LOG_WARNING("EShip::BuildSurfaceDXDY(): density < 0");
		density = 0;
	}

	float	total_surface_area = 0;	
	
	IPxTriMesh	mesh	=	ge()->LoadMeshFromFile( path.CStr() );

	for (uint i=0; i<mesh->GetTriangleNum(); i++) {
	
		uint i0, i1, i2;
		mesh->GetTriangle(i, i0, i1, i2);
		
		EVec3	v0	=	mesh->GetVertex(i0).position;
		EVec3	v1	=	mesh->GetVertex(i1).position;
		EVec3	v2	=	mesh->GetVertex(i2).position;
		EVec3	v01	=	v1 - v0;	//	u
		EVec3	v02	=	v2 - v0;	//	v

		EVec3	n	=	mesh->TriangleNormal(i);
		float	s	=	mesh->TriangleArea(i);
		
		total_surface_area += s;
		
		uint elem_num = (uint)(s * density);
		
		float	ds = s / elem_num;
		
		for (uint j=0; j<elem_num; j++) {
		
			ESurfElem	se;
			
			float u=0, v=0;
			do {
				u	=	FRand(0,1);
				v	=	FRand(0,1);
			} while (u+v>1);
			
			se.position	=	Vec3ToPoint4( v0 + v01 * u + v02 * v );
			se.normal	=	Vec3ToVec4  ( n );
			se.area		=	ds;
		
			surf_elements_local.push_back(se);
		}
	}
	
	LOGF("done : %d surface elements", surf_elements_local.size());
}


//
//	EShip::UpdateHXFTessMicroTris
//
void EShip::UpdateHXFSE( float dtime, IPxWaving waving )
{
	if (surf_elements_local.empty()) {
		LOG_WARNING(__FUNCTION__"() : surface element buffer is empty");
		return;
	}
	
	EVec4 p;
	EQuat q;
	EMatrix4 t;
	
	GetPose(p, q);
	t = Matrix4FromPose( p, q );
	
	for (uint i=0; i<surf_elements_local.size(); i++) {
		
		ESurfElem	se = surf_elements_local[i];
		
		se.position	=	Matrix4Transform( se.position, t );
		se.normal	=	Matrix4Transform( se.normal,   t );

		
		float depth =	-se.position.z;
		
		if (depth<0) {
			continue;
		}
		
		float	p	=	GRAVITY * WATER_DENSITY * (-se.position.z);
		float	s	=	se.area;
		EVec4	f	=	- (se.normal * (p * s));
		
		rs()->GetDVScene()->DrawPoint( se.position, 0.1, EVec4(0.0, 1.0, 0.5, 1.0 ) );
		
		ship_body->AddForceAtPos( f, se.position );
		
		//rs()->GetDVScene()->DrawArrow( se.position, se.normal, 0.5, EVec4(0.0, 1.0, 0.5, 1.0 ) );
	}
	
}