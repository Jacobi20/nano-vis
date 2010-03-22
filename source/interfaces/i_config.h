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
// i_disposable.h - 


#pragma once

/*-----------------------------------------------------------------------------
	IConfig - configurating facility
-----------------------------------------------------------------------------*/

class IVar;
typedef hard_ref<IVar>	IPxVar;

const uint CVAR_USER		=	0x0001;		//	will be saved to user config


#define CONFIG_REGISTER_VAR(var, value)	do {					\
		var	=	Linker()->GetConfig()->RegisterVar(#var, 0);	\
		if (var->Nil()) var->Set(value);						\
	} while (0)


/*-----------------------------------------------------------------------------
	Interfaces :
-----------------------------------------------------------------------------*/

class IVar : public IDisposable {
	public:
		virtual void		Set		( bool value ) = 0;
		virtual void		Set		( int value ) = 0;
		virtual void		Set		( uint value ) { Set((int)value); }
		virtual void		Set		( float value ) = 0;
		virtual void		Set		( const char *value ) = 0;

		virtual bool		Nil		( void ) const = 0;	
		virtual bool		Bool	( void ) const = 0;
		virtual int			Int		( void ) const = 0;
		virtual float		Float	( void ) const = 0;
		virtual const char *String	( void ) const = 0;
		
		virtual EName		Name	( void ) const = 0;
		virtual uint		Flags	( void ) const = 0;
	};


class IConfig : public IDisposable {
	public:
		virtual void		SetUser			( const char *username ) = 0;
		virtual const char	*GetUser		( void ) const = 0;
		virtual IPxVar		RegisterVar		( const char *name, uint flag ) = 0;
		virtual void		LoadConfig		( void ) = 0;
		virtual void		SaveConfig		( void ) = 0;
	};