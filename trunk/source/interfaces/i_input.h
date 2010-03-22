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
// i_input.h - 


#pragma once


struct	mouseInput_s {
		int		dx;
		int		dy;
		int		x;
		int		y;
		bool	left;
		bool	right;
		bool	middle;
	};


enum EInputMode_t {
		IN_MOUSE_CLIP		=	0x0001,		//	clip mouse against window border
		IN_MOUSE_HIDE		=	0x0002,		//	hide mouse cursor
		IN_MOUSE_RELATIVE	=	0x0004,		//	move mouse back to the center of the window on each ProcessInput()
		IN_KB_SCAN			=	0x0008,		//	skan keys (call bound commands)
		IN_KB_CHAR			=	0x0010,		//	enqueue chars and key (like WM_CHAR)
	};


class IInputSystem : public IDisposable { 
	public:
		virtual void		SetTargetWindow		( void *hwnd ) = 0;				//	Sets target window 
		
		virtual void		ProcessInput		( void ) = 0;					//	Should be called on each frame
		virtual void		GetMouseInput		( mouseInput_s &mi ) = 0;		//	Gets mouse input: x, y, dx, dy, buttons etc...
		virtual char		GetKbChar			( void ) = 0;					//	Gets enqueued character (if no ones, returns zero)
		virtual char		GetKbKey			( void ) = 0;					//	Gets enqueued pressed key (if no ones, returns zero)
		
		virtual	void		SaveKeyBindings		( IPxFile file ) = 0;			//	Appends bindings to end of the spcified config
		virtual	uint		GetKeyByCmd			( const char *cmd ) = 0;		//	Returns key code
		virtual void		SetInputMode		( uint flags ) = 0;				//	Sets input mode
	};