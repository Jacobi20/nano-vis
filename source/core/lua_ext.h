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

CORE_API bool		LuaGetField				( lua_State *L, int tidx, const char *key, bool  &value );
CORE_API bool		LuaGetField				( lua_State *L, int tidx, const char *key, int	 &value );
CORE_API bool		LuaGetField				( lua_State *L, int tidx, const char *key, uint	 &value );
CORE_API bool		LuaGetField				( lua_State *L, int tidx, const char *key, float &value );
CORE_API bool		LuaGetField				( lua_State *L, int tidx, const char *key, EName &value );
CORE_API bool		LuaGetField				( lua_State *L, int tidx, const char *key, EVec4 &value );
CORE_API bool		LuaGetField				( lua_State *L, int tidx, const char *key, EQuat &value );
CORE_API bool		LuaGetSubTable			( lua_State *L, int tidx, const char *key );

CORE_API void		LuaDefineCFunction		( lua_State *L, const char *name, lua_CFunction cfunc, void *self );
CORE_API void		*LuaGetSelf				( lua_State *L ) ;

CORE_API void		*LuaToUData				( lua_State *L, int idx, const char *tname);
CORE_API bool		LuaIsUData				( lua_State *L, int idx, const char *tname);

CORE_API void		LuaArgumentError		( lua_State *L, int arg, const char *type, const char *what ) ;
CORE_API const char *LuaRequireString		( lua_State *L, int arg, const char *what ) ;
CORE_API double		LuaRequireNumber		( lua_State *L, int arg, const char *what ) ;
CORE_API bool		LuaRequireBoolean		( lua_State *L, int arg, const char *what ) ;
CORE_API void		LuaRequireUserData		( lua_State *L, int arg, const char *what ) ;
CORE_API void		*LuaRequireUData		( lua_State *L, int arg, const char *tname, const char *what ) ;
CORE_API void		LuaRequireTable			( lua_State *L, int arg, const char *what ) ;
CORE_API double		LuaRequireFieldNumeric	( lua_State *L, int table_index, const char *field );
CORE_API bool		LuaRequireFieldBoolean	( lua_State *L, int table_index, const char *field );
CORE_API EName		LuaRequireFieldString	( lua_State *L, int table_index, const char *field );

CORE_API void		LuaRegisterMethods		( lua_State *L, const char *classname, const luaL_Reg *api );
