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
	
	SetResistance( 0 );
}


//
//	EShip::~EShip
//
EShip::~EShip( void )
{
	if (ship_body) {
		sci_vis->GetNxScene()->releaseActor( *ship_body );
	}
	
	sci_vis->GetFRScene()->RemoveEntity( r_ent );
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
	NxQuat	nxq	=	ship_body->getGlobalOrientationQuat();
	NxVec3	nxv	=	ship_body->getGlobalPosition();
	EQuat	q	=	EQuat(nxq.x, nxq.y, nxq.z, nxq.w);
	EVec4	p	=	EVec4(nxv.x, nxv.y, nxv.z, 1);
	
	position	=	p;
	orient		=	q;
}


//
//	EShip::SetPose
//
void EShip::SetPose( EVec4 &position, EQuat &orient )
{
	ship_body->setGlobalOrientationQuat( ToNxQuat( orient ) );
	ship_body->setGlobalPosition( ToNxVec3( EVec3(position.x, position.y, position.z) ) );
}



/*-----------------------------------------------------------------------------
	Forces :
-----------------------------------------------------------------------------*/

//
//	EShip::UpdateForces
//
void EShip::UpdateForces( float dtime, IPxWaving waving )
{
	if (!mesh_hdf)	{	RAISE_EXCEPTION("HDF mesh not created");		}
	if (!mesh_hsf)	{	RAISE_EXCEPTION("HSF mesh not created");		}
	if (!ship_body)	{	RAISE_EXCEPTION("ship rigid body not created");	}

	float  weight	=	GRAVITY * ship_mass;
	NxVec3 gravity	=	NxVec3(0, 0, -weight);
	
	EQuat	q;
	EVec4	p;
	GetPose(p, q);
	
	EMatrix4 world		=	QuatToMatrix(q) * Matrix4Translate(p);
	mesh_submerged_hsf	=	GetSubmergedMesh( world, EPlane(0,0,1,0) );	
	
	NxVec3 cm = ship_body->getCMassLocalPosition();
	ship_body->addForceAtLocalPos( gravity, cm );

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
	NxVec3 p = ToNxVec3( point );
	NxVec3 f = ToNxVec3( force );
	
	if (local_point) {
		ship_body->addForceAtLocalPos( f, p );
	} else {
		ship_body->addForceAtPos( f, p );
	}
}


//
//	EShip::AddMomentum
//
void EShip::AddMomentum( EVec3 momentum, bool local_momentum )
{
	if (local_momentum) {
		ship_body->addLocalTorque( ToNxVec3( momentum ) );
	} else {
		ship_body->addTorque( ToNxVec3( momentum ) );
	}
}


EVec3 EShip::GetCenterMass( void )
{
	NxVec3 cm = ship_body->getCMassLocalPosition();
	return EVec3( cm.x, cm.y, cm.z );
}


EVec3 EShip::GetInertiaMomentum( EVec3 axis )
{
	return EVec3( 0, 0, 0 );
}