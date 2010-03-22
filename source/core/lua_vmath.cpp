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

#include "../core/core.h"
#include "core.h"

/*-----------------------------------------------------------------------------
	NotePad++ colorizing :
vmath.vec4 vmath.dot vmath.cross vmath.length vmath.normalize
vmath.quat vmath.qrotaxis
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Vec4 methods :
-----------------------------------------------------------------------------*/

static int vec4_x		(lua_State *L) {	lua_pushnumber( L, LuaRequireVec4(L, 1, "self").v[0] );	return 1;	}
static int vec4_y		(lua_State *L) {	lua_pushnumber( L, LuaRequireVec4(L, 1, "self").v[1] );	return 1;	}
static int vec4_z		(lua_State *L) {	lua_pushnumber( L, LuaRequireVec4(L, 1, "self").v[2] );	return 1;	}
static int vec4_w		(lua_State *L) {	lua_pushnumber( L, LuaRequireVec4(L, 1, "self").v[3] );	return 1;	}
static int vec4_clone	(lua_State *L) {	LuaPushVec4( L, LuaRequireVec4(L, 1, "self") );			return 1;	}

static int vec4_tostr	(lua_State *L) {	
	EVec4	a = LuaRequireVec4(L, 1, "self");
	lua_pushstring( L, va("%g %g %g %g", a.x, a.y, a.z, a.w) );
	return 1;	
}

static int vec4_xyzw	(lua_State *L) {	
	EVec4	a = LuaRequireVec4(L, 1, "self");
	lua_pushnumber(L, a.x);
	lua_pushnumber(L, a.y);
	lua_pushnumber(L, a.z);
	lua_pushnumber(L, a.w);
	return 4;	
}

static int vec4__add	(lua_State *L) {	LuaPushVec4( L,  LuaRequireVec4(L, 1, "a") + LuaRequireVec4(L, 2, "b")  );	return 1;	}
static int vec4__sub	(lua_State *L) {	LuaPushVec4( L,  LuaRequireVec4(L, 1, "a") - LuaRequireVec4(L, 2, "b")  );	return 1;	}
static int vec4__mul	(lua_State *L) {	LuaPushVec4( L,  LuaRequireVec4(L, 1, "a") * LuaRequireVec4(L, 2, "b")  );	return 1;	}
static int vec4__unm	(lua_State *L) {	LuaPushVec4( L, -LuaRequireVec4(L, 1, "a")  );	return 1;	}

/*-----------------------------------------------------------------------------
	Quat methods :
-----------------------------------------------------------------------------*/

static int quat_x		(lua_State *L) {	lua_pushnumber( L, LuaRequireQuat(L, 1, "self").x );	return 1;	}
static int quat_y		(lua_State *L) {	lua_pushnumber( L, LuaRequireQuat(L, 1, "self").y );	return 1;	}
static int quat_z		(lua_State *L) {	lua_pushnumber( L, LuaRequireQuat(L, 1, "self").z );	return 1;	}
static int quat_w		(lua_State *L) {	lua_pushnumber( L, LuaRequireQuat(L, 1, "self").w );	return 1;	}
static int quat_clone	(lua_State *L) {	LuaPushQuat( L, LuaRequireQuat(L, 1, "self") );			return 1;	}

static int quat_tostr	(lua_State *L) {	
	EQuat	a = LuaRequireQuat(L, 1, "self");
	lua_pushstring( L, va("%g %g %g %g", a.x, a.y, a.z, a.w) );
	return 1;	
}

static int quat_xyzw	(lua_State *L) {	
	EQuat	a = LuaRequireQuat(L, 1, "self");
	lua_pushnumber(L, a.x);
	lua_pushnumber(L, a.y);
	lua_pushnumber(L, a.z);
	lua_pushnumber(L, a.w);
	return 4;	
}

static int quat__add	(lua_State *L) {	LuaPushQuat( L,  LuaRequireQuat(L, 1, "a") + LuaRequireQuat(L, 2, "b")  );	return 1;	}
static int quat__sub	(lua_State *L) {	LuaPushQuat( L,  LuaRequireQuat(L, 1, "a") - LuaRequireQuat(L, 2, "b")  );	return 1;	}
static int quat__mul	(lua_State *L) {	LuaPushQuat( L,  LuaRequireQuat(L, 1, "a") * LuaRequireQuat(L, 2, "b")  );	return 1;	}
static int quat__unm	(lua_State *L) {	LuaPushQuat( L, -LuaRequireQuat(L, 1, "a")  );	return 1;	}


/*-----------------------------------------------------------------------------
	VMath functions (vector):
-----------------------------------------------------------------------------*/

