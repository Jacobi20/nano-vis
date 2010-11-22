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
#include <queue>

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
	
	bbox.Grow(1,1,1);

	float szx = step_x;
	float szy = step_y;
	float szz = step_z;
	
#if 1

	int min_idx = bbox.Min().x / szx;
	int max_idx = bbox.Max().x / szx;
	int min_idy = bbox.Min().y / szy;
	int max_idy = bbox.Max().y / szy;
	int min_idz = bbox.Min().z / szz;
	int max_idz = bbox.Max().z / szz;
	
	class MeshHitCB : public ITriMeshHitCB {
		public:
		float y, z;
		float szx, szy, szz;
		int min_idx, max_idx;
		EVoxelGrid *grid;
		
		EVertex old_point;
		bool	old_into;
		bool	handled;
		uint	odd;
		
		MeshHitCB ( EVoxelGrid *grid, float y, float z, float szx, float szy, float szz, int min_idx, int max_idx ) {
			handled			=	0;
			old_point.position	=	EVec3(0,0,0);
			old_into		=	false;
			this->grid		=	grid;
			this->y			=	y;
			this->z			=	z;
			this->szx		=	szx;
			this->szy		=	szy;
			this->szz		=	szz;
			this->min_idx	=	min_idx;
			this->max_idx	=	max_idx;
			odd				=	0;
		}
		
		virtual void Callback( EVertex &point, float fraction, bool into_front_side ) {
			odd++;
			if ( into_front_side ) {
				old_point = point;
			} else {
				
				for (int idx = min_idx; idx<= max_idx; idx++) {
					
					float x = idx * szx;
					
					if ( x > old_point.position.x && x < point.position.x ) {
						
						EVoxel	vx;
						float dx	=	0;//FRand(-0.2, 0.2);
						float dy	=	0;//FRand(-0.2, 0.2);
						float dz	=	0;//FRand(-0.2, 0.2);
						vx.center	=	EVec3(x+dx, y+dy, z+dz);
						vx.szx		=	szx;
						vx.szy		=	szy;
						vx.szz		=	szz;
					
						grid->AddVoxel( vx );
					}
					
				}
				
			}
		}
	};
	
	for (int idy=min_idy; idy<=max_idy; idy++) {
		for (int idz=min_idz; idz<=max_idz; idz++) {
	
			//int idy = 0;
			//int idz = 0;
		
			float y = (float)idy * szy;
			float z = (float)idz * szz;
			EVec3	begin ( bbox.Min().x, y, z );
			EVec3	dir	  ( bbox.Size().x, 0, 0);
			
			MeshHitCB	hit_cb( this, y, z, szx, szy, szz, min_idx, max_idx );
			
			mesh->RayIntersect( begin, dir, &hit_cb );
			
		}
	}

#else
	
	//	DFS using recursion :
	//	Fill( mesh, origin, step_x, step_y, step_z, 0,0,0);

	//	BFS using queue :	
	
	queue<vxind_s>	Q;
	
	Q.push( vxind_s(0,0,0) );

	EVoxel new_vx;
	new_vx.idx		=	0;
	new_vx.idy		=	0;
	new_vx.idz		=	0;
	new_vx.szx		=	szx;
	new_vx.szy		=	szy;
	new_vx.szz		=	szz;
	new_vx.center	=	origin;
	AddVoxel( new_vx );

	while ( !Q.empty() ) {
	
		vxind_s vxind = Q.front();
		Q.pop();
		
		int idx = vxind.idx;
		int idy = vxind.idy;
		int idz = vxind.idz;
		float x = origin.x + (float)idx * szx;
		float y = origin.y + (float)idy * szy;
		float z = origin.z + (float)idz * szz;

		if (!bbox.Contains( EVec4( x, y, z, 1 ) )) {
			RUNTIME_ERROR("mesh has holes or origin point is outside of closed mesh");
		}
		
		EVec3	dir;	
		Grow(Q, mesh, EVec3(x,y,z), szx, szy, szz,  idx, idy, idz, -1,  0,  0, dir);
		Grow(Q, mesh, EVec3(x,y,z), szx, szy, szz,  idx, idy, idz, +1,  0,  0, dir);
		Grow(Q, mesh, EVec3(x,y,z), szx, szy, szz,  idx, idy, idz,  0, -1,  0, dir);
		Grow(Q, mesh, EVec3(x,y,z), szx, szy, szz,  idx, idy, idz,  0, +1,  0, dir);
		Grow(Q, mesh, EVec3(x,y,z), szx, szy, szz,  idx, idy, idz,  0,  0, -1, dir);
		Grow(Q, mesh, EVec3(x,y,z), szx, szy, szz,  idx, idy, idz,  0,  0, +1, dir);
	}
