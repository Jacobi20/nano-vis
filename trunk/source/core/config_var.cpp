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
//  global.h    -   global definitions


#include "../core/core.h"
#include "config.h"

/*-----------------------------------------------------------------------------
	Config facility (variables) :
-----------------------------------------------------------------------------*/

//
//	EVar::EVar
//
EVar::EVar( const char *name, uint flags )
{
   this->name	=	name;
   this->flags	=	flags;
   
   L	=	Linker()->GetShell()->Lua();
   lua_pushnil(L);
   lua_setfield(L, LUA_GLOBALSINDEX, name);
}


//
//	EVar::~ECvar
//
EVar::~EVar( void )
{
}


//
//	EVar::Set
//
void EVar::Set( bool value )
{
	lua_pushboolean(L, value);
	lua_setglobal(L, name.Name());
}


//
//	EVar::Set
//
void EVar::Set( int value )
{
	lua_pushnumber(L, (double)value);
	lua_setglobal(L, name.Name());
}


//
//	EVar::Set
//
void EVar::Set( float value )
{
	lua_pushnumber(L, (double)value);
	lua_setglobal(L, name.Name());
}


//
//	EVar::Set
//
void EVar::Set( const char *value )
{
	lua_pushstring(L, value);
	lua_setglobal(L, name.Name());
}


//
//	EVar::Nil
//
bool EVar::Nil( void ) const
{
	lua_getglobal(L, name.Name());
	bool result = lua_isnil(L, -1);
	lua_pop(L, 1);
	return result;
}


//
//	EVar::Bool
//
bool EVar::Bool( void ) const
{
	lua_getglobal(L, name.Name());
	bool result = (lua_toboolean(L, -1)!=0);
	lua_pop(L, 1);
	return result;
}


//
//	EVar::Int
//
int EVar::Int( void ) const
{
	lua_getglobal(L, name.Name());
	int result = lua_tonumber(L, -1);
	lua_pop(L, 1);
	return result;
}


//
//	EVar::Float
//
float EVar::Float( void ) const
{
	lua_getglobal(L, name.Name());
	float result = lua_tonumber(L, -1);
	lua_pop(L, 1);
	return result;
}


//
//	EVar::String
//
const char * EVar::String( void ) const
{
	lua_getglobal(L, name.Name());
	
	const char *s = lua_tostring(L, -1);
	if (s) {
		strval	=	s;
	} else {
		strval	=	"";
	}
	lua_pop(L, 1);
	return strval.c_str();
}