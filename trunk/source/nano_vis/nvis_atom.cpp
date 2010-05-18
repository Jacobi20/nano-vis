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
	vol_fx	=	CompileEffect("volume.fx");
	
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
	SAFE_RELEASE( vol_fx );
	SAFE_RELEASE( mesh_ball );
	SAFE_RELEASE( mesh_stick );
}


//
//	ENanoVis::LoadData
//
EPxCachedMol	ENanoVis::LoadData( const char *path )
{
	for (uint i=0; i<cached_mols.size(); i++) {
		if (cached_mols[i]->name==path) {
			return cached_mols[i];
		}
	}

	LOGF("Loading : %s\r\n", path);

	EPxCachedMol	cmol	=	new ECachedMol();
	cmol->name				=	path;

	//
	//	read molecule :
	//
	OBConversion	conv;
	if (!conv.SetInFormat("CUBE") )		{ RAISE_EXCEPTION(va("failed to set CUBE format")); }
	if (!conv.ReadFile(&cmol->mol, path) )	{ RAISE_EXCEPTION(va("failed to read '%s'", path)); }
	
	LOGF("Molecule has %d atoms", cmol->mol.NumAtoms());
	LOGF("Molecule has %d bonds", cmol->mol.NumBonds());	

	cmol->center	= cmol->mol.Center(0);	
	cmol->mol.Center();			 

	//
	//	read grid data :
	//	
	OBGridData *grid;
	
	std::vector<OBGenericData*>	data_set = cmol->mol.GetAllData( OBGenericDataType::GridData );
	IDirect3DVolumeTexture9	*vol = NULL;

	for (int i=0; i<data_set.size(); i++) {
		LOGF("reading data set entry : %d", i);
		OBGenericData *data = data_set[i];
		
		grid = (OBGridData*)data;
		
		LOGF("max value  : %g", grid->GetMaxValue());
		LOGF("min value  : %g", grid->GetMinValue());
		LOGF("attribute  : %s", grid->GetAttribute().c_str());
		LOGF("points num : %d", grid->GetNumberOfPoints());

		SAFE_RELEASE( vol );
		LoadVolumeData( grid, &vol );
		cmol->volume	=	vol;	
	}		
	
	
	cached_mols.push_back(cmol);
	
	LOGF("Loading complete.");
	return cmol;
}


