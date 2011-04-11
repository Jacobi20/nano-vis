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
	
	view.center	=	EPoint(-15,-15, 5);
	view.yaw	=	45;
	view.pitch	=	15;

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
	rs()->GetFRScene()->SetAmbientLevel( (EColor::kWhite + EColor::kBlue) * 0.0 ) ;
	
	light	=	rs()->GetFRScene()->AddLight();
	light->SetColor( EColor::kWhite * 4 );
	light->SetPose( EPoint(0,0,15), EQuaternion::kIdentity );
	light->SetRadius( 30, 30 );
	light->SetType( RS_LIGHT_SPOT_SHADOW );
	light->SetMask("mask.tga");
	//light->SetType( RS_LIGHT_OMNI );

	EMath::Randf();
	

	//	create magnets :	
	//for ( float x=-9; x<9; x+=3 ) {
	//	for ( float y=-9; y<9; y+=3 ) {
	for ( float x=-3; x<15; x+=5 ) {
		for ( float y=-3; y<=+7; y+=6 ) {
		
			Magnet magnet;
			IPxTriMesh	vis_mesh	=	ge()->LoadMeshFromFile("scene.esx|vis_magnet");
			IPxTriMesh	phys_mesh	=	ge()->LoadMeshFromFile("scene.esx|phys_magnet");
			EPoint		p			=	EPoint(x,y,EMath::Randf(2,5));
			EQuaternion	q			=	EQuaternion::FromAngles(EMath::Randf(-180,180), EMath::Randf(-180,180), EMath::Randf(-180,180));
			//EQuaternion	q			=	EQuaternion::FromAngles(EMath::Randf(-5,5), EMath::Randf(-5,5), EMath::Randf(-5,5));
			
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

	volume = rs()->GetFRScene()->AddVolume();
	volume->LoadColormap("volume/iso.png");

	volume->SetBounds(EBBox(EPoint(), 20, 20, 20));
	volume->SetResolution(32, 32, 32);
	volume->SetInterestRange(-0.2, 0.2);

	//	INFO : uncomment this to add gravity :
	//phys()->SetGravity( EVector::kNegZ );
}


//
//	MagnetGame::~MagnetGame
//
MagnetGame::~MagnetGame( void )
{
	rs()->Uninstall();
}


//	INFO : change this value  :
const float MAGNETIC_CONST	=	200.0f;

EVector	MagneticForce( const EPoint &a, const EPoint &b ) 
{
	EVector	ab		=	EVector(a, b);
	float	len2	=	ab.LengthSqr();
	ab.NormalizeSelf();
	ab = ab * (MAGNETIC_CONST / (1+len2));
	return ab;
}


//
//	MagnetGame::Frame
//
void MagnetGame::Frame( uint dtime )
{
	time += dtime;
	
	DEBUG_STRING("fps : %f", 1000.0f / dtime );
	
	IInputSystem::S3DMouseInput	s3dmin;
	InputSystem()->GetS3DMouseInput( &s3dmin );

	view.yaw	+=	0.01f * s3dmin.rotate[2];
	view.pitch	+=	0.01f * s3dmin.rotate[0];
	view.roll	+=	0.01f * s3dmin.rotate[1];
	view.pitch	=	clamp<float>( view.pitch, 5, 90 );
	
	
	SetupCamera();
	
	for (uint i=0; i<magnets.size(); i++) {
		
		EPoint		p;	
		EQuaternion	q;	
		
		magnets[i].phys_obj->GetPose(p, q);
		magnets[i].rend_obj->SetPose(p, q);
	}
	
	for (uint i=0; i<magnets.size(); i++) {

		IPxPhysEntity	a	=	magnets[i].phys_obj;

		EPoint		p;	
		EQuaternion	q;	
		
		a->GetPose(p, q);
		
		EPoint pp_a	=	EPoint( 0.5,0,0).Transform( EMatrix::FromPose(p,q) );
		EPoint pn_a	=	EPoint(-0.5,0,0).Transform( EMatrix::FromPose(p,q) );
		magnets[i].pp = pp_a;
		magnets[i].pn = pn_a;
		//rs()->GetDVScene()->DrawPoint( pp_a, 0.25, EColor::kRed );
		//rs()->GetDVScene()->DrawPoint( pn_a, 0.25, EColor::kBlue );

		for (uint j=0; j<magnets.size(); j++) {
			IPxPhysEntity	b	=	magnets[j].phys_obj;

			EPoint		p;	
			EQuaternion	q;	
			
			b->GetPose(p, q);

			EPoint pp_b	=	EPoint( 0.5,0,0).Transform( EMatrix::FromPose(p,q) );
			EPoint pn_b	=	EPoint(-0.5,0,0).Transform( EMatrix::FromPose(p,q) );
			
			EVector	fpp	=	MagneticForce(pp_a, pp_b);
			EVector	fnn	=	MagneticForce(pn_a, pn_b);
			EVector	fpn	=	MagneticForce(pp_a, pn_b) * (-1);
			EVector	fnp	=	MagneticForce(pn_a, pp_b) * (-1);
			
			b->AddForceAtPos( fpp, pp_b );
			b->AddForceAtPos( fnn, pn_b );
			b->AddForceAtPos( fnp, pp_b );
			b->AddForceAtPos( fpn, pn_b );
		}
	}

	volume->UpdateData(this);
}


/*-----------------------------------------------------------------------------
	Internal stuff :
-----------------------------------------------------------------------------*/

void MagnetGame::SetupCamera( void )
{
	uint w, h;
	rs()->GetScreenSize(w, h);
	
	float	t	=	time / 1000.0f;
	float	a	=	view.yaw;
	float	b	=	view.pitch;
	float	x	=	15 * cos(EMath::Rad(a)) * cos(EMath::Rad(b));
	float	y	=	15 * sin(EMath::Rad(a)) * cos(EMath::Rad(b));
	float	z	=	15 * sin(EMath::Rad(b));
	
	EQuaternion	z_up	=	EQuaternion::RotateAroundAxis( -PI/2.0f, EVector(0,0,1)) * EQuaternion::RotateAroundAxis(PI/2.0, EVector(1,0,0));
	EPoint			p	=	EPoint( x,y,z );
	EQuaternion		q	=	EQuaternion::FromAngles( 180+a, b, 0) * z_up;
	
	rs()->GetFRScene()->SetProjection( 0.1f, 1000.0f, 2000.0f, 2000.0f / w * h );
	rs()->GetFRScene()->SetView( p, q );
	
	rs()->GetDVScene()->SetProjection( 0.1f, 1000.0f, 2000.0f, 2000.0f / w * h );
	rs()->GetDVScene()->SetView( p, q );
}


/*-----------------------------------------------------------------------------
	FRVolume::DataProvider :
-----------------------------------------------------------------------------*/

float MagnetGame::Value( const EPoint &local_point ) const
{
	float val = 0.0;
	for (int i = 0; i < magnets.size(); ++i)
	{
		float dn = magnets[i].pn.Distance(local_point);
		float dp = magnets[i].pp.Distance(local_point);
		val += ( 1.0 / dp - 1.0 / dn );
	}
	return val;
}