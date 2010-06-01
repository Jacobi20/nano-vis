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


#include "sci_local.h"

/*-----------------------------------------------------------------------------
	Init stuff :
-----------------------------------------------------------------------------*/

#define	VID_WNDCLASSNAME	"D3D Display"
#define	VID_WNDNAME			Version()
#define WS_WINDOW			( WS_CLIPCHILDREN | WS_CLIPSIBLINGS )
#define WS_FULLSCR			( WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP )
#define WBORDER				( 2*GetSystemMetrics(SM_CXFIXEDFRAME) )
#define HBORDER				( 2*GetSystemMetrics(SM_CYFIXEDFRAME)+GetSystemMetrics(SM_CYCAPTION) )

EVidModeDesc_s mode_list[] = {
		//{	640		,	480		,	false	},	//	VGA and MCGA
		//{	800		,	480		,	false	},	//	WGA or WVGA, ASUS Eee PC 700 and 701 series
		{	800		,	600		,	false	},	//	SVGA
		{	1024	,	576		,	false	},	//	WXGA
		{	1024	,	768		,	false	},	//	XGA
		{	1280	,	800		,	false	},	//	WXGA
		{	1280	,	960		,	false	},	//	SXVGA, Quad VGA
		{	1280	,	1024	,	false	},	//	SXGA
		{	1366	,	768		,	false	},	//	WXGA
		{	1440	,	900		,	false	},	//	WXGA+
		{	1400	,	1050	,	false	},	//	SXGA+
		{	1600	,	1024	,	false	},	//	WSXGA
		{	1600	,	1200	,	false	},	//	UXGA
		{	1680	,	1050	,	false	},	//	WSXGA+
		{	1920	,	1200	,	false	},	//	WUXGA
		{	1920	,	1400	,	false	},	//	TXGA
		{	2048	,	1536	,	false	},	//	QXGA
		{	2560	,	1600	,	false	},	//	WQXGA
	};
	
const uint MODE_LIST_SIZE	= (sizeof(mode_list) / sizeof(EVidModeDesc_s));


//
//	ESciVis::CheckDisplayModes
//
void ESciVis::CheckDisplayModes(void)
{
	for (uint i=0; i<MODE_LIST_SIZE; i++)
	{
		DEVMODE	dm;
		memset(&dm, 0, sizeof(dm));
		dm.dmSize		=	sizeof(dm);
		dm.dmPelsWidth	=	mode_list[i].width;
		dm.dmPelsHeight	=	mode_list[i].height;
		dm.dmFields		=	DM_PELSWIDTH | DM_PELSHEIGHT;
		
		int result = ChangeDisplaySettings(&dm, CDS_TEST);
		
		if ( result == DISP_CHANGE_SUCCESSFUL ) {	
			mode_list[i].is_supported	=	true;
		}  else {
			mode_list[i].is_supported	=	false;
		}
	}
}


//
//	ESciVis::ListDisplayModes_f
//
void ESciVis::ListDisplayModes_f(ESciVis *self, int argc, char **argv)
{
	for (uint i=0; i<MODE_LIST_SIZE; i++) {
		const char *cursor = "  ";
		if (i==self->GetCurrentMode()) {
			cursor = "->";
		}
		if (mode_list[i].is_supported) {
			LOGF("%s%2d : %5d %5d : OK\r\n", cursor, i, mode_list[i].width, mode_list[i].height);
		} else {
			LOGF("%s%2d : %5d %5d : X\r\n", cursor, i, mode_list[i].width, mode_list[i].height);
		}
	}
}