//
//	ENanoVis::LoadVolumeData
//
void ENanoVis::LoadVolumeData( const OBGridData *grid, IDirect3DVolumeTexture9 **vol )
{
	*vol	=	NULL;
	
	int steps[3];
	int nx, ny, nz;
	
	grid->GetNumberOfPoints(nx, ny, nz);
	
	HRCALL( d3ddev->CreateVolumeTexture(nx, ny, nz, 1, 0, D3DFMT_R32F, D3DPOOL_MANAGED, vol, NULL ));
	
	ASSERT(vol);
	
	D3DLOCKED_BOX box;
	(*vol)->LockBox(0, &box, NULL, 0);
	
		float *bits = (float*)box.pBits;
	
		for (int i=0; i<nx; i++) {
			for (int j=0; j<ny; j++) {
				for (int k=0; k<nz; k++) {
				
					double value = grid->GetValue(i+0, j+0, k+0);
					
					int pos	=	sizeof(float) * i + j * box.RowPitch + k * box.SlicePitch;
					bits[pos/sizeof(float)] = value;
				}
			}
		}		
	
	(*vol)->UnlockBox(0);
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
	bool	skip_shot		=	false;
	EName	path			=	"";
	EName	shot			=	"shot.png";
	float	yaw				=	0;
	float	roll			=	0;
	float	pitch			=	0;
	float	distance		=	10;
	float	atom_scale		=	0.1f;
	float	bond_radius		=	0.03f;
	bool	use_vdw_radius	=	true;
	LuaGetField( L, 1, "skip_shot",			skip_shot		);
	LuaGetField( L, 1, "yaw",				yaw				);
	LuaGetField( L, 1, "roll",				roll			);
	LuaGetField( L, 1, "pitch",				pitch			);
	LuaGetField( L, 1, "path",				path			);
	LuaGetField( L, 1, "distance",			distance		);
	LuaGetField( L, 1, "use_vdw_radius",	use_vdw_radius	);
	LuaGetField( L, 1, "atom_scale",		atom_scale		);
	LuaGetField( L, 1, "bond_radius",		bond_radius		);

	//
	//	load data :
	//	
	OBMol *g_mol = NULL;
	EPxCachedMol	cmol;
	try {
		if (path=="") {
			RAISE_EXCEPTION("path is not specified");
		}
		cmol = LoadData(path.Name());
		
		if (!cmol) {
			RAISE_EXCEPTION(va("'%s' not found", path.Name()));
		}
		
	} catch (exception &e) {
		LOG_ERROR("LoadData() failed : %s", e.what());
		return;
	}

	g_mol	=	&cmol->mol;	
	
	
	//
	//	setup view :
	//	
	D3DXMATRIX	world;
	D3DXMATRIX	view, view_i, r_yaw, r_pitch, r_roll, z_view_offset;
	D3DXMATRIX	proj;
	
	//	setup view matrix :
	D3DXMatrixIdentity		( &world );	
	D3DXMatrixRotationZ		( &r_yaw,			deg2rad(yaw) );
	D3DXMatrixRotationY		( &r_pitch,			deg2rad(pitch) );
	D3DXMatrixRotationX		( &r_roll,			deg2rad(roll) );
	D3DXMatrixTranslation	( &z_view_offset,	0,0, -distance );
	view	=	r_roll * r_pitch * r_yaw * z_view_offset;



	//	setup projection :	
	D3DXMatrixPerspectiveRH( &proj, 0.100f, 0.075f, 0.075f, 1000 );
	
	D3DXMatrixInverse( &view_i, NULL, &view );

	//	set shader constants :	
	HRCALL( atom_fx->SetMatrix("matrix_world",	&world) );
	HRCALL( atom_fx->SetMatrix("matrix_view",	&view) );
	HRCALL( atom_fx->SetMatrix("matrix_proj",	&proj) );

	D3DXVECTOR4		light_dir	( 0, 0, 1, 0);
	D3DXVECTOR4		view_dir	( 0, 0,-1, 0);
	D3DXVECTOR4		atom_color	( 1, 1, 1, 1);
	D3DXVECTOR4		view_point	( 0, 0,-distance, 0);
	D3DXVec4Transform( &view_point, &view_point, &view );
	D3DXVec4Transform( &view_dir, &view_dir, &view_i );
	D3DXVec4Transform( &view_point, &view_point, &view_i );

	HRCALL( atom_fx->SetVector("light_dir",		&light_dir) );
	HRCALL( atom_fx->SetVector("view_dir",		&view_dir) );
	
	//	
	//	Rendering :
	//
	HRCALL( atom_fx->SetTechnique("solid_body") );
	uint n;
	HRCALL( atom_fx->Begin(&n, 0) );
	
	for (uint pass=0; pass<n; pass++) {
		atom_fx->BeginPass(pass);
		
			for (int i=1; i<=g_mol->NumAtoms(); i++) {
				OBAtom	*a	=	g_mol->GetAtom(i);
				
				double *xyz = a->GetCoordinate();
				
				float r = 1;
				if (use_vdw_radius) {
					r = elem_table.GetVdwRad(a->GetAtomicNum());
				}
				
				float asz = atom_scale * r;

				D3DXMATRIX	t, s;				
				D3DXMatrixTranslation( &t, xyz[0], xyz[1], xyz[2] );
				D3DXMatrixScaling( &s, asz, asz, asz );
				world = s*t;
				
				vector<double> rgb = elem_table.GetRGB(a->GetAtomicNum());

				atom_color	=	D3DXVECTOR4(rgb[0], rgb[1], rgb[2], 1);
				HRCALL( atom_fx->SetVector("atom_color",	&atom_color) );
				HRCALL( atom_fx->SetMatrix("matrix_world",	&world) );
				atom_fx->CommitChanges();
				
				mesh_ball->DrawSubset(0);
			}
			
			
			for (int i=0; i<g_mol->NumBonds(); i++) {
				OBBond	*b	=	g_mol->GetBond(i);
				
				double *xyz0 = b->GetBeginAtom()->GetCoordinate();;
				double *xyz1 = b->GetEndAtom()->GetCoordinate();;
				
				D3DXVECTOR3	p0(xyz0[0], xyz0[1], xyz0[2]);
				D3DXVECTOR3	p1(xyz1[0], xyz1[1], xyz1[2]);
				D3DXVECTOR3 d	=	p1 - p0;
				D3DXVECTOR3 dn;
				D3DXVec3Normalize(&dn, &d);
				D3DXVECTOR3	up(0,0,1);
				if (dn.z==1 || dn.x==-1) {
					up = D3DXVECTOR3(1,0,0);
				}
				D3DXMATRIX	s, t, aim, aim_i;
				
				D3DXMatrixIdentity(&s);
				D3DXMatrixIdentity(&t);
				D3DXMatrixIdentity(&aim);
				D3DXMatrixIdentity(&aim_i);
				
				D3DXMatrixScaling		( &s,		bond_radius, bond_radius, D3DXVec3Length( &d ) );
				D3DXMatrixTranslation	( &t,		0, 0, 0.5 * D3DXVec3Length( &d ) );
				D3DXMatrixLookAtLH		( &aim,		&p0,  &p1,  &up  );
				D3DXMatrixInverse		( &aim_i,	NULL,  &aim );
				
				world = s * t * aim_i;
				
				HRCALL( atom_fx->SetMatrix("matrix_world",	&world) );
				atom_fx->CommitChanges();
				
				mesh_stick->DrawSubset(0);
			} 
			
		
		atom_fx->EndPass();
	}
	
	
	HRCALL( atom_fx->End() );
	
	D3DXMatrixIdentity( &world );
	
	RenderVolume(L, cmol, world, view, proj, view_point);
	
	//
	//	make screen shot :
	//
	if (!skip_shot) {
		IDirect3DSurface9	*surf;
		d3ddev->GetRenderTarget(0, &surf);
		
		HRCALL( D3DXSaveSurfaceToFile(shot.Name(), D3DXIFF_PNG, surf, NULL, NULL) );
	}
}


