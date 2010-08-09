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
	texture stuff :
-----------------------------------------------------------------------------*/

//
//	ED3DRenderSystem::SetTexture
//
void ESciVis::SetTexture( const EString name )
{
	uint id = RegisterTexture( name );
	
	HRCALL( d3ddev->SetTexture( 0, textures[id].texture ) );
}


//
//	ED3DRenderSystem::GetTextureSize
//
void ESciVis::GetTextureSize( const EString texname, uint &width, uint &height )
{
	uint id = RegisterTexture( texname );
	
	LPDIRECT3DTEXTURE9	tex	=	textures[id].texture;
	D3DSURFACE_DESC	desc;
	tex->GetLevelDesc(0, &desc);
	
	width	=	desc.Width;
	height	=	desc.Height;
}


//
//	ED3DRenderSystem::RegisterTexture
//
uint ESciVis::RegisterTexture( const EString path )
{
	LPDIRECT3DTEXTURE9	tex;

	//	search for texture :	
	uint len = textures.size();
	for (uint i=0; i<len; i++) {
		if (textures[i].name==path) {
			return i;
		}
	}

	//	texture was not found, add it :
	try {
		LOGF("Loading : %s", path.CStr());

		//	Virtual texture names :
		if (path.CStr()[0]=='*') {	
			if (strcmp(path.CStr(), "*null")  == 0 )	tex = CreateNullTexture ( );						else
			if (strcmp(path.CStr(), "*black") == 0 )	tex = CreateSolidTexture( 0x00, 0x00, 0x00, 0xFF ); else
			if (strcmp(path.CStr(), "*grey")  == 0 )	tex = CreateSolidTexture( 0x80, 0x80, 0x80, 0xFF ); else
			if (strcmp(path.CStr(), "*white") == 0 )	tex = CreateSolidTexture( 0xFF, 0xFF, 0xFF, 0xFF ); else
			if (strcmp(path.CStr(), "*red")   == 0 )	tex = CreateSolidTexture( 0xFF, 0x00, 0x00, 0xFF ); else
			if (strcmp(path.CStr(), "*green") == 0 )	tex = CreateSolidTexture( 0x00, 0xFF, 0x00, 0xFF ); else
			if (strcmp(path.CStr(), "*blue")  == 0 )	tex = CreateSolidTexture( 0x00, 0x00, 0xFF, 0xFF ); else
			if (strcmp(path.CStr(), "*flat")  == 0 )	tex = CreateSolidTexture( 0x80, 0x80, 0xFF, 0x80 ); else	// flat normal map
			RAISE_EXCEPTION(va("wrong virtual texture name '%s'", path));
		} else {	
			tex	=	LoadTextureFromFile(path);
		}
		
	} catch (exception &e) {	
		LOG_WARNING(e.what());	
		
		tex = CreateNullTexture();
	}

	//	push created texture to the list :
	uint id = textures.size();
	
	texture_s tex_rec;
	tex_rec.name	=	path;
	tex_rec.texture	=	tex;
	
	textures.push_back( tex_rec );

	return id;
}


//
//	ED3DRenderSystem::ReleaseTextures
//
void ESciVis::ReleaseTextures( void )
{
	for ( uint i=0; i<textures.size(); i++) {
		SAFE_RELEASE( textures[i].texture );
	}
	textures.clear();
}


/*-----------------------------------------------------------------------------
	texture creation :
-----------------------------------------------------------------------------*/

//
//	ED3DRenderSystem::CreateNullTexture
//
LPDIRECT3DTEXTURE9 ESciVis::CreateNullTexture( void )
{
	LPDIRECT3DTEXTURE9 d3d_tex;
	HRCALL( d3ddev->CreateTexture(16,16, 0, D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &d3d_tex, NULL) );
	
	D3DLOCKED_RECT rect;	
	HRCALL( d3d_tex->LockRect(0, &rect, NULL, 0) );

		byte *ptr = (byte*)rect.pBits;	
		for (uint x=0; x<16; x++) {
			for (uint y=0; y<16; y++) {
				
				byte clr[4];
				
				// All surface should be BLACK :
				clr[0] = clr[1] = clr[2] = 0x00;
				clr[3] = 0xFF;
				
				if ((x^y)&1) {				// PINK squares, it is so emotinaly
					clr[0] = clr[2] = 0xFF;	
				}
				
				if (x==15 || y==15) {		// YELLOW edge to view mapping.
					clr[0] = clr[1] = 0xFF;
					clr[2] = 0x00;
				}
				
				ptr[ y*rect.Pitch + x*4 + 0 ] = clr[2];
				ptr[ y*rect.Pitch + x*4 + 1 ] = clr[1];
				ptr[ y*rect.Pitch + x*4 + 2 ] = clr[0];
				ptr[ y*rect.Pitch + x*4 + 3 ] = clr[3];
			}
		}
	
	HRCALL( d3d_tex->UnlockRect(0) );
	
	return d3d_tex;
}


//
//	ED3DRenderSystem::CreateSolidTexture
//
LPDIRECT3DTEXTURE9 ESciVis::CreateSolidTexture( byte r, byte g, byte b, byte a )
{
	LPDIRECT3DTEXTURE9 d3d_tex;
	HRCALL( d3ddev->CreateTexture(16,16, 0, D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &d3d_tex, NULL) );
	
	D3DLOCKED_RECT rect;	
	HRCALL( d3d_tex->LockRect(0, &rect, NULL, 0) );

		byte *ptr = (byte*)rect.pBits;	
		for (uint x=0; x<16; x++) {
			for (uint y=0; y<16; y++) {
				byte clr[4] = {r,g,b,a};
				
				ptr[ y*rect.Pitch + x*4 + 0 ] = clr[2];
				ptr[ y*rect.Pitch + x*4 + 1 ] = clr[1];
				ptr[ y*rect.Pitch + x*4 + 2 ] = clr[0];
				ptr[ y*rect.Pitch + x*4 + 3 ] = clr[3];
			}
		}
	
	HRCALL( d3d_tex->UnlockRect(0) );
	
	return d3d_tex;
}


//
//	ED3DRenderSystem::LoadTextureFromFile
//
LPDIRECT3DTEXTURE9 ESciVis::LoadTextureFromFile( const EString name )
{
	//
	//	Read data from disc :
	//	
	IPxFile	file	=	FileSystem()->FileOpen( name.CStr(), FS_OPEN_READ );
	IPxBlob	blob	=	file->BinaryBlob();
	
	uint	file_size	=	blob->GetBufferSize();
	void	*file_data	=	blob->GetBufferPointer();

	//	
	//	Retrieve image data :
	//
	D3DXIMAGE_INFO	info;


	if ( FAILED( D3DXGetImageInfoFromFileInMemory(file_data, file_size, &info) ) ) {
		RAISE_EXCEPTION(va("bad texture file '%s'\r\n", name.CStr()));
	}


	//	TODO : add D3DRTYPE_VOLUMETEXTURE :	
	if (info.ResourceType==D3DRTYPE_TEXTURE) 
	{
		LPDIRECT3DTEXTURE9 d3d_tex;

		if ( FAILED( D3DXCreateTextureFromFileInMemory(d3ddev, file_data, file_size, &d3d_tex) ) ) {
			RAISE_EXCEPTION(va("failed to create 2D texture from '%s'\r\n", name.CStr()));
		} else {
			return d3d_tex;
		}

	} else {
		RAISE_EXCEPTION("Unsupported resource type");
	}
	
	ASSERT(0);
	return NULL;
}