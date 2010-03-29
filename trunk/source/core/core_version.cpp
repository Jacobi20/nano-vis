/*
	The MIT License

	Copyright (c) 2008 IFMO/GameDev Studio

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
//	version.tmpl - template for 'subwcrev.exe'
//	Do not checkin produced 'version.cpp', it would cause local modification
//	Also, do not edit 'version.cpp', editing will cause no effect.
//	Edit 'version.tmpl' instead.

#include "../core/core.h"

#define PROJECT_NAME	"NanoVis"
#define VERSION			"1.00"

#ifdef _DEBUG
#define BUILD_CONFIG	"debug"
#else
#define BUILD_CONFIG	"release"
#endif

#define BUILD_DATE		__DATE__
#define SVN_REVISION 	"42"
#define SVN_DATE     	"2010/03/23 18:44:13"

#define PLATFORM_NAME	"win-x86"

DLL_EXPORT const char *Version(void)
{
	const char *version = PROJECT_NAME" "VERSION"."SVN_REVISION" "PLATFORM_NAME" "BUILD_CONFIG" "BUILD_DATE;
	return version;	
}