int vmath_vec4	(lua_State *L) {	
	int n = lua_gettop(L);
	if (n>=4) {	//	numeric constructor
		float x = LuaRequireNumber(L, 1, "x");
		float y = LuaRequireNumber(L, 2, "y");
		float z = LuaRequireNumber(L, 3, "z");
		float w = LuaRequireNumber(L, 4, "w");
		LuaPushVec4(L, EVec4(x,y,z,w));
		return 1;
	}
	if (n==1) {
		if (LuaIsVec4(L, 1)) {
			EVec4 v = LuaRequireVec4(L, 1, "other vec4");
			LuaPushVec4(L, v);
			return 1;
		}
	}
	luaL_error(L, "function requires four numeric or one vec4 arg");
	return 0;
}


static int vmath_dot	(lua_State *L) {		
	EVec4	a	=	LuaRequireVec4(L, 1, "a");
	EVec4	b	=	LuaRequireVec4(L, 2, "b");
	lua_pushnumber(L, Vec4Dot(a, b));
	return 1;
}


static int vmath_cross	(lua_State *L) {		
	EVec4	a	=	LuaRequireVec4(L, 1, "a");
	EVec4	b	=	LuaRequireVec4(L, 2, "b");
	if (a.w!=0 || b.w!=0) {
		luaL_error(L, "function require homogenious 4d vectors");
	}
	EVec3	a3	=	EVec3(a.x, a.y, a.z);
	EVec3	b3	=	EVec3(b.x, b.y, b.z);
	EVec3	xp	=	Vec3Cross(a3, b3);

	LuaPushVec4(L, EVec4(xp.x, xp.y, xp.z, 0));	
	return 1;
}


static int vmath_length	(lua_State *L) {		
	EVec4	a	=	LuaRequireVec4(L, 1, "a");
	lua_pushnumber(L, Vec4Length(a));
	return 1;
}


static int vmath_normalize	(lua_State *L) {		
	EVec4	a	=	LuaRequireVec4(L, 1, "a");
	LuaPushVec4(L, Vec4Normalize(a));
	return 1;
}


/*-----------------------------------------------------------------------------
	VMath functions (quaternion):
-----------------------------------------------------------------------------*/

int vmath_quat	(lua_State *L) 
{	
	int n = lua_gettop(L);
	if (n>=4) {	//	numeric constructor
		float x = LuaRequireNumber(L, 1, "x");
		float y = LuaRequireNumber(L, 2, "y");
		float z = LuaRequireNumber(L, 3, "z");
		float w = LuaRequireNumber(L, 4, "w");
		LuaPushQuat(L, EQuat(x,y,z,w));
		return 1;
	}
	if (n==1) {
		if (LuaIsQuat(L, 1)) {
			EQuat v = LuaRequireQuat(L, 1, "other quat");
			LuaPushQuat(L, v);
			return 1;
		}
	}
	luaL_error(L, "function requires four numeric or one quat arg");
	return 0;
}


int vmath_qrotaxis(lua_State *L)
{
	int n =	lua_gettop(L);
	//	given : angle + axis vector :
	if (n==2) {
		float	angle	=	LuaRequireNumber(L, 1, "angle");
		EVec4	axis4	=	LuaRequireVec4(L, 2,  "rotation axis vector");
		EVec3	axis	=	EVec3(axis4.x, axis4.y, axis4.z);
		LuaPushQuat( L, QuatRotationAxis(angle, axis) );
		return 1;
	}
	
	//	given : angle + x,y,z :
	if (n==4) {
		float	angle	=	LuaRequireNumber(L, 1, "angle");
		float	x		=	LuaRequireNumber(L, 2, "x-axis");
		float	y		=	LuaRequireNumber(L, 3, "y-axis");
		float	z		=	LuaRequireNumber(L, 4, "z-axis");
		EVec3	axis	=	EVec3(x,y,z);
		LuaPushQuat( L, QuatRotationAxis(angle, axis) );
		return 1;
	}
	
	luaL_error(L, "function usage : qrotaxis(x, y, z, angle) or qrotaxis(vec4, angle)");
	return 0;
}


/*-----------------------------------------------------------------------------
	Lua vector math :
-----------------------------------------------------------------------------*/

const char *VMATH_VEC4	=	"VMATH_VEC4";
const char *VMATH_QUAT	=	"VMATH_QUAT";

static const luaL_Reg vmath_lib[] = {
	{	"vec4"		,	vmath_vec4		},
	{	"dot"		,	vmath_dot		},
	{	"cross"		,	vmath_cross		},
	{	"length"	,	vmath_length	},
	{	"normalize"	,	vmath_normalize	},

	{	"quat"		,	vmath_quat		},
	{	"qrotaxis"	,	vmath_qrotaxis	},
	{	NULL,		NULL	}
};

