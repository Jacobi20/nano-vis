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

#if 0 		
		float d = 1.0f/16.0f;
		for (uint ui=0; ui<16; ui++) {
			for (uint vi=0; vi<16; vi++) {
			
				float u = ui * d;
				float v = vi * d;
				
				EVec3	vn0	=	v0 + v01*(u+0) + v02*(v+0);
				EVec3	vn1	=	v0 + v01*(u+d) + v02*(v+0);
				EVec3	vn2	=	v0 + v01*(u+d) + v02*(v+d);
				EVec3	vn3	=	v0 + v01*(u+0) + v02*(v+d);
				
				EVec3	c0	=	(vn0 + vn1 + vn2) / 3.0f;
				EVec3	c1	=	(vn0 + vn2 + vn3) / 3.0f;
				
				if (ui+vi<16) {
					ESurfElem se0, se1;
					se0.position	= Vec3ToPoint4(c0);
					se1.position	= Vec3ToPoint4(c1);
					
					se0.normal		= Vec3ToVec4(n);
					se1.normal		= Vec3ToVec4(n);
					
					se0.area		= s/36;
					se1.area		= s/36;
					
					if (ui+vi != 256) {
						surf_elements_local.push_back(se0);
						surf_elements_local.push_back(se1);
					} else {
						surf_elements_local.push_back(se0);
					}
				}
			}
		}
#else		
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
#endif		
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
	
	EVec4	tf, tm;	//	total force and total momentum

	
	
	for (uint i=0; i<surf_elements_local.size(); i++) {
		
		ESurfElem	se = surf_elements_local[i];
		
		se.position	=	Matrix4Transform( se.position, t );
		se.normal	=	Matrix4Transform( se.normal,   t );

		
		float depth =	-se.position.z;
		
		if (depth<0) {
			rs()->GetDVScene()->DrawPoint( se.position, 0.1f, EVec4(1, 0, 0, 1 ) );
			continue;
		}
		
		float	pr	=	GRAVITY * WATER_DENSITY * (-se.position.z);
		float	s	=	se.area;
		EVec4	f	=	- (se.normal * (pr * s));
		
		f.x = 0;
		f.y = 0;
		
		rs()->GetDVScene()->DrawPoint( se.position, 0.1f, EVec4(0, 1, 0, 1 ) );
		
		ship_body->AddForceAtPos( f, se.position );
		
		EVec4	m	=	Vec4Cross3( se.position - p, f );
		
		tf	+=	f;
		tm	+=	m;
		
		//rs()->GetDVScene()->DrawArrow( se.position, se.normal, 0.5, EVec4(0.0, 1.0, 0.5, 1.0 ) );
	}
	
	rs()->GetDVScene()->DrawArrow( p, tf, 0.0000001f, EVec4(1, 0, 0, 1 ) );
	rs()->GetDVScene()->DrawArrow( p, tm, 0.0000001f, EVec4(1, 0, 1, 1 ) );
}