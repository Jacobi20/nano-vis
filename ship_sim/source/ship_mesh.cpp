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
	Ship mesh stuff :
-----------------------------------------------------------------------------*/

//
//	EShip::SetVisMesh
//
void EShip::SetVisMesh( const EString path )
{
	LOGF("view mesh : %s", path.CStr());
	mesh_vis		=	ge()->LoadMeshFromFile( path.CStr() );
	r_ent			=	sci_vis->GetFRScene()->AddEntity();
	
	r_ent->SetMesh( mesh_vis );
}


//
//	EShip::SetHSFMesh
//
void EShip::SetHSFMesh( const EString path )
{
	LOGF("HSF mesh  : %s", path.CStr());
	mesh_hsf	=	ge()->LoadMeshFromFile( path.CStr() );
	
	EBBox	bbox	=	mesh_hsf->ComputeBBox();
	ship_length		=	bbox.Size().x;
	ship_width		=	bbox.Size().y;
	ship_height		=	bbox.Size().z;
}


//
//	EShip::SetHDFMesh
//
void EShip::SetHDFMesh( const EString path )
{
	LOGF("HDF mesh  : %s", path.CStr());
	mesh_hdf	=	ge()->LoadMeshFromFile( path.CStr() );
}


//
//	EShip::MakeRigidBody
//
void EShip::MakeRigidBody( const EString path, float mass )
{
	LOGF("RB mesh   : %s", path.CStr());
	IPxTriMesh phys_mesh	=	ge()->LoadMeshFromFile( path.CStr() );
	
	//if (ship_body) {
	//	sci_vis->GetNxScene()->releaseActor( *ship_body );
	//}
	//
	//ship_body	=	sci_vis->CreatePhysMesh( phys_mesh, EVec4(0,0,0,1), EQuat(0,0,0,1), mass );
	//ship_mass	=	ship_body->getMass();
	ship_body	=	phys()->AddEntity();
	
	ship_body->SetMesh( phys_mesh );
	ship_body->SetMass( mass );
	ship_body->SetPose( EVec4(0,0,0,1), EQuat(0,0,0,1) );
	ship_body->SetDynamic();
	
	ship_body->Spawn();
	
	ship_mass	=	mass;
}


//
//	EShip::BuildVoxels
//	
void EShip::BuildVoxels( const EString path, float cube_size )
{
	LOGF("building voxels for : %s", path.CStr());
	LOGF("voxel size : %g", cube_size);
	voxel_grid	=	new EVoxelGrid();
	
	IPxTriMesh	mesh	=	ge()->LoadMeshFromFile( path.CStr() );

	try {	
		voxel_grid->BuildGrid( mesh, EVec3(0,0,0), cube_size, cube_size, cube_size );
	} catch (exception &e) {
		LOG_WARNING("%s", e.what());
	}
	
	LOGF("done : %d voxels", voxel_grid->GetVoxelNum());
}

