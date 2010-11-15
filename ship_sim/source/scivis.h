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
	Nano vis :
-----------------------------------------------------------------------------*/

const uint ROLL_HISTORY_SIZE	=	8192;

#include "static_self.h"

class ESciVis : public IGame, public self_ref<ESciVis>, public IUIDrawCB {
	public:
								ESciVis			( void );
								~ESciVis			( void );

		//	 :
		virtual void			Frame			( uint dtime );

	public:		
		
		//	utility physical functions :
		//NxActor				*	CreatePhysBox		( float sx, float sy, float sz, const EVec4 &pos, const EQuat &orient, float mass );
		//NxActor				*	CreatePhysMesh		( IPxTriMesh mesh, const EVec4 &pos, const EQuat &orient, float mass );
		//NxActor				*	CreatePhysMesh		( vector<IPxTriMesh> meshes, vector<float> masses );
		//NxScene				*	GetNxScene			( void ) { return nx_scene; }
		//NxConvexMesh		*	BuildConvexMesh		( const IPxTriMesh input_mesh );

		//	naval dynamic  :
		void					Simulate			( float dtime );
		void					AddShip				( IPxShip ship );
		void					RemoveShip			( IPxShip ship );

		vector<IPxShip>	ships;

		virtual void		RenderQuads		( uint quad_num, const EUIQuad *quads, EString image );	
		virtual void		GetImageSize	( EString image, uint &width, uint &height ) ;
		
		IPxFRScene			GetFRScene		( void ) { return rs()->GetFRScene(); }

	public:
		void		RegisterAPI			( void );
		static int	remove_all_ships	( lua_State *L );
		static int	set_view			( lua_State *L );
		static int	get_wave_offset		( lua_State *L );
		static int	set_wind			( lua_State *L );
		static int	set_wave			( lua_State *L );
				
	public:
		float				global_simulation_time;
		
		struct {
			float	fov;
			EVec4	position;
			EQuat	orient;
		} view;
	
	public:	
		IPxWaving			waving;
		IPxTriMesh			mesh_sea;

	protected:
		//	PhysX stuff :		
		//void				InitPhysX		( void );
		//void				ShutdownPhysX	( void );
		//void				FramePhysX		( float dtime );
		//uint				rolling_history_ptr;
		//EVec4				rolling_history[ROLL_HISTORY_SIZE];
		
		//ErrorStream			error_stream;
		//EAllocator			allocator;
		//NxPhysicsSDK		*nx;
		//NxScene				*nx_scene;
		//NxCookingInterface	*nx_cook;
		//NxActor				*ship_body;
	public:

	};
