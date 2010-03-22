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

#include <crtdbg.h>

#include "library.h"
#ifdef WIN32

#include <windows.h>
#endif
/*-----------------------------------------------------------------------------
	ASSERT / VERIFY
-----------------------------------------------------------------------------*/

//
//	Lib_Assertion()
//
void Lib_Assertion(const char *expr, const char *func, const char *file, uint line)
{
	char text[1024];
	sprintf(text,
		"Assertion failed\r\n"
		"expr :\t%s\r\n"
		"func :\t%s()\r\n"
		"file :\t%s\r\n"
		"line :\t%d\r\n"
		"\r\n"
		"Press Retry to debug the application\r\n",
		expr, func, file, line);
		
	int res = MessageBox(0, text, "Assertion Failed", MB_ABORTRETRYIGNORE | MB_DEFBUTTON3 | MB_ICONWARNING );
	
	if (res==IDIGNORE)	return;
	if (res==IDRETRY)	DebugBreak();
	if (res==IDABORT)	ExitProcess(0x3);
}


//
//	Lib_Signal
//
void Lib_Signal(const char *msg, const char *func, const char *file, uint line)
{
	char text[1024];
	sprintf(text,
		"Signal\r\n"
		"msg  :\t%s\r\n"
		"func :\t%s()\r\n"
		"file :\t%s\r\n"
		"line :\t%d\r\n"
		"\r\n"
		"Press Retry to debug the application\r\n",
		msg, func, file, line);
		
	int res = MessageBox(0, text, "Signal", MB_ABORTRETRYIGNORE | MB_DEFBUTTON3 | MB_ICONWARNING );
	
	if (res==IDIGNORE)	return;
	if (res==IDRETRY)	DebugBreak();
	if (res==IDABORT)	TerminateProcess(GetCurrentProcess(), 0);
}


//
//	Lib_Fatal
//
void Lib_Fatal(const char *msg, const char *func, const char *file, uint line)
{
	char text[1024];
	
	sprintf(text, 
		"FATAL ERROR:\r\n\r\n"
		"%s\r\n\r\n"
		"func :\t%s()\r\n"
		"file :\t%s\r\n"
		"line :\t%d\r\n",
		msg, func, file, line);
		
	int res = MessageBox(0, text, "Fatal Error", MB_OK | MB_ICONERROR );
	
	TerminateProcess(GetCurrentProcess(), 0);
}


//
//	Lib_Fatal
//
void Lib_LogException(const char *msg, const char *func, const char *file, uint line)
{
	#ifdef _DEBUG
	#endif
}




