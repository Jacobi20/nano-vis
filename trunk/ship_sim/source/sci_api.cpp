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

static const luaL_Reg navallib[] = {
		{ "remove_all_ships",	ESciVis::remove_all_ships	},
		{ NULL, NULL	},
	};

//
//	ESciVis::RegisterAPI
//
void ESciVis::RegisterAPI( void )
{
	lua_State *L = CoreLua();
	LUA_INTERFACE(L);
	
	luaL_register(L, "naval", navallib);
	lua_pop(L, 1);

	//	add 'naval.create_ship' :
	CoreExecuteString("naval.create_ship = ELuaShip;");
}


//
//	ESciVis::remove_all_ships
//
int ESciVis::remove_all_ships( lua_State *L )
{
	ELuaShip::Unregister(L);
	ELuaShip::Register(L);
	self->ships.clear();
	return 0;
}


