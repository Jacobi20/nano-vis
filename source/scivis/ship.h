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
	Nano vis :
-----------------------------------------------------------------------------*/

class	IWaving;
class	IShip;
typedef hard_ref<IWaving>	IPxWaving;
typedef hard_ref<IShip>		IPxShip;

typedef IWaving	*	(*CreateWaving_f)	( lua_State *L, int idx );
typedef IShip	*	(*CreateShip_f)		( lua_State *L, int idx );

struct ERendEnv_s {
		EMatrix4	matrix_view;
		EMatrix4	matrix_proj;
		EVec4		view_pos;
	};


class IWaving : public ICoreObject {
	public:
		virtual void		Update			( float dtime ) = 0;
		virtual void		ReloadShader	( void ) = 0;
		virtual EVec4		GetVelocity		( const EVec4 &init_pos ) const = 0;
		virtual EVec4		GetPosition		( const EVec4 &init_pos ) const = 0;
		virtual float		GetWaveSlopeX	( const EVec4 &init_pos ) const = 0;
	};
	
	
class IShip : public ICoreObject {
	public:
		virtual void		Simulate				( float dtime, IPxWaving waving ) = 0;
		virtual void		ReloadShader			( void ) = 0;
		virtual void		GetPose					( EVec4 &position, EQuat &orient ) = 0;
		virtual void		Render					( ERendEnv_s *rend_env ) = 0;
		virtual void		ApplyForceAtLocalPoint	( const EVec4 &pos, const EVec4 &force ) = 0;
	};