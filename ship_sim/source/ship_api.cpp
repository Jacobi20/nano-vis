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

/*-----------------------------------------------------------------------------
	Ship Lua API :
-----------------------------------------------------------------------------*/

IShip	*create_ship	(lua_State *L, int idx);


ELuaShip::ELuaShip( lua_State *L )
{
	ship	=	create_ship(L, 1);
	ESciVis::self->AddShip( ship );
}


ELuaShip::~ELuaShip( void )
{
	ESciVis::self->RemoveShip( ship );
	ship	=	NULL;
}


LUNA_IMP_BEGIN	(ELuaShip					)
LUNA_IMP_METHOD	(ELuaShip,	set_vis_mesh	)
LUNA_IMP_METHOD	(ELuaShip,	set_hsf_mesh	)
LUNA_IMP_METHOD	(ELuaShip,	set_hdf_mesh	)
LUNA_IMP_METHOD	(ELuaShip,	set_resistance	)
LUNA_IMP_METHOD	(ELuaShip,	make_rigidbody	)
LUNA_IMP_METHOD	(ELuaShip,	build_voxels	)
LUNA_IMP_METHOD	(ELuaShip,	build_surf_dxdy	)
LUNA_IMP_METHOD	(ELuaShip,	add_force		)
LUNA_IMP_METHOD	(ELuaShip,	add_momentum	)
LUNA_IMP_METHOD	(ELuaShip,	get_angles		)
LUNA_IMP_METHOD	(ELuaShip,	get_position	)
LUNA_IMP_METHOD	(ELuaShip,	set_angles		)
LUNA_IMP_METHOD	(ELuaShip,	set_position	)
LUNA_IMP_METHOD	(ELuaShip,	get_hsf_momentum)
LUNA_IMP_METHOD	(ELuaShip,	get_hsf_force	)
LUNA_IMP_METHOD	(ELuaShip,	get_center_mass	)
LUNA_IMP_METHOD	(ELuaShip,	get_right_arm	)
LUNA_IMP_METHOD	(ELuaShip,	simulate		)
LUNA_IMP_METHOD	(ELuaShip,	set_cmass		)
LUNA_IMP_END	(ELuaShip					)

/*-----------------------------------------------------------------------------
	API itself :
-----------------------------------------------------------------------------*/

int ELuaShip::set_vis_mesh( lua_State *L )
{
	LUA_INTERFACE(L);
	ship->SetVisMesh( lua.RequireString( 1, "vis mesh path") );
	return 0;
}


int ELuaShip::set_hsf_mesh( lua_State *L )
{
	LUA_INTERFACE(L);
	ship->SetHSFMesh( lua.RequireString( 1, "HSF mesh path") );
	return 0;
}


int ELuaShip::set_hdf_mesh( lua_State *L )
{
	LUA_INTERFACE(L);
	ship->SetHDFMesh( lua.RequireString( 1, "HDF mesh path") );
	return 0;
}


int ELuaShip::set_resistance( lua_State *L )
{
	LUA_INTERFACE(L);
	ship->SetResistance( lua.RequireNumber( 1, "water resistance coefficient") );
	return 0;
}


int ELuaShip::make_rigidbody( lua_State *L )
{
	LUA_INTERFACE(L);
	ship->MakeRigidBody( lua.RequireString( 1, "RB mesh path"), lua.RequireNumber( 2, "ship mass" ) );
	return 0;
}


int ELuaShip::build_voxels( lua_State *L )
{
	LUA_INTERFACE(L);
	ship->BuildVoxels( lua.RequireString( 1, "mesh path"), lua.RequireNumber( 2, "voxel size" ) );
	return 0;
}


int ELuaShip::build_surf_dxdy( lua_State *L )
{
	LUA_INTERFACE(L);
	ship->BuildSurfaceDXDY( lua.RequireString( 1, "mesh path"), lua.RequireNumber( 2, "density" ) );
	return 0;
}


int ELuaShip::add_force( lua_State *L )
{
	LUA_INTERFACE(L);
	
	int n = lua_gettop(L);

	EVec4	f		=	LuaRequireVec4( L, 1, "force vector" );
	EVec4	p		=	LuaRequireVec4( L, 2, "point of force addition" );
	bool	local	=	LuaRequireBoolean( L, 3, "is point local");
	
	ship->AddForce( EVec3(f.x, f.y, f.z), EVec3(p.x, p.y, p.z), local);
	
	return 0;
}


