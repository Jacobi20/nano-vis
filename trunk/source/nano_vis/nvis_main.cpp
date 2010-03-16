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


#include "nvis_local.h"


DLL_EXPORT INanoVis	*CreateNanoVis	( void ) {	return new ENanoVis(); }


/*-----------------------------------------------------------------------------
	Nano vis :
-----------------------------------------------------------------------------*/

//
//	ENanoVis::ENanoVis
//
ENanoVis::ENanoVis( void )
{
	LOG_SPLIT("NanoVis initialization");
	
	InitDisplay();
	InitDirect3D();
	
	InitAtomRend();
	
	LOG_SPLIT("");
}


//
//	ENanoVis::~ENanoVis
//
ENanoVis::~ENanoVis( void )
{
	LOG_SPLIT("NanoVis shutting down");
	
	ShutdownAtomRend();
	
	ShutdownDirect3D();
	ShutdownDisplay();
	
	LOG_SPLIT("");
}


//
//	ENanoVis::RenderFrame
//
void ENanoVis::RenderFrame( uint dtime )
{
	int clear_flags = D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL;
	HRCALL( d3ddev->Clear(0, NULL, clear_flags, D3DCOLOR_ARGB(0x00,0xF0,0x00,0x80), 1.0f, 0 ) );	

    HRCALL( d3ddev->BeginScene() );

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
ID3DXEffect *ENanoVis::CompileEffect( const char *path )
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

