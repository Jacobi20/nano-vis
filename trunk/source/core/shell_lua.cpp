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

/*-----------------------------------------------------------------------------
	Shell Lua stuff :
-----------------------------------------------------------------------------*/

//
//	EShell::LuaWarning
//
int EShell::LuaWarning( const char *text )
{
	luaL_where(L, 1);

	LOG_WARNING("%s: %s", lua_tostring(L, -1), text);
	
	lua_pop(L, 1);
	
	return 0;
}


//
//	EShell::LuaError
//
int EShell::LuaError( const char *text )
{
	lua_Debug ar;

	string func_name;

	if (lua_getstack(L, 0, &ar)) {
		lua_getinfo(L, "nSlu", &ar);
		if (ar.name) {
			func_name = ar.name;
			func_name += "()";
		}
	}

	if (lua_getstack(L, 1, &ar)) {
		lua_getinfo(L, "nSlu", &ar);
		lua_pushstring(L, va("%s:%d: %s: %s", ar.short_src, ar.currentline, func_name.c_str(), text));
	} else {
		lua_pushstring(L, text);
	}
	return lua_error(L);
}


////
////	EShell::LuaRequireArg
////
//int EShell::LuaRequireArg( int arg, int type, const char *what )
//{
//	const char *type_name = "";
//
//	switch (type) {
//		
//	}
//}