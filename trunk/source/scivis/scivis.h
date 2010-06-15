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
	


class ESciVis : public ISciVis {
	public:
								ESciVis			( void );
								~ESciVis			( void );

		virtual void	*		GetWindowDescriptor	( void ) { return vid_state.h_wnd; }
		virtual void			RenderFrame			( uint dtime );
		virtual void			RenderSnapshot		( const char *command );
		
		void					GetScreenSize		( uint &w, uint &h );
		ID3DXEffect				*CompileEffect		( const char *path );
		ID3DXMesh				*CreateMesh			( IPxTriMesh mesh );
		IPxTriMesh				LoadMesh			( const char *fspath, const char *hpath );
		void					DrawMesh			( ID3DXMesh *mesh );
		void					UpdateMeshVertices	( ID3DXMesh *d3dmesh, const IPxTriMesh trimesh );
		NxActor				*	CreatePhysBox		( float sx, float sy, float sz, const EVec4 &pos, const EQuat &orient, float mass );
		NxScene				*	GetNxScene			( void ) { return nx_scene; }
		
		void					InitRender			( void );
		void					ShutdownRender		( void );
		void					RenderView			( lua_State * L );
		static int				SCI_RenderView		( lua_State * L );
		static int				SCI_ReloadShaders	( lua_State * L );
		
		void					Simulate			( float dtime );
		
		void					UpdateBoat			( float dtime );
		void					UpdateBoatHDF		( float dtime, const EVec4 &position, const EQuat &orient );
		void					UpdateBoatHSF		( float dtime, const EVec4 &position, const EQuat &orient );
		
	public:
		float				global_simulation_time;
	
		IDirect3D9			*d3d;
		IDirect3DDevice9	*d3ddev;
		EVidState_s			vid_state;
		IPxShell			shell;
		IPxGeometryEngine	ge;
		IPxFileSystem		fs;
		
	protected:
		IPxWaving			waving;
		IPxShip				ship_model;
	
		IPxTriMesh			mesh_ship;
		IPxTriMesh			mesh_sea;
		IPxTriMesh			mesh_flow;
		ID3DXMesh			*ship;
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
		
	static void		ListDisplayModes_f	( ESciVis *self, int argc, char **argv );
	static LRESULT	WndProc				( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	};
