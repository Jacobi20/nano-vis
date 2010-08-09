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

struct  EVidModeDesc_s {
		uint	width;
		uint	height;
		bool	is_supported;
	};


/*-----------------------------------------------------------------------------
	Nano vis :
-----------------------------------------------------------------------------*/

const uint ROLL_HISTORY_SIZE	=	8192;

struct vertex_s {
	EVec3   pos;
	EVec3   normal;
	EVec4	color;
	EVec2   uv;
};

const D3DVERTEXELEMENT9 VERTEX_DECL_STATIC[] = {
	{ 0, offsetof(vertex_s, pos			), D3DDECLTYPE_FLOAT3,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0 },
	{ 0, offsetof(vertex_s, normal		), D3DDECLTYPE_FLOAT3,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,		0 },
	{ 0, offsetof(vertex_s, color		), D3DDECLTYPE_FLOAT4,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR	,   0 },
	{ 0, offsetof(vertex_s, uv			), D3DDECLTYPE_FLOAT2,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,   0 },
	D3DDECL_END()
};


struct  EVidState_s	{
		bool	fullscr;
		int		mode;
		HWND	h_wnd;
	};
	

#include "static_self.h"

class ESciVis : public ISciVis, public self_ref<ESciVis>, public IUIDrawCB {
	public:
								ESciVis			( void );
								~ESciVis			( void );

		//	view rendering stuff :
		virtual void	*		GetWindowDescriptor	( void ) { return vid_state.h_wnd; }
		virtual void			RenderFrame			( uint dtime );
		virtual void			RenderSnapshot		( const char *command );
		void					GetScreenSize		( uint &w, uint &h );
		
		//	utility rendering functions :
		ID3DXEffect				*CompileEffect		( const char *path );
		ID3DXMesh				*CreateMesh			( IPxTriMesh mesh );
		ID3DXMesh				*CreateMeshCube		( float szx, float szy, float szz );
		IPxTriMesh				LoadMesh			( const char *path );
		void					DrawMesh			( ID3DXMesh *mesh );
		void					UpdateMeshVertices	( ID3DXMesh *d3dmesh, const IPxTriMesh trimesh );
		
		//	utility physical functions :
		NxActor				*	CreatePhysBox		( float sx, float sy, float sz, const EVec4 &pos, const EQuat &orient, float mass );
		NxActor				*	CreatePhysMesh		( IPxTriMesh mesh, const EVec4 &pos, const EQuat &orient, float mass );
		NxActor				*	CreatePhysMesh		( vector<IPxTriMesh> meshes, vector<float> masses );
		NxScene				*	GetNxScene			( void ) { return nx_scene; }
		NxConvexMesh		*	BuildConvexMesh		( const IPxTriMesh input_mesh );

		//	naval dynamic  :
		void					InitRender			( void );
		void					ShutdownRender		( void );
		void					RegisterAPI			( void );
		void					RenderView			( lua_State *L );
		
		void					Simulate			( float dtime );
		void					AddShip				( IPxShip ship );
		void					RemoveShip			( IPxShip ship );

		vector<IPxShip>	ships;

		virtual void		RenderQuads		( uint quad_num, const EUIQuad *quads, EString image );	
		virtual void		GetImageSize	( EString image, uint &width, uint &height ) ;

	
		void				SetTexture			( const EString name );
		void				GetTextureSize		( const EString texname, uint &width, uint &height );
		uint				RegisterTexture		( const EString path );
		void				ReleaseTextures		( void );
		LPDIRECT3DTEXTURE9	CreateNullTexture	( void );
		LPDIRECT3DTEXTURE9	CreateSolidTexture	( byte r, byte g, byte b, byte a );
		LPDIRECT3DTEXTURE9	LoadTextureFromFile	( const EString name );

		struct texture_s {
			LPDIRECT3DTEXTURE9	texture;
			EString				name;
		};
		
		vector<texture_s> textures;
		
	public:
		static int	remove_all_ships	( lua_State *L );
		static int	render_view			( lua_State * L );
		static int	reload_shaders		( lua_State * L );
		
	public:
		float				global_simulation_time;
	
		IDirect3D9			*d3d;
		IDirect3DDevice9	*d3ddev;
		EVidState_s			vid_state;
		
	protected:
		IPxWaving			waving;
		IPxTriMesh			mesh_sea;
		ID3DXMesh			*sea;
		ID3DXEffect			*shader_fx;

		//	PhysX stuff :		
		void				InitPhysX		( void );
		void				ShutdownPhysX	( void );
		void				FramePhysX		( float dtime );
		void				DebugPhysX		( const D3DXMATRIX &w, const D3DXMATRIX &v, const D3DXMATRIX &p );
		
		void				DebugLine		( EVec3 p0, EVec3 p1, EVec4 color );
		void				DebugLine		( NxVec3 p0, NxVec3 p1, EVec4 color );
		
		uint				rolling_history_ptr;
		EVec4				rolling_history[ROLL_HISTORY_SIZE];
		
		ErrorStream			error_stream;
		EAllocator			allocator;
		NxPhysicsSDK		*nx;
		NxScene				*nx_scene;
		NxCookingInterface	*nx_cook;
		NxActor				*ship_body;
		
	protected:
		void		InitDisplay			( void );
		void		InitDirect3D		( void );
		void		ShutdownDisplay		( void );
		void		ShutdownDirect3D	( void );		
		void		CheckDisplayModes	( void );
		uint		GetModeNum			( void );
		void		GetModeDesc			( uint nmode, EVidModeDesc_s *mode_desc );
		uint		GetCurrentMode		( void );
		
		IPxVar		d3d_mode;
		IPxVar		d3d_fullscr;
		IPxVar		d3d_xpos;
		IPxVar		d3d_ypos;
		
	static void			ListDisplayModes_f	( ESciVis *self, int argc, char **argv );
	static LRESULT		WndProc				( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	public:
		//	bullet stuff :
		void	InitBtPhysics		( void );
		void	ShutdownBtPhysics	( void );
		
		btDefaultCollisionConfiguration			* collisionConfiguration;
		btCollisionDispatcher					* dispatcher;
		btBroadphaseInterface					* overlappingPairCache;
		btSequentialImpulseConstraintSolver		* solver;
		btDiscreteDynamicsWorld					* dynamicsWorld;

		
};
