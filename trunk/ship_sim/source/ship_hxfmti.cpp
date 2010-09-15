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
#include "mersenne.h"

/*-----------------------------------------------------------------------------
	Ship hydrodynamic and hydrostatic microtriangle hull integration
	HXFMTI
-----------------------------------------------------------------------------*/

RandMT	mtrand;

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
	this->surf_elements_radius = gathering_radius;

	LOGF("building surface elements : %s", path.CStr());
	LOGF("density : %g", density);
	LOGF("radius  : %g", surf_elements_radius);
	
	if (density <= 0) {
		LOG_WARNING("EShip::BuildSurfaceDXDY(): density < 0");
		density = 0;
	}

	double	total_surface_area = 0;	
	double	total_surface_area2 = 0;	
	
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
		double	s	=	mesh->TriangleArea(i);

#if 0
		const uint TESS_FACTOR = 4; 		
		float d = 1.0f/(float)TESS_FACTOR;
		
		for (uint ui=0; ui<TESS_FACTOR; ui++) {
			for (uint vi=0; vi<TESS_FACTOR; vi++) {
			
				float u = ui * d;
				float v = vi * d;
				
				uint tri_num =	(1 + TESS_FACTOR) * (TESS_FACTOR) / 2 
							 +	(TESS_FACTOR) * (TESS_FACTOR - 1) / 2 ;
				
				
				EVec3	vn0	=	v0 + v01*(u+0) + v02*(v+0);
				EVec3	vn1	=	v0 + v01*(u+d) + v02*(v+0);
				EVec3	vn2	=	v0 + v01*(u+d) + v02*(v+d);
				EVec3	vn3	=	v0 + v01*(u+0) + v02*(v+d);
				
				EVec3	c0	=	(vn0 + vn1 + vn2) / 3.0f;
				EVec3	c1	=	(vn0 + vn2 + vn3) / 3.0f;
				
				if (ui+vi<TESS_FACTOR) {
					ESurfElem se0, se1;
					se0.position	= Vec3ToPoint4(c0);
					se1.position	= Vec3ToPoint4(c0);
					
					se0.normal		= Vec3ToVec4(n);
					se1.normal		= Vec3ToVec4(n);
					
					se0.area		= s/tri_num;
					se1.area		= s/tri_num;
					
					if (ui+vi != TESS_FACTOR) {
						surf_elements.push_back(se0);
						surf_elements.push_back(se1);
					} else {
						surf_elements.push_back(se0);
					}
				}
			}
		}
#else		
		total_surface_area += s;
		
		uint elem_num = GetNumPoints(s * density);
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
				u	=	mtrand.randomMTf();
				v	=	mtrand.randomMTf();
			} while (u+v>1);
			
			total_surface_area2 += ds;
			
			se.position	=	Vec3ToPoint4( v0 + v01 * u + v02 * v );
			se.normal	=	Vec3ToVec4  ( n );
			se.area		=	ds;
			surf_elements.push_back(se);
		} 
#endif		
	}
	
	BalanceHXFSE();
	LOGF("%f %f", total_surface_area, total_surface_area2);
	
	LOGF("done : %d surface elements", surf_elements.size());
}



//
//	EShip::BalanceHXFSEAxis
//
void EShip::BalanceHXFSEAxis( const EBBox &box, uint axis_id )
{
	uint table_axis0[3] = {0,1,2};
	uint table_axis1[3] = {1,2,0};
	uint table_axis2[3] = {2,0,1};
	uint ax0	=	table_axis0[ axis_id ];
	uint ax1	=	table_axis1[ axis_id ];
	uint ax2	=	table_axis2[ axis_id ];
	
	
}


