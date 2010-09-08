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

#undef fs
 
/*-----------------------------------------------------------------------------
	Ship static stuff :
-----------------------------------------------------------------------------*/

//
//	EShip::GetShipXOZPlane
//
EPlane EShip::GetShipXOZPlane( void )
{
	float	yaw, pitch, roll;
	EQuat	orient;
	EVec4	position;
	
	GetPose(position, orient);
	QuatToAnglesRad( orient, yaw, pitch, roll );

	//	TODO : Fix, CM is not coincide with center of the local coordibates	
	EVec4	cmofs	=	ship_body->GetCMOffset();
	EVec4	cm		=	position + cmofs;	
	return 	PlaneFromPointNormal( cm, Vec3Cross( EVec3(cos(yaw), sin(yaw), 0), EVec3(0,0,1) ) );
}


static float StaticWaveForce( EVec4 pos, float szx, float szy, float szz, float wave_height ) {
	float wh = wave_height;		//	wave height

	float frac = (wh - (pos.z - 0.5f*szz)) / szz;
	frac = Clamp<float>(frac, 0,1);
	
	return frac * szx * szy * szz * WATER_DENSITY * GRAVITY;
}



//
//	EShip::UpdateHSF
//
void EShip::UpdateHSF( float dtime, IPxWaving waving )
{
	if (ship_hsf_method->String()=="box")		UpdateHSFBox	( dtime, waving ); else
	if (ship_hsf_method->String()=="voxel")		UpdateHSFVoxel	( dtime, waving ); else
	if (ship_hsf_method->String()=="surface")	UpdateHSFSurface( dtime, waving ); else 
	{
		LOG_WARNING("'%s' unknown HSF method", ship_hsf_method->String().CStr());
	}
}


//
//	EShip::UpdateHSFBox
//		- computes HSFs using block LxWxH 
//
void EShip::UpdateHSFBox(float dtime, IPxWaving waving)
{
	EVec4	position;
	EQuat	orient;
	GetPose(position, orient);

	uint FEM_X	=	20;
	uint FEM_Y	=	4;
	uint FEM_Z	=	4;
	
	float dx = ship_length / FEM_X;
	float dy = ship_width  / FEM_Y;
	float dz = ship_height / FEM_Z;
	float dm = 1.0f / FEM_X / FEM_Y / FEM_Z;

	NxVec3 fem_weight	=	NxVec3( 0, 0, -GRAVITY) * ship_mass * dm;
	
	EVec3	M = EVec3(0,0,0);
	
	for (uint i=0; i<FEM_X; i++) {
		for (uint j=0; j<FEM_Y; j++) {
			for (uint k=0; k<FEM_Z; k++) {

				float x = -(0.5 * ship_length) + 0.5*dx + dx * (float)i;
				float y = -(0.5 * ship_width ) + 0.5*dy + dy * (float)j;
				float z = -(0.5 * ship_height) + 0.5*dz + dz * (float)k;

				EVec4  pos(x,y,z,0);
				
				pos =  QuatRotateVector(pos, orient);
				pos += position;			
				
				float	wh = waving->GetPosition(EVec4(pos.x, pos.y, pos.z, 1)).z;
				float	fs = StaticWaveForce(pos, dx, dy, dz, wh);					//	static force
				
				ship_body->AddForceAtPos( EVec4(0,0,fs,0), pos );
			}
		}
	}
}


