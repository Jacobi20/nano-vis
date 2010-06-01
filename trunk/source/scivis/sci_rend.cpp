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
	Nano vis :
-----------------------------------------------------------------------------*/

#define SHIP_FS_PATH		"../scidata/boat.esx"
#define SHIP_H_PATH			"|boat1"
#define SHADER_FX			"../scidata/shader.fx"

//
//	ESciVis::InitRender
//
void ESciVis::InitRender( void )
{
	LOG_INIT("Scientific renderer");

	//	read file :	
	ship		=	LoadMesh(SHIP_FS_PATH, SHIP_H_PATH);
	shader_fx	=	CompileEffect(SHADER_FX);
}


//
//	ESciVis::ShutdownRender
//
void ESciVis::ShutdownRender( void )
{
	LOG_SHUTDOWN("Scientific renderer");

	SAFE_RELEASE(ship);
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



int ESciVis::SCI_ReloadShaders( lua_State * L )
{
	ESciVis *self = Linker()->GetSciVis().As<ESciVis>();
	
	self->shader_fx	=	self->CompileEffect(SHADER_FX);
	
	return 0;
}

//
//	ESciVis::RenderView
//
void ESciVis::RenderView( lua_State * L )
{
	if (!shader_fx) {
		return;
	}

	float	yaw				=	0;
	float	roll			=	0;
	float	pitch			=	0;
	float	distance		=	10;
	LuaGetField( L, 1, "yaw",				yaw				);
	LuaGetField( L, 1, "roll",				roll			);
	LuaGetField( L, 1, "pitch",				pitch			);
	LuaGetField( L, 1, "distance",			distance		);

	//
	//	setup view :
	//
	D3DXMATRIX	world;
	D3DXMATRIX	view, view_i, rz, ry, rx, z_view_offset;
	D3DXMATRIX	proj;
	//D3DXMATRIX	z_up, z_up1, z_up2;
	//D3DXMatrixIdentity(&z_up);
	//D3DXMatrixRotationZ(&z_up1, -PI);
	//D3DXMatrixRotationX(&z_up2,  PI);
	//z_up = z_up1 * z_up2;
						
	D3DXMATRIX	z_up(	 0, 0, 1, 0,
						 1, 0, 0, 0,
						 0, 1, 0, 0,
						 0, 0, 0, 1	);
					
	
	//	setup view matrix :
	D3DXMatrixIdentity		( &world );	
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
	D3DXVECTOR4		view_point	( 0, 0,-distance, 0);
	D3DXVec4Transform( &view_point, &view_point, &view );
	D3DXVec4Transform( &view_dir, &view_dir, &view_i );
	D3DXVec4Transform( &view_point, &view_point, &view_i );

	//
	//	commit shader consts :
	//
	HRCALL( shader_fx->SetVector("light_dir",		&light_dir) );
	HRCALL( shader_fx->SetVector("view_dir",		&view_dir) );
	HRCALL( shader_fx->SetMatrix("matrix_world",	&world) );
	HRCALL( shader_fx->SetMatrix("matrix_view",	&view) );
	HRCALL( shader_fx->SetMatrix("matrix_proj",	&proj) );

	//
	//	draw mesh :
	//
	uint n;
	HRCALL( shader_fx->SetTechnique("solid_body") );
	HRCALL( shader_fx->Begin(&n, 0) );
	
	for (uint pass=0; pass<n; pass++) {
	
		HRCALL( shader_fx->BeginPass(pass) );
	
		ship->DrawSubset(0);
		
		HRCALL( shader_fx->EndPass() );
	}

	HRCALL( shader_fx->End() );
}

