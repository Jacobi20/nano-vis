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

#include "openbabel/mol.h"
#include "openbabel/obconversion.h"

#pragma comment( lib, "../ob/lib/openbabel-2.lib" )

using namespace OpenBabel;

/*-----------------------------------------------------------------------------
	Atom visializer :
-----------------------------------------------------------------------------*/

//
//	ENanoVis::InitAtomRend
//
void ENanoVis::InitAtomRend( void )
{
	LOG_INIT("Atom renderer");
	
	atom_fx	=	CompileEffect("atom.fx");
	
	HRCALL( D3DXCreateSphere( d3ddev, 1.0f, 32, 16,		&mesh_ball, NULL) );
	HRCALL( D3DXCreateCylinder( d3ddev, 1.0f, 1.0f, 1.0f, 16, 1.0f, &mesh_stick, NULL) );
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


//
//	ENanoVis::LoadData
//
void ENanoVis::LoadData( const char *path )
{
	OBConversion	conv;
	OBMol			mol;
	VERIFY( conv.SetInFormat("CUBE") );
	VERIFY( conv.ReadFile(&mol, path) );
}