//
//	ESciVis::InitDisplay
//
void ESciVis::InitDisplay(void)
{
	CheckDisplayModes();
	
	CONFIG_REGISTER_VAR(  d3d_fullscr	,	false	);
	CONFIG_REGISTER_VAR(  d3d_mode		,	0		);
	CONFIG_REGISTER_VAR(  d3d_xpos		,	0		);
	CONFIG_REGISTER_VAR(  d3d_ypos		,	0		);
	
	//
	//	CDS :
	//
_try_again:	
	vid_state.fullscr	=	d3d_fullscr->Bool();
	vid_state.mode	=	Clamp<uint>(d3d_mode->Int(), 0, MODE_LIST_SIZE-1);
	vid_state.h_wnd	=	NULL;

	d3d_mode->Set( vid_state.mode );
	
	if (vid_state.fullscr) {
		LOGF("Selected mode : %dx%d FS\r\n", mode_list[vid_state.mode].width, mode_list[vid_state.mode].height);
		
		// selected mode is not supported :
		if (!mode_list[vid_state.mode].is_supported) {
			LOGF("Mode is not supported, searching for appropriate mode...\r\n");
			
			// try to find good mode :
			uint i;
			for (i=0; i<MODE_LIST_SIZE; i++) {
				if (mode_list[i].is_supported) {
					d3d_mode->Set(i);
					goto _try_again;
				}
			}

			// good mode not found (settings should be reseted) :			
			if (i==MODE_LIST_SIZE) {
				LOGF("Appropriate mode not found\r\n");
				d3d_fullscr->Set(false);
				d3d_mode->Set(0);
				goto _try_again;
			}
		}
		
		DEVMODE	dm;
		memset(&dm, 0, sizeof(dm));
		dm.dmSize		=	sizeof(dm);
		dm.dmPelsWidth	=	mode_list[vid_state.mode].width;
		dm.dmPelsHeight	=	mode_list[vid_state.mode].height;
		dm.dmFields		=	DM_PELSWIDTH | DM_PELSHEIGHT;
		
		LOGF("Setting fullscreen mode");
		int result	= ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
		
		if (result!=DISP_CHANGE_SUCCESSFUL) {
			d3d_fullscr->Set(false);
			d3d_mode->Set(0);
			goto _try_again;
			LOG_WARNING("current display mode is not supported in fullscreen");
			LOG_WARNING("switching to windowed mode");
		} else {
		}
		
	} else 
	{
		LOGF("Selected mode : %dx%d W\r\n", mode_list[vid_state.mode].width, mode_list[vid_state.mode].height);
	}
	
	//
	//	Create window :
	//
	//------------------------------------
	//	Register Class :
	WNDCLASSEX	wc;
		memset(&wc, 0, sizeof(WNDCLASSEX));
		wc.cbSize			= sizeof (WNDCLASSEX);
		wc.lpfnWndProc		= (WNDPROC)WndProc;	
		wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.hInstance		= GetModuleHandle(NULL);
		wc.hbrBackground	= CreateSolidBrush(RGB(0,0,0));
		wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
		wc.lpszClassName	= VID_WNDCLASSNAME;
		wc.hIcon			= 0;//LoadIcon(GetModuleHandle("rend_d3d9.dll"), MAKEINTRESOURCE(IDI_ICON1));
		//	TODO DHG : set icon for created window.
		
	if (!RegisterClassEx(&wc)) {
		FATAL("RegisterClassEx() failed");
	}
		

	//------------------------------------
	//	Create Window :
	int w = mode_list[vid_state.mode].width;
	int h = mode_list[vid_state.mode].height;
	int pos[2];
	pos[0] = d3d_xpos->Int();
	pos[1] = d3d_ypos->Int();
	
	if (vid_state.fullscr) {
		vid_state.h_wnd = CreateWindow(VID_WNDCLASSNAME, VID_WNDNAME, WS_FULLSCR, 
							0,0, w,h, 
							NULL, NULL, GetModuleHandle(NULL), 0);
	} else {
		vid_state.h_wnd = CreateWindow(VID_WNDCLASSNAME, VID_WNDNAME, WS_WINDOW, 
							pos[0], pos[1],
							w + WBORDER, h + HBORDER, 
							NULL, NULL, GetModuleHandle(NULL), 0);
	}
							
	if (!vid_state.h_wnd ) { 
		FATAL("CreateWindow() failed");
	}
	
	ShowWindow( vid_state.h_wnd , SW_NORMAL);
	SetForegroundWindow( vid_state.h_wnd );
	SetFocus( vid_state.h_wnd );
	UpdateWindow( vid_state.h_wnd );
}


//
//	ESciVis::ShutdownDisplay
//
void ESciVis::ShutdownDisplay(void)
{
	if (!vid_state.fullscr) {
		RECT rect;
		GetWindowRect( vid_state.h_wnd, &rect );
		d3d_xpos->Set( rect.left );
		d3d_ypos->Set( rect.top );
	}

	if ( vid_state.h_wnd )
		if (!DestroyWindow( vid_state.h_wnd ))
			RAISE_EXCEPTION("DestroyWindow() failed");

	if (!UnregisterClass(VID_WNDCLASSNAME, GetModuleHandle(0)))
		RAISE_EXCEPTION("UnregisterClass() failed");
		
	if (vid_state.fullscr) {
		ChangeDisplaySettings(NULL, NULL);
	}

}


