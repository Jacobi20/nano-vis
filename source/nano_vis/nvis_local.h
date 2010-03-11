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

#include "../core/core.h"

#ifdef _DEBUG
#	define D3D_DEBUG_INFO
#endif

#define D3D_DEBUG_INFO

#include <d3d9.h>
#include <d3dx9.h>

#define	WIN32_LEAN_AND_MEAN
#define	VC_EXTRALEAN
#include <windows.h>

#pragma comment (lib, "d3dxof.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "winmm.lib")


#define CHECK_CRITICAL(expr) if (FAILED(expr)) { SYS_Error(ERR_FATAL, va("%s() : %s failed", __FUNCTION__, #expr)); }

#define SAFE_RELEASE(obj) if (obj) { obj->Release(); obj = NULL; }

//
//	local headers :
//
#include "nvis.h"