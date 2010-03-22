/*
    The MIT License

    Copyright (c) 2009 IFMO/GameDev Studio

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
	CFunction aux functions :
-----------------------------------------------------------------------------*/

inline void LuaDefineCFunction( lua_State *L, const char *name, lua_CFunction cfunc, void *self )
{
	lua_pushlightuserdata(L, self);
	lua_pushcclosure(L, cfunc, 1);
	lua_setglobal(L, name);
}

inline void *LuaGetSelf( lua_State *L ) 
{
	return lua_touserdata(L, lua_upvalueindex(1));
}



/*-----------------------------------------------------------------------------
	Lua argument checking :
-----------------------------------------------------------------------------*/

inline void *LuaToUData( lua_State *L, int idx, const char *tname)
{
	void *p = lua_touserdata(L, idx);
	if (p != NULL) {  /* value is a userdata? */
		if (lua_getmetatable(L, idx)) {  /* does it have a metatable? */
			lua_getfield(L, LUA_REGISTRYINDEX, tname);  /* get correct metatable */
			if (lua_rawequal(L, -1, -2)) {  /* does it have the correct mt? */
				lua_pop(L, 2);  /* remove both metatables */
				return p;
			}
		}
	}
	return NULL;
}


inline bool LuaIsUData( lua_State *L, int idx, const char *tname)
{
	if (LuaToUData(L, idx, tname)) {
		return true;
	} else {
		return false;
	}
}


inline void LuaArgumentError( lua_State *L, int arg, const char *type, const char *what ) 
{
	luaL_error(L, "arg #%d must be %s - %s", arg, type, what);
}


inline const char *LuaRequireString( lua_State *L, int arg, const char *what ) 
{
	if (!lua_isstring(L, arg)) {
		LuaArgumentError(L, arg, "string", what);
		return NULL;
	} else {
		return lua_tostring(L, arg);
	}
}


inline double LuaRequireNumber( lua_State *L, int arg, const char *what ) 
{
	if (!lua_isnumber(L, arg)) {
		LuaArgumentError(L, arg, "number", what);
		return 0;
	} else {
		return lua_tonumber(L, arg);
	}
}


inline bool LuaRequireBoolean( lua_State *L, int arg, const char *what ) 
{
	if (!lua_isboolean(L, arg)) {
		LuaArgumentError(L, arg, "boolean", what);
		return false;
	} else {
		return lua_toboolean(L, arg);
	}
}


inline void LuaRequireUserData( lua_State *L, int arg, const char *what ) 
{
	if (!lua_isuserdata(L, arg)) {
		LuaArgumentError(L, arg, "UD", what);
	}
}


inline void *LuaRequireUData( lua_State *L, int arg, const char *tname, const char *what ) 
{
	void *p = LuaToUData(L, arg, tname);
	if (!p) {
		LuaArgumentError(L, arg, "UD", what);
	}
	return p;
}


/*-----------------------------------------------------------------------------
	Lua table accessing :
-----------------------------------------------------------------------------*/

inline void LuaRequireTable( lua_State *L, int arg, const char *what ) 
{
	if (!lua_istable(L, arg)) {
		LuaArgumentError(L, arg, "table", what);
	}
}


inline double LuaRequireFieldNumeric( lua_State *L, int table_index, const char *field )
{
	lua_getfield(L, table_index, field);
	if (!lua_isnumber(L, -1)) {
		lua_pop(L, 1);
		LuaArgumentError(L, table_index, "table[] number", field);
		return 0;
	} else {
		double value = lua_tonumber(L, -1);
		lua_pop(L, 1);
		return value;
	}
}


inline bool LuaRequireFieldBoolean( lua_State *L, int table_index, const char *field )
{
	lua_getfield(L, table_index, field);
	if (!lua_isboolean(L, -1)) {
		lua_pop(L, 1);
		LuaArgumentError(L, table_index, "table[] boolean", field);
		return 0;
	} else {
		bool value = lua_toboolean(L, -1);
		lua_pop(L, 1);
		return value;
	}
}


inline EName LuaRequireFieldString( lua_State *L, int table_index, const char *field )
{
	lua_getfield(L, table_index, field);
	if (!lua_isstring(L, -1)) {
		lua_pop(L, 1);
		LuaArgumentError(L, table_index, "table[] string", field);
		return 0;
	} else {
		EName value = lua_tostring(L, -1);
		lua_pop(L, 1);
		return value;
	}
}


/*-----------------------------------------------------------------------------
	Working with methods :
-----------------------------------------------------------------------------*/

inline void LuaRegisterMethods	( lua_State *L, const char *classname, const luaL_Reg *api )
{
	luaL_newmetatable(L, classname);
	lua_pushvalue(L, -1);				// push metatable
	lua_setfield(L, -2, "__index");		// metatable.__index = metatable
	luaL_register(L, NULL, api);		// file methods
} 

