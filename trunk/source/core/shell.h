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
// shell.h - 


#pragma once


/*-----------------------------------------------------------------------------
	Shell
-----------------------------------------------------------------------------*/

//class	EVar : public IVar {
//	public:
//							EVar			( class EShell *shell, const char *name, const char *value );
//		virtual				~EVar			( void );
//		
//		virtual const char	*String			( void ) const;
//		virtual void		SetValue		( const char *val );
//
//		virtual int			Int				( void ) const { return atoi( String() ); }
//		virtual float		Float			( void ) const { return atof( String() ); }
//		virtual uint		Flags			( void ) const { return 0; }
//		
//		EName				Name			( void ) const { return name; }
//		
//	private:
//		mutable string		value;
//		lua_State	*L;
//		EShell		*shell;
//		EName		name;
//		uint		flags;
//	};	


class	EShell : public IShell {
	public:			
							EShell					( void );
							~EShell					( void );
					
		virtual void		AddCommand				( const char *name, EShellCmd_f cmd_func, void *self );
		virtual void		RemoveCommand			( const char *name );
		virtual void		DefineCFunc				( const char *name, ELuaCFunc_f lua_cfunc );
		
		virtual void		ExecuteString			( const char *cmd_text,  bool raise_error );
		virtual void		ExecuteScript			( const char *file_name, bool raise_error );

		virtual const char	*CompleteCommand		( const char *text );
		
		virtual lua_State	*Lua					( void ) { return L; }

		virtual int			LuaWarning				( const char *text );
		virtual int			LuaError				( const char *text );
		//virtual int			LuaRequireArg			( int arg, int type, const char *what );
		
	private:
		lua_State	*L;
		
		static int			AtPanic		( lua_State *L );
		static int			Print		( lua_State *L );
		static int			CmdWrapper	( lua_State *L );
		
		vector<EName>		cmd_names;
	};	
	

