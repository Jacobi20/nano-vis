/*
	The MIT License

	Copyright (c) 2008 IFMO/GameDev Studio

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


#include "nvis_local.h"



#ifdef _DEBUG
	#pragma comment( lib, "../obsdk/lib/debug/openbabel-2.lib" )
#else
	#pragma comment( lib, "../obsdk/lib/release/openbabel-2.lib" )
#endif

using namespace OpenBabel;

/*-----------------------------------------------------------------------------
	Atom visializer :
-----------------------------------------------------------------------------*/
const D3DVERTEXELEMENT9 VERTEX_DECL_STATIC[] = {
		{ 0, 0,		D3DDECLTYPE_FLOAT3,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0 },
		{ 0, 12,	D3DDECLTYPE_FLOAT3,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,		0 },
		{ 0, 20,	D3DDECLTYPE_FLOAT2,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,   0 },
		D3DDECL_END()
	};


//
//	ENanoVis::InitAtomRend
//
void ENanoVis::InitAtomRend( void )
{
	LOG_INIT("Atom renderer");
	
	atom_fx	=	CompileEffect("atom.fx");
	
	HRCALL( D3DXCreateSphere( d3ddev, 1.0f, 32, 16,		&mesh_ball, NULL) );
	HRCALL( D3DXCreateCylinder( d3ddev, 1.0f, 1.0f, 1.0f, 16, 1.0f, &mesh_stick, NULL) );

	ID3DXMesh	*mesh_temp_ball		=	mesh_ball;
	ID3DXMesh	*mesh_temp_stick	=	mesh_stick;
	
	HRCALL( mesh_temp_ball->CloneMesh  ( 0, VERTEX_DECL_STATIC, d3ddev, &mesh_ball ) );
	HRCALL( mesh_temp_stick->CloneMesh ( 0, VERTEX_DECL_STATIC, d3ddev, &mesh_stick ) );

	SAFE_RELEASE( mesh_temp_ball );
	SAFE_RELEASE( mesh_temp_stick );

	//	register Lua func :
	lua_register( shell->Lua(), "NVisSnapshot", NVisSnapshot );
}


//
//	ENanoVis::ShutdownAtomRend
//
void ENanoVis::ShutdownAtomRend( void )
{
	LOG_SHUTDOWN("Atom renderer");
	
	SAFE_RELEASE( atom_fx );
	SAFE_RELEASE( mesh_ball );
	SAFE_RELEASE( mesh_stick );
}

OBMol	g_mol;

//
//	ENanoVis::LoadData
//
void ENanoVis::LoadData( const char *path )
{
	LOGF("Loading : %s\r\n", path);

	OBConversion	conv;
	OBMol			mol;
	if (!conv.SetInFormat("CUBE") )		{ RAISE_EXCEPTION(va("failed to set CUBE format")); }
	if (!conv.ReadFile(&mol, path) )	{ RAISE_EXCEPTION(va("failed to read '%s'", path)); }
	
	LOGF("Molecule has %d atoms", mol.NumAtoms());
	LOGF("Molecule has %d bonds", mol.NumBonds());	
	
	mol.Center();			    
	
	LOGF("Loading complete.");
	g_mol	=	mol;
}


//
//	ENanoVis::NVisSnapshot
//
int ENanoVis::NVisSnapshot( lua_State *L )
{
	ENanoVis *self = Linker()->GetNanoVis().To<ENanoVis>();
	self->RenderShot(L);
	
	return 0;
}


//
//	ENanoVis::RenderShot
//
void ENanoVis::RenderShot( lua_State *L )
{
	if (!atom_fx) return;

	//
	//	read table :
	//	
	EName	path	=	"";
	EName	shot	=	"shot.png";
	float	yaw		=	0;
	float	roll	=	0;
	float	pitch	=	0;
	LuaGetField( L, 1, "yaw",	yaw		);
	LuaGetField( L, 1, "roll",	roll	);
	LuaGetField( L, 1, "pitch",	pitch	);
	LuaGetField( L, 1, "path",	path	);

	//
	//	load data :
	//	
	try {
		if (path=="") {
			RAISE_EXCEPTION("path is not specified");
		}
		Linker()->GetNanoVis()->LoadData(path.Name());
	} catch (exception &e) {
		LOG_ERROR("LoadData() failed : %s", e.what());
	}
	
	//
	//	setup view :
	//	
	D3DXMATRIX	world;
	D3DXMATRIX	view, view_i, r_yaw, r_pitch, r_roll;
	D3DXMATRIX	proj;

	D3DXMatrixIdentity( &world );	
	D3DXMatrixRotationZ( &r_yaw,		deg2rad(yaw) );
	D3DXMatrixRotationY( &r_pitch,		deg2rad(pitch) );
	D3DXMatrixRotationX( &r_roll,		deg2rad(roll) );
	view	=	r_roll * r_pitch * r_yaw;
	D3DXMatrixOrthoRH( &proj, 16, 12, -16, 16 );
	
	D3DXMatrixInverse( &view_i, NULL, &view );
	
	HRCALL( atom_fx->SetMatrix("matrix_world",	&world) );
	HRCALL( atom_fx->SetMatrix("matrix_view",	&view) );
	HRCALL( atom_fx->SetMatrix("matrix_proj",	&proj) );

	D3DXVECTOR4		light_dir	( 0, 0, 1, 0);
	D3DXVECTOR4		view_dir	( 0, 0,-1, 0);
	D3DXVECTOR4		atom_color	( 1, 1, 1, 1);
	D3DXVec4Transform( &view_dir, &view_dir, &view_i );

	HRCALL( atom_fx->SetVector("light_dir",		&light_dir) );
	HRCALL( atom_fx->SetVector("view_dir",		&view_dir) );
	HRCALL( atom_fx->SetVector("atom_color",	&atom_color) );
	
	//	
	//	Rendering :
	//
	HRCALL( atom_fx->SetTechnique("solid_body") );
	uint n;
	HRCALL( atom_fx->Begin(&n, 0) );
	
	for (uint i=0; i<n; i++) {
		atom_fx->BeginPass(i);
		
			for (int i=1; i<=g_mol.NumAtoms(); i++) {
				OBAtom	*a	=	g_mol.GetAtom(i);
				
				double *xyz = a->GetCoordinate();
				
				D3DXMatrixTranslation( &world, xyz[0], xyz[1], xyz[2] );
				
				HRCALL( atom_fx->SetMatrix("matrix_world",	&world) );
				atom_fx->CommitChanges();
				
				mesh_ball->DrawSubset(0);
			}
			
		
		atom_fx->EndPass();
	}
	
	
	HRCALL( atom_fx->End() );
	
	//
	//	make screen shot :
	//
	IDirect3DSurface9	*surf;
	d3ddev->GetRenderTarget(0, &surf);
	
	HRCALL( D3DXSaveSurfaceToFile(shot.Name(), D3DXIFF_PNG, surf, NULL, NULL) );
}