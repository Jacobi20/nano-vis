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

#pragma once

/*-----------------------------------------------------------------------------
	grid for voxelization
-----------------------------------------------------------------------------*/

#include "array3d.h"
#include <queue>

typedef HardRef<class EVoxelGrid>	EPxVoxelGrid;

const uint VOXEL_GRID_SIZE	=	10;


//class EVoxelgrid : public ICoreObject {
//	public:
//						EVoxelGrid		( void );
//		virtual			~EVoxelGrid		( void );
//		virtual void	BuildGrid		( const IPxTriMesh mesh, EVec3 origin, float step_x, float step_y, float step_z );
//
//	};


#if 1
struct EVoxel {
	public:
		EVec3	center;
		float	szx, szy, szz;
		int		idx, idy, idz;
	};
	
	

class EVoxelGrid : public ICoreObject {								  
	public:
		struct vxind_s {
			vxind_s (int x, int y, int z) : idx(x), idy(y), idz(z) {}
			int idx, idy, idz;
		};

	public:
						EVoxelGrid		( void );
		virtual			~EVoxelGrid		( void );
		
		virtual void	BuildGrid		( const IPxTriMesh mesh, EVec3 origin, float step_x, float step_y, float step_z );

		virtual void	Clear			( void );
		virtual uint	GetVoxelNum		( void ) const { return grid.size(); }
		virtual void	GetVoxel		( uint index, EVoxel &voxel ) const;		
		virtual void	SetVoxel		( uint index, const EVoxel &voxel );		
		virtual void	AddVoxel		( const EVoxel &voxel );
		
		virtual bool	GetVoxel		( int idx, int idy, int idz, EVoxel &voxel );
		
	protected:
		void		Fill	( const IPxTriMesh mesh, EVec3 origin, float szx, float szy, float szz, int idx, int idy, int idz );
		bool		Grow	( queue<vxind_s> &Q, const IPxTriMesh mesh, EVec3 origin, float szx, float szy, float szz, int idx, int idy, int idz, int dx, int dy, int dz, EVec3 &dir );

		EBBox		bbox;	
		vector<EVoxel> grid;
		
	};
#endif	