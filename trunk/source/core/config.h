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

#pragma once


/*-----------------------------------------------------------------------------
	Config facility
-----------------------------------------------------------------------------*/

class EVar : public IVar {
	public:
							EVar	( const char *name, uint flags );
							~EVar	( void );
	
		virtual void		Set		( bool value );
		virtual void		Set		( int value );
		virtual void		Set		( float value );
		virtual void		Set		( const char *value );
	
		virtual bool		Nil		( void ) const;
		virtual bool		Bool	( void ) const;
		virtual int			Int		( void ) const;
		virtual float		Float	( void ) const;
		virtual const char *String	( void ) const;
		
		virtual EName		Name	( void ) const { return name; }
		virtual uint		Flags	( void ) const { return flags; }
		
	protected:
		EName	name;
		uint	flags;
		lua_State	*L;
		mutable string strval;
	};


class EConfig : public IConfig {	
	public:
							EConfig			( void );
							~EConfig		( void );
		
		virtual void		SetUser			( const char *username );
		virtual const char	*GetUser		( void ) const;
		virtual IPxVar		RegisterVar		( const char *name, uint flag );
		virtual void		LoadConfig		( void );
		virtual void		SaveConfig		( void );
	protected:
		lua_State	*L;
	
		static int			RegVar			( lua_State *L );
		static int			SetVar			( lua_State *L );
		static int			SaveConfig		( lua_State *L );
		static int			LoadConfig		( lua_State *L );
	
		vector<IPxVar>	vars;
	};

