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

class EShipNaive : public IShip {
	public:
							EShipNaive				( lua_State *L, int idx );
							~EShipNaive				( void );
					
		virtual void		Simulate				( float dtime, IPxWaving waving );
		virtual void		ReloadShader			( void );
		virtual void		GetPose					( EVec4 &position, EQuat &orient );
		virtual void		Render					( ERendEnv_s *rend_env );
		virtual void		ApplyForceAtLocalPoint	( const EVec4 &pos, const EVec4 &force );
		
		void				BuildBuyoancyGrid		( float step );
		
		void				ComputeStatic			( void );
		void				ComputeStatic			( float roll, float posz, EVec3 &bc, EVec3 &force, EVec3 &momentum );
		
		IPxTriMesh			SplitMesh				( const IPxTriMesh mesh );
		
	protected:
		IPxTriMesh			GetSubmergedMesh		( const EMatrix4 &world, const EPlane &plane );
	
		bool				numeric;
	
		void				UpdateForces	( float dtime, IPxWaving waving );
		void				UpdateHSF		( float dtime, IPxWaving waving );
		void				UpdateHDF		( float dtime, IPxWaving waving );

		//	stuff for numerical rolling simulation :
		void				UpdateNumeric	( float dtime, IPxWaving waving );
		void				EulerStep		( float dt, IPxWaving waving );
		float	num_roll;
		float	num_roll_d;
		float	num_zeta;
		float	num_zeta_d;
		
		float	ship_length	;
		float	ship_width	;
		float	ship_height	;
		float	ship_mass	;
		
		void	ComputeShipParams	( void );
		
		EPxVoxelGrid	voxel_buyoancy_grid;
		
		struct {
			float	length;
			float	width;
			float	height;
			float	cm_offset;
			
			float	cx;
			
			float	gmt;	//	rolling metacentric height
			float	gml;	//	pitching metacentric height
			
			float	Ix;		//	X-moment of inertia
		} ship_param;
		
		float			self_time;
	
		IPxTriMesh		mesh_vis;
		IPxTriMesh		mesh_flow;
		IPxTriMesh		mesh_submerged;
		IPxTriMesh		mesh_stat;
		ID3DXMesh		*d3d_mesh_vis;
		ID3DXMesh		*d3d_cube;
		ID3DXEffect		*shader_fx;
		
		NxActor			*ship_body;
		
		IPxVar			ship_show_cubes;
		IPxVar			ship_show_hull;
		IPxVar			ship_show_submerge;
	};
	
