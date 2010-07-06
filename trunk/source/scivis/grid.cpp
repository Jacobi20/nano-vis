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

/*-----------------------------------------------------------------------------
	grid for voxelization
-----------------------------------------------------------------------------*/

//
//	EVoxelGrid::EVoxelGrid
//
EVoxelGrid::EVoxelGrid( void )
{
	Clear();
}


//
//	EVoxelGrid::~EVoxelGrid
//
EVoxelGrid::~EVoxelGrid( void )
{

}


//
//	EVoxelGrid::BuildGrid
//
void EVoxelGrid::BuildGrid( const IPxTriMesh mesh, EVec3 origin, float step_x, float step_y, float step_z )
{
	bbox	=	mesh->ComputeBBox();

	Fill( mesh, origin, step_x, step_y, step_z, 0,0,0);
}


//
//	EVoxelGrid::Fill
//
void EVoxelGrid::Fill( const IPxTriMesh mesh, EVec3 origin, float szx, float szy, float szz, int idx, int idy, int idz )
{
	EVoxel voxel;
	voxel.center	=	origin;
	voxel.idx		=	idx;
	voxel.idy		=	idy;
	voxel.idz		=	idz;
	voxel.szx		=	szx;
	voxel.szy		=	szy;
	voxel.szz		=	szz;
	
	AddVoxel( voxel );
	
	if (!bbox.Contains( EVec4( origin.x, origin.y, origin.z, 1 ) )) {
		RAISE_EXCEPTION("mesh has holes or origin point is outside of closed mesh");
	}

	EVec3	dir;	
	if ( Grow(mesh, origin, szx, szy, szz,  idx, idy, idz, -1,  0,  0, dir) ) {	Fill( mesh, origin + dir, szx, szy, szz,  idx-1, idy  , idz  );	}
	if ( Grow(mesh, origin, szx, szy, szz,  idx, idy, idz, +1,  0,  0, dir) ) {	Fill( mesh, origin + dir, szx, szy, szz,  idx+1, idy  , idz  );	}
	if ( Grow(mesh, origin, szx, szy, szz,  idx, idy, idz,  0, -1,  0, dir) ) {	Fill( mesh, origin + dir, szx, szy, szz,  idx  , idy-1, idz  );	}
	if ( Grow(mesh, origin, szx, szy, szz,  idx, idy, idz,  0, +1,  0, dir) ) {	Fill( mesh, origin + dir, szx, szy, szz,  idx  , idy+1, idz  );	}
	if ( Grow(mesh, origin, szx, szy, szz,  idx, idy, idz,  0,  0, -1, dir) ) {	Fill( mesh, origin + dir, szx, szy, szz,  idx  , idy  , idz-1);	}
	if ( Grow(mesh, origin, szx, szy, szz,  idx, idy, idz,  0,  0, +1, dir) ) {	Fill( mesh, origin + dir, szx, szy, szz,  idx  , idy  , idz+1);	}
}


//
//	EVoxelGrid::Grow
//
bool EVoxelGrid::Grow( const IPxTriMesh mesh, EVec3 origin, float szx, float szy, float szz, int idx, int idy, int idz, int dx, int dy, int dz, EVec3 &dir )
{
	EVoxel vx;
	
	if (GetVoxel(idx+dx, idy+dy, idz+dz, vx)) {
		return false;
	}

	dir	=	EVec3( szx * dx, szy * dy, szz * dz );
	
	EVertex v;
	mesh.Get();
	bool r = mesh->RayIntersect( origin, dir, v );

	if (r) {	//	intersection - no grow
		return false;
	}	
	
	return true;
}


//
//	EVoxelGrid::Clear
//
void EVoxelGrid::Clear( void )
{
	grid.clear();
}


//
//	EVoxelGrid::GetVoxel
//
void EVoxelGrid::GetVoxel( uint index, EVoxel &voxel ) const
{
	ASSERT( index < GetVoxelNum() );
	voxel = grid[index];
}


//
//	EVoxelGrid::GetVoxel
//
bool EVoxelGrid::GetVoxel( int idx, int idy, int idz, EVoxel &voxel )
{
	uint n = GetVoxelNum();
	for (uint i=0; i<n; i++) {
		GetVoxel( i, voxel );
		
		if (voxel.idx==idx && voxel.idy==idy && voxel.idz==idz) {
			return true;
		}
	}
	
	return false;
}


//
//	EVoxelGrid::SetVoxel
//
void EVoxelGrid::SetVoxel( uint index, const EVoxel &voxel )
{
	ASSERT(0);
	ASSERT( index < GetVoxelNum() );
	grid[index] = voxel;
}


//
//	EVoxelGrid::AddVoxel
//
void EVoxelGrid::AddVoxel( const EVoxel &voxel )
{
	grid.push_back( voxel );
}

