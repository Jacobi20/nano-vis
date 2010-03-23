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


#include "nvis_run.h"

/*-----------------------------------------------------------------------------
	NVis runner :
-----------------------------------------------------------------------------*/

void PrintCB ( void *s ) 
{
	ELogMessage_s	msg;
	Log()->GetLogLastMessage( msg );
	printf("%s\r\n", msg.message);
}


//
//	NVisInit
//
DLL_EXPORT void NVisInit( void )
{
	Log()->SetWriteCB(PrintCB, NULL);

	InitCoreSubsystems();
	
	Linker()->GetConfig()->LoadConfig();
	
	Linker()->LinkDLLNanoVis("nano_vis.dll");
	Linker()->GetNanoVis()->LoadData("test01.cube");
}


//
//	NVisShutdown
//
DLL_EXPORT void NVisShutdown( void )
{
	Linker()->LinkNanoVis(NULL);
	
	Linker()->GetConfig()->SaveConfig();

	ShutdownCoreSubsystems();
}


//
//	NVisFrame
//
DLL_EXPORT void NVisFrame( uint dtime )
{
	IPxNanoVis	nvis	=	Linker()->GetNanoVis();
	nvis->RenderFrame( dtime );
}


//
//
//
DLL_EXPORT	void NVisSnapshot( float distance, float yaw, float pitch, float roll )
{
	IPxNanoVis	nvis	=	Linker()->GetNanoVis();
	nvis->RenderSnapshot(distance, yaw, pitch, roll); 
}


BOOL WINAPI ConsoleHandleRoutine(DWORD dwCtrlType)
{
	if (dwCtrlType==CTRL_CLOSE_EVENT) {
		printf("\r\n");
		printf("**** IMMEDIATE TERMINATION ****\r\n");
		printf("\r\n");
		
		TerminateProcess(0, 0);
		return TRUE;
	}
	
	return FALSE;
}



//
//	main
//
int main(int argc, const char **argv)
{
	uint	old_time	=	System()->Milliseconds();
	uint	time		=	System()->Milliseconds();
	MSG		msg = {0};

	SetConsoleCtrlHandler( ConsoleHandleRoutine, TRUE );

	try {
	
		NVisInit();

		while ( WM_QUIT != msg.message )
		{
			if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )		
			{			
				TranslateMessage( &msg );		
				DispatchMessage( &msg );				
			}		
			else			
			{
				time = System()->Milliseconds();
				
				NVisFrame(time - old_time);
				
				old_time = time;
			}

		}

		NVisShutdown();
		
	} catch (exception &e) {
		FATAL(e.what());
	}
	
}