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


DLL_EXPORT ISciVis	*CreateSciVis	( void ) {	return new ESciVis(); }

ESciVis	*sci_vis = NULL;

/*-----------------------------------------------------------------------------
	Nano vis :
-----------------------------------------------------------------------------*/

//
//	ESciVis::ESciVis
//
ESciVis::ESciVis( void )
{
	sci_vis	=	this;

	global_simulation_time	=	0;

	LOG_SPLIT("SciVis initialization");
	
	shell	=	Linker()->GetShell();
	ge		=	Linker()->GetGeometryEngine();
	fs		=	Linker()->GetFileSystem();
	
	InitDisplay();
	InitDirect3D();
	
	lua_State *L = shell->Lua();
	lua_register( L, "SCI_RenderView",		SCI_RenderView	);
	lua_register( L, "SCI_ReloadShaders",	SCI_ReloadShaders );
	lua_register( L, "SCI_CreateShip",		SCI_CreateShip	);
	lua_register( L, "SCI_CreateShip2",		SCI_CreateShip2 );
	lua_register( L, "SCI_ShipForce",		SCI_ShipForce	);
	
	InitPhysX();
	InitRender();
	
	LOG_SPLIT("");
}


//
//	ESciVis::~ESciVis
//
ESciVis::~ESciVis( void )
{
	LOG_SPLIT("SciVis shutting down");

	ship_model	=	NULL;
	ship_model2	=	NULL;
	
	
	ShutdownRender();
	ShutdownPhysX();
	
	ShutdownDirect3D();
	ShutdownDisplay();
	
	LOG_SPLIT("");
}


//
//	ESciVis::RenderFrame
//
void ESciVis::RenderFrame( uint dtime )
{
	//int clear_flags = D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL;
	////HRCALL( d3ddev->Clear(0, NULL, clear_flags, D3DCOLOR_ARGB(0x00,0xF0,0x00,0x80), 1.0f, 0 ) );	
	//HRCALL( d3ddev->Clear(0, NULL, clear_flags, D3DCOLOR_ARGB(0xFF,0x00,0x00,0x00), 1.0f, 0 ) );	

 //   HRCALL( d3ddev->BeginScene() );
 //   
 //   RenderShot(10, 0,0,0);

	//HRCALL( d3ddev->EndScene() );
 //   HRCALL( d3ddev->Present( NULL, NULL, NULL, NULL ) );
}


//
//	ESciVis::RenderSnapshot
//
void ESciVis::RenderSnapshot( const char *command )
{
	int clear_flags = D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL;
	HRCALL( d3ddev->Clear(0, NULL, clear_flags, D3DCOLOR_ARGB(0xFF,0x00,0x00,0x00), 1.0f, 0 ) );	

    HRCALL( d3ddev->BeginScene() );
    

    Linker()->GetShell()->ExecuteString( command );
    

	HRCALL( d3ddev->EndScene() );
    HRCALL( d3ddev->Present( NULL, NULL, NULL, NULL ) );
}


/*-----------------------------------------------------------------------------
	Resource registering stuff :
-----------------------------------------------------------------------------*/

static void LogFXCompileMessage( bool is_error, string &str )
{
	uint n0 = 0;
	uint n1 = str.find('\n', n0);
	
	do {		
		string s = str.substr(n0, n1-n0);
		n0 = n1+1;
		n1 = str.find('\n', n0);
		
		if (is_error) {
			LOG_ERROR("%s", s.c_str());
		} else {
			LOG_WARNING("%s", s.c_str());
		}
		
	} while (n1!=string::npos);
}


//
//	ED3DRenderSystem::CompileEffect
//
ID3DXEffect *ESciVis::CompileEffect( const char *path )
{
	ID3DXEffect	*effect = NULL;

	LOGF("compiling : %s", path);
	
	ID3DXBuffer	*errors = NULL;
	uint flags = 0;//D3DXFX_NOT_CLONEABLE;
	
	
	HRESULT hr = D3DXCreateEffectFromFile( d3ddev, path, NULL, NULL, flags, NULL, &effect, &errors );
	
	
	if (FAILED(hr)) {
		effect = NULL;
		string msg = (char*)errors->GetBufferPointer();
		LogFXCompileMessage(true, msg);
		
	} else if (errors) {
		string msg = (char*)errors->GetBufferPointer();
		LogFXCompileMessage(true, msg);
	}
	
	return effect;
}


/*-----------------------------------------------------------------------------
	Resource registering stuff :
-----------------------------------------------------------------------------*/