//
//	ESciVis::GetScreenSize
//
void ESciVis::GetScreenSize(uint &w, uint &h)
{
	w	=	mode_list[vid_state.mode].width;
	h	=	mode_list[vid_state.mode].height;
}


//
//	ESciVis::WndProc
//
LRESULT	ESciVis::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch( uMsg ) { 
		case WM_MOVE :		break;
		case WM_CREATE :	break;
		case WM_DESTROY :	break;
		case WM_KEYDOWN :	break;
		case WM_CHAR :		break;
		case WM_CLOSE :	 
					PostQuitMessage(0);
					return 0;
				break;
		default :	return DefWindowProc( hWnd, uMsg, wParam, lParam ) ;
	} 
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


/*-----------------------------------------------------------------------------
	Direct3D stuff :
-----------------------------------------------------------------------------*/

//
//	ESciVis::InitDirect3D
//
void ESciVis::InitDirect3D(void)
{
	LOGF("D3D_SDK_VERSION : 0x%X\r\n", D3D_SDK_VERSION);
	
	LOGF("Creating Direct3D");
	d3d		=	Direct3DCreate9(D3D_SDK_VERSION);
	
	if (!d3d) {
		LOG_FATAL("Direct3DCreate9() failed");
	} else {
	}
	
	//
	//	Setup default present parameters :
	//
    D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed					=	TRUE;
	d3dpp.SwapEffect				=	D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil	=	TRUE;
	d3dpp.AutoDepthStencilFormat	=	D3DFMT_D24S8;
	d3dpp.BackBufferFormat			=	D3DFMT_A8R8G8B8;
	d3dpp.MultiSampleType			=	D3DMULTISAMPLE_4_SAMPLES;

#define PERF_HUD
#ifdef PERF_HUD	
	uint		adapter			=	D3DADAPTER_DEFAULT; 
	D3DDEVTYPE	devtype			=	D3DDEVTYPE_HAL;
	dword		behavior		=	D3DCREATE_HARDWARE_VERTEXPROCESSING;
	d3dpp.PresentationInterval	=	D3DPRESENT_INTERVAL_ONE;
	d3dpp.SwapEffect			=	D3DSWAPEFFECT_DISCARD;

	for (uint i=0; i<d3d->GetAdapterCount(); i++) {
		D3DADAPTER_IDENTIFIER9	driver_id;
		d3d->GetAdapterIdentifier(i, 0, &driver_id);
		
		LOGF("Adapter # %2d : %s %s\r\n", i, driver_id.Description, driver_id.DeviceName );
		
		if (strstr(driver_id.Description, "PerfHUD")!=0) {
			LOGF("Using PerfHUD...\r\n");
			d3dpp.PresentationInterval	=	D3DPRESENT_INTERVAL_IMMEDIATE;
			adapter	=	i;
			devtype	=	D3DDEVTYPE_REF;
			break;
		}
	}
#else
	uint		adapter			=	D3DADAPTER_DEFAULT;
	D3DDEVTYPE	devtype			=	D3DDEVTYPE_HAL;
	dword		behavior		=	D3DCREATE_HARDWARE_VERTEXPROCESSING;
	d3dpp.PresentationInterval	=	D3DPRESENT_INTERVAL_ONE;
#endif	
		
	
	//
	//	Create D3D Device :
	//
	LOGF("Creating Direct3D Device");
    HRESULT res = d3d->CreateDevice( 
				adapter, devtype, 
				vid_state.h_wnd, behavior,
				&d3dpp,	 &d3ddev
            );

	if (FAILED(res)) {
		LOG_FATAL("CreateDevice() failed");
	}
}


//
//	ESciVis::ShutdownDirect3D
//
void ESciVis::ShutdownDirect3D(void)
{
	LOGF("Releasing Direct3D Device\r\n");
	SAFE_RELEASE(d3ddev);
	
	LOGF("Releasing Direct3D\r\n");
	SAFE_RELEASE(d3d);
}


/*-----------------------------------------------------------------------------
	Additional stuff :
-----------------------------------------------------------------------------*/

uint ESciVis::GetModeNum ( void )
{
	return MODE_LIST_SIZE;
}


void ESciVis::GetModeDesc( uint nmode, EVidModeDesc_s *mode_desc )
{
	memcpy(mode_desc, &mode_list[nmode], sizeof(EVidModeDesc_s));
}


uint ESciVis::GetCurrentMode( void )
{
	return vid_state.mode;
}

