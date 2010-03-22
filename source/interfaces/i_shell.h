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
// i_shell.h - shell interface


#pragma once

/*-----------------------------------------------------------------------------
	Shell
-----------------------------------------------------------------------------*/

typedef void (*EShellCmd_f)( void *self, uint argc, const char **argv );
typedef lua_CFunction		ELuaCFunc_f;

//class	IVar;
//typedef hard_ref<IVar>	IPxVar;


/*-----------------------------------------------------------------------------
	Helper functions and macros :
-----------------------------------------------------------------------------*/

#define LUA_ERROR(s)	Linker()->GetShell()->LuaError(s)
#define LUA_WARNING(s)	Linker()->GetShell()->LuaWarning(s)




/*-----------------------------------------------------------------------------
	Interfaces itself :
-----------------------------------------------------------------------------*/

class	IShell : public IDisposable {
	public:
		virtual				~IShell					( void ) {}
		
		virtual void		AddCommand				( const char *name, EShellCmd_f cmd_func, void *self ) = 0;
		virtual void		RemoveCommand			( const char *name ) = 0;
		virtual void		DefineCFunc				( const char *name, ELuaCFunc_f lua_cfunc ) = 0;
		
		virtual void		ExecuteString			( const char *cmd_text,  bool raise_error=false ) = 0;
		virtual void		ExecuteScript			( const char *file_name, bool raise_error=false ) = 0;
		
		virtual const char	*CompleteCommand		( const char *text ) = 0;

		virtual lua_State	*Lua					( void ) = 0;
		
		virtual int			LuaWarning				( const char *text ) = 0;
		virtual int			LuaError				( const char *text ) = 0;
	};
	
