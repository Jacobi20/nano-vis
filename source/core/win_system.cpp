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
// win_system.cpp - windows system stuff


#include "../core/core.h"
#include <mmsystem.h>
#include "win_system.h"
#pragma comment (lib, "winmm.lib")


/*-----------------------------------------------------------------------------
	EXPORT STUFF
-----------------------------------------------------------------------------*/

EWinSystem	system_imp_static;

DLL_EXPORT ISystem *System(void)
{
	return &system_imp_static;
}


/*-----------------------------------------------------------------------------
	EWinSystem stuff :
-----------------------------------------------------------------------------*/

//
//	XTSystem::XTSystem
//
EWinSystem::EWinSystem(void)
{
	//	change current dir to  :
	char path[2048];
	GetModuleFileName(GetModuleHandle(NULL), path, 2047);
	
	for (uint i = strlen(path)-1; i>0; i--) {
		if (path[i]=='\\') {
			path[i] = '\0';
			break;
		}
	}
	
	VERIFY( SetCurrentDirectory(path) );
	
	//	init timer :
	timeBeginPeriod(1);
	init_msec	=	timeGetTime();
}


//
//	EWinSystem::~EWinSystem
//
EWinSystem::~EWinSystem(void)
{
	timeEndPeriod(1);
}



/*-----------------------------------------------------------------------------
	Program arguments
-----------------------------------------------------------------------------*/

const char *EWinSystem::GetCmdLine(void)
{
	return GetCommandLine();
}


/*-----------------------------------------------------------------------------
	Time
-----------------------------------------------------------------------------*/

uint EWinSystem::Milliseconds(void)
{
	return timeGetTime() - init_msec;
}


systemTime_t EWinSystem::GetGMTTime(void)
{
	systemTime_t	time;
	SYSTEMTIME	systime;
	
	::GetSystemTime(&systime);

	time.day			=	systime.wDay;	
	time.day_of_week	=	systime.wDayOfWeek;	
	time.hour			=	systime.wHour;	
	time.minute			=	systime.wMinute;	
	time.month			=	systime.wMonth;	
	time.msec			=	systime.wMilliseconds;	
	time.sec			=	systime.wSecond;	
	time.year			=	systime.wYear;	
	
	return time;
}


systemTime_t EWinSystem::GetLocalTime(void)
{
	systemTime_t	time;
	SYSTEMTIME	systime;
	
	::GetLocalTime(&systime);
	
	time.day			=	systime.wDay;	
	time.day_of_week	=	systime.wDayOfWeek;	
	time.hour			=	systime.wHour;	
	time.minute			=	systime.wMinute;	
	time.month			=	systime.wMonth;	
	time.msec			=	systime.wMilliseconds;	
	time.sec			=	systime.wSecond;	
	time.year			=	systime.wYear;	
	
	return time;
}



/*-----------------------------------------------------------------------------
	Process control
-----------------------------------------------------------------------------*/

void EWinSystem::Quit(uint code)
{
	PostQuitMessage(code);
}


void EWinSystem::Terminate(uint code)
{
	ExitProcess(code);
}



/*-----------------------------------------------------------------------------
	DLL Stuff :
-----------------------------------------------------------------------------*/

EWinDLL::EWinDLL(const char *dll_name)
{
	name	=	dll_name;
	LOGF("Loading DLL : %s", dll_name);
	h_dll	=	LoadLibrary(dll_name);
	if (!h_dll) {
		RAISE_EXCEPTION(va("DLL \"%s\" has not been loaded", name.c_str()));
	}
}

EWinDLL::~EWinDLL(void)
{
	if (h_dll) {
		LOGF("Unloading DLL : %s", name.c_str());
		FreeLibrary(h_dll);
	}
}


void *EWinDLL::GetProcAddr(const char *func_name)
{
	void *proc	=	GetProcAddress(h_dll, func_name);
	if (!proc) {
		RAISE_EXCEPTION(va("Function '%s' has not been found in '%s'", func_name, name.c_str()));
	}
	return proc;
}


IPxDLL EWinSystem::LoadDLL(const char *dll_name)
{
	IPxDLL	px_dll = new EWinDLL(dll_name);
	return px_dll;
}


/*-----------------------------------------------------------------------------
	Thread Stuff 
-----------------------------------------------------------------------------*/

thread_t	EWinSystem::SpawnThread(threadFunc_f func, void *param)
{
	HANDLE	h_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)func, param, 0, NULL);
	ASSERT(h_thread);
	
	return (thread_t)h_thread;
}

void EWinSystem::KillThread(thread_t thread)
{
	HANDLE	h_thread = (HANDLE)thread;
	VERIFY(TerminateThread(h_thread, 0));
	VERIFY(CloseHandle(h_thread));
}

void EWinSystem::Sleep(uint msec)
{
	::Sleep(msec);
}













