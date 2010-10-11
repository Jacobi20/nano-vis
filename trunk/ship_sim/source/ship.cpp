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
#include "ship.h"

#undef fs


IShip * create_ship( lua_State *L, int idx ) {
	return new EShip(L, idx);
}	

/*-----------------------------------------------------------------------------
	Naive ship simulator :
-----------------------------------------------------------------------------*/

EShip::EShip( lua_State *L, int idx )
{
	self_time	=	0;
	
	
	CONFIG_REGISTER_VAR(ship_show_hull,		true		);
	CONFIG_REGISTER_VAR(ship_show_voxels,	false		);
	CONFIG_REGISTER_VAR(ship_show_submerge, false		);
	CONFIG_REGISTER_VAR(ship_hsf_method,	"surface"	);
	
	SetCMass( 0, 0, 0 );
	
	SetResistance( 0 );
}


//
//	EShip::~EShip
//
EShip::~EShip( void )
{
	phys()->RemoveEntity( ship_body );
	ship_body = NULL;

	sci_vis->GetFRScene()->RemoveEntity( r_ent );
	sci_vis->GetFRScene()->RemoveEntity( r_ent2 );
}


//
//	EShip::Simulate
//
void EShip::Simulate( float dtime, IPxWaving waving )
{
	self_time += dtime;
	UpdateForces( dtime, waving );

	EVec4 p;
	EQuat q;	
	GetPose(p, q);
	r_ent->SetPose(p, q);
}


//
//	EShip::GetPose
//
void EShip::GetPose( EVec4 &position, EQuat &orient )
{
	ship_body->GetPose( position, orient );
}


//
//	EShip::SetPose
//
void EShip::SetPose( EVec4 &position, EQuat &orient )
{
	ship_body->SetPose( position, orient );
}



/*-----------------------------------------------------------------------------
	Forces :
-----------------------------------------------------------------------------*/

//
//	EShip::UpdateForces
//
void EShip::UpdateForces( float dtime, IPxWaving waving )
{
	if (!mesh_hdf)	{	RUNTIME_ERROR("HDF mesh not created");		}
	if (!mesh_hsf)	{	RUNTIME_ERROR("HSF mesh not created");		}
	if (!ship_body)	{	RUNTIME_ERROR("ship rigid body not created");	}

	float  weight	=	GRAVITY * ship_mass;
	EVec4 gravity	=	EVec4(0, 0, -weight,0);
	
	EQuat	q;
	EVec4	p;
	GetPose(p, q);
	
	EMatrix4 world		=	QuatToMatrix(q) * Matrix4Translate(p);
	mesh_submerged_hsf	=	GetSubmergedMesh( world, EPlane(0,0,1,0) );	

	ship_body->AddForceAtLocalPos( gravity, cmass );

	if (!waving) {
		waving	=	sci_vis->waving;
	}	
	
	UpdateHSF( dtime, waving );
	UpdateHDF( dtime, waving );
}


//
//	EShip::AddForce
//
void EShip::AddForce( EVec3 force, EVec3 point, bool local_point )
{
	EVec4 p = Vec3ToVec4( point );
	EVec4 f = Vec3ToVec4( force );
	
	if (local_point) {
		ship_body->AddForceAtLocalPos( f, p );
	} else {
		ship_body->AddForceAtPos( f, p );
	}
}


//
//	EShip::AddMomentum
//
void EShip::AddMomentum( EVec3 momentum, bool local_momentum )
{
	//if (local_momentum) {
	//	ship_body->addLocalTorque( ToNxVec3( momentum ) );
	//} else {
	//	ship_body->addTorque( ToNxVec3( momentum ) );
	//}
}


EVec3 EShip::GetCenterMass( void )
{
	return Vec4ToVec3( ship_body->GetCMOffset() );
}


EVec3 EShip::GetInertiaMomentum( EVec3 axis )
{
	return EVec3( 0, 0, 0 );
}


//
//	EShip::SetCMass
//
void EShip::SetCMass( float x, float y, float z )
{
	cmass.x	=	x;
	cmass.y	=	y;
	cmass.z	=	z;
	cmass.w	=	0;
}