ID3DXMesh *ESciVis::CreateMesh( IPxTriMesh mesh )
{
	uint		 num_tris	=	mesh->GetTriangleNum();
	uint		 num_verts	=	mesh->GetVertexNum();
	ID3DXMesh	*d3dmesh		=	NULL;
	
	HRCALL( D3DXCreateMesh( num_tris, num_verts, D3DXMESH_32BIT | D3DXMESH_DYNAMIC, VERTEX_DECL_STATIC, d3ddev, &d3dmesh ) );

	IDirect3DVertexBuffer9	*vb	=	NULL;	
	IDirect3DIndexBuffer9	*ib	=	NULL;	
	d3dmesh->GetVertexBuffer( &vb );
	d3dmesh->GetIndexBuffer( &ib );

	vertex_s *vb_ptr = NULL;
	uint	 *ib_ptr = NULL;
	
	HRCALL( vb->Lock(0, 0, (void**)&vb_ptr, D3DLOCK_DISCARD ) );
	HRCALL( ib->Lock(0, 0, (void**)&ib_ptr, D3DLOCK_DISCARD ) );
	
	for (uint i=0; i<num_verts; i++) {
		EVertex v = mesh->GetVertex(i);
		vb_ptr[i].pos		= v.position;
		vb_ptr[i].normal	= v.normal;
		vb_ptr[i].uv		= v.uv0;
		vb_ptr[i].color		= v.color0;
	}

	for (uint i=0; i<num_tris; i++) {
		uint i0, i1, i2;
		mesh->GetTriangle(i, i0, i1, i2);
		ib_ptr[i*3 + 0] = i0;
		ib_ptr[i*3 + 1] = i1;
		ib_ptr[i*3 + 2] = i2;
	}

	vb->Unlock();
	ib->Unlock();
	
	SAFE_RELEASE(vb);
	SAFE_RELEASE(ib);
	
	return d3dmesh;
}


//
//	ESciVis::LoadMesh
//
IPxTriMesh ESciVis::LoadMesh( const char *path )
{
	string spath = path;
	uint n = spath.find('|');

	if (n==string::npos) {
		RAISE_EXCEPTION("path must be <file path|hierarchy path>");
	}

	string			fpath	=	spath.substr(0, n);
	string			hpath	=	spath.substr(n); 

	LOGF("Loading : %s %s", fpath.c_str(), hpath.c_str());

	try {
		IPxScene	scene	=	ge->CreateScene();

		IPxFile	f = fs->FileOpen(fpath.c_str(), FS_OPEN_READ);
		vector<char>	buffer;
		buffer.resize(f->Size()+1, '\0');
		
		f->Read(&buffer[0], f->Size());
		f = NULL;

		//	parse XML :
		xml_document<>	xdoc;
		xdoc.parse<0>(&buffer[0]);

		xml_node<> *xscene	=	xdoc.first_node("scene");

		scene->ReadXML( xscene );
		
		IPxSceneNode snode = scene->GetNodeByPath(hpath.c_str());
		
		return snode->GetMesh();
		
	} catch (exception &e) {
		LOG_WARNING("failed to load: %s", e.what());
		return NULL;
	}
}


//
//	ESciVis::UpdateMeshVertices
//
void ESciVis::UpdateMeshVertices( ID3DXMesh *d3dmesh, const IPxTriMesh mesh )
{
	uint	num_tris	=	mesh->GetTriangleNum();
	uint	num_verts	=	mesh->GetVertexNum();
	
	ASSERT( num_tris  == d3dmesh->GetNumFaces() );
	ASSERT( num_verts == d3dmesh->GetNumVertices() );

	vertex_s *vb_ptr = NULL;
	//uint	 *ib_ptr = NULL;
	
	IDirect3DVertexBuffer9	*vb	=	NULL;	
	//IDirect3DIndexBuffer9	*ib	=	NULL;	
	d3dmesh->GetVertexBuffer( &vb );
	//d3dmesh->GetIndexBuffer( &ib );
	
	HRCALL( vb->Lock(0, 0, (void**)&vb_ptr, D3DLOCK_DISCARD ) );
	//HRCALL( ib->Lock(0, 0, (void**)&ib_ptr, D3DLOCK_DISCARD ) );
	
	for (uint i=0; i<num_verts; i++) {
		EVertex v = mesh->GetVertex(i);
		vb_ptr[i].pos		= v.position;
		vb_ptr[i].normal	= v.normal;
		vb_ptr[i].uv		= v.uv0;
	}

	//for (uint i=0; i<num_tris; i++) {
	//	uint i0, i1, i2;
	//	mesh->GetTriangle(i, i0, i1, i2);
	//	ib_ptr[i*3 + 0] = i0;
	//	ib_ptr[i*3 + 1] = i1;
	//	ib_ptr[i*3 + 2] = i2;
	//}

	vb->Unlock();
	//ib->Unlock();
	
	SAFE_RELEASE(vb);
	//SAFE_RELEASE(ib);
}

//
//	ESciVis::DrawMesh
//
void ESciVis::DrawMesh( ID3DXMesh *mesh )
{
	if (!mesh) {
		return;
	}
}




															 