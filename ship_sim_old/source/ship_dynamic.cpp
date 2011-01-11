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
	Ship dynamic stuff :
-----------------------------------------------------------------------------*/
	
//
//	EShip::UpdateHDF
//		- updates HDFs using 'antiscientific' method
//
void EShip::UpdateHDF( float dtime, IPxWaving waving )
{
	if (ship_hsf_method->String()=="hxfse")	{
		return;
	}
	

	EVec4	position;
	EQuat	orient;
	GetPose(position, orient);

	for (uint i=0; i<mesh_hdf->GetTriangleNum(); i++) {
		
		EVec3 fe_pos		=	QuatRotateVector( mesh_hdf->TriangleCenter(i), orient ) + EVec3(position.Ptr());
		EVec3 fe_normal		=	QuatRotateVector( mesh_hdf->TriangleNormal(i), orient );
		float fe_area		=	mesh_hdf->TriangleArea(i);
		EVec3 fe_vel_global	=	Vec4ToVec3( ship_body->GetLocalPointVelocity( Vec3ToVec4(mesh_hdf->TriangleCenter(i)) ) );
		
		/*
		rs()->GetDVScene()->DrawArrow( Vec3ToVec4(fe_pos), Vec3ToVec4(fe_vel_global), 1, EVec4(0.5, 0.5, 1.0, 1.0));
		*/
		
		EVec4	p = EVec4(fe_pos.x, fe_pos.y, 0, 1);
		
		EVec3 flow_vel		=	Vec4ToVec3( waving->GetVelocity( p ) );
		float wave_h		=				waving->GetPosition( p ).z;		  
		
		if (wave_h < fe_pos.z) {	
			continue;
		}
		
		//	element velocity relative to flow :
		EVec3 fe_vel		=	fe_vel_global - flow_vel;
		
		//	cosine angle between velocity and normal :
		float cos_vel_norm		=	Vec3Dot( Vec3Normalize( fe_vel ), fe_normal );
		float cos_2_vel_norm	=	2 * (cos_vel_norm * cos_vel_norm) - 1;
		float cos_sign			=	1 ? -1 : cos_vel_norm > 0;

		//	0.1 - stupid viscosity addition		
		//	0.5 - just front or back pressure 
		//  sqrt(0.5) - because lifting force is maximum at PI/4
		const float	Cxx		=	water_resistance_cx;
		const float Cyy		=	0.0;
		float Cx			=	Cxx * 0.5 * (0.9 * abs(cos_vel_norm) + 0.1);	
		float Cy			=	Cyy * 0.5 * sqrt(0.5) * cos_2_vel_norm;
		
		float hd_force_x	=	Cx * Vec3LengthSqr( fe_vel ) * fe_area * WATER_DENSITY / 2;
		float hd_force_y	=	Cy * Vec3LengthSqr( fe_vel ) * fe_area * WATER_DENSITY / 2;
		
		//	compute vectors of force applying :
		EVec3 force_x_dir	=	Vec3Normalize( fe_vel ) * cos_sign;
		EVec3 force_y_dir	=	- Vec3Normalize( Vec3Cross( fe_vel, Vec3Cross( fe_normal, fe_vel ) ) ) * cos_sign;
		
		EVec3 vhd_force_x	=	force_x_dir * hd_force_x;
		EVec3 vhd_force_y	=	force_y_dir * hd_force_y;
		
		//	add force :
		EVec4	vhd_force	=	Vec3ToVec4( vhd_force_x + vhd_force_y );
		ship_body->AddForceAtPos( vhd_force, Vec3ToVec4(fe_pos) );
	}
}