#endif		
}


//
//	EVoxelGrid::Fill
//
void EVoxelGrid::Fill( const IPxTriMesh mesh, EVec3 origin, float szx, float szy, float szz, int idx, int idy, int idz )
{
	//EVoxel voxel;
	//voxel.center	=	origin;
	//voxel.idx		=	idx;
	//voxel.idy		=	idy;
	//voxel.idz		=	idz;
	//voxel.szx		=	szx;
	//voxel.szy		=	szy;
	//voxel.szz		=	szz;
	//
	//AddVoxel( voxel );
	//
	//if (!bbox.Contains( EVec4( origin.x, origin.y, origin.z, 1 ) )) {
	//	RUNTIME_ERROR("mesh has holes or origin point is outside of closed mesh");
	//}

	//EVec3	dir;	
	//if ( Grow(mesh, origin, szx, szy, szz,  idx, idy, idz, -1,  0,  0, dir) ) {	Fill( mesh, origin + dir, szx, szy, szz,  idx-1, idy  , idz  );	}
	//if ( Grow(mesh, origin, szx, szy, szz,  idx, idy, idz, +1,  0,  0, dir) ) {	Fill( mesh, origin + dir, szx, szy, szz,  idx+1, idy  , idz  );	}
	//if ( Grow(mesh, origin, szx, szy, szz,  idx, idy, idz,  0, -1,  0, dir) ) {	Fill( mesh, origin + dir, szx, szy, szz,  idx  , idy-1, idz  );	}
	//if ( Grow(mesh, origin, szx, szy, szz,  idx, idy, idz,  0, +1,  0, dir) ) {	Fill( mesh, origin + dir, szx, szy, szz,  idx  , idy+1, idz  );	}
	//if ( Grow(mesh, origin, szx, szy, szz,  idx, idy, idz,  0,  0, -1, dir) ) {	Fill( mesh, origin + dir, szx, szy, szz,  idx  , idy  , idz-1);	}
	//if ( Grow(mesh, origin, szx, szy, szz,  idx, idy, idz,  0,  0, +1, dir) ) {	Fill( mesh, origin + dir, szx, szy, szz,  idx  , idy  , idz+1);	}
}


//
//	EVoxelGrid::Grow
//
bool EVoxelGrid::Grow(queue<vxind_s> &Q, const IPxTriMesh mesh, EVec3 origin, float szx, float szy, float szz, int idx, int idy, int idz, int dx, int dy, int dz, EVec3 &dir )
{
	EVoxel vx;
	
	//	neighboring voxel is already exist - no grow :
	if (GetVoxel(idx+dx, idy+dy, idz+dz, vx)) {
		return false;
	}

	//	ray cast against bounding mesh :
	dir	=	EVec3( szx * dx, szy * dy, szz * dz );
	
	EVertex v;
	mesh.getPointer();
	bool r = false;
	
	//	do some attemtps :	
	for (uint i=0; i<10; i++) {
		EVec3 dd = EVec3( vmath::randf(-0.1f,0.1f), vmath::randf(-0.1f,0.1f), vmath::randf(-0.1f,0.1f) );
		r |= mesh->RayIntersect( origin + dd, dir, v );
	}

	//	at least one hit - no grow
	if (r) {	
		return false;
	}	
	
	//	now can grow and add voxels :
	//	push indices to stack :
	Q.push( vxind_s( idx + dx, idy + dy, idz + dz ) );	
	
	EVoxel new_vx;
	new_vx.idx		=	idx + dx;
	new_vx.idy		=	idy + dy;
	new_vx.idz		=	idz + dz;
	new_vx.szx		=	szx;
	new_vx.szy		=	szy;
	new_vx.szz		=	szz;
	new_vx.center	=	origin + dir;
	
	AddVoxel( new_vx );
	
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
	for (int i=n-1; i>=0; i--) {
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

