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


#pragma once

/*-----------------------------------------------------------------------------
	Lua vector math :
-----------------------------------------------------------------------------*/

CORE_API	void	LuaOpenVMath	( lua_State *L );
CORE_API	void	LuaCloseVMath	( lua_State *L );

CORE_API	void	LuaPushVec4		( lua_State *L, const EVec4 &v );
CORE_API	bool	LuaIsVec4		( lua_State *L, int idx );
CORE_API	EVec4	LuaToVec4		( lua_State *L, int idx );
CORE_API	EVec4	LuaRequireVec4	( lua_State *L, int idx, const char *what );

CORE_API	void	LuaPushQuat		( lua_State *L, const EQuat &v );
CORE_API	bool	LuaIsQuat		( lua_State *L, int idx );
CORE_API	EQuat	LuaToQuat		( lua_State *L, int idx );
CORE_API	EQuat	LuaRequireQuat	( lua_State *L, int idx, const char *what );



inline EVec4 LuaRequireFieldVec4( lua_State *L, int table_index, const char *field )
{
	lua_getfield(L, table_index, field);
	if (!LuaIsVec4(L, -1)) {
		lua_pop(L, 1);
		LuaArgumentError(L, table_index, "table[] vec4", field);
		return 0;
	} else {
		EVec4 value = LuaToVec4(L, -1);
		lua_pop(L, 1);
		return value;
	}
}


