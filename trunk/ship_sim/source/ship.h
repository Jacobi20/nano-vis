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
	Naive ship simulator :
-----------------------------------------------------------------------------*/

#include "mersenne.h"

struct ESurfElem {
		EVec4	position;
		EVec4	normal;
		float	area;
	};


class EShip : public IShip {
	public:
							EShip				( lua_State *L, int idx );
							~EShip				( void );
			
		//	general :		
		virtual EString		Name				( void ) const { return name; }
		virtual void		Simulate			( float dtime, IPxWaving waving );

		//	ship properties :
		virtual void		GetPose				( EVec4 &position, EQuat &orient );
		virtual void		SetPose				( EVec4 &position, EQuat &orient );
		
		virtual void		BuildVoxels			( const EString path, float cube_size );
		virtual void		BuildSurfaceDXDY	( const EString path, float density, float gathering_radius );
		
		virtual void		SetVisMesh			( const EString path );
		virtual void		SetHSFMesh			( const EString path );
		virtual void		SetHDFMesh			( const EString path );
		virtual void		SetResistance		( float cx ) { water_resistance_cx = cx; }
		virtual void		MakeRigidBody		( const EString path, float mass );
		virtual void		AddForce			( EVec3 force, EVec3 point, bool local_point );
		virtual void		AddMomentum			( EVec3 momentum, bool local_momentum );
		virtual void		SetCMass			( float x, float y, float z );
		
		virtual EVec3		GetHSFMomentum		( void ) { return total_hsf_momentum; }
		virtual float		GetHSFForce			( void ) { return total_hsf_force; }
		virtual float		GetRightArm			( void ) { return right_arm; }

		virtual EVec3		GetCenterMass		( void );
		virtual EVec3		GetInertiaMomentum	( EVec3 axis );

	protected:
		IPxFREntity			r_ent, r_ent2;
	
		EPlane				GetShipXOZPlane		( void );
	
		EString				name;
		
		float				total_hsf_force;
		float				right_arm;
		EVec3				total_hsf_momentum;
		EVec3				center_of_hsf_force;
		
		float				water_resistance_cx;
	
		IPxTriMesh			GetSubmergedMesh		( const EMatrix4 &world, const EPlane &plane );
	
		void				UpdateForces	( float dtime, IPxWaving waving );
		void				UpdateHSF		( float dtime, IPxWaving waving );
		void				UpdateHDF		( float dtime, IPxWaving waving );
		
		void				UpdateHSFBox	( float dtime, IPxWaving waving );
		void				UpdateHSFVoxel	( float dtime, IPxWaving waving );
		void				UpdateHSFSurface( float dtime, IPxWaving waving );

		void				UpdateHXFSEGrid	( void );		
		void				UpdateHXFSE		( float dtime, IPxWaving waving );
		void				BalanceHXFSE	( void );
		void				BalanceHXFSEAxis( const EBBox &box, uint axis_id, float step );
		struct {
			RandMT				mtrand;
			EString				path;
			float				density;
			float				radius;
			vector<ESurfElem>	grid;
		} hxfgrid;
		

		EPxVoxelGrid	voxel_grid;
		
		
		float			ship_length;
		float			ship_width;
		float			ship_height;
		float			ship_mass;
		float			self_time;
		EVec4			cmass;
	
		IPxTriMesh		mesh_vis;
		IPxTriMesh		mesh_hsf;
		IPxTriMesh		mesh_hdf;
		IPxTriMesh		mesh_submerged_hsf;
		IPxTriMesh		mesh_submerged_hdf;
		
		IPxPhysEntity	ship_body;


		//NxActor			*ship_body;
		
		IPxVar			ship_show_voxels;
		IPxVar			ship_show_hull;
		IPxVar			ship_show_submerge;
		IPxVar			ship_hsf_method;
	};
	