void EShip::BalanceHXFSE( void )
{
	EBBox	bbox;
	
	for (uint i=0; i<surf_elements.size(); i++) {
		ESurfElem	se = surf_elements[i];
		
		bbox.Expand(se.position);
	}
	
	LOGF("balancing...");
	
	BalanceHXFSEAxis( bbox, 0 );
	
	return;
	
	float step = 3;
	
	bbox.Grow( step/2, step/2, step/2 );
	
	for (float x=bbox.Min().x; x<=bbox.Max().x; x+=step) {
		for (float y=bbox.Min().y; y<=bbox.Max().y; y+=step) {
			for (float z=bbox.Min().z; z<=bbox.Max().z; z+=step) {
			
				vector<ESurfElem*>	ses_x;
				vector<ESurfElem*>	ses_y;
				vector<ESurfElem*>	ses_z;
				
				for (uint i=0; i<surf_elements.size(); i++) {
					
					ESurfElem *se	= &surf_elements[i];
					EVec4		p	= se->position;
					
					if ( (p.x > x) && (p.x < x+step)  &&  (p.y > y) && (p.y < y+step) ) {
						ses_z.push_back( se );
					}
					
					if ( (p.y > y) && (p.y < y+step)  &&  (p.z > z) && (p.z < z+step) ) {
						ses_x.push_back( se );
					}
					
					if ( (p.z > z) && (p.z < z+step)  &&  (p.x > x) && (p.x < x+step) ) {
						ses_y.push_back( se );
					}

					EVec4	force_abs(0,0,0,0);
					EVec4	force(0,0,0,0);
					
					//	compute force deviation by X-axis :
					for (uint j=0; j<ses_x.size(); j++) {
						ESurfElem *se = ses_x[j];
						
						float f	=	se->area * se->normal.x;
						
						force.x += f;
						force_abs.x += abs(f);
					}
					
					//	compute force deviation by Y-axis :
					for (uint j=0; j<ses_y.size(); j++) {
						ESurfElem *se = ses_y[j];
						
						float f	=	se->area * se->normal.y;
						
						force.y += f;
						force_abs.y += abs(f);
					}
					
					//	compute force deviation by Z-axis :
					for (uint j=0; j<ses_z.size(); j++) {
						ESurfElem *se = ses_z[j];
						
						float f	=	se->area * se->normal.z;
						
						force.z += f;
						force_abs.z += abs(f);
					}
					
					//	Reduce error :
					
					//	reduce error by X-axis :
					for (uint j=0; j<ses_x.size(); j++) {
						ESurfElem *se = ses_x[j];
						se->normal.x += force.x / force_abs.x;
					}
					
					//	reduce error by Y-axis :
					for (uint j=0; j<ses_y.size(); j++) {
						ESurfElem *se = ses_y[j];
						se->normal.y += force.y / force_abs.y;
					}
					
					//	reduce error by Z-axis :
					for (uint j=0; j<ses_z.size(); j++) {
						ESurfElem *se = ses_z[j];
						se->normal.z += force.z / force_abs.z;
					}
				}
			
			}
		}
	}

	LOGF("done.");
}


//
//	EShip::UpdateHXFTessMicroTris
//
void EShip::UpdateHXFSE( float dtime, IPxWaving waving )
{
	if (surf_elements.empty()) {
		LOG_WARNING(__FUNCTION__"() : surface element buffer is empty");
		return;
	}
	
	EVec4 p;
	EQuat q;
	EMatrix4 t;
	
	GetPose(p, q);
	t = Matrix4FromPose( p, q );
	
	EVec4	tf, tm;	//	total force and total momentum

	float num_pnts[3] = {0,0,0};
	
	
	for (uint i=0; i<surf_elements.size(); i++) {
		
		ESurfElem	se		= surf_elements[i];
		EVec4		normal	= se.normal;

		//	transform surface element to world space :		
		se.position	=	Matrix4Transform( se.position, t );
		se.normal	=	Matrix4Transform( se.normal,   t );

		//	compute depth and force factor :
		float	factor	=	0;
		float	depth	=	-se.position.z;
		
		if (depth>=0) {
			factor = 1;
		}

		//	compute pressure and force :
		float	pr	=	GRAVITY * WATER_DENSITY * (-se.position.z);
		float	s	=	se.area;
		EVec4	f	=	- (se.normal * (pr * s)) * factor;

		//ship_body->AddForceAtPos( f, se.position );
		ship_body->AddForce( f );
		
		//	debug point :
		EVec4	color	=	Vec4Lerp( EVec4(1,0,0,1), EVec4(1,1,0,1), factor);
		rs()->GetDVScene()->DrawPoint( se.position, 0.1f, color );
		
		
		if (normal.x >  0.999f) { num_pnts[0] += factor; }
		if (normal.y >  0.999f) { num_pnts[1] += factor; }
		if (normal.z >  0.999f) { num_pnts[2] += factor; }
		if (normal.x < -0.999f) { num_pnts[0] -= factor; }
		if (normal.y < -0.999f) { num_pnts[1] -= factor; }
		if (normal.z < -0.999f) { num_pnts[2] -= factor; }

		//rs()->GetDVScene()->DrawArrow( se.position, se.normal, 0.5, EVec4(0.0, 1.0, 0.5, 1.0 ) );
	}
	
	//LOGF("%7.2f %7.2f %7.2f", num_pnts[0], num_pnts[1], num_pnts[2]);
	
	EVec4	force, torque;	
	ship_body->GetTotalForces( force, torque );
	//
	LOGF("force  (kN)   : %8.3f %8.3f %8.3f", force.x / 1000.0f, force.y / 1000.0f, force.z / 1000.0f);
	//LOGF("torque (kN*m) : %8.1f", Vec4Length(torque) / 1000.0f );
}