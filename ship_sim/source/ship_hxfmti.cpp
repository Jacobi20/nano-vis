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


static uint GetNumPoints( float num ) {
	if (num<=1) {
		return 1;
	} else {
		return num;
	}
	//	POT numbers :
	if ( num>512)	return 512	;
	if ( num>256)	return 256	;
	if ( num>128)	return 128	;
	if ( num>64	)	return 64	;
	if ( num>32	)	return 32	;
	if ( num>16	)	return 16	;
	if ( num>8	)	return 8	;
	if ( num>4	)	return 4	;
	if ( num>2	)	return 2	;
	if ( num>1	)	return 1	;
	return 1;
}




//
//	EShip::BuildSurfaceElements
//
void EShip::BuildSurfaceDXDY( const EString path, float density, float gathering_radius )
{
	hxfgrid.path		=	path;
	hxfgrid.density		=	density;
	hxfgrid.radius		=	gathering_radius;
}


//
//	EShip::UpdateHXFSEGrid
//
void EShip::UpdateHXFSEGrid( void )
{
	hxfgrid.grid.reserve( hxfgrid.grid.size() + 1 );
	hxfgrid.grid.clear();

	if (hxfgrid.density <= 0) {
		LOG_WARNING("EShip::BuildSurfaceDXDY(): density < 0");
		hxfgrid.density = 0;
	}

	double	total_surface_area = 0;	
	double	total_surface_area2 = 0;	
	
	IPxTriMesh	mesh	=	ge()->LoadMeshFromFile( hxfgrid.path.CStr() );

	for (uint i=0; i<mesh->GetTriangleNum(); i++) {
	
		uint i0, i1, i2;
		mesh->GetTriangle(i, i0, i1, i2);
		
		EVec3	v0	=	mesh->GetVertex(i0).position;
		EVec3	v1	=	mesh->GetVertex(i1).position;
		EVec3	v2	=	mesh->GetVertex(i2).position;
		EVec3	v01	=	v1 - v0;	//	u
		EVec3	v02	=	v2 - v0;	//	v
		

		EVec3	n	=	mesh->TriangleNormal(i);
		double	s	=	mesh->TriangleArea(i);
		total_surface_area += s;
		
		uint elem_num = GetNumPoints(s * hxfgrid.density);
		if (elem_num<1) {
			elem_num = 1;
		}
		
		double	ds = s / elem_num;
		
		for (uint j=0; j<elem_num; j++) {
			ESurfElem	se;
			
			float u=0, v=0;
			do {
				u	=	FRand(0,1);
				v	=	FRand(0,1);
				u	=	hxfgrid.mtrand.randomMTf();
				v	=	hxfgrid.mtrand.randomMTf();
			} while (u+v>1);
			
			total_surface_area2 += ds;
			
			se.position	=	Vec3ToPoint4( v0 + v01 * u + v02 * v );
			se.normal	=	Vec3ToVec4  ( n );
			se.area		=	ds;
			hxfgrid.grid.push_back(se);
		} 
	}
}


//
//	EShip::UpdateHXFSE
//
void EShip::UpdateHXFSE( float dtime, IPxWaving waving )
{
	UpdateHXFSEGrid();
	
	EVec4 p;
	EQuat q;
	EMatrix4 t;
	
	GetPose(p, q);
	t = Matrix4FromPose( p, q );
	
	EVec4	tf, tm;	//	total force and total momentum

	float num_pnts[3] = {0,0,0};
	
	
	for (uint i=0; i<hxfgrid.grid.size(); i++) {
		
		ESurfElem	se		= hxfgrid.grid[i];
		EVec4		normal	= se.normal;
		
		EVec4		vse		= ship_body->GetLocalPointVelocity( se.position );	//	local point velocity
		EVec4		vwtr	= EVec4(0,0,0,0);									//	water velocity
		EVec4		vel		= vse - vwtr;
		float		vel_p2	= Vec4LengthSqr( vel );								//	squared point velocity relative to water particles
		EVec4		vdir	= Vec4Normalize( vel );

		//	transform surface element to world space :		
		se.position	=	Matrix4Transform( se.position, t );
		se.normal	=	Matrix4Transform( se.normal,   t );

		//	compute pressure and force :
		float	pr	=	waving->GetPressure( se.position );
		float	s	=	se.area;
		EVec4	f	=	- (se.normal * (pr * s));

		float	factor	=	pr > 0 ? 1 : 0;
		
		//	compute dynamic pressure :
		float	v_dot_n	=	Vec4Dot( vdir, se.normal );
		float	fds		=	water_resistance_cx * vel_p2 / 2 * abs(v_dot_n) * se.area * WATER_DENSITY * factor;
		EVec4	fd		=	- (vdir * fds);


		ship_body->AddForceAtPos( f + fd, se.position );
		
		//	debug point :
		//EVec4	color	=	Vec4Lerp( EVec4(0,0,1,1), EVec4(1,1,0,1), factor);
		//rs()->GetDVScene()->DrawPoint( se.position, 0.1f, color );
	}
	
	//LOGF("%7.2f %7.2f %7.2f", num_pnts[0], num_pnts[1], num_pnts[2]);
	
	//
	//	damp wrong
	//
	EVec4	force, torque;	
	ship_body->GetTotalForces( force, torque );

	//LOGF("force  (kN)   : %8.3f %8.3f %8.3f", force.x / 1000.0f, force.y / 1000.0f, force.z / 1000.0f);
}