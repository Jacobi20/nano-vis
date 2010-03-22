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

// i_game.h - 


#pragma once


/*-----------------------------------------------------------------------------
	Game
-----------------------------------------------------------------------------*/

class IGEvent;

typedef hard_ref<IGEvent>	IPxGEvent;


class IGEvent : public IDisposable {	
	public:
		virtual void	SetObject	( const char *name, uint obj_id ) = 0;
		virtual void	SetParam	( const char *name, const char *value ) = 0;
		virtual void	SetParam	( const char *name, const EVec4 &value ) = 0;
		virtual void	SetParam	( const char *name, float value ) = 0;
		virtual void	SetParam	( const char *name, int value ) = 0;
	};
	

class IGame : public IDisposable { 
	public:
		virtual IPxGEvent	CreateEvent		( void ) = 0;
		virtual	void		Frame			( uint dtime ) = 0;
	};
	
