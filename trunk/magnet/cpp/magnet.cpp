/*
	The MIT License

	Copyright (c) 2011 Escience Research Institute

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

#include "../../engine/engine.h"
#include "magnet.h"

DLL_EXPORT	IGame *CreateGame() { return new MagnetGame(); }

/*-----------------------------------------------------------------------------
	MAGNET
-----------------------------------------------------------------------------*/

//
//	MagnetGame::MagnetGame
//
MagnetGame::MagnetGame( void )
{
	time	=	0;

	rs()->Install();
	
	InputSystem()->SetInputMode( IN_KB_SCAN );

	CoreExecuteString("require('shaders')");

	//	load base mesh :	
	IPxTriMesh	base_mesh	=	ge()->LoadMeshFromFile("scene.esx|base");

	//	create visible base object :	
	base_rend	=	rs()->GetFRScene()->AddEntity();
	base_rend->SetMesh( base_mesh );

	//	create physical base object	
	base_phys	=	phys()->AddEntity();
	base_phys->SetMesh( base_mesh );
	base_phys->SetStatic();
	base_phys->Spawn();
	
	//	create light :
	light	=	rs()->GetFRScene()->AddLight();
	light->SetColor( EColor::kWhite * 4 );
	light->SetPose( EPoint(0,0,15), EQuaternion::kIdentity );
	light->SetRadius( 20, 20 );
	light->SetType( RS_LIGHT_SPOT_SHADOW );
	//light->SetType( RS_LIGHT_OMNI );

	//	create magnets :	
	for ( float x=-9; x<9; x+=3 ) {
		for ( float y=-9; y<9; y+=3 ) {
		
			Magnet magnet;
			IPxTriMesh	vis_mesh	=	ge()->LoadMeshFromFile("scene.esx|vis_magnet");
			IPxTriMesh	phys_mesh	=	ge()->LoadMeshFromFile("scene.esx|phys_magnet");
			EPoint		p			=	EPoint(x,y,3);
			EQuaternion	q			=	EQuaternion::FromAngles(EMath::Randf(-180,180), EMath::Randf(-180,180), EMath::Randf(-180,180));
			
			magnet.rend_obj	=	rs()->GetFRScene()->AddEntity();
			magnet.rend_obj->SetMesh( vis_mesh );
			magnet.rend_obj->SetPose( p, q );
			
			magnet.phys_obj	=	phys()->AddEntity();
			magnet.phys_obj->SetMass( 10 );
			magnet.phys_obj->SetMesh( phys_mesh );
			magnet.phys_obj->SetPose( p, q );
			magnet.phys_obj->SetDynamic();
			magnet.phys_obj->Spawn();
			
			magnets.push_back( magnet );
		
		}
	}
	
	phys()->SetGravity( EVector::kNegZ );
}


//
//	MagnetGame::~MagnetGame
//
MagnetGame::~MagnetGame( void )
{
	rs()->Uninstall();
}


//
//	MagnetGame::Frame
//
void MagnetGame::Frame( uint dtime )
{
	time += dtime;
	
	DEBUG_STRING("fps : %f", 1000.0f / dtime );
	
	SetupCamera();
	
	for (uint i=0; i<magnets.size(); i++) {
		
		EPoint		p;	
		EQuaternion	q;	
		
		magnets[i].phys_obj->GetPose(p, q);
		magnets[i].rend_obj->SetPose(p, q);
	}
}


/*-----------------------------------------------------------------------------
	Internal stuff :
-----------------------------------------------------------------------------*/

void MagnetGame::SetupCamera( void )
{
	uint w, h;
	rs()->GetScreenSize(w, h);
	
	float	t	=	time / 1000.0f;
	float	a	=	30.0f * t;
	float	x	=	20 * cos(EMath::Rad(a));
	float	y	=	20 * sin(EMath::Rad(a));
	float	z	=	10;
	
	EQuaternion	z_up	=	EQuaternion::RotateAroundAxis( -PI/2.0f, EVector(0,0,1)) * EQuaternion::RotateAroundAxis(PI/2.0, EVector(1,0,0));
	EPoint			p	=	EPoint(x,y,z);
	EQuaternion		q	=	EQuaternion::FromAngles(180+a,25,0) * z_up;
	
	rs()->GetFRScene()->SetProjection( 0.1f, 1000.0f, 2000.0f, 2000.0f / w * h );
	rs()->GetFRScene()->SetView( p, q );
}