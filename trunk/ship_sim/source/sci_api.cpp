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
	Nano vis :
-----------------------------------------------------------------------------*/

//
//	ESciVis::RegisterAPI
//
void ESciVis::RegisterAPI( void )
{
	lua_State *L = CoreLua();
	LUA_INTERFACE(L);
	
	LUALIB_DECLARE	("naval")
	LUALIB_METHOD	( remove_all_ships )
	LUALIB_METHOD	( set_view )
	LUALIB_METHOD	( get_wave_offset )
	LUALIB_METHOD	( set_wind )
	LUALIB_METHOD	( set_wave )
	LUALIB_REGISTER	( L )

	//	add 'naval.create_ship' :
	CoreExecuteString("naval.create_ship = ELuaShip;");
}


//
//	ESciVis::remove_all_ships
//
int ESciVis::remove_all_ships( lua_State *L )
{
	LUA_INTERFACE(L);
	ELuaShip::Unregister(L);
	ELuaShip::Register(L);
	self->ships.clear();
	return 0;
}


//
//	ESciVis::set_view
//
int ESciVis::set_view( lua_State *L )
{
	LUA_INTERFACE(L);
	
	float	f		=	LuaRequireNumber(L, 1, "FOV");
	EVec4	p		=	LuaRequireVec4	(L, 2, "view position");
	float	yaw		=	LuaRequireNumber(L, 3, "yaw");
	float	pitch	=	LuaRequireNumber(L, 4, "pitch");
	float	roll	=	LuaRequireNumber(L, 5, "roll");
	
	if (f>=180) {
		LOG_WARNING("set_view() : FOV >= 180");
		f = 179;
	}

	EQuat	z_up		=	QuatRotationAxis( -PI/2.0f, EVec3(0,0,1)) * QuatRotationAxis(PI/2.0, EVec3(1,0,0));
	EQuat	qrx			=	QuatRotationAxis( vmath::rad(roll),	EVec3(1,0,0) );
	EQuat	qry			=	QuatRotationAxis( vmath::rad(pitch),	EVec3(0,1,0) );
	EQuat	qrz			=	QuatRotationAxis( vmath::rad(yaw),		EVec3(0,0,1) );

	self->view.fov		=	f;
	self->view.position	=	p;
	self->view.orient	=	qrz * qry * qrx * z_up;
	
	return 0;
}


//
//	ESciVis::set_wind
//
int ESciVis::set_wind( lua_State *L )
{
	LUA_INTERFACE(L);
	self->waving->SetWindSpeed( lua.RequireNumber(1, "wind velocity") );
	return 0;
}


//
//	ESciVis::set_wind
//
int ESciVis::set_wave( lua_State *L )
{
	LUA_INTERFACE(L);
	float w = lua.RequireNumber(1, "w");
	float a = lua.RequireNumber(2, "a");
	self->waving->SetSinWave(w,a);
	return 0;
}


//
//	ESciVis::get_wave_offset
//
int ESciVis::get_wave_offset( lua_State *L )
{
	LUA_INTERFACE(L);
	float	x		=	LuaRequireNumber(L, 1, "x");
	float	y		=	LuaRequireNumber(L, 2, "y");
	float	z		=	LuaRequireNumber(L, 3, "z");
	
	EVec4 offs = self->waving->GetPosition(EVec4(x,y,z,1));
	
	lua_pushnumber(L, offs.z);
	
	lua.SetStackGrow(1);
	
	return 1;
}






