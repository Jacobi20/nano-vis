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


IShell *CreateShell( void )
{
	return new EShell();
}


#define LOAD_LUA_LIB(lualib)	do {		\
		lua_pushcfunction(L, lualib);		\
		lua_call(L, 0, 0);					\
	} while (0);


/*-----------------------------------------------------------------------------
	Shell
-----------------------------------------------------------------------------*/

//
//	EShell::EShell
//
EShell::EShell( void )
{
	LOG_INIT("Shell");
	L	=	NULL;
	
	L	=	lua_open();
	
	if (!L) {
		LOG_FATAL("Lua state was not created");
	}
	
	lua_atpanic(L, AtPanic);
	
	LOAD_LUA_LIB(luaopen_base);
	LOAD_LUA_LIB(luaopen_debug);
	LOAD_LUA_LIB(luaopen_io);
	LOAD_LUA_LIB(luaopen_math);
	LOAD_LUA_LIB(luaopen_string);
	LOAD_LUA_LIB(luaopen_table);
	LOAD_LUA_LIB(luaopen_os);
	LOAD_LUA_LIB(luaopen_package);
	
	LuaOpenVMath(L);
	
	DefineCFunc("print", Print);
}


//
//	EShell::~EShell
//
EShell::~EShell( void )
{
	lua_close(L);

	LOG_SHUTDOWN("Shell");
}


/*-----------------------------------------------------------------------------
	Function stuff :
-----------------------------------------------------------------------------*/

//
//	EShell::DefineCFunc
//
void EShell::DefineCFunc( const char *name, ELuaCFunc_f lua_cfunc )
{
	if (lua_cfunc) {
		lua_register( L, name, lua_cfunc );
	} else {
		ExecuteString(va("%s = nil;\r\n", name), false);
	}
}


//
//	EShell::AtPanic
//
int EShell::AtPanic( lua_State *L )
{
	char buffer[256];
	strcpy(buffer, lua_tostring(L, 1));
	RAISE_EXCEPTION(va("Lua panic: %s", buffer));
	return 0;
}


//
//	EShell::Print
//
int EShell::Print( lua_State *L )
{
	int n = lua_gettop(L);	// num args 
	
	string str;
	
	for (int i=1; i<n+1; i++) {
		const char *s = lua_tostring(L, i);
		
		if (!s) {
			if ( lua_isnil(L, i) )		{ s = "nil";		}
			if ( lua_istable(L, i) )	{ s = "{...}";		}
			if ( lua_isuserdata(L, i) )	{ s = va("0x%08X", lua_touserdata(L, 1));		}
			if (!s) {
				s = "<!string>";
			}
		}
		
		str += s;
		str += " ";
	}
	
	LOG_DIM("> %s", str.c_str());
	
	return 0;
}



//
//	EShell::AddCommand
//	
void EShell::AddCommand( const char *name, EShellCmd_f cmd_func, void *self )
{
	lua_pushlightuserdata(L, cmd_func);		
	lua_pushlightuserdata(L, self);
	lua_pushlightuserdata(L, const_cast<char*>(name));
	lua_pushcclosure(L, CmdWrapper, 3);
	lua_setglobal(L, name);
}


//
//	EShell::CmdWrapper
//
int EShell::CmdWrapper( lua_State *L )
{
	void *pcmd  = lua_touserdata(L, lua_upvalueindex(1));
	void *self	= lua_touserdata(L, lua_upvalueindex(2));
	void *name	= lua_touserdata(L, lua_upvalueindex(3));
	
	ASSERT(name);
	ASSERT(pcmd);
	
	EShellCmd_f	cmd	=	(EShellCmd_f)(pcmd);
	EArgs		args;
	
	args.NewArg((char*)name);
	
	int	n	=	lua_gettop(L);
	
	for (int i=1; i<n+1; i++) {
		const char *s = lua_tostring(L, i);
		
		if (!s) {
			lua_pushstring(L, va("CmdWrapper() : argument #%d cann't be converted to string", n));
			lua_error(L);
		}
		
		args.NewArg(s);
	}
	
	cmd(self, args.Argc(), args.Argv());
	
	return 0;
}



//
//	EShell::RemoveCommand
//
void EShell::RemoveCommand( const char *name )
{
	ExecuteString(va("%s = nil;\r\n", name), false);
}


/*-----------------------------------------------------------------------------
	Execing stuff :
-----------------------------------------------------------------------------*/

//
//	EShell::ExecuteString
//
void EShell::ExecuteString( const char *cmd_text, bool raise_error )
{
	try {
		int r = luaL_loadstring(L, cmd_text);

		if (r!=0) {
			char buffer[256];
			strcpy(buffer, lua_tostring(L, -1));
			lua_pop(L, 1);
			RAISE_EXCEPTION(buffer);
		}
		
		int err = lua_pcall(L, 0, LUA_MULTRET, 0);
		
		if (err!=0) {
			RAISE_EXCEPTION(va("Lua: %s", lua_tostring(L, -1)));
		}
		
	} catch (exception &e) {
		if (raise_error) {
			throw;
		}
		LOG_ERROR("%s", e.what());
	}
}


//
//	EShell::ExecuteScript
//
void EShell::ExecuteScript( const char *file_name, bool raise_error )
{
	LOGF("compiling : %s\r\n", file_name);
	
	try {
		int r = luaL_loadfile(L, file_name);

		if (r!=0) {
			char buffer[256];
			strcpy(buffer, lua_tostring(L, -1));
			lua_pop(L, 1);
			RAISE_EXCEPTION(buffer);
		}
		
		int err = lua_pcall(L, 0, LUA_MULTRET, 0);

		if (err!=0) {
			RAISE_EXCEPTION(va("Lua: %s", lua_tostring(L, -1)));
		}

	} catch (exception &e) {
		if (raise_error) {
			throw;
		}
		LOG_ERROR("%s", e.what());
	}
}


//
//	EShell::CompleteCommand
//
const char *EShell::CompleteCommand( const char *part )
{
	static	char	common[1024];
	strcpy(common, "");
	uint matched = 0;

	//	
	//	traverse the globals' table :
	//
	lua_pushnil(L);
	
	while (lua_next(L, LUA_GLOBALSINDEX)) {
	
		//	_G's table key is not a string, skip this :		
		if (!lua_isstring(L, -2)) {
			lua_pop(L, 1);
			continue;
		}
		
		const char	*key	=	lua_tostring(L, -2);
		int			ttype	=	lua_type(L, -1);
		const char	*type	=	lua_typename(L, ttype);
		const char	*value	=	lua_tostring(L, -1);
		
		if (ttype==LUA_TTABLE) {	
			value	=	"{ ... }";
		}

		if (_strnicmp(part, key, strlen(part))==0) {
			LOGF("  %-25s %-10s = %s", key, type, value);
			
			if (common[0]=='\0') {
				strcpy(common, key);
			}
			
			common_parti(common, key);
			matched++;
		}
		
		lua_pop(L, 1);
	}

	if (matched==0) {
		return part;
	}
					
	LOGF("");
	
	if (matched==1)	{
		strcat(common, " ");
	}
	
	return common;
}

