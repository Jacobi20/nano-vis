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


IConfig *CreateConfig( void )
{
	return new EConfig();
}

/*-----------------------------------------------------------------------------
	Config facility
-----------------------------------------------------------------------------*/

//
//	EConfig::EConfig
//
EConfig::EConfig( void )
{
	LOG_INIT("Configuration system");
	
	L	=	Linker()->GetShell()->Lua();

	static const luaL_Reg cfg_api[] = {
		{	"RegisterVar",		EConfig::RegVar			},
		{	"SetVar",			EConfig::SetVar			},
		{	"SaveConfig",		EConfig::SaveConfig		},
		{	"LoadConfig",		EConfig::LoadConfig		},
		{	NULL,			NULL	}
	};

	luaL_register(L, "cfg", cfg_api);
	lua_pop(L, -1);
}


//
//	EConfig::~EConfig
//
EConfig::~EConfig( void )
{
	LOG_SHUTDOWN("Configuration system");
}


//
//	EConfig::SetUser
//
void EConfig::SetUser( const char *username )
{

}


//
//	EConfig::GetUser
//
const char	* EConfig::GetUser( void ) const
{
	return "Player";
}


//
//	EConfig::RegisterVar
//
IPxVar EConfig::RegisterVar( const char *name, uint flags)
{
	EName n = name;

	for (uint i=0; i<vars.size(); i++) {
		if (vars[i]->Name()==n) {
			return vars[i];
		}
	}

	IPxVar	var	=	new EVar(name, flags);
	vars.push_back(var);
	
	return var;
}


//
//	EConfig::LoadConfig
//
void EConfig::LoadConfig( void )
{
	LOGF("Loading configuration...");

	Linker()->GetShell()->ExecuteScript("default.lua");
	Linker()->GetShell()->ExecuteScript("config.lua");
}


//
//	EConfig::SaveConfig
//
void EConfig::SaveConfig( void )
{
	IPxFileSystem	fs	=	Linker()->GetFileSystem();
	
	try {
		IPxFile			fce	=	fs->FileOpen("config.lua", FS_OPEN_WRITE);

		LOGF("Saving configuration...");
		
		//
		//	write engine settings :
		//	
		fce->Printf("-- engine settings --\r\n");	
		fce->Printf("-- warning: syntax errors will cause configuration resetting --\r\n");
		fce->Printf("\r\n");	
		
		for (uint i=0; i<vars.size(); i++) {
			
			lua_getglobal(L, vars[i]->Name().Name());
			
			string	value	= "nil";
			string	name	= va("'%s'", vars[i]->Name().Name());
			
			if (lua_isboolean(L, -1)) {
				if (lua_toboolean(L, -1)) value	=	"true"; else value = "false";
			}

			if (lua_type(L, -1)==LUA_TNUMBER) {
				value = va("%g", lua_tonumber(L, -1));
			}

			if (lua_type(L, -1)==LUA_TSTRING) {
				value = va("'%s'", lua_tostring(L, -1));
			}
		
			fce->Printf("cfg.SetVar(%-29s, %s);\r\n", name.c_str(), value.c_str());
		}

		fce->Printf("\r\n");	

		//
		//	write key bindings :	
		//
		IPxInputSystem	is	=	 Linker()->GetInputSystem();
		
		if (is) {
			is->SaveKeyBindings( fce );
		}
	} catch (exception &e) {
		LOG_WARNING("failed to save configuration: %s", e.what());
	}
}


/*-----------------------------------------------------------------------------
	Lua API for config :
-----------------------------------------------------------------------------*/

int EConfig::RegVar( lua_State *L )
{
	EConfig	*cfg	=	Linker()->GetConfig().To<EConfig>();
	
	const char *name	=	LuaRequireString(L, 1, "cvar name");
	
	IPxVar	var	=	cfg->RegisterVar(name, 0);

	if (var->Nil()) {	
		lua_pushvalue(L, 2);
		lua_setglobal(L, name);
	}
	
	return 0;
}


int EConfig::SetVar( lua_State *L )
{
	EConfig	*cfg	=	Linker()->GetConfig().To<EConfig>();
	
	const char *name	=	LuaRequireString(L, 1, "cvar name");
	
	IPxVar	var	=	cfg->RegisterVar(name, 0);

	lua_pushvalue(L, 2);
	lua_setglobal(L, name);
	
	return 0;
}


int EConfig::SaveConfig( lua_State *L )
{
	EConfig	*cfg	=	Linker()->GetConfig().To<EConfig>();
	
	cfg->SaveConfig();
	
	return 0;
}


int EConfig::LoadConfig( lua_State *L )
{
	EConfig	*cfg	=	Linker()->GetConfig().To<EConfig>();

	cfg->LoadConfig();
	
	return 0;
}

