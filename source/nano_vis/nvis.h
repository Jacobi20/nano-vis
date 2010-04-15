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

using namespace OpenBabel;

struct  EVidState_s	{
		bool	fullscr;
		int		mode;
		HWND	h_wnd;
	};


struct EAtomBall_s {
		float			vdw_radius;
		D3DXVECTOR4		color;
		D3DXVECTOR4		position;
	};
	
struct EAtomStick_s {
		D3DXVECTOR4		color;
		D3DXVECTOR4		pos0, pos1;
	};	
	

class ECachedMol;
typedef hard_ref<ECachedMol> EPxCachedMol;

class ECachedMol : public IDisposable {
	public:
		ECachedMol	( void ) {
			volume = NULL;
		}
		~ECachedMol	( void ) {
			SAFE_RELEASE( volume );
		}
		OBMol		mol;
		EName		name;
		OBGridData	*grid;
		IDirect3DVolumeTexture9	*volume;
	protected:
	};



class ENanoVis : public INanoVis {
	public:
							ENanoVis			( void );
							~ENanoVis			( void );
												
		virtual void			RenderFrame			( uint dtime );
		virtual void			RenderSnapshot		( const char *command );
		virtual EPxCachedMol	LoadData			( const char *path );
		
		virtual void		RenderShot			( lua_State *L );
		
		void				GetScreenSize		( uint &w, uint &h );
		void				*GetWndDescriptor	( void );
		ID3DXEffect			*CompileEffect		( const char *path );
		
		static	int			NVisSnapshot		( lua_State *L );

	public:
		IDirect3D9			*d3d;
		IDirect3DDevice9	*d3ddev;
		EVidState_s			vid_state;
		IPxShell			shell;
		
	protected:
		void		InitAtomRend		( void );
		void		ShutdownAtomRend	( void );
		void		LoadVolumeData		( const OBGridData *grid, IDirect3DVolumeTexture9 **vol );
		void		RenderVolume		( OBGridData *grid,  IDirect3DVolumeTexture9 **vol, D3DXMATRIX &w, D3DXMATRIX &v, D3DXMATRIX &p, D3DXVECTOR4 &view_point, int slice_num, float intens_scale );
		ID3DXEffect				*atom_fx;
		ID3DXEffect				*vol_fx;
		ID3DXMesh				*mesh_ball;
		ID3DXMesh				*mesh_stick;
		
		vector<EAtomBall_s>		balls;
		vector<EAtomStick_s>	sticks;
		
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
		
		vector<EPxCachedMol>	cached_mols;

	static void		ListDisplayModes_f	( ENanoVis *self, int argc, char **argv );
	
	static LRESULT	WndProc				( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
};
