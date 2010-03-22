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
// shell.cpp - 

#include "../core/core.h"
#include "shell.h"

#if 0

/*-----------------------------------------------------------------------------
	Shell vars :
-----------------------------------------------------------------------------*/

//
//	EVar::EVar
//
EVar::EVar( EShell *shell, const char *name, const char *value )
{
	this->shell = shell;
	this->L		= shell->Lua();
	this->name	= name;
	
	ASSERT(name);
	ASSERT(value);
	
	lua_getfield(L, LUA_GLOBALSINDEX, name);
	
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);

		lua_pushstring(L, value);
		lua_setfield(L, LUA_GLOBALSINDEX, name);
	} else {
		lua_pop(L, 1);
	}
}


//
//	EVar::~EVar
//
EVar::~EVar( void )
{

}


//
//	EVar::SetValue
//
void EVar::SetValue( const char *val )
{
	lua_pushstring(L, val);
	lua_setfield(L, LUA_GLOBALSINDEX, name.Name());
}


//
//	EVar::String
//
const char	* EVar::String( void ) const
{
	lua_getfield(L, LUA_GLOBALSINDEX, name.Name());
	
	if (const char *s = lua_tostring(L, -1)) {
		value	=	s;
	} else {
		value	=	"";
	}
	
	lua_pop(L, -1);
	
	return value.c_str();
}

#endif