//
//	ENanoVis::RenderVolume
//
void ENanoVis::RenderVolume( lua_State *L, EPxCachedMol cmol, D3DXMATRIX &w, D3DXMATRIX &v, D3DXMATRIX &p, D3DXVECTOR4 &view_point )
{
	if (!vol_fx) return;

	int		slice_num		=	100; 
	float	intens_scale	=	100;
	
	float	value_low		=	0;
	float	value_high		=	1;
	
	LuaGetField( L, 1, "slice_num",		slice_num	);
	LuaGetField( L, 1, "intens_scale",	intens_scale);
	LuaGetField( L, 1, "value_low",		value_low	);
	LuaGetField( L, 1, "value_high",	value_high	);
	
	std::vector<OBGenericData*>	data_set = cmol->mol.GetAllData( OBGenericDataType::GridData );

	if (data_set.empty()) {
		return;
	}	
	
	
	OBGridData *grid = (OBGridData*)data_set[0];
	
	
	
	//
	//	setup :
	//		
	IDirect3DTexture9	*palette;
	HRCALL( D3DXCreateTextureFromFile( d3ddev, "palettes/palette2.tga",  &palette) );
	
	HRCALL( vol_fx->SetTechnique("volume") );

		HRCALL( vol_fx->SetMatrix	("matrix_world",	&w ) );
		HRCALL( vol_fx->SetMatrix	("matrix_view",		&v ) );
		HRCALL( vol_fx->SetMatrix	("matrix_proj",		&p ) );
		HRCALL( vol_fx->SetVector	("view_point",		&view_point ) );
		HRCALL( vol_fx->SetInt		("slice_num",		slice_num) );
		HRCALL( vol_fx->SetFloat	("vol_scale",		intens_scale) );
		HRCALL( vol_fx->SetFloat	("value_low",		value_low	) );
		HRCALL( vol_fx->SetFloat	("value_high",		value_high	) );
		
		vector3 vx, vy, vz;
		vector3	pos	=	grid->GetOriginVector();
		vector3	c	=	cmol->center;
		pos = pos - c;
		grid->GetAxes(vx, vy, vz);

		int nx, ny, nz;		
		grid->GetNumberOfPoints(nx, ny, nz);
		
		D3DXMATRIX	box_orient;
		D3DXMATRIX	box_offset;
		D3DXMATRIX	box_matrix;
		
		D3DXMatrixTranslation(&box_offset, pos.x(), pos.y(), pos.z());
		D3DXMatrixIdentity( &box_orient );
		float sx = nx;// * 0.5;
		float sy = ny;// * 0.5;
		float sz = nz;// * 0.5;
		
		box_orient(0, 0) = sx*vx.x();	box_orient(0, 1) = sx*vx.y();	box_orient(0, 2) = sx*vx.z();	box_orient(0, 3) = 0;
		box_orient(1, 0) = sy*vy.x();	box_orient(1, 1) = sy*vy.y();	box_orient(1, 2) = sy*vy.z();	box_orient(1, 3) = 0;
		box_orient(2, 0) = sz*vz.x();	box_orient(2, 1) = sz*vz.y();	box_orient(2, 2) = sz*vz.z();	box_orient(2, 3) = 0;
		box_orient(3, 0) = 0;			box_orient(3, 1) = 0;			box_orient(3, 2) = 0;			box_orient(3, 3) = 1;
		
		box_matrix	=	box_orient * box_offset;
		
		HRCALL( vol_fx->SetMatrix("matrix_box",			&box_matrix ) );
		HRCALL( vol_fx->SetTexture("volume_data_tex",	cmol->volume) );
		HRCALL( vol_fx->SetTexture("palette_tex",		palette) );
		
	//
	//	rendering :
	//		
	struct vert_s {
		vert_s (D3DXVECTOR3	_p,
				D3DXVECTOR3 _n,
				UINT		_c,
				D3DXVECTOR2	_uv0,
				D3DXVECTOR2	_uv1) {
			p	=	_p;
			n	=	_n;
			c	=	_c;
			uv0	=	_uv0;
			uv1	=	_uv1;
		}

		D3DXVECTOR3	p;
		D3DXVECTOR3 n;
		UINT		c;
		D3DXVECTOR2	uv0;
		D3DXVECTOR2	uv1;
	};
	
	HRCALL( d3ddev->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2 ));
	
	uint n;
	HRCALL( vol_fx->Begin(&n, 0) );
	
	for (uint pass=0; pass<n; pass++) {
		vol_fx->BeginPass(pass);
		
		uint steps = slice_num;

			//	Z planes :
			for (uint i=0; i<steps; i++) {			
				float	z	=	((float) i / (float) steps);// * 2 - 1;
				float	s	=	((float) i / (float) steps);
				vert_s	verts[] = {
					vert_s(D3DXVECTOR3(+1, +1, z), D3DXVECTOR3(0,0,1), 0xFF00FF00, D3DXVECTOR2(1,1), D3DXVECTOR2(s,0)), 
					vert_s(D3DXVECTOR3( 0, +1, z), D3DXVECTOR3(0,0,1), 0xFF000000, D3DXVECTOR2(0,1), D3DXVECTOR2(s,0)), 
					vert_s(D3DXVECTOR3( 0,  0, z), D3DXVECTOR3(0,0,1), 0x00000000, D3DXVECTOR2(0,0), D3DXVECTOR2(s,0)), 
					vert_s(D3DXVECTOR3(+1,  0, z), D3DXVECTOR3(0,0,1), 0x0000FF00, D3DXVECTOR2(1,0), D3DXVECTOR2(s,0)),
				};
				d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, verts, sizeof(vert_s));
			}			

			//	Y planes :
			for (uint i=0; i<steps; i++) {			
				float	y	=	((float) i / (float) steps);// * 2 - 1;
				float	s	=	((float) i / (float) steps);
				vert_s	verts[] = {
					vert_s(D3DXVECTOR3(+1, y, +1), D3DXVECTOR3(0,1,0), 0xFF00FF00, D3DXVECTOR2(1,s), D3DXVECTOR2(1,0)), 
					vert_s(D3DXVECTOR3( 0, y, +1), D3DXVECTOR3(0,1,0), 0xFF000000, D3DXVECTOR2(0,s), D3DXVECTOR2(1,0)), 
					vert_s(D3DXVECTOR3( 0, y,  0), D3DXVECTOR3(0,1,0), 0x00000000, D3DXVECTOR2(0,s), D3DXVECTOR2(0,0)), 
					vert_s(D3DXVECTOR3(+1, y,  0), D3DXVECTOR3(0,1,0), 0x0000FF00, D3DXVECTOR2(1,s), D3DXVECTOR2(0,0)),
				};
				d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, verts, sizeof(vert_s));
			}			

			//	X planes :
			for (uint i=0; i<steps; i++) {			
				float	x	=	((float) i / (float) steps);// * 2 - 1;
				float	s	=	((float) i / (float) steps);
				vert_s	verts[] = {
					vert_s(D3DXVECTOR3(x, +1, +1), D3DXVECTOR3(1,0,0), 0xFF00FF00, D3DXVECTOR2(s,1), D3DXVECTOR2(1,0)), 
					vert_s(D3DXVECTOR3(x,  0, +1), D3DXVECTOR3(1,0,0), 0xFF000000, D3DXVECTOR2(s,0), D3DXVECTOR2(1,0)), 
					vert_s(D3DXVECTOR3(x,  0,  0), D3DXVECTOR3(1,0,0), 0x00000000, D3DXVECTOR2(s,0), D3DXVECTOR2(0,0)), 
					vert_s(D3DXVECTOR3(x, +1,  0), D3DXVECTOR3(1,0,0), 0x0000FF00, D3DXVECTOR2(s,1), D3DXVECTOR2(0,0)),
				};
				d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, verts, sizeof(vert_s));
			}			

		
		vol_fx->EndPass();
	}
	
	HRCALL( vol_fx->End() );
}









