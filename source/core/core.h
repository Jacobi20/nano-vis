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
	Core global definitions. This file must be included to all projects
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    Check and tune compiler options :
-----------------------------------------------------------------------------*/

#pragma warning (disable : 4800)	//	'int' : forcing value to bool 'true' or 'false' (performance warning)
#pragma warning (disable : 4996)	//	'function' : was declared deprecated
#pragma warning (disable : 4244)	//	'variable' : conversion from 'double' to 'float', possible loss of data
#pragma warning (disable : 4251)	//	'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'


#ifdef WIN32 
	#ifndef _CPPRTTI
		#error  Run-time type information is disabled!
	#endif

	#ifndef _CPPUNWIND
		#error  Exception handling is disabled!
	#endif
#endif


/*-----------------------------------------------------------------------------
    Libs
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
    General macros
-----------------------------------------------------------------------------*/

#undef DLL_EXPORT
#undef DLL_IMPOR
#ifdef _WIN32 
    #define DLL_EXPORT  extern "C"  __declspec(dllexport)
    #define DLL_IMPORT  extern "C"  __declspec(dllimport)
#else
    #define DLL_EXPORT
    #define DLL_IMPORT
#endif


#ifdef STATIC_CORE
	#define CORE_API
#else
	#ifdef CORE_EXPORTS
		#define CORE_API  __declspec(dllexport)
	#else
		#define CORE_API  __declspec(dllimport)
	#endif
#endif

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#define VC_EXTRALEAN
	#include <windows.h>

	#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
	#endif
	
	#define OS_MAX_PATH MAX_PATH

	#include "win_key_list.h"
#endif


/*-----------------------------------------------------------------------------
    Library headers :
-----------------------------------------------------------------------------*/

//	library :
#include "../library/library.h"

//	tiny xml :
#include "../etc/tinyxml/tinyxml.h"

//	rapid xml :
#include "../etc/rapidxml/rapidxml.hpp"
#include "../etc/rapidxml/rapidxml_ext.hpp"
using namespace rapidxml;

//	Lua :
extern "C" {
    #include "../etc/lua/src/lua.h"
    #include "../etc/lua/src/lualib.h"
    #include "../etc/lua/src/lauxlib.h"
}

/*-----------------------------------------------------------------------------
    Interfaces :
-----------------------------------------------------------------------------*/
#ifndef NO_CORE_INTERFACES
#include "../interfaces/interfaces.h"
#endif

#include "../core/lua_ext.h"
#include "../core/lua_vmath.h"

/*-----------------------------------------------------------------------------
    Console macros :
-----------------------------------------------------------------------------*/

#define LOGF(frmt, ...)			Log()->LogMessage(LOG_MSG_INFO,		__FILE__,__LINE__, frmt, __VA_ARGS__)
#define LOG_WARNING(frmt, ...)	Log()->LogMessage(LOG_MSG_WARNING,	__FILE__,__LINE__, frmt, __VA_ARGS__)
#define LOG_ERROR(frmt, ...)	Log()->LogMessage(LOG_MSG_ERROR,	__FILE__,__LINE__, frmt, __VA_ARGS__)
#define LOG_FATAL(frmt, ...)	Log()->LogMessage(LOG_MSG_FATAL,	__FILE__,__LINE__, frmt, __VA_ARGS__)
#define LOG_DEBUG(frmt, ...)	Log()->LogMessage(LOG_MSG_DEBUG,	__FILE__,__LINE__, frmt, __VA_ARGS__)
#define LOG_DIM(frmt, ...)		Log()->LogMessage(LOG_MSG_DIMMED,	__FILE__,__LINE__, frmt, __VA_ARGS__)

#define LOG_INIT(s)				Log()->LogMessage(LOG_MSG_INFO,		__FILE__,__LINE__, "INIT : %s", s)
#define LOG_SHUTDOWN(s)			Log()->LogMessage(LOG_MSG_INFO,		__FILE__,__LINE__, "SHUTDOWN : %s", s)
#define LOG_SPLIT(headline)		Log()->LogSplit(headline);

/*-----------------------------------------------------------------------------
    OS specific dependencies :
-----------------------------------------------------------------------------*/


