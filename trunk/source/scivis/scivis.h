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

struct  EVidState_s	{
		bool	fullscr;
		int		mode;
		HWND	h_wnd;
	};
	


class ESciVis : public ISciVis {
	public:
								ESciVis			( void );
								~ESciVis			( void );

		virtual void	*		GetWindowDescriptor	( void ) { return vid_state.h_wnd; }
		virtual void			RenderFrame			( uint dtime );
		virtual void			RenderSnapshot		( const char *command );
		
		void					GetScreenSize		( uint &w, uint &h );
		ID3DXEffect				*CompileEffect		( const char *path );
		
		void					InitRender			( void );
		void					ShutdownRender		( void );
		void					RenderView			( lua_State * L );
		static int				SCI_RenderView		( lua_State * L );
		
	public:
		IDirect3D9			*d3d;
		IDirect3DDevice9	*d3ddev;
		EVidState_s			vid_state;
		IPxShell			shell;
		
	protected:
		
		
	protected:
		void		InitDisplay			( void );
		void		InitDirect3D		( void );
		void		ShutdownDisplay		( void );
		void		ShutdownDirect3D	( void );		
		void		CheckDisplayModes	( void );
		uint		GetModeNum			( void );
		void		GetModeDesc			( uint nmode, EVidModeDesc_s *mode_desc );
		uint		GetCurrentMode		( void );
		
		IPxVar		d3d_mode;
		IPxVar		d3d_fullscr;
		IPxVar		d3d_xpos;
		IPxVar		d3d_ypos;
		
	static void		ListDisplayModes_f	( ESciVis *self, int argc, char **argv );
	static LRESULT	WndProc				( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	};