//
//	EShip::UpdateHSFVoxel
//		- computes HSFs using voxels
//
void EShip::UpdateHSFVoxel( float dtime, IPxWaving waving )
{
	EVec3	center_of_buyoancy	 =	EVec3(0,0,0);
	total_hsf_force	=	0;

	EVec4	position;
	EQuat	orient;
	GetPose(position, orient);
	
	EPlane	ship_plane	=	GetShipXOZPlane();
	

	for (uint i=0; i<voxel_grid->GetVoxelNum(); i++) {
		
		EVoxel vx;
		voxel_grid->GetVoxel(i, vx);
	
		EVec4  pos(vx.center.x, vx.center.y, vx.center.z, 0);
		EVec3  pos3(vx.center.x, vx.center.y, vx.center.z);
		float dx = vx.szx;
		float dy = vx.szy;
		float dz = vx.szz;
		
		pos =  QuatRotateVector(pos, orient);
		pos += position;			
		
		float	wh = waving->GetPosition(EVec4(pos.x, pos.y, pos.z, 1)).z;
		float	fs = StaticWaveForce(pos, dx, dy, dz, wh);					//	static force
		
		ship_body->AddForceAtPos( EVec4(0,0,fs,0), pos );

		//	compute total forces :
		total_hsf_force		+=	fs;
		center_of_buyoancy	+=	(fs * EVec3(pos.x, pos.y, pos.z));
	}

	center_of_buyoancy	/=	total_hsf_force;

	EVec3 b		=	center_of_buyoancy;
	right_arm	=	PlaneDistance(ship_plane, EVec4(b.x, b.y, b.z, 1));
}


EVec3 GetCenterOfBuyoancy( const IPxTriMesh mesh ) ;


//
//	EShip::UpdateHSFSurface
//		- computes HSFs using hull surface pressure integration
//
void EShip::UpdateHSFSurface( float dtime, IPxWaving waving )
{
	EVec3	center_of_buyoancy	 =	EVec3(0,0,0);
	total_hsf_force	=	0;
	

	EVec4	position;
	EQuat	orient;
	
	GetPose(position, orient);

	//	computing ship plane :	
	EPlane	ship_plane	=	GetShipXOZPlane();
	

	for (uint i=0; i<mesh_submerged_hsf->GetTriangleNum(); i++) {
		uint i0, i1, i2;
		mesh_submerged_hsf->GetTriangle( i, i0, i1, i2 );

		//	vertices :		
		EVec3	v0	=	mesh_submerged_hsf->GetVertex( i0 ).position;
		EVec3	v1	=	mesh_submerged_hsf->GetVertex( i1 ).position;
		EVec3	v2	=	mesh_submerged_hsf->GetVertex( i2 ).position;

		//	pressure :		
		float	p0	=	GRAVITY * WATER_DENSITY * (-v0.z);
		float	p1	=	GRAVITY * WATER_DENSITY * (-v1.z);
		float	p2	=	GRAVITY * WATER_DENSITY * (-v2.z);
		
		//	average pressure :
		float	pa	=	( p0 + p1 + p2 ) / 3;	
		
		//if (pa<0.001) { 
		//	continue;
		//}
		
		//	point of force application :
		float	cu	=	(p0 + 2*p1 + p2) / 4 / (p0 + p1 + p2);
		float	cv	=	(p0 + p1 + 2*p2) / 4 / (p0 + p1 + p2);
		
		EVec3	c	=	(v1 - v0) * cu + (v2 - v0) * cv + v0;
		float	s	=	mesh_submerged_hsf->TriangleArea(i);
		EVec3	n	=	mesh_submerged_hsf->TriangleNormal(i);
		
		//	absolute HSF 
		float	f	=	s * pa;

		//	force vector :		
		EVec3	fv	=	EVec3( -f*n.x, -f*n.y, -f*n.z );
		
		ship_body->AddForceAtPos( Vec3ToVec4(fv), Vec3ToPoint4(c) );

		/*
		rs()->GetDVScene()->DrawArrow( Vec3ToPoint4(c), Vec3ToVec4(fv), 0.000003, EVec4(1,0,0,1) );	 
		*/

		//	computing right arm :
		center_of_buyoancy	+=	(fv.z * c);
		total_hsf_force		+=	fv.z;
		
	}
	
	center_of_buyoancy	/=	total_hsf_force;

	EVec3 b		=	center_of_buyoancy;
	right_arm	=	PlaneDistance(ship_plane, EVec4(b.x, b.y, b.z, 1));
}

