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

#define SHADER_FX			"../scidata/shader.fx"

const float	SHIP_LENGTH		=	105;		//	m
const float	SHIP_WIDTH		=	6;			//	m
const float	SHIP_HEIGHT		=	7;			//	m
const float	SHIP_MASS		=	2000000;	//	kg
const float SHIP_CM_OFFSET	=  -0.7f;
const float WATER_DENSITY	=	1000;		//	kg/m^3
const float SHIP_CX			=	0.2f;

IWaving *create_boukh_waving(lua_State *L, int idx);
IShip	*create_naive_ship	(lua_State *L, int idx);

//
//	ESciVis::InitRender
//
void ESciVis::InitRender( void )
{
	LOG_INIT("Scientific renderer");
	
	
	rolling_history_ptr	=	NULL;
	memset(rolling_history, 0, sizeof(rolling_history));
	

	//	read file :	
	mesh_sea	=	LoadMesh("../scidata/uboat.esx|sea2d");
	sea			=	CreateMesh( mesh_sea );
	shader_fx	=	CompileEffect(SHADER_FX);
	
	change_papams_to_base_reson();
	
	ship_model	=	NULL;
	ship_model2	=	NULL;
	waving		=	create_boukh_waving( NULL, 0 );
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
//	ESciVis::SCI_RenderView
//
int ESciVis::SCI_RenderView( lua_State * L )
{
	ESciVis *self = Linker()->GetSciVis().As<ESciVis>();
	
	self->RenderView( L );
	
	return 0;
}


//
//	ESciVis::SCI_CreateShip
//
int ESciVis::SCI_CreateShip( lua_State * L )
{
	ESciVis *self = Linker()->GetSciVis().As<ESciVis>();
	self->ship_model	=	create_naive_ship( L, 1 );
	return 0;
}

int ESciVis::SCI_CreateShip2( lua_State * L )
{
	ESciVis *self = Linker()->GetSciVis().As<ESciVis>();
	self->ship_model2	=	create_naive_ship( L, 1 );
	return 0;
}


//
//
//
int ESciVis::SCI_ReloadShaders( lua_State * L )
{
	ESciVis *self = Linker()->GetSciVis().As<ESciVis>();
	
	self->shader_fx	=	self->CompileEffect(SHADER_FX);
	self->ship_model->ReloadShader();
	
	return 0;
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
	float	ship_course		=	0;
	LuaGetField( L, 1, "dtime",				dtime			);
	LuaGetField( L, 1, "yaw",				yaw				);
	LuaGetField( L, 1, "roll",				roll			);
	LuaGetField( L, 1, "pitch",				pitch			);
	LuaGetField( L, 1, "distance",			distance		);
	
	LuaGetField( L, 1, "ship_course",		ship_course		);
	Xi	=	ship_course;
	//
	//	simulate :
	//
	//UpdateBoat(dtime);
	waving->Update( dtime );
	
	if (ship_model) {
		ship_model->Simulate( dtime, waving );
	}
	if (ship_model2) {
		ship_model2->Simulate( dtime, waving );
	}
	FramePhysX(dtime);
	
	Simulate(dtime);

	//
	//	setup view :
	//
	D3DXMATRIX	world, ship_yaw, ship_roll, ship_pitch, ship_heaving;
	D3DXMATRIX	view, view_i, rz, ry, rx, z_view_offset;
	D3DXMATRIX	proj;

	//	setup world matrix :
	D3DXMatrixIdentity		( &world );	
	D3DXMatrixRotationZ		( &ship_yaw		,	deg2rad(Xi+180) );
	D3DXMatrixRotationX		( &ship_roll	,	Q			);
	D3DXMatrixRotationY		( &ship_pitch	,	-F			);
	D3DXMatrixTranslation	( &ship_heaving	,	0, 0,	E	); 

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
	//	commit shader consts :
	//
	HRCALL( shader_fx->SetVector("light_dir",		&light_dir) );
	HRCALL( shader_fx->SetVector("view_pos",		&view_pos) );
	HRCALL( shader_fx->SetMatrix("matrix_world",	&world) );
	HRCALL( shader_fx->SetMatrix("matrix_view",	&view) );
	HRCALL( shader_fx->SetMatrix("matrix_proj",	&proj) );

	//
	//	draw ship :
	//
	ERendEnv_s	renv;
	renv.matrix_proj	=	EMatrix4((FLOAT*)proj);
	renv.matrix_view	=	EMatrix4((FLOAT*)view);
	renv.view_pos		=	EVec4((FLOAT*)view_pos);
	
	if (ship_model) {
		ship_model->Render( &renv );
	}
	if (ship_model2) {
		ship_model2->Render( &renv );
	}

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
	
	D3DXMatrixIdentity		( &world );	
	HRCALL( shader_fx->SetMatrix("matrix_world",	&world) );
	
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
		
	
	//	-------------------------------------------------
	//	show rolling graph :
	//	-------------------------------------------------
	if (true) {
		float yaw, pitch, roll;
		EVec4 p;
		EQuat q;	
		
		if (ship_model) {
			ship_model->GetPose( p, q );
		}
		QuatToAngles(q, yaw, pitch, roll);
		
		EVec4 roll_record(yaw, p.z*10, roll, 1);
		rolling_history[ (rolling_history_ptr) % ROLL_HISTORY_SIZE ] = roll_record;
		rolling_history_ptr ++;
		
		for (uint i=0; i<ROLL_HISTORY_SIZE; i++) {
			rolling_history[i].w *= 0.9999f;
		}
	}

	IDirect3DVertexDeclaration9 *line_decl	=	NULL;
	HRCALL( d3ddev->CreateVertexDeclaration( VERTEX_DECL_STATIC, &line_decl ) );
	HRCALL( d3ddev->SetVertexDeclaration( line_decl ) );

	D3DXMatrixIdentity( &world );
	D3DXMatrixIdentity( &view );
	uint w, h;
	GetScreenSize(w, h);
	D3DXMatrixOrthoOffCenterRH( &proj, 0, w, h, 0, -9999,9999 );
	HRCALL( shader_fx->SetMatrix("matrix_world",	&world) );
	HRCALL( shader_fx->SetMatrix("matrix_view",	&view) );
	HRCALL( shader_fx->SetMatrix("matrix_proj",	&proj) );

	vertex_s verts[ROLL_HISTORY_SIZE];	

	//	rolling :	
	for (uint i=0; i<ROLL_HISTORY_SIZE; i++) {
		verts[i].color	=	EVec4(1,1,0.5, rolling_history[i].w);
		verts[i].normal	=	EVec3(0,0,0);
		verts[i].pos	=	EVec3(0.125 * i, -rolling_history[i].z + 40, 0);
		verts[i].uv		=	EVec2(0,0);
	}

	HRCALL( shader_fx->SetTechnique("debug") );
	HRCALL( shader_fx->Begin(&n, 0) );

	for (uint pass=0; pass<n; pass++) {
	
		HRCALL( shader_fx->BeginPass(pass) );
	
		HRCALL( d3ddev->DrawPrimitiveUP(D3DPT_LINESTRIP, ROLL_HISTORY_SIZE-1, verts, sizeof(vertex_s)) );
		
		HRCALL( shader_fx->EndPass() );
	}

	HRCALL( shader_fx->End() );

	//	pitching :	
	for (uint i=0; i<ROLL_HISTORY_SIZE; i++) {
		verts[i].color	=	EVec4(1,1,0.5, rolling_history[i].w);
		verts[i].normal	=	EVec3(0,0,0);
		verts[i].pos	=	EVec3(0.125 * i, -rolling_history[i].y + 80, 0);
		verts[i].uv		=	EVec2(0,0);
	}

	HRCALL( shader_fx->SetTechnique("debug") );
	HRCALL( shader_fx->Begin(&n, 0) );

	for (uint pass=0; pass<n; pass++) {
	
		HRCALL( shader_fx->BeginPass(pass) );
	
		HRCALL( d3ddev->DrawPrimitiveUP(D3DPT_LINESTRIP, ROLL_HISTORY_SIZE-1, verts, sizeof(vertex_s)) );
		
		HRCALL( shader_fx->EndPass() );
	}

	HRCALL( shader_fx->End() );


	SAFE_RELEASE( line_decl );
}

