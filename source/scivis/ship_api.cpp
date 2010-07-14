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
LUNA_IMP_METHOD	(ELuaShip,	add_force		)
LUNA_IMP_METHOD	(ELuaShip,	get_angles		)
LUNA_IMP_METHOD	(ELuaShip,	get_position	)
LUNA_IMP_METHOD	(ELuaShip,	set_angles		)
LUNA_IMP_METHOD	(ELuaShip,	set_position	)
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


int ELuaShip::add_force( lua_State *L )
{
	return 0;
}


int ELuaShip::get_angles( lua_State *L )
{
	EQuat q;
	EVec4 p;
	ship->GetPose( p, q );
	float yaw, pitch, roll;
	
	QuatToAngles( q, yaw, pitch, roll );
	
	lua_pushnumber( L, yaw );
	lua_pushnumber( L, pitch );
	lua_pushnumber( L, roll );
	return 3;
}


int ELuaShip::get_position( lua_State *L )
{
	EQuat q;
	EVec4 p;
	ship->GetPose( p, q );
	
	lua_pushnumber( L, p.x );
	lua_pushnumber( L, p.y );
	lua_pushnumber( L, p.z );
	return 3;
}


int ELuaShip::set_angles( lua_State *L )
{
	return 0;
}


int ELuaShip::set_position( lua_State *L )
{
	return 0;
}