int ELuaShip::add_momentum( lua_State *L )
{
	LUA_INTERFACE(L);
	
	int n = lua_gettop(L);

	EVec4	m		=	LuaRequireVec4( L, 1, "momentum vector" );
	bool	local	=	LuaRequireBoolean( L, 2, "is local space momentum");
	
	ship->AddMomentum( EVec3(m.x, m.y, m.z), local);
	
	return 0;
}


int ELuaShip::get_angles( lua_State *L )
{
	LUA_INTERFACE(L);
	EQuat q;
	EVec4 p;
	ship->GetPose( p, q );
	float yaw, pitch, roll;
	
	QuatToAngles( q, yaw, pitch, roll );
	
	lua_pushnumber( L, yaw );
	lua_pushnumber( L, pitch );
	lua_pushnumber( L, roll );
	lua.SetNResults(3);
	return 3;
}


int ELuaShip::get_position( lua_State *L )
{
	LUA_INTERFACE(L);
	EQuat q;
	EVec4 p;
	ship->GetPose( p, q );
	
	lua_pushnumber( L, p.x );
	lua_pushnumber( L, p.y );
	lua_pushnumber( L, p.z );
	lua.SetNResults(3);
	return 3;
}


int ELuaShip::set_angles( lua_State *L )
{
	LUA_INTERFACE(L);
	EQuat q;
	EVec4 p;
	ship->GetPose( p, q );

	float yaw	=	lua.RequireNumber( 1, "yaw"		);
	float pitch	=	lua.RequireNumber( 2, "pitch"	);
	float roll	=	lua.RequireNumber( 3, "roll"	);
	q			=	QuatRotationAxis( deg2rad( yaw   ),	EVec3(0,0,1))
				*	QuatRotationAxis( deg2rad( pitch ),	EVec3(0,1,0))
				*	QuatRotationAxis( deg2rad( roll  ),	EVec3(1,0,0));

	
	ship->SetPose( p, q );	
	return 0;
}


int ELuaShip::set_position( lua_State *L )
{
	LUA_INTERFACE(L);
	EQuat q;
	EVec4 p;
	ship->GetPose( p, q );

	float x		=	lua.RequireNumber( 1, "x position"		);
	float y		=	lua.RequireNumber( 2, "y position"	);
	float z		=	lua.RequireNumber( 3, "z position"	);

	p	=	EVec4(x,y,z,1);
	
	ship->SetPose( p, q );	
	return 0;
}


int ELuaShip::get_hsf_momentum( lua_State *L )
{
	LUA_INTERFACE(L);
	EVec3 m = ship->GetHSFMomentum();

	LuaPushVec4( L, EVec4(m.x, m.y, m.z, 0) );
	
	lua.SetNResults(1);
	return 1;
}


int ELuaShip::get_hsf_force( lua_State *L )
{
	LUA_INTERFACE(L);
	float f = ship->GetHSFForce();

	lua_pushnumber(L, f);

	lua.SetNResults(1);
	return 1;
}


int ELuaShip::get_center_mass( lua_State *L )
{
	LUA_INTERFACE(L);
	EVec3 cm = ship->GetCenterMass();

	LuaPushVec4( L, EVec4(cm.x, cm.y, cm.z, 0) );

	lua.SetNResults(1);
	return 1;
}


int ELuaShip::get_right_arm( lua_State *L )
{
	LUA_INTERFACE(L);
	float ra = ship->GetRightArm();

	lua_pushnumber(L, ra);

	lua.SetNResults(1);
	return 1;
}


int ELuaShip::simulate( lua_State *L )
{
	LUA_INTERFACE(L);
	float dt = lua.RequireNumber(1, "dtime");
	
	ship->Simulate( dt, NULL );
	
	return 0;
}


int ELuaShip::set_cmass( lua_State *L )
{
	LUA_INTERFACE(L);
	float x		=	lua.RequireNumber( 1, "cmass X position"		);
	float y		=	lua.RequireNumber( 2, "cmass Y position"	);
	float z		=	lua.RequireNumber( 3, "cmass Z position"	);
	
	ship->SetCMass(x,y,z);
	
	return 0;
}