static const luaL_Reg vec4_lib[] = {
	{	"x"		,	vec4_x		},
	{	"y"		,	vec4_y		},
	{	"z"		,	vec4_z		},
	{	"w"		,	vec4_w		},
	{	"xyzw"	,	vec4_xyzw	},
	{	"clone"	,	vec4_clone	},
	{	"tostr"	,	vec4_tostr	},
	{NULL, NULL}
};

static const luaL_Reg quat_lib[] = {
	{	"x"		,	quat_x		},
	{	"y"		,	quat_y		},
	{	"z"		,	quat_z		},
	{	"w"		,	quat_w		},
	{	"xyzw"	,	quat_xyzw	},
	{	"clone"	,	quat_clone	},
	{	"tostr"	,	quat_tostr	},
	{NULL, NULL}
};


//
//	LuaOpenVMath
//
void LuaOpenVMath( lua_State *L )
{
	//
	//	register vec4 methods:
	//
	luaL_newmetatable(L, VMATH_VEC4);	// create metatable

	lua_pushcfunction(L, vec4__add);   	lua_setfield(L, -2, "__add");
	lua_pushcfunction(L, vec4__sub);   	lua_setfield(L, -2, "__sub");
	lua_pushcfunction(L, vec4__unm);   	lua_setfield(L, -2, "__unm");
	lua_pushcfunction(L, vec4__mul);   	lua_setfield(L, -2, "__mul");
	
	lua_pushvalue(L, -1);				// push metatable
	lua_setfield(L, -2, "__index");		// metatable.__index = metatable
	luaL_register(L, NULL, vec4_lib);	// methods

	//
	//	register quaternion methods:
	//
	luaL_newmetatable(L, VMATH_QUAT);	// create metatable

	lua_pushcfunction(L, quat__add);   	lua_setfield(L, -2, "__add");
	lua_pushcfunction(L, quat__sub);   	lua_setfield(L, -2, "__sub");
	lua_pushcfunction(L, quat__unm);   	lua_setfield(L, -2, "__unm");
	lua_pushcfunction(L, quat__mul);   	lua_setfield(L, -2, "__mul");
	
	lua_pushvalue(L, -1);				// push metatable
	lua_setfield(L, -2, "__index");		// metatable.__index = metatable
	luaL_register(L, NULL, quat_lib);	// methods

	//
	//	register 'vmath' namespace:
	//
	luaL_register(L, "vmath", vmath_lib);
}


//
//	LuaPushVec4
//
void LuaPushVec4( lua_State *L, const EVec4 &v )
{
	EVec4 *pv = (EVec4*)lua_newuserdata(L, sizeof(EVec4));
	*pv = v;
	
	luaL_getmetatable(L, VMATH_VEC4);
	lua_setmetatable(L, -2);
}


//
//	LuaIsVec4
//
bool LuaIsVec4( lua_State *L, int idx )
{
	return LuaIsUData(L, idx, VMATH_VEC4);	
}


//
//	LuaToVec4
//
EVec4 LuaToVec4( lua_State *L, int idx )
{
	EVec4 *pv = (EVec4*)luaL_checkudata(L, idx, VMATH_VEC4);
	ASSERT(pv);
	return *pv;
}


//
//	LuaRequireVec4
//
EVec4 LuaRequireVec4( lua_State *L, int idx, const char *what )
{
	if (!LuaIsVec4(L, idx)) {
		LuaArgumentError(L, idx, "vec4", what);
	}
	return LuaToVec4(L, idx);
}


/*-----------------------------------------------------------------------------
	Quaternion stuff :
-----------------------------------------------------------------------------*/

void LuaPushQuat( lua_State *L, const EQuat &q )
{
	EQuat *pq = (EQuat*)lua_newuserdata(L, sizeof(EQuat));
	*pq = q;
	
	luaL_getmetatable(L, VMATH_QUAT);
	lua_setmetatable(L, -2);
}


bool LuaIsQuat( lua_State *L, int idx )
{
	return LuaIsUData(L, idx, VMATH_QUAT);	
}


EQuat LuaToQuat( lua_State *L, int idx )
{
	EQuat *pq = (EQuat*)luaL_checkudata(L, idx, VMATH_QUAT);
	ASSERT(pq);
	return *pq;
}


EQuat LuaRequireQuat( lua_State *L, int idx, const char *what )
{
	if (!LuaIsQuat(L, idx)) {
		LuaArgumentError(L, idx, "quat", what);
	}
	return LuaToQuat(L, idx);
}

