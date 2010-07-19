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
#include "linear/linear.h"
#include "waves/waves.h"
#include "waves/newave.h"

/*-----------------------------------------------------------------------------
	Nano vis :
-----------------------------------------------------------------------------*/

#define SHADER_FX			"shader.fx"

IWaving *create_boukh_waving(lua_State *L, int idx);
IShip	*create_ship	(lua_State *L, int idx);

//
//	ESciVis::InitRender
//
void ESciVis::InitRender( void )
{
	LOG_INIT("Scientific renderer");
	
	
	rolling_history_ptr	=	NULL;
	memset(rolling_history, 0, sizeof(rolling_history));
	

	//	read file :	
	mesh_sea	=	LoadMesh("uboat.esx|sea2d");
	sea			=	CreateMesh( mesh_sea );
	shader_fx	=	CompileEffect(SHADER_FX);
	
	change_papams_to_base_reson();
	
	waving		=	create_boukh_waving( NULL, 0 );
}


//
//	ESciVis::AddShip
//
void ESciVis::AddShip( IPxShip ship )
{
	ships.push_back( ship );
}


//
//	ESciVis::RemoveShip
//
void ESciVis::RemoveShip( IPxShip ship )
{
	vector<IPxShip>::iterator	vi;
	vi = find(ships.begin(), ships.end(), ship);
	
	if (vi!=ships.end()) {
		ships.erase( vi );
	}
}


//
//	ESciVis::ShutdownRender
//
void ESciVis::ShutdownRender( void )
{
	LOG_SHUTDOWN("Scientific renderer");

	SAFE_RELEASE(shader_fx);
}


//
//
//
void ESciVis::Simulate( float dtime )
{
	global_simulation_time += dtime;
}



//
//	ESciVis::RenderView
//
void ESciVis::RenderView( lua_State * L )
{
	if (!shader_fx) {
		return;
	}
	
	float	dtime			=	0.016f;
	float	yaw				=	0;
	float	roll			=	0;
	float	pitch			=	0;
	float	distance		=	10;
	LuaGetField( L, 1, "dtime",				dtime			);
	LuaGetField( L, 1, "yaw",				yaw				);
	LuaGetField( L, 1, "roll",				roll			);
	LuaGetField( L, 1, "pitch",				pitch			);
	LuaGetField( L, 1, "distance",			distance		);
	
	//
	//	simulate :
	//
//--------------------------------------------------------
		//UpdateBoat(dtime);
	waving->Update( dtime );
	
	for (uint  i=0; i<ships.size(); i++) {
		ships[i]->Simulate( dtime, waving );
	}
	
	FramePhysX(dtime);
	
	Simulate(dtime);

	//
	//	setup view :
	//
	D3DXMATRIX	view, view_i, rz, ry, rx, z_view_offset;
	D3DXMATRIX	proj;

	//	setup view matrix :
	D3DXMATRIX	z_up(	 0, 0, 1, 0,
						 1, 0, 0, 0,
						 0, 1, 0, 0,
						 0, 0, 0, 1	);
	
	D3DXMatrixRotationX		( &rx,	deg2rad(roll) );
	D3DXMatrixRotationY		( &ry,	deg2rad(pitch) );
	D3DXMatrixRotationZ		( &rz,	deg2rad(yaw) );
	D3DXMatrixTranslation	( &z_view_offset,	0,0, -distance );
	view	=	 rz * ry * rx * z_up * z_view_offset;
	
	D3DXMatrixInverse( &view_i, NULL, &view );

	//	setup projection :	
	D3DXMatrixPerspectiveRH( &proj, 0.100f, 0.075f, 0.075f, 1000 );

	//	setup view & light vectors :
	D3DXVECTOR4		light_dir	( 0, 0, 1, 0);
	D3DXVECTOR4		view_dir	( 0, 0,-1, 0);
	D3DXVECTOR4		atom_color	( 1, 1, 1, 1);
	D3DXVECTOR4		view_pos	( 0, 0, 0, 1);
	D3DXVec4Transform( &view_pos, &view_pos, &view_i );
	D3DXVec4Transform( &view_dir, &view_dir, &view_i );

	//
	//	draw ships :
	//
	ERendEnv_s	renv;
	renv.matrix_proj	=	EMatrix4((FLOAT*)proj);
	renv.matrix_view	=	EMatrix4((FLOAT*)view);
	renv.view_pos		=	EVec4((FLOAT*)view_pos);
	
	for (uint i=0; i<ships.size(); i++) {
		ships[i]->Render( &renv );
	}

	//--------------------------------------------------------
	//
	//	draw sea :
	//
	for (uint i=0; i<mesh_sea->GetVertexNum(); i++) {
		EVertex		v = mesh_sea->GetVertex(i);
		
		v.position.z	=	waving->GetPosition( Vec3ToPoint4(v.position) ).z;
		
		mesh_sea->SetVertex(i, v);
	}
	mesh_sea->ComputeNormals();
	
	UpdateMeshVertices( sea, mesh_sea );

	D3DXMATRIX world;	
	D3DXMatrixIdentity		( &world );	
	HRCALL( shader_fx->SetMatrix("matrix_world",	&D3DXMATRIX( world ) ) );
	HRCALL( shader_fx->SetMatrix("matrix_view",		&D3DXMATRIX( renv.matrix_view.Ptr() ) ) );
	HRCALL( shader_fx->SetMatrix("matrix_proj",		&D3DXMATRIX( renv.matrix_proj.Ptr() ) ) );
	HRCALL( shader_fx->SetVector("view_pos",		&D3DXVECTOR4( renv.view_pos.Ptr() ) ) );
	
	uint n;	
	HRCALL( shader_fx->SetTechnique("solid_body") );
	HRCALL( shader_fx->Begin(&n, 0) );

	for (uint pass=0; pass<n; pass++) {
	
		HRCALL( shader_fx->BeginPass(pass) );
	
		sea->DrawSubset(0);
		
		HRCALL( shader_fx->EndPass() );
	}

	HRCALL( shader_fx->End() );
	
	DebugPhysX(world, view, proj);
	
	//ID3DXFont	*font;
	//D3DXCreateFont(d3ddev, 	

	if (1) {	
		IPxUIFont	font	=	UserInterface()->RegisterFont("fonts/console_font.fnt");
		
		uint n = Log()->GetDebugStringNum();
		
		for (uint i=0; i<n; i++) {
			font->DrawString(this, 12, 24+i*12, Log()->GetDebugString(i), EVec4(1,1,1,1));
		}
	}
	Log()->ClearDebugStrings();
}

